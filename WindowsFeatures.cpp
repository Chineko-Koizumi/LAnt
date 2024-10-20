#include "WindowsFeatures.h"

#include "VertexDefinitions.h"
#include <windows.h>
#include <thread>   

// Use to convert bytes to KB


da::WindowsFeatures::WindowsFeatures()
{
}

da::WindowsFeatures::~WindowsFeatures()
{
}

uint64_t da::WindowsFeatures::GetFreeMemoryInKB()
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	return statex.ullAvailPhys / KB;
}

uint32_t da::WindowsFeatures::GetThreadCountForMeshSize(uint64_t windowWidth, uint64_t windowHeight)
{
	uint32_t processor_count = std::thread::hardware_concurrency();

	uint64_t takenMemory	= windowWidth * windowHeight * SIZE_OF_VERTEX / KB; //memory in KB taken by vertex array
	uint64_t freeMemory		= GetFreeMemoryInKB();

	for (; 0 < processor_count; processor_count--)
	{
		if (processor_count * takenMemory < freeMemory) break;
	}

	return processor_count;
}

bool da::WindowsFeatures::IsEnoughFreeMemory(uint64_t windowWidth, uint64_t windowHeight, uint64_t sizeOfItem)
{
	uint64_t TakenMemory = windowWidth * windowHeight * sizeOfItem / da::KB;
	if (TakenMemory > da::WindowsFeatures::GetFreeMemoryInKB())
	{
		std::cout << " Not enough free memory for this mesh size" << std::endl;
		std::cout << " Free memory in KB: " << da::WindowsFeatures::GetFreeMemoryInKB() << std::endl;
		std::cout << " Memory to be allocated in KB: " << TakenMemory << std::endl;
		return false;
	}

	return true;
}

std::string da::WindowsFeatures::GenerateProgressBar(float progress, uint8_t barLength)
{
	m_ProgressBar.clear();
	m_ProgressBar.push_back(185U);

	progress *= 100.0f;
	float oneBarDeterminator = 100.0f / barLength;

	for (float i = 0.0f; i < progress; i += oneBarDeterminator)
	{
		m_ProgressBar.push_back(178U);
	}
	size_t temp = m_ProgressBar.length();
	for (size_t i = 0; i < barLength - temp + 2; ++i)
	{
		m_ProgressBar.push_back(32U);
	}

	m_ProgressBar.push_back(204U);
	return m_ProgressBar;
}

void da::WindowsFeatures::AntMovesAndProgressBar(uint64_t moves, float progress, uint8_t barLength)
{
	m_Output.str("");
	
	m_Output<< "\x1b[2k\x1b[A" << "\x1b[2k\x1b[A" << "\x1b[2k\x1b[A" << " Simulation threshold in: " << floor(progress * 100.0f) << "%" << "\n\r" << GenerateProgressBar(progress, barLength)<< "\n\r" << " Ant moves: " << moves << std::endl <<"\r";
	std::cout << m_Output.str();
}

void da::WindowsFeatures::SetConsoleModeToVTP()
{
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		std::cout<< "SetConsoleModeToVTP GetStdHandle error:" << GetLastError();
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		std::cout << "SetConsoleModeToVTP GetConsoleMode error:" << GetLastError();
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		std::cout << "SetConsoleModeToVTP SetConsoleMode error:" << GetLastError();
	}

	CONSOLE_CURSOR_INFO CURSOR;
	CURSOR.dwSize = 1;
	CURSOR.bVisible = FALSE;
	if (!SetConsoleCursorInfo(hOut, &CURSOR))
	{
		std::cout << "SetConsoleModeToVTP SetConsoleCursorInfo error:" << GetLastError();
	}
}

void da::WindowsFeatures::RestoreOldConsoleMode()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		std::cout << "RestoreOldConsoleMode GetStdHandle error:" << GetLastError();
	}

	CONSOLE_CURSOR_INFO CURSOR;
	CURSOR.dwSize = 1;
	CURSOR.bVisible = TRUE;
	if (!SetConsoleCursorInfo(hOut, &CURSOR))
	{
		std::cout << "SetConsoleModeToVTP SetConsoleCursorInfo error:" << GetLastError();
	}
}

void da::WindowsFeatures::InitTerminalForThreads(uint16_t threadMax)
{
	for (size_t i = 0; i < threadMax; ++i)
	{
		std::cout << std::endl;
	}
}

void da::WindowsFeatures::ThreadProgressGeneretor(uint16_t threadNumber, const std::string& infoUpdate)
{
	for (int i = 0; i < threadNumber; ++i)
	{
		std::cout << "\x1b[A";
	}
		std::cout << "\x1b[2k\r" << infoUpdate;

	for (int i = 0; i < threadNumber; ++i)
	{
		std::cout << "\x1b[B";
	}
}

std::stringstream da::WindowsFeatures::m_Output;
std::string da::WindowsFeatures::m_ProgressBar = std::string("");


