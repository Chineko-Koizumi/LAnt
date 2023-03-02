#include "Mesh.h"               // field for ant and ant itself
#include "FileParser.h"         // main function argument parrser
#include "WindowsFeatures.h"    // for Windows winapi features
#include <fstream>
 

static uint32_t WINDOW_WIDTH                = 0;
static uint32_t WINDOW_HEIGHT               = 0;
static uint64_t SIMULATION_STEPS_THRESHOLD  = 0;
static uint8_t GENERATION_TYPE              = 0;

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
    WINDOW_WIDTH               = atoi(argv[1]);
    WINDOW_HEIGHT              = atoi(argv[2]);
    GENERATION_TYPE            = atoi(argv[3]);
    SIMULATION_STEPS_THRESHOLD = atoi(argv[4]);

#pragma region Window
    
    switch (GENERATION_TYPE)
    {
        case da::ANT_GUI:
        {
            if ( !da::WindowsFeatures::IsEnoughFreeMemory(WINDOW_WIDTH, WINDOW_HEIGHT, da::SIZE_OF_VERTEX) ) return 0;
            
            sf::Event event; // for windows event pool
            sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Langton's Ant", sf::Style::Default);
            da::Mesh mesh(WINDOW_WIDTH, WINDOW_HEIGHT, &window, nullptr, nullptr, &da::KeyboardMethods::m_RenderStepCount);

            std::ifstream infile("data.txt");
            std::string line;
            std::getline(infile, line);

            sf::Color* colors = da::FileParser::CreateColorArray(line); // parsed colors for mesh from arguments
            mesh.SetFilePrefix(da::FileParser::m_AntCurrentPathString);

            da::Ant ant(&mesh, nullptr, colors, nullptr, WINDOW_WIDTH, WINDOW_HEIGHT);

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

                for (size_t i = 0; i < da::KeyboardMethods::m_RenderStepCount; ++i)ant.NextMove();   
            }
            da::FileParser::DeleteColorArray();

        }break;

        case da::ANT_NOGUI_FILE: 
        {
            if (!da::WindowsFeatures::IsEnoughFreeMemory(WINDOW_WIDTH, WINDOW_HEIGHT, da::SIZE_OF_VERTEX)) return 0;

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

                    da::Ant ant(&mesh, nullptr, colors, nullptr, WINDOW_WIDTH, WINDOW_HEIGHT);

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

                        for (size_t i = 0; i < LambdaRenderStepCount; ++i) ant.NextMove();

                        Progress++;

                    }
                    delete[]colors;

                }
                thrStatus[threadIndex] = false;
            };

            for (int i = 0; i < Thread_count; ++i)
            {
                threadsStatus[i] = true;

                threads[i] = std::thread(LambdaThread, i, threadsStatus, &infile, SIMULATION_STEPS_THRESHOLD, &mtxCout, &mtxDumpFile, &window);
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
                for (size_t i = 0; i < Thread_count; ++i)
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
             if (!da::WindowsFeatures::IsEnoughFreeMemory(WINDOW_WIDTH, WINDOW_HEIGHT, da::SIZE_OF_MASKED_COLOR)) return 0;

             auto start = std::chrono::high_resolution_clock::now();

             std::ifstream infile("data.txt");
             std::string line;
             std::getline(infile, line);

             da::MegaMesh megamesh(WINDOW_WIDTH, WINDOW_HEIGHT, &da::KeyboardMethods::m_RenderStepCount);
             
             da::GreenColor* daGreenColors = da::FileParser::CreateDaGreenColorArray(line); // parsed colors for mesh from arguments
     
             megamesh.SetFilePrefix(da::FileParser::m_AntCurrentPathString);

             uint64_t Progress = 0;

             da::Ant ant(nullptr, &megamesh, nullptr, daGreenColors, WINDOW_WIDTH, WINDOW_HEIGHT);

             da::KeyboardMethods::m_RenderStepCount = 100000000;// 0x7FFFFFFU;
             while (da::KeyboardMethods::m_RenderStepCount != 0)
             {
                 if (Progress > SIMULATION_STEPS_THRESHOLD)
                 {
                     std::cout << double(SIMULATION_STEPS_THRESHOLD) * da::KeyboardMethods::m_RenderStepCount << " moves, reached simulation limit" << std::endl;

                     megamesh.DumpToFileBig(daGreenColors);
                     da::KeyboardMethods::m_RenderStepCount = 0;
                     break;
                 }
                 std::cout << " Ant moves: " << (Progress) * da::KeyboardMethods::m_RenderStepCount << " Simulation threshold in %: " << (double(Progress++) / double(SIMULATION_STEPS_THRESHOLD)) * 100.0f <<  std::endl;
                
                 ant.NextMegaMove(da::KeyboardMethods::m_RenderStepCount);

             }
             da::FileParser::DeleteDaGreenColorArray();

             auto stop = std::chrono::high_resolution_clock::now();
             auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

             std::cout << " Whole operation took: " << duration.count() << "[ms]" << std::endl;

         }break;

        default: 
        {
            std::cout << "Unknown argument: " + std::string(argv[3]) << std::endl;
        } break;
    }
#pragma endregion
	return 0;
}