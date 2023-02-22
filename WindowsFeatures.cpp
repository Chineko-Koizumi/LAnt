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
