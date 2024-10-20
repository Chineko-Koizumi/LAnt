#include "Mesh.h"               // field for ant and ant itself
#include "FileParser.h"         // main function argument parrser
#include "WindowsFeatures.h"    // for Windows winapi features
#include "DrawingAppConstants.h"

#include <fstream>
#include <utility>

static uint32_t WINDOW_WIDTH                = 0;
static uint32_t WINDOW_HEIGHT               = 0;
static uint64_t SIMULATION_STEPS_THRESHOLD  = 0;
static uint8_t GENERATION_TYPE              = 0;

static std::string ANT_PATHS_FILE_PATH("EMPTY");
static std::string ANT_PATH_FROM_CL("EMPTY");

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
        ANT_GUI                 = 0,
        ANT_PARALLEL_FILE       = 1,
        ANT_NOGUI_LARGE_FILE    = 2,
        EXIT = 100
    };
}

int main(int argc, char* argv[])
{
    if (!strcmp(argv[1], "-CLG")) //generete from command line
    {
        WINDOW_WIDTH                = atoi(argv[2]);
        WINDOW_HEIGHT               = atoi(argv[3]);
        SIMULATION_STEPS_THRESHOLD  = atoi(argv[4]);
        ANT_PATH_FROM_CL            = std::string(argv[5]);

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

#pragma region Window
    
    switch (GENERATION_TYPE)
    {
        case da::ANT_GUI:
        {
            if ( !da::WindowsFeatures::IsEnoughFreeMemory(WINDOW_WIDTH, WINDOW_HEIGHT, da::SIZE_OF_VERTEX) ) break;
            
            sf::Event eventGui; // for windows event pool
            sf::RenderWindow windowGUI(sf::VideoMode(WINDOW_WIDTH/2U, WINDOW_HEIGHT/3U), "GUI", sf::Style::Resize);
            windowGUI.setPosition(sf::Vector2i(0, 0));

            sf::Event eventAnt; // for windows event pool
            sf::RenderWindow windowAnt(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Langton's Ant", sf::Style::None);

            sf::Font font;
            font.loadFromFile("arial.ttf");

            sf::Text GUItexts[3];
            GUItexts[0].setString(ANT_PATH_FROM_CL.c_str());
            GUItexts[1].setString(std::to_string( da::KeyboardMethods::m_RenderStepCount ));

            GUItexts[0].setCharacterSize(24);
            GUItexts[1].setCharacterSize(24);

            GUItexts[0].setFillColor(sf::Color::Red);
            GUItexts[1].setFillColor(sf::Color::Red);

            GUItexts[0].setPosition( sf::Vector2f(0.0f, 0.0f) );
            GUItexts[1].setPosition( sf::Vector2f(0.0f, 40.0f) );

            GUItexts[0].setStyle(sf::Text::Bold);
            GUItexts[1].setStyle(sf::Text::Bold);

            GUItexts[0].setFont(font);
            GUItexts[1].setFont(font);

            sf::Color* colors = da::FileParser::CreateColorArrayFromCL(ANT_PATH_FROM_CL);
            da::Ant ant(&windowAnt, 0, colors, WINDOW_WIDTH, WINDOW_HEIGHT, ANT_PATH_FROM_CL);

            windowAnt.setActive(true);

            windowGUI.setActive(true);

            windowGUI.draw(GUItexts[0]);
            windowGUI.draw(GUItexts[1]);
            windowGUI.display();

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
                            ant.DumpToFile();
                            exit = true;
                            continue;
                        }break;
                        case sf::Keyboard::Right:
                        {
                            da::KeyboardMethods::SpeedUpRender();
                            GUItexts[1].setString(std::to_string(da::KeyboardMethods::m_RenderStepCount));

                            windowGUI.clear(sf::Color::Black);
                            windowGUI.draw(GUItexts[1]);
                            windowGUI.display();

                        }break;
                        case sf::Keyboard::Left:
                        {
                            da::KeyboardMethods::SpeedDownRender();
                            GUItexts[1].setString(std::to_string(da::KeyboardMethods::m_RenderStepCount));

                            windowGUI.clear(sf::Color::Black);
                            windowGUI.draw(GUItexts[1]);
                            windowGUI.display();

                        }break;
                        }
                    }break;
                    }
                }

                ant.DrawMesh();

                windowAnt.display();
                

                if (!ant.NextMove(da::KeyboardMethods::m_RenderStepCount))
                {
                    ant.DumpToFile();
                    break;
                }
            }
            delete[] colors;

        }break;

        case da::ANT_PARALLEL_FILE: 
        {
            if (!da::WindowsFeatures::IsEnoughFreeMemory(WINDOW_WIDTH, WINDOW_HEIGHT, da::SIZE_OF_VERTEX)) break;

            std::mutex mtxCout, mtxDumpFile;
            std::thread* threads;
            bool* threadsStatus;

            uint16_t Thread_count = da::WindowsFeatures::GetThreadCountForMeshSize(WINDOW_WIDTH, WINDOW_HEIGHT);

            threads = new std::thread[Thread_count];
            threadsStatus = new bool[Thread_count];

            sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Langton's Ant", sf::Style::Default);
            window.setActive(false);

            da::WindowsFeatures::InitTerminalForThreads(Thread_count);

            std::vector< std::pair<std::string, sf::Color* > > paths;

            std::string line;
            while (!infile.eof())
            {
                std::getline(infile, line);

                sf::Color* pColor = da::FileParser::CreateColorArrayFromCL(line);
                if (pColor != nullptr) 
                {
                    paths.push_back({ line, pColor });
                }
            }
            
            auto LambdaThread = [](
                uint16_t threadIndex, 
                uint16_t threadMax,
                bool* thrStatus, 
                std::vector< std::pair<std::string, sf::Color* >>* vectorPaths, 
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

                        sf::Color* colors = vectorPaths->back().second;

                        da::Ant ant(w, threadIndex, colors, WINDOW_WIDTH, WINDOW_HEIGHT, vectorPaths->back().first);

                        vectorPaths->pop_back();
                    mc->unlock();

                    LambdaRenderStepCount = 10000000;
                    Progress = 0;
                    while (LambdaRenderStepCount != 0)
                    {
                        if (Progress > SimulationStepsThresholdFromArgument)
                        {
                            md->lock();
                                ant.DumpToFile();
                            md->unlock();
                            break;
                        }
                        mc->lock();

                            std::string ThreadText(6, ' ');
                            ThreadText.replace(0, sThreadID.length(), sThreadID);

                            Output.str("");
                            Output<<da::WindowsFeatures::GenerateProgressBar((float(Progress) / SIMULATION_STEPS_THRESHOLD), 28) << " Thread: [" << ThreadText<<"] File number: " << fileNumer;
                            da::WindowsFeatures::ThreadProgressGeneretor(threadIndex, Output.str());
                        
                       mc->unlock();

                        if (!ant.NextMove(LambdaRenderStepCount)) 
                        {
                            md->lock();
                                ant.DumpToFile();
                            md->unlock();
                            break;
                        }
                        Progress++;
                    }
                    delete[]colors;
                    fileNumer++;
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
             if (!da::WindowsFeatures::IsEnoughFreeMemory(WINDOW_WIDTH, WINDOW_HEIGHT, da::SIZE_OF_MASKED_COLOR)) break;

             auto start = std::chrono::high_resolution_clock::now();

             std::cout << std::endl << std::endl << std::endl;//new lines as place for ant moves and progress bar;

            
             da::GreenColor* daGreenColors = da::FileParser::CreateDaGreenColorArray(ANT_PATH_FROM_CL); // parsed colors for mesh from arguments
     
             uint64_t Progress = 0U;

             uint8_t* ColorMaskedTransitionArray = (uint8_t*)_alloca(ANT_PATH_FROM_CL.size());

             da::MegaAnt megaAnt(&da::KeyboardMethods::m_RenderStepCount, 0U, WINDOW_WIDTH, WINDOW_HEIGHT, ANT_PATH_FROM_CL, daGreenColors, ColorMaskedTransitionArray, ANT_PATH_FROM_CL.size());

             da::KeyboardMethods::m_RenderStepCount = 100000000;
             while (da::KeyboardMethods::m_RenderStepCount != 0)
             {
                 if (Progress > SIMULATION_STEPS_THRESHOLD)
                 {
                     std::cout << std::endl << " " << (long double)SIMULATION_STEPS_THRESHOLD * da::KeyboardMethods::m_RenderStepCount << " moves, reached simulation limit" << std::endl;

                     megaAnt.DumpToFile();
                     da::KeyboardMethods::m_RenderStepCount = 0;
                     break;
                 }
                
                 da::WindowsFeatures::AntMovesAndProgressBar( Progress * da::KeyboardMethods::m_RenderStepCount, (float(Progress) / SIMULATION_STEPS_THRESHOLD), 28 );

                 if (!megaAnt.NextMove(da::KeyboardMethods::m_RenderStepCount)) 
                 {
                     megaAnt.DumpToFile();
                     break;
                 }

                 ++Progress;
             }
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