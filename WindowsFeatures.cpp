#include <windows.h>
#include <thread>   

#include "WindowsFeatures.hpp"
#include "DrawingAppConstants.hpp"

// Use to convert bytes to KB

namespace da 
{
	WindowsFeatures::WindowsFeatures()
	{
	}

	WindowsFeatures::~WindowsFeatures()
	{
	}

	uint64_t WindowsFeatures::GetFreeMemoryInKB()
	{
		MEMORYSTATUSEX statex;

		statex.dwLength = sizeof(statex);

		GlobalMemoryStatusEx(&statex);

		return statex.ullAvailPhys / constants::KB;
	}

	uint32_t WindowsFeatures::GetThreadCountForMeshSize(uint64_t windowWidth, uint64_t windowHeight)
	{
		uint32_t processor_count = std::thread::hardware_concurrency();

		uint64_t takenMemory = windowWidth * windowHeight * constants::SIZE_OF_VERTEX / constants::KB; //memory in KB taken by vertex array
		uint64_t freeMemory = GetFreeMemoryInKB();

		for (; 0 < processor_count; processor_count--)
		{
			if (processor_count * takenMemory < freeMemory) break;
		}

		return processor_count;
	}

	bool WindowsFeatures::IsEnoughFreeMemory(uint64_t windowWidth, uint64_t windowHeight, uint64_t sizeOfItem)
	{
		uint64_t TakenMemory = windowWidth * windowHeight * sizeOfItem / constants::KB;
		if (TakenMemory > WindowsFeatures::GetFreeMemoryInKB())
		{
			std::cout << " Not enough free memory for this mesh size" << std::endl;
			std::cout << " Free memory in KB: " << WindowsFeatures::GetFreeMemoryInKB() << std::endl;
			std::cout << " Memory to be allocated in KB: " << TakenMemory << std::endl;
			return false;
		}

		return true;
	}

	std::string WindowsFeatures::GenerateProgressBar(float progress, uint8_t barLength)
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

	void WindowsFeatures::AntMovesAndProgressBar(uint64_t moves, float progress, uint8_t barLength)
	{
		m_Output.str("");

		m_Output << "\x1b[2k\x1b[A" << "\x1b[2k\x1b[A" << "\x1b[2k\x1b[A" << " Simulation threshold in: " << floor(progress * 100.0f) << "%" << "\n\r" << GenerateProgressBar(progress, barLength) << "\n\r" << " Ant moves: " << moves << std::endl << "\r";
		std::cout << m_Output.str();
	}

	void WindowsFeatures::SetConsoleModeToVTP()
	{
		// Set output mode to handle virtual terminal sequences
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut == INVALID_HANDLE_VALUE)
		{
			std::cout << "SetConsoleModeToVTP GetStdHandle error:" << GetLastError();
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

	void WindowsFeatures::RestoreOldConsoleMode()
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

	void WindowsFeatures::InitTerminalForThreads(uint16_t threadMax)
	{
		for (size_t i = 0; i < threadMax; ++i)
		{
			std::cout << std::endl;
		}
	}

	void WindowsFeatures::ThreadProgressGeneretor(uint16_t threadNumber, const std::string& infoUpdate)
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

	std::stringstream WindowsFeatures::m_Output;
	std::string WindowsFeatures::m_ProgressBar = std::string("");




}

