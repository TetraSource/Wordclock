#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// The default mode that doesn't allow to do anything.
	/// @tparam letter the position of the letter that represents the mode,
	///                calculated by the POINT macro.
	template <uint8_t letter>
	class ModeDefault : public ModeBase
	{
	public:
		void paint();
	};

	// implementation //

	template <uint8_t letter>
	void ModeDefault<letter>::paint()
	{
		Utilities::printTime();

#ifdef SHOW_MODE
		Painter::paint(letter);
#endif
	}
}