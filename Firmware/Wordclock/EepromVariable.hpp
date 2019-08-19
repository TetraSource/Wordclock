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
		EepromVariable();
		inline void setDefault(const T &);
		inline T &get();
		inline void set(const T &);
	};

	template <class T>
	EepromVariable<T>::EepromVariable()
		: EepromAccess<T>()
	{}

	template <class T>
	inline void EepromVariable<T>::setDefault(const T &default_)
	{
		value = default_;
		super::setDefault(value);
	}

	template <class T>
	inline T &EepromVariable<T>::get()
	{
		return value;
	}

	template <class T>
	inline void EepromVariable<T>::set(const T &val)
	{
		super::set(val);
		value = val;
	}
}