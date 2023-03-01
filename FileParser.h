#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <unordered_set>

namespace da 
{
	class FileParser
	{
	public:
		static sf::Color*		m_pColorArray;
		static da::Color*		m_pDaColorArray;
		static da::GreenColor*	m_pDaGreenColorArray;
		static uint32_t		m_ColorCount; // Count of all numbers provaided as argument

		static std::vector<std::string> m_VectorforParsedValues;
		static std::string m_AntCurrentPathString;
		static std::unordered_set<std::string> m_SetOfPaths;

	private:
		static bool CheckAndInsert();
	
	public:
		static sf::Color* CreateColorArray(const std::string& data);
		static void	DeleteColorArray();

		static da::Color* CreateDaColorArray(const std::string& data);
		static void	DeleteDaColorArray();

		static da::GreenColor* CreateDaGreenColorArray(const std::string& data);
		static void	DeleteDaGreenColorArray();

	private:
		FileParser() {};
		~FileParser() {};
	
		static void getNumberFromString(const std::string& s);
	
	};

}


