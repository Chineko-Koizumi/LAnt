#include "Ant.hpp"

namespace da 
{
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

	void Ant::DumpToFile(const std::string& outputPath)
	{
		m_pMesh->DumpToFile(outputPath);
	}

	void Ant::DrawMesh()
	{
		m_pMesh->DrawMesh();
	}
}