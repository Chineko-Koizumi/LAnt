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

#endif

#ifdef __linux

#endif

	std::stringstream OsFeatures::m_Output;
	std::string OsFeatures::m_ProgressBar = std::string("");
}

