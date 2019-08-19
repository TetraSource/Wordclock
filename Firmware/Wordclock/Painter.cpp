
#include "Painter.hpp"
#define IMPORT_LAYOUT
#include "Config.hpp"

namespace Wordclock
{
	CRGB Painter::buffer[LED_COUNT] = {};

	CRGB Painter::leds[LED_COUNT] = {};
	CRGB Painter::curr = CRGB(0, 0, 0);

	const uint8_t Painter::width = WIDTH;
	const uint8_t Painter::height = HEIGHT;
	const uint8_t Painter::maxLength = MAX_LENGTH;

	CRGB Painter::getPixel(const uint8_t &x, const uint8_t &y)
	{
		if (x >= ROW_LENGTH) return;
		uint8_t position = x + y * ROW_LENGTH;
		if (position >= LED_COUNT) return;
		return leds[arrangement[position]];
	}

	void Painter::paintAll()
	{
#ifdef DEBUG_DISPLAY
		Serial.println("ma "); // mark all
#endif
		for (uint8_t position = 0; position < LED_COUNT; position++) {
			leds[position] = curr;
		}
	}

	void Painter::paint(const uint8_t &position)
	{
		if (position < LED_COUNT)
			leds[arrangement[position]] = curr;
	}

	void Painter::paint(const uint8_t &x, const uint8_t &y)
	{
#ifdef DEBUG_DISPLAY
		Serial.print("ms "); // mark spot
		Serial.print(x);
		Serial.print(" ");
		Serial.println(y);
#endif
		if (x < ROW_LENGTH)
			paint(POINT(x, y));
	}

	void Painter::paint(
		const uint8_t &x, const uint8_t &y,
		const uint8_t &w, const uint8_t &h = 1)
	{
#ifdef DEBUG_DISPLAY
		Serial.print("mr "); // mark rectangle
		Serial.print(x);
		Serial.print(" ");
		Serial.print(y);
		Serial.print(" ");
		Serial.print(w);
		Serial.print(" ");
		Serial.println(h);
#endif

		if (x >= ROW_LENGTH) return;
		uint8_t position = x + y * ROW_LENGTH;
		if (position >= LED_COUNT) return;

		uint8_t newWidth = x + w <= ROW_LENGTH ? w : ROW_LENGTH - x;

		for (uint8_t i = 0; i < h && position < LED_COUNT; i++)
		{
			for (uint8_t j = 0; j < newWidth; j++) {
				leds[arrangement[position++]] = curr;
			}
			position += ROW_LENGTH - newWidth;
		}
	}

	bool Painter::areIdentical(CRGB c0, CRGB c1)
	{
		return c0.r - 5 <= c1.r && c0.r + 5 >= c1.r &&
			c0.g - 5 <= c1.g && c0.g + 5 >= c1.g &&
			c0.b - 5 <= c1.b && c0.b + 5 >= c1.b;
	}
}