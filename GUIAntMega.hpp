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
			LAST
		};

	private:
		sf::RenderTexture m_ProgressBarRenderTexture;

		sf::Texture m_ProgressBarInsideTexture;
		sf::Texture m_ProgressBarOutsideTexture;

		uint16_t m_ProgressBarWidth;
		uint16_t m_ProgressBarHeight;

		sf::Sprite m_ProgressBarInsideSprite;
		sf::Sprite m_ProgressBarOutsideSprite;

	public:
		GUIAntMega(uint32_t windowWidth, uint32_t windowHeight);
		virtual ~GUIAntMega();

		void SetProgress( float progressInPercent);

		virtual void Redraw();

	private:

	};

}
#endif