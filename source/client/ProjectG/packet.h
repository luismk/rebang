// Original: d:\build\custom\temp\code\client\projectg\packet.h
#ifndef PG_CLIENT_PROJECTG_PACKET_H
#define PG_CLIENT_PROJECTG_PACKET_H

#include <string>

#include "..\..\defines.h"

// Packet identifiers of the client protocol (enumClientPacket, values only
// known by number so far).
enum enumClientPacket {
	CLIENT_PACKET_UNKNOWN = 0
};

// Destination of WSendPacket::Send: the WNetworkSystem connection the packet
// goes out on (value 2 and anything above 4 send nothing).
enum eSendTo {
	SEND_TO_DEFAULT = 0,    // NET_UNIT_DEFAULT (GameUnit)
	SEND_TO_MSN = 1,        // NET_UNIT_MSN
	SEND_TO_LOGIN = 3,      // NET_UNIT_LOGIN
	SEND_TO_RANK = 4        // NET_UNIT_RANK
};

// A growable byte buffer (vtable ??_7WMemBlock, size 0x10 from the
// WSendPacket and WReceivedPacket layouts).
class WMemBlock {
public:
	WMemBlock(void);
	WMemBlock(unsigned int size);
	virtual ~WMemBlock(void);

	void ResetBuffer(void);
	int Free(void);
	int Alloc(unsigned int size);
	int ReAlloc(unsigned int size);
	// packet.h:33
	const unsigned char* GetBuffer(void) const {
		return m_buffer;
	}

	// packet.h:38
	unsigned char* LockBuffer(int offset) {
		return m_buffer + offset;
	}

	// packet.h:43
	unsigned int GetLength(void) const {
		return m_length;
	}

protected:
	// packet.h:57
	unsigned int GetBufferSize(void) {
		return m_bufferSize;
	}

	unsigned char* m_buffer;          // 0x4, GetBuffer
	unsigned int m_bufferSize;        // 0x8, GetBufferSize
	unsigned int m_length;            // 0xC, GetLength; read position of a WReceivedPacket
	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(WMemBlock, 0x10);
BEGIN_LAYOUT_CHECK(WMemBlock)
	CHECK_FIELD(WMemBlock, m_buffer, 0x4);
	CHECK_FIELD(WMemBlock, m_bufferSize, 0x8);
	CHECK_FIELD(WMemBlock, m_length, 0xC);
END_LAYOUT_CHECK

// An outgoing packet (size 0x18 from its stack use; vtable ??_7WSendPacket).
class WSendPacket : public WMemBlock {
public:
	WSendPacket(void);
	// packet.h:91
	WSendPacket(enumClientPacket type) : WMemBlock(0x40) { InitPacket(type); }
	// packet.h:92
	WSendPacket(int type) : WMemBlock(0x40) { InitPacket(type); }
	virtual ~WSendPacket(void);

	void InitPacket(int type);
	int GetPacketType(void) const;
	int GetMark(void) const;
	void EncodeData(int offset, char* data, int length);
	void MakePacketComplete(int option);
	void RollBack(void);
	void Send(eSendTo to);
	void Encode1(unsigned char value);
	void Encode2(unsigned short value);
	void Encode4(unsigned long value);
	void Encode8(__int64 value);
	void EncodeStr(const std::string& value);
	void EncodeEncryptedStr(const std::string& value);
	void EncodeBuffer(const void* data, unsigned short length);
	bool SetSendPacket(const unsigned char* data, unsigned int length, unsigned char flag, unsigned char option);
	unsigned int GetLengthData(void);

	static unsigned char ms_nPacketCount;  // sequence byte of MakePacketComplete

protected:
	void MakeCheckSum(void);

private:
	int m_packetType;                 // 0x10, GetPacketType
	unsigned char m_cryptKey;         // 0x14, set by MakePacketComplete, key of RollBack's decrypt
	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(WSendPacket, 0x18);
BEGIN_LAYOUT_CHECK(WSendPacket)
	CHECK_FIELD(WSendPacket, m_packetType, 0x10);
	CHECK_FIELD(WSendPacket, m_cryptKey, 0x14);
END_LAYOUT_CHECK

// An incoming packet (vtable ??_7WReceivedPacket). The WMemBlock length is
// its read position.
class WReceivedPacket : public WMemBlock {
public:
	WReceivedPacket(void);
	virtual ~WReceivedPacket(void);

	bool DecodeCheckSum(void);
	unsigned char Decode1(void);
	unsigned short Decode2(void);
	unsigned long Decode4(void);
	__int64 Decode8(void);
	std::string DecodeStr(void);
	bool DecodeStr(std::string& value);
	std::string DecodeEncryptedStr(unsigned long key);
	void DecodeBuffer(void* data, int length);
	bool SetRcvPacket(const unsigned char* data, unsigned int length, unsigned char flag, unsigned char option);
	void Copy(WReceivedPacket* pSource);
	bool IsValid(bool check);
	void Reset(void);
	unsigned char* GetRemainBuffer(void);
	int GetRemainLength(void);

private:
	int m_rcvOption;                  // 0x10, 4th argument of SetRcvPacket
	unsigned int m_rcvLength;         // 0x14, length given to SetRcvPacket
	unsigned int m_dataLength;        // 0x18, length given to SetRcvPacket
	unsigned char m_rcvFlag;          // 0x1C, 3rd argument of SetRcvPacket
	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(WReceivedPacket, 0x20);
BEGIN_LAYOUT_CHECK(WReceivedPacket)
	CHECK_FIELD(WReceivedPacket, m_rcvOption, 0x10);
	CHECK_FIELD(WReceivedPacket, m_rcvLength, 0x14);
	CHECK_FIELD(WReceivedPacket, m_dataLength, 0x18);
	CHECK_FIELD(WReceivedPacket, m_rcvFlag, 0x1C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_PACKET_H
