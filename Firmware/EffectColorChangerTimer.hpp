#pragma once

#include "EffectColorBase.hpp"
#include "GeneratorBase.hpp"
#include "Painter.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class Painter;

	/// automatically changes the color once the timer elapses.
	/// @tparam time - the time between each selection of the next.
	template <uint32_t time>
	class EffectColorChangerTimer : public EffectColorBase
	{
	protected:
		GeneratorBase* gen;

	public:
		/// initializes the effect.
		/// @param generator - the color generator that select a new color when necessary.
		EffectColorChangerTimer(GeneratorBase* generator);

		void select();

		void update();

		void paint(Painter* painter);
	};

	template <uint32_t time>
	EffectColorChangerTimer<time>::EffectColorChangerTimer(GeneratorBase* generator)
	{
		gen = generator;
	}

	template <uint32_t time>
	void EffectColorChangerTimer<time>::select()
	{
		update();
	}

	template <uint32_t time>
	void EffectColorChangerTimer<time>::update()
	{
		currColor = gen->nextRGBColor();
		Wordclock::repaint();
		startTimer(time);
	}

	template <uint32_t time>
	void EffectColorChangerTimer<time>::paint(Painter* painter)
	{
		painter->setForeground(currColor);
		painter->markAll();
	}
}