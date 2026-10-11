// Original: d:\build\custom\temp\code\client\projectg\encrypttypes.h
#ifndef PG_CLIENT_PROJECTG_ENCRYPTTYPES_H
#define PG_CLIENT_PROJECTG_ENCRYPTTYPES_H

#include "..\..\defines.h"

#include <stdlib.h>

// A value kept XOR-encrypted in memory against memory editors, with a
// checksum of the encrypted bytes. Layout from the constructor (0x431d00) and
// MakeCheckSum (0x4320c0). Get and Set branch on sizeof(T): 1-byte values
// are keyed by m_unk0E, 4-byte ones by m_key.
template <typename T>
class WCrypticValue {
public:
	// encrypttypes.h:9
	WCrypticValue(void) {
		m_key = ((((rand() % 255) << 8 | rand() % 254) << 8 | (rand() % 254 | 0x1234F0)) << 8) | rand() % 255;
		m_unk0C = (unsigned short)((unsigned char)(rand() % 254 | 0xF0) << 8 | rand() % 255);
		m_unk0E = (unsigned char)(rand() % 255);
	}

	// encrypttypes.h:19
	bool IsSafe(void) {
		const WCrypticValue *self = this;
		for (unsigned int i = 0; i < sizeof(T); i++) {
			if (m_checkSum[i] != (reinterpret_cast<unsigned char*>(&m_value)[i] ^ reinterpret_cast<const unsigned char*>(&self)[i]))
				return false;
		}
		return true;
	}

	// encrypttypes.h:30
	WCrypticValue &operator=(T value) {
		Set(value);
		return *this;
	}

	// encrypttypes.h:45
	bool operator>(T value) {
		return Get() > value;
	}

	// encrypttypes.h:49
	bool operator<(T value) {
		return Get() < value;
	}

	// encrypttypes.h:61
	void operator+=(T value) {
		value = Get() + value;
		Set(value);
	}

	// encrypttypes.h:66
	void operator-=(T value) {
		value = Get() - value;
		Set(value);
	}

	// encrypttypes.h:85
	T operator-(T value) {
		return Get() - value;
	}

	// encrypttypes.h:102
	operator T(void) { return Get(); }

private:
	// encrypttypes.h:109
	void MakeCheckSum(void) {
		const WCrypticValue *self = this;
		for (unsigned int i = 0; i < sizeof(T); i++)
			m_checkSum[i] = reinterpret_cast<unsigned char*>(&m_value)[i] ^ reinterpret_cast<const unsigned char*>(&self)[i];
	}

	// encrypttypes.h:118
	T Get(void) const {
		if (sizeof(T) == 1) {
			unsigned char value = ~(*reinterpret_cast<const unsigned char*>(&m_value) ^ m_unk0E);
			return *reinterpret_cast<T*>(&value);
		}
		unsigned long value = ~(*reinterpret_cast<const unsigned long*>(&m_value) ^ m_key);
		return *reinterpret_cast<T*>(&value);
	}

	// encrypttypes.h:149
	void Set(T value) {
		if (sizeof(T) == 1)
			*reinterpret_cast<unsigned char*>(&m_value) = ~*reinterpret_cast<unsigned char*>(&value) ^ m_unk0E;
		else
			*reinterpret_cast<unsigned long*>(&m_value) = ~*reinterpret_cast<unsigned long*>(&value) ^ m_key;
		const WCrypticValue *self = this;
		for (unsigned int i = 0; i < sizeof(T); i++)
			m_checkSum[i] = reinterpret_cast<unsigned char*>(&m_value)[i] ^ reinterpret_cast<const unsigned char*>(&self)[i];
	}

	T m_value;                                  // 0x0, the value XOR m_key, inverted
	unsigned char m_checkSum[8 - sizeof(T)];    // sizeof(T), m_value's bytes XOR this's
	unsigned long m_key;                        // 0x8, random
	unsigned short m_unk0C;                     // 0xc, random
	unsigned char m_unk0E;                      // 0xe, random; the key of 1-byte values
};

CHECK_SIZE(WCrypticValue<int>, 0x10);
CHECK_SIZE(WCrypticValue<float>, 0x10);
CHECK_SIZE(WCrypticValue<bool>, 0x10);

#endif // PG_CLIENT_PROJECTG_ENCRYPTTYPES_H
