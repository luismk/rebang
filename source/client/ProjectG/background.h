// Original: d:\build\custom\temp\code\client\projectg\background.h
#ifndef PG_CLIENT_PROJECTG_BACKGROUND_H
#define PG_CLIENT_PROJECTG_BACKGROUND_H

#include "..\..\defines.h"
#include "../client/Wangreal/include/wtypes.h"

class Bitmap;
class WSplash;

// A full-screen scrolling wallpaper (PDB layout, size 0x18). The two Draw
// overloads take vtable slots 3 (0xc, the rectangle form) and 4 (0x10).
class CBackGround {
	LAYOUT_CHECK_FRIEND;
public:
	CBackGround(const char* filename, bool bScale);
	CBackGround(bool bScale, const Bitmap* bitmap);
	virtual ~CBackGround(void);

	virtual void Load(const char* filename, bool bScale);
	virtual void Process(const float deltaTime);
	virtual void Draw(unsigned long color);
	virtual void Draw(const WPoint* pos, const WRect* src, unsigned long color);
	virtual void ResetScreenSize(void);
	virtual int GetWidth(void);
	virtual int GetHeight(void);

	void SetDUV(float du, float dv) { m_du = du; m_dv = dv; }   // background.h:29
	bool IsLoaded(void);

protected:
	WSplash* m_pSplash;                   // 0x4
	float m_du;                           // 0x8
	float m_dv;                           // 0xc
	float m_u;                            // 0x10
	float m_v;                            // 0x14
};

CHECK_SIZE(CBackGround, 0x18);
BEGIN_LAYOUT_CHECK(CBackGround)
	CHECK_FIELD(CBackGround, m_pSplash, 0x4);
	CHECK_FIELD(CBackGround, m_du, 0x8);
	CHECK_FIELD(CBackGround, m_dv, 0xc);
	CHECK_FIELD(CBackGround, m_u, 0x10);
	CHECK_FIELD(CBackGround, m_v, 0x14);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_BACKGROUND_H
