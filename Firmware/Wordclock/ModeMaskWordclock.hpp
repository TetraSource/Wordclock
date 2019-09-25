#pragma once

#include "ModeMaskBase.hpp"
#include "Utilities.hpp"

namespace Wordclock
{
	/// Just displays the current time with a precision of five minutes by
	/// printing the mode first and masking it then at the time indicating
	/// pixels.
	/// @tparam Mode - the mode used to print the "background".
	///                This mode must no inherit from another masking mode.
	template <class Mode>
	class ModeMaskWordclock
		: public ModeMaskBase<ModeMaskWordclock<Mode>, Mode, 500>
	{
	protected:
		typedef ModeMaskBase<ModeMaskWordclock, Mode, 500> super;
	public:
		void mask();
	};

	template <class Mode>
	void ModeMaskWordclock<Mode>::mask()
	{
		Utilities::printTime();
	}
}