// Original: d:\build\custom\temp\code\client\projectg\imessage.h
#ifndef PG_CLIENT_PROJECTG_IMESSAGE_H
#define PG_CLIENT_PROJECTG_IMESSAGE_H

#include <string>

#include "..\..\defines.h"

// A fixed 0x2000-byte packet buffer with a write/read cursor.
class MemBlock {
public:
	MemBlock();
	MemBlock(unsigned int size);
	virtual ~MemBlock();            // vtable slot 0

	// imessage.h:23
	const unsigned char* GetBuffer() const {
		return m_buffer;
	}

	// imessage.h:28
	unsigned char* LockBuffer(int offset) {
		return m_buffer + offset;
	}

	// imessage.h:33
	unsigned int GetLength() const {
		return m_length;
	}

	void Reset();
	int Alloc(unsigned int size);
	int ReAlloc(unsigned int size);
	int Free();

	unsigned char m_buffer[0x2000]; // 0x4
	unsigned char* m_pBuffer;       // 0x2004, points at m_buffer
	unsigned int m_bufferSize;      // 0x2008, always 0x2000
	unsigned int m_length;          // 0x200c, write length / read cursor
};
CHECK_SIZE(MemBlock, 0x2010);
CHECK_FIELD(MemBlock, m_buffer, 0x4);
CHECK_FIELD(MemBlock, m_pBuffer, 0x2004);
CHECK_FIELD(MemBlock, m_bufferSize, 0x2008);
CHECK_FIELD(MemBlock, m_length, 0x200c);

// An outgoing packet: a 2-byte length, the 2-byte message id, then the data.
class ISendMsg : public MemBlock {
public:
	ISendMsg();
	ISendMsg(unsigned short msgId);
	virtual ~ISendMsg();

	// imessage.h:77
	virtual void RollBack() {
	}
	virtual void MakePacketComplete();
	virtual void MakeCheckSum();

	void Encode1(unsigned char value);
	void Encode2(unsigned short value);
	void Encode4(unsigned long value);
	void EncodeBuffer(const void* data, unsigned short size);
	void EncodeStr(const char* str);
	void FixData(int offset, char* data, int size);

	unsigned int m_msgId;           // 0x2010
};
CHECK_SIZE(ISendMsg, 0x2014);
CHECK_FIELD(ISendMsg, m_msgId, 0x2010);

// An incoming packet, decoded from the start of its buffer.
class IRecvMsg : public MemBlock {
public:
	IRecvMsg();
	virtual ~IRecvMsg();

	// imessage.h:107
	virtual void DecodeComplete() {
	}

	unsigned char Decode1();
	unsigned short Decode2();
	unsigned long Decode4();
	std::string DecodeStr();
	void DecodeBuffer(void* data, int size);
	bool DecodeCheckSum();
	bool IsValid(bool check);
	void SetRcvPacket(const char* data, int size);

	unsigned int m_packetSize;      // 0x2010
};
CHECK_SIZE(IRecvMsg, 0x2014);
CHECK_FIELD(IRecvMsg, m_packetSize, 0x2010);

#endif
