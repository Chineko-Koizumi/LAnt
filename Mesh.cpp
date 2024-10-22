#include "Mesh.hpp"
#include "WindowsFeatures.hpp"

#include <fstream>
#include <chrono>

#pragma region Mesh

namespace da 
{
	Mesh::Mesh(uint32_t width, uint32_t height, sf::RenderWindow* window)
		: MeshBase(width, height)
		, m_pWindow(window)
		, m_pfield(nullptr)
	{
		m_pfield = new sf::VertexArray(sf::Points, uint64_t(height) * uint64_t(width));

		InitFieldPossition();
	}

	Mesh::~Mesh()
	{
		delete m_pfield;
	}

	uint64_t Mesh::TwoDimensionalIndextoOneDimensionalIndex(uint32_t x, uint32_t y)
	{
		return (uint64_t(y) * uint64_t(m_FieldWidth) + uint64_t(x));
	}

	sf::Color* Mesh::GetColor(uint32_t x, uint32_t y)
	{
		return &(m_pfield->operator[](TwoDimensionalIndextoOneDimensionalIndex(x, y)).color);
	}

	void Mesh::SetColor(uint32_t x, uint32_t y, sf::Color c)
	{
		m_pfield->operator[](TwoDimensionalIndextoOneDimensionalIndex(x, y)).color = c;
	}

	void Mesh::SetFilePrefix(const std::string& s)
	{
		m_FilePrefix = s;
		m_FilePrefix.push_back('_');
	}

	daTypes::PointUI32 Mesh::GetCenterPoint()
	{
		return daTypes::PointUI32{ m_FieldWidth / 2, m_FieldHeight / 2 };
	}

	void Mesh::DrawMesh()//unsafe for generating without Window
	{
		m_pWindow->draw(*m_pfield);
		m_pWindow->display();
	}

	void Mesh::InitFieldColor(daTypes::Color c)
	{
		uint64_t fieldrenge = uint64_t(m_FieldWidth) * uint64_t(m_FieldHeight);

		for (uint64_t i = 0; i < fieldrenge; i++)
		{
			m_pfield->operator[](i).color = sf::Color(c.r, c.g,  c.b, c.a);
		}
	}

	void Mesh::DumpToFile(const std::string& outputPath)
	{
		if (m_pWindow == nullptr)return;

		sf::Texture texture;

		m_pWindow->setActive(true);

			m_pWindow->clear(sf::Color::Black);
			DrawMesh();

			texture.create(m_pWindow->getSize().x, m_pWindow->getSize().y);
			texture.update(*m_pWindow);

			sf::Image img = texture.copyToImage();

			filesystem::create_directories(outputPath);

			std::string FileName(m_FilePrefix + std::to_string(m_FieldWidth) + "x" + std::to_string(m_FieldHeight) + ".png");
			img.saveToFile(outputPath + FileName);

		m_pWindow->setActive(false);
	}

	void Mesh::InitFieldPossition()
	{
		for (uint32_t y = 0; y < m_FieldHeight; y++)
		{
			for (uint32_t x = 0; x < m_FieldWidth; x++)
			{
				m_pfield->operator[](TwoDimensionalIndextoOneDimensionalIndex(x, y)).position = sf::Vector2f(float(x), float(y));
			}
		}
	}

#pragma endregion

#pragma region MegaMesh

	MegaMesh::MegaMesh(uint32_t width, uint32_t height, daTypes::GreenColor* daGreenColorTransitionArray /* Green color with encoded data one alpa channel*/)
		: MeshBase(width, height)
		, m_fieldSize(0U)
		, m_pfield(nullptr)
		, m_pDaGreenColorTransitionArray(daGreenColorTransitionArray)
	{
		m_fieldSize = uint64_t(height) * uint64_t(width);
		m_pfield = new uint8_t[m_fieldSize];
	}

	MegaMesh::~MegaMesh()
	{
		delete[] m_pfield;
	}

	uint8_t* MegaMesh::GetFieldPtr()
	{
		return m_pfield;
	}

	void MegaMesh::SetFilePrefix(const std::string& s)
	{
		m_FilePrefix = s;
		m_FilePrefix.push_back('_');
	}

	daTypes::PointUI32 MegaMesh::GetCenterPoint()
	{
		return daTypes::PointUI32{ m_FieldWidth / 2, m_FieldHeight / 2 };
	}

	void MegaMesh::InitFieldColor(daTypes::Color c)
	{
		uint64_t fieldrenge = uint64_t(m_FieldWidth) * uint64_t(m_FieldHeight);
		for (uint64_t i = 0U; i < fieldrenge; i++)
		{
			m_pfield[i] = c.a; //this is field with already encoded values on 8 bit value see CreateDaGreenColorArray(const std::string& data) for more info
		}
	}

	void MegaMesh::DumpToFile(const std::string& outputPath)
	{
		std::string FileName(m_FilePrefix + std::to_string(m_FieldWidth) + "x" + std::to_string(m_FieldHeight) + ".ppm");
		std::stringstream Progress;

		filesystem::create_directories(outputPath);

		std::ofstream SSDump;
		SSDump.open(outputPath + FileName);

		SSDump << "P3" << std::endl;
		SSDump << m_FieldWidth << " " << m_FieldHeight << std::endl;
		SSDump << 255 << std::endl;

		auto start = std::chrono::high_resolution_clock::now();
		std::cout << std::endl << " Dumping started" << std::endl << std::endl << std::endl;

		std::string DumpSplitter = "";

		for (uint64_t i = 0; i < m_fieldSize; i++)
		{
			DumpSplitter += std::to_string(0);
			DumpSplitter += " ";
			DumpSplitter += std::to_string(m_pDaGreenColorTransitionArray[0x0F & m_pfield[i]].g); //0x0F <=> 0000 1111;
			DumpSplitter += " ";
			DumpSplitter += std::to_string(0);
			DumpSplitter += " ";
			if ((i % 1000) == 0)
			{
				SSDump << DumpSplitter;
				DumpSplitter = "";
				if ((i % 100000000) == 0)
				{
					Progress.clear();

					Progress << "\x1b[2k\x1b[A" << "\x1b[2k\x1b[A" << " done in: " << floor(double(i) / m_fieldSize * 100.0f) << "%      " << "\n\r" << WindowsFeatures::GenerateProgressBar(float(i) / m_fieldSize, 28) << std::endl;
					std::cout << Progress.str();
				}
			}
		}

		Progress.clear();
		Progress << "\x1b[2k\x1b[A" << "\x1b[2k\x1b[A" << " done in: 100%       " << "\n\r" << WindowsFeatures::GenerateProgressBar(1.0f, 28) << "\r";
		std::cout << Progress.str();

		if (DumpSplitter.compare(std::string("")) != 0)
		{
			SSDump << DumpSplitter;
		}

		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

		SSDump.close();

		std::cout << std::endl << " File generated in: " << duration.count() << "[ms] screenshot saved as " << FileName << std::endl;
	}

#pragma endregion




}