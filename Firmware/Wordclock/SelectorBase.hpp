#pragma once

#include <Arduino.h>

// The type of the range the results of the selector lie in.
#define STYPE uint8_t
#define SIGNED_STYPE int8_t

namespace Wordclock
{
	/// The base class for all selectors.
	/// A selector is a class that can select numbers out of a certain
	/// range. Thus they can be used to select item out of this range.
	class SelectorBase
	{};
}