#pragma once

#include "EffectColorBase.hpp"
#include "GeneratorBase.hpp"
#include "Painter.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class EffectColorChangerTimeBase : public EffectColorBase
	{
	protected:
		static uint8_t lastTime;
		static uint8_t advances;
	};

	/// automatically changes the color once a certain time type advances.
	/// @tparam timeType - the unit of the time type that has to advance in order to change the color preset.
	/// @tparam threshold - how often the time has to advance before the color preset changes.
	template <TimeTypes timeType = Minutes, uint8_t threshold = 1>
	class EffectColorChangerTime : public EffectColorChangerTimeBase
	{
	protected:
		GeneratorBase* gen;

	public:
		/// initializes the effect.
		/// @param generator - the color generator that select a new color when necessary.
		EffectColorChangerTime(GeneratorBase* generator);

		void select();

		void update();

		void paint(Painter* painter);
	};

	template <TimeTypes timeType, uint8_t threshold>
	EffectColorChangerTime<timeType, threshold>::EffectColorChangerTime(GeneratorBase* generator)
	{
		gen = generator;
	}

	template <TimeTypes timeType, uint8_t threshold>
	void EffectColorChangerTime<timeType, threshold>::select()
	{
		lastTime = Wordclock::getTime(timeType);
		advances = 0;
		startTimer(1000);
	}

	template <TimeTypes timeType, uint8_t threshold>
	void EffectColorChangerTime<timeType, threshold>::update()
	{
		if ((Wordclock::getTime(timeType) - lastTime) > 0) {
			lastTime = Wordclock::getTime(timeType);
			if (++advances >= threshold) {
				advances = 0;
				currColor = gen->nextRGBColor();
				Wordclock::repaint();
			}
		}
		startTimer(1000);
	}

	template <TimeTypes timeType, uint8_t threshold>
	void EffectColorChangerTime<timeType, threshold>::paint(Painter* painter)
	{
		painter->setForeground(currColor);
		painter->markAll();
	}
}