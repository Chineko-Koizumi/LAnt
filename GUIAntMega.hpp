#ifndef GUIANTMEGA_HPP
#define GUIANTMEGA_HPP

#include "GUIBase.hpp"

namespace da
{
	class GUIAntMega : public GUIBase
	{
	public:
		enum Names : uint16_t
		{
			FIRST = 0U,
			PATH = FIRST,
			MOVES,
			THRESHOLD,
			INFO,
			GENERATING_SPEED,
			MAX_SPEED,
			COPYING_PROGRESS,
			LAST
		};

	private:
		sf::Texture m_BackgroundTexture;
		sf::Sprite m_BackgroundSprite;

		sf::Vector2f m_aTextsPos[LAST]
		{
			  sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.085f,	static_cast<float>(m_WindowHeight) * 0.062f)
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.060f,	static_cast<float>(m_WindowHeight) * 0.110f)
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.060f,	static_cast<float>(m_WindowHeight) * 0.160f)
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.060f,	static_cast<float>(m_WindowHeight) * 0.278f)
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.060f,	static_cast<float>(m_WindowHeight) * 0.596f)
			, sf::Vector2f(static_cast<float>(m_WindowWidth) * 0.230f,	static_cast<float>(m_WindowHeight) * 0.596f)
		};

		struct ProgressBar
		{
			sf::RenderTexture m_ProgressBarRenderTexture;

			sf::Texture m_ProgressBarInsideTexture;
			sf::Texture m_ProgressBarOutsideTexture;

			uint16_t m_ProgressBarWidth{ 0U };
			uint16_t m_ProgressBarHeight{ 0U };

			sf::Sprite m_ProgressBarInsideSprite;
			sf::Sprite m_ProgressBarOutsideSprite;
		};

		ProgressBar m_ThreasholdBar;
		ProgressBar m_CopyBar;

		bool m_CopyStarted;

		static constexpr uint8_t PX_READINGS_SIZE = 10U;
		uint64_t pxReadings[PX_READINGS_SIZE];

		uint64_t m_MaxPxPerSec;

	public:
		GUIAntMega(uint32_t windowWidth, uint32_t windowHeight, std::string& path);
		virtual ~GUIAntMega();

		void SetProgressThreshold( float progressInPercent);
		void SetProgressCopy( float progressInPercent);

		void setPxPerS(uint64_t pxPerSec);

		bool IsCopyStarted();

		virtual void Redraw() override;

	private:
		void InitBackground();
		void InitThreasholdBar();
		void InitCopyBar();
		void InitText();

	};

}
#endif