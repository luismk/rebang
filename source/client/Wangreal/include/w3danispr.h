// Original: d:\build\custom\temp\code\client\wangreal\include\w3danispr.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_W3DANISPR_H
#define PG_CLIENT_WANGREAL_INCLUDE_W3DANISPR_H

#include "..\..\defines.h"
#include "wlist.h"
#include "wmath.h"
#include "wresource.h"
#include "wvideo.h"

class WView;

// One frame of an animated sprite: a texture and its UV rectangle.
struct _WSPRITE {
	int hTexture;       // 0x0
	float fStartU;      // 0x4
	float fStartV;      // 0x8
	float fEndU;        // 0xc
	float fEndV;        // 0x10
};
CHECK_SIZE(_WSPRITE, 0x14);
CHECK_FIELD(_WSPRITE, hTexture, 0x0);
CHECK_FIELD(_WSPRITE, fStartU, 0x4);
CHECK_FIELD(_WSPRITE, fStartV, 0x8);
CHECK_FIELD(_WSPRITE, fEndU, 0xc);
CHECK_FIELD(_WSPRITE, fEndV, 0x10);

// A camera-facing animated sprite in 3D (PDB layout, size 0x140).
class W3dAniSpr : public WResource {
	LAYOUT_CHECK_FRIEND;
public:
	enum w_spr_align {
		CAMERA_XY_ALIGN = 0,
		CAMERA_X_ALIGN = 1,
		X_ALIGN = 2,
		Y_ALIGN = 3,
		Z_ALIGN = 4,
		NO_ALIGN = 5
	};

	W3dAniSpr(void);
	virtual ~W3dAniSpr(void);

	int LoadSprite(const char* filename, int count);
	int LoadSpritesInOneTexture(const char* filename, int count, float w, float h);
	int LoadTexture(int texhandle, float w, float h);
	void SetPos(const WVector& vec);
	void SetRect(float width, float height, float pivot_x, float pivot_y);
	void SetColor(const int color);
	void Render(WView* view, const WMatrix& rot, int type, int nSprNum);
	void Render(WView* view, const WVector& angle, int type, int nSprNum);
	void Render(WView* view, float angle, int type, int nSprNum);
	void Render(WView* view, int type, w_spr_align align, int nSprNum);
	int GetSpriteNum(void) const { return m_nTotalSprite; }   // w3danispr.h:55

private:
	void Rotate(float x, float y, float& tx, float& ty, float angle);
	void AddSprite(_WSPRITE* spr);
	void DelSprite(_WSPRITE* spr);
	void AllDelSprite(void);
	_WSPRITE* FindSprite(int index);

	WList<int> m_TextureList;           // 0x2c
	WList<_WSPRITE*> m_SpriteList;      // 0x4c
	int m_nTotalSprite;                 // 0x6c
	float w1;                           // 0x70
	float w2;                           // 0x74
	float h1;                           // 0x78
	float h2;                           // 0x7c
	WVector pos;                        // 0x80
	WTVertex vtx[4];                    // 0x8c
	WTVertex* vl[5];                    // 0x12c
};
CHECK_SIZE(W3dAniSpr, 0x140);
BEGIN_LAYOUT_CHECK(W3dAniSpr)
	CHECK_FIELD(W3dAniSpr, m_TextureList, 0x2c);
	CHECK_FIELD(W3dAniSpr, m_SpriteList, 0x4c);
	CHECK_FIELD(W3dAniSpr, m_nTotalSprite, 0x6c);
	CHECK_FIELD(W3dAniSpr, w1, 0x70);
	CHECK_FIELD(W3dAniSpr, w2, 0x74);
	CHECK_FIELD(W3dAniSpr, h1, 0x78);
	CHECK_FIELD(W3dAniSpr, h2, 0x7c);
	CHECK_FIELD(W3dAniSpr, pos, 0x80);
	CHECK_FIELD(W3dAniSpr, vtx, 0x8c);
	CHECK_FIELD(W3dAniSpr, vl, 0x12c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_W3DANISPR_H
