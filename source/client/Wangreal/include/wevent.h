// Original: d:\build\custom\temp\code\client\wangreal\include\wevent.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WEVENT_H
#define PG_CLIENT_WANGREAL_INCLUDE_WEVENT_H

#include <windows.h>

#include "..\..\defines.h"

// A Win32 event handle, closed by the destructor. Only the inline
// constructor and destructor are transcribed so far.
class WSyncEvent {
	LAYOUT_CHECK_FRIEND;
public:
	WSyncEvent() { m_handle = NULL; m_state = 0; }
	~WSyncEvent() { if (m_handle != NULL) CloseHandle(m_handle); }

protected:
	HANDLE m_handle;    // 0x0
	int m_state;        // 0x4, purpose unknown (0 after construction)
};
CHECK_SIZE(WSyncEvent, 0x8);
BEGIN_LAYOUT_CHECK(WSyncEvent)
	CHECK_FIELD(WSyncEvent, m_handle, 0x0);
	CHECK_FIELD(WSyncEvent, m_state, 0x4);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WEVENT_H
