#ifndef PG_CLIENT_PROJECTG_MD5_H
#define PG_CLIENT_PROJECTG_MD5_H

#include "..\..\defines.h"

// Colin Plumb's public-domain MD5, compiled as C++ into ProjectG (free
// functions are __fastcall under /Gr).
typedef unsigned int uint32;

struct MD5Context {
	uint32 buf[4];         // 0x0
	uint32 bits[2];        // 0x10
	unsigned char in[64];  // 0x18
};
CHECK_SIZE(MD5Context, 0x58);
CHECK_FIELD(MD5Context, buf, 0x0);
CHECK_FIELD(MD5Context, bits, 0x10);
CHECK_FIELD(MD5Context, in, 0x18);

void GetMD5String(const char *src, char *dst);
void MD5Init(struct MD5Context *ctx);
void MD5Update(struct MD5Context *ctx, unsigned char const *buf, unsigned len);
void MD5Final(unsigned char digest[16], struct MD5Context *ctx);
void MD5Transform(uint32 buf[4], uint32 const in[16]);

#endif // PG_CLIENT_PROJECTG_MD5_H
