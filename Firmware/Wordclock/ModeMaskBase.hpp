#pragma once

#include "ModeBase.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// @tparam Mode - the underlying mode. This mode must not inherit
	///                from ModeMaskBase.
	/// @tparam interval - The time between each call to Wordclock::repaint()
	///                    Pass 0 to never have it called automatically.
	template <class Derived, class Mode, uint32_t interval>
	class ModeMaskBase : public ModeBase
	{
	protected:
		typedef ModeBase super;
		Mode mode;

		ModeMaskBase()
			: ModeBase()
		{
			mode = Mode();
		}

	public:
		void select()
		{
			if (isInTransition())
				ModeBase::select();
			else {
				if (interval != 0)
					Wordclock::startTimer(this, interval, transitionChannel);
				mode.select();
			}
		}

		void deselect()
		{
			if (isInTransition())
				ModeBase::deselect();
			else {
				if (interval != 0)
					Wordclock::cancelTimer(this, transitionChannel);
				mode.deselect();
			}
		}

		uint32_t timer(const uint8_t &channel)
		{
			if (isInTransition())
				return ModeBase::timer(channel);
			else if (interval != 0 && channel == transitionChannel) {
				Wordclock::repaint();
				((Derived*)this)->update();
				return interval;
			}
			else
				return mode.timer(channel);
		}

		void paint()
		{
			if (isInTransition()) {
				ModeBase::paint();
				return;
			}
			Painter::prepareReplacing();
			((Derived*)this)->mask();
			Painter::setReplacing(true);
			mode.paint();
			Painter::setReplacing(false);
		}

		void update()
		{
			Wordclock::repaint();
		}

		void mask()
		{}
	};
}