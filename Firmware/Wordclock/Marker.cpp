
#include "Marker.hpp"

namespace Wordclock
{
	void Marker::clear()
	{
		for (uint8_t i = 0; i < LED_COUNT; i++) {
			mask[i] = false;
		}
	}

	void Marker::note(const uint8_t &index)
	{
		mask[index] = true;
	}

	bool Marker::query(const uint8_t &index)
	{
		return mask[index];
	}

	Marker::Marker()
	{
		clear();
	}
}