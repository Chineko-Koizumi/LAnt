#include "Mesh.h"       // field for ant and ant itself
#include "FileParser.h" // main function argument parrser
#include <fstream>

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
            std::cout << "Iteration per Update: " << m_RenderStepCount << std::endl;
        }

        static void SpeedDownRender()
        {
            if (m_RenderStepCount > 1U)m_RenderStepCount /= 10;
            std::cout << "Iteration per Update: " << m_RenderStepCount << std::endl;
        }

    private:
        KeyboardMethods() {};
        ~KeyboardMethods() {};
  
    };
    uint64_t KeyboardMethods::m_RenderStepCount = 1;

    enum MenuOptions
    {
        ANT_GUI = 0,
        ANT_NOGUI_FILE = 1,
        EXIT = 100
    };
}

int main(int argc, char* argv[])
{
    WINDOW_WIDTH    = atoi(argv[1]);
    WINDOW_HEIGHT   = atoi(argv[2]);

    uint8_t GenerationType = atoi(argv[3]);

    uint32_t SimulationStepsThreshold = atoi(argv[4]);
    sf::Color* colors = nullptr; // parsed colors for mesh from arguments
                                //da::FileParser::CreateColorArray(CSV); 

    sf::Event event; // for windows event pool
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Langton's Ant", true );
    da::Mesh mesh(WINDOW_WIDTH, WINDOW_HEIGHT, &window, &da::KeyboardMethods::m_RenderStepCount);
    

#pragma region Window
    uint64_t Progress = 0;
    switch (GenerationType)
    {
        case da::ANT_GUI:
        {
            std::ifstream infile("data.txt");
            std::string line;
            std::getline(infile, line);

            colors = da::FileParser::CreateColorArray(line);
            mesh.SetFilePrefix(da::FileParser::m_AntCurrentPathString);

            da::Ant ant(&mesh, colors, WINDOW_WIDTH, WINDOW_HEIGHT);

            window.setActive(true);
            while (da::KeyboardMethods::m_RenderStepCount != 0)
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
                            mesh.DumpToFile();
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

                for (size_t i = 0; i < da::KeyboardMethods::m_RenderStepCount; i++)ant.NextMove();   
            }
            da::FileParser::DeleteColorArray();
        }break;

        case da::ANT_NOGUI_FILE: 
        {
            std::ifstream infile("data.txt");
            std::string line;

            window.setActive(true);
            while (std::getline(infile, line))
            {
                std::cout << std::endl << "Line contents: " << line << std::endl;

                da::KeyboardMethods::m_RenderStepCount = 10000000;
                Progress = 0;

                colors = da::FileParser::CreateColorArray(line);
                if (colors == nullptr)
                {
                    std::cout << "AntiString detected skiping image generation: " << line << std::endl;
                    continue;
                }
                mesh.SetFilePrefix(da::FileParser::m_AntCurrentPathString);

                da::Ant ant(&mesh, colors, WINDOW_WIDTH, WINDOW_HEIGHT);

                while (da::KeyboardMethods::m_RenderStepCount != 0)
                {
                    if (Progress > SimulationStepsThreshold)
                    {
                        std::cout << double(SimulationStepsThreshold+1)/100 << " Bilion moves reached, cycle implied on file number[" << mesh.GetFileNumber()<<"]" << std::endl;
                        mesh.DumpToFile();
                        da::KeyboardMethods::m_RenderStepCount = 0;
                        Progress = 0;
                        break;
                    }
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
                                    mesh.DumpToFile();
                                    da::KeyboardMethods::m_RenderStepCount = 0;
                                    Progress = 0;
                                    continue;
                                }break;
                                }
                            }break;
                        }
                    }
                    std::cout << "File number:[" << mesh.GetFileNumber() << "] Ant moves: " << (Progress++) * da::KeyboardMethods::m_RenderStepCount << std::endl;
                    for (size_t i = 0; i < da::KeyboardMethods::m_RenderStepCount; i++) ant.NextMove();
                    
                }
                da::FileParser::DeleteColorArray();
            }
        }break;

        default: 
        {
            std::cout << "Unknown argument: " + atoi(argv[3])<<std::endl;
        } break;
    }
    

#pragma endregion
	return 0;
}