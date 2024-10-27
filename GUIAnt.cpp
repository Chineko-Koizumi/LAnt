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
		:GUIBase(windowWidth / 2U, windowHeight / 3U, Names::LAST)
	{	
		float windowSpacing = static_cast<float>(windowHeight) / 30U;
		for (Names name = Names::FIRST; name < Names::LAST; ++name)
		{
			m_pGUITexts[name].setFont(m_Font);
			m_pGUITexts[name].setFillColor(sf::Color(74U, 246U, 38U));
			m_pGUITexts[name].setOutlineColor(sf::Color(74U, 246U, 38U));
			m_pGUITexts[name].setString("");
			m_pGUITextsStrings[name] = std::string("");
			m_pGUITexts[name].setCharacterSize(windowSpacing);
			m_pGUITexts[name].setPosition(0.0f, m_pGUITexts[name].getCharacterSize() * name);
		}
	}

	GUIAnt::~GUIAnt() {}

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
