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
			RECENTLY_STARTED_PATH,
			CURRENT_TIME,
			CURRENT_PATH_STATUS,
			LAST
		};

		enum Updates : uint16_t
		{
			PATHS_PROGRESSBAR_UPDATE = 0U,
			THREADS_PROGRESSBAR_UPDATE,
			THREADS_PATH_UPDATE
		};

	private:
		sf::Vector2f m_aTextsPos[LAST]
		{
			  sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.045f,	static_cast<float>(m_WindowHeight) * 0.006f)	/*TITLE*/
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.025f,	static_cast<float>(m_WindowHeight) * 0.038f)	/*RECENTLY_STARTED_PATH*/
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.025f,	static_cast<float>(m_WindowHeight) * 0.078f)	/*CURRENT_TIME*/
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.450f,	static_cast<float>(m_WindowHeight) * 0.078f)	/*CURRENT_PATH_STATUS*/
		};

		uint64_t m_ThreadCount;
		uint64_t m_PathsCount;
		uint64_t m_PathsGenerated;

		Background m_Background;

		ProgressBar m_PathsProgress;
		ProgressBar* m_aThreadProgressBars;

	public:
		GUIAntParallel(uint64_t windowHeight, uint64_t threadCount);
		~GUIAntParallel();

		virtual void FetchDataForGUI(uint64_t msgCountPerFetch);
		virtual void Redraw(bool clearScreen, bool pushToScreen);

		void SetPathsCount(uint64_t pathsCount);

	private:
		void InitBackground();
		void InitBars();
		void InitText();
	};
}

#endif