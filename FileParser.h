#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

namespace da 
{
	class FileParser
	{
	public:
		static sf::Color*	m_pColorArray;
		static uint32_t		m_ColorCount; // Count of all numbers provaided as argument
		static std::vector<std::string> m_VectorforParsedValues;
	
	private:
		
	
	public:
		static sf::Color* CreateColorArray(const std::string& data);
		static void	DeleteColorArray();
	
	private:
		FileParser() {};
		~FileParser() {};
	
		static void getNumberFromString(const std::string& s);
	
	};
}


