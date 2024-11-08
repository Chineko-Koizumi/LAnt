#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <unordered_set>

#include "DrawingAppConstants.hpp"

namespace da 
{
	class InputParser
	{
	public:
		static std::unordered_set<std::string> m_SetOfPaths;

	private:
		static bool CheckAndInsert(const std::string& data);
	
	public:
		static sf::Color* CreateColorArrayFromCL(const std::string& data);
		static daTypes::GreenColor* CreateDaGreenColorArrayFromCL(const std::string& data);

	private:
		InputParser() = delete;
		~InputParser() = delete;
	};
}
