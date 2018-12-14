#pragma once

#include "ModeBase.hpp"

namespace Wordclock
{
	/// Base class for all configuration modes that set or edit colors.
	/// Automatically selects the first effect when being selected.
	/// and select the previous one when being deselected.
	class ModeColorBase : public ModeBase
	{
	protected:
		static uint8_t prevMode;

	public:
		void select();

		void deselect();
	};
}