#ifndef ANT_HPP
#define ANT_HPP

#include "Mesh.h"

namespace da 
{
	class Ant
	{
	private:
		Mesh* m_pMesh;

		sf::Color* m_pColorTransitionArray;
		sf::Color* m_pCurrentAntColor;

		uint16_t		m_ThreadID;

	protected:
		int32_t		m_x;
		int32_t		m_y;

		int32_t		m_Width;
		int32_t		m_Height;

		uint8_t		m_Facing;
		uint8_t		m_NextTurn;

		int32_t		m_DistanceToYWall;
		int32_t		m_DistanceToXWall;
		int32_t		m_NextCheck;

	public:
		Ant(
			sf::RenderWindow* window,
			uint16_t threadIndex,
			sf::Color* ColorTransitionArray,
			uint32_t Width,
			uint32_t Height,
			std::string& antPath
		);

		~Ant();

		virtual bool NextMove(uint64_t repetitions);
		virtual void DumpToFile();

		void SetOffset(uint32_t x, uint32_t y);
		void SetOffset(da::PointUI32 p);

		void DrawMesh();

	private:

		inline void MoveLeft()
		{
			switch (m_Facing)
			{
			case 0U: m_x--; m_Facing = 3U; break;
			case 1U: m_y--; m_Facing = 0U; break;
			case 2U: m_x++; m_Facing = 1U; break;
			case 3U: m_y++; m_Facing = 2U; break;
			}
		}

		inline void MoveRight()
		{
			switch (m_Facing)
			{
			case 0U:m_x++; m_Facing = 1U; break;
			case 1U:m_y++; m_Facing = 2U; break;
			case 2U:m_x--; m_Facing = 3U; break;
			case 3U:m_y--; m_Facing = 0U; break;
			}
		}
	};

	class MegaAnt : public Ant
	{
	private:
		uint8_t* m_pMeshFieldCopy;
		MegaMesh	m_MegaMesh;

		da::GreenColor* m_pDaGreenColorTransitionArray;
		uint8_t* m_pColorMaskedTransitionArray;

		uint16_t		m_CurrentAntColorMasked;
		uint16_t		m_CurrentAntColorMaskedCount;

	public:
		MegaAnt(uint64_t* loopEnd
			, uint8_t threadIndex
			, uint32_t Width
			, uint32_t Height
			, std::string& antPath
			, da::GreenColor* DaGreenColorTransitionArray
			, uint8_t* ColorMaskedTransitionArray
			, uint16_t ColorMaskedCount);

		~MegaAnt();

		virtual void DumpToFile() override;

		virtual inline bool NextMove(uint64_t repetition) override
		{
			for (uint64_t i = repetition; i; --i)
			{
				m_CurrentAntColorMasked = m_pMeshFieldCopy[uint64_t(m_y) * m_Width + m_x];

				switch (constants::TURN_MASK & m_CurrentAntColorMasked)
				{
				case constants::LEFT:

					m_CurrentAntColorMasked &= 0x0FU;
					m_CurrentAntColorMasked = (m_CurrentAntColorMasked == m_CurrentAntColorMaskedCount - 1 ? 0 : ++m_CurrentAntColorMasked);

					m_pMeshFieldCopy[uint64_t(m_y) * m_Width + m_x] = m_pColorMaskedTransitionArray[m_CurrentAntColorMasked] + m_CurrentAntColorMasked;

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
					m_CurrentAntColorMasked = (m_CurrentAntColorMasked == m_CurrentAntColorMaskedCount - 1 ? 0 : ++m_CurrentAntColorMasked);

					m_pMeshFieldCopy[uint64_t(m_y) * m_Width + m_x] = m_pColorMaskedTransitionArray[m_CurrentAntColorMasked] + m_CurrentAntColorMasked;

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
	};

}

#endif