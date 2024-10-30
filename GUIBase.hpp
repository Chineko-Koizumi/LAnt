#ifndef GUIBASE_HPP
#define GUIBASE_HPP

#include <SFML/Graphics.hpp>

#include <cassert>

namespace da 
{
	class GUIBase
	{
	protected:
		sf::RenderWindow* m_pWindow;

		uint32_t m_WindowWidth;
		uint32_t m_WindowHeight;

		sf::Text*		m_pGUITexts;
		std::string*	m_pGUITextsStrings;

		sf::Font m_FontConsolas;
		sf::Font m_FontTahoma;
		sf::Font m_FontTahomaBold;

		sf::Color m_ColorWindowsGreen{ 73U, 158U, 157U };
		sf::Color m_ColorWindowsBlack{ 6U, 8U, 12U };
		sf::Color m_ColorWindowsLightBlack{ 19U, 21U, 25U };

	public:

		GUIBase(uint32_t windowWidth, uint32_t windowHeight, uint16_t enumCount);
		virtual ~GUIBase();

		sf::RenderWindow* GetWindowPtr();
		void UpdateText(uint16_t name, const std::string& text);
		void UpdateText(uint16_t name, const char* text);
		void UpdateTextAfter(uint16_t name, uint16_t charactersToSkip, const std::string& text);
		void AppendText(uint16_t name, const std::string& text);

		virtual void Redraw() = 0;

	private:
		
	};
}

#endif