
#include "Mesh.hpp"                 // Field for ant and ant itself
#include "Ant.hpp"                  // Ant backend
#include "AntMega.hpp"              // Ant Mega backend

#include "GUIAnt.hpp"               // GUIAnt windowParallelAnt
#include "GUIAntMega.hpp"           // GUIAntMega with seperate windowParallelAnt
#include "GUIAntParallel.hpp"       // GUIAntParallel with seperate windowParallelAnt

#include "InputParser.hpp"          // Main function argument parrser
#include "OsFeatures.hpp"      // Windows winapi features
#include "IPC.hpp"

#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>      // Getting path progressBarData from file
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */
#include <queue>

static uint32_t MESH_WIDTH                = 0;
static uint32_t MESH_HEIGHT               = 0;
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

    if (argc < 2 || argc > 6)
    {
        GENERATION_TYPE = da::MenuOptions::EXIT;
    }
    else if (!strcmp(argv[1], "-CLG")) //generete from command line
    {
        MESH_WIDTH                = atoi(argv[2]);
        MESH_HEIGHT               = atoi(argv[3]);
        ANT_PATH_FROM_CL            = std::string(argv[4]);

        GENERATION_TYPE = da::MenuOptions::ANT_GUI;
    }
    else if (!strcmp(argv[1], "-FPG")) //generete from file, parrallel
    {
        MESH_WIDTH                = atoi(argv[2]);
        MESH_HEIGHT               = atoi(argv[3]);
        SIMULATION_STEPS_THRESHOLD  = atoi(argv[4]);
        ANT_PATHS_FILE_PATH         = std::string(argv[5]);

        GENERATION_TYPE = da::MenuOptions::ANT_PARALLEL_FILE;
    }
    else if (!strcmp(argv[1], "-CLMG")) //generete mega file from command line
    {
        MESH_WIDTH                = atoi(argv[2]);
        MESH_HEIGHT               = atoi(argv[3]);
        SIMULATION_STEPS_THRESHOLD  = atoi(argv[4]);
        ANT_PATH_FROM_CL            = std::string(argv[5]);

        GENERATION_TYPE = da::MenuOptions::ANT_NOGUI_LARGE_FILE;
    }
    else 
    {
        GENERATION_TYPE = da::MenuOptions::EXIT;
    }

    std::ifstream infile(ANT_PATHS_FILE_PATH);

    {// no need to use those variables outside this braces

        time_t rawtime;
        struct tm timeinfo;
        char buffer[80];

        time(&rawtime);
        localtime_s(&timeinfo, &rawtime);

        strftime(buffer, 80, "%F(%Hh%Mm%Ss)/", &timeinfo);

        OUTPUT_PATH_VAR += OUTPUT_PATH_CONST;
        OUTPUT_PATH_VAR += buffer;
    }

    switch (GENERATION_TYPE)
    {
        case da::ANT_GUI:
        {
            if ( !da::OsFeatures::IsEnoughFreeMemory(MESH_WIDTH, MESH_HEIGHT, daConstants::SIZE_OF_VERTEX) ) break;
            
            sf::Event eventAnt; // for windowAnt event pool and GUI windowParallelAnt
            sf::RenderWindow windowAnt(sf::VideoMode(MESH_WIDTH, MESH_HEIGHT), "Langton's Ant", sf::Style::None);

            da::GUIAnt windowGUI(MESH_WIDTH, MESH_HEIGHT, &windowAnt);
            windowGUI.UpdateText(da::GUIAnt::PATH,          ANT_PATH_FROM_CL);
            windowGUI.UpdateText(da::GUIAnt::SPEED,         std::string("Speed: " + std::to_string(da::KeyboardMethods::m_RenderStepCount)));
            windowGUI.UpdateText(da::GUIAnt::MOVES,         std::string("Moves: " + std::to_string(0U)));
            windowGUI.UpdateText(da::GUIAnt::EXIT_INFO,     std::string("\n"));
            windowGUI.Redraw(daConstants::CLEAR_SCREEN, daConstants::PUSH_TO_SCREEN);

            daTypes::GreenColor* pGreenColorArray = da::InputParser::CreateDaGreenColorArrayFromCL(ANT_PATH_FROM_CL);

            da::Ant ant(&windowAnt, MESH_WIDTH, MESH_HEIGHT, ANT_PATH_FROM_CL, pGreenColorArray);

            windowAnt.setActive(false);

            uint64_t antMoves = 0U;
            bool exit = false;
            while ( !exit )
            {
                if (windowAnt.pollEvent(eventAnt))
                {
                    switch (eventAnt.type )
                    {
                    case sf::Event::KeyPressed:
                    {
                        switch (eventAnt.key.code)
                        {
                        case sf::Keyboard::Escape:
                        {
                            windowGUI.UpdateTextAfter(da::GUIAnt::EXIT_INFO, 2U, std::string("Escape clicked\nClick any key to exit."));
                            windowGUI.Redraw(daConstants::CLEAR_SCREEN, daConstants::PUSH_TO_SCREEN);
                            exit = true;
                            continue;
                        }break;
                        case sf::Keyboard::Right:
                        {
                            da::KeyboardMethods::SpeedUpRender();
                            windowGUI.UpdateTextAfter(da::GUIAnt::SPEED, 8U, std::to_string(da::KeyboardMethods::m_RenderStepCount));
                            windowGUI.Redraw(daConstants::CLEAR_SCREEN, daConstants::PUSH_TO_SCREEN);
                        }break;
                        case sf::Keyboard::Left:
                        {
                            da::KeyboardMethods::SpeedDownRender();
                            windowGUI.UpdateTextAfter(da::GUIAnt::SPEED, 8U, std::to_string(da::KeyboardMethods::m_RenderStepCount));
                            windowGUI.Redraw(daConstants::CLEAR_SCREEN, daConstants::PUSH_TO_SCREEN);
                        }break;
                        }
                    }break;
                    default: break;
                    }
                }

                uint64_t movesLeft = ant.NextMove(da::KeyboardMethods::m_RenderStepCount);
                if (movesLeft == 0)
                {
                    antMoves += da::KeyboardMethods::m_RenderStepCount;
                    windowGUI.UpdateTextAfter(da::GUIAnt::MOVES, 8U, std::to_string(antMoves) );
                }
                else 

                {
                    antMoves += movesLeft;
                    windowGUI.UpdateTextAfter(da::GUIAnt::MOVES,        8U, std::to_string(antMoves));
                    windowGUI.UpdateTextAfter(da::GUIAnt::EXIT_INFO,    2U, std::string("Ant reached border\nClick any key to exit."));
                    windowGUI.Redraw(daConstants::NO_CLEAR_SCREEN, daConstants::NO_PUSH_TO_SCREEN);
                    break;
                } 

                ant.DrawMesh(daConstants::NO_CLEAR_SCREEN, daConstants::NO_PUSH_TO_SCREEN);
                windowGUI.Redraw(daConstants::NO_CLEAR_SCREEN, daConstants::PUSH_TO_SCREEN);
            }

            ant.DrawMesh(daConstants::CLEAR_SCREEN, daConstants::PUSH_TO_SCREEN);
            ant.DumpToFile(OUTPUT_PATH_VAR);

            exit = false;
            while ( !exit )
            {
                if (windowAnt.pollEvent(eventAnt))
                {
                    switch (eventAnt.type)
                    {
                    case sf::Event::KeyPressed:
                    {
                        exit = true;
                        continue;
                    }break;
                    }
                }
            }

            delete[] pGreenColorArray;
        }break;

        case da::ANT_PARALLEL_FILE: 
        {
            if (!da::OsFeatures::IsEnoughFreeMemory(MESH_WIDTH, MESH_HEIGHT, daConstants::SIZE_OF_VERTEX)) break;

            std::mutex mtxAnt, mtxDumpFile, mtxDraw;
            std::thread* threads;
            bool* threadsStatus;

            sf::Event eventParallelAnt; // for windows event pool

            uint32_t Thread_count = da::OsFeatures::GetThreadCountForMeshSize(MESH_WIDTH, MESH_HEIGHT);
            threads = new std::thread[Thread_count];
            threadsStatus = new bool[Thread_count];

            sf::RenderWindow windowParallelAnt(sf::VideoMode(MESH_WIDTH, MESH_HEIGHT), "Langton's Ant", sf::Style::None);
            windowParallelAnt.setActive(false);

            std::vector< std::string > paths;
            std::string line;

            while (!infile.eof())
            {
                std::getline(infile, line);
                paths.push_back(line);  
            }
            
            auto start = std::chrono::high_resolution_clock::now();
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);

            da::GUIAntParallel parallelGUI(MESH_WIDTH, MESH_WIDTH, Thread_count);
            parallelGUI.SetPathsCount(paths.size());
            parallelGUI.UpdateText(da::GUIAntParallel::TITLE, ANT_PATHS_FILE_PATH);
            parallelGUI.UpdateText(da::GUIAntParallel::RECENTLY_STARTED_PATH, "Last Started: " + paths.back());
            parallelGUI.UpdateText(da::GUIAntParallel::CURRENT_TIME, "Elapsed time: " + std::to_string(duration.count()) + "s");
            parallelGUI.Redraw(daConstants::NO_CLEAR_SCREEN, daConstants::PUSH_TO_SCREEN);

            auto LambdaIPCThresholdSend = [](uint16_t threadIndex, float barPercent)
                {
                    size_t bufferSize =     sizeof(uint16_t) + sizeof(float);
                    uint8_t progressBarData[sizeof(uint16_t) + sizeof(float)];

                    memcpy_s(progressBarData,                       bufferSize, &threadIndex,   sizeof(uint16_t));
                    memcpy_s(progressBarData + sizeof(uint16_t),    bufferSize, &barPercent,    sizeof(float));

                    IPC::SendMessege(IPC::GUI_MESSAGE_VALUE_UPDATE, da::GUIAntParallel::THREADS_PROGRESSBAR_UPDATE, progressBarData, bufferSize);
                };

            auto LambdaIPCPathSend = [](uint16_t threadIndex,const std::string& path)
                {
                    size_t bufferSize = sizeof(uint16_t) + path.size() + 1U;
                    uint8_t* progressBarData = reinterpret_cast<uint8_t*>( alloca(bufferSize) );

                    memcpy_s(progressBarData,                       bufferSize, &threadIndex, sizeof(uint16_t));
                    memcpy_s(progressBarData + sizeof(uint16_t),    bufferSize, path.c_str(), path.size() + 1U);

                    IPC::SendMessege(IPC::GUI_MESSAGE_VALUE_UPDATE, da::GUIAntParallel::THREADS_PATH_UPDATE, progressBarData, bufferSize);
                };

            auto LambdaThread = [&LambdaIPCThresholdSend, &LambdaIPCPathSend](
                uint16_t threadIndex,
                bool* thrStatus, 
                std::vector< std::string>* pVectorPaths,
                uint64_t SimulationStepsThreshold, 
                std::mutex* pMutexAnt, 
                std::mutex* pMutexDump,
                std::mutex* pMutexDraw,
                sf::RenderWindow* pWindow)
            {
                uint64_t lambdaRenderStepCount = 0U;
                uint64_t progress = 0U;
                uint64_t antMoves = 0U;
                while (true)
                {
                    pMutexAnt->lock();
                    if (pVectorPaths->empty())
                    {
                        pMutexAnt->unlock();

                        break;
                    }

                    std::string tempPath = pVectorPaths->back();
                    pVectorPaths->pop_back();

                    pMutexAnt->unlock();

                    daTypes::GreenColor* pGreenColorArray = da::InputParser::CreateDaGreenColorArrayFromCL(tempPath);

                    if (pGreenColorArray == nullptr) 
                    {
                        IPC::SendMessege(IPC::GUI_MESSAGE_VALUE_UPDATE, da::GUIAntParallel::PATHS_PROGRESSBAR_UPDATE);
                        IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, da::GUIAntParallel::CURRENT_PATH_STATUS, nullptr, 0);
                        LambdaIPCThresholdSend(threadIndex, 1.0f);

                        continue;
                    }

                    da::Ant ant(pWindow, MESH_WIDTH, MESH_HEIGHT, tempPath, pGreenColorArray);

                    IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, da::GUIAntParallel::RECENTLY_STARTED_PATH, tempPath);

                    LambdaIPCPathSend(threadIndex, tempPath);
                    LambdaIPCThresholdSend(threadIndex, 0.0f);

                    lambdaRenderStepCount = 10000000U;
                    progress = 0U;
                    while (true)
                    {
                        if (progress > SimulationStepsThreshold) break;
                                            
                        LambdaIPCThresholdSend(threadIndex, float(progress) / SIMULATION_STEPS_THRESHOLD);

                        uint64_t movesLeft = ant.NextMove(lambdaRenderStepCount);
                        if (movesLeft == 0)
                        {
                            antMoves += lambdaRenderStepCount;
                        }
                        else
                        {
                            antMoves += movesLeft;
                            break;
                        }
                        
                        progress++;
                    }

                    IPC::SendMessege(IPC::GUI_MESSAGE_VALUE_UPDATE, da::GUIAntParallel::PATHS_PROGRESSBAR_UPDATE);
                    IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, da::GUIAntParallel::CURRENT_PATH_STATUS, nullptr, 0);

                    pMutexDraw->lock();
                    pWindow->setActive(true);
                        ant.DrawMesh(daConstants::NO_CLEAR_SCREEN, daConstants::PUSH_TO_SCREEN);
                        ant.DrawMesh(daConstants::NO_CLEAR_SCREEN, daConstants::PUSH_TO_SCREEN);
                    pWindow->setActive(false);
                    pMutexDraw->unlock();

                    pMutexDump->lock();
                        ant.DumpToFile(OUTPUT_PATH_VAR);
                    pMutexDump->unlock();

                   delete[] pGreenColorArray;
                }

                LambdaIPCThresholdSend(threadIndex, 1.0f);

                thrStatus[threadIndex] = false;
            };

            for (uint16_t i = 0U; i < Thread_count; ++i)
            {
                threadsStatus[i] = true;

                threads[i] = std::thread(LambdaThread, i, threadsStatus, &paths, SIMULATION_STEPS_THRESHOLD, &mtxAnt, &mtxDumpFile, &mtxDraw, &windowParallelAnt);
                threads[i].detach();
            }

            bool IsAllDetached = false;
            while ( !IsAllDetached )
            {
                if (windowParallelAnt.pollEvent(eventParallelAnt))
                {
                    switch (eventParallelAnt.type)
                    {
                        case sf::Event::Closed: 
                        {
                            windowParallelAnt.close();
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

                stop = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);

                IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE,  da::GUIAntParallel::CURRENT_TIME, std::to_string(duration.count()) + "s ");

                parallelGUI.FetchDataForGUI(Thread_count * 2U);
                parallelGUI.Redraw(daConstants::NO_CLEAR_SCREEN, daConstants::PUSH_TO_SCREEN);

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            bool exit = false;
            while (!exit)
            {
                if (windowParallelAnt.pollEvent(eventParallelAnt))
                {
                    switch (eventParallelAnt.type)
                    {
                    case sf::Event::KeyPressed:
                    {
                        exit = true;
                        continue;
                    }break;
                    }
                }
            }

            delete[] threads;
            delete[] threadsStatus;

        }break;

        case da::ANT_NOGUI_LARGE_FILE:
        {
            if (!da::OsFeatures::IsEnoughFreeMemory(MESH_WIDTH, MESH_HEIGHT, daConstants::SIZE_OF_MASKED_COLOR)) break;

            auto start = std::chrono::high_resolution_clock::now();
     
            daTypes::GreenColor* pGreenColor = da::InputParser::CreateDaGreenColorArrayFromCL(ANT_PATH_FROM_CL);

            da::AntMega megaAnt(MESH_WIDTH, MESH_HEIGHT, ANT_PATH_FROM_CL, pGreenColor);

            std::atomic_uint64_t progress = 0U;
            std::atomic_uint64_t antMoves = 0U;
            std::atomic_bool exit = false;
            uint64_t megaAntRenderStepCount = 50000000U;

            auto GUIThread = []( 
                std::atomic_uint64_t* pProgress, 
                std::atomic_uint64_t* pAntMoves, 
                std::atomic_bool* pExit)
                {
                    using namespace std::chrono_literals;
                    auto start = std::chrono::high_resolution_clock::now();

                    da::GUIAntMega AntMegaGUI( 1000U, ANT_PATH_FROM_CL);///to do: get monitor resolution
                    
                    AntMegaGUI.UpdateText(da::GUIAntMega::INFO, std::string(" Mesh size: ") + std::to_string(MESH_WIDTH) + "x" + std::to_string(MESH_HEIGHT));
                    AntMegaGUI.Redraw(daConstants::CLEAR_SCREEN, daConstants::PUSH_TO_SCREEN);

                    sf::Event eventGUI;
                    bool exitLoop = false;

                    while ( !exitLoop )
                    {
                    if (AntMegaGUI.GetWindowPtr()->pollEvent(eventGUI))
                    {
                        switch (eventGUI.type)  
                        {
                            case sf::Event::KeyPressed:
                            {
                                switch (eventGUI.key.code)
                                {
                                case sf::Keyboard::Enter:
                                {
                                    *pExit = true;
                                }break;
                                case sf::Keyboard::Escape:
                                {
                                    if ( *pExit )
                                    {
                                        exitLoop = true;
                                    }  
                                }break;
                                default:
                                    break;
                                }
                            }break;
                        }
                    }

                    AntMegaGUI.FetchDataForGUI( 5U );

                    if ( AntMegaGUI.GetState() == da::GUIAntMega::States::GENERATING)
                    {
                        auto timeStamp = std::chrono::high_resolution_clock::now();
                        auto cycleTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeStamp - start);

                        uint64_t pxPerSec = *pAntMoves / (cycleTime.count() + 1U) * 1000U;
                        AntMegaGUI.setPxPerS(pxPerSec);

                        std::string formatNumber(std::to_string(*pAntMoves));

                        daFunctions::AddCommasToStringNumber(formatNumber);

                        AntMegaGUI.UpdateTextAfter(da::GUIAntMega::MOVES, 8U, formatNumber);

                        float thresholdPercent = static_cast<float>(*pProgress) / SIMULATION_STEPS_THRESHOLD;
                        char temp[10];
                        snprintf(temp, sizeof(temp), "%3.0f", thresholdPercent * 100.0f);

                        AntMegaGUI.UpdateTextAfter(da::GUIAntMega::THRESHOLD, 23U, std::string(temp) + "%");

                        AntMegaGUI.m_ThreasholdBar.SetProgress(thresholdPercent, 1.0f);
                    }
                        
                    AntMegaGUI.Redraw(daConstants::NO_CLEAR_SCREEN, daConstants::PUSH_TO_SCREEN);
                    std::this_thread::sleep_for(33ms);
                    }
                };

            std::thread threadGUI = std::thread(GUIThread, &progress, &antMoves, &exit);

            while (!exit)
            {
                uint64_t movesLeft = megaAnt.NextMove(megaAntRenderStepCount);
                if (movesLeft == 0)
                {
                    antMoves += megaAntRenderStepCount;
                }
                else
                {
                    antMoves += movesLeft;

                    std::string formatedMoves = std::to_string(antMoves);
                    daFunctions::AddCommasToStringNumber(formatedMoves);

                    IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, da::GUIAntMega::INFO, std::string(" Ant hit wall, moves: " + formatedMoves));
                    IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, da::GUIAntMega::MOVES, formatedMoves);
                    break;
                }
                 
                ++progress;
                if (progress == SIMULATION_STEPS_THRESHOLD)
                {
                    std::string formatedMoves = std::to_string(antMoves);

                    daFunctions::AddCommasToStringNumber(formatedMoves);

                    IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, da::GUIAntMega::INFO, std::string(" Reached simulation limit, moves: " + formatedMoves));
                    IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, da::GUIAntMega::MOVES, formatedMoves);
                    IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, da::GUIAntMega::THRESHOLD, std::string("100%"));

                    float thresholdFloat = 100.0f;
                    IPC::SendMessege(IPC::GUI_MESSAGE_VALUE_UPDATE, da::GUIAntMega::THRESHOLD_PROGRESSBAR_UPDATE, static_cast<const void*>(&thresholdFloat), sizeof(float));

                    break;
                }
            }

            megaAnt.DumpToFile(OUTPUT_PATH_VAR);

            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

            IPC::SendMessege(IPC::GUI_MESSAGE_TEXT_UPDATE, da::GUIAntMega::INFO, std::string(" Whole operation took: " + std::to_string(duration.count()) + "[ms]"));

            threadGUI.join();

            delete[] pGreenColor;
        }break;

        default: 
        {
            std::cout << "Unknown argument: " + std::string(argv[1]) << std::endl;
        } break;
    }

	return 0;
}