#pragma once

#include "ModeBase.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	/// allows manual choice of current color preset.
	class ModeColorPreset : public ModeBase
	{
	protected:
		typedef ModeBase super;
	public:
		void actionButton(const bool &inc);
		void paint();
	};
}