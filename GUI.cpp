#include "GUI.h"

da::GUI::Names& operator++ (da::GUI::Names& name)
{
	uint8_t temp = name;
	name = static_cast<da::GUI::Names>(++temp);

	return name;
}

da::GUI::GUI(uint32_t windowWidth, uint32_t windowHeight)
	:m_WindowWidth(windowWidth)
	,m_WindowHeight(windowHeight)
	,m_pWindow(new sf::RenderWindow(sf::VideoMode(windowWidth / 2U, windowHeight / 3U), "GUI", sf::Style::Resize))
	,m_Font()
{
	m_pWindow->setPosition(sf::Vector2i(0, 0));

	m_Font.loadFromFile("Consolas.ttf");

	float windowSpacing = static_cast<float>(windowHeight) / 30U;
	for (Names name = Names::FIRST; name < Names::LAST; ++name)
	{
		m_aGUITexts[name].setFont( m_Font );
		m_aGUITexts[name].setFillColor(		sf::Color(74U, 246U, 38U) );
		m_aGUITexts[name].setOutlineColor(	sf::Color(74U, 246U, 38U) );
		m_aGUITexts[name].setString( "NO TEXT" );
		m_aGUITexts[name].setCharacterSize( windowSpacing );
		m_aGUITexts[name].setPosition( 0.0f, windowSpacing * name );
	}
}

da::GUI::~GUI()
{
	delete m_pWindow;
}

void da::GUI::UpdateText(GUI::Names name, const std::string& text)
{
	m_aGUITexts[name].setString(text);
}

void da::GUI::Redraw()
{
	m_pWindow->clear(sf::Color::Black);

	for (Names name = Names::FIRST; name < Names::LAST; ++name)
	{
		m_pWindow->draw(m_aGUITexts[name]);
	}
	m_pWindow->display();
}