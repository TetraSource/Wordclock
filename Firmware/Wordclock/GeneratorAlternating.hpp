#pragma once

#include "GeneratorBase.hpp"

namespace Wordclock
{
	/// Uses the two given color generator alternatingly to generate its colors.
	/// Thus, this is just a more efficient specialization of GeneratorUnion.
	/// @tparam Generator1 - the first color generator.
	/// @tparam Generator2 - the second color generator.
	template <class Generator1, class Generator2>
	class GeneratorAlternating : public GeneratorBase
	{
	protected:
		typedef GeneratorBase super;
		Generator1 gen1;
		Generator2 gen2;
		bool curr;
	public:
		GeneratorAlternating();
		CRGB next();
	};

	template <class Generator1, class Generator2>
	GeneratorAlternating<Generator1, Generator2>::GeneratorAlternating()
	{
		gen1 = Generator1();
		gen2 = Generator2();
		curr = true;
	}

	template <class Generator1, class Generator2>
	CRGB GeneratorAlternating<Generator1, Generator2>::next()
	{
		curr = !curr;
		return curr ? gen2.next() : gen1.next();
	}
}