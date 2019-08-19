#pragma once

#include "ModeColorChangerBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

#define UPDATE_INTERVAL 500

namespace Wordclock
{
	/// automatically changes the color once a certain time type advances.
	/// @tparam Generator - the color generator that select a new color
	///                     when necessary.
	/// @tparam threshold - how often the time has to advance before the
	///                     color preset changes.
	/// @tparam timeType - the unit of the time type that has to advance
	///                    in order to change the color preset.
	/// If threshold == 5 and timeType == Minutes, the color changes all
	/// five minutes for example.
	template <class Generator, uint8_t threshold, TimeTypes timeType>
	class ModeColorChangerTime : public ModeColorChangerBase
	{
	protected:
		typedef ModeColorChangerBase super;
		Generator gen;
		uint8_t lastTime;

	public:
		ModeColorChangerTime();
		void select();
		uint32_t timer(const uint8_t &channel);
	};

	template <class Generator, uint8_t threshold, TimeTypes timeType>
	ModeColorChangerTime<Generator, threshold, timeType>::ModeColorChangerTime()
		: ModeColorChangerBase()
	{
		gen = Generator();
	}

	template <class Generator, uint8_t threshold, TimeTypes timeType>
	void ModeColorChangerTime<Generator, threshold, timeType>::select()
	{
		if (isInTransition()) {
			ModeBase::select();
		}
		else {
			currColor = gen.next();
			lastTime = Wordclock::getTime(timeType);
			Wordclock::startTimer(this, UPDATE_INTERVAL, 0);
		}
	}

	template <class Generator, uint8_t threshold, TimeTypes timeType>
	uint32_t ModeColorChangerTime<Generator, threshold, timeType>::timer(
		const uint8_t &channel)
	{
		if (isInTransition()) {
			return ModeBase::timer(channel);
		}
		if ((Wordclock::getTime(timeType) - lastTime) >= threshold) {
			lastTime = Wordclock::getTime(timeType);
			currColor = gen.next();
			Wordclock::repaint();
		}
		return UPDATE_INTERVAL;
	}
}