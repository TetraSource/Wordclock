#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// allows manual changing of the order of the current color preset.
	class ModeRearrangeColorPreset : public ModeBase
	{
	protected:
		typedef ModeBase super;
	public:
		void actionButton(const bool &inc);
		void paint();
	};
}