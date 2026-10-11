// Original: d:\build\custom\temp\code\client\projectg\codetinker.cpp
#include "CodeTinker.h"

// One entry of the hole drop effect table (0x84 bytes, 20 entries at 0xa956d8).
struct sHoleDropEffect {
	DWORD id;           // 0x0
	char name[0x80];    // 0x4
};
CHECK_SIZE(sHoleDropEffect, 0x84);
CHECK_FIELD(sHoleDropEffect, id, 0x0);
CHECK_FIELD(sHoleDropEffect, name, 0x4);

extern "C" sHoleDropEffect data_a956d8[20];

// codetinker.cpp:8
CCodeTinker::CCodeTinker() {
}

// codetinker.cpp:12
CCodeTinker::~CCodeTinker() {
}

// codetinker.cpp:17
const char* CCodeTinker::GetHoleDropEffectName(DWORD effectId) {
	const char* name = NULL;
	for (int i = 0; i < 20; ++i) {
		if (effectId == data_a956d8[i].id) {
			name = data_a956d8[i].name;
			break;
		}
	}
	return name;
}

// codetinker.cpp:62
bool CCodeTinker::IsNeedOpenMiniButton(DWORD itemTypeId) {
	switch (itemTypeId) {
	case 0x1a00003b:
	case 0x1a000048:
	case 0x1a000052:
	case 0x1a0000bc:
		return true;
	}
	return false;
}
