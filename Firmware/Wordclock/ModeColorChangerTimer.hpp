#pragma once

#include "GeneratorBase.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// automatically changes the color once the timer elapses.
	/// @tparam Generator - the color generator that selects a new color
	///                     when necessary.
	/// @tparam time - the time between each selection of the next.
	template <class Generator, uint32_t time>
	class ModeColorChangerTimer : public ModeColorChangerBase
	{
	protected:
		typedef ModeColorChangerBase super;
		Generator gen;

	public:
		ModeColorChangerTimer();
		uint32_t timer(const uint8_t &);
	};

	template <class Generator, uint32_t time>
	ModeColorChangerTimer<Generator, time>::ModeColorChangerTimer()
		: ModeColorChangerBase()
	{
		gen = Generator();
	}

	template <class Generator, uint32_t time>
	uint32_t ModeColorChangerTimer<Generator, time>::timer(
		const uint8_t &channel)
	{
		if (isInTransition()) {
			return ModeBase::timer(channel);
		}
		else {
			currColor = gen.next();
			Wordclock::repaint();
			return time;
		}
	}
}