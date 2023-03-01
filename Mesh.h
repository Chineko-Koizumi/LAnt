#include <SFML/Graphics.hpp>

#include <thread>         
#include <mutex>
#include <iostream>
namespace da
{

static const uint8_t COLOR_INDEX_MASK = 15;
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

	private:

		da::GreenColor*				   m_pfield;
		uint64_t					m_fieldSize;

		uint32_t m_FieldWidth;
		uint32_t m_FieldHeight;

		std::string m_FilePrefix;

	public:
		MegaMesh(uint32_t width, uint32_t height, uint64_t* loopEnd);
		~MegaMesh();

		inline da::GreenColor* GetColor(uint32_t x, uint32_t y)
		{
			return &(m_pfield[TwoDimensionalIndextoOneDimensionalIndex(x, y)]);
		}

		inline void SetColor(uint32_t x, uint32_t y, da::GreenColor c)
		{
			m_pfield[TwoDimensionalIndextoOneDimensionalIndex(x, y)] = c;
		}

		void SetFilePrefix(const std::string& s);

		da::PointUI32 GetCenterPoint();
		void InitFieldColor(da::GreenColor c);
		void DumpToFileBig();

	private:

		inline uint64_t TwoDimensionalIndextoOneDimensionalIndex(uint32_t x, uint32_t y)
		{
			return (uint64_t(y) * uint64_t(m_FieldWidth) + uint64_t(x));
		}
	};


	class Ant
	{
	private:
		Mesh*		m_pMesh;
		MegaMesh*	m_pMegaMesh;

		sf::Color*			m_pColorTransitionArray;
		da::GreenColor*		m_pDaGreenColorTransitionArray;

		uint8_t		m_IteratorSize;
		uint8_t		m_IteratorIndex;

		int32_t		m_x;
		int32_t		m_y;

		int32_t		m_Width;
		int32_t		m_Height;

		int8_t		m_Facing;
		uint8_t		m_NextTurn;

		sf::Color*	m_pCurrentAntColor;
		da::GreenColor*  m_pCurrentAntDaGreenColor;

	public:
		Ant(Mesh* mesh, MegaMesh* megaMesh, sf::Color* ColorTransitionArray, da::GreenColor* DaGreenColorTransitionArray, uint32_t Width, uint32_t Height);
		~Ant();

		void NextMove();

		inline void NextMegaMove(uint32_t repetition)
		{
			for (uint32_t i = 0; i < repetition && *m_pMegaMesh->m_ploopEnd; i++)
			{
				m_pCurrentAntDaGreenColor = m_pMegaMesh->GetColor(m_x, m_y);

				m_NextTurn = TURN_MASK & m_pCurrentAntDaGreenColor->a;

				m_pMegaMesh->SetColor(m_x, m_y, m_pDaGreenColorTransitionArray[COLOR_INDEX_MASK & m_pCurrentAntDaGreenColor->a]);

				switch (m_NextTurn)
				{
				case 0:		MoveLeft();		break;
				case 16:	MoveRight();	break;
				}
				CheckMegaBounds();
			}
		}

		void SetOffset(uint32_t x, uint32_t y);
		void SetOffset(da::PointUI32 p);

	private:

		void SetColorIndexSize(uint8_t i);
		void CheckBounds();

		inline void CheckMegaBounds()
		{
			if (m_x > m_Width - 1)
			{
				m_pMegaMesh->DumpToFileBig();
			}
			else if (m_x < 0)
			{
				m_pMegaMesh->DumpToFileBig();
			}

			if (m_y > m_Height - 1)
			{
				m_pMegaMesh->DumpToFileBig();
			}
			else if (m_y < 0)
			{
				m_pMegaMesh->DumpToFileBig();
			}
		}

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