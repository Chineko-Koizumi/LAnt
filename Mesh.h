#include <SFML/Graphics.hpp>

#include <thread>         
#include <mutex>
#include <iostream>

#include "VertexDefinitions.h"
namespace da
{
static const uint8_t TURN_MASK		  = 16;

	class Mesh
	{
	public:
		sf::RenderWindow* m_pWindow;
		std::mutex* m_pMutexCout;

	private:
		std::mutex* m_pMutexDumpFile;

		sf::VertexArray *m_pfield;

		uint32_t m_FieldWidth;
		uint32_t m_FieldHeight;

		uint16_t m_AdditionalNumberForFileName;
		uint64_t* m_ploopEnd;

		std::string m_FilePrefix;

	public:
		Mesh(uint32_t width, uint32_t height, sf::RenderWindow* window, std::mutex* mtxCout, std::mutex* mtxDumpFile, uint64_t* loopEnd);
		~Mesh();

		uint16_t GetFileNumber();

		sf::Color* GetColor(uint32_t x, uint32_t y);
		void SetColor(uint32_t x, uint32_t y, sf::Color c);
		void SetFilePrefix(const std::string & s);

		da::PointUI32 GetCenterPoint();
		void DrawMesh();
		void InitFieldColor(sf::Color c);

		void DumpToFile(const std::string& s);
		void DumpToFileAndContinue();

	private:
		uint64_t TwoDimensionalIndextoOneDimensionalIndex(uint32_t x, uint32_t y);
		void InitFieldPossition();
		void ThreadSafeDumpToFile(const std::string& s);
	};

	class MegaMesh
	{
	public:
		uint64_t* m_ploopEnd;
		uint8_t* m_pfield;

	private:

		
		uint64_t m_fieldSize;
		uint32_t m_FieldWidth;
		uint32_t m_FieldHeight;

		std::string m_FilePrefix;

	public:
		MegaMesh(uint32_t width, uint32_t height, uint64_t* loopEnd);
		~MegaMesh();

		inline uint8_t GetColor(uint32_t x, uint32_t y)
		{
			return m_pfield[uint64_t(y) * uint64_t(m_FieldWidth) + uint64_t(x)];
		}

		inline void SetColor(uint32_t x, uint32_t y, uint8_t c)
		{
			m_pfield[uint64_t(y) * uint64_t(m_FieldWidth) + uint64_t(x)] = c;
		}

		void SetFilePrefix(const std::string& s);

		da::PointUI32 GetCenterPoint();
		void InitFieldColor(uint8_t c);
		void DumpToFileBig(da::GreenColor* daGreenColors);

	private:

	};


	class Ant
	{
	private:
		Mesh*		m_pMesh;
		uint8_t*	m_pMeshFieldCopy;
		MegaMesh*	m_pMegaMesh;

		sf::Color*			m_pColorTransitionArray;
		da::GreenColor*		m_pDaGreenColorTransitionArray;
		uint8_t*			m_pColorMaskedTransitionArray;

		int32_t		m_x;
		int32_t		m_y;

		int32_t		m_Width;
		int32_t		m_Height;

		uint8_t		m_Facing;
		uint8_t		m_NextTurn;

		uint64_t* m_pLoopEndCopy;

		sf::Color*			m_pCurrentAntColor;
		uint8_t				m_CurrentAntColorMasked;

	public:
		Ant(Mesh* mesh, MegaMesh* megaMesh, sf::Color* ColorTransitionArray, da::GreenColor* DaGreenColorTransitionArray, uint32_t Width, uint32_t Height);
		~Ant();

		void NextMove();

		inline void NextMegaMove(uint32_t repetition)
		{
			for (uint32_t i = repetition; i ; --i)
			{
				m_CurrentAntColorMasked = m_pMeshFieldCopy[uint64_t(m_y) * uint64_t(m_Width) + uint64_t(m_x)];

				switch (TURN_MASK & m_CurrentAntColorMasked)
				{
				case 0:

					m_CurrentAntColorMasked &= 0x0F;
					m_CurrentAntColorMasked = (m_CurrentAntColorMasked == 13 ? 0 : ++m_CurrentAntColorMasked);

					m_pMeshFieldCopy[uint64_t(m_y) * uint64_t(m_Width) + uint64_t(m_x)] = m_pColorMaskedTransitionArray[m_CurrentAntColorMasked] + m_CurrentAntColorMasked;

					switch (m_Facing)
					{
						case 0: --m_x; m_Facing = 3U; break;
						case 1: --m_y; --m_Facing; break;
						case 2: ++m_x; --m_Facing; break;
						case 3: ++m_y; --m_Facing; break;
					}
					
					break;

				case 16:

					m_CurrentAntColorMasked &= 0x0F;
					m_CurrentAntColorMasked = (m_CurrentAntColorMasked == 13 ? 0 : ++m_CurrentAntColorMasked);

					m_pMeshFieldCopy[uint64_t(m_y) * uint64_t(m_Width) + uint64_t(m_x)] = m_pColorMaskedTransitionArray[m_CurrentAntColorMasked] + m_CurrentAntColorMasked;
					
					switch (m_Facing)
					{
						case 0:++m_x; ++m_Facing; break;
						case 1:++m_y; ++m_Facing; break;
						case 2:--m_x; ++m_Facing; break;
						case 3:--m_y; m_Facing = 0U; break;
					}
					
					break;
				}

				if (m_x == m_Width)	    m_pMegaMesh->DumpToFileBig(m_pDaGreenColorTransitionArray);
				else if (m_x == -1)		m_pMegaMesh->DumpToFileBig(m_pDaGreenColorTransitionArray);
				
				if (m_y == m_Height)	m_pMegaMesh->DumpToFileBig(m_pDaGreenColorTransitionArray);
				else if (m_y == -1)		m_pMegaMesh->DumpToFileBig(m_pDaGreenColorTransitionArray);
				
				switch (*m_pLoopEndCopy)
				{
					case 0: i = 1U;
				}
			}
		}

		void SetOffset(uint32_t x, uint32_t y);
		void SetOffset(da::PointUI32 p);

	private:

		void CheckBounds();

		inline void MoveLeft()
		{
			switch (m_Facing)
			{
			case 0: m_x--; m_Facing = 3; break;
			case 1: m_y--; m_Facing = 0; break;
			case 2: m_x++; m_Facing = 1; break;
			case 3: m_y++; m_Facing = 2; break;
			}
		}

		inline void MoveRight()
		{
			switch (m_Facing)
			{
			case 0:m_x++; m_Facing = 1; break;
			case 1:m_y++; m_Facing = 2; break;
			case 2:m_x--; m_Facing = 3; break;
			case 3:m_y--; m_Facing = 0; break;
			}
		}

	};
}