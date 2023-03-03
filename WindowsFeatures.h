#include <iostream>
#include <sstream>

namespace da
{
	const static uint32_t KB = 1024;

	class WindowsFeatures
	{
	public:
		static uint64_t GetFreeMemoryInKB();
		static uint32_t GetThreadCount(uint64_t windowWidth, uint64_t windowHeight);
		static bool IsEnoughFreeMemory(uint64_t windowWidth, uint64_t windowHeight, uint64_t sizeOfItem);
		static void AntMovesAndProgressBar(uint64_t moves, float progress, uint8_t barLength);
		static void SetConsoleModeToVTP();
		static std::string GenerateProgressBar(float progress/* Fixed <0;1> value*/, uint8_t barLength /* bar length in characters*/ );
	private:
		WindowsFeatures();
		~WindowsFeatures();

		static std::stringstream m_Output;
		static std::string m_ProgressBar;
	};
}