#pragma once

#include "ModeBaseInterval.hpp"

namespace Wordclock
{
	/// The default mode that just displays the current time with a precision
	/// of five minutes.
	class ModeWordclock : public ModeTimeBound
	{
	protected:
		typedef ModeBase super;
	public:
		void paint();
	};
}