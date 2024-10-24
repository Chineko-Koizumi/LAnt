#include "Ant.hpp"

namespace da 
{

#pragma region Ant
	Ant::Ant
	(
		sf::RenderWindow* window,
		daTypes::GreenColor* pDaGreenColorTransitionArray,
		uint32_t Width,
		uint32_t Height,
		const std::string& rAntPath
	) 
		: AntBase( pDaGreenColorTransitionArray, Width, Height, rAntPath )
		, m_pMesh( new Mesh(Width, Height, window) )
	{
		SetOffset(m_pMesh->GetCenterPoint());

		m_pMesh->InitField( pDaGreenColorTransitionArray[0].a );
		m_pMesh->SetFilePrefix(rAntPath);
		
		m_pMeshFieldCopy = m_pMesh->GetFieldPtr();
	}

	Ant::~Ant()
	{
		delete m_pMesh;
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

	MegaAnt::MegaAnt(
		  uint32_t Width
		, uint32_t Height
		, std::string& antPath
		, daTypes::GreenColor* DaGreenColorTransitionArray
		, uint8_t* ColorMaskedTransitionArray
		, uint16_t ColorMaskedCount)

		:Ant(nullptr, nullptr, Width, Height, antPath)
		, m_pMegaMesh(new MeshMega(Width, Height, DaGreenColorTransitionArray))
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
		m_pMegaMesh->InitField( m_pColorMaskedTransitionArray[0] );

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