#pragma once

#include "GeneratorBase.hpp"

namespace Wordclock
{
	/// generates random colors in RGB or HSV color space that can be restricted.
	/// @tparam value0Min - the minimum red amount or hue of the color.
	/// @tparam value0Max - the maximum red amount or hue of the color.
	/// @tparam value1Min - the minimum green amount or saturation of the color.
	/// @tparam value1Max - the maximum green amount or saturation of the color.
	/// @tparam value2Min - the minimum blue amount or value of the color.
	/// @tparam value3Max - the maximum blue amount or value of the color.
	/// @tparam colorType - the color space of the color.
	template <uint8_t value0Min = 0, uint8_t value0Max = 255, uint8_t value1Min = 0, uint8_t value1Max = 255, uint8_t value2Min = 0, uint8_t value2Max = 255, ColorTypes colorType = RGB>
	class GeneratorRandom : public GeneratorBase
	{
	public:
		CRGB nextColor();

		ColorTypes getColorType();
	};

	template <uint8_t value0Min, uint8_t value0Max, uint8_t value1Min, uint8_t value1Max, uint8_t value2Min, uint8_t value2Max, ColorTypes colorType>
	CRGB GeneratorRandom<value0Min, value0Max, value1Min, value1Max, value2Min, value2Max, colorType>::nextColor()
	{
		return CRGB(random(value0Min, value0Max + 1), random(value1Min, value1Max + 1), random(value2Min, value2Max + 1));
	}

	template <uint8_t value0Min, uint8_t value0Max, uint8_t value1Min, uint8_t value1Max, uint8_t value2Min, uint8_t value2Max, ColorTypes colorType>
	ColorTypes GeneratorRandom<value0Min, value0Max, value1Min, value1Max, value2Min, value2Max, colorType>::getColorType()
	{
		return colorType;
	}
}