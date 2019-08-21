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
		ModeBrightness();
		void actionButton(const bool &inc);
		void paint();
	};

	// implementation //

	template <uint8_t levelCount>
	ModeBrightness<levelCount>::ModeBrightness()
		: ModeBase()
	{}

	template <uint8_t levelCount>
	void ModeBrightness<levelCount>::actionButton(const bool &inc)
	{
		if (!isInTransition())
			Wordclock::setBrightness(Utilities::changeLevel(
				Wordclock::getBrightness(), levelCount, inc));
	}

	template <uint8_t levelCount>
	void ModeBrightness<levelCount>::paint()
	{
		if (isInTransition()) {
			ModeBase::paint();
		}
		else {
			Painter::setColor(Wordclock::getCurrentPreset());
			Utilities::printHex(
				Utilities::getLevel(Wordclock::getBrightness(), levelCount)+1);
		}
	}
}