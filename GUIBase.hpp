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

			uint64_t progressBarTextureWidth{ 0U };
			uint64_t progressBarTextureHeight{ 0U };

			sf::Sprite insideSprite;
			sf::Sprite outsideSprite;
			sf::Sprite outputSprite;
			sf::Text progressBarLabel;

			void InitLabel(const sf::Font& font, const sf::Color& color, const std::string& label) 
			{
				progressBarLabel.setFont(font);
				progressBarLabel.setFillColor(color);
				progressBarLabel.setOutlineColor(color);
				progressBarLabel.setString(label);
			}

			void Init(
				uint64_t windowWidth, 
				uint64_t windowHeight, 
				const std::string& sInsideTexture, 
				const std::string& sOtsideTexture,
				float scalingX,
				float scalingY)
			{

				if (!insideTexture.loadFromFile(sInsideTexture))	assert(false);
				if (!outsideTexture.loadFromFile(sOtsideTexture))	assert(false);

				progressBarTextureWidth = static_cast<uint64_t>(outsideTexture.getSize().x);
				progressBarTextureHeight = static_cast<uint64_t>(outsideTexture.getSize().y);

				insideTexture.setSmooth(true);
				outsideTexture.setSmooth(true);

				insideSprite.setTexture(insideTexture);
				outsideSprite.setTexture(outsideTexture);

				float scalingFactor = static_cast<float>(windowWidth) / progressBarTextureWidth;

				insideSprite.setScale(scalingFactor		* scalingX, scalingFactor * scalingY);
				outsideSprite.setScale(scalingFactor	* scalingX, scalingFactor * scalingY);

				renderTexture.create(windowWidth, windowHeight);
				outputSprite.setTexture(renderTexture.getTexture());

				progressBarLabel.setCharacterSize(insideSprite.getGlobalBounds().height * 1.15f);
			}

			void SetPossition(float xNormalizedIn, float yNormalizedIn, float xNormalizedOut, float yNormalizedOut)
			{
				insideSprite.setPosition(	xNormalizedIn *		renderTexture.getSize().x, yNormalizedIn *	renderTexture.getSize().y);
				outsideSprite.setPosition(	xNormalizedOut *	renderTexture.getSize().x, yNormalizedOut * renderTexture.getSize().y);

				progressBarLabel.setPosition(insideSprite.getGlobalBounds().left, insideSprite.getGlobalBounds().top - insideSprite.getGlobalBounds().height/4.0f);
			}
			void SetProgress(float progressInPercent, float scaleX)
			{
				insideSprite.setTextureRect(sf::IntRect(
					0,
					0,
					static_cast<int>(progressBarTextureWidth * scaleX * progressInPercent),
					static_cast<int>(progressBarTextureHeight)));
			}
			sf::Sprite& DrawnSprite() 
			{
				renderTexture.draw(outsideSprite);
				renderTexture.draw(insideSprite);
				renderTexture.draw(progressBarLabel);
				renderTexture.display();

				return outputSprite;
			}
		};

		struct Background
		{
			sf::Texture texture;
			sf::Sprite sprite;
		};

		sf::RenderWindow* m_pWindow;

		uint64_t m_WindowWidth;
		uint64_t m_WindowHeight;

		sf::Text*		m_pGUITexts;
		std::string*	m_pGUITextsStrings;

		sf::Font m_FontConsolas;
		sf::Font m_FontConsolasBold;
		sf::Font m_FontTahoma;
		sf::Font m_FontTahomaBold;

		sf::Color m_ColorWindowsBlack{ 6U, 8U, 12U };
		sf::Color m_ColorWindowsLightBlack{ 19U, 21U, 25U };

		bool m_ExternalWindowPointer;

	public:

		GUIBase(uint64_t windowWidth, uint64_t windowHeight, uint16_t enumCount);
		GUIBase(uint64_t windowWidth, uint64_t windowHeight, uint16_t enumCount, sf::RenderWindow* pExistingWindow);

		virtual ~GUIBase();

		sf::RenderWindow* GetWindowPtr();
		void UpdateText(uint16_t name, const std::string& text);
		void UpdateText(uint16_t name, const char* text);
		void UpdateTextAfter(uint16_t name, uint16_t charactersToSkip, const std::string& text);
		void AppendText(uint16_t name, const std::string& text);

		virtual void FetchDataForGUI(uint64_t msgCountPerFetch) = 0;
		virtual void Redraw(bool clearScreen, bool pushToScreen) = 0;

	private:
		
	};
}

#endif