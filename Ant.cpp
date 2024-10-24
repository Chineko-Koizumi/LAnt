#include "Ant.hpp"

namespace da 
{

#pragma region Ant
	Ant::Ant
	(
		sf::RenderWindow* window,
		uint16_t threadIndex,
		daTypes::GreenColor* DaGreenColorTransitionArray,
		uint32_t Width,
		uint32_t Height,
		std::string& antPath
	)
		:m_pMesh(new Mesh(Width, Height, window))
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
		, m_pMeshFieldCopy(nullptr)
	{

		SetOffset(m_pMesh->GetCenterPoint());

		m_pMesh->InitFieldColor( daTypes::Color(DaGreenColorTransitionArray[0]) );
		m_pMesh->SetFilePrefix(antPath);
		
		m_pDaGreenColorTransitionArray = DaGreenColorTransitionArray;

		m_CurrentAntColorMaskedCount = antPath.size();
		m_pColorMaskedTransitionArray = new uint8_t[m_CurrentAntColorMaskedCount];

		for (size_t i = 0U; i < m_CurrentAntColorMaskedCount; i++)
		{
			m_pColorMaskedTransitionArray[i] = constants::TURN_MASK & DaGreenColorTransitionArray[i].a;
		}

		m_pMeshFieldCopy = m_pMesh->GetFieldPtr();
	}

	Ant::~Ant()
	{
		delete m_pMesh;
		delete m_pColorMaskedTransitionArray;
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