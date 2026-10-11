// Original: d:\build\custom\temp\code\client\projectg\blowfish.cpp
#include "blowfish.h"

// blowfish.h2: the random digit tables (data of this object, read from the exe)
extern const unsigned long data_aa2540[18];      // bf_P
extern const unsigned long data_aa2588[4][256]; // bf_S

#define S(x, i) (SBoxes[i][x.w.byte##i])
#define bf_F(x) (((S(x, 0) + S(x, 1)) ^ S(x, 2)) + S(x, 3))
#define ROUND(a, b, n) (a.dword ^= bf_F(b) ^ PArray[n])

// blowfish.cpp:21
CBlowFish::CBlowFish()
{
	PArray = new unsigned long[18];
	SBoxes = new unsigned long[4][256];
}

// blowfish.cpp:27
CBlowFish::~CBlowFish()
{
	delete PArray;
	delete[] SBoxes;
}

// blowfish.cpp:34
// the low level (private) encryption function
void CBlowFish::Blowfish_encipher(unsigned long *xl, unsigned long *xr)
{
	union aword Xl, Xr;

	Xl.dword = *xl;
	Xr.dword = *xr;

	Xl.dword ^= PArray[0];
	ROUND(Xr, Xl, 1);  ROUND(Xl, Xr, 2);
	ROUND(Xr, Xl, 3);  ROUND(Xl, Xr, 4);
	ROUND(Xr, Xl, 5);  ROUND(Xl, Xr, 6);
	ROUND(Xr, Xl, 7);  ROUND(Xl, Xr, 8);
	ROUND(Xr, Xl, 9);  ROUND(Xl, Xr, 10);
	ROUND(Xr, Xl, 11); ROUND(Xl, Xr, 12);
	ROUND(Xr, Xl, 13); ROUND(Xl, Xr, 14);
	ROUND(Xr, Xl, 15); ROUND(Xl, Xr, 16);
	Xr.dword ^= PArray[17];

	*xr = Xl.dword;
	*xl = Xr.dword;
}

// blowfish.cpp:57
// the low level (private) decryption function
void CBlowFish::Blowfish_decipher(unsigned long *xl, unsigned long *xr)
{
	union aword Xl;
	union aword Xr;

	Xl.dword = *xl;
	Xr.dword = *xr;

	Xl.dword ^= PArray[17];
	ROUND(Xr, Xl, 16); ROUND(Xl, Xr, 15);
	ROUND(Xr, Xl, 14); ROUND(Xl, Xr, 13);
	ROUND(Xr, Xl, 12); ROUND(Xl, Xr, 11);
	ROUND(Xr, Xl, 10); ROUND(Xl, Xr, 9);
	ROUND(Xr, Xl, 8);  ROUND(Xl, Xr, 7);
	ROUND(Xr, Xl, 6);  ROUND(Xl, Xr, 5);
	ROUND(Xr, Xl, 4);  ROUND(Xl, Xr, 3);
	ROUND(Xr, Xl, 2);  ROUND(Xl, Xr, 1);
	Xr.dword ^= PArray[0];

	*xl = Xr.dword;
	*xr = Xl.dword;
}

// blowfish.cpp:82
// constructs the encryption sieve
void CBlowFish::Initialize(unsigned char key[], int keybytes)
{
	int i, j;
	unsigned long datal, datar;

	// first fill arrays from data tables
	for (i = 0; i < 18; i++)
		PArray[i] = data_aa2540[i];

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 256; j++)
			SBoxes[i][j] = data_aa2588[i][j];
	}

	j = 0;
	for (i = 0; i < NPASS + 2; ++i) {
		PArray[i] ^= (((((unsigned long)key[j] << 8) | key[(j + 1) % keybytes]) << 8 | key[(j + 2) % keybytes]) << 8) | key[(j + 3) % keybytes];
		j = (j + 4) % keybytes;
	}

	datal = 0;
	datar = 0;

	for (i = 0; i < NPASS + 2; i += 2) {
		Blowfish_encipher(&datal, &datar);
		PArray[i] = datal;
		PArray[i + 1] = datar;
	}

	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 256; j += 2) {
			Blowfish_encipher(&datal, &datar);
			SBoxes[i][j] = datal;
			SBoxes[i][j + 1] = datar;
		}
	}
}

// blowfish.cpp:134
// get output length, which must be even MOD 8
unsigned long CBlowFish::GetOutputLength(unsigned long lInputLong)
{
	unsigned long lVal;

	lVal = lInputLong % 8; // find out if uneven number of bytes at the end
	if (lVal != 0)
		return lInputLong + 8 - lVal;
	else
		return lInputLong;
}

// blowfish.cpp:148
// Encode pInput into pOutput. Input length in lSize. Returned value is the
// length of output which will be even MOD 8 bytes.
unsigned long CBlowFish::Encode(unsigned char *pInput, unsigned char *pOutput, unsigned long lSize)
{
	unsigned long lCount, lOutSize, lGoodBytes;
	unsigned char *pi, *po;
	int i, j;
	int SameDest = (pInput == pOutput ? 1 : 0);

	lOutSize = GetOutputLength(lSize);
	for (lCount = 0; lCount < lOutSize; lCount += 8) {
		if (SameDest) { // encoded data is being written into the input buffer
			if (lCount < lSize - 7) { // not dealing with uneven bytes at end
				Blowfish_encipher((unsigned long *)pInput,
					(unsigned long *)(pInput + 4));
			} else { // pad end of data with null bytes to complete encryption
				po = pInput + lSize;
				j = (int)(lOutSize - lSize);
				for (i = 0; i < j; i++)
					*po++ = 0;
				Blowfish_encipher((unsigned long *)pInput,
					(unsigned long *)(pInput + 4));
			}
			pInput += 8;
		} else { // copy input to the output buffer prior to encrypting
			if (lCount < lSize - 7) { // not dealing with uneven bytes at end
				pi = pInput;
				po = pOutput;
				for (i = 0; i < 8; i++)
					*po++ = *pi++;
				Blowfish_encipher((unsigned long *)pOutput,
					(unsigned long *)(pOutput + 4));
			} else { // pad end of data with null bytes to complete encryption
				lGoodBytes = lSize - lCount;
				po = pOutput;
				for (i = 0; i < (int)lGoodBytes; i++)
					*po++ = *pInput++;
				for (j = i; j < 8; j++)
					*po++ = 0;
				Blowfish_encipher((unsigned long *)pOutput,
					(unsigned long *)(pOutput + 4));
			}
			pInput += 8;
			pOutput += 8;
		}
	}
	return lOutSize;
}

// blowfish.cpp:208
// Decode pInput into pOutput. Input length in lSize.
void CBlowFish::Decode(unsigned char *pInput, unsigned char *pOutput, unsigned long lSize)
{
	unsigned long lCount;
	unsigned char *pi, *po;
	int i;
	int SameDest = (pInput == pOutput ? 1 : 0);

	for (lCount = 0; lCount < lSize; lCount += 8) {
		if (SameDest) { // encoded data is being written into the input buffer
			Blowfish_decipher((unsigned long *)pInput,
				(unsigned long *)(pInput + 4));
			pInput += 8;
		} else { // copy input to output before decoding
			pi = pInput;
			po = pOutput;
			for (i = 0; i < 8; i++)
				*po++ = *pi++;
			Blowfish_decipher((unsigned long *)pOutput,
				(unsigned long *)(pOutput + 4));
			pInput += 8;
			pOutput += 8;
		}
	}
}
