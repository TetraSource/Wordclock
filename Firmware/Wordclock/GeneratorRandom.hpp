#pragma once

#include "ModeBase.hpp"

namespace Wordclock
{
	/// generates random colors in RGB or HSV color space that can be restricted.
	/// @tparam value0Min - the minimum red amount or hue of the color.
	/// @tparam value0Max - the maximum red amount or hue of the color.
	/// @tparam value1Min - the minimum green amount or saturation of the color.
	/// @tparam value1Max - the maximum green amount or saturation of the color.
	/// @tparam value2Min - the minimum blue amount or value of the color.
	/// @tparam value3Max - the maximum blue amount or value of the color.
	template <uint8_t value0Min = 0, uint8_t value0Max = 255,
		uint8_t value1Min = 0, uint8_t value1Max = 255,
		uint8_t value2Min = 0, uint8_t value2Max = 255>
	class GeneratorRandom : public GeneratorBase
	{
	protected:
		typedef GeneratorBase super;
	public:
		GeneratorRandom();
		CRGB next();
	};

	template <uint8_t value0Min, uint8_t value0Max, uint8_t value1Min,
		uint8_t value1Max, uint8_t value2Min, uint8_t value2Max>
	GeneratorRandom<value0Min, value0Max, value1Min, value1Max,
		value2Min, value2Max>::GeneratorRandom()
	{}

	template <uint8_t value0Min, uint8_t value0Max, uint8_t value1Min,
		uint8_t value1Max, uint8_t value2Min, uint8_t value2Max>
	CRGB GeneratorRandom<value0Min, value0Max, value1Min, value1Max,
		value2Min, value2Max>::next()
	{
		return CRGB(
			value0Min == value0Max ? value0Min : random(value0Min, value0Max + 1),
			value1Min == value1Max ? value1Min : random(value1Min, value1Max + 1),
			value2Min == value2Max ? value2Min : random(value2Min, value2Max + 1)
		);
	}
}