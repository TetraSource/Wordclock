#pragma once

#include "EffectBase.hpp"
#include "Painter.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class EffectGlowingBase : public EffectBase
	{
	protected:
		static uint8_t glowing[HEIGHT][WIDTH];

		static void setGlow(const uint8_t &x, const uint8_t &y, const uint8_t &glow);
	};

	/// turns on the LEDs around the activated ones using the a dimmed color to let them appear glowing.
	/// @tparam radius - the radius of the area which LEDs are turned on in around activated LEDs.
	template <uint8_t radius = 1>
	class EffectGlowing : public EffectGlowingBase
	{
	public:
		void paint(Painter* painter);
	};

	template <uint8_t radius>
	void EffectGlowing<radius>::paint(Painter* painter)
	{
		int8_t setX, setY;
		for (int8_t y = 0; y < HEIGHT; y++) {
			for (int8_t x = 0; x < WIDTH; x++) {
				glowing[y][x] = 0;
			}
		}
		for (int8_t y = 0; y < HEIGHT; y++) {
			for (int8_t x = 0; x < WIDTH; x++) {
				if (painter->isForeground(x, y)) {
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

		CRGB* colors = new CRGB[radius + 2];
		{
			colors[0] = CRGB(0, 0, 0);
			CRGB color = Wordclock::getCurrentPreset();
			for (int8_t i = 1; i <= radius; i++) {
				colors[i].r = ((color.r * i * i) / ((radius + 1) * (radius + 1))) >> 1;
				colors[i].g = ((color.g * i * i) / ((radius + 1) * (radius + 1))) >> 1;
				colors[i].b = ((color.b * i * i) / ((radius + 1) * (radius + 1))) >> 1;
			}
			colors[radius + 1] = color;
			painter->setForeground(color);
		}

		for (int8_t y = 0; y < HEIGHT; y++) {
			for (int8_t x = 0; x < WIDTH; x++) {
				painter->setBackground(colors[glowing[y][x]]);
				painter->mark(x, y);
			}
		}
		delete[] colors;
	}
}