#pragma once

#include <Arduino.h>
#include "Wordclock.hpp"

namespace Wordclock
{
	class Painter;

	struct Fraction
	{
		uint32_t numerator;
		uint32_t denominator;
	};

	/// contains some utility functions for the Wordclock.
	class Utilities
	{
	public:
		/// returns a incrementation or decrementation a value in range from 0
		/// to 255 by one.
		/// A wrap is performed when the limit is hit.
		/// @param value - the old value
		/// @param limit - the limit for the value. Note that the wrap is
		/// performed before the number is hit.
		/// @param inc - whether to increment or decrement the level
		/// @returns the new value
		static uint8_t changeValue(
			const uint8_t &value,
			const uint8_t &limit,
			const bool &inc = true);
		
		/// returns the level of the given value if it is seperated into
		/// levelCount units instead of 255 units.
		/// @param value - the value in in a 255 unit system.
		/// @param levelCount - the count of units in the target system.
		/// @returns - the unit in the target system, aka the level.
		inline static uint8_t getLevel(
			const uint8_t &value,
			const uint8_t &levelCount);

		/// retuns the progess for the specified time scope. If the time type
		/// is minutes, the scope is 30 and the 15th minute just passed, then
		/// the progess will be approximately 50%.
		/// @param timeType - the unit of the scope.
		/// @param scope - the upper limit for the given time type. This is
		///                never more than the maximum for the given time type.
		///                Thus you can pass 255 to use the full scope always.
		/// @returns - the progress f. Note that
		///              progress = f.numerator / f.denominator <= 1
		///            applies always.
		static Fraction getTimeProgress(
			const TimeTypes &timeType,
			const uint8_t &scope = 255);

		/// returns a incrementation or decrementation a value in range from 0
		/// to 255 in levels.
		/// A wrap is performed when the limit is hit.
		/// @param value - the old value
		/// @param levelCount - the count of levels to graduate the value
		/// @param inc - whether to increment or decrement the level
		/// @returns the new value
		static uint8_t changeLevel(
			const uint8_t &value,
			const uint8_t &levelCount,
			const bool &inc = true);

		/// marks the given time on the display (without "ES IST").
		/// @param hours - the hours of the time to be marked.
		/// @param minutes - the minutes of the time to be marked.
		static void printTime(uint8_t hours, uint8_t minutes);

		/// marks the current time on the display (with "ES IST").
		static void printTime();

		/// marks a digit with the size of 7*5 at some position on
		/// the display.
		/// @param number - the number to be marked.
		/// @param x - the x coordinate of the number.
		/// @param y - the y coordinate of the number.
		static void printNumber(
			const uint8_t &number,
			const uint8_t &x,
			const uint8_t &y);

		/// marks a two digit decimal number on the display.
		/// @param number - the number to be marked.
		static void printNumber(const uint8_t &number, const uint8_t &y = 2);

		/// marks a two digit hexadecimal number on the display.
		/// @param number - the number to be marked.
		static void printHex(const uint8_t &number, const uint8_t &y = 2);

		/// either marks an arrow or an cross depending on the input.
		/// @param answer - whether to use an arrow or not.
		static void printAnswere(const bool &answer);
	};

	inline uint8_t Utilities::getLevel(
		const uint8_t &value, const uint8_t &levelCount)
	{
		return (value * levelCount) >> 8;
	}
}