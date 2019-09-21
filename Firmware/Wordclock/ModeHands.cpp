#include "ModeHands.hpp"
#include "Painter.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	void ModeHandsUtilities::paintHands(
		const uint8_t &timeType, const uint8_t &scope,
		const uint8_t &ring, const uint8_t &rings)
	{
		const Fraction amount = Utilities::getTimeProgress(timeType, scope);

		for (uint8_t r = ring; r < ring + rings &&
			Painter::minLength > (r << 1); r++)
		{
			const uint8_t size[2] =
				{Painter::width - (r << 1), Painter::height - (r << 1)};
			uint8_t coord[2] = {size[0] >> 1, 0};

			for (uint8_t i = 0; true; i++) {
				uint8_t ringSize = (i < 2 ? size[i] :
					(Painter::width + Painter::height - 2 - (r << 2))) << 1;
				uint8_t pos = (ringSize * amount.numerator +
					(amount.denominator >> 1)) / amount.denominator;
				if (pos >= ringSize)
					pos -= ringSize;
				
				if (i == 2) {
					// Calculate position on ring
					for (i = 0; i <= 3 && pos > 0; i++) {
						uint8_t space = size[i & 1]-1 - (i & 2 ? 0 : coord[i]);
						if (space > pos)
							space = pos;
						pos -= space;
						if (i & 2)
							coord[i & 1] -= space;
						else
							coord[i] += space;
					}
					coord[0] += pos;
					break;
				}
				else if (size[1 - i] == 1) {
					// We are not dealing with a ring but with a stripe here
					// since it has just a width/height of 1.
					coord[i] = pos < size[i] ? pos : ringSize - pos - 1;
					break;
				}
			}
			Painter::paint(coord[0] + r, coord[1] + r);
		}
	}
}