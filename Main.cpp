#include "Mesh.h"       // field for ant and ant itself
#include "FileParser.h" // main function argument parrser

#include <fstream>
#include <thread>         
#include <mutex> 

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

    uint8_t GenerationType              = atoi(argv[3]);
    uint32_t SimulationStepsThreshold   = atoi(argv[4]);
    
#pragma region Window
    
    switch (GenerationType)
    {
        case da::ANT_GUI:
        {
            sf::Event event; // for windows event pool
            sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Langton's Ant", true);
            da::Mesh mesh(WINDOW_WIDTH, WINDOW_HEIGHT, &window, &da::KeyboardMethods::m_RenderStepCount);

            std::ifstream infile("data.txt");
            std::string line;
            std::getline(infile, line);

            sf::Color* colors = da::FileParser::CreateColorArray(line); // parsed colors for mesh from arguments
            mesh.SetFilePrefix(da::FileParser::m_AntCurrentPathString);

            da::Ant ant(&mesh, colors, WINDOW_WIDTH, WINDOW_HEIGHT);

            uint64_t Progress = 0;

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
            std::mutex mtx;
            std::thread* threads;

            const uint16_t processor_count = std::thread::hardware_concurrency();

            threads = new std::thread[processor_count];
           
            auto LambdaThread = []( std::ifstream* OpenFile, uint32_t SimulationStepsThresholdFromArgument, std::mutex* m)
            {
                uint64_t LambdaRenderStepCount = 0;
                std::thread::id thread_id = std::this_thread::get_id();

                sf::Event event; // for windows event pool
                sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Langton's Ant", true);
                da::Mesh mesh(WINDOW_WIDTH, WINDOW_HEIGHT, &window, &LambdaRenderStepCount);
                std::string line;

                uint64_t Progress = 0;

                window.setActive(true);
                while (true)
                {
                    m->lock();
                        std::getline(*OpenFile, line);
                        if (OpenFile->eof()) 
                        {
                            m->unlock();
                            break;
                        }
                    m->unlock();

                    //std::cout << std::endl << "Line contents: " << line << std::endl;

                    LambdaRenderStepCount = 10000000;
                    Progress = 0;

                    m->lock();
                        sf::Color* colors = da::FileParser::CreateColorArray(line);
                        if (colors == nullptr)
                        {
                            std::cout << "AntiString detected skiping image generation: " << line << std::endl;
                            m->unlock();
                            continue;
                        }
                        mesh.SetFilePrefix(da::FileParser::m_AntCurrentPathString);
                    m->unlock();

                    da::Ant ant(&mesh, colors, WINDOW_WIDTH, WINDOW_HEIGHT);

                    while (LambdaRenderStepCount != 0)
                    {
                        if (Progress > SimulationStepsThresholdFromArgument)
                        {
                            std::cout << double(SimulationStepsThresholdFromArgument + 1) / 100 << " Bilion moves reached, cycle implied on file number[" << mesh.GetFileNumber() << "]" << std::endl;
                            mesh.DumpToFile();
                            LambdaRenderStepCount = 0;
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
                                    LambdaRenderStepCount = 0;
                                    Progress = 0;
                                    continue;
                                }break;
                                case sf::Keyboard::Enter:
                                {
                                    mesh.DumpToFileAndContinue();
                                }break;
                                }
                            }break;
                            }
                        }
                        m->lock();
                        std::cout<<"Thread ID"<< thread_id << " File number:[" << mesh.GetFileNumber() << "] Ant moves: " << (Progress++) * LambdaRenderStepCount << std::endl;
                        m->unlock();
                        for (size_t i = 0; i < LambdaRenderStepCount; i++) ant.NextMove();

                    }
                    delete[]colors;
                }
                window.close();
            };

            for (size_t i = 0; i < processor_count; i++)
            {
                threads[i] = std::thread(LambdaThread, &infile, SimulationStepsThreshold, &mtx);
            }

            for (size_t i = 0; i < processor_count; i++)
            {
                threads[i].join();
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