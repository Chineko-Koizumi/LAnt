#include "GUI.hpp"


da::GUI::GUI(uint32_t windowWidth, uint32_t windowHeight)
	:m_WindowWidth(windowWidth)
	,m_WindowHeight(windowHeight)
{
	m_pWindow = new sf::RenderWindow(sf::VideoMode(m_WindowWidth / 2U, m_WindowHeight / 3U), "GUI", sf::Style::Resize);

	for (Names i = Names::FIRST; i < Names::LAST; ++i)
	{
		UpdateText(i, "NO TEXT");
	}
}

da::GUI::~GUI()
{

}

void da::GUI::UpdateText(GUI::Names name, const std::string& text)
{
	m_aGUITexts[name].setString(text);
}

void da::GUI::Redraw()
{
	m_pWindow->clear(sf::Color::Black);

	for (Names i = Names::FIRST; i < Names::LAST; ++i)
	{
		m_pWindow->draw(m_aGUITexts[i]);
	}
	m_pWindow->display();
}