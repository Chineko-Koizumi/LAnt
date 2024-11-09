
#include "GUIAntParallel.hpp"

namespace da
{
	GUIAntParallel::GUIAntParallel(uint32_t windowWidth, uint32_t windowHeight, sf::RenderWindow* pWindow)
		: GUIBase(windowWidth/2.5f, windowHeight/ 2.5f, Names::LAST, pWindow)
	{
		InitBackground();
	}

	GUIAntParallel::~GUIAntParallel()
	{
	}

	void GUIAntParallel::FetchDataForGUI(uint8_t msgCountPerFetch)
	{
	}

	void GUIAntParallel::Redraw(bool clearScreen, bool pushToScreen)
	{
		if (clearScreen) m_pWindow->clear(sf::Color::Black);
			m_pWindow->draw(m_BackgroundSprite);
		if (pushToScreen) m_pWindow->display();
	}

	void GUIAntParallel::InitBackground()
	{
		m_BackgroundTexture.loadFromFile("./Sprites/GUI/AntMega/BackgroundParallel.png");
		m_BackgroundSprite.setTexture(m_BackgroundTexture);

		float scalingFactor = static_cast<float>(m_WindowWidth) / m_BackgroundTexture.getSize().x;
		m_BackgroundSprite.setScale(scalingFactor, scalingFactor);
		m_BackgroundSprite.setScale(scalingFactor, scalingFactor);
	}
}