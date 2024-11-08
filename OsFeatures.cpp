#include <windows.h>
#include <thread>   

#include "OsFeatures.hpp"
#include "DrawingAppConstants.hpp"

// Use to convert bytes to KB

namespace da 
{
#ifdef _WIN64

	uint64_t OsFeatures::GetFreeMemoryInKB()
	{
		MEMORYSTATUSEX statex;

		statex.dwLength = sizeof(statex);

		GlobalMemoryStatusEx(&statex);

		return statex.ullAvailPhys / daConstants::KB;
	}

	uint32_t OsFeatures::GetThreadCountForMeshSize(uint64_t windowWidth, uint64_t windowHeight)
	{
		uint32_t processor_count = std::thread::hardware_concurrency();

		uint64_t takenMemory = windowWidth * windowHeight * daConstants::SIZE_OF_VERTEX / daConstants::KB; //memory in KB taken by vertex array
		uint64_t freeMemory = GetFreeMemoryInKB();

		for (; 0 < processor_count; processor_count--)
		{
			if (processor_count * takenMemory < freeMemory) break;
		}

		return processor_count;
	}

	bool OsFeatures::IsEnoughFreeMemory(uint64_t windowWidth, uint64_t windowHeight, uint64_t sizeOfItem)
	{
		uint64_t TakenMemory = windowWidth * windowHeight * sizeOfItem / daConstants::KB;
		if (TakenMemory > OsFeatures::GetFreeMemoryInKB())
		{
			std::cout << " Not enough free memory for this mesh size" << std::endl;
			std::cout << " Free memory in KB: " << OsFeatures::GetFreeMemoryInKB() << std::endl;
			std::cout << " Memory to be allocated in KB: " << TakenMemory << std::endl;
			return false;
		}

		return true;
	}

	std::string OsFeatures::GenerateProgressBar(float progress, uint8_t barLength)
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

	void OsFeatures::SetConsoleModeToVTP()
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

	void OsFeatures::RestoreOldConsoleMode()
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

	void OsFeatures::InitTerminalForThreads(uint32_t threadMax)
	{
		for (size_t i = 0; i < threadMax; ++i)
		{
			std::cout << std::endl;
		}
	}

	void OsFeatures::ThreadProgressGeneretor(uint16_t threadNumber, const std::string& infoUpdate)
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

#endif

#ifdef __linux

#endif

	std::stringstream OsFeatures::m_Output;
	std::string OsFeatures::m_ProgressBar = std::string("");
}

