#pragma once

#include "ModeColorBase.hpp"
#include "GeneratorBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// automatically changes the color once the timer elapses.
	/// @tparam time - the time between each selection of the next.
	template <uint32_t time>
	class ModeColorChangerTimer : public ModeColorBase
	{
	protected:
		GeneratorBase* gen;

	public:
		/// initializes the effect.
		/// @param generator - the color generator that select a new color when necessary.
		ModeColorChangerTimer(GeneratorBase* generator);

		void select();

		void timer();

		void paint();
	};

	template <uint32_t time>
	ModeColorChangerTimer<time>::ModeColorChangerTimer(
		GeneratorBase* generator)
	{
		gen = generator;
	}

	template <uint32_t time>
	void ModeColorChangerTimer<time>::select()
	{
		timer();
	}

	template <uint32_t time>
	void ModeColorChangerTimer<time>::timer()
	{
		currColor = gen->nextRGBColor();
		Wordclock::repaint();
		startTimer(time);
	}

	template <uint32_t time>
	void ModeColorChangerTimer<time>::paint()
	{
		Painter::setColor(currColor);
		Utilities::printTime();
	}
}