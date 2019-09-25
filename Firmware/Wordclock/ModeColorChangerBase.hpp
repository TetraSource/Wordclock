#pragma once

#include "ModeBase.hpp"

namespace Wordclock
{
	class ModeColorChangerBase : public ModeBase
	{
	protected:
		typedef ModeBase super;
		CRGB currColor;
		ModeColorChangerBase();

	public:
		void select();
		void deselect();
		void paint();
	};
}