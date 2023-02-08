#pragma once
#include <functional>  // for function
#include <iostream>  // for basic_ostream::operator<<, operator<<, endl, basic_ostream, basic_ostream<>::__ostream_type, cout, ostream
#include <string>    // for string, basic_string, allocator
#include <vector>    // for vector

#include "ftxui/component/captured_mouse.hpp"      // for ftxui
#include "ftxui/component/component.hpp"           // for Menu
#include "ftxui/component/component_options.hpp"   // for MenuOption
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive


namespace da 
{
	class Menu
	{
		ftxui::ScreenInteractive m_MenuScreen;

		std::vector<std::string> m_MainMenuEntries;
		int m_MainMenuIndex;

		ftxui::MenuOption ExitCurrentLoopOption;

	public:
		Menu();
		~Menu();

		int Main();

	private:

	};

	enum MenuOption
	{
		MAIN		= 0,
		ANT_GUI		= 1,
		ANT_NOGUI	= 2,
		EXIT		= 100
	};
}
