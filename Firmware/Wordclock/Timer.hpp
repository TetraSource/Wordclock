#pragma once

#include "Config.hpp"
#include "Heap.hpp"
#include "ModeBase.hpp"

namespace Wordclock
{
	class TimerItem
	{
	public:
		uint32_t end;
		uint8_t channel;
		ModeBase *callback;

		TimerItem(const ModeBase *callback, const uint8_t &channel,
			const uint32_t &time);
		TimerItem(const TimerItem &timer);

		bool operator < (const TimerItem &timer);
	};

	extern uint32_t internalMillis();
	void printItem(const TimerItem *item);

	template<typename SizeType>
	class TimerHeap : protected Heap<SizeType, TimerItem, true>
	{
	protected:
		typedef Heap<SizeType, TimerItem, true> super;
	public:
		// Time may be at most 2^32 - MAX_TRIGGER_DELAY instead of 2^32.
		void start(const ModeBase *callback, const uint8_t &channel,
			const uint32_t &time);
		// If channel == 255, all timers with the given callback are canceled
		// independently of their actual channel. 
		void cancel(const ModeBase *callback,
			const uint8_t &channel, const bool &all);
		// If channel == 255, all timers with the given callback are canceled
		// independently of their actual channel.
		void extend(const uint32_t &time, const ModeBase *callback,
			const uint8_t &channel, const bool &all);
		// Triggers all expired timers. This method must be called at least
		// once all MAX_TRIGGER_DELAY milliseconds.
		void update();
	};

	template<typename SizeType>
	void TimerHeap<SizeType>::start(const ModeBase *callback_,
		const uint8_t &channel_, const uint32_t &time_)
	{
		if (channel_ == 255)
			return;
		insert(TimerItem(callback_, channel_, internalMillis() +
			(time_ > (uint32_t)(0 - MAX_TRIGGER_DELAY) ?
			(uint32_t)(0 - MAX_TRIGGER_DELAY) : time_)));
#ifdef HEAP_DEBUG
		Serial.println("i"); print(&printItem);
#endif
	}

	template<typename SizeType>
	void TimerHeap<SizeType>::cancel(
		const ModeBase *callback, const uint8_t &channel, const bool &all)
	{
		SizeType i = 0;
		while (i < this->size) {
			if ((channel == 255 || channel == this->items[i]->channel) &&
				callback == this->items[i]->callback)
			{
				removeRoot(i);
				if (!all)
					break;
			}
			else
				i++;
		}
#ifdef HEAP_DEBUG
		Serial.print("c\'"); Serial.println(channel);
		print(&printItem);
#endif
	}

	template<typename SizeType>
	void TimerHeap<SizeType>::extend(const uint32_t &time,
		const ModeBase *callback, const uint8_t &channel, const bool &all)
	{
		for (SizeType i = 0; i < this->size; i++) {
			if ((channel == 255 || channel == this->items[i]->channel) &&
				callback == this->items[i]->callback)
			{
				this->items[i]->end += time;
				heapifyDown(i);
				if (!all)
					break;
			}
		}
#ifdef HEAP_DEBUG
		Serial.print(time); Serial.print(" e\'"); Serial.println(channel);
		print(&printItem);
#endif
	}

	template<typename SizeType>
	void TimerHeap<SizeType>::update()
	{
		while (true) {
			TimerItem *item = this->items[0];
			if (isEmpty() ||
				((uint32_t)(internalMillis() - item->end)) >= MAX_TRIGGER_DELAY)
				return;
#ifdef HEAP_DEBUG
			Serial.print("u "); Serial.println(internalMillis());
			print(&printItem);
#endif
			uint32_t refresh = item->callback->timer(item->channel);
			if (refresh == 0) {
				cancel(item->callback, item->channel, false);
			}
			else {
				extend(refresh, item->callback, item->channel, false);
			}
		}
	}
}