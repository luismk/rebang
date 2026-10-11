// Original: d:\build\custom\temp\code\client\projectg\socket.cpp
#include "Socket.h"

#include "Exception.h"

int WSocket::ms_bInit;
sockaddr_in WSocket::ms_addrEmpty;

// socket.cpp:22
WSocket::WSocket(unsigned int socket, sockaddr_in address)
	: m_socket(INVALID_SOCKET), m_bConnected(0)
{
	if (!ms_bInit) Startup();

	if (socket == INVALID_SOCKET) {
		m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
		m_bConnected = 0;
	}
	else {
		m_socket = socket;
		m_address = address;
		m_bConnected = 1;
	}
}

// socket.cpp:42
WSocket::~WSocket(void)
{
	Close();
}

// socket.cpp:47
void WSocket::Startup(void)
{
	WSADATA wsaData;

	ms_bInit = 1;
	if (WSAStartup(0x101, &wsaData) != 0) {
		throw WSysException("! Socket initialization failed");
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		throw WSysException("! Cannot find usable winsock version");
	}
	memset(&ms_addrEmpty, 0, sizeof(ms_addrEmpty));
}

// socket.cpp:73
void WSocket::Cleanup(void)
{
	WSACleanup();
}

// socket.cpp:82
void WSocket::CreateSocket(void)
{
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	m_bConnected = 0;
}

// socket.cpp:97
void WSocket::Close(void)
{
	if (m_bConnected || m_socket != INVALID_SOCKET) {
		if (closesocket(m_socket) != 0) {
			int error = GetLastError();
			if (error == WSAEWOULDBLOCK) {
				linger lingerNew;
				linger lingerOld;
				int length;

				lingerNew.l_onoff = 1;
				lingerNew.l_linger = 0;
				length = sizeof(lingerOld);
				getsockopt(m_socket, SOL_SOCKET, SO_LINGER, (char*)&lingerOld, &length);
				setsockopt(m_socket, SOL_SOCKET, SO_LINGER, (char*)&lingerNew, sizeof(lingerNew));
				closesocket(m_socket);
				setsockopt(m_socket, SOL_SOCKET, SO_LINGER, (char*)&lingerOld, sizeof(lingerOld));
				m_bConnected = 0;
				m_socket = INVALID_SOCKET;
			}
		}
	}
	m_bConnected = 0;
	m_socket = INVALID_SOCKET;
}

// socket.cpp:145
int WSocket::Poll(int timeout)
{
	timeval time;
	fd_set readSet;
	fd_set exceptSet;
	fd_set writeSet;

	time.tv_sec = timeout / 1000;
	time.tv_usec = timeout % 1000 * 1000;
	readSet.fd_count = 1;
	writeSet.fd_count = 0;
	exceptSet.fd_count = 0;
	readSet.fd_array[0] = m_socket;

	int result = select(m_socket + 1, &readSet, &writeSet, &exceptSet, &time);

	return result != 0;
}

// socket.cpp:160
std::string& WSocket::GetAddressString(std::string& str)
{
	return str;
}

// socket.cpp:166
void WSocket::Listen(void)
{
	if (listen(m_socket, 5) != 0) {
		throw WSysException("! Socket listen error");
	}
	m_bConnected = 1;
}

// socket.cpp:177
WSocket* WSocket::Accept(void)
{
	sockaddr_in address;
	int length = sizeof(address);

	unsigned int socket = accept(m_socket, (sockaddr*)&address, &length);

	if (socket == INVALID_SOCKET) {
		std::string message;
	}
	return 0;
}

// socket.cpp:203
int WSocket::Accept(WSocket& socket, sockaddr* address, int* length)
{
	sockaddr_in addressIn;
	int lengthIn = sizeof(addressIn);

	unsigned int newSocket = accept(m_socket, (sockaddr*)&addressIn, &lengthIn);
	if (newSocket != INVALID_SOCKET) {
		socket.m_socket = newSocket;
		socket.m_address = addressIn;
		socket.m_bConnected = 1;
		m_bConnected = 1;
		if (address) *(sockaddr_in*)address = addressIn;
		if (length) *length = lengthIn;
		return 1;
	}
	else {
		std::string message;
	}
	return 0;
}

// socket.cpp:233
void WSocket::Bind(unsigned long address, unsigned short port)
{
	m_address.sin_family = AF_INET;
	m_address.sin_addr.s_addr = address;
	m_address.sin_port = port;
	if (bind(m_socket, (sockaddr*)&m_address, sizeof(m_address)) != 0) {
		throw WSysException("! Socket bind error");
	}
}

// socket.cpp:243
std::string& WSocket::GetInfo(std::string& str)
{
	std::string info;

	return str;
}

// socket.cpp:251
// The body between the braces (lines 252-257) was compiled out in the
// original: only the return of the caller's string remains.
std::string& WSocket::GetAddressString(std::string& str, unsigned long address)
{
	return str;
}

// socket.cpp:261
int WSocket::Connect(unsigned long address, unsigned short port)
{
	m_address.sin_family = AF_INET;
	m_address.sin_addr.s_addr = address;
	m_address.sin_port = port;

	int result = connect(m_socket, (sockaddr*)&m_address, sizeof(m_address));
	if (result == 0) {
		m_bConnected = 1;
	}
	else {
		GetLastError();
	}
	return m_bConnected;
}

// socket.cpp:279
int WSocket::Read(unsigned char* buffer, int length)
{
	return recv(m_socket, (char*)buffer, length, 0);
}

// socket.cpp:284
int WSocket::Write(const unsigned char* buffer, int length)
{
	return send(m_socket, (const char*)buffer, length, 0);
}

// socket.cpp:289
int WSocket::SetSocketOpt(int level, int name, const char* value, int length)
{
	return setsockopt(m_socket, level, name, value, length);
}

// socket.cpp:294
int WSocket::GetSocketOpt(int level, int name, char* value, int* length)
{
	return getsockopt(m_socket, level, name, value, length);
}
