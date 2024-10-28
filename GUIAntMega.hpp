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
			MOVES = FIRST,
			THRESHOLD,
			INFO,
			LAST
		};

	private:
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

	public:
		GUIAntMega(uint32_t windowWidth, uint32_t windowHeight);
		virtual ~GUIAntMega();

		void InitThreasholdBar();
		void InitCopyBar();

		void SetProgressThreshold( float progressInPercent);
		void SetProgressCopy( float progressInPercent);

		virtual void Redraw();

	private:

	};

}
#endif