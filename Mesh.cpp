#include "Mesh.h"

#include <chrono>
using namespace std::chrono;

#define COLOR_INDEX_MASK	15
#define TURN_MASK			16

#pragma region Mesh

da::Mesh::Mesh(uint32_t width, uint32_t height, sf::RenderWindow* window, uint64_t* loopEnd)
	:m_FieldWidth(width)
	,m_FieldHeight(height)
	,m_pWindow(window)
	,m_pVertexAccesPointer(nullptr)
	,m_AdditionalNumberForFileName(0)
	,m_ploopEnd(loopEnd)
	,m_FilePrefix(std::string("NoPrefixSet_"))
{
	m_pfield = new sf::VertexArray(sf::Points, height * width);

	InitFieldPossition();
}

da::Mesh::~Mesh()
{
	delete m_pfield;
}

uint16_t da::Mesh::GetFileNumber()
{
	return m_AdditionalNumberForFileName;
}

uint32_t da::Mesh::TwoDimensionalIndextoOneDimensionalIndex(uint32_t x, uint32_t y)
{
	return (y * m_FieldWidth + x);
}

sf::Color* da::Mesh::GetColor(uint32_t x, uint32_t y)
{
	return &(m_pfield->operator[](TwoDimensionalIndextoOneDimensionalIndex(x, y)).color);
}

void da::Mesh::SetColor(uint32_t x, uint32_t y, sf::Color c) 
{
	m_pfield->operator[](TwoDimensionalIndextoOneDimensionalIndex(x, y)).color = c;
}

void da::Mesh::SetFilePrefix(const std::string& s)
{
	m_FilePrefix = s;
	m_FilePrefix.push_back('_');
}

void da::Mesh::SetColor(uint32_t x, uint32_t y, sf::Color* c)
{
	memcpy((void*)&m_pfield->operator[](TwoDimensionalIndextoOneDimensionalIndex(x, y)), (void*)c, sizeof(sf::Color));
}

da::PointUI32 da::Mesh::GetCenterPoint()
{
	return da::PointUI32{ m_FieldWidth / 2, m_FieldHeight / 2 };
}

void da::Mesh::DrawMesh()
{
	m_pWindow->draw(*m_pfield);
}

void da::Mesh::InitFieldColor(sf::Color c)
{
	for (size_t i = 0; i < m_FieldWidth * m_FieldHeight; i++)
	{
		m_pfield->operator[](i).color = c;
	}
}

void da::Mesh::DumpToFile()
{
	m_pWindow->clear();
	DrawMesh();
	m_pWindow->display();
	DrawMesh();
	m_pWindow->display();

	sf::Texture texture;
	texture.create(m_pWindow->getSize().x, m_pWindow->getSize().y);
	texture.update(*m_pWindow);

	sf::Image img = texture.copyToImage();

	std::string FileName(m_FilePrefix + std::to_string(m_FieldWidth)+"x" + std::to_string(m_FieldHeight) + "_" + std::to_string(m_AdditionalNumberForFileName) + ".png");
	img.saveToFile(FileName);

	std::cout << "screenshot saved as " << FileName << std::endl;
	m_AdditionalNumberForFileName++;
	*m_ploopEnd = 0;
}

void da::Mesh::InitFieldPossition()
{
	for (size_t y = 0; y < m_FieldHeight; y++)
	{
		for (size_t x = 0; x < m_FieldWidth; x++)
		{
			m_pfield->operator[](TwoDimensionalIndextoOneDimensionalIndex(x, y)).position = sf::Vector2f(float(x), float(y));
		}
	}
}

#pragma endregion

#pragma region Ant
da::Ant::Ant(Mesh* mesh, sf::Color* ColorTransitionArray, uint32_t Width, uint32_t Height)
	:m_x(0)
	,m_y(0)
	,m_Height(Height)
	,m_Width(Width)
	,m_Facing(0)
	,m_NextTurn(0)
	,m_pMesh(mesh)
	,m_pColorTransitionArray(ColorTransitionArray)
	,m_pCurrentAntColor(nullptr)
	,m_CurrentColorIndex(0)
{
	SetOffset(mesh->GetCenterPoint());
	m_pMesh->InitFieldColor(*m_pColorTransitionArray);
}

da::Ant::~Ant(){}

void da::Ant::NextMove()
{
	m_pCurrentAntColor = m_pMesh->GetColor(m_x, m_y);

	m_NextTurn = TURN_MASK & m_pCurrentAntColor->a;

	m_pMesh->SetColor(m_x, m_y, m_pColorTransitionArray[COLOR_INDEX_MASK & m_pCurrentAntColor->a]);

	switch (m_NextTurn)
	{
		case 0:		MoveLeft();		break;
		case 16:	MoveRight();	break;
	}
	CheckBounds();
}

void da::Ant::SetOffset(uint32_t x, uint32_t y)
{
	m_x = x;
	m_y = y;
}

void da::Ant::SetOffset(PointUI32 p)
{
	m_x = p.x;
	m_y = p.y;
}

void da::Ant::SetColorIndexSize(uint8_t i)
{
	m_IteratorSize = i;
}

void da::Ant::CheckBounds()
{
	if (m_x > m_Width - 1 || m_x < 0) 
	{
		std::cout << std::endl << "ant reached border: x > Width || x < 0" << std::endl;
		m_pMesh->DumpToFile();
	}
	if(m_y > m_Height - 1 || m_y < 0)
	{
		std::cout << std::endl << "ant reached border: y > Height || y < 0" << std::endl;
		m_pMesh->DumpToFile();
	}
}

uint8_t da::Ant::GetNextIndex()
{
	if (m_IteratorIndex < m_IteratorSize) 
	{
		return ++m_IteratorIndex;
	}
	else
	{
		m_IteratorIndex = 0;
		return m_IteratorIndex;
	}
}

void da::Ant::MoveLeft()
{
	switch (m_Facing)
	{
		case 0: m_x--; m_Facing = 3; break;
		case 1: m_y--; m_Facing = 0; break;
		case 2: m_x++; m_Facing = 1; break;
		case 3: m_y++; m_Facing = 2; break;
	}
}
void da::Ant::MoveRight()
{
	switch (m_Facing)
	{
		case 0:m_x++; m_Facing = 1; break;
		case 1:m_y++; m_Facing = 2; break;
		case 2:m_x--; m_Facing = 3; break;
		case 3:m_y--; m_Facing = 0; break;
	}
}
#pragma endregion

