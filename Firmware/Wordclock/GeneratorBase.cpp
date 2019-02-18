
#include "GeneratorBase.hpp"

namespace Wordclock
{
	ColorTypes GeneratorBase::getColorType()
	{
		return RGB;
	}

	CRGB GeneratorBase::nextRGBColor()
	{
		if (this->getColorType() == RGB) {
			return this->nextColor();
		}
		if (this->getColorType() == HSV) {
			CRGB color = this->nextColor();
			hsv2rgb_rainbow(CHSV(color.r, color.g, color.b), color);
			return color;
		}
	}

	CHSV GeneratorBase::nextHSVColor()
	{
		if (this->getColorType() == RGB) {
			return rgb2hsv_approximate(this->nextColor());
		}
		else if (this->getColorType() == HSV) {
			CRGB color = this->nextColor();
			return CHSV(color.r, color.g, color.b);
		}
	}
}