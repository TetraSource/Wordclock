#pragma once

#include "ModeMaskBase.hpp"

namespace Wordclock
{
	class ModeCoffeeImpl {
	protected:
		uint8_t state;
	public:
		ModeCoffeeImpl();
		void update();
		void mask();
	};

	/// Displays a coffee mug with moving steam using the background specified
	/// by the given mode.
	/// @tparam Mode - the mode that is masked in the shape of a mug with steam.
	///                The mode should fill the entire screen with colors.
	/// @tparam speed - the time between each movement of the steam in
	///                 milliseconds.
	template <class Mode, uint32_t speed>
	class ModeCoffee : public ModeMaskBase<Mode, speed>
	{
	protected:
		typedef ModeMaskBase<Mode, speed> super;

		ModeCoffeeImpl self;
	public:
		ModeCoffee();
		void update();
		void mask();
	};

	template <class Mode, uint32_t speed>
	ModeCoffee<Mode, speed>::ModeCoffee()
		: ModeMaskBase<Mode, speed>()
	{
		self = ModeCoffeeImpl();
	}

	template <class Mode, uint32_t speed>
	void ModeCoffee<Mode, speed>::update()
	{
		self.update();
	}
	
	template <class Mode, uint32_t speed>
	void ModeCoffee<Mode, speed>::mask()
	{
		self.mask();
	}
}