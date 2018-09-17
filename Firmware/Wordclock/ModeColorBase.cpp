
#include "ModeColorBase.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	uint8_t ModeColorBase::prevMode = 0;

	void ModeColorBase::select()
	{
		prevMode = Wordclock::getEffectIndex();
		Wordclock::setEffectIndex(0);
	}

	void ModeColorBase::deselect()
	{
		Wordclock::setEffectIndex(prevMode);
	}
}