#pragma once

#include "GeneratorBase.hpp"

namespace Wordclock
{
	class GeneratorGradientBase : public GeneratorBase
	{
	protected:
		static uint16_t rep;
		static int16_t rVal;
		static int16_t gVal;
		static int16_t bVal;
		static GeneratorBase* lastGen;

		GeneratorBase* gen;

		CRGB advance(const uint8_t &repFactor);
		void select();

	public:
		GeneratorGradientBase(GeneratorBase* generator);

		ColorTypes getColorType();
	};

	/// generates a succession of similar colors so that they can be used for a gradient or fader.
	/// @tparam speed - the speed of the color drifting as a number in range [0, 255]. Too hight values might cause color leaps though.
	template <uint8_t speed = 125>
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
		return advance(255 - speed);
	}
}