#include <Arduino.h>
#include "Timer.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	uint32_t *TimerItem::lastMillis = nullptr;

	TimerItem::TimerItem(const ModeBase *callback_, const uint8_t &channel_,
		const uint32_t &time)
		: end(time), channel(channel_), callback(callback_)
	{}

	TimerItem::TimerItem(const TimerItem &timer)
		: end(timer.end), channel(timer.channel), callback(timer.callback)
	{}

	bool TimerItem::operator < (const TimerItem &timer)
	{
		uint32_t ms = internalMillis();
		// If the current timer is up, assume it is smaller than all
		// other timers. Hence the first expression.
		return (uint32_t)(ms - end) < MAX_TRIGGER_DELAY ||
			(uint32_t)(end - ms) < (uint32_t)(timer.end - ms);
	}

#ifdef HEAP_DEBUG
	void printItem(const TimerItem *item)
	{
		Serial.print(item->end);
		Serial.print('\'');
		Serial.print(item->channel);
	}
#endif
}