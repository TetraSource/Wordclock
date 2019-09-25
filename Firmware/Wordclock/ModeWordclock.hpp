#pragma once

#include "ModeBaseInterval.hpp"
#include "Utilities.hpp"

namespace Wordclock
{
	/// Displays the current time with a precision of five minutes using
	/// the Words on the display.
	/// @tparam Generator - generates the colors of the text.
	template <class Generator>
	class ModeWordclock : public ModeTimeBound
	{
	protected:
		typedef ModeBase super;
		Generator gen;
	public:
		ModeWordclock();
		void paint();
	};

	template <class Generator>
	ModeWordclock<Generator>::ModeWordclock()
	{
		gen = Generator();
	}

	template <class Generator>
	void ModeWordclock<Generator>::paint()
	{
		if (isInTransition()) {
			ModeBase::paint();
		}
		else {
			Painter::setColor(gen.next());
			Utilities::printTime();
		}
	}
}