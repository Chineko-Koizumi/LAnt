#include "GUIBase.hpp"

namespace da 
{
	GUIBase::GUIBase(uint32_t windowWidth, uint32_t windowHeight, uint16_t enumCount)
		: m_WindowWidth(windowWidth)
		, m_WindowHeight(windowHeight)
		, m_pWindow(new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "GUI", sf::Style::Titlebar))
		, m_pGUITexts(			enumCount > 0U ? new sf::Text[enumCount]	: nullptr)
		, m_pGUITextsStrings(	enumCount > 0U ? new std::string[enumCount] : nullptr)
		, m_Font()
	{
		m_pWindow->setPosition(sf::Vector2i(0, 0));

		if(!m_Font.loadFromFile("Consolas.ttf")) assert(false);
	}

	GUIBase::~GUIBase()
	{
		if(m_pGUITextsStrings != nullptr)	delete[] m_pGUITextsStrings;
		if(m_pGUITexts != nullptr)			delete[] m_pGUITexts;

		delete m_pWindow;
	}

	void GUIBase::UpdateText(uint16_t name, const std::string& text)
	{
		m_pGUITextsStrings[name] = text;
		m_pGUITexts[name].setString(text);
	}

	void GUIBase::UpdateTextAfter(uint16_t name, uint16_t charactersToSkip, const std::string& text)
	{
		m_pGUITextsStrings[name].erase(charactersToSkip - 1U); //needs to be offseted, this is index not count
		m_pGUITextsStrings[name].insert(charactersToSkip - 1U, text);
		m_pGUITexts[name].setString(m_pGUITextsStrings[name]);
	}

	sf::RenderWindow* da::GUIBase::GetWindowPtr()
	{
		return m_pWindow;
	}
}

