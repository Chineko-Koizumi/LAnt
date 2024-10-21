#include "Ant.hpp"


#pragma region Ant
da::Ant::Ant
(
	sf::RenderWindow* window,
	uint16_t threadIndex,
	sf::Color* ColorTransitionArray,
	uint32_t Width,
	uint32_t Height,
	std::string& antPath
)
	:m_pMesh(new da::Mesh(Width, Height, window))
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
	m_pMesh->InitFieldColor(*m_pColorTransitionArray);
	m_pMesh->SetFilePrefix(antPath);
}

da::Ant::~Ant()
{
	delete m_pMesh;
}

bool da::Ant::NextMove(uint64_t repetitions)
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
			if (m_x > m_Width - 1)	return false;
			else if (m_x < 0)		return false;


			if (m_y > m_Height - 1)	return false;
			else if (m_y < 0)		return false;

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

void da::Ant::SetOffset(uint32_t x, uint32_t y)
{
	m_x = x;
	m_y = y;
}

void da::Ant::SetOffset(da::PointUI32 p)
{
	m_x = p.x;
	m_y = p.y;
}

void da::Ant::DumpToFile()
{
	m_pMesh->DumpToFile();
}

void da::Ant::DrawMesh()
{
	m_pMesh->DrawMesh();
}

#pragma endregion

#pragma region MegaAnt

da::MegaAnt::MegaAnt(uint64_t* loopEnd
	, uint8_t threadIndex
	, uint32_t Width
	, uint32_t Height
	, std::string& antPath
	, da::GreenColor* DaGreenColorTransitionArray
	, uint8_t* ColorMaskedTransitionArray
	, uint16_t ColorMaskedCount)

	:Ant(nullptr, threadIndex, nullptr, Width, Height, antPath)
	, m_MegaMesh(MegaMesh(Width, Height))
	, m_CurrentAntColorMaskedCount(ColorMaskedCount)
	, m_pDaGreenColorTransitionArray(DaGreenColorTransitionArray)
	, m_pColorMaskedTransitionArray(ColorMaskedTransitionArray)
	, m_CurrentAntColorMasked(0U)

{
	SetOffset(m_MegaMesh.GetCenterPoint());

	for (size_t i = 0; i < m_CurrentAntColorMaskedCount; i++)
	{
		m_pColorMaskedTransitionArray[i] = constants::TURN_MASK & m_pDaGreenColorTransitionArray[i].a;
	}

	m_MegaMesh.SetFilePrefix(antPath);
	m_MegaMesh.InitFieldColor(m_pColorMaskedTransitionArray[0]);

	m_pMeshFieldCopy = m_MegaMesh.GetFieldPtr();
}

da::MegaAnt::~MegaAnt()
{
}

void da::MegaAnt::DumpToFile()
{
	m_MegaMesh.DumpToFileBig(m_pDaGreenColorTransitionArray);
}

#pragma endregion