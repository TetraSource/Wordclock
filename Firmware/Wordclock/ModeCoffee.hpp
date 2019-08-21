#pragma once

#include "ModeMaskBase.hpp"

namespace Wordclock
{
	template <class Mode, uint32_t speed>
	class ModeCoffee : public ModeMaskBase<Mode, speed>
	{
	protected:
		typedef ModeMaskBase<Mode, speed> super;
		uint8_t state;
	public:
		ModeCoffee();
		void update();
		void mask();
	};

	template <class Mode, uint32_t speed>
	ModeCoffee<Mode, speed>::ModeCoffee()
		: ModeMaskBase<Mode, speed>()
	{
		state = 0;
	}

	template <class Mode, uint32_t speed>
	void ModeCoffee<Mode, speed>::update()
	{
		state = (state+1) % 5;
		Wordclock::repaint();
	}
	
	template <class Mode, uint32_t speed>
	void ModeCoffee<Mode, speed>::mask()
	{
		// cup
		Painter::paint(3, 4, 5, 5);
		Painter::paint(4, 9, 3);
		// handle
		Painter::paint(8, 5);
		Painter::paint(8, 8);
		Painter::paint(9, 5, 1, 4);
		// steam
		if (state == 0 || state == 4) {
			Painter::paint(2, 0);
			Painter::paint(3, 1, 1, 2);
		}
		else {
			Painter::paint(2, 0, 1, 2);
			Painter::paint(3, 2);
		}
		switch (state) {
		case 0:
			Painter::paint(5, 0, 1, 3);
			Painter::paint(7, 0, 1, 2);
			break;
		case 1:
			Painter::paint(5, 0, 1, 2);
			Painter::paint(8, 0);
			break;
		case 2:
			Painter::paint(5, 0);
			break;
		case 3:
			Painter::paint(5, 2);
			Painter::paint(7, 2);
			break;
		case 4:
			Painter::paint(5, 1, 1, 2);
			Painter::paint(7, 1, 1, 2);
		}
	}
}