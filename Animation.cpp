#include "Animation.hpp"

namespace da
{
	Animation::Animation(
		  uint16_t totalFrames
		, uint16_t timeBetweenFramesMs
		, float posX
		, float posY
		, float scale
		, const std::string& pathToTexture)
			: m_TotalFrames(totalFrames)
			, m_CurrentFrame(0U)
			, m_TimeBetweenFramesMs(timeBetweenFramesMs)
			, m_TextureGridWidth(0U)
			, m_TextureGridHeight(0U)
			, m_SpriteWindowWidth(0U)
			, m_SpriteWindowHeight(0U)
	{
		m_Grid.loadFromFile(pathToTexture);

		m_TextureGridWidth		= m_Grid.getSize().x;
		m_TextureGridHeight		= m_Grid.getSize().y;
		m_SpriteWindowWidth		= m_Grid.getSize().x;
		m_SpriteWindowHeight	= m_Grid.getSize().y / m_TotalFrames;

		m_AnimatedSprite.setTexture(m_Grid);
		m_AnimatedSprite.setTextureRect(sf::IntRect(0, 0, m_SpriteWindowWidth, m_SpriteWindowHeight));

		m_AnimatedSprite.setPosition(posX, posY);
		m_AnimatedSprite.setScale(scale, scale);
	}

	Animation::~Animation()
	{
	}

	sf::Sprite& da::Animation::GetCurrentFrame()
	{
		static bool isTimingFrame = false;

		if (!isTimingFrame) 
		{
			m_start = std::chrono::high_resolution_clock::now();
			isTimingFrame = true;

		}
		else 
		{
			m_reference = std::chrono::high_resolution_clock::now();

			if (std::chrono::duration_cast<std::chrono::milliseconds>(m_reference - m_start).count() >= m_TimeBetweenFramesMs) 
			{
				if (m_CurrentFrame == m_TotalFrames)
				{
					m_CurrentFrame = 0U;
				}

				m_AnimatedSprite.setTextureRect(sf::IntRect(0, m_SpriteWindowHeight * m_CurrentFrame, m_SpriteWindowWidth, m_SpriteWindowHeight));


				m_start = std::chrono::high_resolution_clock::now();
				++m_CurrentFrame;
			}
		}
		return m_AnimatedSprite;
	}
}