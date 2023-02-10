#include <SFML/Graphics.hpp>
#include "VertexDefinitions.h"

#include <iostream>

namespace da
{
	class Mesh
	{
	public:
		sf::RenderWindow* m_pWindow;

	private:
		sf::VertexArray *m_pfield;
		sf::Vertex* m_pVertexAccesPointer;

		uint32_t m_FieldWidth;
		uint32_t m_FieldHeight;

		uint16_t m_AdditionalNumberForFileName;
		uint64_t* m_ploopEnd;

		std::string m_FilePrefix;

	public:
		Mesh(uint32_t width, uint32_t height, sf::RenderWindow* window, uint64_t* loopEnd);
		~Mesh();

		uint16_t GetFileNumber();

		sf::Color* GetColor(uint32_t x, uint32_t y);
		void SetColor(uint32_t x, uint32_t y, sf::Color* c);
		void SetColor(uint32_t x, uint32_t y, sf::Color c);
		void SetFilePrefix(const std::string & s);

		da::PointUI32 GetCenterPoint();
		void DrawMesh();
		void InitFieldColor(sf::Color c);

		void DumpToFile();
		void DumpToFileAndContinue();

	private:
		uint32_t TwoDimensionalIndextoOneDimensionalIndex(uint32_t x, uint32_t y);
		void InitFieldPossition();
	};

	class Ant
	{
	private:
		Mesh*		m_pMesh;

		sf::Color*	m_pColorTransitionArray;

		uint8_t		m_IteratorSize;
		uint8_t		m_IteratorIndex;

		int32_t		m_x;
		int32_t		m_y;

		int32_t		m_Width;
		int32_t		m_Height;

		int8_t		m_Facing;
		uint8_t		m_NextTurn;

		sf::Color*	m_pCurrentAntColor;
		uint8_t		m_CurrentColorIndex;

	public:
		Ant(Mesh* mesh, sf::Color* ColorTransitionArray, uint32_t Width, uint32_t Height);
		~Ant();

		void NextMove();
		void SetOffset(uint32_t x, uint32_t y);
		void SetOffset(PointUI32 p);

	private:

		void SetColorIndexSize(uint8_t i);
		void CheckBounds();
		uint8_t GetNextIndex();

		void MoveLeft();
		void MoveRight();

	};
}