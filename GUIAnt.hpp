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
		GUIAnt(uint32_t windowWidth, uint32_t windowHeight, sf::RenderWindow* pExistingWindow);
		~GUIAnt();

		virtual void FetchDataForGUI(uint64_t msgCountPerFetch) override;
		void Redraw(bool clearScreen, bool pushToScreen) override;

	private:

	};
}

#endif