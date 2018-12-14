#pragma once

#include "DisplayTool.hpp"

namespace Wordclock
{
	/// provides the means for marking LEDs in the matrix for the @c Painter class.
	class Marker : public DisplayTool
	{
		friend class Painter;
		friend class Wordclock;
		friend class Core;

	protected:
		bool mask[LED_COUNT];

		void clear();
		void note(const uint8_t &index);
		bool query(const uint8_t &index);

	public:
		Marker();
	};
}