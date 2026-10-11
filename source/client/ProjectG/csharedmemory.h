// Original: d:\build\custom\temp\code\client\projectg\csharedmemory.h
#ifndef PG_CLIENT_PROJECTG_CSHAREDMEMORY_H
#define PG_CLIENT_PROJECTG_CSHAREDMEMORY_H

#include <windows.h>

#include "..\..\defines.h"

// Size of the named file mapping shared by cIPCMemServer and cIPCMemClient.
#define IPC_MEM_SIZE 0x1000

// Creates a named page-file mapping and copies blocks in and out of it.
class cIPCMemServer {
public:
	cIPCMemServer();
	~cIPCMemServer();

	bool Create(const char* name);
	bool Send(void* data, int size, int offset);
	bool Recv(void* data, int size, int offset);
	void ShutDown(void);

private:
	HANDLE m_hMap;      // 0x0

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(cIPCMemServer, 0x4);
BEGIN_LAYOUT_CHECK(cIPCMemServer)
	CHECK_FIELD(cIPCMemServer, m_hMap, 0x0);
END_LAYOUT_CHECK

// Opens the mapping a cIPCMemServer created and copies blocks in and out of it.
class cIPCMemClient {
public:
	cIPCMemClient();
	~cIPCMemClient();

	bool Connection(const char* name);
	bool Send(void* data, int size);
	bool Recv(void* data, int size);
	void Close(void);

private:
	HANDLE m_hMap;      // 0x0

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(cIPCMemClient, 0x4);
BEGIN_LAYOUT_CHECK(cIPCMemClient)
	CHECK_FIELD(cIPCMemClient, m_hMap, 0x0);
END_LAYOUT_CHECK

#endif
