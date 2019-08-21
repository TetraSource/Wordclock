#pragma once

#include "ModeBase.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeLayerModeSwitcherBase : public ModeBase
	{
	protected:
		typedef ModeBase super;
		bool active;
		ModeLayerModeSwitcherBase(const bool &active_);
	public:
		void paint();
	};

	/// Selects all interval milliseconds a (new) mode on the specified layer.
	/// You can turn activate the mode with the increment button and
	/// decrement it with the decrement button.
	/// @tparam Selector - the selector that performs the selection of the
	///                    index of the mode.
	/// @tparam interval - the time between each new selection in milliseconds.
	/// @tparam layer - the layer the mode is selected on.
	/// @tparam initActive - whether this mode is active from the very beginning
	///                      or not.
	template <class Selector, uint32_t interval, uint8_t layer,
		bool initActive = false>
	class ModeLayerModeSwitcher : public ModeLayerModeSwitcherBase
	{
	protected:
		typedef ModeLayerModeSwitcherBase super;
		Selector selector;
	public:
		ModeLayerModeSwitcher();
		void actionButton(const bool &inc);
		uint32_t timer(const uint8_t &channel);
	};

	template <class Selector, uint32_t interval, uint8_t layer, bool initActive>
	ModeLayerModeSwitcher<Selector, interval, layer, initActive>::
		ModeLayerModeSwitcher() : ModeLayerModeSwitcherBase(initActive)
	{
		selector = Selector();
		if (initActive)
			Wordclock::startTimer(this, interval, 0);
	}

	template <class Selector, uint32_t interval, uint8_t layer, bool initActive>
	void ModeLayerModeSwitcher<Selector, interval, layer, initActive>::
		actionButton(const bool &inc)
	{
		if (active == inc)
			return;
		active = inc;
		if (active)
			Wordclock::startTimer(this, interval, 0);
		else
			Wordclock::cancelTimer(this, 0);
		Wordclock::repaint();
	}

	template <class Selector, uint32_t interval, uint8_t layer, bool initActive>
	uint32_t ModeLayerModeSwitcher<Selector, interval, layer, initActive>::
		timer(const uint8_t &channel)
	{
		if (channel != 0)
			return ModeBase::timer(channel);
		Wordclock::setMode(layer, selector.next() % MODE_COUNT);
		return interval;
	}
}