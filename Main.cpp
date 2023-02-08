#include "Mesh.h"       // field for ant and ant itself
#include "FileParser.h" // main function argument parrser
#include "Menus.h"      // ftxui menus

static uint32_t  WINDOW_WIDTH  = 0;
static uint32_t  WINDOW_HEIGHT = 0;

static const uint8_t  LEFT     = 0;
static const uint8_t  RIGHT    = 16;
namespace da
{
    class KeyboardMethods
    {
    public:
        static uint64_t m_RenderStepCount;

    private:
    public:
        static void SpeedUpRender()
        {
            if (m_RenderStepCount < 100000000U)m_RenderStepCount *= 10;
            std::cout << m_RenderStepCount << std::endl;
        }

        static void SpeedDownRender()
        {
            if (m_RenderStepCount > 1U)m_RenderStepCount /= 10;
            std::cout << m_RenderStepCount << std::endl;
        }

    private:
        KeyboardMethods() {};
        ~KeyboardMethods() {};
  
    };
    uint64_t KeyboardMethods::m_RenderStepCount = 1;
}

int main(int argc, char* argv[])
{
    std::string CSV(argv[1]);
    std::cout << CSV;

    WINDOW_WIDTH    = atoi(argv[2]);
    WINDOW_HEIGHT   = atoi(argv[3]);

    sf::Color* colors = da::FileParser::CreateColorArray(CSV); // parsed colors for mesh from arguments

    sf::Event event; // for windows event pool
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "ANt", true);

    da::Menu menu = da::Menu();

    da::Mesh mesh(WINDOW_WIDTH, WINDOW_HEIGHT, &window, atoi(argv[4]));
    da::Ant ant(&mesh, colors, WINDOW_WIDTH, WINDOW_HEIGHT);

    ant.SetOffset( WINDOW_WIDTH / 2 - 20, WINDOW_HEIGHT / 2);

#pragma region MainLoop
    uint8_t MenuOption=0;

    while(true)
    switch (MenuOption)
    {
        case da::MAIN:
        {
            MenuOption = menu.Main();
        }break;
        case da::ANT_GUI:
        {
            window.setActive(true);
            while (window.isOpen())
            {
                if (window.pollEvent(event))
                {
                    switch (event.type)
                    {
                    case sf::Event::KeyPressed:
                    {
                        switch (event.key.code)
                        {
                        case sf::Keyboard::Escape:
                        {
                            da::FileParser::DeleteColorArray();
                            mesh.DumpToFileAndExit();
                        }break;
                        case sf::Keyboard::Right:
                        {
                            da::KeyboardMethods::SpeedUpRender();
                        }break;
                        case sf::Keyboard::Left:
                        {
                            da::KeyboardMethods::SpeedDownRender();
                        }break;
                        }
                    }break;
                    }
                }

                mesh.DrawMesh();
                window.display();

                for (size_t i = 0; i < da::KeyboardMethods::m_RenderStepCount; i++)
                {
                    ant.NextMove();
                }
            }

        }break;

        case da::ANT_NOGUI:
        {
            uint64_t Progress = 0;
            while (window.isOpen())
            {
                if (window.pollEvent(event))
                {
                    switch (event.type)
                    {
                    case sf::Event::KeyPressed:
                    {
                        switch (event.key.code)
                        {
                        case sf::Keyboard::Escape:
                        {
                            mesh.DumpToFileAndExit();
                        }break;
                        }
                    }break;
                    }
                }
                for (size_t i = 0; i < 10000000; i++) ant.NextMove();
                std::cout << std::endl << (++Progress) * 10000000;
            }

        }break;
        case da::EXIT:
        {
            da::FileParser::DeleteColorArray();
            return 0;
        }break;

        default:
            break;
    }

#pragma endregion
	return 0;
}