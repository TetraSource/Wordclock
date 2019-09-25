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
		inline void setDefault(const uint8_t &idx, const T &val)
		{
			array[idx] = val;
			super::setDefault(idx, array[idx]);
		}

		/// Returns the element at the given index in the array. Since it is
		/// returned by reference you can even alter externally. However, after
		/// this you need to call save(idx).
		/// @parm idx - the index of the item in the array.
		inline T &get(const uint8_t &idx)
		{
			return array[idx];
		}

		/// saves the element at the given index.
		/// Call this if you used the reference returned by get(idx) to
		/// modify the item.
		/// @param idx - the index of the item in the array.
		inline void save(const uint8_t &idx)
		{
			super::set(idx, array[idx]);
		}

		/// sets the given value at the given index in the array and saves.
		/// @param idx - the index of the item in the array.
		/// @param val - the value to be set.
		inline void set(const uint8_t &idx, const T &val)
		{
			array[idx] = val;
			save(idx);
		}
	};
}