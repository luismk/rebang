// Original: d:\build\custom\temp\code\client\projectg\imessage.cpp
#include <string.h>
#include <string>
#include "IMessage.h"

// imessage.cpp:20
MemBlock::MemBlock() {
	m_pBuffer = m_buffer;
	m_bufferSize = 0x2000;
	m_length = 0;
}

// imessage.cpp:28
MemBlock::MemBlock(unsigned int size) {
	m_pBuffer = m_buffer;
	m_bufferSize = 0x2000;
	m_length = 0;
}

// imessage.cpp:35
void MemBlock::Reset() {
	m_pBuffer = m_buffer;
	memset(m_buffer, 0, sizeof(m_buffer));
	m_length = 0;
	m_bufferSize = 0x2000;
}

// imessage.cpp:43
MemBlock::~MemBlock() {
}

// imessage.cpp:48
int MemBlock::Alloc(unsigned int size) {
	return 1;
}

// imessage.cpp:66
int MemBlock::ReAlloc(unsigned int size) {
	return 1;
}

// imessage.cpp:94
int MemBlock::Free() {
	return 1;
}

// imessage.cpp:111
ISendMsg::ISendMsg() {
	m_msgId = 0;
	m_length = 0;
}

// imessage.cpp:118
ISendMsg::~ISendMsg() {
}

// imessage.cpp:126
ISendMsg::ISendMsg(unsigned short msgId) {
	m_length = 0;
	Encode2(0);
	m_msgId = msgId;
	Encode2(msgId);
}

// imessage.cpp:138
void ISendMsg::MakeCheckSum() {
	unsigned char sum = 0;
	for (unsigned int i = 4; i < m_length; i++)
		sum += m_pBuffer[i];
	m_pBuffer[m_length] = sum;
	m_length += 1;
}

// imessage.cpp:150
void ISendMsg::MakePacketComplete() {
	unsigned short size = (unsigned short)(m_length - 2);
	m_buffer[0] = (unsigned char)size;
	m_buffer[1] = (unsigned char)(size >> 8);
}

// imessage.cpp:163
void ISendMsg::Encode1(unsigned char value) {
	m_pBuffer[m_length] = value;
	m_length += 1;
}

// imessage.cpp:173
void ISendMsg::Encode2(unsigned short value) {
	m_pBuffer[m_length] = (unsigned char)value;
	m_length += 1;
	m_pBuffer[m_length] = (unsigned char)(value >> 8);
	m_length += 1;
}

// imessage.cpp:182
void ISendMsg::Encode4(unsigned long value) {
	m_pBuffer[m_length] = (unsigned char)value;
	m_length += 1;
	m_pBuffer[m_length] = (unsigned char)(value >> 8);
	m_length += 1;
	m_pBuffer[m_length] = (unsigned char)(value >> 16);
	m_length += 1;
	m_pBuffer[m_length] = (unsigned char)(value >> 24);
	m_length += 1;
}

// imessage.cpp:193
void ISendMsg::EncodeBuffer(const void* data, unsigned short size) {
	memcpy(m_pBuffer + m_length, data, size);
	m_length += size;
}

// imessage.cpp:207
void ISendMsg::EncodeStr(const char* str) {
	unsigned short length = (unsigned short)strlen(str);
	if (length != 0) {
		Encode2(length);
		memcpy(m_pBuffer + m_length, str, length);
		m_length += length;
	}
}

// imessage.cpp:226
void ISendMsg::FixData(int offset, char* data, int size) {
	memcpy(m_pBuffer + 4 + offset, data, size);
}

// imessage.cpp:236
IRecvMsg::IRecvMsg() {
}

// imessage.cpp:243
IRecvMsg::~IRecvMsg() {
}

// imessage.cpp:250
unsigned char IRecvMsg::Decode1() {
	if (m_length + 1 > m_packetSize) {
		m_length = m_packetSize;
		return 0;
	}
	unsigned char value = m_pBuffer[m_length];
	m_length += 1;
	return value;
}

// imessage.cpp:266
unsigned short IRecvMsg::Decode2() {
	if (m_length + 2 > m_packetSize) {
		m_length = m_packetSize;
		return 0;
	}
	unsigned short value = (unsigned short)(m_pBuffer[m_length] | (m_pBuffer[m_length + 1] << 8));
	m_length += 2;
	return value;
}

// imessage.cpp:284
unsigned long IRecvMsg::Decode4() {
	if (m_length + 4 > m_packetSize) {
		m_length = m_packetSize;
		return 0;
	}
	unsigned long value = m_pBuffer[m_length] | (m_pBuffer[m_length + 1] << 8) | (m_pBuffer[m_length + 2] << 16) | (m_pBuffer[m_length + 3] << 24);
	m_length += 4;
	return value;
}

// imessage.cpp:301
std::string IRecvMsg::DecodeStr() {
	unsigned short length = Decode2();
	if (length == 0)
		return "";
	if (m_length + length > m_packetSize) {
		m_length = m_packetSize;
		return "";
	}
	std::string str("");
	str.append((const char*)m_pBuffer + m_length, length);
	m_length += length;
	return str;
}

// imessage.cpp:326
void IRecvMsg::DecodeBuffer(void* data, int size) {
	if (m_length + size > m_packetSize) {
		m_length = m_packetSize;
		memset(data, 0, size);
		return;
	}
	memcpy(data, m_pBuffer + m_length, size);
	m_length += size;
}

// imessage.cpp:344
bool IRecvMsg::DecodeCheckSum() {
	unsigned char sum = m_pBuffer[m_length];
	m_length += 1;
	for (unsigned int i = 0; i < m_packetSize - 1; i++)
		sum -= m_pBuffer[i];
	return sum == 0 ? true : false;
}

// imessage.cpp:363
bool IRecvMsg::IsValid(bool check) {
	return true;
}

// imessage.cpp:372
void IRecvMsg::SetRcvPacket(const char* data, int size) {
	memset(m_buffer, 0, sizeof(m_buffer));
	m_pBuffer = m_buffer;
	m_packetSize = size;
	m_bufferSize = 0x2000;
	m_length = 0;
	memcpy(m_buffer, data, size);
	m_packetSize = size;
}
