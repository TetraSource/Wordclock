
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
		if (minutes >= 25) { // choose "VOR" as of minute twenty-five
			hours++;
		}
		hours %= 12;
		minutes -= minutes % 5;

		if (minutes == 5 || minutes == 25 || minutes == 35 || minutes == 55) {
			marker->mark(7, 0, 4); // FUENF
		}
		else if (minutes == 10 || minutes == 50) {
			marker->mark(0, 1, 4); // ZEHN
		}
		else if (minutes == 15 || minutes == 45) {
			marker->mark(4, 2, 7); // VIERTEL
		}
		else if (minutes == 20 || minutes == 40) {
			marker->mark(4, 1, 7); // ZWANZIG
		}
		if (minutes == 0) {
			marker->mark(8, 9, 3); // UHR
		}
		else if (minutes == 25 || minutes == 30 || minutes == 35) {
			marker->mark(0, 4, 4); // HALB
		}
		if (minutes == 5 || minutes == 10 || minutes == 15 || minutes == 20 || minutes == 35) {
			marker->mark(2, 3, 4); // NACH
		}
		else if (minutes == 25 || minutes == 40 || minutes == 45 || minutes == 50 || minutes == 55) {
			marker->mark(6, 3, 3); // VOR
		}

		if (hours == 0) {
			marker->mark(5, 4, 5); // ZWÖLF
		}
		else if (hours == 1) {
			if (minutes == 0)
				marker->mark(2, 5, 3); // EIN
			else
				marker->mark(2, 5, 4); // EINS
		}
		else if (hours == 2) {
			marker->mark(0, 5, 4); // ZWEI
		}
		else if (hours == 3) {
			marker->mark(1, 6, 4); // DREI
		}
		else if (hours == 4) {
			marker->mark(7, 7, 4); // VIER
		}
		else if (hours == 5) {
			marker->mark(7, 6, 4); // FUENF
		}
		else if (hours == 6) {
			marker->mark(1, 9, 5); // SECHS
		}
		else if (hours == 7) {
			marker->mark(5, 5, 6); // SIEBEN
		}
		else if (hours == 8) {
			marker->mark(1, 8, 4); // ACHT
		}
		else if (hours == 9) {
			marker->mark(3, 7, 4); // NEUN
		}
		else if (hours == 10) {
			marker->mark(5, 8, 4); // ZEHN
		}
		else if (hours == 11) {
			marker->mark(0, 7, 3); // ELF
		}
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