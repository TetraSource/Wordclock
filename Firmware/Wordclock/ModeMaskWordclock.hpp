#pragma once

#include "ModeMaskBase.hpp"
#include "Utilities.hpp"

namespace Wordclock
{
	/// Just displays the current time with a precision of five minutes by
	/// printing the mode first and masking it then at the time indicating
	/// pixels.
	/// @tparam Mode - the mode used to print the "background".
	///                This mode must no inherit from another masing mode.
	template <class Mode>
	class ModeMaskWordclock : public ModeMaskBase<Mode, 500>
	{
	protected:
		typedef ModeMaskBase<Mode, 500> super;
	public:
		ModeMaskWordclock();
		void mask();
	};

	template <class Mode>
	ModeMaskWordclock<Mode>::ModeMaskWordclock()
		: ModeMaskBase<Mode, 500>()
	{}

	template <class Mode>
	void ModeMaskWordclock<Mode>::mask()
	{
		Utilities::printTime();
	}
}