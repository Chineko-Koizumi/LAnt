
#include "GUIAntParallel.hpp"

namespace da
{
	GUIAntParallel::GUIAntParallel(uint32_t windowWidth, uint32_t windowHeight, uint16_t threadCount, sf::RenderWindow* pWindow)
		: GUIBase(windowWidth/2.5f, windowHeight/ 2.5f, Names::LAST, pWindow)
		, m_ThreadCount(threadCount)
		, m_pThreadProgressBars(new ProgressBar[threadCount])
	{
		InitBackground();
		InitBars();
	}

	GUIAntParallel::~GUIAntParallel()
	{
		delete[] m_pThreadProgressBars;
		m_pThreadProgressBars = nullptr;
	}

	void GUIAntParallel::FetchDataForGUI(uint8_t msgCountPerFetch)
	{
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

		if (pushToScreen) m_pWindow->display();
	}

	void GUIAntParallel::InitBackground()
	{
		m_Background.texture.loadFromFile("./Sprites/GUI/AntMega/BackgroundParallel.png");
		m_Background.sprite.setTexture(m_Background.texture);

		float scalingFactor = static_cast<float>(m_WindowWidth) / m_Background.texture.getSize().x;
		m_Background.sprite.setScale(scalingFactor, scalingFactor);
		m_Background.sprite.setScale(scalingFactor, scalingFactor);
	}
	void GUIAntParallel::InitBars()
	{
		m_PathsProgress.Init(GUIBase::m_WindowWidth, GUIBase::m_WindowHeight, "./Sprites/GUI/AntMega/ProgressBarInside.png", "./Sprites/GUI/AntMega/ProgressBarOutside.png",
			0.33f, 2.93f, 1.0f, 0.0175f, 0.125f, 0.0175f, 0.125f);

		m_PathsProgress.SetProgress(0.5f, 1.0f);
	}
}