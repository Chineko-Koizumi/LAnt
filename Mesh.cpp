#include "Mesh.hpp"
#include "OsFeatures.hpp"

#include <filesystem>   // Creation of folder

namespace da 
{
	Mesh::Mesh(uint32_t width, uint32_t height, sf::RenderWindow* window)
		: MeshBase(width, height)
		, m_pWindow(window)
		, m_pfieldVertex( new sf::VertexArray(sf::Points, uint64_t(height) * uint64_t(width)) )
		, m_pTexture( new sf::Texture )
	{
		InitFieldPossition();

		m_pTexture->create(m_pWindow->getSize().x, m_pWindow->getSize().y);
	}

	Mesh::~Mesh()
	{
		delete m_pfieldVertex;
		delete m_pTexture;
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
		if (m_pWindow == nullptr)return;

		m_pWindow->setActive(true);

			m_pWindow->clear(sf::Color::Black);

			DrawMesh(false, true);
			DrawMesh(false, true);
				
			m_pTexture->update(*m_pWindow);
			
			sf::Image img = m_pTexture->copyToImage();

			std::filesystem::create_directories(outputPath);

			std::string FileName(outputPath + m_FilePrefix + std::to_string(m_FieldWidth) + "x" + std::to_string(m_FieldHeight) + ".png");
		
			img.saveToFile(FileName);
		
		m_pWindow->setActive(false);
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