// Original: d:\build\custom\temp\code\client\projectg\threadload.h
#ifndef PG_CLIENT_PROJECTG_THREADLOAD_H
#define PG_CLIENT_PROJECTG_THREADLOAD_H

#include <windows.h>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"

class WOverlay;
class WPuppet;

// Kind of resource the loading thread fetches (sParam::type, the int
// parameter of CThreadLoad::Load).
enum {
	THREADLOAD_PUPPET = 0,
	THREADLOAD_TEXTURE = 1,
	THREADLOAD_OVERLAY = 2
};

// Request handed to the loading thread (g_loadParam).
struct sParam {
	char name[0x20];             // 0x00
	union {                      // 0x20
		WPuppet* pPuppet;
		int texture;
		WOverlay* pOverlay;
	} result;
	int type;                    // 0x24
	bool bLoaded;                // 0x28
	PADDING(0x29, 0x2c);         // 0x29
	unsigned long startTime;     // 0x2c
};
CHECK_SIZE(sParam, 0x30);
CHECK_FIELD(sParam, name, 0x00);
CHECK_FIELD(sParam, result, 0x20);
CHECK_FIELD(sParam, type, 0x24);
CHECK_FIELD(sParam, bLoaded, 0x28);
CHECK_FIELD(sParam, startTime, 0x2c);

extern sParam g_loadParam;

// Loads one puppet, texture or overlay on a worker thread.
class CThreadLoad : public WSingleton<CThreadLoad> {
	LAYOUT_CHECK_FRIEND;
public:
	CThreadLoad(void);
	virtual ~CThreadLoad(void);

	void StopLoading(void);
	void LoadPuppet(const char* name);
	void LoadTexture(const char* name);
	void LoadOverlay(const char* name);

private:
	void Load(const char* name, int type);

	HANDLE m_hThread;            // 0x28
};
CHECK_SIZE(CThreadLoad, 0x2c);
BEGIN_LAYOUT_CHECK(CThreadLoad)
	CHECK_FIELD(CThreadLoad, m_hThread, 0x28);
END_LAYOUT_CHECK

#endif
