#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// allows to set the brightness of all LEDs gradually.
	/// @tparam levelCount - the count of grades of brightness the user
	///                      can choose from.
	template <uint8_t levelCount>
	class ModeBrightness : public ModeBase
	{
	protected:
		typedef ModeBase super;

	public:
		void actionButton(const bool &inc)
		{
			if (!isInTransition())
				Wordclock::setBrightness(Utilities::changeLevel(
					Wordclock::getBrightness(), levelCount, inc));
		}

		void paint()
		{
			if (isInTransition()) {
				ModeBase::paint();
			}
			else {
				Painter::setColor(Wordclock::getCurrentPreset());
				Utilities::printHex(Utilities::getLevel(
					Wordclock::getBrightness(), levelCount)+1);
			}
		}
	};
}