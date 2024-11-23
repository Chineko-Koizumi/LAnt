#include "AntBase.hpp"
#include "InputParser.hpp"

namespace da 
{
	da::AntBase::AntBase(
		uint32_t Width,
		uint32_t Height,
		const std::string& rAntPath,
		daTypes::GreenColor* pEncodedColorArray)
					: m_pMeshFieldCopy(nullptr)
					, m_CurrentAntColorMaskedCount( rAntPath.size() )
					, m_pColorMaskedTransitionArray( new uint8_t[m_CurrentAntColorMaskedCount] )
					, m_CurrentAntColorMasked(0U)
					, m_CurrentTurn(0U)
					, m_pDaGreenColorTransitionArray( pEncodedColorArray )
					, m_x(0)
					, m_y(0)
					, m_Width(Width)
					, m_Height(Height)
					, m_Facing(0U)
					, m_NextTurn(0U)
					, m_DistanceToYWall(0)
					, m_DistanceToXWall(0)
					, m_NextCheck(0)
	{
		for (size_t i = 0U; i < m_CurrentAntColorMaskedCount; i++)
		{
			m_pColorMaskedTransitionArray[i] = daConstants::TURN_MASK & m_pDaGreenColorTransitionArray[i].a;
		}
	}

	da::AntBase::~AntBase()
	{
		delete[] m_pColorMaskedTransitionArray;
	}

	void AntBase::SetOffset(uint64_t x, uint64_t y)
	{
		m_x = x;
		m_y = y;
	}

	void AntBase::SetOffset(daTypes::PointUI64 p)
	{
		m_x = p.x;
		m_y = p.y;
	}
}