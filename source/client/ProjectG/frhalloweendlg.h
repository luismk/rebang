// Original: d:\build\custom\temp\code\client\projectg\frhalloweendlg.h
#ifndef PG_CLIENT_PROJECTG_FRHALLOWEENDLG_H
#define PG_CLIENT_PROJECTG_FRHALLOWEENDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "wtypes.h"
#include "../shared/globalgamedefine.h"

class Bitmap;
class FrArea;
class FrButton;
class FrViewer;

// The Halloween event gift window: two frames shown or hidden by the gift
// count. 0x118 bytes.
class FrHalloweenEventGiftDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frhalloweendlg.h:9

	FrHalloweenEventGiftDlg(void);
	virtual ~FrHalloweenEventGiftDlg(void);

	virtual bool OnInit(void);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitFrame01(intptr_t param);
	void OnInitFrameCover(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

	FrArea* m_pFrame01;       // 0x110
	FrArea* m_pFrameCover;    // 0x114
};

CHECK_SIZE(FrHalloweenEventGiftDlg, 0x118);
BEGIN_LAYOUT_CHECK(FrHalloweenEventGiftDlg)
	CHECK_FIELD(FrHalloweenEventGiftDlg, m_pFrame01, 0x110);
	CHECK_FIELD(FrHalloweenEventGiftDlg, m_pFrameCover, 0x114);
END_LAYOUT_CHECK

// The Halloween event description window: one viewer sized to its image.
// 0x118 bytes.
class FrHalloweenEventDescDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frhalloweendlg.h:32

	FrHalloweenEventDescDlg(void);
	virtual ~FrHalloweenEventDescDlg(void);

	virtual bool OnInit(void);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitDescView(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

	bool m_bScroll;           // 0x110
	FrViewer* m_pDescView;    // 0x114
};

CHECK_SIZE(FrHalloweenEventDescDlg, 0x118);
BEGIN_LAYOUT_CHECK(FrHalloweenEventDescDlg)
	CHECK_FIELD(FrHalloweenEventDescDlg, m_bScroll, 0x110);
	CHECK_FIELD(FrHalloweenEventDescDlg, m_pDescView, 0x114);
END_LAYOUT_CHECK

// The Halloween event window: one image pair, rectangle and course bit per
// course slot (CMapTypeAdapter::eMapIndex, 19 slots). 0x4A8 bytes.
class FrHalloweenEventDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	// The two images of a course slot (8 bytes).
	struct sMapImage {
		const Bitmap* m_pImage;       // 0x0
		const Bitmap* m_pImageOver;   // 0x4
	};

	enum { MAP_SLOT_COUNT = 19 };

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frhalloweendlg.h:54

	FrHalloweenEventDlg(void);
	virtual ~FrHalloweenEventDlg(void);

	virtual bool OnInit(void);

	bool OnDescDlgResult(int result, FrForm* pForm);
	void SetMapImage(eMapType type, const char* image, const char* imageOver);
	void SetMapImageRect(eMapType type, WRect& rect);
	void SetMapFlagVector(eMapType type);
	void DrawMapImage(void);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitDescButton(intptr_t param);
	void OnLBDownDescButton(void);
	void OnMapIconOwnerDraw(void);
	void OnCurrentItemOwnerDraw(int param);

private:
	static sFRESH_ENTRY _MsgEntries[];

	FrButton* m_pDescButton;                  // 0x110
	PADDING(0x114, 0x118);                    // FrHalloweenEventDlg members
	const Bitmap* m_pCurrentItemImage;        // 0x118
	FrForm* m_pDescDlg;                       // 0x11c
	const Bitmap* m_pMarkImage;               // 0x120 ("hw_mark", OnInit)
	WRect m_mapImageRect[MAP_SLOT_COUNT];     // 0x124
	sMapImage m_mapImage[MAP_SLOT_COUNT];     // 0x254
	WRect m_mapSubRect[MAP_SLOT_COUNT];       // 0x2ec (second rectangle of a course slot, OnInit)
	CMapTypeAdapter::eMapFlag m_mapFlag[MAP_SLOT_COUNT];   // 0x41c
	int m_currentMapIndex;                    // 0x468 (zeroed by the constructor; use not seen yet)
	int m_currentItemCount[4];                // 0x46c
	WPoint m_currentItemPos[4];               // 0x47c
	WPoint m_itemFramePos;                    // 0x49c (set by OnInit; reader not decompiled yet)
	PADDING(0x4a4, 0x4a8);                    // FrHalloweenEventDlg members
};

CHECK_SIZE(FrHalloweenEventDlg, 0x4a8);
BEGIN_LAYOUT_CHECK(FrHalloweenEventDlg)
	CHECK_FIELD(FrHalloweenEventDlg, m_pDescButton, 0x110);
	CHECK_FIELD(FrHalloweenEventDlg, m_pCurrentItemImage, 0x118);
	CHECK_FIELD(FrHalloweenEventDlg, m_pDescDlg, 0x11c);
	CHECK_FIELD(FrHalloweenEventDlg, m_pMarkImage, 0x120);
	CHECK_FIELD(FrHalloweenEventDlg, m_mapImageRect, 0x124);
	CHECK_FIELD(FrHalloweenEventDlg, m_mapImage, 0x254);
	CHECK_FIELD(FrHalloweenEventDlg, m_mapSubRect, 0x2ec);
	CHECK_FIELD(FrHalloweenEventDlg, m_mapFlag, 0x41c);
	CHECK_FIELD(FrHalloweenEventDlg, m_currentItemCount, 0x46c);
	CHECK_FIELD(FrHalloweenEventDlg, m_currentItemPos, 0x47c);
	CHECK_FIELD(FrHalloweenEventDlg, m_itemFramePos, 0x49c);
END_LAYOUT_CHECK
CHECK_SIZE(FrHalloweenEventDlg::sMapImage, 0x8);

#endif // PG_CLIENT_PROJECTG_FRHALLOWEENDLG_H
