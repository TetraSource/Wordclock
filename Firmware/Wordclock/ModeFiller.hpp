#pragma once

#include "ModeBaseInterval.hpp"

namespace Wordclock
{
	class ModeFillerUtilities
	{
	private:
		static void modeFillerPaint(const Directions &,
			const TimeTypes &timeType, const uint8_t &scope);
	
	template <class, Directions, TimeTypes, uint8_t> friend class ModeFiller;
	};

	/// Shows the time by turning on a certain amount of the display.
	/// The current and the two next color presets are used thereby.
	/// @tparam Generator - generates the colors for the filled area.
	/// @tparam direction - specifies the side whence the filling advances.
	/// @tparam timeType - the unit of the time that determines how great
	///                    the percentage of filled area is.
	/// @tparam scope - specifies what time is equal to a 100% filled area.
	///                 If your time type is Minutes for instance and you set
	///                 scope to 5, then the entire area is filled right
	///                 before the 5th minutes is complete. When it is
	///                 complete it is filled by 0% again. Same applies to the
	///                 10th, 15th, ... minute - so its basically a modulo.
	///                 This value never exceeds the maximal possible value for
	///                 the given time type, so you can set it to 255 to use
	///                 the entire scope of the specified time type.
	///                 This value should always divide the maximal possible
	///                 value for the given time type (5 minutes divide 60
	///                 minutes for instance). If the maximal value is not
	///                 static - as with days (January has more than February),
	///                 then you should set it to 255 always.
	template <class Generator, Directions direction,
		TimeTypes timeType, uint8_t scope = 255>
	class ModeFiller : public ModeTimeBound
	{
	protected:
		typedef ModeTimeBound super;
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
		else {
			Painter::setColor(gen.next());
			ModeFillerUtilities::modeFillerPaint(direction, timeType, scope);
		}
	}
}
