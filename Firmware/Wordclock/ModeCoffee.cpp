#include "ModeCoffee.hpp"

namespace Wordclock
{
	ModeCoffeeImpl::ModeCoffeeImpl()
	{
		state = 0;
	}

	void ModeCoffeeImpl::update()
	{
		state = (state+1) % 15;
		Wordclock::repaint();
	}

	void ModeCoffeeImpl::paint()
	{
		// mug
		Painter::paint(3, 4, 5, 5);
		Painter::paint(4, 9, 3);
		// handle
		Painter::paint(8, 5);
		Painter::paint(8, 8);
		Painter::paint(9, 5, 1, 4);
		// steam
		if (state < 8 || state > 13) {
			Painter::paint(2, 0);
			Painter::paint(3, 1, 1, 2);
		}
		else {
			Painter::paint(2, 0, 1, 2);
			Painter::paint(3, 2);
		}
		switch (state) {
		case 0:
			Painter::paint(5, 2);
		case 1:
			Painter::paint(5, 0, 1, 2);
			Painter::paint(7, 0, 1, 2);
			break;
		case 2:
			Painter::paint(7, 1);
		case 3:
			Painter::paint(5, 0, 1, 2);
			Painter::paint(8, 0);
			break;
		case 4:
			Painter::paint(5, 0, 1, 2);
			break;
		case 5:
			Painter::paint(5, 0);
			break;
		case 6:
			break;
		case 9:
			Painter::paint(5, 2);
		case 7:
		case 8:
			Painter::paint(7, 2);
			break;
		case 10:
			Painter::paint(5, 2);
			Painter::paint(7, 1, 1, 2);
			break;
		case 11:
			Painter::paint(5, 1, 1, 2);
			Painter::paint(7, 1, 1, 2);
			break;
		case 12:
			Painter::paint(5, 1, 1, 2);
			Painter::paint(7, 0, 1, 3);
		case 13:
		case 14:
			Painter::paint(5, 1, 1, 2);
			Painter::paint(7, 0, 1, 2);
		}
	}
}