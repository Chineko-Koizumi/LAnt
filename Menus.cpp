#include "Menus.h"

da::Menu::Menu()
    :m_MainMenuIndex(0)
    ,m_MenuScreen(ftxui::ScreenInteractive::TerminalOutput())
{
    m_MainMenuEntries =
    {
        "Ant generation with preview    (slow)",
        "Ant generetion without preview (fast)",
        "Exit"
    };

    ExitCurrentLoopOption.on_enter = m_MenuScreen.ExitLoopClosure();
}

da::Menu::~Menu()
{
}

int da::Menu::Main()
{
    ftxui::Component menu = ftxui::Menu(&m_MainMenuEntries, &m_MainMenuIndex, &ExitCurrentLoopOption);

    ftxui::Component renderer = ftxui::Renderer(menu,
        [&] { return  ftxui::vbox
        (
            {
                 ftxui::hbox(ftxui::text("Ant main menu")),
                 ftxui::separator(),
                 menu->Render(),
            }
    ) | ftxui::border; });

    m_MenuScreen.Loop(renderer);

    switch (m_MainMenuIndex)
    {
        case 0:     m_MainMenuIndex = ANT_GUI;      break;
        case 1:     m_MainMenuIndex = ANT_NOGUI;    break;
        case 2:     m_MainMenuIndex = EXIT;         break;
        default:    m_MainMenuIndex = EXIT;         break;
    }

    return m_MainMenuIndex;
}
