
#include "Marker.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	uint8_t Utilities::changeValue(const uint8_t &value, const uint8_t &limit, const bool &inc = true)
	{
		if (inc)
			return value + 1 == limit ? 0 : value + 1;
		else
			return value == 0 ? limit - 1 : value - 1;
	}

	uint8_t Utilities::changeLevel(const uint8_t &value, const uint8_t &levelCount, const bool &inc = true)
	{
		int8_t level = (value * levelCount) >> 8;
		level = inc ? level + 1 : level - 1;
		if (level >= levelCount)
			level = 0;
		else if (level < 0)
			level = levelCount - 1;
		return (255 + (level << 8) - level) / levelCount;
	}

	void Utilities::shapeTime(Marker* marker, uint8_t hours, uint8_t minutes)
	{
#include "Languages.hpp"
	}

	void Utilities::shapeTime(Marker* marker)
	{
		marker->mark(0, 0, 2); // ES
		marker->mark(3, 0, 3); // IST

		shapeTime(marker, Wordclock::getTime(Hours), Wordclock::getTime(Minutes));
	}

	void Utilities::shapeNumber(Marker* marker, const uint8_t &number, const uint8_t &x, const uint8_t &y)
	{
		if (number == 0) {
			marker->mark(x + 1, y + 0, 3);
			marker->mark(x + 0, y + 1, 1, 5);
			marker->mark(x + 4, y + 1, 1, 5);
			marker->mark(x + 1, y + 4);
			marker->mark(x + 2, y + 3);
			marker->mark(x + 3, y + 2);
			marker->mark(x + 1, y + 6, 3);
		}
		else if (number == 1) {
			marker->mark(x + 1, y + 1);
			marker->mark(x + 2, y + 0, 1, 6);
			marker->mark(x + 1, y + 6, 3);
		}
		else if (number == 2) {
			marker->mark(x + 0, y + 1);
			marker->mark(x + 1, y + 0, 3);
			marker->mark(x + 4, y + 1, 1, 2);
			marker->mark(x + 3, y + 3);
			marker->mark(x + 2, y + 4);
			marker->mark(x + 1, y + 5);
			marker->mark(x + 0, y + 6, 5);
		}
		else if (number == 3) {
			marker->mark(x + 0, y + 0, 5);
			marker->mark(x + 3, y + 1);
			marker->mark(x + 2, y + 2);
			marker->mark(x + 3, y + 3);
			marker->mark(x + 4, y + 4, 1, 2);
			marker->mark(x + 0, y + 5);
			marker->mark(x + 1, y + 6, 3);
		}
		else if (number == 4) {
			marker->mark(x + 3, y + 0, 1, 7);
			marker->mark(x + 0, y + 4, 3);
			marker->mark(x + 4, y + 4);
			marker->mark(x + 0, y + 3);
			marker->mark(x + 1, y + 2);
			marker->mark(x + 2, y + 1);
		}
		else if (number == 5) {
			marker->mark(x + 0, y + 0, 5);
			marker->mark(x + 0, y + 1, 1, 2);
			marker->mark(x + 0, y + 3, 4);
			marker->mark(x + 4, y + 4, 1, 2);
			marker->mark(x + 0, y + 5);
			marker->mark(x + 1, y + 6, 3);
		}
		else if (number == 6) {
			marker->mark(x + 2, y + 0, 2);
			marker->mark(x + 1, y + 1);
			marker->mark(x + 0, y + 2, 1, 4);
			marker->mark(x + 1, y + 3, 3);
			marker->mark(x + 4, y + 4, 1, 2);
			marker->mark(x + 1, y + 6, 3);
		}
		else if (number == 7) {
			marker->mark(x + 0, y + 0, 5);
			marker->mark(x + 4, y + 1);
			marker->mark(x + 3, y + 2);
			marker->mark(x + 2, y + 3);
			marker->mark(x + 1, y + 4, 1, 3);
		}
		else if (number == 8) {
			marker->mark(x + 1, y + 0, 3);
			marker->mark(x + 0, y + 1, 1, 2);
			marker->mark(x + 4, y + 1, 1, 2);
			marker->mark(x + 1, y + 3, 3);
			marker->mark(x + 0, y + 4, 1, 2);
			marker->mark(x + 4, y + 4, 1, 2);
			marker->mark(x + 1, y + 6, 3);
		}
		else if (number == 9) {
			marker->mark(x + 1, y + 0, 3);
			marker->mark(x + 0, y + 1, 1, 2);
			marker->mark(x + 4, y + 1, 1, 4);
			marker->mark(x + 1, y + 3, 3);
			marker->mark(x + 3, y + 5);
			marker->mark(x + 1, y + 6, 2);
		}
		else { // Invalid number. Show an "E"
			marker->mark(x + 0, y + 0, 1, 7);
			marker->mark(x + 1, y + 0, 4);
			marker->mark(x + 1, y + 3, 4);
			marker->mark(x + 1, y + 6, 4);
		}
	}

	void Utilities::shapeNumber(Marker* marker, const uint8_t &number)
	{
		Utilities::shapeNumber(marker, (number / 10) % 10, 0, 2);
		Utilities::shapeNumber(marker, number % 10, 6, 2);
	}

	void Utilities::shapeAnswere(Marker* marker, const bool &answer)
	{
		if (answer) {
			marker->mark(9, 2);
			marker->mark(8, 3);
			marker->mark(7, 4);
			marker->mark(6, 5);
			marker->mark(5, 6);
			marker->mark(4, 7);
			marker->mark(3, 8);
			marker->mark(2, 7);
			marker->mark(1, 6);
		}
		else {
			marker->mark(2, 2);
			marker->mark(3, 3);
			marker->mark(4, 4);
			marker->mark(5, 5);
			marker->mark(6, 6);
			marker->mark(7, 7);
			marker->mark(8, 8);
			marker->mark(8, 2);
			marker->mark(7, 3);
			marker->mark(6, 4);
			marker->mark(4, 6);
			marker->mark(3, 7);
			marker->mark(2, 8);
		}
	}
}