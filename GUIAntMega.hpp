#ifndef GUIANTMEGA_HPP
#define GUIANTMEGA_HPP

#include "GUIBase.hpp"
#include "Animation.hpp"

namespace da
{
	class GUIAntMega : public GUIBase
	{
	public:
		ProgressBar m_ThreasholdBar;

		enum States : uint16_t
		{
			GENERATING = 0U,
			COPING,
			IDLE_AFTER_COPING
		};

		enum Names : uint16_t
		{
			FIRST = 0U,
			PATH = FIRST,
			MOVES,
			THRESHOLD,
			INFO,
			GENERATING_SPEED,
			MAX_SPEED,
			OUTPUT_FILE,
			SOURCE_DESTINATION,
			COPYING_PROGRESS,
			LAST
		};

		enum Updates : uint16_t
		{
			THRESHOLD_PROGRESSBAR_UPDATE = 0U,
			COPY_PROGRESSBAR_UPDATE, 
			GUI_STATE_UPDATE
		};

	private:
		sf::Vector2f m_aTextsPos[LAST]
		{
			  sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.085f,	static_cast<float>(m_WindowHeight) * 0.069f)	/*PATH*/
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.060f,	static_cast<float>(m_WindowHeight) * 0.110f)	/*MOVES*/
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.060f,	static_cast<float>(m_WindowHeight) * 0.160f)	/*THRESHOLD*/
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.060f,	static_cast<float>(m_WindowHeight) * 0.278f)	/*INFO*/
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.060f,	static_cast<float>(m_WindowHeight) * 0.596f)	/*GENERATING_SPEED*/
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.230f,	static_cast<float>(m_WindowHeight) * 0.596f)	/*MAX_SPEED*/
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.375f,	static_cast<float>(m_WindowHeight) * 0.548f)	/*OUTPUT_FILE*/
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.375f,	static_cast<float>(m_WindowHeight) * 0.578f)	/*SOURCE_DESTINATION*/
		};

		Background m_Background;
																													
		ProgressBar m_CopyBar;

		Animation m_CopyAnimation;

		static constexpr uint8_t PX_READINGS_SIZE = 5U;
		uint64_t pxReadings[PX_READINGS_SIZE];

		uint64_t m_MaxPxPerSec;

		uint16_t m_CurrentState;

	public:
		GUIAntMega( uint32_t windowHeight, std::string& path);
		virtual ~GUIAntMega();

		void setPxPerS(uint64_t pxPerSec);

		void SetState(uint16_t stateNum);
		uint16_t GetState();

		virtual void FetchDataForGUI(uint64_t msgCountPerFetch) override;
		virtual void Redraw(bool clearScreen, bool pushToScreen) override;

	private:
		void InitBackground();
		void InitText();
		void InitBars();
	};

}
#endif