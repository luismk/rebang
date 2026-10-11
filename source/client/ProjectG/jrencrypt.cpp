// Original: d:\build\custom\temp\code\client\projectg\jrencrypt.cpp
#include <new>
#include <string.h>

// Stream ciphers of the network packets. The stream is processed as 32-bit
// words followed by up to three trailing bytes; each trailing byte is XORed
// with one byte of the last word (or of the key when there is no word).

// jrencrypt.cpp:29
void SimpleStreamEncrypt_Alpha(const char* source, char* dest, unsigned int length, unsigned int key) {
	char* buffer = dest;
	bool allocated = false;

	if (source == dest) {
		buffer = new char[length];
		allocated = true;
	}

	unsigned int wordCount = length >> 2;
	unsigned int byteCount = length & 3;
	unsigned int last;
	unsigned int offset;
	if (wordCount > 0) {
		((unsigned int*)buffer)[0] = ((const unsigned int*)source)[0] ^ key;
		for (unsigned int i = 1; i < wordCount; i++)
			((unsigned int*)buffer)[i] = ((const unsigned int*)source)[i - 1] ^ ((const unsigned int*)source)[i];
		last = ((const unsigned int*)source)[wordCount - 1];
		offset = wordCount * 4;
	} else {
		last = key;
		offset = 0;
	}

	for (unsigned int i = 0; i < byteCount; i++)
		buffer[offset + i] = source[offset + i] ^ (char)(last >> (i * 8));

	memcpy((void*)source, buffer, length);

	if (allocated)
		delete[] buffer;
}

// jrencrypt.cpp:76
void SimpleStreamDecrypt_Alpha(const char* source, char* dest, unsigned int length, unsigned int key) {
	char* buffer = dest;
	bool allocated = false;
	if (source == dest) {
		buffer = new char[length];
		allocated = true;
	}

	unsigned int wordCount = length >> 2;
	unsigned int byteCount = length & 3;
	unsigned int last;
	unsigned int offset;
	if (wordCount > 0) {
		((unsigned int*)buffer)[0] = ((const unsigned int*)source)[0] ^ key;
		for (unsigned int i = 1; i < wordCount; i++)
			((unsigned int*)buffer)[i] = ((const unsigned int*)source)[i] ^ ((unsigned int*)buffer)[i - 1];
		last = ((unsigned int*)buffer)[wordCount - 1];
		offset = wordCount * 4;
	} else {
		last = key;
		offset = 0;
	}

	for (unsigned int i = 0; i < byteCount; i++)
		buffer[offset + i] = source[offset + i] ^ (char)(last >> (i * 8));

	memcpy((void*)source, buffer, length);

	if (allocated)
		delete[] buffer;
}

// jrencrypt.cpp:123
void SimpleStreamEncrypt_Delta(const char* source, char* dest, unsigned int length, unsigned int key) {
	unsigned int wordCount = length >> 2;
	unsigned int byteCount = length & 3;
	unsigned int last;
	unsigned int offset;
	if (wordCount > 0) {
		((unsigned int*)dest)[0] = ((const unsigned int*)source)[0] ^ key ^ 0x9C6C95CE;
		for (unsigned int i = 1; i < wordCount; i++)
			((unsigned int*)dest)[i] = (i + 966) * (i + 2020) * i * i ^ ((const unsigned int*)source)[i - 1] ^ ((const unsigned int*)source)[i] ^ 0x9C6C95CE;
		last = ((const unsigned int*)source)[wordCount - 1];
		offset = wordCount * 4;
	} else {
		last = key;
		offset = 0;
	}

	for (unsigned int i = 0; i < byteCount; i++)
		dest[offset + i] = source[offset + i] ^ (char)(last >> (i * 8));
}

// jrencrypt.cpp:149
void SimpleStreamDecrypt_Delta(const char* source, char* dest, unsigned int length, unsigned int key) {
	unsigned int wordCount = length >> 2;
	unsigned int byteCount = length & 3;
	unsigned int last;
	unsigned int offset;
	if (wordCount > 0) {
		((unsigned int*)dest)[0] = ((const unsigned int*)source)[0] ^ key ^ 0x9C6C95CE;
		for (unsigned int i = 1; i < wordCount; i++)
			((unsigned int*)dest)[i] = (i + 966) * (i + 2020) * i * i ^ ((const unsigned int*)source)[i] ^ ((unsigned int*)dest)[i - 1] ^ 0x9C6C95CE;
		last = ((unsigned int*)dest)[wordCount - 1];
		offset = wordCount * 4;
	} else {
		last = key;
		offset = 0;
	}

	for (unsigned int i = 0; i < byteCount; i++)
		dest[offset + i] = source[offset + i] ^ (char)(last >> (i * 8));
}
