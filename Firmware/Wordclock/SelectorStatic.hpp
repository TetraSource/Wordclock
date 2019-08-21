#pragma once

#include "SelectorBase.hpp"

namespace Wordclock
{
	/// Returns the same number always
	/// @tparam number - the number to be returned.
	template <STYPE number>
	class SelectorStatic : public SelectorBase
	{
	protected:
		typedef SelectorBase super;
	public:
		SelectorStatic();
		STYPE next();
	};

	template <STYPE number>
	SelectorStatic<number>::SelectorStatic()
	{}

	template <STYPE number>
	STYPE SelectorStatic<number>::next()
	{
		return number;
	}
}