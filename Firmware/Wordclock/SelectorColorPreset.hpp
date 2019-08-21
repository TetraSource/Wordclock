#pragma once

#include "SelectorBase.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// Always returns the current color preset index plus a certain offset.
	/// @tparam offset - the optional offset to the current color preset index.
	template <STYPE offset = 0>
	class SelectorColorPreset : public SelectorBase
	{
	protected:
		typedef SelectorBase super;
	public:
		SelectorColorPreset();
		STYPE next();
	};

	template <STYPE offset>
	SelectorColorPreset<offset>::SelectorColorPreset()
	{}

	template <STYPE offset>
	STYPE SelectorColorPreset<offset>::next()
	{
		return Wordclock::getColorPresetIndex() + offset;
	}
}