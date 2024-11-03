
#include "DrawingAppConstants.hpp"
#include "InputParser.hpp"

#include <vector>

namespace da
{
    std::unordered_set<std::string> InputParser::m_SetOfPaths = std::unordered_set<std::string>();

    // checking if inside set negation of path exists, returns true after insertion
    bool InputParser::CheckAndInsert(const std::string& data)
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
    sf::Color* InputParser::CreateColorArrayFromCL(const std::string& data)
    {
        if (!CheckAndInsert(data)) return nullptr;

        uint8_t colorCount = data.size();
        sf::Color* pColorArray = new sf::Color[colorCount];

        //encoding information for ant on alfa channel bits 
        // Alfa channel bits 0 to 3 are for encoding index of next color in ants path
        // Alfa channel bit 4 => 0 turn left, 1(16 or 0001 0000) turn right
        // Alfa channel bits 5 to 7 are used as alfa channel value 
        for (size_t i = 0; i < colorCount; i++)
        {
            int temp = (data[i] == 'L') ? daConstants::LEFT : daConstants::RIGHT;
            int temp2 = (i != colorCount - 1U) ? (i + 1U) : 0U;

            pColorArray[i] = sf::Color(
                0U,
                static_cast<uint8_t>(daConstants::COLOR_RANGE_8BIT * (static_cast<float>(i) / colorCount)),
                0U,
                static_cast<uint8_t>(daConstants::ALFA_BASE_VALUE + temp2 + temp));
        }

        return pColorArray;
    }

    //same as aboce but using different output type
    daTypes::GreenColor* InputParser::CreateDaGreenColorArrayFromCL(const std::string& data)
    {
        if (!CheckAndInsert(data)) return nullptr;

        uint8_t colorCount = data.size();
        daTypes::GreenColor* pGreenColorArray = new daTypes::GreenColor[colorCount];

        //encoding information for ant on alfa channel bits 
        // Alfa channel bits 0 to 3 are for encoding index of next color in ants path
        // Alfa channel bit 4 => 0 turn left, 1(16 or 0001 0000) turn right
        // Alfa channel bits 5 to 7 are used as alfa channel value 
        for (size_t i = 0; i < colorCount; i++)
        {
            uint8_t temp = (data[i] == 'L') ? daConstants::LEFT : daConstants::RIGHT;
            uint8_t temp2 = (i != colorCount - 1U) ? (i + 1U) : 0U;                 // @Bug only 15 colors possible

            pGreenColorArray[i] = daTypes::GreenColor{
                                        static_cast<uint8_t>(daConstants::COLOR_RANGE_8BIT * (static_cast<float>(i) / colorCount)),
                                        static_cast<uint8_t>(daConstants::ALFA_BASE_VALUE + temp2 + temp)
            };
        }

        return pGreenColorArray;
    }

}

