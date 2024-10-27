#ifndef GUIANT_HPP
#define GUIANT_HPP

#include "GUIBase.hpp"

namespace da 
{
	class GUIAnt : public GUIBase
	{
	public:
		enum Names : uint16_t
		{
			FIRST = 0U,
			PATH = FIRST,
			SPEED,
			MOVES,
			EXIT_INFO,
			LAST
		};

	private:

	public:
		GUIAnt(uint32_t windowWidth, uint32_t windowHeight);
		~GUIAnt();

		void Redraw() override;
		void UpdateText(uint16_t name, const std::string& text)									override;
		void UpdateTextAfter(uint16_t name, uint16_t charactersToSkip, const std::string& text)	override;

	private:

	};
}

#endif