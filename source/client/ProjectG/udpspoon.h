// Original: d:\build\custom\temp\code\client\projectg\udpspoon.h
#ifndef PG_CLIENT_PROJECTG_UDPSPOON_H
#define PG_CLIENT_PROJECTG_UDPSPOON_H

#include <winsock2.h>
#include <windows.h>

#include "..\..\defines.h"

// Receives a datagram: sender, data, length (called from UDPSpoon::Run).
typedef void (*SpoonRecvProc)(sockaddr_in* from, char* data, int length);
// Reports a failure message (UDPSpoon::Error).
typedef void (*SpoonErrorProc)(const char* message);

// What UDPSpoon::Open copies into the spoon (size 0xC from Open).
struct SpoonParam {
	SpoonRecvProc recvProc;   // 0x0
	SpoonErrorProc errorProc; // 0x4
	unsigned short port;      // 0x8, 0 = any port
};
CHECK_SIZE(SpoonParam, 0xC);
CHECK_FIELD(SpoonParam, recvProc, 0x0);
CHECK_FIELD(SpoonParam, errorProc, 0x4);
CHECK_FIELD(SpoonParam, port, 0x8);

// A UDP socket with a receiving loop (size 0x20 from the constructor, Open
// and Close).
class UDPSpoon {
public:
	UDPSpoon(void);
	virtual ~UDPSpoon(void);

	bool Open(SpoonParam& param);
	void Close(void);
	int Send(const char* data, int length, sockaddr_in* to);

private:
	void Error(const char* message);
	void Run(void);

	friend unsigned int __stdcall SubSpoonThread(void* param);
	LAYOUT_CHECK_FRIEND;

	SOCKET m_socket;       // 0x4
	HANDLE m_event;        // 0x8
	SpoonParam m_param;    // 0xC
	bool m_stop;           // 0x18
	HANDLE m_thread;       // 0x1C
};
CHECK_SIZE(UDPSpoon, 0x20);
BEGIN_LAYOUT_CHECK(UDPSpoon)
	CHECK_FIELD(UDPSpoon, m_socket, 0x4);
	CHECK_FIELD(UDPSpoon, m_event, 0x8);
	CHECK_FIELD(UDPSpoon, m_param, 0xC);
	CHECK_FIELD(UDPSpoon, m_stop, 0x18);
	CHECK_FIELD(UDPSpoon, m_thread, 0x1C);
END_LAYOUT_CHECK

unsigned int __stdcall SubSpoonThread(void* param);
void MakeSpoon(const char* address, int port, sockaddr_in* out);

#endif // PG_CLIENT_PROJECTG_UDPSPOON_H
