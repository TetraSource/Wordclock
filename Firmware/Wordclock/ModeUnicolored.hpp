#pragma once

#include "ModeBase.hpp"

namespace Wordclock
{
	/// Paints the whole screen with one color.
	/// Generator - generates the colors used to paint the screen.
	template <class Generator>
	class ModeUnicolored : public ModeBase
	{
	protected:
		typedef ModeBase super;
		Generator gen;

	public:
		ModeUnicolored()
			: ModeBase()
		{
			gen = Generator();
		}

		void paint()
		{
			Painter::setColor(gen.next());
			Painter::paintAll();
		}
	};
}