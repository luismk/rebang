// Original: d:\build\custom\temp\code\client\projectg\udpspoon.cpp
#include "UDPSpoon.h"

#include <string.h>

static char data_b79e80[0x1000];

// udpspoon.cpp:24
unsigned int __stdcall SubSpoonThread(void* param) {
	((UDPSpoon*)param)->Run();
	return 0;
}

// udpspoon.cpp:35
void MakeSpoon(const char* address, int port, sockaddr_in* out) {
	memset(out, 0, sizeof(sockaddr_in));
	out->sin_addr.s_addr = inet_addr(address);
	out->sin_family = AF_INET;
	out->sin_port = htons(port);
}

// udpspoon.cpp:49
UDPSpoon::UDPSpoon(void) {
	WSADATA wsaData;
	memset(&wsaData, 0, sizeof(wsaData));
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	m_stop = false;
}

// udpspoon.cpp:60
UDPSpoon::~UDPSpoon(void) {
	CloseHandle(m_event);
	WSACleanup();
}

// udpspoon.cpp:69
void UDPSpoon::Error(const char* message) {
	m_param.errorProc(message);
}

// udpspoon.cpp:78
bool UDPSpoon::Open(SpoonParam& param) {
	m_param = param;

	protoent* protocol = getprotobyname("UDP");

	if (protocol == NULL) {
		Error("getprotobyname(UDP) Failed");
		return false;
	}

	m_socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);

	if (m_socket == INVALID_SOCKET) {
		Error("WSASocket Faield");
		return false;
	}

	sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (m_param.port != 0) {
		address.sin_port = htons(param.port);
	} else {
		address.sin_port = htons(0);
	}

	if (bind(m_socket, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
		Error("bind Failed");
		return false;
	}

	m_event = CreateEvent(NULL, FALSE, FALSE, NULL);

	return true;
}

// udpspoon.cpp:141
void UDPSpoon::Close(void) {
	m_stop = true;
	WaitForSingleObject(m_thread, INFINITE);
	CloseHandle(m_thread);
	closesocket(m_socket);
}

// udpspoon.cpp:152
void UDPSpoon::Run(void) {
	WSANETWORKEVENTS networkEvents;

	while (!m_stop) {
		if (WSAWaitForMultipleEvents(1, &m_event, FALSE, 10, FALSE) == WAIT_OBJECT_0) {
			if (WSAEnumNetworkEvents(m_socket, m_event, &networkEvents) == SOCKET_ERROR) {
				break;
			}

			if (networkEvents.lNetworkEvents & FD_READ) {
				if (networkEvents.iErrorCode[FD_READ_BIT] == 0) {
					sockaddr_in from;
					int fromLength;

					memset(data_b79e80, 0, 0x400);

					fromLength = sizeof(from);
					int length = recvfrom(m_socket, data_b79e80, sizeof(data_b79e80), 0, (sockaddr*)&from, &fromLength);

					if (length == SOCKET_ERROR) {
						GetLastError();
					} else {
						m_param.recvProc(&from, data_b79e80, length);
					}
				}
			}
		}

		Sleep(1);
	}
}

// udpspoon.cpp:206
int UDPSpoon::Send(const char* data, int length, sockaddr_in* to) {
	return sendto(m_socket, data, length, 0, (sockaddr*)to, sizeof(sockaddr_in));
}
