#include <iostream>
#include <sstream>

namespace da
{
	class OsFeatures
	{
	public:
		static uint64_t GetFreeMemoryInKB();
		static uint32_t GetThreadCountForMeshSize(uint64_t windowWidth, uint64_t windowHeight);
		static bool IsEnoughFreeMemory(uint64_t windowWidth, uint64_t windowHeight, uint64_t sizeOfItem);
		static void ResolutionFromSystem(uint64_t& rScreenWidth, uint64_t& rScreenHeight);


	private:
		OsFeatures()	= delete;
		~OsFeatures()	= delete;
	};
}