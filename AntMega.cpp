
#include "AntMega.hpp"

namespace da 
{
	AntMega::AntMega(
		  uint32_t Width
		, uint32_t Height
		, std::string& antPath
		, daTypes::GreenColor* DaGreenColorTransitionArray
		, uint16_t ColorMaskedCount)
			: AntBase(DaGreenColorTransitionArray, Width, Height, antPath)
			, m_pMegaMesh(new MeshMega(Width, Height, DaGreenColorTransitionArray))
	{
		SetOffset(m_pMegaMesh->GetCenterPoint());

		for (size_t i = 0U; i < m_CurrentAntColorMaskedCount; i++)
		{
			m_pColorMaskedTransitionArray[i] = daConstants::TURN_MASK & DaGreenColorTransitionArray[i].a;
		}

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