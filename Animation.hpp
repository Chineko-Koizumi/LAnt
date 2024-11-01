#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <chrono>

namespace da 
{
	class Animation
	{
	private:
		std::chrono::steady_clock::time_point m_start;
		std::chrono::steady_clock::time_point m_reference;

		sf::Texture m_Grid;
		sf::Sprite m_AnimatedSprite;

		uint16_t m_TotalFrames;
		uint16_t m_CurrentFrame;
		uint16_t m_TimeBetweenFramesMs;

		uint32_t m_SpriteWindowWidth;
		uint32_t m_SpriteWindowHeight;

		uint32_t m_TextureGridWidth;
		uint32_t m_TextureGridHeight;

	public:
		Animation(
			uint16_t totalFrames
			, uint16_t timeBetweenFramesMs
			, float posX
			, float posY
			, float scale
			, const std::string& pathToTexture);

		~Animation();

		sf::Sprite& GetCurrentFrame();

	private:

	};
}

#endif