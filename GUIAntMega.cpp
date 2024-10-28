#include "GUIAntMega.hpp"

namespace da 
{
	GUIAntMega::Names& operator++ (GUIAntMega::Names& name)
	{
		uint8_t temp = name;
		name = static_cast<GUIAntMega::Names>(++temp);

		return name;
	}

	GUIAntMega::GUIAntMega(uint32_t windowWidth, uint32_t windowHeight)
		: GUIBase(windowWidth / 2U, windowHeight / 3U, Names::LAST)
		, m_CopyStarted(false)
	{
		m_pWindow->setTitle("Ant MegaPath");
		InitThreasholdBar();
		InitCopyBar();

		float windowSpacing = static_cast<float>(windowHeight) / 30U;
		for (Names name = Names::FIRST; name < Names::LAST; ++name)
		{
			m_pGUITexts[name].setFont(m_Font);
			m_pGUITexts[name].setFillColor(sf::Color(74U, 246U, 38U));
			m_pGUITexts[name].setOutlineColor(sf::Color(74U, 246U, 38U));
			m_pGUITexts[name].setString("");
			m_pGUITextsStrings[name] = std::string("");
			m_pGUITexts[name].setCharacterSize(windowSpacing);
			m_pGUITexts[name].setLineSpacing(1.1f);

			m_pGUITexts[name].setPosition(
									0.0f, 
									static_cast<float>(m_pGUITexts[name].getCharacterSize()) * m_pGUITexts[name].getLineSpacing() * name);
		}

		float progressBarYPos = m_pGUITexts[Names::THRESHOLD].getPosition().y + m_pGUITexts[Names::THRESHOLD].getCharacterSize() * 1.5f; // we need set possition referencing Text THRESHOLD
		m_ThreasholdBar.m_ProgressBarOutsideSprite.setPosition(	0.0f, progressBarYPos);
		m_ThreasholdBar.m_ProgressBarInsideSprite.setPosition(	0.0f, progressBarYPos);

		m_pGUITexts[Names::INFO].setPosition(0.0f, progressBarYPos + m_ThreasholdBar.m_ProgressBarOutsideSprite.getGlobalBounds().height);
		m_pGUITexts[Names::INFO].setCharacterSize(windowSpacing / 2U);

		float copyBarYPos = m_pGUITexts[Names::INFO].getPosition().y + m_pGUITexts[Names::INFO].getCharacterSize() * 1.5f; // we need set possition referencing Text THRESHOLD
		m_CopyBar.m_ProgressBarOutsideSprite.setPosition(0.0f, copyBarYPos);
		m_CopyBar.m_ProgressBarInsideSprite.setPosition(0.0f, copyBarYPos);
	}

	GUIAntMega::~GUIAntMega()
	{
	}

	void da::GUIAntMega::InitThreasholdBar()
	{
		if (!m_ThreasholdBar.m_ProgressBarOutsideTexture.loadFromFile("./Sprites/GUI/AntMega/ProgressBarOutside.png"))	assert(false);
		if (!m_ThreasholdBar.m_ProgressBarInsideTexture.loadFromFile("./Sprites/GUI/AntMega/ProgressBarInside.png"))	assert(false);

		m_ThreasholdBar.m_ProgressBarWidth = m_ThreasholdBar.m_ProgressBarOutsideTexture.getSize().x;
		m_ThreasholdBar.m_ProgressBarHeight = m_ThreasholdBar.m_ProgressBarOutsideTexture.getSize().y;

		m_ThreasholdBar.m_ProgressBarRenderTexture.create(m_ThreasholdBar.m_ProgressBarWidth, m_ThreasholdBar.m_ProgressBarHeight);

		m_ThreasholdBar.m_ProgressBarOutsideTexture.setSmooth(true);
		m_ThreasholdBar.m_ProgressBarInsideTexture.setSmooth(true);

		m_ThreasholdBar.m_ProgressBarOutsideSprite.setTexture(m_ThreasholdBar.m_ProgressBarOutsideTexture);
		m_ThreasholdBar.m_ProgressBarInsideSprite.setTexture(m_ThreasholdBar.m_ProgressBarInsideTexture);

		float scalingFactor = static_cast<float>(m_WindowWidth) / m_ThreasholdBar.m_ProgressBarWidth;
		m_ThreasholdBar.m_ProgressBarOutsideSprite.setScale(scalingFactor, scalingFactor);
		m_ThreasholdBar.m_ProgressBarInsideSprite.setScale(scalingFactor, scalingFactor);
	}

	void da::GUIAntMega::InitCopyBar()
	{
		if (!m_CopyBar.m_ProgressBarOutsideTexture.loadFromFile("./Sprites/GUI/AntMega/CopingFileBackground.png"))	assert(false);
		if (!m_CopyBar.m_ProgressBarInsideTexture.loadFromFile("./Sprites/GUI/AntMega/CopingProgressBar.png"))		assert(false);

		m_CopyBar.m_ProgressBarWidth = m_CopyBar.m_ProgressBarOutsideTexture.getSize().x;
		m_CopyBar.m_ProgressBarHeight = m_CopyBar.m_ProgressBarOutsideTexture.getSize().y;

		m_CopyBar.m_ProgressBarRenderTexture.create(m_CopyBar.m_ProgressBarWidth, m_CopyBar.m_ProgressBarHeight);

		m_CopyBar.m_ProgressBarOutsideTexture.setSmooth(true);
		m_CopyBar.m_ProgressBarInsideTexture.setSmooth(true);

		m_CopyBar.m_ProgressBarOutsideSprite.setTexture(m_CopyBar.m_ProgressBarOutsideTexture);
		m_CopyBar.m_ProgressBarInsideSprite.setTexture(m_CopyBar.m_ProgressBarInsideTexture);

		float scalingFactor = static_cast<float>(m_WindowWidth) / m_CopyBar.m_ProgressBarWidth;
		m_CopyBar.m_ProgressBarOutsideSprite.setScale(scalingFactor, scalingFactor);
		m_CopyBar.m_ProgressBarInsideSprite.setScale(scalingFactor, scalingFactor);
	}

	void GUIAntMega::SetProgressThreshold(float progressInPercent)
	{
		m_ThreasholdBar.m_ProgressBarInsideSprite.setTextureRect( sf::IntRect( 0,0, m_ThreasholdBar.m_ProgressBarWidth * progressInPercent, m_ThreasholdBar.m_ProgressBarHeight ) );
	}

	void da::GUIAntMega::SetProgressCopy(float progressInPercent)
	{
		m_CopyStarted = true;

		m_CopyBar.m_ProgressBarInsideSprite.setTextureRect(sf::IntRect(0, 0, m_CopyBar.m_ProgressBarWidth * progressInPercent, m_CopyBar.m_ProgressBarHeight));
	}

	void GUIAntMega::Redraw()
	{
		m_pWindow->clear(sf::Color::Black);
	
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
			}

			for (Names name = Names::FIRST; name < Names::LAST; ++name)
			{
				m_pWindow->draw(m_pGUITexts[name]);
			}

		m_pWindow->display();
	}
}