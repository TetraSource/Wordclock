#pragma once

#include "SelectorBase.hpp"

namespace Wordclock
{
	/// Selects successively numbers out of a fixed range.
	/// @tparam from - the lower border of the range.
	/// @tparam to - the index of the element behind upper border of the range.
	/// @tparam increment - specifies the incremention of the number to
	///                     get to the next number.
	/// @tparam init - the first value returned by this selector.
	template <STYPE from = 0, STYPE to = 255,
		SIGNED_STYPE increment = 1, STYPE init = 0>
	class SelectorSuccessive : public SelectorBase
	{
	protected:
		typedef SelectorBase super;
		STYPE curr;
	public:
		SelectorSuccessive();
		STYPE next();
	};

	template <STYPE from, STYPE to,
		SIGNED_STYPE increment, STYPE init>
	SelectorSuccessive<from, to, increment, init>::
		SelectorSuccessive()
	{
		curr = init - increment;
	}

	template <STYPE from, STYPE to,
		SIGNED_STYPE increment, STYPE init>
	STYPE SelectorSuccessive<from, to, increment, init>::next()
	{
		curr = ((STYPE)(curr + increment)) % (to - from);
		return curr + from;
	}
}