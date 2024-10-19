#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <unordered_set>

#include "VertexDefinitions.h"

namespace da 
{
	class FileParser
	{
	public:
		static std::unordered_set<std::string> m_SetOfPaths;

	private:
		static bool CheckAndInsert(const std::string& data);
	
	public:
		static sf::Color* CreateColorArrayFromCL(const std::string& data);
		static da::GreenColor* CreateDaGreenColorArray(const std::string& data);

	private:
		FileParser() {};
		~FileParser() {};
	};
}
