#ifndef GUIBASE_HPP
#define GUIBASE_HPP

#include <SFML/Graphics.hpp>

namespace da 
{
	class GUIBase
	{
	protected:
		sf::RenderWindow* m_pWindow;

		uint32_t m_WindowWidth;
		uint32_t m_WindowHeight;

		sf::Text* m_pGUITexts;

		sf::Font m_Font;

	public:

		GUIBase(uint32_t windowWidth, uint32_t windowHeight, uint16_t enumCount);
		~GUIBase();

		virtual void UpdateText(uint16_t name, const std::string& text) = 0;
		virtual void Redraw() = 0;

	private:
		
	};
}

#endif