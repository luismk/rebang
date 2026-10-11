// Original: d:\build\custom\temp\code\client\wangreal\include\wscene.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WSCENE_H
#define PG_CLIENT_WANGREAL_INCLUDE_WSCENE_H

#include <string.h>

#include "..\..\defines.h"
#include "wmath.h"

// One light of a scene (PDB layout, size 0x1c).
struct LightSet {
	enum {
		W_LIGHT_NOT_INITIALIZED = 0,
		W_LIGHT_POINT = 1,
		W_LIGHT_DIRECTIONAL = 2
	};

	LightSet() : type(W_LIGHT_NOT_INITIALIZED), diffuse(0), ambient(0), ambient2(0xffffff) {   // wscene.h:21
		nearone.Reset();
	}

	void operator=(const LightSet& l) {   // wscene.h:26
		memcpy(this, &l, sizeof(LightSet));
	}

	unsigned long type;         // 0x0
	WVector nearone;            // 0x4
	unsigned long diffuse;      // 0x10
	unsigned long ambient;      // 0x14
	unsigned long ambient2;     // 0x18
};
CHECK_SIZE(LightSet, 0x1c);
CHECK_FIELD(LightSet, type, 0x0);
CHECK_FIELD(LightSet, nearone, 0x4);
CHECK_FIELD(LightSet, diffuse, 0x10);
CHECK_FIELD(LightSet, ambient, 0x14);
CHECK_FIELD(LightSet, ambient2, 0x18);

class WScene {
public:
	virtual ~WScene() {}
	virtual LightSet* GetLightSet(const WVector&) = 0;                          // slot 1
	virtual float CollTest(const WVector&, const WVector&, WPlane*, float) = 0;  // slot 2
};
CHECK_SIZE(WScene, 0x4);

#endif // PG_CLIENT_WANGREAL_INCLUDE_WSCENE_H
