#pragma once

#include "ModeBase.hpp"

namespace Wordclock
{
	/// The default mode that doesn't allow to do anything.
	class ModeWordclock : public ModeBase
	{
	protected:
		typedef ModeBase super;
	public:
		void paint();
	};
}