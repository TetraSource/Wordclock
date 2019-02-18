#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

// The brightness of each glowing LEDs is divided by 2^DIMING_FAKTOR to
// let them appear darker than the shown time
#define DIMING_FAKTOR 3

namespace Wordclock
{
	class ModeGlowingBase : public ModeBase
	{
	protected:
		static uint8_t glowing[HEIGHT][WIDTH];

		static void setGlow(
			const uint8_t &x,
			const uint8_t &y,
			const uint8_t &glow);
	};

	/// turns on the LEDs around the activated ones using the a dimmed color
	/// to let them appear glowing.
	/// @tparam radius - the radius of the area which LEDs are turned on in
	///                  around activated LEDs.
	template <uint8_t radius = 1>
	class ModeGlowing : public ModeGlowingBase
	{
	public:
		void paint();
	};

	template <uint8_t radius>
	void ModeGlowing<radius>::paint()
	{
		Painter::setColor(CRGB(255, 255, 255));
		Utilities::printTime();

		int8_t setX, setY;
		for (int8_t y = 0; y < HEIGHT; y++) {
			for (int8_t x = 0; x < WIDTH; x++) {
				glowing[y][x] = 0;
			}
		}
		for (int8_t y = 0; y < HEIGHT; y++) {
			for (int8_t x = 0; x < WIDTH; x++) {
				if (Painter::getPixel(x, y).r != 0) {
					setGlow(x, y, radius + 1);
					for (int8_t i = 1; i <= radius; i++) {
						for (int8_t j = 0; j < i; j++) {
							setX = x + j - i;
							setY = y - j;
							if (setX >= 0 && setY >= 0)
								setGlow(setX, setY, radius - i + 1);

							setX = x + j;
							setY = y + j - i;
							if (setX < WIDTH && setY >= 0)
								setGlow(setX, setY, radius - i + 1);

							setX = x + i - j;
							setY = y + j;
							if (setX < WIDTH && setY < HEIGHT)
								setGlow(setX, setY, radius - i + 1);

							setX = x - j;
							setY = y + i - j;
							if (setX >= 0 && setY < HEIGHT)
								setGlow(setX, setY, radius - i + 1);
						}
					}
				}
			}
		}

		// calculated dimmed colors
		CRGB* colors = new CRGB[radius + 2];
		{
			colors[0] = CRGB(0, 0, 0);
			CRGB color = Wordclock::getCurrentPreset();
			for (int8_t i = 1; i <= radius; i++) {
				colors[i].r = ((color.r * i) / (radius + 1)) >> DIMING_FAKTOR;
				colors[i].g = ((color.g * i) / (radius + 1)) >> DIMING_FAKTOR;
				colors[i].b = ((color.b * i) / (radius + 1)) >> DIMING_FAKTOR;
			}
			colors[radius + 1] = color;
		}

		for (int8_t y = 0; y < HEIGHT; y++) {
			for (int8_t x = 0; x < WIDTH; x++) {
				Painter::setColor(colors[glowing[y][x]]);
				if (Painter::hasColor())
					Painter::paint(x, y);
			}
		}
		delete[] colors;
	}
}

#undef DIMING_FAKTOR