#include "FileParser.h"

#include <vector>

#define TURN_MASK			16

sf::Color* da::FileParser::CreateColorArray(const std::string& data)
{
    m_VectorforParsedValues.clear();
    getNumberFromString(data);

    m_AntCurrentPathString.clear();

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

        char c = ((TURN_MASK & m_pColorArray[i].a) == 16) ? 'R' : 'L';
        m_AntCurrentPathString.push_back(c);
    }
    std::string AntiString = m_AntCurrentPathString;

    for (size_t i = 0; i < m_AntCurrentPathString.length(); i++)
    {
        switch (AntiString[i])
        {
            case 'R': AntiString[i] = 'L'; break;
            case 'L': AntiString[i] = 'R'; break;
        }
    }

    if (m_SetOfPaths.end() == m_SetOfPaths.find(AntiString))
    {
        m_SetOfPaths.insert(m_AntCurrentPathString);
        return m_pColorArray;
    }
    else 
    {
        return nullptr;
    }
}

void da::FileParser::DeleteColorArray()
{
    if(m_pColorArray !=nullptr) delete[] m_pColorArray;
}

da::Color* da::FileParser::CreateDaColorArray(const std::string& data)
{
    m_VectorforParsedValues.clear();
    getNumberFromString(data);

    m_AntCurrentPathString.clear();

    m_ColorCount = std::stoi(m_VectorforParsedValues[0]);
    m_pDaColorArray = new da::Color[m_ColorCount];

    for (size_t i = 0; i < m_ColorCount; i++)
    {
        m_pDaColorArray[i] = da::Color{
              (uint8_t)std::stoi(m_VectorforParsedValues[i * 4 + 1])
            , (uint8_t)std::stoi(m_VectorforParsedValues[i * 4 + 2])
            , (uint8_t)std::stoi(m_VectorforParsedValues[i * 4 + 3])
            , (uint8_t)std::stoi(m_VectorforParsedValues[i * 4 + 4])
        };

        char c = ((TURN_MASK & m_pDaColorArray[i].a) == 16) ? 'R' : 'L';
        m_AntCurrentPathString.push_back(c);
    }
    std::string AntiString = m_AntCurrentPathString;

    for (size_t i = 0; i < m_AntCurrentPathString.length(); i++)
    {
        switch (AntiString[i])
        {
        case 'R': AntiString[i] = 'L'; break;
        case 'L': AntiString[i] = 'R'; break;
        }
    }

    if (m_SetOfPaths.end() == m_SetOfPaths.find(AntiString))
    {
        m_SetOfPaths.insert(m_AntCurrentPathString);
        return m_pDaColorArray;
    }
    else
    {
        return nullptr;
    }
}

void da::FileParser::DeleteDaColorArray()
{
    if (m_pDaColorArray != nullptr) delete[] m_pDaColorArray;
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

sf::Color* da::FileParser::m_pColorArray   = nullptr;
da::Color* da::FileParser::m_pDaColorArray = nullptr;
uint32_t da::FileParser::m_ColorCount = 0;
std::vector<std::string> da::FileParser::m_VectorforParsedValues = std::vector<std::string>();
std::string da::FileParser::m_AntCurrentPathString = std::string();
std::unordered_set<std::string> da::FileParser::m_SetOfPaths = std::unordered_set<std::string>();


