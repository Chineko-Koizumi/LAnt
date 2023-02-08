#include "FileParser.h"

#include <vector>

sf::Color* da::FileParser::CreateColorArray(const std::string& data)
{
    getNumberFromString(data);

    m_ColorCount    = std::stoi(m_VectorforParsedValues[0]);
    m_pColorArray   = new sf::Color[m_ColorCount];

    for (size_t i = 0; i < m_ColorCount; i++)
    {
        m_pColorArray[i] = sf::Color(
             std::stoi(m_VectorforParsedValues[i * 4 + 1])
            ,std::stoi(m_VectorforParsedValues[i * 4 + 2])
            ,std::stoi(m_VectorforParsedValues[i * 4 + 3])
            ,std::stoi(m_VectorforParsedValues[i * 4 + 4])
        );
    }
	return m_pColorArray;
}

void da::FileParser::DeleteColorArray()
{
    delete[] m_pColorArray;
}

void da::FileParser::getNumberFromString(const std::string& s)
{
    std::stringstream ss(s);
    while (ss.good()) 
    {
        std::string substr;
        std::getline(ss, substr, ',');
        m_VectorforParsedValues.push_back(substr);
    } 
}

sf::Color* da::FileParser::m_pColorArray = nullptr;
uint32_t da::FileParser::m_ColorCount = 0;
std::vector<std::string> da::FileParser::m_VectorforParsedValues = std::vector<std::string>();


