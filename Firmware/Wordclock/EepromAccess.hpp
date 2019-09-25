#pragma once

#include <Arduino.h>
#include "Config.hpp"
#include <EEPROM.h>

#define EEPROM_SIZE_TYPE uint16_t

namespace Wordclock
{
	class EepromUtilites
	{
	protected:
		static EEPROM_SIZE_TYPE nextSlot;
	};

	template <class T>
	class EepromAccess : protected EepromUtilites
	{
	protected:
		EEPROM_SIZE_TYPE slot;
	public:
		EepromAccess();
		void setDefault(T &val);
		void get(T &val) const;
		void set(const T &val) const;
	};

	template <class T>
	EepromAccess<T>::EepromAccess()
	{
		slot = nextSlot;
		nextSlot += sizeof(T);
	}

	template <class T>
	inline void EepromAccess<T>::setDefault(T &val)
	{
#ifdef RESET_EEPROM
		set(val);
#else
		get(val);
#endif
	}

	template <class T>
	inline void EepromAccess<T>::get(T &val) const
	{
		EEPROM.get(slot, val);
	}

	template <class T>
	inline void EepromAccess<T>::set(const T &val) const
	{
		EEPROM.put(slot, val);
	}

	template <class T, uint8_t size>
	class EepromArrayAccess : protected EepromUtilites
	{
	protected:
		EEPROM_SIZE_TYPE slot;
	public:
		EepromArrayAccess();
		inline void setDefault(const uint8_t &idx, T &val);
		void get(const uint8_t &idx, T &val) const;
		void set(const uint8_t &idx, const T &val) const;
	};

	template <class T, uint8_t size>
	EepromArrayAccess<T, size>::EepromArrayAccess()
	{
		slot = nextSlot;
		nextSlot += size * sizeof(T);
	}

	template <class T, uint8_t size>
	inline void EepromArrayAccess<T, size>::setDefault(
		const uint8_t &idx, T &val)
	{
#ifdef RESET_EEPROM
		set(idx, val);
#else
		get(idx, val);
#endif
	}

	template <class T, uint8_t size>
	void EepromArrayAccess<T, size>::get(const uint8_t &idx, T &val) const
	{
		EEPROM.get(slot + idx * sizeof(T), val);
	}

	template <class T, uint8_t size>
	void EepromArrayAccess<T, size>::set(const uint8_t &idx, const T &val) const
	{ 
		EEPROM.put(slot + idx * sizeof(T), val);
	}
}