#include "Ant.hpp"

namespace da 
{
	Ant::Ant
	(
		sf::RenderWindow* window,
		uint32_t Width,
		uint32_t Height,
		const std::string& rAntPath
	) 
		: AntBase( Width, Height, rAntPath )
		, m_pMesh( new Mesh(Width, Height, window) )
	{
		SetOffset(m_pMesh->GetCenterPoint());

		if (m_pDaGreenColorTransitionArray == nullptr) return; //path was allready generated

		m_pMesh->InitField( m_pDaGreenColorTransitionArray[0].a );
		m_pMesh->SetFilePrefix(rAntPath);
		
		m_pMeshFieldCopy = m_pMesh->GetFieldPtr();
	}

	Ant::~Ant()
	{
		delete m_pMesh;
	}

	void Ant::DumpToFile(const std::string& outputPath)
	{
		m_pMesh->DumpToFile(outputPath, m_pDaGreenColorTransitionArray);
	}

	void Ant::DrawMesh(bool clearScreen, bool pushToScreen)
	{
		m_pMesh->DrawMesh(clearScreen, pushToScreen);
	}
}