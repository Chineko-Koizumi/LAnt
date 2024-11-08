#ifndef DRAWINGAPPCONSTANTSANTS_HPP
#define DRAWINGAPPCONSTANTSANTS_HPP
#include <stdint.h>
#include <string>

#include <SFML/Graphics/Color.hpp>

namespace daConstants 
{
    static constexpr uint8_t COLOR_RANGE_8BIT = 255U;
    static constexpr uint8_t ALFA_BASE_VALUE = 224U;	//1110 0000
           
    static constexpr uint8_t TURN_MASK = 16U;	//0001 0000
    static constexpr uint8_t COLOR_INDEX_MASK = 15U;	//0000 1111
           
    static constexpr uint8_t LEFT = 0U;	//0000 0000
    static constexpr uint8_t RIGHT = 16U;	//0001 0000
           
    static constexpr uint32_t SIZE_OF_VERTEX = 20U;
    static constexpr uint32_t SIZE_OF_DAVERTEX = 12U;
    static constexpr uint32_t SIZE_OF_DACOLOR = 4U;
    static constexpr uint32_t SIZE_OF_DAGREENCOLOR = 2U;
    static constexpr uint32_t SIZE_OF_MASKED_COLOR = 1U;
           
    static constexpr uint32_t KB = 1024U;
           
    static constexpr float GUI_ANT_MEGA_ASPECT_RATIO = 1.333f;

    static constexpr bool CLEAR_SCREEN      = true;
    static constexpr bool NO_CLEAR_SCREEN   = false;

    static constexpr bool PUSH_TO_SCREEN    = true;
    static constexpr bool NO_PUSH_TO_SCREEN = false;

}

namespace daTypes 
{
    struct PointF
    {
        float x;
        float y;
    };

    struct PointUI32
    {
        uint32_t x;
        uint32_t y;
    };

    struct PointUI64
    {
        uint64_t x;
        uint64_t y;
    };

    struct GreenColor
    {
        uint8_t g; /// Green component
        uint8_t a; /// Alpha (opacity) component
    };
}

namespace daFunctions 
{
  inline void AddCommasToStringNumber(std::string& numberInString )
    {
        for (int16_t i = static_cast<int16_t>( numberInString.size() - 3); i > 0; i -= 3)
        {
            numberInString.insert(i, ",");
        }
    }
}

#endif
