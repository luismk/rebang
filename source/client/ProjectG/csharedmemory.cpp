// Original: d:\build\custom\temp\code\client\projectg\csharedmemory.cpp
#include <string.h>
#include <windows.h>
#include "cSharedMemory.h"

// csharedmemory.cpp:17
cIPCMemServer::cIPCMemServer() {
	m_hMap = NULL;
}

// csharedmemory.cpp:21
cIPCMemServer::~cIPCMemServer() {
	if (m_hMap) CloseHandle(m_hMap);
}

// csharedmemory.cpp:26
bool cIPCMemServer::Create(const char* name) {
	m_hMap = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, IPC_MEM_SIZE, name);

	return m_hMap ? true : false;
}

// csharedmemory.cpp:39
bool cIPCMemServer::Send(void* data, int size, int offset) {
	if (m_hMap == NULL)
		return false;

	if (size > IPC_MEM_SIZE)
		return false;

	void* view = MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (view == NULL)
		return false;

	memcpy(view, data, size);

	return true;
}

// csharedmemory.cpp:79
bool cIPCMemServer::Recv(void* data, int size, int offset) {
	if (m_hMap == NULL)
		return false;

	if (size > IPC_MEM_SIZE)
		return false;

	void* view = MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (view == NULL)
		return false;

	memcpy(data, view, size);

	return true;
}

// csharedmemory.cpp:105
void cIPCMemServer::ShutDown(void) {
	if (m_hMap)
		CloseHandle(m_hMap);
}

// csharedmemory.cpp:116
cIPCMemClient::cIPCMemClient() {
	m_hMap = NULL;
}

// csharedmemory.cpp:120
cIPCMemClient::~cIPCMemClient() {
	if (m_hMap) CloseHandle(m_hMap);
}

// csharedmemory.cpp:125
bool cIPCMemClient::Connection(const char* name) {
	m_hMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, name);

	return m_hMap ? true : false;
}

// csharedmemory.cpp:136
bool cIPCMemClient::Send(void* data, int size) {
	if (m_hMap == NULL)
		return false;

	if (size > IPC_MEM_SIZE)
		return false;

	void* view = MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (view == NULL)
		return false;

	memcpy(view, data, size);

	return true;
}

// csharedmemory.cpp:158
bool cIPCMemClient::Recv(void* data, int size) {
	if (m_hMap == NULL)
		return false;

	void* view = MapViewOfFile(m_hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (view == NULL)
		return false;

	memcpy(data, view, size);

	return true;
}

// csharedmemory.cpp:174
void cIPCMemClient::Close(void) {
	if (m_hMap)
		CloseHandle(m_hMap);
}
