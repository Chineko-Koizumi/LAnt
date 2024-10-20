#include "Mesh.h"
#include "WindowsFeatures.h"

#include <fstream>
#include <chrono>

#pragma region Mesh

static const uint8_t COLOR_INDEX_MASK = 15;

da::Mesh::Mesh() 
	:m_pWindow(nullptr)
	,m_FieldWidth(0)
	,m_FieldHeight(0)
	,m_AdditionalNumberForFileName(0)
	,m_FilePrefix(std::string("NoPrefixSet_"))
	,m_pfield(nullptr)
{
}

da::Mesh::Mesh(uint32_t width, uint32_t height, sf::RenderWindow* window)
	:m_pWindow(window)
	,m_FieldWidth(width)
	,m_FieldHeight(height)
	,m_AdditionalNumberForFileName(0)
	,m_FilePrefix(std::string("NoPrefixSet_"))
{
	m_pfield = new sf::VertexArray(sf::Points, uint64_t(height) * uint64_t(width));

	InitFieldPossition();
}

da::Mesh::~Mesh()
{
	delete m_pfield;
}

uint64_t da::Mesh::TwoDimensionalIndextoOneDimensionalIndex(uint32_t x, uint32_t y)
{
	return (uint64_t(y) * uint64_t(m_FieldWidth) + uint64_t(x));
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

da::PointUI32 da::Mesh::GetCenterPoint()
{
	return da::PointUI32{ m_FieldWidth / 2, m_FieldHeight / 2 };
}

void da::Mesh::DrawMesh()//unsafe for generating without Window
{
	m_pWindow->draw(*m_pfield);
	m_pWindow->display();
}

void da::Mesh::InitFieldColor(sf::Color c)
{
	uint64_t fieldrenge = uint64_t(m_FieldWidth) * uint64_t(m_FieldHeight);

	for (uint64_t i = 0; i < fieldrenge; i++)
	{
		m_pfield->operator[](i).color = c;
	}
}

void da::Mesh::DumpToFile()
{
	if (m_pWindow == nullptr)return;

	sf::Texture texture;

	m_pWindow->setActive(true);

		m_pWindow->clear();
		DrawMesh();
		m_pWindow->display();
		DrawMesh();
		m_pWindow->display();
	
		texture.create(m_pWindow->getSize().x, m_pWindow->getSize().y);
		texture.update(*m_pWindow);

	m_pWindow->setActive(false);

	sf::Image img = texture.copyToImage();

	std::string FileName(m_FilePrefix + std::to_string(m_FieldWidth)+"x" + std::to_string(m_FieldHeight) + "_" + std::to_string(m_AdditionalNumberForFileName) + ".png");
	img.saveToFile(FileName);

	m_AdditionalNumberForFileName++;
}

void da::Mesh::InitFieldPossition()
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

da::MegaMesh::MegaMesh(uint32_t width, uint32_t height)
	:m_FieldWidth(width)
	,m_FieldHeight(height)
	
{
	m_fieldSize = uint64_t(height) * uint64_t(width);
	m_pfield	= new uint8_t[m_fieldSize];
}

da::MegaMesh::~MegaMesh()
{
	delete[] m_pfield;
}

uint8_t* da::MegaMesh::GetFieldPtr()
{
	return m_pfield;
}

void da::MegaMesh::SetFilePrefix(const std::string& s)
{
	m_FilePrefix = s;
	m_FilePrefix.push_back('_');
}

da::PointUI32 da::MegaMesh::GetCenterPoint()
{
	return da::PointUI32{ m_FieldWidth / 2, m_FieldHeight / 2 };
}

void da::MegaMesh::InitFieldColor(uint8_t c)
{
	uint64_t fieldrenge = uint64_t(m_FieldWidth) * uint64_t(m_FieldHeight);
	for (uint64_t i = 0; i < fieldrenge; i++)
	{
		m_pfield[i] = c;
	}
}

void da::MegaMesh::DumpToFileBig(da::GreenColor* daGreenColors)
{
	std::string FileName(m_FilePrefix + std::to_string(m_FieldWidth) + "x" + std::to_string(m_FieldHeight) + "_" + ".ppm");
	std::stringstream Progress;

	std::ofstream SSDump;
	SSDump.open(FileName);

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
		DumpSplitter += std::to_string(daGreenColors[0x0F & m_pfield[i]].g);
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

				Progress << "\x1b[2k\x1b[A" << "\x1b[2k\x1b[A" << " done in: " << floor(double(i) / m_fieldSize * 100.0f) << "%      " << "\n\r" << da::WindowsFeatures::GenerateProgressBar(float(i) / m_fieldSize, 28)<<std::endl;
				std::cout << Progress.str();
			}
		}
	}

	Progress.clear();
	Progress << "\x1b[2k\x1b[A" << "\x1b[2k\x1b[A"<< " done in: 100%       " << "\n\r" << da::WindowsFeatures::GenerateProgressBar(1.0f, 28) << "\r";
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


#pragma region Ant
da::Ant::Ant
	(
	sf::RenderWindow* window,
	uint16_t threadIndex, 
	sf::Color* ColorTransitionArray, 
	uint32_t Width, 
	uint32_t Height,
	std::string& antPath
	)	
		:m_pMesh(new da::Mesh(Width, Height, window))
		,m_pColorTransitionArray(ColorTransitionArray)
		,m_x(0)
		,m_y(0)
		,m_Width(Width)
		,m_Height(Height)
		,m_Facing(0U)
		,m_NextTurn(0U)
		,m_NextCheck(0)
		,m_DistanceToYWall(0)
		,m_DistanceToXWall(0)
		,m_ThreadID(threadIndex)
		,m_pCurrentAntColor(nullptr)
{
		if (m_pColorTransitionArray == nullptr) return;

		SetOffset(m_pMesh->GetCenterPoint());
		m_pMesh->InitFieldColor(*m_pColorTransitionArray);
		m_pMesh->SetFilePrefix(antPath);
}

da::Ant::~Ant()
{
	delete m_pMesh;
}

bool da::Ant::NextMove(uint64_t repetitions)
{
	for (size_t i = 0; i < repetitions; i++)
	{
		m_pCurrentAntColor = m_pMesh->GetColor(m_x, m_y);

		m_NextTurn = constants::TURN_MASK & m_pCurrentAntColor->a;

		m_pMesh->SetColor(m_x, m_y, m_pColorTransitionArray[constants::COLOR_INDEX_MASK & m_pCurrentAntColor->a]);

		switch (m_NextTurn)
		{
		case constants::LEFT:	MoveLeft();		break;
		case constants::RIGHT:	MoveRight();	break;
		}

		if (m_NextCheck <= 0) 
		{
			if (m_x > m_Width - 1)	return false;
			else if (m_x < 0)		return false;
			

			if (m_y > m_Height - 1)	return false;
			else if (m_y < 0)		return false;
			
			m_DistanceToYWall = m_x < m_Width - 1 - m_x		? m_x : m_Width - 1 - m_x;
			m_DistanceToXWall = m_y < m_Height - 1 - m_y	? m_y : m_Height - 1 - m_y;

			m_NextCheck = m_DistanceToYWall < m_DistanceToXWall ? m_DistanceToYWall : m_DistanceToXWall; // shortest stright line to mesh border
			m_NextCheck *= 2;// The shortest path is always diagonal so on the grid this mean 2x more steps than stright line
			++m_NextCheck;
		}
		--m_NextCheck;
	}
	return true;
}

void da::Ant::SetOffset(uint32_t x, uint32_t y)
{
	m_x = x;
	m_y = y;
}

void da::Ant::SetOffset(da::PointUI32 p)
{
	m_x = p.x;
	m_y = p.y;
}

void da::Ant::DumpToFile()
{
	m_pMesh->DumpToFile();
}

void da::Ant::DrawMesh()
{
	m_pMesh->DrawMesh();
}

#pragma endregion

#pragma region MegaAnt

da::MegaAnt::MegaAnt(	 uint64_t* loopEnd
						,uint8_t threadIndex
						,uint32_t Width
						,uint32_t Height
						,std::string& antPath
						,da::GreenColor* DaGreenColorTransitionArray
						,uint8_t* ColorMaskedTransitionArray
						,uint16_t ColorMaskedCount)

			:Ant(nullptr, threadIndex, nullptr, Width, Height, antPath)
			,m_MegaMesh(MegaMesh(Width, Height))
			,m_CurrentAntColorMaskedCount(ColorMaskedCount)
			,m_pDaGreenColorTransitionArray(DaGreenColorTransitionArray)
			,m_pColorMaskedTransitionArray(ColorMaskedTransitionArray)
			,m_CurrentAntColorMasked(0U)

{
	SetOffset(m_MegaMesh.GetCenterPoint());

	for (size_t i = 0; i < m_CurrentAntColorMaskedCount; i++)
	{
		m_pColorMaskedTransitionArray[i] = constants::TURN_MASK & m_pDaGreenColorTransitionArray[i].a;
	}

	m_MegaMesh.SetFilePrefix(antPath);
	m_MegaMesh.InitFieldColor(m_pColorMaskedTransitionArray[0]);

	m_pMeshFieldCopy = m_MegaMesh.GetFieldPtr();
}

da::MegaAnt::~MegaAnt()
{
}

void da::MegaAnt::DumpToFile()
{
	m_MegaMesh.DumpToFileBig(m_pDaGreenColorTransitionArray);
}

#pragma endregion

