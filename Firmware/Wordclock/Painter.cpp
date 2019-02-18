
#define IMPORT_LETTERS
#include "Config.hpp"
#include "Painter.hpp"

// constants for arrange method
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

// constants for painting methods
#ifdef VERTICAL_STRIPES
#define PAINT_X y
#define PAINT_Y x
#define PAINT_WIDTH h
#define PAINT_HEIGHT w
#define ADAPT_TO_GRID(index) index / ROW_LENGTH + (index % ROW_LENGTH) * ROW_LENGTH
#else
#define PAINT_X x
#define PAINT_Y y
#define PAINT_WIDTH w
#define PAINT_HEIGHT h
#define ADAPT_TO_GRID(index) index
#endif

namespace Wordclock
{
	CRGB Painter::buffer[LED_COUNT] = {};
	
	CRGB Painter::leds[LED_COUNT] = {};
	CRGB Painter::curr = CRGB(0, 0, 0);

	const uint8_t Painter::width = WIDTH;
	const uint8_t Painter::height = HEIGHT;
	const uint8_t Painter::maxLength = MAX_LENGTH;

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

	CRGB Painter::getColor()
	{
		return curr;
	}

	bool Painter::hasColor()
	{
		return curr.r != 0 && curr.g != 0 && curr.b != 0;
	}

	void Painter::setColor(const CRGB &color)
	{
		curr = color;
	}

	CRGB Painter::getPixel(const uint8_t &x, const uint8_t &y)
	{
		if (PAINT_X >= ROW_LENGTH) return;
		uint8_t index = PAINT_X + PAINT_Y * ROW_LENGTH;
		if (index >= LED_COUNT) return;
		return leds[index];
	}

	void Painter::paintAll()
	{
#ifdef DEBUG_DISPLAY
		Serial.println("ma "); // mark all
#endif
		for (uint8_t index = 0; index < LED_COUNT; index++) {
			leds[index] = curr;
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
		if (PAINT_X >= ROW_LENGTH) return;
		uint8_t index = PAINT_X + PAINT_Y * ROW_LENGTH;
		if (index >= LED_COUNT) return;
		leds[index] = curr;
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

		if (PAINT_X >= ROW_LENGTH) return;
		uint8_t index = PAINT_X + PAINT_Y * ROW_LENGTH;
		if (index >= LED_COUNT) return;

		uint8_t newWidth = PAINT_X + PAINT_WIDTH <= ROW_LENGTH ? PAINT_WIDTH : ROW_LENGTH - PAINT_X;

		for (uint8_t i = 0; i < PAINT_HEIGHT && index < LED_COUNT; i++)
		{
			for (uint8_t j = 0; j < newWidth; j++) {
				leds[index++] = curr;
			}
			index += ROW_LENGTH - newWidth;
		}
	}

	bool Painter::paint(const char &letter)
	{
#ifdef DEBUG_DISPLAY
		Serial.print("ml "); // mark letter
		Serial.println(letter);
#endif
		if (letter == ' ')
			return true;

		// Try to find a solitary version of the letter
		uint8_t rowEnd;
		uint8_t index = 0;
		while (index < LED_COUNT) {
			rowEnd = index + width;
			do {
				if (leds[index+1].r != 0 || leds[index+1].g != 0 || leds[index+1].b != 0) {
					index += 3;
				}
				else if (leds[index].r != 0 || leds[index].g != 0 || leds[index].b != 0) {
					index += 2;
				}
				else if (letters[index] == letter) {
					leds[ADAPT_TO_GRID(index)] = curr;
					return true;
				}
				else {
					index += 1;
				}
			} while (index <= rowEnd);
			index = rowEnd;
		}

		// Just try to find the letter
		index = 0;
		while (index < LED_COUNT && letters[index] != letter)
			index++;
		if (index < LED_COUNT)
			leds[ADAPT_TO_GRID(index)] = curr;
		return false;
	}

	bool Painter::areIdentical(CRGB c0, CRGB c1)
	{
		return c0.r - 5 <= c1.r && c0.r + 5 >= c1.r &&
			c0.g - 5 <= c1.g && c0.g + 5 >= c1.g &&
			c0.b - 5 <= c1.b && c0.b + 5 >= c1.b;
	}
}

#undef INCREMENT
#undef SOURCE_INIT
#undef BACKWARDS_INIT

#undef PAINT_X
#undef PAINT_Y
#undef PAINT_WIDTH
#undef PAINT_HEIGHT
#undef ADAPT_TO_GRID