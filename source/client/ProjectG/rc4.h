// Original: d:\build\custom\temp\code\client\projectg\rc4.h
#ifndef PG_CLIENT_PROJECTG_RC4_H
#define PG_CLIENT_PROJECTG_RC4_H

#include "..\..\defines.h"

// RC4 stream cipher keyed by a 4-byte key (SetKey). The key schedule and
// keystream follow the widespread public-domain rc4.c (prepare_key / rc4);
// layout from the constructor and Conversion (rc4.cpp:17-82).
class CRC4 {
	LAYOUT_CHECK_FRIEND;
public:
	CRC4(void);
	CRC4(unsigned char *key, int keyLength);
	virtual ~CRC4(void);

	void SetKey(unsigned char *key);
	void Conversion(unsigned char *buffer, int length);

private:
	void PrepareKey(unsigned char *key, int keyLength);

	unsigned char m_state[256];              // 0x4
	unsigned char m_x;                       // 0x104
	unsigned char m_y;                       // 0x105
	unsigned char *m_key;                    // 0x108, malloc(0x11), the first 4 bytes are the key
};

CHECK_SIZE(CRC4, 0x10C);
BEGIN_LAYOUT_CHECK(CRC4)
	CHECK_FIELD(CRC4, m_state, 0x4);
	CHECK_FIELD(CRC4, m_x, 0x104);
	CHECK_FIELD(CRC4, m_y, 0x105);
	CHECK_FIELD(CRC4, m_key, 0x108);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_RC4_H
