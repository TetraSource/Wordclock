#pragma once

#include "GeneratorBase.hpp"

namespace Wordclock
{
	/// Uses the two given color generator to generate its colors.
	/// @tparam Generator1 - the first color generator.
	/// @tparam Generator2 - the second color generator.
	/// @tparam Selector - determines which generator is choosen.
	///                    In case of even numbers the first generator is
	///                    choosen. Otherwise, the second one is choosen.
	template <class Generator1, class Generator2, class Selector>
	class GeneratorUnion : public GeneratorBase
	{
	protected:
		typedef GeneratorBase super;
		Generator1 gen1;
		Generator2 gen2;
		Selector selector;
	public:
		GeneratorUnion();
		CRGB next();
	};

	template <class Generator1, class Generator2, class Selector>
	GeneratorUnion<Generator1, Generator2, Selector>::GeneratorUnion()
	{
		gen1 = Generator1();
		gen2 = Generator2();
		selector = Selector();
	}

	template <class Generator1, class Generator2, class Selector>
	CRGB GeneratorUnion<Generator1, Generator2, Selector>::next()
	{
		return (selector.next() & 1) ? gen2.next() : gen1.next();
	}
}