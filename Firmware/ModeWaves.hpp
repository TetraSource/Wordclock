#pragma once

#include "GeneratorBase.hpp"
#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeWavesBase : public ModeBase
	{
	protected:
		static CRGB* colors;
		static uint8_t count;

		GeneratorBase* gen;

		void nextColor();
		void init(const uint8_t &count);

	public:
		ModeWavesBase(GeneratorBase* gen);

		void deselect();
	};

	/// moves color a infinite succession of colored stripes across the
	/// display.
	/// @tparam direction - the direction whence the waves come to simulate
	///                     the tunnel effect.
	/// @tparam interval - the time between each movement of the waves in
	///                    milliseconds.
	template <Directions direction = Center, uint32_t interval = 500>
	class ModeWaves : public ModeWavesBase
	{
	public:
		/// initializes the effect.
		/// @param generator - the color generator that chooses the colors
		///                    for the waves.
		ModeWaves(GeneratorBase* generator);

		void select();

		void timer();

		void paint();
	};

	template <Directions direction, uint32_t interval>
	ModeWaves<direction, interval>::ModeWaves(GeneratorBase* generator)
		: ModeWavesBase(generator)
	{}

	template <Directions direction, uint32_t interval>
	void ModeWaves<direction, interval>::select()
	{
		init((direction == Center || direction == Border) ?
			(Painter::maxLength + 1) >> 1 : Painter::maxLength);
	}

	template <Directions direction, uint32_t interval>
	void ModeWaves<direction, interval>::timer()
	{
		nextColor();
		startTimer(interval);
	}

	template <Directions direction, uint32_t interval>
	void ModeWaves<direction, interval>::paint()
	{
		if (direction == Top) {
			for (uint8_t y = 0; y < HEIGHT; y++) {
				Painter::setColor(colors[y % count]);
				Painter::paint(0, y, WIDTH, 1);
			}
		}
		else if (direction == Bottom) {
			for (uint8_t y = 0; y < HEIGHT; y++) {
				Painter::setColor(colors[y % count]);
				Painter::paint(0, HEIGHT - y - 1, WIDTH, 1);
			}
		}
		else if (direction == Left) {
			for (uint8_t x = 0; x < WIDTH; x++) {
				Painter::setColor(colors[x % count]);
				Painter::paint(x, 0, 1, HEIGHT);
			}
		}
		else if (direction == Right) {
			for (uint8_t x = 0; x < WIDTH; x++) {
				Painter::setColor(colors[x % count]);
				Painter::paint(WIDTH - x - 1, 0, 1, HEIGHT);
			}
		}
		else if (direction == Center) {
			uint8_t x0 = (WIDTH - 1) >> 1;
			uint8_t y0 = (HEIGHT - 1) >> 1;
			uint8_t ringCount =
				WIDTH >= HEIGHT ? (WIDTH + 1) >> 1 : (HEIGHT + 1) >> 1;

			for (uint8_t i = 0; i < ringCount; i++) {
				Painter::setColor(colors[i % count]);

				uint8_t x1 = WIDTH - x0 - 1;
				uint8_t y1 = HEIGHT - y0 - 1;

				for (uint8_t x = x0; x <= x1; x++) {
					Painter::paint(x, y0);
					Painter::paint(x, y1);
				}
				for (uint8_t y = y0 + 1; y < y1; y++) {
					Painter::paint(x0, y);
					Painter::paint(x1, y);
				}

				x0--;
				y0--;
			}
		}
		else if (direction == Border) {
			uint8_t ringCount =
				WIDTH >= HEIGHT ? (WIDTH + 1) >> 1 : (HEIGHT + 1) >> 1;

			for (uint8_t i = 0; i < ringCount; i++) {
				Painter::setColor(colors[i % count]);

				uint8_t &x0 = i;
				uint8_t &y0 = i;
				uint8_t x1 = WIDTH - x0 - 1;
				uint8_t y1 = HEIGHT - y0 - 1;

				for (uint8_t x = x0; x <= x1; x++) {
					Painter::paint(x, y0);
					Painter::paint(x, y1);
				}
				for (uint8_t y = y0 + 1; y < y1; y++) {
					Painter::paint(x0, y);
					Painter::paint(x1, y);
				}
			}
		}

		Painter::setColor(Wordclock::getCurrentPreset());
		Utilities::printTime();
	}
}