
#include "ModeEffect.hpp"

namespace Wordclock
{
	void ModeEffectBase::increment(const bool &inc)
	{
		Wordclock::setEffectIndex(
			Utilities::changeValue(Wordclock::getEffectIndex(), EFFECT_COUNT, inc));
	}
}