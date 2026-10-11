// Original: d:\build\custom\temp\code\shared\tea.cpp

// XTEA block decipher (16 rounds) and an N-byte decoder built on it.

// tea.cpp:48
void Decipher(const unsigned int* const v, unsigned int* const w, const unsigned int* const k)
{
	unsigned int sum = 0xE3779B90;
	unsigned int delta = 0x9E3779B9;

	w[0] = v[0];
	w[1] = v[1];

	for (int i = 0; i < 16; i++) {
		w[1] -= ((w[0] << 4 ^ w[0] >> 5) + w[0]) ^ (sum + k[sum >> 11 & 3]);
		sum -= delta;
		w[0] -= ((w[1] << 4 ^ w[1] >> 5) + w[1]) ^ (sum + k[sum & 3]);
	}
}

// tea.cpp:85
int DecodeTEA_NBYTE(char* szDest, int nSize, const char* szSrc, const unsigned int* paKey)
{
	unsigned int k[4] = { paKey[0], paKey[1], paKey[2], paKey[3] };
	unsigned int v[2] = { 0, 0 };
	for (int i = 0; i < nSize; i += 8) {
		v[0] = *(const unsigned int*)(szSrc + i);

		v[1] = *(const unsigned int*)(szSrc + i + 4);

		Decipher(v, (unsigned int*)(szDest + i), k);
	}

	return 8;
}
