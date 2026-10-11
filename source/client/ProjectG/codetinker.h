// Original: d:\build\custom\temp\code\client\projectg\codetinker.h
#ifndef PG_CLIENT_PROJECTG_CODETINKER_H
#define PG_CLIENT_PROJECTG_CODETINKER_H

#include <windows.h>

#include "..\..\defines.h"

class CCodeTinker {
public:
	CCodeTinker();
	virtual ~CCodeTinker();     // vtable slot 0 (??_E / ??_G)

	const char* GetHoleDropEffectName(DWORD effectId);
	bool IsNeedOpenMiniButton(DWORD itemTypeId);
};
CHECK_SIZE(CCodeTinker, 0x4);

#endif
