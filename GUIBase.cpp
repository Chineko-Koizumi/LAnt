#include "GUIBase.hpp"

namespace da 
{
	GUIBase::GUIBase(uint32_t windowWidth, uint32_t windowHeight, uint16_t enumCount)
		: m_WindowWidth(windowWidth)
		, m_WindowHeight(windowHeight)
		, m_pWindow(new sf::RenderWindow(sf::VideoMode(windowWidth / 2U, windowHeight / 3U), "GUI", sf::Style::None))
		, m_pGUITexts(new sf::Text[enumCount])
		, m_pGUITextsStrings(new std::string[enumCount])
		, m_Font()
	{
		m_pWindow->setPosition(sf::Vector2i(0, 0));

		m_Font.loadFromFile("Consolas.ttf");
	}

	GUIBase::~GUIBase()
	{
		delete[] m_pGUITextsStrings;
		delete[] m_pGUITexts;
		delete m_pWindow;
	}

	sf::RenderWindow* da::GUIBase::GetWindowPtr()
	{
		return m_pWindow;
	}

}

