
#include "DrawingAppConstants.h"
#include "FileParser.h"

#include <vector>

bool da::FileParser::CheckAndInsert(const std::string& data) // checking if inside set negation of path exists, returns true after insertion
{
    std::string AntiString = data;

    for (size_t i = 0; i < data.length(); ++i)
    {
        switch (AntiString[i])
        {
        case 'R': AntiString[i] = 'L'; break;
        case 'L': AntiString[i] = 'R'; break;
        }
    }

    if (m_SetOfPaths.end() == m_SetOfPaths.find(AntiString))
    {
        m_SetOfPaths.insert(data);
        return true;
    }
    else
    {
        return false;
    }
}

sf::Color* da::FileParser::CreateColorArrayFromCL(const std::string& data)
{
    if ( !CheckAndInsert(data) ) return nullptr;
    
    uint8_t colorCount = data.size();
    sf::Color*  pColorArray = new sf::Color[colorCount];

    //encoding information for ant on alfa channel bits 
    // Alfa channel bits 0 to 3 are for encoding index of next color in ants path
    // Alfa channel bit 4 => 0 turn left, 1(16 or 0001 0000) turn right
    // Alfa channel bits 5 to 7 are used as alfa channel value 
    for (size_t i = 0; i < colorCount; i++)
    {
        int temp = (data[i] == 'L') ? constants::LEFT : constants::RIGHT;
        int temp2 = (i != colorCount - 1) ? (i + 1) : 0;

        pColorArray[i] = sf::Color(0U, constants::COLOR_RANGE_8BIT * (static_cast<float>(i)/ colorCount), 0U, constants::ALFA_BASE_VALUE + temp2 + temp);
    } 

    return pColorArray;
}

da::GreenColor* da::FileParser::CreateDaGreenColorArray(const std::string& data)
{
    m_VectorforParsedValues.clear();
    getNumberFromString(data);

    m_AntCurrentPathString.clear();

    m_ColorCount = std::stoi(m_VectorforParsedValues[0]);
    m_pDaGreenColorArray = new da::GreenColor[m_ColorCount];

    for (size_t i = 0; i < m_ColorCount; ++i)
    {
        m_pDaGreenColorArray[i] = da::GreenColor{
              (uint8_t)std::stoi(m_VectorforParsedValues[i * 4 + 2])
            , (uint8_t)std::stoi(m_VectorforParsedValues[i * 4 + 4])
        };

        char c = ((constants::TURN_MASK & m_pDaGreenColorArray[i].a) == constants::RIGHT) ? 'R' : 'L';
        m_AntCurrentPathString.push_back(c);
    }

    if (CheckAndInsert(m_AntCurrentPathString))     return m_pDaGreenColorArray;
    else                                            return nullptr;
}

void da::FileParser::DeleteDaGreenColorArray()
{
    if (m_pDaGreenColorArray != nullptr) delete[] m_pDaGreenColorArray;
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

da::GreenColor* da::FileParser::m_pDaGreenColorArray    = nullptr;

uint32_t da::FileParser::m_ColorCount = 0;
std::vector<std::string> da::FileParser::m_VectorforParsedValues = std::vector<std::string>();
std::string da::FileParser::m_AntCurrentPathString = std::string();
std::unordered_set<std::string> da::FileParser::m_SetOfPaths = std::unordered_set<std::string>();


