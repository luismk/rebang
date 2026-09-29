#pragma once

#include <string>

enum enumClientPacket;
enum eSendTo;

class WMemBlock
{
public:
	const unsigned char* GetBuffer() const { return m_pBuffer; }

	unsigned char* LockBuffer(int offset) { return m_pBuffer + offset; }

	unsigned int GetLength() const { return m_nLength; }

	WMemBlock();
	WMemBlock(unsigned int size);
	virtual ~WMemBlock();

	int Alloc(unsigned int size);
	int ReAlloc(unsigned int size);
	int Free();
	void ResetBuffer();

protected:
	unsigned int GetBufferSize() { return m_nBufferSize; }

	unsigned char* m_pBuffer;
	unsigned int m_nBufferSize;
	unsigned int m_nLength;
};

class WSendPacket : public WMemBlock
{
public:
	WSendPacket();
	WSendPacket(enumClientPacket type)
		: WMemBlock(0x40)
	{
		InitPacket(type);
	}
	WSendPacket(int type)
		: WMemBlock(0x40)
	{
		InitPacket(type);
	}
	virtual ~WSendPacket();

	void InitPacket(int type);
	int GetPacketType() const;
	int GetMark() const;
	unsigned int GetLengthData();

	void Encode1(unsigned char value);
	void Encode2(unsigned short value);
	void Encode4(unsigned long value);
	void Encode8(__int64 value);
	void EncodeStr(const std::string& str);
	void EncodeEncryptedStr(const std::string& str);
	void EncodeBuffer(const void* buf, unsigned short len);
	void EncodeData(int offset, char* data, int len);

	bool SetSendPacket(const unsigned char* data, unsigned int len,
		unsigned char key, unsigned char seed);
	void MakePacketComplete(int key);
	void RollBack();
	void Send(eSendTo to);

	static unsigned char ms_nPacketCount;

protected:
	void MakeCheckSum();

	int m_nPacketType;
	unsigned char m_nCryptKey;
};

class WReceivedPacket : public WMemBlock
{
public:
	WReceivedPacket();
	virtual ~WReceivedPacket();

	bool SetRcvPacket(const unsigned char* data, unsigned int len,
		unsigned char key, unsigned char seed);
	bool IsValid(bool bNoKey);
	void Copy(WReceivedPacket* dest);
	void Reset();

	bool DecodeCheckSum();
	unsigned char Decode1();
	unsigned short Decode2();
	unsigned long Decode4();
	__int64 Decode8();
	std::string DecodeStr();
	bool DecodeStr(std::string& str);
	std::string DecodeEncryptedStr(unsigned long key);
	void DecodeBuffer(void* buf, int len);

	unsigned char* GetRemainBuffer();
	int GetRemainLength();

protected:
	int m_nSeed;
	unsigned int m_nRcvLength;
	unsigned int m_nDataLength;
	unsigned char m_nKey;
};
