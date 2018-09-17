
#include "EffectDefault.hpp"
#include "Painter.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	void EffectDefault::paint(Painter* painter)
	{
		painter->setForeground(Wordclock::getCurrentPreset());
		painter->markAll();
	}
}