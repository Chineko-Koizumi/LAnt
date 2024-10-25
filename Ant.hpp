#ifndef ANT_HPP
#define ANT_HPP

#include "Mesh.hpp"
#include "MeshMega.hpp"

#include "AntBase.hpp"

namespace da 
{
	class Ant : public AntBase
	{
	private:
		da::Mesh* m_pMesh;

	public:
		Ant(
			sf::RenderWindow* window,
			daTypes::GreenColor* DaGreenColorTransitionArray,
			uint32_t Width,
			uint32_t Height,
			const std::string& rAntPath
		);

		virtual ~Ant();

		virtual inline bool NextMove(uint64_t repetition)
		{
			for (uint64_t i = repetition; i; --i)
			{
				m_CurrentAntColorMasked = m_pMeshFieldCopy[uint64_t(m_y) * m_Width + m_x];

				switch (constants::TURN_MASK & m_CurrentAntColorMasked)
				{
				case constants::LEFT:

					m_CurrentAntColorMasked &= 0x0FU;
					++m_CurrentAntColorMasked;
					if (m_CurrentAntColorMasked == m_CurrentAntColorMaskedCount) m_CurrentAntColorMasked = 0;

					m_pMeshFieldCopy[uint64_t(m_y) * m_Width + m_x] = m_pColorMaskedTransitionArray[m_CurrentAntColorMasked] + m_CurrentAntColorMasked;
					m_pMesh->m_pfieldVertex->operator[](uint64_t(m_y)* m_Width + m_x).color = sf::Color(0U, m_pDaGreenColorTransitionArray[m_CurrentAntColorMasked].g, 0U);

					switch (m_Facing)
					{
					case 0U: --m_x; m_Facing = 3U; break;
					case 1U: --m_y; --m_Facing; break;
					case 2U: ++m_x; --m_Facing; break;
					case 3U: ++m_y; --m_Facing; break;
					}

					break;

				case constants::RIGHT:

					m_CurrentAntColorMasked &= 0x0FU;
					++m_CurrentAntColorMasked;
					if (m_CurrentAntColorMasked == m_CurrentAntColorMaskedCount) m_CurrentAntColorMasked = 0;

					m_pMeshFieldCopy[uint64_t(m_y) * m_Width + m_x] = m_pColorMaskedTransitionArray[m_CurrentAntColorMasked] + m_CurrentAntColorMasked;
					m_pMesh->m_pfieldVertex->operator[](uint64_t(m_y)* m_Width + m_x).color = sf::Color(0U, m_pDaGreenColorTransitionArray[m_CurrentAntColorMasked].g, 0U);

					switch (m_Facing)
					{
					case 0U:++m_x; ++m_Facing;		break;
					case 1U:++m_y; ++m_Facing;		break;
					case 2U:--m_x; ++m_Facing;		break;
					case 3U:--m_y; m_Facing = 0U;	break;
					}

					break;
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
		virtual void DumpToFile(const std::string& outputPath);
		void DrawMesh();

	private:
	};
}

#endif