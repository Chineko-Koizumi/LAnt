#include <SFML/Graphics.hpp>

#include <thread>         
#include <mutex>
#include <iostream>

#include "VertexDefinitions.h"
#include "DrawingAppConstants.h"

namespace da
{
	class Mesh
	{
	public:
		sf::RenderWindow* m_pWindow;

	private:
		sf::VertexArray *m_pfield;

		uint32_t m_FieldWidth;
		uint32_t m_FieldHeight;

		uint16_t m_AdditionalNumberForFileName;

		std::string m_FilePrefix;

	public:
		Mesh();
		Mesh(uint32_t width, uint32_t height, sf::RenderWindow* window);
		~Mesh();

		sf::Color* GetColor(uint32_t x, uint32_t y);
		void SetColor(uint32_t x, uint32_t y, sf::Color c);
		void SetFilePrefix(const std::string & s);

		da::PointUI32 GetCenterPoint();
		void DrawMesh();
		void InitFieldColor(sf::Color c);

		void DumpToFile();

	private:
		uint64_t TwoDimensionalIndextoOneDimensionalIndex(uint32_t x, uint32_t y);
		void InitFieldPossition();
	};

	class MegaMesh
	{
	private:
		uint8_t* m_pfield;

		uint64_t m_fieldSize;
		uint32_t m_FieldWidth;
		uint32_t m_FieldHeight;

		std::string m_FilePrefix;

	public:
		MegaMesh(uint32_t width, uint32_t height);
		~MegaMesh();

		uint8_t* GetFieldPtr();
		
		void SetFilePrefix(const std::string& s);

		da::PointUI32 GetCenterPoint();
		void InitFieldColor(uint8_t c);
		void DumpToFileBig(da::GreenColor* daGreenColors);

	private:

	};

	class Ant
	{
	private:
		Mesh		m_Mesh;

		sf::Color*			m_pColorTransitionArray;
		da::GreenColor*		m_pDaGreenColorTransitionArray;

		int32_t		m_DistanceToYWall;
		int32_t		m_DistanceToXWall;
		int32_t		m_NextCheck;

		uint8_t		m_ThreadID;

		sf::Color*	m_pCurrentAntColor;

	protected:
		int32_t		m_x;
		int32_t		m_y;

		int32_t		m_Width;
		int32_t		m_Height;

		uint8_t		m_Facing;
		uint8_t		m_NextTurn;
		uint64_t*	m_ploopEnd;

	public:
		Ant(
			sf::RenderWindow* window, 
			uint64_t* loopEnd, 
			uint8_t threadIndex,  
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
		uint8_t*	m_pMeshFieldCopy;
		MegaMesh	m_MegaMesh;

		da::GreenColor* m_pDaGreenColorTransitionArray;
		uint8_t*		m_pColorMaskedTransitionArray;

		uint8_t			m_CurrentAntColorMasked;
		uint8_t			m_CurrentAntColorMaskedCount;

	public:

		MegaAnt(	 uint64_t* loopEnd
					,uint8_t threadIndex
					,uint32_t Width
					,uint32_t Height
					,std::string& antPath
					,da::GreenColor* DaGreenColorTransitionArray
					,uint8_t* ColorMaskedTransitionArray
					,uint8_t ColorMaskedCount );

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
					case 0U:++m_x; ++m_Facing; break;
					case 1U:++m_y; ++m_Facing; break;
					case 2U:--m_x; ++m_Facing; break;
					case 3U:--m_y; m_Facing = 0U; break;
					}

					break;
				}

				if (m_x == m_Width)		return false;
				else if (m_x == -1)		return false;
										
				if (m_y == m_Height)	return false;
				else if (m_y == -1)		return false;

				switch (*m_ploopEnd)
				{
					case 0: i = 1U;
				}
			}

			return true;
		}
	};
}