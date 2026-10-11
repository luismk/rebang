// Original: d:\build\custom\temp\code\client\projectg\rc4.cpp
#include "RC4.h"

#include <stdlib.h>
#include <string.h>

// rc4.cpp:17
CRC4::CRC4(void) {
	m_key = (unsigned char *)malloc(0x11);
}

// rc4.cpp:22
CRC4::CRC4(unsigned char *key, int keyLength) {
}

// rc4.cpp:26
CRC4::~CRC4(void) {
	free(m_key);
}

// rc4.cpp:31
void CRC4::SetKey(unsigned char *key) {
	memcpy(m_key, key, 4);
}

// rc4.cpp:36
void CRC4::PrepareKey(unsigned char *key, int keyLength) {
	unsigned char swapByte;
	unsigned char index1;
	unsigned char index2;
	unsigned char *state = &m_state[0];
	short counter;
	for (counter = 0; counter < 256; counter++)
		state[counter] = (unsigned char)counter;

	m_x = m_y = 0;
	index1 = 0;
	index2 = 0;

	for (counter = 0; counter < 256; counter++) {
		index2 = (state[counter] + index2 + key[index1]) % 256;
		swapByte = state[counter]; state[counter] = state[index2]; state[index2] = swapByte;

		index1 = (index1 + 1) % keyLength;
	}
}

// rc4.cpp:57
void CRC4::Conversion(unsigned char *buffer, int length) {
	PrepareKey(m_key, 4);

	unsigned char x;
	unsigned char y;
	unsigned char *state;
	unsigned char xorIndex;
	int counter;
	x = m_x;
	y = m_y;

	state = &m_state[0];
	unsigned char swapByte;

	for (counter = 0; counter < length; counter++) {
		x = (x + 1) % 256;
		y = (state[x] + y) % 256;
		swapByte = state[x]; state[x] = state[y]; state[y] = swapByte;

		xorIndex = state[x] + state[y] % 256;
		buffer[counter] ^= state[xorIndex];
	}

	m_x = 0;
	m_y = 0;
}
