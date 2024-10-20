#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>

namespace da 
{
	class GUI
	{
	public:
		enum Names : uint8_t
		{
			FIRST = 0U,
			PATH = FIRST,

			MULTIPLIER,
			LAST
		};

	private:
		sf::RenderWindow* m_pWindow;

		uint32_t m_WindowWidth;
		uint32_t m_WindowHeight;

		sf::Text m_aGUITexts[GUI::Names::LAST];

		sf::Font m_Font;

	public:

		GUI(uint32_t windowWidth, uint32_t windowHeight);
		~GUI();

		void UpdateText(Names name, const std::string& text);
		void Redraw();

	private:
		
	};
}

#endif