#include "Mesh.hpp"
#include "WindowsFeatures.hpp"

#include <filesystem>   // Creation of folder

#pragma region Mesh

namespace da 
{
	Mesh::Mesh(uint32_t width, uint32_t height, sf::RenderWindow* window)
		: MeshBase(width, height)
		, m_pWindow(window)
		, m_pfieldVertex( new sf::VertexArray(sf::Points, uint64_t(height) * uint64_t(width)) )
		, m_pTexture(new sf::Texture)
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

	void Mesh::DrawMesh()//unsafe for generating without Window
	{
		m_pWindow->draw(*m_pfieldVertex);
		m_pWindow->display();
		m_pWindow->draw(*m_pfieldVertex);
		m_pWindow->display();
	}

	void Mesh::InitFieldColor(daTypes::Color c)
	{
		uint64_t fieldrenge = uint64_t(m_FieldWidth) * uint64_t(m_FieldHeight);

		for (uint64_t i = 0; i < fieldrenge; i++)
		{
			m_pfieldVertex->operator[](i).color = sf::Color(c.r, c.g,  c.b);
			m_pfield[i] = constants::TURN_MASK & c.a;
		}
	}

	void Mesh::DumpToFile(const std::string& outputPath)
	{
		if (m_pWindow == nullptr)return;

		m_pWindow->setActive(true);

			m_pWindow->clear(sf::Color::Black);

			DrawMesh();
				
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

#pragma endregion

}