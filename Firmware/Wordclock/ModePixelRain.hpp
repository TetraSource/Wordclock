#pragma once

#include "ModeBase.hpp"
#include "GeneratorBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModePixelRainBase : public ModeBase
	{
	protected:
		typedef ModeBase super;

		struct PixelDrop
		{
			/// color of the head of the pixel
			CRGB color;
			/// position of the axis alongside which the pixel moves.
			int8_t axisPos : 5;
			/// position of the pixel on the axis.
			int8_t pos : 5;
			/// tail length
			uint8_t len: 4;
			/// direction of pixel
			Directions dir : 2;
		};

		static PixelDrop* pixels;
		static uint8_t count;

		void internalSelect(const uint8_t &newCount);
		void movePixels();
		void genPixel(const uint8_t &directions,
			const uint8_t &minLength, const uint8_t &maxLength,
			const CRGB &color);
		ModePixelRainBase();

	public:
		void deselect();
		void paint();
	};

	/// Lets pixels fly from one side of the display to another.
	/// They can also have tail of any length and strength.
	/// @tparam directions - the directions whence the pixels fly as bit mask.
	///                      You can use the following formula to active a
	///                      direction of your choice:
	///                   directions |= 1 << Wordclock::Directions::direction;
	/// @tparam Generator - the color generator that selects a new color
	///                     when necessary.
	/// @tparam interval - determines the velocity of the pixels by specifying
	///                    the time between each movements of them in
	///                    milliseconds.
	/// @tparam minLength - the lower limit for the random generated length
	///                     for pixel head and tail.
	/// @tparam maxLength - the upper limit for the random generated length
	///                     for pixel head and tail.
	/// @tparam minPixelCount - the lower limit of pixels that have to be on
	///                         the display at one time.
	/// @tparam maxPixelCount - the upper limit of pixels that have to be on
	///                         the display at one time.
	template <class Generator, uint8_t directions, uint16_t interval,
		uint8_t minLength, uint8_t maxLength,
		uint8_t minPixelCount, uint8_t maxPixelCount>
	class ModePixelRain : public ModePixelRainBase
	{
	protected:
		typedef ModePixelRainBase super;
		Generator gen;
	public:
		/// initializes the effect.
		/// @param generator - the color generator for the color of the
		///                    head pixel.
		ModePixelRain();
		void select();
		uint32_t timer(const uint8_t &channel);
	};

	template <class Generator, uint8_t directions, uint16_t interval,
		uint8_t minLength, uint8_t maxLength,
		uint8_t minPixelCount, uint8_t maxPixelCount>
	ModePixelRain<Generator, directions, interval, minLength,
		maxLength, minPixelCount, maxPixelCount>::ModePixelRain()
		: ModePixelRainBase()
	{
		gen = Generator();
	}

	template <class Generator, uint8_t directions, uint16_t interval,
		uint8_t minLength, uint8_t maxLength,
		uint8_t minPixelCount, uint8_t maxPixelCount>
	void ModePixelRain<Generator, directions, interval, minLength,
		maxLength, minPixelCount, maxPixelCount>::select()
	{
		if (isInTransition())
			ModeBase::select();
		else
			internalSelect(maxPixelCount);
	}

	template <class Generator, uint8_t directions, uint16_t interval,
		uint8_t minLength, uint8_t maxLength,
		uint8_t minPixelCount, uint8_t maxPixelCount>
	uint32_t ModePixelRain<Generator, directions, interval, minLength,
		maxLength, minPixelCount, maxPixelCount>::timer(const uint8_t &channel)
	{
		if (isInTransition())
			return ModeBase::timer(channel);
		movePixels();
		while (count < maxPixelCount && (count < minPixelCount ||
			random(minPixelCount, maxPixelCount + 1) > count))
		{
			genPixel(directions, minLength, maxLength, gen.next());
		}
		Wordclock::repaint();
		return interval;
	}
}