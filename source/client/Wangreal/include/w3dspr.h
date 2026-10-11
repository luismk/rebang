// Original: d:\build\custom\temp\code\client\wangreal\include\w3dspr.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_W3DSPR_H
#define PG_CLIENT_WANGREAL_INCLUDE_W3DSPR_H

#include "..\..\defines.h"
#include "wmath.h"
#include "wresource.h"
#include "wvideo.h"

class WView;

// A textured, camera-facing quad in 3D (PDB layout, size 0x104).
class W3dSpr : public WResource {
	LAYOUT_CHECK_FRIEND;
public:
	enum w_spr_align {
		CAMERA_XY_ALIGN = 0,
		CAMERA_X_ALIGN = 1,
		NO_ALIGN = 2
	};

	W3dSpr(void);
	virtual ~W3dSpr(void);

	int LoadSprite(const char* filename, int type);
	int AttachTexture(int texHandle);
	void SetPos(const WVector& vec);
	void SetRect(float width, float height, float pivot_x, float pivot_y);
	void SetColor(const int color);
	void Render(WView* view, float angle, int type);
	void Render(WView* view, int type, w_spr_align align);

private:
	void Rotate(float x, float y, float& tx, float& ty, float angle);

	int m_handle;          // 0x2c
	float w1;              // 0x30
	float w2;              // 0x34
	float h1;              // 0x38
	float h2;              // 0x3c
	WVector pos;           // 0x40
	WTVertex vtx[4];       // 0x4c
	WTVertex* vl[5];       // 0xec
	bool m_texLoaded;      // 0x100
};
CHECK_SIZE(W3dSpr, 0x104);
BEGIN_LAYOUT_CHECK(W3dSpr)
	CHECK_FIELD(W3dSpr, m_handle, 0x2c);
	CHECK_FIELD(W3dSpr, w1, 0x30);
	CHECK_FIELD(W3dSpr, w2, 0x34);
	CHECK_FIELD(W3dSpr, h1, 0x38);
	CHECK_FIELD(W3dSpr, h2, 0x3c);
	CHECK_FIELD(W3dSpr, pos, 0x40);
	CHECK_FIELD(W3dSpr, vtx, 0x4c);
	CHECK_FIELD(W3dSpr, vl, 0xec);
	CHECK_FIELD(W3dSpr, m_texLoaded, 0x100);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_W3DSPR_H
