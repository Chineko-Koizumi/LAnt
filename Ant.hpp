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
			uint32_t Width,
			uint32_t Height,
			const std::string& rAntPath,
			daTypes::GreenColor* pEncodedColorArray);

		virtual ~Ant();

		virtual inline uint64_t NextMove(uint64_t repetition)
		{
			for (uint64_t i = repetition; i; --i)
			{
				m_CurrentAntColorMasked = m_pMeshFieldCopy[uint64_t(m_y) * m_Width + m_x]; //current ant location 
				m_CurrentTurn = m_CurrentAntColorMasked & daConstants::TURN_MASK; // decoding next turn

				m_CurrentAntColorMasked &= daConstants::COLOR_INDEX_MASK; // decoding current color
				++m_CurrentAntColorMasked; //incrementing to get next color from path
				if (m_CurrentAntColorMasked == m_CurrentAntColorMaskedCount) m_CurrentAntColorMasked = 0; // check for bounds

				m_pMeshFieldCopy[uint64_t(m_y) * m_Width + m_x] = m_pColorMaskedTransitionArray[m_CurrentAntColorMasked] + m_CurrentAntColorMasked;
				m_pMesh->m_pfieldVertex->operator[](uint64_t(m_y)* m_Width + m_x).color = sf::Color(0U, m_pDaGreenColorTransitionArray[m_CurrentAntColorMasked].g, 0U);

				switch (m_CurrentTurn)
				{

				case daConstants::LEFT:

					switch (m_Facing)
					{
					case 0U: --m_x; m_Facing = 3U;	break;
					case 1U: --m_y; --m_Facing;		break;
					case 2U: ++m_x; --m_Facing;		break;
					case 3U: ++m_y; --m_Facing;		break;
					}

					break;

				case daConstants::RIGHT:

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
					if (m_x >= m_Width - 1)		return i;
					else if (m_x <= 0)			return i;
													   
					if (m_y >= m_Height - 1)	return i;
					else if (m_y <= 0)			return i;

					m_DistanceToYWall = m_x < m_Width - 1 - m_x ? m_x : m_Width - 1 - m_x;
					m_DistanceToXWall = m_y < m_Height - 1 - m_y ? m_y : m_Height - 1 - m_y;

					m_NextCheck = m_DistanceToYWall < m_DistanceToXWall ? m_DistanceToYWall : m_DistanceToXWall; // shortest stright line to mesh border
					m_NextCheck *= 2;// The shortest path is always diagonal so on the grid this mean 2x more steps than stright line
					++m_NextCheck;
				}
				--m_NextCheck;
			}

			return 0U;
		}
		virtual void DumpToFile(const std::string& outputPath);
		void DrawMesh(bool clearScreen, bool pushToScreen);

	private:
	};
}

#endif