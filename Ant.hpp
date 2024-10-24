#ifndef ANT_HPP
#define ANT_HPP

#include "Mesh.hpp"
#include "MeshMega.hpp"

namespace da 
{
	class Ant
	{
	private:
		Mesh* m_pMesh;

		/// <summary>
		uint8_t* m_pMeshFieldCopy;

		uint8_t m_CurrentAntColorMaskedCount;
		uint8_t* m_pColorMaskedTransitionArray;

		uint8_t m_CurrentAntColorMasked;

		daTypes::GreenColor* m_pDaGreenColorTransitionArray;
		/// </summary>

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
			daTypes::GreenColor* DaGreenColorTransitionArray,
			uint32_t Width,
			uint32_t Height,
			std::string& antPath
		);

		~Ant();

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
					m_pMesh->	m_pfieldVertex->operator[](uint64_t(m_y)* m_Width + m_x).color = sf::Color(0U, m_pDaGreenColorTransitionArray[m_CurrentAntColorMasked].g, 0U);

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

		void SetOffset(uint32_t x, uint32_t y);
		void SetOffset(daTypes::PointUI32 p);

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
		uint8_t*	m_pMeshFieldCopy;
		MeshMega*	m_pMegaMesh;

		uint8_t* m_pColorMaskedTransitionArray;

		uint16_t		m_CurrentAntColorMasked;
		uint16_t		m_CurrentAntColorMaskedCount;

	public:
		MegaAnt(uint64_t* loopEnd
			, uint8_t threadIndex
			, uint32_t Width
			, uint32_t Height
			, std::string& antPath
			, daTypes::GreenColor* DaGreenColorTransitionArray
			, uint8_t* ColorMaskedTransitionArray
			, uint16_t ColorMaskedCount);

		~MegaAnt();

		virtual void DumpToFile(const std::string& outputPath) override;

		virtual inline bool NextMove(uint64_t repetition) override
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
	};

}

#endif