#include <SFML/Graphics.hpp>
#include "VertexDefinitions.h"

#include <thread>         
#include <mutex>
#include <iostream>

namespace da
{
	class Mesh
	{
	public:
		sf::RenderWindow* m_pWindow;

	private:
		std::mutex* m_pmtx;

		sf::VertexArray *m_pfield;
		sf::Vertex* m_pVertexAccesPointer;

		uint64_t m_FieldWidth;
		uint64_t m_FieldHeight;

		uint16_t m_AdditionalNumberForFileName;
		uint64_t* m_ploopEnd;

		std::string m_FilePrefix;

	public:
		Mesh(uint64_t width, uint64_t height, sf::RenderWindow* window, std::mutex* mtx, uint64_t* loopEnd);
		~Mesh();

		uint16_t GetFileNumber();

		sf::Color* GetColor(uint64_t x, uint64_t y);
		void SetColor(uint64_t x, uint64_t y, sf::Color* c);
		void SetColor(uint64_t x, uint64_t y, sf::Color c);
		void SetFilePrefix(const std::string & s);

		da::PointUI64 GetCenterPoint();
		void DrawMesh();
		void InitFieldColor(sf::Color c);

		void DumpToFile();
		void DumpToFileAndContinue();
		void DumpToFileBig();

	private:
		uint64_t TwoDimensionalIndextoOneDimensionalIndex(uint64_t x, uint64_t y);
		void InitFieldPossition();
		void ThreadSafeDumpToFile();
	};

	class Ant
	{
	private:
		Mesh*		m_pMesh;

		sf::Color*	m_pColorTransitionArray;

		uint8_t		m_IteratorSize;
		uint8_t		m_IteratorIndex;

		int64_t		m_x;
		int64_t		m_y;

		int64_t		m_Width;
		int64_t		m_Height;

		int8_t		m_Facing;
		uint8_t		m_NextTurn;

		sf::Color*	m_pCurrentAntColor;
		uint8_t		m_CurrentColorIndex;

	public:
		Ant(Mesh* mesh, sf::Color* ColorTransitionArray, uint64_t Width, uint64_t Height);
		~Ant();

		void NextMove();
		void SetOffset(uint64_t x, uint64_t y);
		void SetOffset(PointUI64 p);

	private:

		void SetColorIndexSize(uint8_t i);
		void CheckBounds();
		uint8_t GetNextIndex();

		void MoveLeft();
		void MoveRight();

	};
}