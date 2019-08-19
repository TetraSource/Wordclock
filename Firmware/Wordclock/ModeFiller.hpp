#pragma once

#include "ModeBaseInterval.hpp"

namespace Wordclock
{
	class ModeFillerUtilities
	{
	private:
		static void modeFillerPaint(const CRGB &color, const Directions &,
			const TimeTypes &, uint8_t maxSecs);
	
	template <class, Directions, TimeTypes, uint8_t> friend class ModeFiller;
	};

	/// Shows the time by turning on a certain amount of the display.
	/// The current and the two next color presets are used thereby.
	/// @tparam direction - specifies the side whence the filling advances
	///                     (enum Wordclock::Directions).
	/// @tparam timeType - the unit of the time for the scope
	/// @tparam scope - the scope of time that is shown. If the time exceeds
	///                 this value, it will be displayed as 0 again.
	///                 This value never exceeds the maximal possible time for
	///                 the given time type. You can safely pass 255 to use
	///                 the maximal possible scope always.
	template <class Generator, Directions direction,
		TimeTypes timeType = Minutes, uint8_t scope = 255>
	class ModeFiller : public ModeBaseInterval<1000>
	{
	protected:
		typedef ModeBaseInterval<1000> super;
		Generator gen;
	public:
		ModeFiller();
		void paint();
	};

	template <class Generator, Directions direction,
		TimeTypes timeType, uint8_t scope>
	ModeFiller<Generator, direction, timeType, scope>::ModeFiller()
	{
		gen = Generator();
	}

	template <class Generator, Directions direction,
		TimeTypes timeType, uint8_t scope>
	void ModeFiller<Generator, direction, timeType, scope>::paint()
	{
		if (isInTransition())
			ModeBase::paint();
		else
			ModeFillerUtilities::modeFillerPaint(
				gen.next(), direction, timeType, scope);
	}
}
