#pragma once

#include "GeneratorBase.hpp"

namespace Wordclock
{
	class GeneratorGradientBase : public GeneratorBase
	{
	protected:
		static uint8_t rep;
		static int16_t rVec;
		static int16_t gVec;
		static int16_t bVec;
		static int16_t rVal;
		static int16_t gVal;
		static int16_t bVal;

		GeneratorBase* gen;

		CRGB advance(const uint8_t &speed);

	public:
		GeneratorGradientBase(GeneratorBase* generator);

		ColorTypes getColorType();
	};

	/// generates a succession of similar colors so that they can be used for a gradient or fader.
	/// @tparam pace - the pace of the color drifting. Too hight values might cause color leaps.
	///                It determines the maximal value any color may be changed by between two calls to @c GeneratorBase.nextColor.
	template <uint8_t speed = 2>
	class GeneratorGradient : public GeneratorGradientBase
	{
	public:
		/// initializes the color generator.
		/// @param generator - the color generator that generates the colors for the gradient.
		GeneratorGradient(GeneratorBase* generator);

		CRGB nextColor();
	};

	// implementation //

	template <uint8_t speed>
	GeneratorGradient<speed>::GeneratorGradient(GeneratorBase* generator)
		: GeneratorGradientBase(generator)
	{}

	template <uint8_t speed>
	CRGB GeneratorGradient<speed>::nextColor()
	{
		return advance(speed);
	}
}