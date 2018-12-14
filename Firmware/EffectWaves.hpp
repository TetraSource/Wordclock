#pragma once

#include "EffectBase.hpp"
#include "GeneratorBase.hpp"
#include "Painter.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class EffectWavesBase : public EffectBase
	{
	protected:
		static CRGB* colors;
		static uint8_t count;

		GeneratorBase* gen;

		void nextColor();
		void init(const uint8_t &count);

	public:
		EffectWavesBase(GeneratorBase* gen);

		void deselect();
	};

	/// moves color a infinite succession of colored stripes across the display.
	/// @tparam direction - the direction whence the waves come to simulate the tunnel effect.
	/// @tparam interval - the time between each movement of the waves in milliseconds.
	template <Directions direction = Center, uint32_t interval = 500>
	class EffectWaves : public EffectWavesBase
	{
	public:
		/// initializes the effect.
		/// @param generator - the color generator that chooses the colors for the waves.
		EffectWaves(GeneratorBase* generator);

		void select();

		void update();

		void paint(Painter* painter);
	};

	template <Directions direction, uint32_t interval>
	EffectWaves<direction, interval>::EffectWaves(GeneratorBase* generator)
		: EffectWavesBase(generator)
	{}

	template <Directions direction, uint32_t interval>
	void EffectWaves<direction, interval>::select()
	{
		init((direction == Center || direction == Border) ? (Painter::maxLength + 1) >> 1 : Painter::maxLength);
	}

	template <Directions direction, uint32_t interval>
	void EffectWaves<direction, interval>::update()
	{
		nextColor();
		startTimer(interval);
	}

	template <Directions direction, uint32_t interval>
	void EffectWaves<direction, interval>::paint(Painter* painter)
	{
		painter->setForeground(Wordclock::getCurrentPreset());

		if (direction == Top) {
			for (uint8_t y = 0; y < HEIGHT; y++) {
				painter->setBackground(colors[y % count]);
				painter->mark(0, y, WIDTH, 1);
			}
		}
		else if (direction == Bottom) {
			for (uint8_t y = 0; y < HEIGHT; y++) {
				painter->setBackground(colors[y % count]);
				painter->mark(0, HEIGHT - y - 1, WIDTH, 1);
			}
		}
		else if (direction == Left) {
			for (uint8_t x = 0; x < WIDTH; x++) {
				painter->setBackground(colors[x % count]);
				painter->mark(x, 0, 1, HEIGHT);
			}
		}
		else if (direction == Right) {
			for (uint8_t x = 0; x < WIDTH; x++) {
				painter->setBackground(colors[x % count]);
				painter->mark(WIDTH - x - 1, 0, 1, HEIGHT);
			}
		}
		else if (direction == Center) {
			uint8_t x0 = (WIDTH - 1) >> 1;
			uint8_t y0 = (HEIGHT - 1) >> 1;
			uint8_t ringCount = WIDTH >= HEIGHT ? (WIDTH + 1) >> 1 : (HEIGHT + 1) >> 1;

			for (uint8_t i = 0; i < ringCount; i++) {
				painter->setBackground(colors[i % count]);
				uint8_t x1 = WIDTH - x0 - 1;
				uint8_t y1 = HEIGHT - y0 - 1;
				for (uint8_t x = x0; x <= x1; x++) {
					painter->mark(x, y0);
					painter->mark(x, y1);
				}
				for (uint8_t y = y0 + 1; y < y1; y++) {
					painter->mark(x0, y);
					painter->mark(x1, y);
				}
				x0--;
				y0--;
			}
		}
		else if (direction == Border) {
			uint8_t ringCount = WIDTH >= HEIGHT ? (WIDTH + 1) >> 1 : (HEIGHT + 1) >> 1;

			for (uint8_t i = 0; i < ringCount; i++) {
				painter->setBackground(colors[i % count]);
				uint8_t &x0 = i;
				uint8_t &y0 = i;
				uint8_t x1 = WIDTH - x0 - 1;
				uint8_t y1 = HEIGHT - y0 - 1;
				for (uint8_t x = x0; x <= x1; x++) {
					painter->mark(x, y0);
					painter->mark(x, y1);
				}
				for (uint8_t y = y0 + 1; y < y1; y++) {
					painter->mark(x0, y);
					painter->mark(x1, y);
				}
			}
		}
	}
}