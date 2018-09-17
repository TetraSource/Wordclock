
#define IMPORT_LETTERS
#include "Utilities.hpp"
#include "DisplayTool.hpp"

#ifdef VERTICAL_STRIPES
#define MARK_X y
#define MARK_Y x
#define MARK_WIDTH h
#define MARK_HEIGHT w
#define ADAPT_TO_GRID(index) index / ROW_LENGTH + (index % ROW_LENGTH) * ROW_LENGTH
#else
#define MARK_X x
#define MARK_Y y
#define MARK_WIDTH w
#define MARK_HEIGHT h
#define ADAPT_TO_GRID(index) index
#endif

namespace Wordclock
{
	const uint8_t DisplayTool::width = WIDTH;
	const uint8_t DisplayTool::height = HEIGHT;
	const uint8_t DisplayTool::maxLength = MAX_LENGTH;

	void DisplayTool::markAll()
	{
#ifdef DEBUG_DISPLAY
		Serial.println("ma "); // mark all
#endif
		for (uint8_t index = 0; index < LED_COUNT; index++) {
			this->note(index);
		}
	}

	void DisplayTool::mark(const uint8_t &x, const uint8_t &y)
	{
#ifdef DEBUG_DISPLAY
		Serial.print("ms "); // mark spot
		Serial.print(x);
		Serial.print(" ");
		Serial.println(y);
#endif
		if (MARK_X >= ROW_LENGTH) return;
		uint8_t index = MARK_X + MARK_Y * ROW_LENGTH;
		if (index >= LED_COUNT) return;
		this->note(index);
	}

	void DisplayTool::mark(const uint8_t &x, const uint8_t &y, const uint8_t &w, const uint8_t &h = 1)
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

		if (MARK_X >= ROW_LENGTH) return;
		uint8_t index = MARK_X + MARK_Y * ROW_LENGTH;
		if (index >= LED_COUNT) return;

		uint8_t newWidth = MARK_X + MARK_WIDTH <= ROW_LENGTH ? MARK_WIDTH : ROW_LENGTH - MARK_X;

		for (uint8_t i = 0; i < MARK_HEIGHT && index < LED_COUNT; i++)
		{
			for (uint8_t j = 0; j < newWidth; j++) {
				this->note(index++);
			}
			index += ROW_LENGTH - newWidth;
		}
	}

	bool DisplayTool::mark(const char &letter)
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
				if (this->query(index + 1)) {
					index += 3;
				}
				else if (this->query(index)) {
					index += 2;
				}
				else if (letters[index] == letter) {
					this->note(ADAPT_TO_GRID(index));
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
			this->note(ADAPT_TO_GRID(index));
		return false;
	}
}

#undef X
#undef Y
#undef WIDTH
#undef HEIGHT
#undef ADAPT_TO_GRID