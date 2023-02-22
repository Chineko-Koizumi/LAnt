#pragma once

#include <iostream>

namespace da
{
	const static uint32_t KB = 1024;

	class WindowsFeatures
	{
	public:
		static uint64_t GetFreeMemoryInKB();
		static uint32_t GetThreadCount(uint64_t windowWidth, uint64_t windowHeight);

	private:
		WindowsFeatures();
		~WindowsFeatures();
	};
}