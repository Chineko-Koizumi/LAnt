#include "Mesh.hpp"
#include "OsFeatures.hpp"

#include "png.hpp"

#include <filesystem>   // Creation of folder

namespace da 
{
	Mesh::Mesh(uint32_t width, uint32_t height, sf::RenderWindow* window, daTypes::GreenColor* pDaGreenColorTransitionArray)
		: MeshBase(width, height, pDaGreenColorTransitionArray)
		, m_pWindow(window)
		, m_pfieldVertex( new sf::VertexArray(sf::Points, uint64_t(height) * uint64_t(width)) )
	{
		InitFieldPossition();
	}

	Mesh::~Mesh()
	{
		delete m_pfieldVertex;
	}

	uint64_t Mesh::TwoDimensionalIndextoOneDimensionalIndex(uint32_t x, uint32_t y)
	{
		return (uint64_t(y) * uint64_t(m_FieldWidth) + uint64_t(x));
	}

	void Mesh::DrawMesh(bool clearScreen, bool pushToScreen)//unsafe for generating without Window
	{
		if (clearScreen) m_pWindow->clear(sf::Color::Black);
		m_pWindow->draw(*m_pfieldVertex);
		if(pushToScreen) m_pWindow->display();
	}

	void Mesh::InitField(uint8_t encodedInitialPixel)
	{
		for (uint64_t i = 0; i < m_fieldSize; i++)
		{
			m_pfieldVertex->operator[](i).color = sf::Color(0u, 0u,  0u);
			m_pfield[i] = daConstants::TURN_MASK & encodedInitialPixel;
		}
	}

	void Mesh::DumpToFile(const std::string& outputPath)
	{
		png::image< png::rgb_pixel > pngImage(m_FieldWidth, m_FieldHeight);
		for (png::uint_32 y = 0; y < pngImage.get_height(); ++y)
		{
			for (png::uint_32 x = 0; x < pngImage.get_width(); ++x)
			{
				uint16_t decodedIndex = m_pfield[TwoDimensionalIndextoOneDimensionalIndex(x, y)] & daConstants::COLOR_INDEX_MASK;

				daTypes::GreenColor temp = m_pDaGreenColorTransitionArray[decodedIndex];

				pngImage[y][x] = png::rgb_pixel(0U, temp.g, 0U);
			}
		}
		
		std::filesystem::create_directories(outputPath);

		pngImage.write(outputPath + m_FilePrefix + std::to_string(m_FieldWidth) + "x" + std::to_string(m_FieldHeight) + ".png");
	}

	void Mesh::InitFieldPossition()
	{
		for (uint32_t y = 0; y < m_FieldHeight; y++)
		{
			for (uint32_t x = 0; x < m_FieldWidth; x++)
			{
				m_pfieldVertex->operator[](TwoDimensionalIndextoOneDimensionalIndex(x, y)).position = sf::Vector2f(float(x), float(y));
			}
		}
	}
}