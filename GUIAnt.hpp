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

		virtual void FetchDataForGUI(uint8_t msgCountPerFetch) override;
		void Redraw() override;

	private:

	};
}

#endif