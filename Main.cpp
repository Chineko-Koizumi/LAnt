#include "Mesh.hpp"                 // Field for ant and ant itself
#include "Ant.hpp"                  // Ant backend
#include "GUIAnt.hpp"               // GUI with seperate window
#include "InputParser.hpp"          // Main function argument parrser
#include "WindowsFeatures.hpp"      // Windows winapi features
#include "DrawingAppConstants.hpp"  // Usefull constants

#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>      // Getting path data from file
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */

static uint32_t WINDOW_WIDTH                = 0;
static uint32_t WINDOW_HEIGHT               = 0;
static uint64_t SIMULATION_STEPS_THRESHOLD  = 0;
static uint8_t GENERATION_TYPE              = 0;

static std::string ANT_PATHS_FILE_PATH("EMPTY");
static std::string ANT_PATH_FROM_CL("EMPTY");

static std::string OUTPUT_PATH_VAR("");
static const std::string OUTPUT_PATH_CONST("./Output/");

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
        }

        static void SpeedDownRender()
        {
            if (m_RenderStepCount > 1U)m_RenderStepCount /= 10;
        }

    private:
        KeyboardMethods() {};
        ~KeyboardMethods() {};
  
    };
    uint64_t KeyboardMethods::m_RenderStepCount = 1;

    enum MenuOptions
    {
        ANT_GUI = 0,
        ANT_PARALLEL_FILE = 1,
        ANT_NOGUI_LARGE_FILE = 2,
        EXIT = 100
    };
}

int main(int argc, char* argv[])
{
    if (!strcmp(argv[1], "-CLG")) //generete from command line
    {
        WINDOW_WIDTH                = atoi(argv[2]);
        WINDOW_HEIGHT               = atoi(argv[3]);
        ANT_PATH_FROM_CL            = std::string(argv[4]);

        GENERATION_TYPE = da::MenuOptions::ANT_GUI;
    }
    else if (!strcmp(argv[1], "-FPG")) //generete from file, parrallel
    {
        WINDOW_WIDTH                = atoi(argv[2]);
        WINDOW_HEIGHT               = atoi(argv[3]);
        SIMULATION_STEPS_THRESHOLD  = atoi(argv[4]);
        ANT_PATHS_FILE_PATH         = std::string(argv[5]);

        GENERATION_TYPE = da::MenuOptions::ANT_PARALLEL_FILE;
    }
    else if (!strcmp(argv[1], "-CLMG")) //generete mega file from command line
    {
        WINDOW_WIDTH                = atoi(argv[2]);
        WINDOW_HEIGHT               = atoi(argv[3]);
        SIMULATION_STEPS_THRESHOLD  = atoi(argv[4]);
        ANT_PATH_FROM_CL            = std::string(argv[5]);

        GENERATION_TYPE = da::MenuOptions::ANT_NOGUI_LARGE_FILE;
    }
    else 
    {
        GENERATION_TYPE = da::MenuOptions::EXIT;
    }

    da::WindowsFeatures::SetConsoleModeToVTP();

    std::ifstream infile(ANT_PATHS_FILE_PATH);

    time_t rawtime;
    struct tm timeinfo;
    char buffer[80];

    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);

    strftime(buffer, 80, "%F(%Hh%Mm%Ss)/", &timeinfo);

    OUTPUT_PATH_VAR += OUTPUT_PATH_CONST;
    OUTPUT_PATH_VAR += buffer;

#pragma region Window
    
    switch (GENERATION_TYPE)
    {
        case da::ANT_GUI:
        {
            if ( !da::WindowsFeatures::IsEnoughFreeMemory(WINDOW_WIDTH, WINDOW_HEIGHT, constants::SIZE_OF_VERTEX) ) break;
            
            da::GUIAnt windowGUI(WINDOW_WIDTH, WINDOW_HEIGHT);
            windowGUI.UpdateText(da::GUIAnt::PATH, ANT_PATH_FROM_CL);
            windowGUI.UpdateText(da::GUIAnt::MULTIPLIER, std::to_string( da::KeyboardMethods::m_RenderStepCount));
            windowGUI.Redraw();

            sf::Event eventAnt; // for windows event pool
            sf::RenderWindow windowAnt(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Langton's Ant", sf::Style::None);

            daTypes::GreenColor* greenColors = da::InputParser::CreateDaGreenColorArrayFromCL(ANT_PATH_FROM_CL);
            da::Ant ant(&windowAnt, 0, greenColors, WINDOW_WIDTH, WINDOW_HEIGHT, ANT_PATH_FROM_CL);

            windowAnt.setActive(true);

            bool exit = false;
            while ( !exit )
            {
                if (windowAnt.pollEvent(eventAnt))
                {
                    switch (eventAnt.type)
                    {
                    case sf::Event::KeyPressed:
                    {
                        switch (eventAnt.key.code)
                        {
                        case sf::Keyboard::Escape:
                        {
                            exit = true;
                            continue;
                        }break;
                        case sf::Keyboard::Right:
                        {
                            da::KeyboardMethods::SpeedUpRender();
                            windowGUI.UpdateText(da::GUIAnt::MULTIPLIER, std::to_string(da::KeyboardMethods::m_RenderStepCount));
                            windowGUI.Redraw();
                        }break;
                        case sf::Keyboard::Left:
                        {
                            da::KeyboardMethods::SpeedDownRender();
                            windowGUI.UpdateText(da::GUIAnt::MULTIPLIER, std::to_string(da::KeyboardMethods::m_RenderStepCount));
                            windowGUI.Redraw();
                        }break;
                        }
                    }break;
                    }
                }

                if (!ant.NextMove(da::KeyboardMethods::m_RenderStepCount))  break;

                ant.DrawMesh();
            }

            ant.DumpToFile(OUTPUT_PATH_VAR);
            delete[] greenColors;

        }break;

        case da::ANT_PARALLEL_FILE: 
        {
            if (!da::WindowsFeatures::IsEnoughFreeMemory(WINDOW_WIDTH, WINDOW_HEIGHT, constants::SIZE_OF_VERTEX)) break;

            std::mutex mtxCout, mtxDumpFile;
            std::thread* threads;
            bool* threadsStatus;

            uint16_t Thread_count = da::WindowsFeatures::GetThreadCountForMeshSize(WINDOW_WIDTH, WINDOW_HEIGHT);

            threads = new std::thread[Thread_count];
            threadsStatus = new bool[Thread_count];

            sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Langton's Ant", sf::Style::None);
            window.setActive(false);

            da::WindowsFeatures::InitTerminalForThreads(Thread_count);

            std::vector< std::pair<std::string, daTypes::GreenColor* > > paths;

            std::string line;
            while (!infile.eof())
            {
                std::getline(infile, line);

                daTypes::GreenColor* pColor = da::InputParser::CreateDaGreenColorArrayFromCL(line);
                if (pColor != nullptr) 
                {
                    paths.push_back({ line, pColor });
                }
            }
            
            auto LambdaThread = [](
                uint16_t threadIndex, 
                uint16_t threadMax,
                bool* thrStatus, 
                std::vector< std::pair<std::string, daTypes::GreenColor* >>* vectorPaths,
                uint64_t SimulationStepsThresholdFromArgument, 
                std::mutex* mc, 
                std::mutex* md, 
                sf::RenderWindow* w)
            {
                std::thread::id thread_id = std::this_thread::get_id();

                std::stringstream Output;
                Output << thread_id;
                std::string sThreadID = Output.str();

                mc->lock();
                    da::WindowsFeatures::ThreadProgressGeneretor(threadIndex, std::string(" Entering Thread : [" + sThreadID + "]"));
                mc->unlock();
              
                uint16_t fileNumer = 0u;
                uint64_t LambdaRenderStepCount = 0;
                uint64_t Progress = 0;
                while (true)
                {
                    mc->lock();
                        if (vectorPaths->empty())
                        {
                            mc->unlock();
                            break;
                        }

                            daTypes::GreenColor* pGreenColor = vectorPaths->back().second;
                            da::Ant ant(w, threadIndex, pGreenColor, WINDOW_WIDTH, WINDOW_HEIGHT, vectorPaths->back().first);

                        vectorPaths->pop_back();
                    mc->unlock();

                    LambdaRenderStepCount = 10000000;
                    Progress = 0;
                    while (LambdaRenderStepCount != 0)
                    {
                        if (Progress > SimulationStepsThresholdFromArgument) break;
                        
                        mc->lock();

                            std::string ThreadText(6, ' ');
                            ThreadText.replace(0, sThreadID.length(), sThreadID);

                            Output.str("");
                            Output<<da::WindowsFeatures::GenerateProgressBar((float(Progress) / SIMULATION_STEPS_THRESHOLD), 28) << " Thread: [" << ThreadText<<"] File number: " << fileNumer;
                            da::WindowsFeatures::ThreadProgressGeneretor(threadIndex, Output.str());
                        
                       mc->unlock();

                        if (!ant.NextMove(LambdaRenderStepCount)) break;
                        
                        Progress++;
                    }

                    md->lock();
                        ant.DumpToFile(OUTPUT_PATH_VAR);
                    md->unlock();

                    fileNumer++;

                    if (pGreenColor != nullptr) 
                    {
                        delete[] pGreenColor;
                        pGreenColor = nullptr;
                    }
                }
                mc->lock();

                    std::string ThreadText(6, ' ');
                    ThreadText.replace(0, sThreadID.length(), sThreadID);
                    
                    da::WindowsFeatures::ThreadProgressGeneretor(threadIndex, std::string(" Thread: [" + ThreadText + "] Done generating                                            "));

                mc->unlock();   

                thrStatus[threadIndex] = false;
            };

            for (uint16_t i = 0; i < Thread_count; ++i)
            {
                threadsStatus[i] = true;

                threads[i] = std::thread(LambdaThread, i, Thread_count, threadsStatus, &paths, SIMULATION_STEPS_THRESHOLD, &mtxCout, &mtxDumpFile, &window);
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
             if (!da::WindowsFeatures::IsEnoughFreeMemory(WINDOW_WIDTH, WINDOW_HEIGHT, constants::SIZE_OF_MASKED_COLOR)) break;

             auto start = std::chrono::high_resolution_clock::now();

             std::cout << std::endl << std::endl << std::endl;//new lines as place for ant moves and progress bar;

             daTypes::GreenColor* daGreenColors = da::InputParser::CreateDaGreenColorArrayFromCL(ANT_PATH_FROM_CL); // parsed colors for mesh from arguments
     
             uint8_t* ColorMaskedTransitionArray = (uint8_t*)_alloca(ANT_PATH_FROM_CL.size());

             da::MegaAnt megaAnt(&da::KeyboardMethods::m_RenderStepCount, 0U, WINDOW_WIDTH, WINDOW_HEIGHT, ANT_PATH_FROM_CL, daGreenColors, ColorMaskedTransitionArray, ANT_PATH_FROM_CL.size());

             uint64_t Progress = 0U;
             da::KeyboardMethods::m_RenderStepCount = 100000000;
             while (da::KeyboardMethods::m_RenderStepCount != 0)
             {
                 if (Progress > SIMULATION_STEPS_THRESHOLD)
                 {
                     std::cout << std::endl << " " << (long double)SIMULATION_STEPS_THRESHOLD * da::KeyboardMethods::m_RenderStepCount << " moves, reached simulation limit" << std::endl;

                     da::KeyboardMethods::m_RenderStepCount = 0;
                     break;
                 }
                
                 da::WindowsFeatures::AntMovesAndProgressBar( Progress * da::KeyboardMethods::m_RenderStepCount, (float(Progress) / SIMULATION_STEPS_THRESHOLD), 28 );

                 if (!megaAnt.NextMove(da::KeyboardMethods::m_RenderStepCount)) break;
                 
                 ++Progress;
             }

             megaAnt.DumpToFile(OUTPUT_PATH_VAR);

             delete[] daGreenColors;

             auto stop = std::chrono::high_resolution_clock::now();
             auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

             std::cout << " Whole operation took: " << duration.count() << "[ms]" << std::endl;

         }break;

        default: 
        {
            std::cout << "Unknown argument: " + std::string(argv[1]) << std::endl;
        } break;
    }
#pragma endregion

    da::WindowsFeatures::RestoreOldConsoleMode();
	return 0;
}