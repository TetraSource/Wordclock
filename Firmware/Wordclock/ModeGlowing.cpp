#include "ModeGlowing.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	CRGB **ModeGlowingBase::glowing = nullptr;

	ModeGlowingBase::ModeGlowingBase()
		: ModeBase()
	{}

	void ModeGlowingBase::setGlow(const uint8_t &x, const uint8_t &y,
		const CRGB &color)
	{
		if (glowing[y][x].r < color.r)
			glowing[y][x].r = color.r;
		if (glowing[y][x].g < color.g)
			glowing[y][x].g = color.g;
		if (glowing[y][x].b < color.b)
			glowing[y][x].b = color.b;
	}

	void ModeGlowingBase::setGlowingPixel(const uint8_t &x, const uint8_t &y,
		CRGB color, const uint8_t &strength, const uint8_t &maxStrength)
	{
		if (color.r)
			color.r = ((color.r * strength) / maxStrength) >> DIMING_FACTOR;
		if (color.g)
			color.g = ((color.g * strength) / maxStrength) >> DIMING_FACTOR;
		if (color.b)
			color.b = ((color.b * strength) / maxStrength) >> DIMING_FACTOR;
		setGlow(x, y, color);
	}

	void ModeGlowingBase::glowPaint(const uint8_t &radius)
	{
		// Init the matrix glowing.
		glowing = new CRGB*[Painter::height];
		for (uint8_t y = 0; y < Painter::height; y++) {
			glowing[y] = new CRGB[Painter::width];
			for (uint8_t x = 0; x < Painter::width; x++) {
				glowing[y][x] = CRGB(0, 0, 0);
			}
		}
		
		// Fill the matrix with the new content to be displayed.
		for (uint8_t y = 0; y < Painter::height; y++) {
			for (uint8_t x = 0; x < Painter::width; x++) {
				CRGB pixel = Painter::getPixel(x, y);
				if (pixel.r == 0 && pixel.g == 0 && pixel.b == 0)
					continue;
				setGlow(x, y, pixel);
				// Add a glowing cycle around the cycle.
				for (uint8_t i = 1; i < radius; i++) {
					uint8_t glow = radius - i;
					for (uint8_t j = 0; j < i; j++) {
						int8_t px, py;
						px = x + j - i;
						py = y - j;
						if (px >= 0 && py >= 0)
							setGlowingPixel(px, py, pixel, glow, radius);

						px = x + j;
						py = y + j - i;
						if (px < Painter::width && py >= 0)
							setGlowingPixel(px, py, pixel, glow, radius);

						px = x + i - j;
						py = y + j;
						if (px < Painter::width && py < Painter::height)
							setGlowingPixel(px, py, pixel, glow, radius);

						px = x - j;
						py = y + i - j;
						if (px >= 0 && py < Painter::height)
							setGlowingPixel(px, py, pixel, glow, radius);
					}
				}
			}
		}

		// Print the colors stored in the matrix glowing to the screen.
		for (int8_t y = 0; y < Painter::height; y++) {
			CRGB *row = glowing[y];
			for (int8_t x = 0; x < Painter::width; x++) {
				Painter::setColor(row[x]);
				if (Painter::hasColor())
					Painter::paint(x, y);
			}
			delete[] row;
		}
		delete[] glowing;
		glowing = nullptr;
	}
}