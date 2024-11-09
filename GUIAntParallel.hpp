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
		sf::Texture m_BackgroundTexture;
		sf::Sprite m_BackgroundSprite;

	public:
		GUIAntParallel(uint32_t windowWidth, uint32_t windowHeight, sf::RenderWindow* pWindow);
		~GUIAntParallel();

		virtual void FetchDataForGUI(uint8_t msgCountPerFetch);
		virtual void Redraw(bool clearScreen, bool pushToScreen);

	private:
		void InitBackground();

	};
}

#endif