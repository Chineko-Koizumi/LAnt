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

uint32_t da::WindowsFeatures::GetThreadCount(uint64_t windowWidth, uint64_t windowHeight)
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
	uint64_t TakenMemory = uint64_t(windowWidth) * uint64_t(windowHeight) * sizeOfItem / da::KB;
	if (TakenMemory > da::WindowsFeatures::GetFreeMemoryInKB())
	{
		std::cout << " Not enough free memory for this mesh size" << std::endl;
		std::cout << " Free memory in KB: " << da::WindowsFeatures::GetFreeMemoryInKB() << std::endl;
		std::cout << " Memory to be allocated in KB: " << TakenMemory << std::endl;
		return false;
	}

	return true;
}



