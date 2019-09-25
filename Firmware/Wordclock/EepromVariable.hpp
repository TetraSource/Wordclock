#pragma once

#include "EepromAccess.hpp"

#define EEPROM_SIZE_TYPE uint16_t

namespace Wordclock
{
	/// Used to store data that is supposed to survive a power down of the
	/// Wordclock. Hence the data is stored on the EEPROM of the Arduino
	/// as well as on the RAM for performance reasons and to go easy on
	/// the EEPROM. All changes to the data are written to the EEPROM
	/// immediately.
	/// For accessing the EEPROM you must use this or a similar class.
	template <class T>
	class EepromVariable : protected EepromAccess<T>
	{
	protected:
		typedef EepromAccess<T> super;
		T value;
	public:
		inline void setDefault(const T &val)
		{
			value = val;
			super::setDefault(value);
		}

		/// Returns the value. Since it is returned by reference it can be
		/// altered. You need to call save() afterwards though.
		inline T &get()
		{
			return value;
		}

		/// Saves the value on the EEPROM.
		inline void save()
		{
			super::set(value);
		}

		/// Overwrites the value with the given one and saves it on the EEPROM.
		/// @param val - the new value.
		inline void set(const T &val)
		{
			value = val;
			save();
		}
	};
}