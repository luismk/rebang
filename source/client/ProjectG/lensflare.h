// Original: d:\build\custom\temp\code\client\projectg\lensflare.h
#ifndef PG_CLIENT_PROJECTG_LENSFLARE_H
#define PG_CLIENT_PROJECTG_LENSFLARE_H

#include "..\..\defines.h"
#include "wmath.h"

class WOverlay;
class W3dSpr;
class WView;

// The sun's lens flare: a glare and a sun sprite at the projected sun
// position, and a row of flare rings along the line through the screen
// centre (size 0x4C).
class CLensFlare {
	LAYOUT_CHECK_FRIEND;
public:
	CLensFlare(void);
	~CLensFlare(void);

	void Init(const WVector& sunDir);
	void SetSunDirection(const WVector& sunDir);
	void Process(float elapsed);
	void Render(WView* view);

protected:
	// One flare ring: the overlay it draws, its place on the line from the
	// sun through the screen centre (1 = the sun, -1 = mirrored), its size
	// and colour.
	struct w_flare_set {
		int overlay;            // 0x0
		float position;         // 0x4
		float scale;            // 0x8
		unsigned long color;    // 0xc
	};

	static w_flare_set ms_lenzTable[7];

	struct SpriteSize {
		float width;   // 0x0
		float height;  // 0x4
	};

	WVector m_sunDir;               // 0x0
	float m_angle;                  // 0xc: rotation of the sun rays, wraps at 120 pi
	float m_alpha;                  // 0x10: opacity of the flare sprites
	WOverlay* m_overlays[2];        // 0x14: flare rings
	W3dSpr* m_sprites[4];           // 0x1c: glare, sun, halo, rays
	SpriteSize m_spriteSize[4];     // 0x2c
};
CHECK_SIZE(CLensFlare, 0x4C);
BEGIN_LAYOUT_CHECK(CLensFlare)
	CHECK_FIELD(CLensFlare, m_sunDir, 0x0);
	CHECK_FIELD(CLensFlare, m_angle, 0xC);
	CHECK_FIELD(CLensFlare, m_alpha, 0x10);
	CHECK_FIELD(CLensFlare, m_overlays, 0x14);
	CHECK_FIELD(CLensFlare, m_sprites, 0x1C);
	CHECK_FIELD(CLensFlare, m_spriteSize, 0x2C);
	CHECK_SIZE(CLensFlare::w_flare_set, 0x10);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_LENSFLARE_H
