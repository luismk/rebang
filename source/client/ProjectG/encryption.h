// Original: d:\build\custom\temp\code\client\projectg\encryption.h
#ifndef PG_CLIENT_PROJECTG_ENCRYPTION_H
#define PG_CLIENT_PROJECTG_ENCRYPTION_H

// Layout not yet reconstructed (only the inline members are emitted so far).
class CEncryption {
public:
	// encryption.h:139
	static CEncryption* Instance() {
		static CEncryption _Instance;
		return &_Instance;
	}

private:
	CEncryption();
	~CEncryption();
};

// encryption.h:61
// XORs data with the 0x26-character key, cycling over whichever of the two
// is shorter; returns the number of bytes processed. GameUnit::OnConnect
// uses it on the session key it sends (the copy there is file-local).
static unsigned int XorWithKey(char *data, unsigned int size) {
	const char *key = "{782AE110-2EEF-4c61-B030-A53F17634F7D}";
	unsigned int i = 0;
	unsigned int j;

	if (size < 0x26) {
		for (j = 0; j < 0x26; j++) {
			data[i++] ^= key[j];
			if (i >= size)
				i = 0;
		}
		return 0x26;
	}

	for (j = 0; j < size; j++) {
		data[j] ^= key[i++];
		if (i >= 0x26)
			i = 0;
	}
	return size;
}

#endif // PG_CLIENT_PROJECTG_ENCRYPTION_H
