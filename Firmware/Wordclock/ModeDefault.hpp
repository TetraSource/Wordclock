#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// The default mode that doesn't allow to do anything.
	/// @tparam letter - the letter that represents the mode.
	template <char letter>
	class ModeDefault : public ModeBase
	{
	public:
		void paint();
	};

	// implementation //

	template <char letter>
	void ModeDefault<letter>::paint()
	{
		Utilities::printTime();

#ifdef SHOW_MODE
		Painter::paint(letter);
#endif
	}
}