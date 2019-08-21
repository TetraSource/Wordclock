#pragma once

#include "EepromAccess.hpp"

namespace Wordclock
{
	/// Used to store an array of data that is supposed to survive a power
	/// down of the Wordclock. Hence the data is stored on the EEPROM of the
	/// Arduino as well as on the RAM for performance reasons and to go easy
	/// on the EEPROM. All changes to the data are written to the EEPROM
	/// immediately.
	/// For accessing the EEPROM you must use this or a similar class.
	template <class T, uint8_t size>
	class EepromArray : protected EepromArrayAccess<T, size>
	{
	protected:
		typedef EepromArrayAccess<T, size> super;
		T array[size];
	public:
		EepromArray();
		inline void setDefault(const uint8_t &idx, const T &val);
		inline T &get(const uint8_t &idx);
		inline void set(const uint8_t &idx, const T val);
	};

	template <class T, uint8_t size>
	EepromArray<T, size>::EepromArray()
		: EepromArrayAccess<T, size>()
	{}

	template <class T, uint8_t size>
	inline void EepromArray<T, size>::setDefault(
		const uint8_t &idx, const T &val)
	{
		array[idx] = val;
		super::setDefault(idx, array[idx]);
	}

	template <class T, uint8_t size>
	inline T &EepromArray<T, size>::get(const uint8_t &idx)
	{
		return array[idx];
	}

	template <class T, uint8_t size>
	inline void EepromArray<T, size>::set(const uint8_t &idx, const T val)
	{
		super::set(idx, val);
		array[idx] = val;
	}
}