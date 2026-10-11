#ifndef PG_CLIENT_PROJECTG_BLOWFISH_H
#define PG_CLIENT_PROJECTG_BLOWFISH_H

#include "..\..\defines.h"

#define MAXKEYBYTES 56 // 448 bits max
#define NPASS 16       // SBox passes

class CBlowFish {
	LAYOUT_CHECK_FRIEND;

private:
	unsigned long *PArray;        // 0x00, 18 entries
	unsigned long (*SBoxes)[256]; // 0x04, 4 boxes

	void Blowfish_encipher(unsigned long *xl, unsigned long *xr);
	void Blowfish_decipher(unsigned long *xl, unsigned long *xr);

public:
	CBlowFish();
	~CBlowFish();
	void Initialize(unsigned char key[], int keybytes);
	unsigned long GetOutputLength(unsigned long lInputLong);
	unsigned long Encode(unsigned char *pInput, unsigned char *pOutput, unsigned long lSize);
	void Decode(unsigned char *pInput, unsigned char *pOutput, unsigned long lSize);
};
CHECK_SIZE(CBlowFish, 0x8);
BEGIN_LAYOUT_CHECK(CBlowFish)
	CHECK_FIELD(CBlowFish, PArray, 0x0);
	CHECK_FIELD(CBlowFish, SBoxes, 0x4);
END_LAYOUT_CHECK

// choose a byte order for your hardware: little-endian (Intel)
union aword {
	unsigned long dword;
	unsigned char byte[4];
	struct {
		unsigned int byte3 : 8;
		unsigned int byte2 : 8;
		unsigned int byte1 : 8;
		unsigned int byte0 : 8;
	} w;
};
CHECK_SIZE(aword, 0x4);

#endif // PG_CLIENT_PROJECTG_BLOWFISH_H
