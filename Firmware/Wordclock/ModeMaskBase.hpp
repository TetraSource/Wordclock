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
		ModeMaskBase();
	public:
		void deselect();
		void select();
		uint32_t timer(const uint8_t &channel);
		void paint();
		void update();
		void mask();
	};

	template <class Derived, class Mode, uint32_t interval>
	ModeMaskBase<Derived, Mode, interval>::ModeMaskBase()
		: ModeBase()
	{
		mode = Mode();
	}

	template <class Derived, class Mode, uint32_t interval>
	void ModeMaskBase<Derived, Mode, interval>::select()
	{
		if (isInTransition())
			ModeBase::select();
		else {
			if (interval != 0)
				Wordclock::startTimer(this, interval, transitionChannel);
			mode.select();
		}
	}

	template <class Derived, class Mode, uint32_t interval>
	void ModeMaskBase<Derived, Mode, interval>::deselect()
	{
		if (isInTransition())
			ModeBase::deselect();
		else {
			if (interval != 0)
				Wordclock::cancelTimer(this, transitionChannel);
			mode.deselect();
		}
	}

	template <class Derived, class Mode, uint32_t interval>
	uint32_t ModeMaskBase<Derived, Mode, interval>::timer(const uint8_t &channel)
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

	template <class Derived, class Mode, uint32_t interval>
	void ModeMaskBase<Derived, Mode, interval>::paint()
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

	template <class Derived, class Mode, uint32_t interval>
	void ModeMaskBase<Derived, Mode, interval>::update()
	{
		Wordclock::repaint();
	}

	template <class Derived, class Mode, uint32_t interval>
	void ModeMaskBase<Derived, Mode, interval>::mask()
	{}
}