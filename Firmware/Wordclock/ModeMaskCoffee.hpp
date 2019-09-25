#pragma once

#include "ModeCoffee.hpp"
#include "ModeMaskBase.hpp"

namespace Wordclock
{
	/// Displays a coffee mug with moving steam using the background specified
	/// by the given mode.
	/// @tparam Mode - the mode that is masked in the shape of a mug with steam.
	///                The mode should fill the entire screen with colors.
	/// @tparam speed - the time between each movement of the steam in
	///                 milliseconds.
	template <class Mode, uint32_t speed>
	class ModeMaskCoffee :
		public ModeMaskBase<ModeMaskCoffee<Mode, speed>, Mode, speed>
	{
	protected:
		typedef ModeMaskBase<ModeMaskCoffee<Mode, speed>, Mode, speed> super;

		ModeCoffeeImpl impl;
	public:
		ModeMaskCoffee();
		void update();
		void mask();
	};

	template <class Mode, uint32_t speed>
	ModeMaskCoffee<Mode, speed>::ModeMaskCoffee()
		: ModeMaskBase<ModeMaskCoffee<Mode, speed>, Mode, speed>()
	{
		impl = ModeCoffeeImpl();
	}

	template <class Mode, uint32_t speed>
	void ModeMaskCoffee<Mode, speed>::update()
	{
		impl.update();
	}
	
	template <class Mode, uint32_t speed>
	void ModeMaskCoffee<Mode, speed>::mask()
	{
		impl.paint();
	}
}