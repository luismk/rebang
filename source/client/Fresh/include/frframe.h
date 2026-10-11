// Original: d:\build\custom\temp\code\client\fresh\include\frframe.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRFRAME_H
#define PG_CLIENT_FRESH_INCLUDE_FRFRAME_H

#include <string>

#include "..\..\defines.h"
#include "frwnd.h"

class Bitmap;
class FrElementFrame;
class FrGuiItem;
class FrWndManager;

// A framed window drawn from nine-slice bitmaps, with a caption and a
// description. Layout, access and methods from the PDB (size 0x19c, base
// FrWnd, no new virtual). The inline bodies carry frframe.h line marks.
class FrFrame : public FrWnd {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frframe.h:20

	FrFrame(void);
	virtual ~FrFrame(void);

	void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);
	void SetCaption(const char* caption) { m_caption = caption; }  // frframe.h:27
	void SetDesc(const char* desc) { m_desc = desc; }              // frframe.h:28
	void UseEmoAtDesc(bool b) { m_emoAtDesc = b; }                 // frframe.h:29
	void SetCaptionFocus(bool b) { m_bCaptionFocus = b; }          // frframe.h:30
	void SetBlink(bool b);
	void SetCaptionOffset(bool b) { m_captionOffset = b; }         // frframe.h:32

protected:
	void Release(void);
	void UpdateRectInfo(int index, const Bitmap** ppBmp);
	void DrawFrame(int index, float x, float y, const Bitmap** ppBmp);
	virtual void OnDraw(void);
	virtual void OnProc(const float dt);

	FrGuiItem* m_pItem;                 // 0xd4
	FrElementFrame* m_pFrame;           // 0xd8
	WSize m_a;                          // 0xdc
	WSize m_b;                          // 0xe4
	WSize m_c;                          // 0xec
	WSize m_min;                        // 0xf4
	float m_width;                      // 0xfc
	float m_height;                     // 0x100
	const Bitmap* m_pBaseBmp[9];        // 0x104
	const Bitmap* m_pSubBmp[9];         // 0x128
	const Bitmap* m_pBlinkBmp[3];       // 0x14c
	std::string m_caption;              // 0x158
	std::string m_desc;                 // 0x174
	bool m_emoAtDesc;                   // 0x190
	bool m_bCaptionFocus;               // 0x191
	bool m_bBlink;                      // 0x192
	bool m_bBlinkShow;                  // 0x193
	bool m_captionOffset;               // 0x194
	float m_fBlinkTime;                 // 0x198
};

CHECK_SIZE(FrFrame, 0x19c);
BEGIN_LAYOUT_CHECK(FrFrame)
	CHECK_FIELD(FrFrame, m_pItem, 0xd4);
	CHECK_FIELD(FrFrame, m_pFrame, 0xd8);
	CHECK_FIELD(FrFrame, m_a, 0xdc);
	CHECK_FIELD(FrFrame, m_b, 0xe4);
	CHECK_FIELD(FrFrame, m_c, 0xec);
	CHECK_FIELD(FrFrame, m_min, 0xf4);
	CHECK_FIELD(FrFrame, m_width, 0xfc);
	CHECK_FIELD(FrFrame, m_height, 0x100);
	CHECK_FIELD(FrFrame, m_pBaseBmp, 0x104);
	CHECK_FIELD(FrFrame, m_pSubBmp, 0x128);
	CHECK_FIELD(FrFrame, m_pBlinkBmp, 0x14c);
	CHECK_FIELD(FrFrame, m_caption, 0x158);
	CHECK_FIELD(FrFrame, m_desc, 0x174);
	CHECK_FIELD(FrFrame, m_emoAtDesc, 0x190);
	CHECK_FIELD(FrFrame, m_bCaptionFocus, 0x191);
	CHECK_FIELD(FrFrame, m_bBlink, 0x192);
	CHECK_FIELD(FrFrame, m_bBlinkShow, 0x193);
	CHECK_FIELD(FrFrame, m_captionOffset, 0x194);
	CHECK_FIELD(FrFrame, m_fBlinkTime, 0x198);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRFRAME_H
