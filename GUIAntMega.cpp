#include "GUIAntMega.hpp"
#include "IPC.hpp"

namespace da 
{
	GUIAntMega::Names& operator++ (GUIAntMega::Names& name)
	{
		uint8_t temp = name;
		name = static_cast<GUIAntMega::Names>(++temp);

		return name;
	}

	GUIAntMega::GUIAntMega(uint32_t windowWidth, uint32_t windowHeight, std::string& path)
		: GUIBase(windowWidth, windowHeight, LAST)
		, m_CopyStarted(false)
		, m_MaxPxPerSec(0U)
		, m_CopyAnimation(26U, 50U, static_cast<float>(m_WindowWidth) * 0.370f, static_cast<float>(m_WindowHeight) * 0.435f, 1.75f, "./Sprites/GUI/AntMega/CopyAnimation.png")
	{
		m_pWindow->setTitle("Ant MegaPath");

		InitBackground();
		InitThreasholdBar();
		InitCopyBar();
		InitText();

		UpdateText(PATH, path);
	}

	GUIAntMega::~GUIAntMega()
	{
	}

	void GUIAntMega::InitBackground()
	{
		m_BackgroundTexture.loadFromFile("./Sprites/GUI/AntMega/Background.png");
		m_BackgroundSprite.setTexture(m_BackgroundTexture);

		float scalingFactor = static_cast<float>(m_WindowWidth) / m_BackgroundTexture.getSize().x;
		m_BackgroundSprite.setScale(scalingFactor, scalingFactor);
		m_BackgroundSprite.setScale(scalingFactor, scalingFactor);
	}

	void GUIAntMega::InitThreasholdBar()
	{
		if (!m_ThreasholdBar.m_ProgressBarOutsideTexture.loadFromFile("./Sprites/GUI/AntMega/ProgressBarOutside.png"))	assert(false);
		if (!m_ThreasholdBar.m_ProgressBarInsideTexture.loadFromFile("./Sprites/GUI/AntMega/ProgressBarInside.png"))	assert(false);

		m_ThreasholdBar.m_ProgressBarWidth = m_ThreasholdBar.m_ProgressBarOutsideTexture.getSize().x;
		m_ThreasholdBar.m_ProgressBarHeight = m_ThreasholdBar.m_ProgressBarOutsideTexture.getSize().y;

		m_ThreasholdBar.m_ProgressBarOutsideTexture.setSmooth(true);
		m_ThreasholdBar.m_ProgressBarInsideTexture.setSmooth(true);

		m_ThreasholdBar.m_ProgressBarOutsideSprite.setTexture(m_ThreasholdBar.m_ProgressBarOutsideTexture);
		m_ThreasholdBar.m_ProgressBarInsideSprite.setTexture(m_ThreasholdBar.m_ProgressBarInsideTexture);

		float scalingFactor = static_cast<float>(m_WindowWidth) / m_ThreasholdBar.m_ProgressBarWidth;
		scalingFactor *= 0.33f;

		m_ThreasholdBar.m_ProgressBarOutsideSprite.setScale(scalingFactor * 1.53f, scalingFactor);
		m_ThreasholdBar.m_ProgressBarInsideSprite.setScale(scalingFactor * 1.53f, scalingFactor);

		m_ThreasholdBar.m_ProgressBarOutsideSprite.setPosition(	static_cast<float>(m_WindowWidth) * 0.069f, static_cast<float>(m_WindowHeight) * 0.211f);
		m_ThreasholdBar.m_ProgressBarInsideSprite.setPosition(	static_cast<float>(m_WindowWidth) * 0.069f, static_cast<float>(m_WindowHeight) * 0.211f);

		m_ThreasholdBar.m_ProgressBarRenderTexture.create(m_WindowWidth, m_WindowHeight);
	}

	void GUIAntMega::InitCopyBar()
	{
		if (!m_CopyBar.m_ProgressBarOutsideTexture.loadFromFile("./Sprites/GUI/AntMega/CopingFileBackground.png"))	assert(false);
		if (!m_CopyBar.m_ProgressBarInsideTexture.loadFromFile("./Sprites/GUI/AntMega/CopingProgressBar.png"))		assert(false);

		m_CopyBar.m_ProgressBarWidth = m_CopyBar.m_ProgressBarOutsideTexture.getSize().x;
		m_CopyBar.m_ProgressBarHeight = m_CopyBar.m_ProgressBarOutsideTexture.getSize().y;

		m_CopyBar.m_ProgressBarOutsideTexture.setSmooth(false);
		m_CopyBar.m_ProgressBarInsideTexture.setSmooth(false);

		m_CopyBar.m_ProgressBarOutsideSprite.setTexture(m_CopyBar.m_ProgressBarOutsideTexture);
		m_CopyBar.m_ProgressBarInsideSprite.setTexture(m_CopyBar.m_ProgressBarInsideTexture);

		float scalingFactor = static_cast<float>(m_WindowWidth) / m_CopyBar.m_ProgressBarWidth;
		scalingFactor *= 0.50f;

		m_CopyBar.m_ProgressBarOutsideSprite.setScale(scalingFactor, scalingFactor);
		m_CopyBar.m_ProgressBarInsideSprite.setScale(scalingFactor, scalingFactor);

		m_CopyBar.m_ProgressBarOutsideSprite.setPosition(static_cast<float>(m_WindowWidth) * 0.355f, static_cast<float>(m_WindowHeight) * 0.400f);
		m_CopyBar.m_ProgressBarInsideSprite.setPosition( static_cast<float>(m_WindowWidth) * 0.375f, static_cast<float>(m_WindowHeight) * 0.400f);

		m_CopyBar.m_ProgressBarRenderTexture.create(m_WindowWidth, m_WindowHeight);
	}

	void GUIAntMega::InitText() 
	{
		float windowSpacing = static_cast<float>(m_WindowHeight) / 30U;
		for (Names name = FIRST; name < LAST; ++name)
		{
			m_pGUITexts[name].setFont(m_FontTahoma);
			m_pGUITexts[name].setFillColor(m_ColorWindowsBlack);
			m_pGUITexts[name].setOutlineColor(m_ColorWindowsLightBlack);
			m_pGUITexts[name].setString("");
			m_pGUITextsStrings[name] = std::string("");
			m_pGUITexts[name].setCharacterSize(windowSpacing);
			m_pGUITexts[name].setLineSpacing(1.1f);
			m_pGUITexts[name].setPosition(m_aTextsPos[name]);
		}

		m_pGUITexts[PATH].setFont(m_FontTahomaBold);
		m_pGUITexts[PATH].setFillColor(sf::Color::White);
		m_pGUITexts[PATH].setCharacterSize(m_pGUITexts[PATH].getCharacterSize() * 0.70f);

		m_pGUITexts[INFO].setCharacterSize(m_pGUITexts[INFO].getCharacterSize() * 0.50f);

		m_pGUITexts[GENERATING_SPEED].setCharacterSize(m_pGUITexts[GENERATING_SPEED].getCharacterSize() * 0.70f);

		m_pGUITexts[MAX_SPEED].setCharacterSize(m_pGUITexts[MAX_SPEED].getCharacterSize() * 0.70f);
		UpdateText(MAX_SPEED, std::to_string(m_MaxPxPerSec) + " px/s");

		m_pGUITexts[OUTPUT_FILE].setCharacterSize(m_pGUITexts[OUTPUT_FILE].getCharacterSize() * 0.50f);

		m_pGUITexts[SOURCE_DESTINATION].setCharacterSize(m_pGUITexts[SOURCE_DESTINATION].getCharacterSize() * 0.50f);
	}

	void GUIAntMega::SetProgressThreshold(float progressInPercent)
	{
		m_ThreasholdBar.m_ProgressBarInsideSprite.setTextureRect( sf::IntRect(
																	0, 
																	0, 
																	m_ThreasholdBar.m_ProgressBarWidth * progressInPercent,
																	m_ThreasholdBar.m_ProgressBarHeight ) );
	}

	void GUIAntMega::SetProgressCopy(float progressInPercent)
	{
		m_CopyBar.m_ProgressBarInsideSprite.setTextureRect(sf::IntRect(
																0,
																0, 
																m_CopyBar.m_ProgressBarWidth * 0.734f * progressInPercent, // shift because progress bar is asymetrical
																m_CopyBar.m_ProgressBarHeight ) );
	}

	void GUIAntMega::setPxPerS(uint64_t pxPerSec)
	{
		static uint8_t firsReadings = 0U;
		static uint8_t itt = 0U;

		if (firsReadings < PX_READINGS_SIZE)
		{
			pxReadings[firsReadings] = pxPerSec;

			UpdateText(GENERATING_SPEED, std::to_string(pxReadings[0]) + " px/s");
			++firsReadings;
			return;
		}

		switch (itt)
		{
			case 0U:	pxReadings[itt] = pxPerSec;		++itt;	break;
			case 1U:	pxReadings[itt] = pxPerSec;		++itt;	break;
			case 2U:	pxReadings[itt] = pxPerSec;		++itt;	break;
			case 3U:	pxReadings[itt] = pxPerSec;		++itt;	break;
			case 4U:	pxReadings[itt] = pxPerSec;		itt=0U;	break;
		}

		if (itt == 0U) 
		{
			uint64_t avgPxPerSec = (pxReadings[0] + pxReadings[1] + pxReadings[2] + pxReadings[3] + pxReadings[4]) / PX_READINGS_SIZE;
			if (m_MaxPxPerSec < avgPxPerSec)m_MaxPxPerSec = avgPxPerSec;

			UpdateText(GENERATING_SPEED, std::to_string(avgPxPerSec) + " px/s");
			UpdateText(MAX_SPEED, std::to_string(m_MaxPxPerSec) + " px/s");
		}
	}

	bool GUIAntMega::IsCopying()
	{
		return m_CopyStarted;
	}

	void GUIAntMega::SetCopying(bool setValue)
	{
		 m_CopyStarted = setValue;
	}

	void GUIAntMega::FetchDataForGUI(uint8_t msgCountPerFetch)
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
				case INFO:
				{
					AppendText(
						msg.valueName,
						reinterpret_cast<char*>(msg.message));
				}break;
				case MOVES:
				{
					UpdateTextAfter(
						msg.valueName,
						8U,
						reinterpret_cast<char*>(msg.message));
				}break;
				case THRESHOLD:
				{
					UpdateTextAfter(
						msg.valueName,
						23U,
						reinterpret_cast<char*>(msg.message));
				}break;
				default:
				{
					UpdateText(
						msg.valueName,
						reinterpret_cast<char*>(msg.message));
				}break;
				}

			}break;
			case IPC::messageType::GUI_MESSAGE_VALUE_UPDATE:
			{
				switch (msg.valueName)
				{
				case COPY_PROGRESSBAR_UPDATE:
				{
					SetProgressCopy(*reinterpret_cast<float*>(msg.message));
				}break;
				case COPY_WINDOW_UPDATE:
				{
					SetCopying(*reinterpret_cast<bool*>(msg.message));
				}break;
				case THRESHOLD_PROGRESSBAR_UPDATE:
				{
					SetProgressThreshold(*reinterpret_cast<float*>(msg.message));
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

	void GUIAntMega::Redraw()
	{
		m_pWindow->clear(m_ColorWindowsGreen);
	
		m_pWindow->draw(m_BackgroundSprite);

				m_ThreasholdBar.m_ProgressBarRenderTexture.draw(m_ThreasholdBar.m_ProgressBarOutsideSprite);
				m_ThreasholdBar.m_ProgressBarRenderTexture.draw(m_ThreasholdBar.m_ProgressBarInsideSprite);
				m_ThreasholdBar.m_ProgressBarRenderTexture.display();

				sf::Sprite outputThresholdSprite(m_ThreasholdBar.m_ProgressBarRenderTexture.getTexture());

			m_pWindow->draw(outputThresholdSprite);

			if (m_CopyStarted) 
			{
				m_CopyBar.m_ProgressBarRenderTexture.draw(m_CopyBar.m_ProgressBarOutsideSprite);
				m_CopyBar.m_ProgressBarRenderTexture.draw(m_CopyBar.m_ProgressBarInsideSprite);
				m_CopyBar.m_ProgressBarRenderTexture.display();

				sf::Sprite outputCopySprite(m_CopyBar.m_ProgressBarRenderTexture.getTexture());

				m_pWindow->draw(outputCopySprite);

				m_pWindow->draw(m_CopyAnimation.GetCurrentFrame());
			}

			for (Names name = FIRST; name < LAST; ++name)
			{
				m_pWindow->draw(m_pGUITexts[name]);
			}

		m_pWindow->display();
	}
}