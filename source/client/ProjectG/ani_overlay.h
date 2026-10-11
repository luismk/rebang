// Original: d:\build\custom\temp\code\client\projectg\ani_overlay.h
#ifndef PG_CLIENT_PROJECTG_ANI_OVERLAY_H
#define PG_CLIENT_PROJECTG_ANI_OVERLAY_H

#include "..\..\defines.h"
#include "../client/Wangreal/include/wtypes.h"

class WOverlay;

// An overlay cut into equal frames (columns x rows) drawn one at a time
// (ani_overlay.cpp). Size 0x38 (the operator new of CEffectOverlay's ctor).
class CAniOverlay {
	LAYOUT_CHECK_FRIEND;
public:
	CAniOverlay(const char* name, float frameWidth, float frameHeight);
	~CAniOverlay(void);

	void Render(float x, float y, int frame, int flags, int color, float scale);

private:
	WOverlay* m_pOverlay;   // 0x0
	float m_frameWidth;     // 0x4
	float m_frameHeight;    // 0x8
	int m_frameCount;       // 0xc: m_columns * m_rows
	WRect m_dstRect;        // 0x10
	WRect m_srcRect;        // 0x20
	int m_columns;          // 0x30
	int m_rows;             // 0x34
};

BEGIN_LAYOUT_CHECK(CAniOverlay)
	CHECK_FIELD(CAniOverlay, m_pOverlay, 0x0);
	CHECK_FIELD(CAniOverlay, m_frameWidth, 0x4);
	CHECK_FIELD(CAniOverlay, m_frameHeight, 0x8);
	CHECK_FIELD(CAniOverlay, m_frameCount, 0xc);
	CHECK_FIELD(CAniOverlay, m_dstRect, 0x10);
	CHECK_FIELD(CAniOverlay, m_srcRect, 0x20);
	CHECK_FIELD(CAniOverlay, m_columns, 0x30);
	CHECK_FIELD(CAniOverlay, m_rows, 0x34);
END_LAYOUT_CHECK
CHECK_SIZE(CAniOverlay, 0x38);

// The bar impact effect: an 8-frame CAniOverlay animation shown over the
// shot bar once SetActive starts it (ani_overlay.cpp).
class CEffectOverlay {
	LAYOUT_CHECK_FRIEND;
public:
	CEffectOverlay(void);
	~CEffectOverlay(void);

	void Process(float elapsed);
	void SetActive(void);
	void Display(void);

private:
	CAniOverlay* m_pAniOverlay;   // 0x0
	int m_frame;                  // 0x4
	float m_frameTime;            // 0x8: seconds per frame
	bool m_active;                // 0xc
};

BEGIN_LAYOUT_CHECK(CEffectOverlay)
	CHECK_FIELD(CEffectOverlay, m_pAniOverlay, 0x0);
	CHECK_FIELD(CEffectOverlay, m_frame, 0x4);
	CHECK_FIELD(CEffectOverlay, m_frameTime, 0x8);
	CHECK_FIELD(CEffectOverlay, m_active, 0xc);
END_LAYOUT_CHECK
CHECK_SIZE(CEffectOverlay, 0x10);

#endif // PG_CLIENT_PROJECTG_ANI_OVERLAY_H
