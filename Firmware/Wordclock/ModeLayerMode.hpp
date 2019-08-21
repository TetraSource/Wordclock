#pragma once

#include "ModeBase.hpp"
#include "Painter.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// Allows to change the current mode in the specified layer.
	/// @tparam layer - the layer whose mode is choosen. Layer 1 is the
	///                 first layer in the background. Note that the layer
	///                 in the foreground, layer 0, must not be changed by
	///                 this mode.
	/// @tparam from - the smallest index of a mode that can be selected.
	/// @tparam to - the biggest index of a mode that can be selected.
	///              If this value is bigger than MODE_COUNT,
	///              MODE_COUNT is used instead.
	template <uint8_t layer = 1, uint8_t from = 0, uint8_t to = 255>
	class ModeLayerMode : public ModeBase
	{
	protected:
		typedef ModeBase super;
	public:
		ModeLayerMode();
		void actionButton(const bool &);
		void paint();
	};

	template <uint8_t layer, uint8_t from, uint8_t to>
	ModeLayerMode<layer, from, to>::ModeLayerMode()
		: ModeBase()
	{}

	template <uint8_t layer, uint8_t from, uint8_t to>
	void ModeLayerMode<layer, from, to>::actionButton(const bool &inc)
	{
		Wordclock::setMode(layer,
			Wordclock::getNextMode(layer, inc, from, to));
	}

	template <uint8_t layer, uint8_t from, uint8_t to>
	void ModeLayerMode<layer, from, to>::paint()
	{
		if (isInTransition()) {
			ModeBase::paint();
		}
		else {
			Painter::setColor(Wordclock::getCurrentPreset());
			Utilities::printNumber(Wordclock::getMode(layer) + 1);
		}
	}
}