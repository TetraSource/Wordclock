#pragma once

#include "ModeBaseInterval.hpp"
#include "Utilities.hpp"
#include "Wordclock.hpp"

namespace Wordclock
{
	class ModeHandsUtilities
	{
	private:
		static void paintHands(const uint8_t &timeType,
			const uint8_t &scope, const uint8_t &ring, const uint8_t &rings);
	
	template <class, TimeTypes, uint8_t, uint8_t, uint8_t>
	friend class ModeHands;
	};

	/// Shows the time using hands of variable length. The hands can also
	/// be extended into a slice of a rotating pie chart like thing.
	/// @tparam Generator 
	/// @tparam timeType - the unit of the time that determines how great
	///                    the percentage of filled area is.
	/// @tparam scope - specifies what time is equal to the hand pointing to
	///                 the top. If your time type is Minutes for instance and
	///                 you set scope to 5, then it points to the top after the
	///                 5th, 10th, 15th, ... minute - so its basically a modulo.
	///                 This value never exceeds the maximal possible value for
	///                 the given time type, so you can set it to 255 to use
	///                 the entire scope of the specified time type.
	///                 This value should always divide the maximal possible
	///                 value for the given time type (5 minutes divide 60
	///                 minutes for instance). If the maximal value is not
	///                 static - as with days (January has more than February),
	///                 then you should set it to 255 always.
	/// @tparam ring - the ring the hand start in. 0 specifies the outer ring
	///                and min{Painter::width, Painter::height}/2 specifies the
	///                ring in the center. This is also the maximal value for
	///                this value.
	/// @tparam rings - specifies how many rings are used by the hand. So this
	///                 basically specifies the length of the hand.
	template <class Generator, TimeTypes timeType, uint8_t scope = 255,
		uint8_t ring = 0, uint8_t rings = 1>
	class ModeHands : public ModeTimeBound
	{
	protected:
		typedef ModeTimeBound super;
		Generator gen;
	public:
		ModeHands();
		void paint();
	};

	template <class Generator, TimeTypes timeType, uint8_t scope,
		uint8_t ring, uint8_t rings>
	ModeHands<Generator, timeType, scope, ring, rings>::ModeHands()
	{
		gen = Generator();
	}

	template <class Generator, TimeTypes timeType, uint8_t scope,
		uint8_t ring, uint8_t rings>
	void ModeHands<Generator, timeType, scope, ring, rings>::paint()
	{
		if (isInTransition())
			ModeBase::paint();
		else {
			Painter::setColor(gen.next());
			ModeHandsUtilities::paintHands(timeType, scope, ring, rings);
		}
	}
}