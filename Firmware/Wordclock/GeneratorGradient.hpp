#pragma once

#include "GeneratorBase.hpp"

namespace Wordclock
{
	class GeneratorGradientBase : public GeneratorBase
	{
	protected:
		typedef GeneratorBase super;
		static uint16_t rep;
		static int16_t rVal;
		static int16_t gVal;
		static int16_t bVal;
		static int16_t rVec;
		static int16_t gVec;
		static int16_t bVec;
		static GeneratorBase *lastGen;

		void internalSelect(const CRGB &nextColor);
		bool initVec(const CRGB &nextColor, const uint8_t &repFactor);
		CRGB advance();
	};

	/// generates a succession of similar colors so that they can be used for
	/// a gradient or fader.
	/// @tparam speed - the speed of the color drifting as a number in range
	///                 [0, 255]. Too hight values might cause color leaps
	///                 though.
	template <class Generator, uint8_t speed>
	class GeneratorGradient : public GeneratorGradientBase
	{
	protected:
		typedef GeneratorBase super;
		Generator gen;
	public:
		GeneratorGradient();
		CRGB next();
	};

	template <class Generator, uint8_t speed>
	GeneratorGradient<Generator, speed>::GeneratorGradient()
	{
		gen = Generator();
	}

	template <class Generator, uint8_t speed>
	CRGB GeneratorGradient<Generator, speed>::next()
	{
		if (lastGen != this) {
			lastGen = this;
			internalSelect(gen.next());
		}
		while (rep == 0 && initVec(gen.next(), 255 - speed));
		return advance();
	}
}