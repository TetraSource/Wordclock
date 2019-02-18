#pragma once

#include "GeneratorBase.hpp"
#include "ModeColorBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

// the amount of milliseconds that pass between each update of the
// current color.
#define UPDATE_INTERVAL 1000

namespace Wordclock
{
	class ModeColorChangerTimeBase : public ModeColorBase
	{
	protected:
		static uint8_t lastTime;
		static uint8_t advances;
	};

	/// automatically changes the color once a certain time type advances.
	/// @tparam timeType - the unit of the time type that has to advance
	///                    in order to change the color preset.
	/// @tparam threshold - how often the time has to advance before the
	///                     color preset changes.
	template <TimeTypes timeType = Minutes, uint8_t threshold = 1>
	class ModeColorChangerTime : public ModeColorChangerTimeBase
	{
	protected:
		GeneratorBase* gen;

	public:
		/// initializes the effect.
		/// @param generator - the color generator that select a new color
		///                    when necessary.
		ModeColorChangerTime(GeneratorBase* generator);

		void select();

		void timer();

		void paint();
	};

	template <TimeTypes timeType, uint8_t threshold>
	ModeColorChangerTime<timeType, threshold>::ModeColorChangerTime(
		GeneratorBase* generator)
	{
		gen = generator;
	}

	template <TimeTypes timeType, uint8_t threshold>
	void ModeColorChangerTime<timeType, threshold>::select()
	{
		lastTime = Wordclock::getTime(timeType);
		advances = 0;
		currColor = gen->nextRGBColor();
		startTimer(UPDATE_INTERVAL);
	}

	template <TimeTypes timeType, uint8_t threshold>
	void ModeColorChangerTime<timeType, threshold>::timer()
	{
		if ((Wordclock::getTime(timeType) - lastTime) > 0) {
			lastTime = Wordclock::getTime(timeType);
			if (++advances >= threshold) {
				advances = 0;
				currColor = gen->nextRGBColor();
				Wordclock::repaint();
			}
		}
		startTimer(UPDATE_INTERVAL);
	}

	template <TimeTypes timeType, uint8_t threshold>
	void ModeColorChangerTime<timeType, threshold>::paint()
	{
		Painter::setColor(currColor);
		Utilities::printTime();
	}
}

#undef UPDATE_INTERVAL
