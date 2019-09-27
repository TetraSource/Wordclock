#pragma once

#include "ModeFiller.hpp"
#include "ModeMaskBase.hpp"

namespace Wordclock
{
	/// Shows the time by turning on a certain amount of the display.
	/// The current and the two next color presets are used thereby.
	/// @tparam Mode - the mode used to print the "background".
	///                This mode must no inherit from another masking mode.
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
	/// @tparam direction - specifies the side whence the filling advances.
	template <class Mode, TimeTypes timeType, uint8_t scope,
		Directions direction>
	class ModeMaskFiller : public ModeMaskBase<ModeMaskFiller<
		Mode, timeType, scope, direction>, Mode, 500>
	{
	protected:
		typedef ModeMaskBase<ModeMaskFiller<
			Mode, timeType, scope, direction>, Mode, 500> super;
		
	public:
		void mask()
		{
			ModeFillerUtilities::modeFillerPaint(timeType, scope, direction);
		}
	};
}