#pragma once

#include "ModeBase.hpp"
#include "GeneratorBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeFlyingPixelsBase : public ModeBase
	{
	protected:
		struct FlyingPixel
		{
			/// position of the axis alongside which the pixel moves.
			int8_t axisPos;
			/// position of the pixel on the axis.
			int8_t pos;
			/// tail length
			uint8_t len;
			/// direction
			uint8_t dir;
			/// color of the head of the pixel
			CRGB color;
		};

		static FlyingPixel* pixels;
		static uint8_t count;

		GeneratorBase* gen;

		void init(const uint8_t &newCount);

	public:
		ModeFlyingPixelsBase(GeneratorBase* generator);

		void deselect();
	};

	/// Lets pixels fly from one side of the display to another.
	/// They can also have tail of any length and strength.
	/// @tparam directions - the directions whence the pixels fly as bit mask.
	///                      You can use the following formula to active a
	///                      direction of your choice:
	///                   directions |= 1 << Wordclock::Directions::direction;
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
	template <uint8_t directions, uint32_t interval = 500,
		uint8_t minLength = 1, uint8_t maxLength = 1,
		uint8_t minPixelCount = 0, uint8_t maxPixelCount = MAX_LENGTH>
	class ModeFlyingPixels : public ModeFlyingPixelsBase
	{
	public:
		/// initializes the effect.
		/// @param generator - the color generator for the color of the
		///                    head pixel.
		ModeFlyingPixels(GeneratorBase* generator);

		void select();

		void timer();

		void paint();
	};

	template <uint8_t directions, uint32_t interval, uint8_t minLength,
		uint8_t maxLength, uint8_t minPixelCount, uint8_t maxPixelCount>
	ModeFlyingPixels<directions, interval, minLength, maxLength,
		minPixelCount, maxPixelCount>::ModeFlyingPixels(
			GeneratorBase* generator)
		: ModeFlyingPixelsBase(generator)
	{}

	template <uint8_t directions, uint32_t interval, uint8_t minLength,
		uint8_t maxLength, uint8_t minPixelCount, uint8_t maxPixelCount>
	void ModeFlyingPixels<directions, interval, minLength, maxLength,
		minPixelCount, maxPixelCount>::select()
	{
		init(maxPixelCount);
	}

	template <uint8_t directions, uint32_t interval, uint8_t minLength,
		uint8_t maxLength, uint8_t minPixelCount, uint8_t maxPixelCount>
	void ModeFlyingPixels<directions, interval, minLength, maxLength,
		minPixelCount, maxPixelCount>::timer()
	{
		for (uint8_t i = 0; i < count; i++) {
			if ((pixels[i].dir == Top || pixels[i].dir == Left)
				&& ++pixels[i].pos - pixels[i].len
				>= (pixels[i].dir == Left ? Painter::width : Painter::height)
				|| (pixels[i].dir == Bottom || pixels[i].dir == Right)
				&& --pixels[i].pos + pixels[i].len < 0)
			{
				for (uint8_t j = i + 1; j < count; j++) {
					pixels[j - 1] = pixels[j];
				}
				count--;
			}
		}

		if (count < maxPixelCount &&
			((minPixelCount == maxPixelCount && count < maxPixelCount)
				|| (minPixelCount < maxPixelCount &&
				random(minPixelCount, maxPixelCount + 1) >= count))) {

			ModeFlyingPixels::FlyingPixel* pixel = &pixels[count++];

			// set color
			pixel->color = gen->nextRGBColor();
			if (Painter::areIdentical(pixel->color,
				Wordclock::getColorPreset(Wordclock::getColorPresetIndex())))
			{
				pixel->color = gen->nextRGBColor();
			}

			// set direction
			uint8_t dirCount = 0;
			for (uint8_t i = 0; i <= Right; i++) {
				if (directions & (1 << i))
					dirCount++;
			}
			if (dirCount > 0) {
				uint8_t dir = random(dirCount);
				for (uint8_t i = 0; i <= dir; i++) {
					if (!(directions & (1 << i)))
						dir++;
				}

				// set anything else
				pixel->dir = dir;
				pixel->len = random(minLength, maxLength + 1);
				pixel->axisPos = random(
					dir <= Bottom ? Painter::width : Painter::height);

				if (dir == Bottom)
					pixel->pos = Painter::height - 1;
				else if (dir == Right)
					pixel->pos = Painter::width - 1;
				else
					pixel->pos = 0;
			}
		}

		Wordclock::repaint();
		startTimer(interval);
	}

#define DRAW_FLYING_PIXEL(x, y) \
for (uint8_t j = 0; j < length; j++) { \
	uint8_t factor = length - j; \
	Painter::setColor(CRGB( \
		(color.r * factor) / length, \
		(color.g * factor) / length, \
		(color.b * factor) / length)); \
	Painter::paint(x, y); \
}

	template <uint8_t directions, uint32_t interval, uint8_t minLength,
		uint8_t maxLength, uint8_t minPixelCount, uint8_t maxPixelCount>
	void ModeFlyingPixels<directions, interval, minLength, maxLength,
		minPixelCount, maxPixelCount>::paint()
	{
		for (uint8_t i = 0; i < count; i++) {
			CRGB color = pixels[i].color;
			uint8_t length = pixels[i].len;

			if (pixels[i].dir == Top) {
				DRAW_FLYING_PIXEL(pixels[i].axisPos, pixels[i].pos - j)
			}
			else if (pixels[i].dir == Bottom) {
				DRAW_FLYING_PIXEL(pixels[i].axisPos, pixels[i].pos + j)
			}
			else if (pixels[i].dir == Left) {
				DRAW_FLYING_PIXEL(pixels[i].pos - j, pixels[i].axisPos)
			}
			else { //if (pixels[i].dir == Right) {
				DRAW_FLYING_PIXEL(pixels[i].pos + j, pixels[i].axisPos)
			}
		}

		// paint time
		Painter::setColor(Wordclock::getCurrentPreset());
		Utilities::printTime();
	}

#undef DRAW_FLYING_PIXEL
}