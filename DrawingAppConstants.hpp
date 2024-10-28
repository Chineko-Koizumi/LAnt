#ifndef DRAWINGAPPCONSTANTS_HPP
#define DRAWINGAPPCONSTANTS_HPP
#include <stdint.h>

#include <SFML/Graphics/Color.hpp>

namespace constants 
{
	static const uint8_t COLOR_RANGE_8BIT	= 255U;
	static const uint8_t ALFA_BASE_VALUE	= 224U;	//1110 0000

	static const uint8_t TURN_MASK			= 16U;	//0001 0000
	static const uint8_t COLOR_INDEX_MASK	= 15U;	//0000 1111

	static const uint8_t LEFT				= 0U;	//0000 0000
	static const uint8_t RIGHT				= 16U;	//0001 0000

    const static uint32_t SIZE_OF_VERTEX        = 20U;
    const static uint32_t SIZE_OF_DAVERTEX      = 12U;
    const static uint32_t SIZE_OF_DACOLOR       = 4U;
    const static uint32_t SIZE_OF_DAGREENCOLOR  = 2U;
    const static uint32_t SIZE_OF_MASKED_COLOR  = 1U;

    const static uint32_t KB = 1024U;
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

    enum MessageTypes : uint8_t
    {
        TEXT_UPDATE = 0U,
        PROGRESSBAR_UPDATE
    };

    struct TextureUpdateMSG
    {
        uint8_t type; /// type of msg, (0 text update, 1 progressbar float value)

        uint8_t enumValue; /// enum indicating texture
        char messageUpdate[50];
        uint8_t* arg1[8];
        uint8_t* arg2[8];
    };
}

#endif
