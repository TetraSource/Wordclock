#pragma once

#include "GeneratorBase.hpp"

namespace Wordclock
{
	/// Always generates the same color.
	/// @tparam value0 - the red amount or hue of the color.
	/// @tparam value1 - the green amount or saturation of the color.
	/// @tparam value2 - the blue amount or saturation of the color.
	template <uint8_t value0, uint8_t value1, uint8_t value2>
	class GeneratorStatic : public GeneratorBase
	{
	protected:
		typedef GeneratorBase super;
	public:
		GeneratorStatic();
		CRGB next();
	};

	template <uint8_t value0, uint8_t value1, uint8_t value2>
	GeneratorStatic<value0, value1, value2>::GeneratorStatic()
	{}

	template <uint8_t value0, uint8_t value1, uint8_t value2>
	CRGB GeneratorStatic<value0, value1, value2>::next()
	{
		return CRGB(value0, value1, value2);
	}
}