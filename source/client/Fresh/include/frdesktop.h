// Original: d:\build\custom\temp\code\client\fresh\include\frdesktop.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRDESKTOP_H
#define PG_CLIENT_FRESH_INCLUDE_FRDESKTOP_H

#include <string>

#include "..\..\defines.h"
#include "frwnd.h"
#include "../client/Wangreal/include/wtypes.h"

class Bitmap;
class CBackGround;
class FrGuiItem;
class WOverlay;

// The root window of a FrWndManager: wallpaper and scrolling animated
// background. Layout, access and methods from the PDB (size 0x124, base
// FrWnd, no new virtual).
class FrDesktop : public FrWnd {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frdesktop.h:18

	FrDesktop(void);
	virtual ~FrDesktop(void);

	void Init(float width, float height);
	void SetWallPaper(const char* pszFilename, bool bShow);
	void SetWallPaperFromBitmap(const Bitmap* bitmap, bool bShow);
	void LockWallPaper(bool bLock) { m_bLocked = bLock; }           // frdesktop.h:27
	void SetBgColor(unsigned long color) { m_bgColor = color; }      // frdesktop.h:28
	void Show(bool bShow) { m_bShow = bShow; }                       // frdesktop.h:29
	bool IsWallPaperVisible(void);
	void ResetRect(void);
	void SetScroll(float du, float dv);
	void SetAniBg(FrGuiItem& aniBg);

protected:
	virtual void OnProc(const float deltaTime);
	virtual void OnDraw(void);

	CBackGround* m_pBackGround;           // 0xd4
	bool m_bShow;                         // 0xd8
	unsigned long m_bgColor;              // 0xdc
	std::string m_bgFilename;             // 0xe0
	bool m_bLocked;                       // 0xfc
	WOverlay* m_pAniBg;                   // 0x100
	WRect m_aniBgDest;                    // 0x104
	int m_velX;                           // 0x114
	int m_velY;                           // 0x118
	float m_aniBgSrcX;                    // 0x11c
	float m_aniBgSrcY;                    // 0x120
};

CHECK_SIZE(FrDesktop, 0x124);
BEGIN_LAYOUT_CHECK(FrDesktop)
	CHECK_FIELD(FrDesktop, m_pBackGround, 0xd4);
	CHECK_FIELD(FrDesktop, m_bShow, 0xd8);
	CHECK_FIELD(FrDesktop, m_bgColor, 0xdc);
	CHECK_FIELD(FrDesktop, m_bgFilename, 0xe0);
	CHECK_FIELD(FrDesktop, m_bLocked, 0xfc);
	CHECK_FIELD(FrDesktop, m_pAniBg, 0x100);
	CHECK_FIELD(FrDesktop, m_aniBgDest, 0x104);
	CHECK_FIELD(FrDesktop, m_velX, 0x114);
	CHECK_FIELD(FrDesktop, m_velY, 0x118);
	CHECK_FIELD(FrDesktop, m_aniBgSrcX, 0x11c);
	CHECK_FIELD(FrDesktop, m_aniBgSrcY, 0x120);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRDESKTOP_H
