
namespace da 
{
    const static uint32_t SIZE_OF_VERTEX = 20;
    const static uint32_t SIZE_OF_DAVERTEX = 12;

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
    
    struct Color
    {
        uint8_t r; ///< Red component
        uint8_t g; ///< Green component
        uint8_t b; ///< Blue component
        uint8_t a; ///< Alpha (opacity) component
    };


    struct DaVertex 
    {
        PointUI32 position;
        Color color;
    };
}
