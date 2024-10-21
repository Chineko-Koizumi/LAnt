#include "GUIBase.h"

namespace da 
{
	GUIBase::GUIBase(uint32_t windowWidth, uint32_t windowHeight, uint16_t enumCount)
		:m_WindowWidth(windowWidth)
		,m_WindowHeight(windowHeight)
		,m_pWindow(new sf::RenderWindow(sf::VideoMode(windowWidth / 2U, windowHeight / 3U), "GUI", sf::Style::Resize))
		,m_pGUITexts(new sf::Text[enumCount])
		,m_Font()
	{
		m_pWindow->setPosition(sf::Vector2i(0, 0));

		m_Font.loadFromFile("Consolas.ttf");
	}

	GUIBase::~GUIBase()
	{
		delete m_pWindow;
	}

}

