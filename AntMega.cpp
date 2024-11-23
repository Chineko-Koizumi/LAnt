
#include "AntMega.hpp"

namespace da 
{
	AntMega::AntMega(
		uint32_t Width,
		uint32_t Height,
		std::string& antPath,
		daTypes::GreenColor* pEncodedColorArray)
			: AntBase( Width, Height, antPath, pEncodedColorArray)
			, m_pMegaMesh(new MeshMega(Width, Height, pEncodedColorArray))
	{
		SetOffset(m_pMegaMesh->GetCenterPoint());

		m_pMegaMesh->SetFilePrefix(antPath);
		m_pMegaMesh->InitField(m_pTurnsFromColor[0]);

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