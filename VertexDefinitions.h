#include <cstdint>
#include <SFML/Graphics/Color.hpp>

namespace da 
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
    
    struct Color
    {
        uint8_t r; ///< Red component
        uint8_t g; ///< Green component
        uint8_t b; ///< Blue component
        uint8_t a; ///< Alpha (opacity) component
    };

    struct ColorIndex 
    {
        Color       c;
        uint8_t     i;
    };
}
