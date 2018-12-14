#pragma once

#include <Arduino.h>

namespace Wordclock
{
	class Marker;

	/// contains some utility functions for the Wordclock.
	class Utilities
	{
	public:
		/// increments or decrements a value in range from 0 to 255 by one.
		/// A wrap is performed when the limit is hit.
		/// @param value - the old value
		/// @param limit - the limit for the value. Note that the wrap is performed before the number is hit.
		/// @param inc - whether to increment or decrement the level
		/// @returns the new value
		static uint8_t changeValue(const uint8_t &value, const uint8_t &limit, const bool &inc = true);

		/// increments or decrements a value in range from 0 to 255 in levels.
		/// A wrap is performed when the limit is hit.
		/// @param value - the old value
		/// @param levelCount - the count of levels to graduate the value
		/// @param inc - whether to increment or decrement the level
		/// @returns the new value
		static uint8_t changeLevel(const uint8_t &value, const uint8_t &levelCount, const bool &inc = true);

		/// marks the given time on the display (without "ES IST").
		/// @param marker - the marker to perform the operation on.
		/// @param hours - the hours of the time to be marked.
		/// @param minutes - the minutes of the time to be marked.
		static void shapeTime(Marker* marker, uint8_t hours, uint8_t minutes);

		/// marks the current time on the display (with "ES IST").
		/// @param marker - the marker to perform the operation on.
		static void shapeTime(Marker* marker);

		/// marks a on digit number with the size of 7*5 at some position on the display.
		/// @param marker - the marker to perform the operation on.
		/// @param number - the number to be marked.
		/// @param x - the x coordinate of the number.
		/// @param y - the y coordinate of the number.
		static void shapeNumber(Marker* marker, const uint8_t &number, const uint8_t &x, const uint8_t &y);

		/// marks a two digit number on the display.
		/// @param marker - the marker to perform the operation on.
		/// @param number - the number to be marked.
		static void shapeNumber(Marker* marker, const uint8_t &number);

		/// either marks an arrow or an cross depending on the input.
		/// @param marker - the marker to perform the operation on.
		/// @param answer - whether to use an arrow or not.
		static void shapeAnswere(Marker* marker, const bool &answer);
	};
}