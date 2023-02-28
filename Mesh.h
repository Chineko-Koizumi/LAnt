#include <SFML/Graphics.hpp>

#include <thread>         
#include <mutex>
#include <iostream>

namespace da
{
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

	private:

		da::DaVertex*				m_pfield;
		uint64_t					m_fieldSize;

		uint32_t m_FieldWidth;
		uint32_t m_FieldHeight;

		uint64_t* m_ploopEnd;

		std::string m_FilePrefix;

	public:
		MegaMesh(uint32_t width, uint32_t height, uint64_t* loopEnd);
		~MegaMesh();

		da::Color* GetColor(uint32_t x, uint32_t y);
		void SetColor(uint32_t x, uint32_t y, da::Color c);
		void SetFilePrefix(const std::string& s);

		da::PointUI32 GetCenterPoint();
		void InitFieldColor(da::Color c);
		void DumpToFileBig();

	private:
		uint64_t TwoDimensionalIndextoOneDimensionalIndex(uint32_t x, uint32_t y);
		void InitFieldPossition();
	};


	class Ant
	{
	private:
		Mesh*		m_pMesh;
		MegaMesh*	m_pMegaMesh;

		sf::Color*	m_pColorTransitionArray;
		da::Color*  m_pDaColorTransitionArray;

		uint8_t		m_IteratorSize;
		uint8_t		m_IteratorIndex;

		int32_t		m_x;
		int32_t		m_y;

		int32_t		m_Width;
		int32_t		m_Height;

		int8_t		m_Facing;
		uint8_t		m_NextTurn;

		sf::Color*	m_pCurrentAntColor;
		da::Color*  m_pCurrentAntDaColor;

	public:
		Ant(Mesh* mesh, MegaMesh* megaMesh, sf::Color* ColorTransitionArray, da::Color* DaColorTransitionArray, uint32_t Width, uint32_t Height);
		~Ant();

		void NextMove();
		void NextMegaMove();
		void SetOffset(uint32_t x, uint32_t y);
		void SetOffset(da::PointUI32 p);

	private:

		void SetColorIndexSize(uint8_t i);
		void CheckBounds();
		void CheckMegaBounds();

		void MoveLeft();
		void MoveRight();

	};
}