
#include "Mesh.hpp"                 // Field for ant and ant itself
#include "Ant.hpp"                  // Ant backend
#include "AntMega.hpp"              // Ant Mega backend
#include "GUIAnt.hpp"               // GUIAnt with seperate window
#include "GUIAntMega.hpp"           // GUIAntMega with seperate window
#include "InputParser.hpp"          // Main function argument parrser
#include "WindowsFeatures.hpp"      // Windows winapi features
#include "IPC.hpp"

#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>      // Getting path data from file
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */
#include <queue>

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
            windowGUI.UpdateText(da::GUIAnt::PATH,          ANT_PATH_FROM_CL);
            windowGUI.UpdateText(da::GUIAnt::SPEED,         std::string("Speed: " + std::to_string(da::KeyboardMethods::m_RenderStepCount)));
            windowGUI.UpdateText(da::GUIAnt::MOVES,         std::string("Moves: " + std::to_string(0U)));
            windowGUI.UpdateText(da::GUIAnt::EXIT_INFO,     std::string("\n"));
            windowGUI.Redraw();

            sf::Event eventAnt; // for windowAnt event pool and GUI window
            sf::RenderWindow windowAnt(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Langton's Ant", sf::Style::None);

            daTypes::GreenColor* greenColors = da::InputParser::CreateDaGreenColorArrayFromCL(ANT_PATH_FROM_CL);
            da::Ant ant(&windowAnt, greenColors, WINDOW_WIDTH, WINDOW_HEIGHT, ANT_PATH_FROM_CL);

            windowAnt.setActive(true);


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
                            windowGUI.Redraw();
                            exit = true;
                            continue;
                        }break;
                        case sf::Keyboard::Right:
                        {
                            da::KeyboardMethods::SpeedUpRender();
                            windowGUI.UpdateTextAfter(da::GUIAnt::SPEED, 8U, std::to_string(da::KeyboardMethods::m_RenderStepCount));
                            windowGUI.Redraw();
                        }break;
                        case sf::Keyboard::Left:
                        {
                            da::KeyboardMethods::SpeedDownRender();
                            windowGUI.UpdateTextAfter(da::GUIAnt::SPEED, 8U, std::to_string(da::KeyboardMethods::m_RenderStepCount));
                            windowGUI.Redraw();
                        }break;
                        }
                    }break;
                    default: break;
                    }
                }

                uint32_t movesLeft = ant.NextMove(da::KeyboardMethods::m_RenderStepCount);
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
                    windowGUI.Redraw();
                    break;
                } 

                ant.DrawMesh();
                windowGUI.Redraw();
            }

            ant.DrawMesh();
            ant.DumpToFile(OUTPUT_PATH_VAR);
            delete[] greenColors;

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
                bool* thrStatus, 
                std::vector< std::pair<std::string, daTypes::GreenColor* >>* pVectorPaths,
                uint64_t SimulationStepsThreshold, 
                std::mutex* pMutexAnt, 
                std::mutex* pMutexDump, 
                sf::RenderWindow* pWindow)
            {
                std::thread::id thread_id = std::this_thread::get_id();

                std::stringstream Output;
                Output << thread_id;
                std::string sThreadID = Output.str();

                pMutexAnt->lock();
                    da::WindowsFeatures::ThreadProgressGeneretor(threadIndex, std::string(" Entering Thread : [" + sThreadID + "]"));
                pMutexAnt->unlock();
              
                uint16_t fileNumer = 0U;
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

                        daTypes::GreenColor* pGreenColor = pVectorPaths->back().second;
                        da::Ant ant(pWindow, pGreenColor, WINDOW_WIDTH, WINDOW_HEIGHT, pVectorPaths->back().first);

                        pVectorPaths->pop_back();
                    pMutexAnt->unlock();

                    lambdaRenderStepCount = 10000000U;
                    progress = 0U;
                    while (true)
                    {
                        if (progress > SimulationStepsThreshold) break;
                        
                        pMutexAnt->lock();

                            std::string threadText(6U, ' ');
                            threadText.replace(0U, sThreadID.length(), sThreadID);

                            Output.str("");
                            Output<<da::WindowsFeatures::GenerateProgressBar((float(progress) / SIMULATION_STEPS_THRESHOLD), 28) << " Thread: [" << threadText<<"] File number: " << fileNumer;
                            da::WindowsFeatures::ThreadProgressGeneretor(threadIndex, Output.str());
                        
                       pMutexAnt->unlock();

                        uint32_t movesLeft = ant.NextMove(lambdaRenderStepCount);
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

                    pMutexDump->lock();
                        ant.DumpToFile(OUTPUT_PATH_VAR);
                    pMutexDump->unlock();

                    fileNumer++;

                    if (pGreenColor != nullptr) 
                    {
                        delete[] pGreenColor;
                        pGreenColor = nullptr;
                    }
                }
                pMutexAnt->lock();

                    std::string threadText(6U, ' ');
                    threadText.replace(0, sThreadID.length(), sThreadID);
                    
                    da::WindowsFeatures::ThreadProgressGeneretor(threadIndex, std::string(" Thread: [" + threadText + "] Done generating                                            "));

                pMutexAnt->unlock();   

                thrStatus[threadIndex] = false;
            };

            for (uint16_t i = 0U; i < Thread_count; ++i)
            {
                threadsStatus[i] = true;

                threads[i] = std::thread(LambdaThread, i, threadsStatus, &paths, SIMULATION_STEPS_THRESHOLD, &mtxCout, &mtxDumpFile, &window);
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

             daTypes::GreenColor* daGreenColors = da::InputParser::CreateDaGreenColorArrayFromCL(ANT_PATH_FROM_CL); // parsed colors for mesh from arguments
     
             da::AntMega megaAnt(WINDOW_WIDTH, WINDOW_HEIGHT, ANT_PATH_FROM_CL, daGreenColors, ANT_PATH_FROM_CL.size());

             std::atomic_uint64_t progress = 0U;
             std::atomic_uint64_t antMoves = 0U;
             std::atomic_bool exit = false;
             uint64_t megaAntRenderStepCount = 50000000U;

             auto LambdaThread = []( 
                 std::atomic_uint64_t* pProgress, 
                 std::atomic_uint64_t* pAntMoves, 
                 std::atomic_bool* pExit)
                 {
                     using namespace std::chrono_literals;
                     auto start = std::chrono::high_resolution_clock::now();

                     da::GUIAntMega AntMegaGUI(1280U, 960U, ANT_PATH_FROM_CL);///@to_do get monitor resolution

                     AntMegaGUI.UpdateText(da::GUIAntMega::MOVES, std::string(" Moves: 0"));
                     AntMegaGUI.UpdateText(da::GUIAntMega::THRESHOLD, std::string(" Simulation threshold:   0%"));
                     AntMegaGUI.UpdateText(da::GUIAntMega::INFO, std::string(" Mesh size: ") + std::to_string(WINDOW_WIDTH) + "x" + std::to_string(WINDOW_HEIGHT));
                     AntMegaGUI.SetProgressThreshold(0.0f);
                     AntMegaGUI.Redraw();

                     bool isMouseDragging = false;
                     sf::Event eventGUI;

                     int16_t lastDownX = 0;
                     int16_t lastDownY = 0;
                     while (!*pExit)
                     {
                        if (AntMegaGUI.GetWindowPtr()->pollEvent(eventGUI))
                        {
                            switch (eventGUI.type)
                            {
                            case sf::Event::KeyPressed:
                            {
                                switch (eventGUI.key.code)
                                {
                                case sf::Keyboard::Escape:
                                {
                                    *pExit = true;
                                }break;
                                default:
                                    break;
                                }
                                     
                            }break;
                            
                            }
                        }

                        for (uint8_t i = 0U; ! IPC::_G_MSG_Queue.empty() && i < 5U; ++i )
                        {
                            IPC::Message msg;

                            IPC::_G_MSG_Mutex.lock();

                                msg = IPC::_G_MSG_Queue.front();
                                IPC::_G_MSG_Queue.pop();

                            IPC::_G_MSG_Mutex.unlock();
                            
                            switch (msg.messageType)
                            {
                                case IPC::messageType::GUI_MESSAGE_TEXT_UPDATE: 
                                {
                                    IPC::GUIMessageTextUpdate* updateMsg = reinterpret_cast<IPC::GUIMessageTextUpdate*>(&msg);

                                    switch (updateMsg->textName)
                                    {
                                        case da::GUIAntMega::INFO:
                                        {
                                            AntMegaGUI.AppendText(
                                                updateMsg->textName,
                                                reinterpret_cast<char*>(&updateMsg->message)  /* adress of second byte is start of new string for Text*/);
                                        }break;
                                        default:
                                        {
                                            AntMegaGUI.UpdateText(
                                                updateMsg->textName,
                                                reinterpret_cast<char*>(&updateMsg->message)  /* adress of second byte is start of new string for Text*/);
                                        }break;
                                    }

                                }break;
                                case IPC::messageType::GUI_MESSAGE_VALUE_UPDATE:
                                {
                                    IPC::GUIMessageValueUpdate* updateMsg = reinterpret_cast<IPC::GUIMessageValueUpdate*>(&msg);

                                    switch (updateMsg->valueName)
                                    {
                                        case IPC::GUIData::PROGRESSBAR_UPDATE:
                                        {
                                            AntMegaGUI.SetProgressCopy(*reinterpret_cast<float*>(updateMsg->message));
                                        }break;
                                        case IPC::GUIData::COPY_WINDOW_UPDATE:
                                        {
                                            AntMegaGUI.SetCopyStarted(*reinterpret_cast<bool*>(updateMsg->message));
                                        }break;
                                        default:
                                            break;
                                    }

                                }break;
                                default: 
                                {
                                    IPC::_G_MSG_Mutex.lock();
                                        IPC::_G_MSG_Queue.push(msg);
                                    IPC::_G_MSG_Mutex.unlock();

                                    i = 10U; /// THIS IS LOOP VALUE TO GET OUT OF IT!!!
                                    continue;
                                }break;
                            }
                        }

                        if ( !AntMegaGUI.IsCopyStarted() )
                        {
                            auto timeStamp = std::chrono::high_resolution_clock::now();
                            auto cycleTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeStamp - start);

                            uint64_t pxPerSec = *pAntMoves / (cycleTime.count() + 1U) * 1000U;
                            AntMegaGUI.setPxPerS(pxPerSec);

                            float thresholdPercent = static_cast<float>(*pProgress) / SIMULATION_STEPS_THRESHOLD;
                            AntMegaGUI.UpdateTextAfter(da::GUIAntMega::MOVES, 8U, std::to_string(*pAntMoves));

                            char temp[10];
                            snprintf(temp, sizeof buffer, "%3.0f", thresholdPercent * 100.0f);
                            AntMegaGUI.UpdateTextAfter(da::GUIAntMega::THRESHOLD, 23U, std::string(temp) + "%");
                            AntMegaGUI.SetProgressThreshold(thresholdPercent);
                        }

                        AntMegaGUI.Redraw();
                        std::this_thread::sleep_for(33ms);
                     }
                 };

             std::thread threadGUI = std::thread(LambdaThread, &progress, &antMoves, &exit);

             while (!exit)
             {
                 uint32_t movesLeft = megaAnt.NextMove(megaAntRenderStepCount);
                 if (movesLeft == 0)
                 {
                     antMoves += megaAntRenderStepCount;
                 }
                 else
                 {
                     antMoves += movesLeft;
                     break;
                 }
                 
                 if (progress == SIMULATION_STEPS_THRESHOLD)
                 {
                     std::string msgS(" Reached simulation limit, moves: " + std::to_string(antMoves));

                     IPC::GUIMessageTextUpdate updateMsg;
                     updateMsg.textName     = da::GUIAntMega::INFO;

                     memcpy_s(updateMsg.message, msgS.size() + 1U, msgS.c_str(), msgS.size() + 1U);

                     IPC::_G_MSG_Mutex.lock();

                        IPC::_G_MSG_Queue.push(*reinterpret_cast<IPC::Message*>(&updateMsg));

                     IPC::_G_MSG_Mutex.unlock();

                     break;
                 }
                 ++progress;
             }

             megaAnt.DumpToFile(OUTPUT_PATH_VAR);

             auto stop = std::chrono::high_resolution_clock::now();
             auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

             std::string msgS(" Whole operation took: " + std::to_string(duration.count()) + "[ms]");

             IPC::GUIMessageTextUpdate updateMsg;
             updateMsg.textName     = da::GUIAntMega::INFO;

             memcpy_s(updateMsg.message, msgS.size() + 1U, msgS.c_str(), msgS.size() + 1U);

             IPC::_G_MSG_Mutex.lock();

                IPC::_G_MSG_Queue.push(*reinterpret_cast<IPC::Message*>(&updateMsg));

             IPC::_G_MSG_Mutex.unlock();

             delete[] daGreenColors;
             threadGUI.join();
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