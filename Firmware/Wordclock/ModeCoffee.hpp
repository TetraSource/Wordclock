#pragma once

#include "ModeBaseInterval.hpp"

namespace Wordclock
{
	class ModeCoffeeImpl {
	protected:
		uint8_t state;
	public:
		ModeCoffeeImpl();
		void update();
		void paint();
	};

	/// Displays a coffee mug with moving steam.
	/// @tparam Generator - generates the colors of the picture.
	/// @tparam speed - the time between each movement of the steam in
	///                 milliseconds.
	template <class Generator, uint32_t speed>
	class ModeCoffee : public ModeBaseInterval<speed>
	{
	protected:
		typedef ModeBaseInterval<speed> super;
		Generator gen;
		ModeCoffeeImpl impl;
		
	public:
		ModeCoffee()
			: ModeBaseInterval<speed>()
		{
			gen = Generator();
			impl = ModeCoffeeImpl();
		}

		uint32_t timer(const uint8_t &channel)
		{
			if (channel == 0)
				impl.update();
			return super::timer(channel);
		}

		void paint()
		{
			Painter::setColor(gen.next());
			impl.paint();
		}
	};
}