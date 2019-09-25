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
		ModeCoffee();
		uint32_t timer(const uint8_t &channel);
		void paint();
	};

	template <class Generator, uint32_t speed>
	ModeCoffee<Generator, speed>::ModeCoffee()
		: ModeBaseInterval<speed>()
	{
		gen = Generator();
		impl = ModeCoffeeImpl();
	}

	template <class Generator, uint32_t speed>
	uint32_t ModeCoffee<Generator, speed>::timer(const uint8_t &channel)
	{
		if (channel == 0)
			impl.update();
		return super::timer(channel);
	}
	
	template <class Generator, uint32_t speed>
	void ModeCoffee<Generator, speed>::paint()
	{
		Painter::setColor(gen.next());
		impl.paint();
	}
}