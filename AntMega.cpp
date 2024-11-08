
#include "AntMega.hpp"

namespace da 
{
	AntMega::AntMega(
		  uint32_t Width
		, uint32_t Height
		, std::string& antPath)
			: AntBase( Width, Height, antPath)
			, m_pMegaMesh(new MeshMega(Width, Height, m_pDaGreenColorTransitionArray))
	{
		SetOffset(m_pMegaMesh->GetCenterPoint());

		m_pMegaMesh->SetFilePrefix(antPath);
		m_pMegaMesh->InitField(m_pColorMaskedTransitionArray[0]);

		m_pMeshFieldCopy = m_pMegaMesh->GetFieldPtr();
	}

	AntMega::~AntMega()
	{
		delete m_pMegaMesh;
	}

	void AntMega::DumpToFile(const std::string& outputPath)
	{
		m_pMegaMesh->DumpToFile(outputPath);
	}
}