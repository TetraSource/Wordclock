#include "ModeFiller.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	void ModeFillerUtilities::modeFillerPaint(const Directions &direction,
		const TimeTypes &timeType, const uint8_t &scope)
	{
		const Fraction amount = Utilities::getTimeProgress(timeType, scope);

		uint8_t filledW = Painter::width > 1 ?
			((Painter::width + 1) * amount.numerator) / amount.denominator :
			(Painter::width == 1 ? amount.numerator >= (amount.denominator >> 1
			) : 0);
		uint8_t filledH = Painter::height > 1 ?
			((Painter::height + 1) * amount.numerator) / amount.denominator :
			(Painter::height == 1 ? amount.numerator >= (amount.denominator >> 1
			) : 0);

		if (direction == Top || direction == Bottom)
		{
			Painter::paint(
				0, direction == Bottom ? Painter::height - filledH : 0,
				Painter::width, filledH);
		}
		else if (direction == Left || direction == Right)
		{
			Painter::paint(
				direction == Right ? Painter::width - filledW : 0, 0,
				filledW, Painter::height);
		}
		else if (direction == Center || direction == Border) {
			filledW = Painter::width & 1 ? filledW | 0x01 : filledW & 0xfe;
			filledH = Painter::height & 1 ? filledH | 0x01 : filledH & 0xfe;
			if (direction == Border) {
				Painter::paint(0, 0,
					Painter::width, (Painter::height - filledH) >> 1);
				Painter::paint(0, (Painter::height + filledH) >> 1,
					Painter::width, (Painter::height - filledH) >> 1);
				Painter::paint(0, (Painter::height - filledH) >> 1,
					(Painter::width - filledW) >> 1, filledH);
				Painter::paint((Painter::width + filledW) >> 1,
					(Painter::height - filledH) >> 1,
					(Painter::width - filledW) >> 1, filledH);
			}
			else {
				Painter::paint(
					(Painter::width - filledW) >> 1,
					(Painter::height - filledH) >> 1,
					filledW, filledH);
			}
		}
	}
}