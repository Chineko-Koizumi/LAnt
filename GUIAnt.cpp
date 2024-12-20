#include "GUIAnt.hpp"

namespace da 
{
	GUIAnt::Names& operator++ (GUIAnt::Names& name)
	{
		uint16_t temp = name;
		name = static_cast<GUIAnt::Names>(++temp);

		return name;
	}

	GUIAnt::GUIAnt(uint32_t windowWidth, uint32_t windowHeight, sf::RenderWindow* pExistingWindow)
		:GUIBase(windowWidth, windowHeight, Names::LAST, pExistingWindow)
	{	
		float windowSpacing = static_cast<float>(windowHeight) / 30U;
		for (Names name = Names::FIRST; name < Names::LAST; ++name)
		{
			m_pGUITexts[name].setFont(m_FontConsolas);
			m_pGUITexts[name].setFillColor(sf::Color(74U, 246U, 38U));
			m_pGUITexts[name].setOutlineColor(sf::Color(74U, 246U, 38U));
			m_pGUITexts[name].setString("");
			m_pGUITextsStrings[name] = std::string("");
			m_pGUITexts[name].setCharacterSize( static_cast<unsigned int>( windowSpacing ) );
			m_pGUITexts[name].setPosition(0.0f, static_cast<float>(m_pGUITexts[name].getCharacterSize() * name ) );
		}
	}

	GUIAnt::~GUIAnt() {}

	void GUIAnt::FetchDataForGUI(uint64_t msgCountPerFetch)
	{
		msgCountPerFetch;
		//to do: implement body for message fetching 
	}

	void GUIAnt::Redraw(bool clearScreen, bool pushToScreen)
	{
		if(clearScreen) m_pWindow->clear(sf::Color::Black);

		for (Names name = Names::FIRST; name < Names::LAST; ++name)
		{
			m_pWindow->draw(m_pGUITexts[name]);
		}
		if(pushToScreen) m_pWindow->display();
	}
}
