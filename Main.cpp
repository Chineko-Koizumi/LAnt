#include "Mesh.h"               // field for ant and ant itself
#include "FileParser.h"         // main function argument parrser
#include "WindowsFeatures.h"    // for Windows winapi features

#include <fstream>
 

static uint64_t  WINDOW_WIDTH  = 0;
static uint64_t  WINDOW_HEIGHT = 0;

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
        ANT_GUI                 = 0,
        ANT_NOGUI_FILE          = 1,
        ANT_NOGUI_LARGE_FILE    = 2,
        EXIT = 100
    };
}

int main(int argc, char* argv[])
{
    WINDOW_WIDTH    = atoi(argv[1]);
    WINDOW_HEIGHT   = atoi(argv[2]);

    uint64_t TakenMemory = WINDOW_WIDTH * WINDOW_HEIGHT * da::SIZE_OF_VERTEX / da::KB;
    if (TakenMemory > da::WindowsFeatures::GetFreeMemoryInKB()) 
    {
        std::cout << " Not enough free memory for this mesh size" << std::endl;
        std::cout << " Free memory in KB: " << da::WindowsFeatures::GetFreeMemoryInKB() <<std::endl;
        std::cout << " Memory to be allocated in KB: " << TakenMemory << std::endl;
        return 0;
    }

    uint8_t GenerationType              = atoi(argv[3]);
    uint64_t SimulationStepsThreshold   = atoi(argv[4]);
    
#pragma region Window
    
    switch (GenerationType)
    {
        case da::ANT_GUI:
        {
            sf::Event event; // for windows event pool
            sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Langton's Ant", sf::Style::Default);
            da::Mesh mesh(WINDOW_WIDTH, WINDOW_HEIGHT, &window, nullptr, nullptr, &da::KeyboardMethods::m_RenderStepCount);

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
                            mesh.DumpToFile(std::string("ESC Hit"));
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
            std::mutex mtxCout, mtxDumpFile;
            std::thread* threads;
            bool* threadsStatus;

            uint16_t Thread_count = da::WindowsFeatures::GetThreadCount(WINDOW_WIDTH, WINDOW_HEIGHT);

            threads = new std::thread[Thread_count];
            threadsStatus = new bool[Thread_count];

            sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Langton's Ant", sf::Style::Default);
            window.setActive(false);

            auto LambdaThread = [](int threadIndex, bool* thrStatus, std::ifstream* OpenFile, uint64_t SimulationStepsThresholdFromArgument, std::mutex* mc, std::mutex* md, sf::RenderWindow* w)
            {
                uint64_t LambdaRenderStepCount = 0;
                uint64_t Progress = 0;

                std::thread::id thread_id = std::this_thread::get_id();

                mc->lock();
                    std::cout << " Entering Thread:[" << thread_id << "]" << std::endl;
                mc->unlock();

                da::Mesh mesh(WINDOW_WIDTH, WINDOW_HEIGHT, w, mc, md, &LambdaRenderStepCount);
                
                std::string line;
                while (true)
                {
                    mc->lock();
                        if (OpenFile->eof())
                        {
                            mc->unlock();
                            break;
                        }
                        std::getline(*OpenFile, line);

                    mc->unlock();

                    LambdaRenderStepCount = 10000000;
                    Progress = 0;

                    mc->lock();
                        sf::Color* colors = da::FileParser::CreateColorArray(line);
                        if (colors == nullptr)
                        {
                            std::cout << " In Thread:["<< thread_id <<"] AntiString detected skiping image generation" << std::endl;
                            mc->unlock();
                            continue;
                        }
                        mesh.SetFilePrefix(da::FileParser::m_AntCurrentPathString);
                    mc->unlock();

                    da::Ant ant(&mesh, colors, WINDOW_WIDTH, WINDOW_HEIGHT);

                    while (LambdaRenderStepCount != 0)
                    {
                        if (Progress > SimulationStepsThresholdFromArgument)
                        {
                            mc->lock();
                                std::cout<<" " << double(SimulationStepsThresholdFromArgument) / 100 << " Bilion moves, reached simulation limit in Thread: [" << thread_id << "]" << std::endl;
                            mc->unlock();

                            mesh.DumpToFile(std::string("Simulation threshold exceeded,"));
                            LambdaRenderStepCount = 0;
                            Progress = 0;
                            break;
                        }

                        for (size_t i = 0; i < LambdaRenderStepCount; i++) ant.NextMove();

                        Progress++;

                    }
                    delete[]colors;
                }
                thrStatus[threadIndex] = false;
            };

            for (int i = 0; i < Thread_count; i++)
            {
                threadsStatus[i] = true;

                threads[i] = std::thread(LambdaThread, i, threadsStatus, &infile, SimulationStepsThreshold, &mtxCout, &mtxDumpFile, &window);
                threads[i].detach();
            }

            sf::Event event; // for windows event pool
            bool IsAllDetached = false;

            while (window.isOpen())
            {
                if (window.pollEvent(event))
                {
                    switch (event.type)
                    {
                        case sf::Event::Closed: 
                        {
                            window.close();
                        }break;

                        default: break;
                    }
                }

                IsAllDetached = true;
                for (size_t i = 0; i < Thread_count; i++)
                {
                    if (threadsStatus[i])
                    {
                        IsAllDetached = false;
                        break;
                    }  
                }
                if (IsAllDetached)window.close();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            delete[] threads;
            delete[] threadsStatus;
        }break;

         case da::ANT_NOGUI_LARGE_FILE:
         {
             std::ifstream infile("data.txt");
             std::string line;
             std::getline(infile, line);

             da::Mesh mesh(WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, nullptr, &da::KeyboardMethods::m_RenderStepCount);
             
             sf::Color* colors = da::FileParser::CreateColorArray(line); // parsed colors for mesh from arguments
             mesh.SetFilePrefix(da::FileParser::m_AntCurrentPathString);

             uint64_t Progress = 0;

             da::Ant ant(&mesh, colors, WINDOW_WIDTH, WINDOW_HEIGHT);

             da::KeyboardMethods::m_RenderStepCount = 10000000;
             while (da::KeyboardMethods::m_RenderStepCount != 0)
             {
                 if (Progress > SimulationStepsThreshold)
                 {
                     std::cout << double(SimulationStepsThreshold) / 100 << " Bilion moves, reached simulation limit" << std::endl;

                     mesh.DumpToFileBig();
                     da::KeyboardMethods::m_RenderStepCount = 0;
                     break;
                 }
                 std::cout << " Ant moves: " << (Progress++) * da::KeyboardMethods::m_RenderStepCount << " Simulation threshold in %: " << (double(Progress) / double(SimulationStepsThreshold)) * 100.0f <<  std::endl;
                
                 for (size_t i = 0; i < da::KeyboardMethods::m_RenderStepCount; i++) ant.NextMove();

             }
             da::FileParser::DeleteColorArray();
         }break;

        default: 
        {
            std::cout << "Unknown argument: " + atoi(argv[3])<<std::endl;
        } break;
    }
#pragma endregion
	return 0;
}