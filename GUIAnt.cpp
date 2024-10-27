#include "GUIAnt.hpp"

namespace da 
{
	GUIAnt::Names& operator++ (GUIAnt::Names& name)
	{
		uint8_t temp = name;
		name = static_cast<GUIAnt::Names>(++temp);

		return name;
	}

	GUIAnt::GUIAnt(uint32_t windowWidth, uint32_t windowHeight)
		:GUIBase(windowWidth, windowHeight, Names::LAST)
	{
		float windowSpacing = static_cast<float>(windowHeight) / 30U;
		for (Names name = Names::FIRST; name < Names::LAST; ++name)
		{
			m_pGUITexts[name].setFont(m_Font);
			m_pGUITexts[name].setFillColor(sf::Color(74U, 246U, 38U));
			m_pGUITexts[name].setOutlineColor(sf::Color(74U, 246U, 38U));
			m_pGUITexts[name].setString("NO TEXT");
			m_pGUITextsStrings[name] = std::string("NO TEXT");
			m_pGUITexts[name].setCharacterSize(windowSpacing);
			m_pGUITexts[name].setPosition(0.0f, windowSpacing * name);
		}
	}

	GUIAnt::~GUIAnt() {}

	void GUIAnt::UpdateText(uint16_t name, const std::string& text)
	{
		m_pGUITextsStrings[name] = text;
		m_pGUITexts[name].setString(text);
	}

	void da::GUIAnt::UpdateTextAfter(uint16_t name, uint16_t charactersToSkip, const std::string& text)
	{
		m_pGUITextsStrings[name].erase(charactersToSkip - 1U); //needs to be offseted, this is index not count
		m_pGUITextsStrings[name].insert(charactersToSkip - 1U, text);
		m_pGUITexts[name].setString(m_pGUITextsStrings[name]);
	}

	void GUIAnt::Redraw()
	{
		m_pWindow->clear(sf::Color::Black);

		for (Names name = Names::FIRST; name < Names::LAST; ++name)
		{
			m_pWindow->draw(m_pGUITexts[name]);
		}
		m_pWindow->display();
	}
}
