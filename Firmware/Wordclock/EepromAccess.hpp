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
		void setDefault(T &default_);
		void get(T &var);
		void set(const T &value_);
	};

	template <class T>
	EepromAccess<T>::EepromAccess()
	{
		slot = nextSlot;
		nextSlot += sizeof(T);
	}

	template <class T>
	void EepromAccess<T>::setDefault(T &default_)
	{
#ifdef RESET_EEPROM
		EEPROM.put(slot, default_);
#else
		EEPROM.get(slot, default_);
#endif
	}

	template <class T>
	void EepromAccess<T>::get(T &var)
	{
		EEPROM.get(slot, var);
	}

	template <class T>
	void EepromAccess<T>::set(const T &value_)
	{
#ifndef RESET_EEPROM
		EEPROM.put(slot, value_);
#endif
	}

	template <class T, uint8_t size>
	class EepromArrayAccess : protected EepromUtilites
	{
	protected:
		EEPROM_SIZE_TYPE slot;
	public:
		EepromArrayAccess();
		void setDefault(T *default_);
		void get(const uint8_t &idx, T &val);
		void set(const uint8_t &idx, const T &val);
	};

	template <class T, uint8_t size>
	EepromArrayAccess<T, size>::EepromArrayAccess()
	{
		slot = nextSlot;
		nextSlot += size * sizeof(T);
	}

	template <class T, uint8_t size>
	void EepromArrayAccess<T, size>::setDefault(T *default_)
	{
		EEPROM_SIZE_TYPE currSlot = slot;
		for (uint8_t i = 0; i < size; i++) {
#ifdef RESET_EEPROM
			EEPROM.put(currSlot, default_[i]);
#else
			EEPROM.get(currSlot, default_[i]);
#endif
			currSlot += sizeof(T);
		}
	}

	template <class T, uint8_t size>
	void EepromArrayAccess<T, size>::get(const uint8_t &idx, T &val)
	{
#ifdef RESET_EEPROM
		EEPROM.put(slot + idx * sizeof(T), val);
#else
		EEPROM.get(slot + idx * sizeof(T), val);
#endif
	}

	template <class T, uint8_t size>
	void EepromArrayAccess<T, size>::set(const uint8_t &idx, const T &val)
	{
#ifndef RESET_EEPROM
		EEPROM.put(slot + idx * sizeof(T), val);
#endif
	}
}