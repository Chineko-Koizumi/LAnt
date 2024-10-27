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
		: GUIBase(windowWidth / 2U, windowHeight / 5U, Names::LAST)
		, m_ProgressBarWidth(0)
		, m_ProgressBarHeight(0)
	{
		if (!m_ProgressBarOutsideTexture.loadFromFile("./Sprites/GUI/AntMega/ProgressBarOutside.png"))	assert(false);
		if (!m_ProgressBarInsideTexture.loadFromFile("./Sprites/GUI/AntMega/ProgressBarInside.png"))	assert(false);

		m_ProgressBarWidth = m_ProgressBarOutsideTexture.getSize().x;
		m_ProgressBarHeight = m_ProgressBarOutsideTexture.getSize().y;

		m_ProgressBarRenderTexture.create(m_ProgressBarWidth, m_ProgressBarHeight);

		m_ProgressBarOutsideTexture.setSmooth(true);
		m_ProgressBarInsideTexture.setSmooth(true);

		m_ProgressBarOutsideSprite.setTexture(m_ProgressBarOutsideTexture);
		m_ProgressBarInsideSprite.setTexture(m_ProgressBarInsideTexture);

		float scalingFactor = static_cast<float>(m_WindowWidth) / m_ProgressBarWidth;
		m_ProgressBarOutsideSprite.setScale(scalingFactor, scalingFactor);
		m_ProgressBarInsideSprite.setScale(scalingFactor, scalingFactor);

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

		float progressBarYPos = m_pGUITexts[Names::LAST - 2U].getPosition().y + m_pGUITexts[Names::LAST - 2U].getCharacterSize() * 1.5f; // -2 because we need set possition referencing Text THRESHOLD
		m_ProgressBarOutsideSprite.setPosition(	0.0f, progressBarYPos);
		m_ProgressBarInsideSprite.setPosition(	0.0f, progressBarYPos);

		m_pGUITexts[Names::INFO].setPosition(0.0f, progressBarYPos + m_ProgressBarOutsideSprite.getLocalBounds().height);
	}

	GUIAntMega::~GUIAntMega()
	{
	}

	void GUIAntMega::SetProgress(float progressInPercent)
	{
		m_ProgressBarInsideSprite.setTextureRect( sf::IntRect( 0,0, m_ProgressBarWidth * progressInPercent, m_ProgressBarHeight ) );
	}

	void GUIAntMega::Redraw()
	{
		m_pWindow->clear(sf::Color::Black);
		m_ProgressBarRenderTexture.clear(sf::Color::Black);
		
		m_ProgressBarRenderTexture.draw(m_ProgressBarOutsideSprite);
		m_ProgressBarRenderTexture.draw(m_ProgressBarInsideSprite);
		m_ProgressBarRenderTexture.display();

		sf::Sprite outputSprite(m_ProgressBarRenderTexture.getTexture());

		m_pWindow->draw(outputSprite);

		for (Names name = Names::FIRST; name < Names::LAST; ++name)
		{
			m_pWindow->draw(m_pGUITexts[name]);
		}

		m_pWindow->display();
	}
}