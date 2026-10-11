// Original: d:\build\custom\temp\code\client\projectg\worldtoureventdlg.h
#ifndef PG_CLIENT_PROJECTG_WORLDTOUREVENTDLG_H
#define PG_CLIENT_PROJECTG_WORLDTOUREVENTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "wtypes.h"
#include "../shared/globalgamedefine.h"

class Bitmap;
class FrArea;
class FrButton;
class FrViewer;

// The world-tour event window: one image pair, rectangle, stamp rectangle
// and course bit per course slot (CMapTypeAdapter::eMapIndex, 17 slots).
// Its size (0x438) is the `new` of FrWorldTourEventDlgMakeInstance.
class FrWorldTourEventDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	// The two images of a course slot (8 bytes).
	struct sMapImage {
		const Bitmap* m_pImage;       // 0x0 (drawn when the course is played)
		const Bitmap* m_pImageOver;   // 0x4
	};

	enum { MAP_SLOT_COUNT = 17 };

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // worldtoureventdlg.h:15

	FrWorldTourEventDlg(void);
	virtual ~FrWorldTourEventDlg(void);

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	void ClearVariables(void);
	void SetMapFlag(unsigned long mapFlag);
	void DrawMapImage(void);
	bool OnDescDlgResult(int result, FrForm* pForm);
	bool OnGiftDlgResult(int result, FrForm* pForm);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void SetMapFlagVector(eMapType type);
	void SetMapImageRect(eMapType type, WRect& rect);
	void SetMapImage(eMapType type, const char* image, const char* imageOver);

	void OnMapIconOwnerDraw(void);
	void OnInitGiftImageLeftTop(intptr_t param);
	void OnInitGiftImageRightTop(intptr_t param);
	void OnInitGiftImageLeftBottom(intptr_t param);
	void OnInitGiftImageRightBottom(intptr_t param);
	void OnInitDescButton(intptr_t param);
	void OnLBDownDescButton(void);
	void OnInitCourseButton(intptr_t param);
	void OnLBDownCourseButton(void);
	void OnInitGiftButtonCover(intptr_t param);
	void OnInitGiftButton(intptr_t param);
	void OnLBDownGiftButton(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

	float m_time;                                          // 0x110 (OnProc)
	bool m_bInit;                                          // 0x114 (ClearVariables)
	unsigned long m_mapFlag;                               // 0x118 (SetMapFlag)
	CMapTypeAdapter::eMapFlag m_mapFlagVector[MAP_SLOT_COUNT];   // 0x11c
	WRect m_mapImageRect[MAP_SLOT_COUNT];                  // 0x160
	sMapImage m_mapImage[MAP_SLOT_COUNT];                  // 0x270
	WRect m_stampRect[MAP_SLOT_COUNT];                     // 0x2f8
	const Bitmap* m_pStampImage;                           // 0x408
	FrArea* m_pGiftButtonCover;                            // 0x40c
	FrButton* m_pDescButton;                               // 0x410
	FrButton* m_pCourseButton;                             // 0x414
	FrButton* m_pGiftButton;                               // 0x418
	FrArea* m_pGiftImageLeftTop;                           // 0x41c
	FrArea* m_pGiftImageRightTop;                          // 0x420
	FrArea* m_pGiftImageLeftBottom;                        // 0x424
	FrArea* m_pGiftImageRightBottom;                       // 0x428
	FrForm* m_pGiftDlg;                                    // 0x42c
	FrForm* m_pDescDlg;                                    // 0x430
	PADDING(0x434, 0x438);                                 // FrWorldTourEventDlg members
};

CHECK_SIZE(FrWorldTourEventDlg, 0x438);
BEGIN_LAYOUT_CHECK(FrWorldTourEventDlg)
	CHECK_FIELD(FrWorldTourEventDlg, m_time, 0x110);
	CHECK_FIELD(FrWorldTourEventDlg, m_bInit, 0x114);
	CHECK_FIELD(FrWorldTourEventDlg, m_mapFlag, 0x118);
	CHECK_FIELD(FrWorldTourEventDlg, m_mapFlagVector, 0x11c);
	CHECK_FIELD(FrWorldTourEventDlg, m_mapImageRect, 0x160);
	CHECK_FIELD(FrWorldTourEventDlg, m_mapImage, 0x270);
	CHECK_FIELD(FrWorldTourEventDlg, m_stampRect, 0x2f8);
	CHECK_FIELD(FrWorldTourEventDlg, m_pStampImage, 0x408);
	CHECK_FIELD(FrWorldTourEventDlg, m_pGiftButtonCover, 0x40c);
	CHECK_FIELD(FrWorldTourEventDlg, m_pDescButton, 0x410);
	CHECK_FIELD(FrWorldTourEventDlg, m_pCourseButton, 0x414);
	CHECK_FIELD(FrWorldTourEventDlg, m_pGiftButton, 0x418);
	CHECK_FIELD(FrWorldTourEventDlg, m_pGiftImageLeftTop, 0x41c);
	CHECK_FIELD(FrWorldTourEventDlg, m_pGiftImageRightBottom, 0x428);
	CHECK_FIELD(FrWorldTourEventDlg, m_pGiftDlg, 0x42c);
	CHECK_FIELD(FrWorldTourEventDlg, m_pDescDlg, 0x430);
END_LAYOUT_CHECK
CHECK_SIZE(FrWorldTourEventDlg::sMapImage, 0x8);

// The world-tour event description window: one image viewer. 0x118 bytes.
class FrWorldTourEventDescDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // worldtoureventdlg.h:101

	FrWorldTourEventDescDlg(void);
	virtual ~FrWorldTourEventDescDlg(void);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitDescView(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

	FrViewer* m_pDescView;    // 0x110 (OnInitDescView)
	PADDING(0x114, 0x118);    // FrWorldTourEventDescDlg members
};

CHECK_SIZE(FrWorldTourEventDescDlg, 0x118);
BEGIN_LAYOUT_CHECK(FrWorldTourEventDescDlg)
	CHECK_FIELD(FrWorldTourEventDescDlg, m_pDescView, 0x110);
END_LAYOUT_CHECK

// The world-tour event gift confirmation window: yes and no buttons.
// 0x118 bytes.
class FrWorldTourEventGiftDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // worldtoureventdlg.h:120

	FrWorldTourEventGiftDlg(void);
	virtual ~FrWorldTourEventGiftDlg(void);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitYesButton(intptr_t param);
	void OnLBDownYesButton(void);
	void OnInitNoButton(intptr_t param);
	void OnLBDownNoButton(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

	FrButton* m_pYesButton;   // 0x110
	FrButton* m_pNoButton;    // 0x114
};

CHECK_SIZE(FrWorldTourEventGiftDlg, 0x118);
BEGIN_LAYOUT_CHECK(FrWorldTourEventGiftDlg)
	CHECK_FIELD(FrWorldTourEventGiftDlg, m_pYesButton, 0x110);
	CHECK_FIELD(FrWorldTourEventGiftDlg, m_pNoButton, 0x114);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_WORLDTOUREVENTDLG_H
