#pragma once

#include "GeneratorBase.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// This generator chooses its colors among the given color presets.
	/// @tparam Selector - the selector that determines the index of the
	///                    color from the color preset.
	template <class Selector>
	class GeneratorColorPreset : public GeneratorBase
	{
	protected:
		typedef GeneratorBase super;
		Selector selector;
	public:
		GeneratorColorPreset();
		CRGB next();
	};

	template <class Selector>
	GeneratorColorPreset<Selector>::GeneratorColorPreset()
	{
		selector = Selector();
	}

	template <class Selector>
	CRGB GeneratorColorPreset<Selector>::next()
	{
		return Wordclock::getColorPreset(selector.next() % COLOR_PRESET_COUNT);
	}
}