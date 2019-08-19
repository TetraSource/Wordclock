#pragma once

#include "GeneratorBase.hpp"

namespace Wordclock
{
	/// This class operates under the assumption that the color generator
	/// Generator generates HSV colors that are stored in CRGB objects.
	/// Those objects are converted into RGB colors stored in CRGB objects.
	/// @tparam Generator - the underlying generator whose colors are converted
	///                     into RGB colors.
	template <class Generator>
	class GeneratorRGBAdapter : public GeneratorBase
	{
	protected:
		typedef GeneratorBase super;
		Generator gen;
	public:
		GeneratorRGBAdapter();
		CRGB next();
	};

	template <class Generator>
	GeneratorRGBAdapter<Generator>::GeneratorRGBAdapter()
	{
		gen = Generator();
	}

	template <class Generator>
	CRGB GeneratorRGBAdapter<Generator>::next()
	{
		CRGB color = gen.next();
		hsv2rgb_rainbow(CHSV(color.r, color.g, color.b), color);
		return color;
	}
}