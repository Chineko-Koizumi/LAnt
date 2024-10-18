#ifndef DRAWINGAPPCONSTANTS_H
#define DRAWINGAPPCONSTANTS_H
#include <stdint.h>

namespace constants 
{
	static const uint8_t COLOR_RANGE_8BIT = 255U;
	static const uint8_t ALFA_BASE_VALUE = 224U; //1110 0000

	static const uint8_t TURN_MASK = 16U;  //0001 0000
	static const uint8_t LEFT = 0U;
	static const uint8_t RIGHT = 16U;	//0001 0000

	static const uint8_t LIMIT_NOT_REACHED	= 0U;
	static const uint8_t X_ABOVE_LIMIT		= 1U;	
	static const uint8_t X_BELOW_LIMIT		= 2U;
	static const uint8_t Y_ABOVE_LIMIT		= 3U;
	static const uint8_t Y_BELOW_LIMIT		= 4U;

}

#endif
