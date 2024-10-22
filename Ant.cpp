#include "Ant.hpp"

namespace da 
{

#pragma region Ant
	Ant::Ant
	(
		sf::RenderWindow* window,
		uint16_t threadIndex,
		sf::Color* ColorTransitionArray,
		uint32_t Width,
		uint32_t Height,
		std::string& antPath
	)
		:m_pMesh(new Mesh(Width, Height, window))
		, m_pColorTransitionArray(ColorTransitionArray)
		, m_x(0)
		, m_y(0)
		, m_Width(Width)
		, m_Height(Height)
		, m_Facing(0U)
		, m_NextTurn(0U)
		, m_NextCheck(0)
		, m_DistanceToYWall(0)
		, m_DistanceToXWall(0)
		, m_ThreadID(threadIndex)
		, m_pCurrentAntColor(nullptr)
	{
		if (m_pColorTransitionArray == nullptr) return;

		SetOffset(m_pMesh->GetCenterPoint());

		m_pMesh->InitFieldColor(daTypes::Color( m_pColorTransitionArray[0] ));
		m_pMesh->SetFilePrefix(antPath);
	}

	Ant::~Ant()
	{
		delete m_pMesh;
	}

	bool Ant::NextMove(uint64_t repetitions)
	{
		for (size_t i = 0; i < repetitions; i++)
		{
			m_pCurrentAntColor = m_pMesh->GetColor(m_x, m_y);

			m_NextTurn = constants::TURN_MASK & m_pCurrentAntColor->a;

			m_pMesh->SetColor(m_x, m_y, m_pColorTransitionArray[constants::COLOR_INDEX_MASK & m_pCurrentAntColor->a]);

			switch (m_NextTurn)
			{
			case constants::LEFT:	MoveLeft();		break;
			case constants::RIGHT:	MoveRight();	break;
			}

			if (m_NextCheck <= 0)
			{
				if (m_x == m_Width - 1)		return false;
				else if (m_x == 0)			return false;


				if (m_y == m_Height - 1)	return false;
				else if (m_y == 0)			return false;

				m_DistanceToYWall = m_x < m_Width - 1 - m_x ? m_x : m_Width - 1 - m_x;
				m_DistanceToXWall = m_y < m_Height - 1 - m_y ? m_y : m_Height - 1 - m_y;

				m_NextCheck = m_DistanceToYWall < m_DistanceToXWall ? m_DistanceToYWall : m_DistanceToXWall; // shortest stright line to mesh border
				m_NextCheck *= 2;// The shortest path is always diagonal so on the grid this mean 2x more steps than stright line
				++m_NextCheck;
			}
			--m_NextCheck;
		}
		return true;
	}

	void Ant::SetOffset(uint32_t x, uint32_t y)
	{
		m_x = x;
		m_y = y;
	}

	void Ant::SetOffset(daTypes::PointUI32 p)
	{
		m_x = p.x;
		m_y = p.y;
	}

	void Ant::DumpToFile(const std::string& outputPath)
	{
		m_pMesh->DumpToFile(outputPath);
	}

	void Ant::DrawMesh()
	{
		m_pMesh->DrawMesh();
	}

#pragma endregion

#pragma region MegaAnt

	MegaAnt::MegaAnt(uint64_t* loopEnd
		, uint8_t threadIndex
		, uint32_t Width
		, uint32_t Height
		, std::string& antPath
		, daTypes::GreenColor* DaGreenColorTransitionArray
		, uint8_t* ColorMaskedTransitionArray
		, uint16_t ColorMaskedCount)

		:Ant(nullptr, threadIndex, nullptr, Width, Height, antPath)
		, m_pMegaMesh(new MegaMesh(Width, Height, DaGreenColorTransitionArray))
		, m_CurrentAntColorMaskedCount(ColorMaskedCount)
		, m_pColorMaskedTransitionArray(ColorMaskedTransitionArray)
		, m_CurrentAntColorMasked(0U)

	{
		SetOffset(m_pMegaMesh->GetCenterPoint());

		for (size_t i = 0U; i < m_CurrentAntColorMaskedCount; i++)
		{
			m_pColorMaskedTransitionArray[i] = constants::TURN_MASK & DaGreenColorTransitionArray[i].a;
		}

		m_pMegaMesh->SetFilePrefix(antPath);
		m_pMegaMesh->InitFieldColor(daTypes::GreenColor{ 0U, m_pColorMaskedTransitionArray[0] } );

		m_pMeshFieldCopy = m_pMegaMesh->GetFieldPtr();
	}

	MegaAnt::~MegaAnt()
	{
		delete m_pMegaMesh;
	}

	void MegaAnt::DumpToFile(const std::string& outputPath)
	{
		m_pMegaMesh->DumpToFile( outputPath );
	}

#pragma endregion

}