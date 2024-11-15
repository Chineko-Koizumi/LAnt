
#include "GUIAntParallel.hpp"
#include "IPC.hpp"

namespace da
{
	GUIAntParallel::Names& operator++ (GUIAntParallel::Names& name)
	{
		uint16_t temp = name;
		name = static_cast<GUIAntParallel::Names>(++temp);

		return name;
	}

	GUIAntParallel::GUIAntParallel(uint32_t windowWidth, uint32_t windowHeight, uint16_t threadCount)
		: GUIBase(windowWidth / 2.0f, windowHeight/ 2.0f, Names::LAST)
		, m_ThreadCount(threadCount)
		, m_aThreadProgressBars(new ProgressBar[threadCount])
		, m_PathsCount(0U)
		, m_PathsGenerated(0U)
	{
		InitBackground();
		InitBars();
		InitText();
	}

	GUIAntParallel::~GUIAntParallel()
	{
		delete[] m_aThreadProgressBars;
		m_aThreadProgressBars = nullptr;
	}

	void GUIAntParallel::SetPathsCount(uint16_t pathsCount)
	{
		m_PathsCount = pathsCount;
	}

	void GUIAntParallel::FetchDataForGUI(uint8_t msgCountPerFetch)
	{
		IPC::Message msg;

		for (uint8_t i = 0U; i < msgCountPerFetch; ++i)
		{
			if (!IPC::GetMessage(&msg)) break;

			switch (msg.messageType)
			{
			case IPC::messageType::GUI_MESSAGE_TEXT_UPDATE:
			{
				switch (msg.valueName)
				{
				case RECENTLY_STARTED_PATH: 
				{
					UpdateTextAfter(msg.valueName, 15U, reinterpret_cast<char*>(msg.message));
				}break;

				case CURRENT_TIME: 
				{
					UpdateTextAfter(msg.valueName, 15U, std::string(reinterpret_cast<char*>(msg.message)) );
				}break;

				case CURRENT_PATH_STATUS:
				{
					UpdateText(msg.valueName, std::to_string(m_PathsGenerated) + "/" + std::to_string(m_PathsCount));
				}break;

				default:
				{
					UpdateText( msg.valueName, reinterpret_cast<char*>(msg.message));
				}break;
				}

			}break;
			case IPC::messageType::GUI_MESSAGE_VALUE_UPDATE:
			{
				switch (msg.valueName)
				{
				case PATHS_PROGRESSBAR_UPDATE:
				{
					if(m_PathsGenerated < m_PathsCount) ++m_PathsGenerated;
					m_PathsProgress.SetProgress(static_cast<float>( m_PathsGenerated) / m_PathsCount, 1.0f);
				}break;

				case THREADS_PROGRESSBAR_UPDATE: 
				{
					m_aThreadProgressBars[*reinterpret_cast<uint16_t*>(msg.message)].SetProgress(*reinterpret_cast<float*>(&msg.message[2]), 1.0f);
				}break;

				default:
					break;
				}

			}break;
			default:
			{
				IPC::SendMessege(&msg);
			}break;
			}
		}
	}

	void GUIAntParallel::Redraw(bool clearScreen, bool pushToScreen)
	{
		if (clearScreen) m_pWindow->clear(sf::Color::Black);

			m_pWindow->draw(m_Background.sprite);

			m_PathsProgress.renderTexture.draw(m_PathsProgress.outsideSprite);
			m_PathsProgress.renderTexture.draw(m_PathsProgress.insideSprite);
			m_PathsProgress.renderTexture.display();

			sf::Sprite outputThresholdSprite(m_PathsProgress.renderTexture.getTexture());

			m_pWindow->draw(outputThresholdSprite);

			for (Names name = FIRST; name < LAST; ++name)
			{
				m_pWindow->draw(m_pGUITexts[name]);
			}

			for (size_t i = 0; i < m_ThreadCount; i++)
			{
				m_aThreadProgressBars[i].renderTexture.draw(m_aThreadProgressBars[i].outsideSprite);
				m_aThreadProgressBars[i].renderTexture.draw(m_aThreadProgressBars[i].insideSprite);
				m_aThreadProgressBars[i].renderTexture.display();

				sf::Sprite outputThresholdSprite(m_aThreadProgressBars[i].renderTexture.getTexture());

				m_pWindow->draw(outputThresholdSprite);
			}

		if (pushToScreen) m_pWindow->display();
	}

	void GUIAntParallel::InitBackground()
	{
		m_Background.texture.loadFromFile("./Sprites/GUI/AntParallel/Background.png");
		m_Background.sprite.setTexture(m_Background.texture);

		float scalingFactor = static_cast<float>(m_WindowWidth) / m_Background.texture.getSize().x;
		m_Background.sprite.setScale(scalingFactor, scalingFactor);
		m_Background.sprite.setScale(scalingFactor, scalingFactor);
	}

	void GUIAntParallel::InitBars()
	{
	
		m_PathsProgress.Init(GUIBase::m_WindowWidth, GUIBase::m_WindowHeight, "./Sprites/GUI/ProgressBarInside.png", "./Sprites/GUI/ProgressBarOutside.png",
			0.963f,
			0.33f);

		m_PathsProgress.SetPossition(
			0.0175f, 
			0.125f,
			0.0175f, 
			0.125f);

		m_PathsProgress.SetProgress(0.0f, 1.0f);


		for (size_t i = 0; i < m_ThreadCount; i++)
		{
			m_aThreadProgressBars[i].Init(GUIBase::m_WindowWidth, GUIBase::m_WindowHeight, "./Sprites/GUI/ProgressBarInside.png", "./Sprites/GUI/ProgressBarOutside.png",
				0.971f,
				0.38f);

			float barRelativeHeight = static_cast<float>(m_aThreadProgressBars[i].progressBarHeight * m_aThreadProgressBars[i].insideSprite.getScale().y) / GUIBase::m_WindowWidth;
			barRelativeHeight *= 1.15; //10% margin

			m_aThreadProgressBars[i].SetPossition(
				0.0150f,
				0.173f + barRelativeHeight * i,
				0.0150f,
				0.173f + barRelativeHeight * i);

			m_aThreadProgressBars[i].SetProgress(0.0f, 1.0f);
		}
	}

	void GUIAntParallel::InitText()
	{
		float windowSpacing = static_cast<float>(m_WindowHeight) / 30U;
		for (Names name = FIRST; name < LAST; ++name)
		{
			m_pGUITexts[name].setFont(m_FontTahoma);
			m_pGUITexts[name].setFillColor(m_ColorWindowsBlack);
			m_pGUITexts[name].setOutlineColor(m_ColorWindowsLightBlack);
			m_pGUITexts[name].setString("");
			m_pGUITextsStrings[name] = std::string("");
			m_pGUITexts[name].setCharacterSize(static_cast<unsigned int>(windowSpacing));
			m_pGUITexts[name].setLineSpacing(1.1f);
			m_pGUITexts[name].setPosition(m_aTextsPos[name]);
		}

		m_pGUITexts[TITLE].setFont(m_FontTahomaBold);
		m_pGUITexts[TITLE].setFillColor(sf::Color::White);
		m_pGUITexts[TITLE].setCharacterSize(static_cast<unsigned int>(m_pGUITexts[TITLE].getCharacterSize() * 0.70f));
	}
}