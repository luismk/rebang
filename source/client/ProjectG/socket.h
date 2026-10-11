// Original: d:\build\custom\temp\code\client\projectg\socket.h
#ifndef PG_CLIENT_PROJECTG_SOCKET_H
#define PG_CLIENT_PROJECTG_SOCKET_H

#include <winsock2.h>
#include <windows.h>

#include <string>

#include "..\..\defines.h"

// A Winsock socket (size 0x1C from WSocket::WSocket). Vtable order from
// ??_7WSocket@@6B@: destructor, CreateSocket, Close, Read, Write.
class WSocket {
public:
	WSocket(unsigned int socket, sockaddr_in address);
	virtual ~WSocket(void);
	virtual void CreateSocket(void);
	virtual void Close(void);
	virtual int Read(unsigned char* buffer, int length);
	virtual int Write(const unsigned char* buffer, int length);

	static void Startup(void);
	static void Cleanup(void);

	int Poll(int timeout);
	WSocket* Accept(void);
	int Accept(WSocket& socket, sockaddr* address, int* length);
	void Listen(void);
	void Bind(unsigned long address, unsigned short port);
	int Connect(unsigned long address, unsigned short port);
	int SetSocketOpt(int level, int name, const char* value, int length);
	int GetSocketOpt(int level, int name, char* value, int* length);

	std::string& GetInfo(std::string& str);
	std::string& GetAddressString(std::string& str);
	static std::string& GetAddressString(std::string& str, unsigned long address);

	unsigned int m_socket;          // 0x4, INVALID_SOCKET (-1) when none
	sockaddr_in m_address;          // 0x8
	int m_bConnected;               // 0x18, NetworkUnit::IsConnected

protected:
	static int ms_bInit;            // set by Startup
	static sockaddr_in ms_addrEmpty;
	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(WSocket, 0x1C);
BEGIN_LAYOUT_CHECK(WSocket)
	CHECK_FIELD(WSocket, m_socket, 0x4);
	CHECK_FIELD(WSocket, m_address, 0x8);
	CHECK_FIELD(WSocket, m_bConnected, 0x18);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SOCKET_H
