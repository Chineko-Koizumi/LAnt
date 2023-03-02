#include "Mesh.h"

#include <fstream>
#include <chrono>

#pragma region Mesh

static const uint8_t COLOR_INDEX_MASK = 15;

da::Mesh::Mesh(uint32_t width, uint32_t height, sf::RenderWindow* window, std::mutex* mtxCout, std::mutex* mtxDumpFile, uint64_t* loopEnd)
	:m_pWindow(window)
	,m_pMutexCout(mtxCout)
	,m_pMutexDumpFile(mtxDumpFile)
	,m_FieldWidth(width)
	,m_FieldHeight(height)
	,m_AdditionalNumberForFileName(0)
	,m_ploopEnd(loopEnd)
	,m_FilePrefix(std::string("NoPrefixSet_"))
{
	m_pfield = new sf::VertexArray(sf::Points, uint64_t(height) * uint64_t(width));

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
}

void da::Mesh::InitFieldColor(sf::Color c)
{
	uint64_t fieldrenge = uint64_t(m_FieldWidth) * uint64_t(m_FieldHeight);

	for (uint64_t i = 0; i < fieldrenge; i++)
	{
		m_pfield->operator[](i).color = c;
	}
}

void da::Mesh::ThreadSafeDumpToFile(const std::string& s)
{
	sf::Texture texture;

	m_pMutexDumpFile->lock();
		m_pWindow->setActive(true);

		m_pWindow->clear();
		DrawMesh();
		m_pWindow->display();
		DrawMesh();
		m_pWindow->display();

		texture.create(m_pWindow->getSize().x, m_pWindow->getSize().y);
		texture.update(*m_pWindow);

		m_pWindow->setActive(false);
	m_pMutexDumpFile->unlock();

	sf::Image img = texture.copyToImage();

	std::string FileName(m_FilePrefix + std::to_string(m_FieldWidth) + "x" + std::to_string(m_FieldHeight) + "_" + std::to_string(m_AdditionalNumberForFileName) + ".png");
	img.saveToFile(FileName);

	m_pMutexCout->lock();
		std::cout << s <<" screenshot saved as " << FileName << std::endl;
	m_pMutexCout->unlock();

	m_AdditionalNumberForFileName++;
	*m_ploopEnd = 0;
}

void da::Mesh::DumpToFile(const std::string& s)
{
	if (m_pWindow == nullptr)return;
	if (m_pMutexCout != nullptr) 
	{
		ThreadSafeDumpToFile(s);
		return;
	}

	sf::Texture texture;
	m_pWindow->clear();
	DrawMesh();
	m_pWindow->display();
	DrawMesh();
	m_pWindow->display();
	
	texture.create(m_pWindow->getSize().x, m_pWindow->getSize().y);
	texture.update(*m_pWindow);

	sf::Image img = texture.copyToImage();

	std::string FileName(m_FilePrefix + std::to_string(m_FieldWidth)+"x" + std::to_string(m_FieldHeight) + "_" + std::to_string(m_AdditionalNumberForFileName) + ".png");
	img.saveToFile(FileName);

	std::cout << s << " screenshot saved as " << FileName << std::endl;
	
	m_AdditionalNumberForFileName++;
	*m_ploopEnd = 0;
}

void da::Mesh::DumpToFileAndContinue()
{
	if (m_pWindow == nullptr)return;

	m_pWindow->clear();
	DrawMesh();
	m_pWindow->display();
	DrawMesh();
	m_pWindow->display();

	sf::Texture texture;
	texture.create(m_pWindow->getSize().x, m_pWindow->getSize().y);
	texture.update(*m_pWindow);

	sf::Image img = texture.copyToImage();

	std::string FileName(m_FilePrefix + std::to_string(m_FieldWidth) + "x" + std::to_string(m_FieldHeight) + "_Snap_Shot_Of" + std::to_string(m_AdditionalNumberForFileName) + ".png");
	img.saveToFile(FileName);

	std::cout << "screenshot saved as " << FileName << std::endl;
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

da::MegaMesh::MegaMesh(uint32_t width, uint32_t height, uint64_t* loopEnd)
	:m_ploopEnd(loopEnd)
	,m_FieldWidth(width)
	,m_FieldHeight(height)
	
{
	m_fieldSize = uint64_t(height) * uint64_t(width);
	m_pfield	= new uint8_t[m_fieldSize];

}

da::MegaMesh::~MegaMesh()
{
	delete[] m_pfield;
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

	std::ofstream SSDump;
	SSDump.open(FileName);

	SSDump << "P3" << std::endl;
	SSDump << m_FieldWidth << " " << m_FieldHeight << std::endl;
	SSDump << 255 << std::endl;

	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "Dumping started:" << std::endl;

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
			if ((i % 100000000) == 0)std::cout << "done in %: " << double(i) / double(m_fieldSize) * 100.0f << std::endl;
		}
	}
	std::cout << "done in %: 100" << std::endl;

	if (DumpSplitter.compare(std::string("")) != 0)
	{
		SSDump << DumpSplitter;
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	SSDump.close();

	std::cout << " File generated in: " << duration.count() << "[ms] screenshot saved as " << FileName << std::endl;
	*m_ploopEnd = 0;
}

#pragma endregion


#pragma region Ant
da::Ant::Ant(Mesh* mesh, MegaMesh* megaMesh, sf::Color* ColorTransitionArray, da::GreenColor* DaGreenColorTransitionArray, uint32_t Width, uint32_t Height)
	:m_pMesh(mesh)
	,m_pMegaMesh(megaMesh)
	,m_pColorTransitionArray(ColorTransitionArray)
	,m_pDaGreenColorTransitionArray(DaGreenColorTransitionArray)
	,m_x(0)
	,m_y(0)
	,m_Width(Width)
	,m_Height(Height)
	,m_Facing(0)
	,m_NextTurn(0)

	,m_pCurrentAntColor(nullptr)
{
	if (m_pMesh != nullptr) 
	{
		SetOffset(m_pMesh->GetCenterPoint());
		m_pMesh->InitFieldColor(*m_pColorTransitionArray);
	}
	else 
	{
		SetOffset(m_pMegaMesh->GetCenterPoint());

		m_pColorMaskedTransitionArray = new uint8_t[14];
		for (size_t i = 0; i < 14; i++)
		{
			m_pColorMaskedTransitionArray[i] = da::TURN_MASK & DaGreenColorTransitionArray[i].a;
		}

		m_pMegaMesh->InitFieldColor(m_pColorMaskedTransitionArray[0]);
	}
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

void da::Ant::SetOffset(da::PointUI32 p)
{
	m_x = p.x;
	m_y = p.y;
}

void da::Ant::CheckBounds()
{
	if (m_x > m_Width - 1) 
	{
		m_pMesh->DumpToFile(std::string(" ant reached border: x > Width,"));
	}
	else if(m_x < 0)
	{
		m_pMesh->DumpToFile(std::string(" ant reached border: x < 0,"));
	}

	if(m_y > m_Height - 1)
	{
		m_pMesh->DumpToFile(std::string(" ant reached border: y > Height,"));
	}
	else if (m_y < 0)
	{
		m_pMesh->DumpToFile(std::string(" ant reached border: y < 0,"));
	}
}

#pragma endregion

