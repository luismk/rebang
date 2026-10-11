// Original: d:\build\custom\temp\code\client\projectg\packet.cpp
#include "Packet.h"
#include "NetworkSystem.h"

#include "../shared/uselzo.h"

#include <stdlib.h>
#include <string.h>

// jrencrypt.cpp (no header reconstructed yet).
void SimpleStreamEncrypt_Alpha(const char* source, char* dest, unsigned int length, unsigned int key);
void SimpleStreamDecrypt_Alpha(const char* source, char* dest, unsigned int length, unsigned int key);

// Key tables of MakePacketComplete, indexed by [option][random] (no symbol).
extern "C" const unsigned char data_aa0518[][256];
extern "C" const unsigned char data_aa1518[][256];

// packet.cpp:42
WMemBlock::WMemBlock(void) : m_bufferSize(0), m_length(0) {
	m_buffer = NULL;
}

// packet.cpp:60
WMemBlock::WMemBlock(unsigned int size) : m_bufferSize(0), m_length(0) {
	m_buffer = NULL;
	Alloc(size);
}

// packet.cpp:94
void WMemBlock::ResetBuffer(void) {
	memset(m_buffer, 0, m_bufferSize);
	m_length = 0;
}

// packet.cpp:102
WMemBlock::~WMemBlock(void) {
	Free();
}

// packet.cpp:110
int WMemBlock::Alloc(unsigned int size) {
	if (size <= m_bufferSize) {
		m_length = 0;
	} else {
		Free();
		m_buffer = (unsigned char*)malloc(size);
		m_length = 0;
		m_bufferSize = size;
	}
	return 1;
}

// packet.cpp:140
int WMemBlock::ReAlloc(unsigned int size) {
	if (size > GetBufferSize()) {
		if (m_buffer == NULL)
			return Alloc(size);
		if (size > 0) {
			m_buffer = (unsigned char*)realloc(m_buffer, size);
			if (m_buffer != NULL) {
				m_bufferSize = size;
			} else {
				return 0;
			}
		}
	}
	return 1;
}

// packet.cpp:190
int WMemBlock::Free(void) {
	if (m_buffer != NULL) {
		free(m_buffer);
		m_length = 0;
		m_bufferSize = 0;
		m_buffer = NULL;
	}
	return 1;
}

// packet.cpp:220
WSendPacket::WSendPacket(void) : WMemBlock(0x2000) {
	m_packetType = 0;
	m_length = 0;
}

// packet.cpp:227
WSendPacket::~WSendPacket(void) {
}

// packet.cpp:231
int WSendPacket::GetPacketType(void) const {
	return m_packetType;
}

// packet.cpp:245
void WSendPacket::InitPacket(int type) {
	m_packetType = type;
	m_length = 0;
	Encode1(0);
	Encode2(0);
	Encode1(0);
	Encode1(0);

	Encode2((unsigned short)m_packetType);
}

// packet.cpp:260
void WSendPacket::MakeCheckSum(void) {
	unsigned char sum = 0;
	for (unsigned int i = 3; i < m_length; i++)
		sum += m_buffer[i];
	Encode1(sum);
}

// packet.cpp:268
void WSendPacket::MakePacketComplete(int option) {
	unsigned short length = (unsigned short)(m_length - 4);
	unsigned char* buffer = m_buffer;
	unsigned char random = (unsigned char)(rand() % 256);
	buffer[0] = random;
	buffer[1] = (unsigned char)length;
	buffer[2] = (unsigned char)(length >> 8);
	buffer[3] = ms_nPacketCount;
	ms_nPacketCount++;
	if (option != -1) {
		unsigned char key = data_aa0518[option][random];
		buffer[4] = data_aa1518[option][random];
		SimpleStreamEncrypt_Alpha((const char*)(buffer + 4), (char*)(buffer + 4), length, key);
		m_cryptKey = key;
	}
}

// packet.cpp:309
void WSendPacket::RollBack(void) {
	SimpleStreamDecrypt_Alpha((const char*)(m_buffer + 4), (char*)(m_buffer + 4), (unsigned short)(m_length - 4) + 1, m_cryptKey);
}

// packet.cpp:317
int WSendPacket::GetMark(void) const {
	return m_length - 7;
}

// packet.cpp:324
void WSendPacket::Send(eSendTo to) {
	WNetworkSystem::eNetUnit unit;

	if (to != SEND_TO_DEFAULT)
		--ms_nPacketCount;

	switch (to) {
	case SEND_TO_DEFAULT:
		unit = WNetworkSystem::NET_UNIT_DEFAULT;
		break;
	case SEND_TO_MSN:
		unit = WNetworkSystem::NET_UNIT_MSN;
		break;
	case SEND_TO_LOGIN:
		unit = WNetworkSystem::NET_UNIT_LOGIN;
		break;
	case SEND_TO_RANK:
		unit = WNetworkSystem::NET_UNIT_RANK;
		break;
	default:
		return;
	}

	WClientSocket *socket = WNetworkSystem::Instance()->GetSocket(unit);
	if (socket) {
		socket->SendPacket(*this);
		WNetworkSystem::Instance()->WriteSendTime();
	}
}

// packet.cpp:382
void WSendPacket::Encode1(unsigned char value) {
	if (m_length >= m_bufferSize)
		ReAlloc(m_bufferSize * 2);
	m_buffer[m_length++] = value;
}

// packet.cpp:390
void WSendPacket::Encode2(unsigned short value) {
	Encode1((unsigned char)value);
	Encode1((unsigned char)(value >> 8));
}

// packet.cpp:396
void WSendPacket::Encode4(unsigned long value) {
	Encode1((unsigned char)value);
	Encode1((unsigned char)(value >> 8));
	Encode1((unsigned char)(value >> 16));
	Encode1((unsigned char)(value >> 24));
}

// packet.cpp:405
void WSendPacket::Encode8(__int64 value) {
	Encode1((unsigned char)value);
	Encode1((unsigned char)(value >> 8));
	Encode1((unsigned char)(value >> 16));
	Encode1((unsigned char)(value >> 24));
	Encode1((unsigned char)(value >> 32));
	Encode1((unsigned char)(value >> 40));
	Encode1((unsigned char)(value >> 48));
	Encode1((unsigned char)(value >> 56));
}

// packet.cpp:418
void WSendPacket::EncodeStr(const std::string& value) {
	unsigned short length = (unsigned short)value.length();
	Encode2(length);
	if (m_length + length >= m_bufferSize)
		ReAlloc(length >= m_bufferSize ? m_bufferSize * 2 + length : m_bufferSize * 2);
	memcpy(m_buffer + m_length, value.c_str(), length);
	m_length += length;
}

// packet.cpp:438
void WSendPacket::EncodeEncryptedStr(const std::string& value) {
	EncodeStr(value);
}

// packet.cpp:444
void WSendPacket::EncodeBuffer(const void* data, unsigned short length) {
	if (m_length + length >= m_bufferSize)
		ReAlloc(length >= m_bufferSize ? m_bufferSize * 2 + length : m_bufferSize * 2);
	memcpy(m_buffer + m_length, data, length);
	m_length += length;
}

// packet.cpp:454
void WSendPacket::EncodeData(int offset, char* data, int length) {
	memcpy(m_buffer + offset + 7, data, length);
}

// packet.cpp:461
bool WSendPacket::SetSendPacket(const unsigned char* data, unsigned int length, unsigned char flag, unsigned char option) {
	Alloc(length);
	memcpy(m_buffer, data, length);
	m_length = length;
	return true;
}

// packet.cpp:500
unsigned int WSendPacket::GetLengthData(void) {
	unsigned int length = m_length;
	return length <= 7 ? 0 : length - 7;
}

// packet.cpp:514
WReceivedPacket::WReceivedPacket(void) : m_rcvOption(0), m_rcvFlag(0) {
}

// packet.cpp:518
WReceivedPacket::~WReceivedPacket(void) {
}

// packet.cpp:524
bool WReceivedPacket::SetRcvPacket(const unsigned char* data, unsigned int length, unsigned char flag, unsigned char option) {
	Alloc(length);
	memcpy(m_buffer, data, length);
	m_dataLength = length;
	m_length = 0;
	m_rcvOption = option;
	m_rcvFlag = flag;
	m_rcvLength = length;
	return true;
}

// packet.cpp:564
void WReceivedPacket::Copy(WReceivedPacket* pTarget) {
	pTarget->SetRcvPacket(m_buffer, m_dataLength, m_rcvFlag, (unsigned char)m_rcvOption);
	pTarget->m_length = 1;
}

// packet.cpp:572
bool WReceivedPacket::IsValid(bool check) {
	if (!check) {
		unsigned char key = data_aa0518[m_rcvFlag][m_rcvOption];
		unsigned char checkSum = data_aa1518[m_rcvFlag][m_rcvOption];
		if (m_buffer == NULL)
			return false;

		SimpleStreamDecrypt_Alpha((const char*)m_buffer, (char*)m_buffer, m_dataLength, key);

		CCompressBuffer compressBuffer;
		unsigned int result = compressBuffer.Decompress(m_buffer + 1, m_dataLength - 1);
		if (result > 0)
			return false;

		int newSize = compressBuffer.GetNewSize();
		if (compressBuffer.GetNewBuffer() != NULL && compressBuffer.GetNewSize() != 0) {
			if (m_bufferSize < (unsigned int)(newSize + 4))
				ReAlloc(newSize + 4);
			memcpy(m_buffer + 1, compressBuffer.GetNewBuffer(), newSize);
			m_dataLength = newSize + 1;
			if (checkSum != Decode1())
				return false;
		} else {
			return false;
		}
	}
	return true;
}

// packet.cpp:631
bool WReceivedPacket::DecodeCheckSum(void) {
	m_length += 1;
	return true;
}

// packet.cpp:649
unsigned char WReceivedPacket::Decode1(void) {
	return m_buffer[m_length++];
}

// packet.cpp:658
unsigned short WReceivedPacket::Decode2(void) {
	unsigned short value = (unsigned short)(m_buffer[m_length] | (m_buffer[m_length + 1] << 8));
	m_length += 2;
	return value;
}

// packet.cpp:668
unsigned long WReceivedPacket::Decode4(void) {
	unsigned short low = Decode2();
	unsigned short high = Decode2();
	return (unsigned long)low | ((unsigned long)high << 16);
}

// packet.cpp:676
__int64 WReceivedPacket::Decode8(void) {
	unsigned long low = Decode4();
	unsigned long high = Decode4();
	return low | (high << 32);
}

// packet.cpp:684
std::string WReceivedPacket::DecodeStr(void) {
	unsigned short length = Decode2();

	std::string value;
	value.append((const char*)(m_buffer + m_length), length);
	m_length += length;

	return value;
}

// packet.cpp:697
bool WReceivedPacket::DecodeStr(std::string& value) {
	unsigned short length = Decode2();
	if (m_length + length > m_bufferSize)
		return false;

	value.assign((char*)(m_buffer + m_length), (char*)(m_buffer + m_length + length));

	m_length += length;
	return true;
}

// packet.cpp:709
std::string WReceivedPacket::DecodeEncryptedStr(unsigned long key) {
	std::string value;
	DecodeStr(value);
	return value;
}

// packet.cpp:717
void WReceivedPacket::DecodeBuffer(void* data, int length) {
	memcpy(data, m_buffer + m_length, length);
	m_length += length;
}

// packet.cpp:725
void WReceivedPacket::Reset(void) {
	m_length = 1;
}

// packet.cpp:732
unsigned char* WReceivedPacket::GetRemainBuffer(void) {
	return m_buffer + m_length;
}

// packet.cpp:737
int WReceivedPacket::GetRemainLength(void) {
	return m_dataLength - m_length;
}
