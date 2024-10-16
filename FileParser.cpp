
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

// returns poiter to encoded color values with information for ant on alfa channel
// or null pointer if path or antipath exists in collection
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
        int temp2 = (i != colorCount - 1U) ? (i + 1U) : 0U;

        pColorArray[i] = sf::Color(
                                    0U, 
                                    static_cast<uint8_t>(constants::COLOR_RANGE_8BIT * (static_cast<float>(i)/ colorCount)), 
                                    0U, 
                                    static_cast<uint8_t>(constants::ALFA_BASE_VALUE + temp2 + temp));
    } 

    return pColorArray;
}

da::GreenColor* da::FileParser::CreateDaGreenColorArray(const std::string& data)
{
    if (!CheckAndInsert(data)) return nullptr;

    uint8_t colorCount = data.size();
    da::GreenColor* pGreenColorArray = new da::GreenColor[colorCount];

    //encoding information for ant on alfa channel bits 
    // Alfa channel bits 0 to 3 are for encoding index of next color in ants path
    // Alfa channel bit 4 => 0 turn left, 1(16 or 0001 0000) turn right
    // Alfa channel bits 5 to 7 are used as alfa channel value 
    for (size_t i = 0; i < colorCount; i++)
    {
        uint8_t temp = (data[i] == 'L') ? constants::LEFT : constants::RIGHT;
        uint8_t temp2 = (i != colorCount - 1U) ? (i + 1U) : 0U;

        pGreenColorArray[i] = da::GreenColor{ 
                                                static_cast<uint8_t>(constants::COLOR_RANGE_8BIT * (static_cast<float>(i) / colorCount)), 
                                                static_cast<uint8_t>(constants::ALFA_BASE_VALUE + temp2 + temp)
                                             };
    }

    return pGreenColorArray;
}

std::unordered_set<std::string> da::FileParser::m_SetOfPaths = std::unordered_set<std::string>();


