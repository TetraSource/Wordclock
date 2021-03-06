#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeWavesBase : public ModeBase
	{
	protected:
		typedef ModeBase super;
		static CRGB *colors;
		static uint8_t count;

		void nextColor();
		void printCenterWaves();
		void printBorderWaves();
		ModeWavesBase();
	};

	/// moves color a infinite succession of colored stripes across the
	/// display.
	/// @tparam Generator - the color generator that selects a new color
	///                     when necessary.
	/// @tparam direction - the direction whence the waves come to simulate
	///                     the tunnel effect.
	/// @tparam interval - the time between each movement of the waves in
	///                    milliseconds.
	template <class Generator, Directions direction, uint32_t interval>
	class ModeWaves : public ModeWavesBase
	{
	protected:
		typedef ModeWavesBase super;
		Generator gen;
	public:
		ModeWaves()
			: ModeWavesBase()
		{
			gen = Generator();
		}

		void select()
		{
			if (isInTransition()) {
				ModeBase::select();
				return;
			}
			if (direction == Center || direction == Border)
				count = (Painter::maxLength + 1) >> 1;
			else
				count = Painter::maxLength;
			colors = new CRGB[count];

			uint8_t i = count-1;
			do {
				colors[i] = gen.next();
			} while(i-- != 0);

			Wordclock::startTimer(this, interval, 0);
		}

		void deselect()
		{
			if (isInTransition()) {
				ModeBase::deselect();
				return;
			}
			count = 0;
			delete[] colors;
			Wordclock::cancelTimer(this, 0);
		}

		uint32_t timer(const uint8_t &channel)
		{
			if (channel != 0)
				return ModeBase::timer(channel);
			nextColor();
			colors[0] = gen.next();
			return interval;
		}

		void paint()
			{
			if (isInTransition()) {
				ModeBase::paint();
				return;
			}
			switch(direction)
			{
			case Top:
				for (uint8_t y = 0; y < Painter::height; y++) {
					Painter::setColor(colors[y % count]);
					Painter::paint(0, y, Painter::width, 1);
				}
				break;
			case Bottom:
				for (uint8_t y = 0; y < Painter::height; y++) {
					Painter::setColor(colors[y % count]);
					Painter::paint(0, Painter::height - y - 1,
						Painter::width, 1);
				}
				break;
			case Left:
				for (uint8_t x = 0; x < Painter::width; x++) {
					Painter::setColor(colors[x % count]);
					Painter::paint(x, 0, 1, Painter::height);
				}
				break;
			case Right:
				for (uint8_t x = 0; x < Painter::width; x++) {
					Painter::setColor(colors[x % count]);
					Painter::paint(Painter::width - x - 1, 0,
						1, Painter::height);
				}
				break;
			case Center:
				printCenterWaves();
				break;
			case Border:
				printBorderWaves();
			}
		}
	};
}