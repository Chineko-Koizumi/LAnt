#ifndef GUIBASE_HPP
#define GUIBASE_HPP

#include <SFML/Graphics.hpp>

#include <cassert>

namespace da 
{
	class GUIBase
	{
	protected:
		struct ProgressBar
		{
			sf::RenderTexture renderTexture;

			sf::Texture insideTexture;
			sf::Texture outsideTexture;

			uint16_t progressBarWidth{ 0U };
			uint16_t progressBarHeight{ 0U };

			sf::Sprite insideSprite;
			sf::Sprite outsideSprite;
			 
			void Init(
				uint16_t windowWidth, 
				uint16_t windowHeight, 
				const std::string& sInsideTexture, 
				const std::string& sOtsideTexture,
				float scalingFactorCorrection,
				float scalingX,
				float scalingY,
				float posXIn,	//relative to window
				float posYIn,	//relative to window
				float posXOut,	//relative to window
				float posYOut	//relative to window
				)
			{
				if (!insideTexture.loadFromFile(sInsideTexture))	assert(false);
				if (!outsideTexture.loadFromFile(sOtsideTexture))	assert(false);

				progressBarWidth = static_cast<uint16_t>(outsideTexture.getSize().x);
				progressBarHeight = static_cast<uint16_t>(outsideTexture.getSize().y);

				insideTexture.setSmooth(true);
				outsideTexture.setSmooth(true);

				insideSprite.setTexture(insideTexture);
				outsideSprite.setTexture(outsideTexture);

				float scalingFactor = static_cast<float>(windowWidth) / progressBarWidth;
				scalingFactor *= scalingFactorCorrection;

				insideSprite.setScale(scalingFactor		* scalingX, scalingFactor * scalingY);
				outsideSprite.setScale(scalingFactor	* scalingX, scalingFactor * scalingY);

				insideSprite.setPosition(static_cast<float>(windowWidth)	* posXIn, static_cast<float>(windowHeight)	* posYIn);
				outsideSprite.setPosition(static_cast<float>(windowWidth)	* posXOut, static_cast<float>(windowHeight) * posYOut);

				renderTexture.create(windowWidth, windowHeight);
			}

			void SetProgress(float progressInPercent, float scaleX)
			{
				insideSprite.setTextureRect(sf::IntRect(
					0,
					0,
					static_cast<int>(progressBarWidth * scaleX * progressInPercent),
					static_cast<int>(progressBarHeight)));
			}
		};

		struct Background
		{
			sf::Texture texture;
			sf::Sprite sprite;

		};

		sf::RenderWindow* m_pWindow;

		uint32_t m_WindowWidth;
		uint32_t m_WindowHeight;

		sf::Text*		m_pGUITexts;
		std::string*	m_pGUITextsStrings;

		sf::Font m_FontConsolas;
		sf::Font m_FontTahoma;
		sf::Font m_FontTahomaBold;

		sf::Color m_ColorWindowsBlack{ 6U, 8U, 12U };
		sf::Color m_ColorWindowsLightBlack{ 19U, 21U, 25U };

		bool m_ExternalWindowPointer;

	public:

		GUIBase(uint32_t windowWidth, uint32_t windowHeight, uint16_t enumCount);
		GUIBase(uint32_t windowWidth, uint32_t windowHeight, uint16_t enumCount, sf::RenderWindow* pExistingWindow);

		virtual ~GUIBase();

		sf::RenderWindow* GetWindowPtr();
		void UpdateText(uint16_t name, const std::string& text);
		void UpdateText(uint16_t name, const char* text);
		void UpdateTextAfter(uint16_t name, uint16_t charactersToSkip, const std::string& text);
		void AppendText(uint16_t name, const std::string& text);

		virtual void FetchDataForGUI(uint8_t msgCountPerFetch) = 0;
		virtual void Redraw(bool clearScreen, bool pushToScreen) = 0;

	private:
		
	};
}

#endif