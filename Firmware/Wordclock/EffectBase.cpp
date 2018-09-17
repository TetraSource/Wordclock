
#include "EffectBase.hpp"

namespace Wordclock
{
	uint32_t EffectBase::updateTime = 0;
	uint32_t EffectBase::updateThreshold = 0;

	void EffectBase::startTimer(const uint32_t &time)
	{
		updateTime = time;
		updateThreshold = millis();
	}

	void EffectBase::cancelTimer()
	{
		updateTime = 0;
	}

	void EffectBase::select()
	{}

	void EffectBase::deselect()
	{}

	void EffectBase::update()
	{}
}