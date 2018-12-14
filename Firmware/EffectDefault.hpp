#pragma once

#include "EffectBase.hpp"

namespace Wordclock
{
	class Painter;

	/// Just uses the current color preset without selecting another one to dye all marked LEDs.
	class EffectDefault : public EffectBase
	{
	public:
		void paint(Painter* painter);
	};
}