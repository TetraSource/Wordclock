#include <FastLED.h>
#include "ModeTimeSlice.hpp"
#include "Utilities.hpp"

namespace Wordclock
{
	void ModeTimeSliceUtils::paintSlice(
		const uint8_t &timeType, const uint8_t &scope)
	{
		const Fraction amount = Utilities::getTimeProgress(timeType, scope);

		for (uint8_t r = 0; Painter::minLength > (r << 1); r++)
		{
			const uint8_t size[2] =
				{Painter::width - (r << 1), Painter::height - (r << 1)};
			uint8_t coord[2] = {(size[0] >> 1) + r, r};
			
			uint8_t ringSize =
				(Painter::width + Painter::height - 2 - (r << 2)) << 1;
			uint8_t pos = (ringSize * amount.numerator +
				(amount.denominator >> 1)) / amount.denominator;
			
			uint8_t j = coord[0] - r + 1;
			for (uint8_t i = 0; i <= 4 && pos > 0; i++) {
				uint8_t n = size[i & 1];
				for (; pos > 0 && j < n; j++) {
					Painter::paint(coord[0], coord[1]);
					pos--;
					coord[i & 1] = i & 2 ? coord[i & 1]-1 : coord[i & 1]+1;
				}
				j = 1;
			}
			if (pos > 0)
				Painter::paint(coord[0], coord[1]);
		}
	}
}