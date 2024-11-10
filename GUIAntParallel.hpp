#ifndef GUIANTPARALLEL_HPP
#define GUIANTPARALLEL_HPP

#include "GUIBase.hpp"

namespace da 
{
	class GUIAntParallel : public GUIBase
	{
	public:
		enum Names : uint16_t
		{
			FIRST = 0U,
			TITLE = FIRST,
			RECENTLY_FINISHED_PATH,
			CURRENT_PROGRESS,
			LAST
		};

	private:
		uint16_t m_ThreadCount;

		Background m_Background;

		ProgressBar m_PathsProgress;
		ProgressBar* m_pThreadProgressBars;

	public:
		GUIAntParallel(uint32_t windowWidth, uint32_t windowHeight, uint16_t threadCount, sf::RenderWindow* pWindow);
		~GUIAntParallel();

		virtual void FetchDataForGUI(uint8_t msgCountPerFetch);
		virtual void Redraw(bool clearScreen, bool pushToScreen);

	private:
		void InitBackground();
		void InitBars();
	};
}

#endif