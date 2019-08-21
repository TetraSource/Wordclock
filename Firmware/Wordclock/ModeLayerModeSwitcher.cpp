#include "ModeLayerModeSwitcher.hpp"
#include "Utilities.hpp"

namespace Wordclock
{
	ModeLayerModeSwitcherBase::ModeLayerModeSwitcherBase(const bool &active_)
		: active(active_), ModeBase()
	{}

	void ModeLayerModeSwitcherBase::paint()
	{
		if (isInTransition())
			ModeBase::paint();
		else {
			Painter::setColor(Wordclock::getCurrentPreset());
			Utilities::printAnswere(active);
		}
	}
}