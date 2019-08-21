
#include "Painter.hpp"
#define IMPORT_LAYOUT
#include "Config.hpp"

namespace Wordclock
{
	CRGB Painter::buffer[LED_COUNT] = {};

	CRGB Painter::leds[LED_COUNT] = {};
	CRGB Painter::curr = CRGB(0, 0, 0);
	uint8_t *Painter::replace = nullptr;
	bool Painter::replacing = false;

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

	void Painter::paintPixel(const uint8_t &position)
	{
		if (position > LED_COUNT)
			return;
		if (replace != nullptr) {
			if (!replacing) {
				replace[position >> 3] |= 1 << (position & 0x07);
				return;
			}
			else if ((replace[position >> 3] & (1 << (position & 0x07))) == 0)
				return;
		}
		leds[position] = curr;
	}

	void Painter::paintAll()
	{
#ifdef DEBUG_DISPLAY
		Serial.println("ma "); // mark all
#endif
		for (uint8_t i = 0; i < LED_COUNT; i++) {
			paintPixel(i);
		}
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
			paintPixel(arrangement[POINT(x, y)]);
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
		uint8_t position = POINT(x, y);
		if (position >= LED_COUNT) return;

		uint8_t newWidth = x + w <= ROW_LENGTH ? w : ROW_LENGTH - x;

		for (uint8_t i = 0; i < h && position < LED_COUNT; i++)
		{
			for (uint8_t j = 0; j < newWidth; j++) {
				paintPixel(arrangement[position++]);
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

	void Painter::prepareReplacing()
	{
		replace = new uint8_t[(LED_COUNT >> 3) + 1];
		for (uint8_t i = 0; i <= (LED_COUNT >> 3); i++)
			replace[i] = 0;
	}

	static void Painter::setReplacing(const bool &b)
	{
		if (replace == nullptr)
			return;
		replacing = b;
		if (!b) {
			delete[] replace;
			replace = nullptr;
		}
	}
}