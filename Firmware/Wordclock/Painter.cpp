
#include "Marker.hpp"
#include "Painter.hpp"

#ifdef UPSIDE_DOWN
#define INCREMENT(var) var--
#define SOURCE_INIT LED_COUNT - 1
#else
#define INCREMENT(var) var++
#define SOURCE_INIT 0
#endif
#ifdef MIRROR
#define BACKWARDS_INIT true
#else
#define BACKWARDS_INIT false
#endif

namespace Wordclock
{
	CRGB Painter::buffer[LED_COUNT] = {};

	void Painter::arrange()
	{
		uint8_t rowCount = LED_COUNT / ROW_LENGTH;
		bool backwards = BACKWARDS_INIT;
		uint8_t source = SOURCE_INIT, target = 0;
		uint8_t i, j;

		for (i = 0; i < rowCount; i++) {
			if (backwards) {
				target += ROW_LENGTH - 1;
				for (j = 0; j < ROW_LENGTH; j++) {
					buffer[target--] = leds[INCREMENT(source)];
				}
				target += ROW_LENGTH + 1;
			}
			else {
				for (j = 0; j < ROW_LENGTH; j++) {
					buffer[target++] = leds[INCREMENT(source)];
				}
			}
#ifdef CHANGE_DIRECTION
			backwards = !backwards;
#endif
		}
		for (i = 0; i < LED_COUNT; i++) {
			leds[i] = buffer[i];
		}
	}

	void Painter::note(const uint8_t &index)
	{
		leds[index] = marker->mask[index] ? foreground : background;
	}

	bool Painter::query(const uint8_t &index)
	{
		return leds[index].r != 0 || leds[index].g != 0 || leds[index].b != 0;
	}

	Painter::Painter(Marker* marker)
	{
		this->marker = marker;
	}

	bool Painter::isForeground(const uint8_t &x, const uint8_t &y)
	{
#ifdef VERTICAL_STRIPES
		if (y >= ROW_LENGTH) return false;
		uint8_t index = y + x * ROW_LENGTH;
		if (index >= LED_COUNT) return false;
#else
		if (x >= ROW_LENGTH) return false;
		uint8_t index = x + y * ROW_LENGTH;
		if (index >= LED_COUNT) return false;
#endif
		return marker->query(index);
	}

	void Painter::setForeground(const CRGB &foreground)
	{
		this->foreground = foreground;
	}

	CRGB Painter::getForeground()
	{
		return foreground;
	}

	void Painter::setBackground(const CRGB &background)
	{
		this->background = background;
	}

	CRGB Painter::getBackground()
	{
		return background;
	}

	bool Painter::areIdentical(CRGB c0, CRGB c1)
	{
		return c0.r - 5 <= c1.r && c0.r + 5 >= c1.r &&
			c0.g - 5 <= c1.g && c0.g + 5 >= c1.g &&
			c0.b - 5 <= c1.b && c0.b + 5 >= c1.b;
	}

	bool Painter::areIdentical()
	{
		return areIdentical(foreground, background);
	}
}