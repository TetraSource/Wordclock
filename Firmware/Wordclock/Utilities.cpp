#include "Painter.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	uint8_t Utilities::changeValue(
		const uint8_t &value,
		const uint8_t &limit,
		const bool &inc = true)
	{
		if (inc)
			return value + 1 >= limit ? 0 : value + 1;
		else
			return value <= 0 ? limit - 1 : value - 1;
	}

	uint8_t Utilities::changeLevel(
		const uint8_t &value,
		const uint8_t &levelCount,
		const bool &inc = true)
	{
		int8_t level = Utilities::getLevel(value, levelCount);
		level = inc ? level + 1 : level - 1;
		if (level >= levelCount)
			level = 0;
		else if (level < 0)
			level = levelCount - 1;
		return (255 + (level << 8) - level) / levelCount;
	}

	Fraction Utilities::getTimeProgress(
		const TimeTypes &timeType,
		const uint8_t &scope)
	{
		Fraction frac;
		if (timeType >= Years) {
			frac.numerator = 0;
			frac.denominator = 1;
			return frac;
		}
		
		uint32_t secs = scope * Wordclock::getUnitSeconds(timeType);
		if (timeType < Years) {
			uint32_t maxSecs = Wordclock::getUnitSeconds(
				timeType + (timeType == Weekdays ? 2 : 1));
			if (secs > maxSecs)
				secs = maxSecs;
		}
		frac.denominator = secs - 1;
		
		secs = 0;
		for (uint8_t i = 0; i <= timeType; i++) {
			if (i != Days)
				secs += Wordclock::getTime(i) * Wordclock::getUnitSeconds(i);
		}
		frac.numerator = secs % (frac.denominator+1);

		while (frac.denominator >= 0xffffff) {
			frac.denominator >>= 1;
			frac.numerator >>= 1;
		}
		return frac;
	}

	void Utilities::printTime(uint8_t hours, uint8_t minutes)
	{
#if LANGUAGE == 0 // English

#elif LANGUAGE == 1 // German
		if (minutes >= 25) { // choose "VOR" as off minute twenty-five
			hours++;
		}
		hours %= 12;
		minutes -= minutes % 5;

		if (minutes == 5 || minutes == 25 || minutes == 35 || minutes == 55) {
			Painter::paint(7, 0, 4); // FUENF
		}
		else if (minutes == 10 || minutes == 50) {
			Painter::paint(0, 1, 4); // ZEHN
		}
		else if (minutes == 15 || minutes == 45) {
			Painter::paint(4, 2, 7); // VIERTEL
		}
		else if (minutes == 20 || minutes == 40) {
			Painter::paint(4, 1, 7); // ZWANZIG
		}

		if (minutes == 0) {
			Painter::paint(8, 9, 3); // UHR
		}
		else if (minutes == 25 || minutes == 30 || minutes == 35) {
			Painter::paint(0, 4, 4); // HALB
		}

		if (minutes == 5 || minutes == 10 || minutes == 15 || minutes == 20 || minutes == 35) {
			Painter::paint(2, 3, 4); // NACH
		}
		else if (minutes == 25 || minutes == 40 || minutes == 45 || minutes == 50 || minutes == 55) {
			Painter::paint(6, 3, 3); // VOR
		}

		if (hours == 0) {
			Painter::paint(5, 4, 5); // ZWOELF
		}
		else if (hours == 1) {
			if (minutes == 0)
				Painter::paint(2, 5, 3); // EIN
			else
				Painter::paint(2, 5, 4); // EINS
		}
		else if (hours == 2) {
			Painter::paint(0, 5, 4); // ZWEI
		}
		else if (hours == 3) {
			Painter::paint(1, 6, 4); // DREI
		}
		else if (hours == 4) {
			Painter::paint(7, 7, 4); // VIER
		}
		else if (hours == 5) {
			Painter::paint(7, 6, 4); // FUENF
		}
		else if (hours == 6) {
			Painter::paint(1, 9, 5); // SECHS
		}
		else if (hours == 7) {
			Painter::paint(5, 5, 6); // SIEBEN
		}
		else if (hours == 8) {
			Painter::paint(1, 8, 4); // ACHT
		}
		else if (hours == 9) {
			Painter::paint(3, 7, 4); // NEUN
		}
		else if (hours == 10) {
			Painter::paint(5, 8, 4); // ZEHN
		}
		else if (hours == 11) {
			Painter::paint(0, 7, 3); // ELF
		}
#endif
	}

	void Utilities::printTime()
	{
		Painter::paint(0, 0, 2); // ES
		Painter::paint(3, 0, 3); // IST

		printTime(Wordclock::getTime(Hours), Wordclock::getTime(Minutes));
	}

	void Utilities::printNumber(
		const uint8_t &number,
		const uint8_t &x,
		const uint8_t &y)
	{
		if (number == 0) {
			Painter::paint(x + 1, y + 0, 3);
			Painter::paint(x + 0, y + 1, 1, 5);
			Painter::paint(x + 4, y + 1, 1, 5);
			Painter::paint(x + 1, y + 4);
			Painter::paint(x + 2, y + 3);
			Painter::paint(x + 3, y + 2);
			Painter::paint(x + 1, y + 6, 3);
		}
		else if (number == 1) {
			Painter::paint(x + 1, y + 1);
			Painter::paint(x + 2, y + 0, 1, 6);
			Painter::paint(x + 1, y + 6, 3);
		}
		else if (number == 2) {
			Painter::paint(x + 0, y + 1);
			Painter::paint(x + 1, y + 0, 3);
			Painter::paint(x + 4, y + 1, 1, 2);
			Painter::paint(x + 3, y + 3);
			Painter::paint(x + 2, y + 4);
			Painter::paint(x + 1, y + 5);
			Painter::paint(x + 0, y + 6, 5);
		}
		else if (number == 3) {
			Painter::paint(x + 0, y + 0, 5);
			Painter::paint(x + 3, y + 1);
			Painter::paint(x + 2, y + 2);
			Painter::paint(x + 3, y + 3);
			Painter::paint(x + 4, y + 4, 1, 2);
			Painter::paint(x + 0, y + 5);
			Painter::paint(x + 1, y + 6, 3);
		}
		else if (number == 4) {
			Painter::paint(x + 3, y + 0, 1, 7);
			Painter::paint(x + 0, y + 4, 3);
			Painter::paint(x + 4, y + 4);
			Painter::paint(x + 0, y + 3);
			Painter::paint(x + 1, y + 2);
			Painter::paint(x + 2, y + 1);
		}
		else if (number == 5) {
			Painter::paint(x + 0, y + 0, 5);
			Painter::paint(x + 0, y + 1, 1, 2);
			Painter::paint(x + 0, y + 3, 4);
			Painter::paint(x + 4, y + 4, 1, 2);
			Painter::paint(x + 0, y + 5);
			Painter::paint(x + 1, y + 6, 3);
		}
		else if (number == 6) {
			Painter::paint(x + 2, y + 0, 2);
			Painter::paint(x + 1, y + 1);
			Painter::paint(x + 0, y + 2, 1, 4);
			Painter::paint(x + 1, y + 3, 3);
			Painter::paint(x + 4, y + 4, 1, 2);
			Painter::paint(x + 1, y + 6, 3);
		}
		else if (number == 7) {
			Painter::paint(x + 0, y + 0, 5);
			Painter::paint(x + 4, y + 1);
			Painter::paint(x + 3, y + 2);
			Painter::paint(x + 2, y + 3);
			Painter::paint(x + 1, y + 4, 1, 3);
		}
		else if (number == 8) {
			Painter::paint(x + 1, y + 0, 3);
			Painter::paint(x + 0, y + 1, 1, 2);
			Painter::paint(x + 4, y + 1, 1, 2);
			Painter::paint(x + 1, y + 3, 3);
			Painter::paint(x + 0, y + 4, 1, 2);
			Painter::paint(x + 4, y + 4, 1, 2);
			Painter::paint(x + 1, y + 6, 3);
		}
		else if (number == 9) {
			Painter::paint(x + 1, y + 0, 3);
			Painter::paint(x + 0, y + 1, 1, 2);
			Painter::paint(x + 4, y + 1, 1, 4);
			Painter::paint(x + 1, y + 3, 3);
			Painter::paint(x + 3, y + 5);
			Painter::paint(x + 1, y + 6, 2);
		}
		else if (number == 10) {
			Painter::paint(x + 2, y + 0);
			Painter::paint(x + 1, y + 1);
			Painter::paint(x + 0, y + 2, 1, 5);
			Painter::paint(x + 4, y + 2, 1, 5);
			Painter::paint(x + 1, y + 3, 3);
		}
		else if (number == 11) {
			Painter::paint(x + 0, y + 1, 3);
			Painter::paint(x + 0, y + 1, 1, 5);
			Painter::paint(x + 3, y + 1, 1, 2);
			Painter::paint(x + 1, y + 3, 3);
			Painter::paint(x + 4, y + 4, 1, 2);
			Painter::paint(x + 0, y + 6, 4);
		}
		else if (number == 12) {
			Painter::paint(x + 1, y + 0, 3);
			Painter::paint(x + 0, y + 1, 1, 5);
			Painter::paint(x + 1, y + 6, 3);
			Painter::paint(x + 4, y + 1);
			Painter::paint(x + 4, y + 5);
		}
		else if (number == 13) {
			Painter::paint(x + 0, y + 0, 4);
			Painter::paint(x + 0, y + 1, 1, 5);
			Painter::paint(x + 0, y + 6, 4);
			Painter::paint(x + 4, y + 1, 1, 5);
		}
		else if (number == 14) {
			Painter::paint(x + 0, y + 1, 1, 5);
			Painter::paint(x + 0, y + 0, 5);
			Painter::paint(x + 1, y + 3, 4);
			Painter::paint(x + 0, y + 6, 5);
		}
		else if (number == 15) {
			Painter::paint(x + 0, y + 1, 1, 6);
			Painter::paint(x + 0, y + 0, 5);
			Painter::paint(x + 1, y + 3, 4);
		}
		else { // Invalid number. Print an "-"
			Painter::paint(x + 1, y + 3, 3);
		}
	}

	void Utilities::printNumber(const uint8_t &number, const uint8_t &y)
	{
		Utilities::printNumber((number / 10) % 10, 0, y);
		Utilities::printNumber(number % 10, 6, y);
	}

	void Utilities::printHex(const uint8_t &number, const uint8_t &y)
	{
		Utilities::printNumber((number >> 4) & 0xf, 0, y);
		Utilities::printNumber(number & 0xf, 6, y);
	}

	void Utilities::printAnswere(const bool &answer)
	{
		if (answer) {
			Painter::paint(9, 2);
			Painter::paint(8, 3);
			Painter::paint(7, 4);
			Painter::paint(6, 5);
			Painter::paint(5, 6);
			Painter::paint(4, 7);
			Painter::paint(3, 8);
			Painter::paint(2, 7);
			Painter::paint(1, 6);
		}
		else {
			Painter::paint(2, 2);
			Painter::paint(3, 3);
			Painter::paint(4, 4);
			Painter::paint(5, 5);
			Painter::paint(6, 6);
			Painter::paint(7, 7);
			Painter::paint(8, 8);
			Painter::paint(8, 2);
			Painter::paint(7, 3);
			Painter::paint(6, 4);
			Painter::paint(4, 6);
			Painter::paint(3, 7);
			Painter::paint(2, 8);
		}
	}
}