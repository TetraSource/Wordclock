#pragma once

#include "ModeBase.hpp"

namespace Wordclock
{
	/// Fuses two modes into one. As the name suggests, both modes run
	/// parallely whenever this mode is selected. Note that the first mode
	/// appears in the foreground. Besides, the modes do not receive signals
	/// from the mode buttons, so do not use modes that depend on it.
	/// @tparam Mode1 - the first mode.
	/// @tparam Mode2 - the second and last mode.
	template <class Mode1, class Mode2>
	class ModeParallel : public ModeBase
	{
	protected:
		typedef ModeBase super;
		Mode1 mode1;
		Mode2 mode2;

	public:
		ModeParallel()
		{
			mode1 = Mode1();
			mode2 = Mode2();
		}

		void select()
		{
			if (isInTransition()) {
				ModeBase::select();
				return;
			}
			mode2.select();
			mode1.select();
		}

		void deselect()
		{
			if (isInTransition()) {
				ModeBase::deselect();
				return;
			}
			mode1.deselect();
			mode2.deselect();
		}

		void actionButton(const bool &inc)
		{
			mode1.actionButton(inc);
			mode2.actionButton(inc);
		}

		void paint()
		{
			if (isInTransition()) {
				ModeBase::paint();
				return;
			}
			mode2.paint();
			mode1.paint();
		}
	};
}