#include "GUI.h"

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

	AntGUI::Names& operator++ (AntGUI::Names& name)
	{
		uint8_t temp = name;
		name = static_cast<AntGUI::Names>(++temp);

		return name;
	}

	AntGUI::AntGUI(uint32_t windowWidth, uint32_t windowHeight)
		:GUIBase(windowWidth, windowHeight, Names::LAST)
	{
		float windowSpacing = static_cast<float>(windowHeight) / 30U;
		for (Names name = Names::FIRST; name < Names::LAST; ++name)
		{
			m_pGUITexts[name].setFont(m_Font);
			m_pGUITexts[name].setFillColor(sf::Color(74U, 246U, 38U));
			m_pGUITexts[name].setOutlineColor(sf::Color(74U, 246U, 38U));
			m_pGUITexts[name].setString("NO TEXT");
			m_pGUITexts[name].setCharacterSize(windowSpacing);
			m_pGUITexts[name].setPosition(0.0f, windowSpacing * name);
		}
	}

	AntGUI::~AntGUI(){}

	void AntGUI::UpdateText(uint16_t name, const std::string& text)
	{
		m_pGUITexts[name].setString(text);
	}

	void AntGUI::Redraw()
	{
		m_pWindow->clear(sf::Color::Black);

		for (Names name = Names::FIRST; name < Names::LAST; ++name)
		{
			m_pWindow->draw(m_pGUITexts[name]);
		}
		m_pWindow->display();
	}

}

