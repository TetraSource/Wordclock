#pragma once

#include "SelectorBase.hpp"

namespace Wordclock
{
	/// Selects a random number out of the given range
	/// @tparam from - the lower border of the range.
	/// @tparam to - the index of the element behind upper border of the range.
	template <STYPE from = 0, STYPE to = 255>
	class SelectorRandom : public SelectorBase
	{
	protected:
		typedef SelectorBase super;
	public:
		SelectorRandom();
		STYPE next();
	};

	template <STYPE from, STYPE to>
	SelectorRandom<from, to>::SelectorRandom()
	{}

	template <STYPE from, STYPE to>
	STYPE SelectorRandom<from, to>::next()
	{
		return random(from, to);
	}
}