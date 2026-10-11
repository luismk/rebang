// Original: d:\build\custom\temp\code\client\projectg\frimagenoticedlg.h
#ifndef PG_CLIENT_PROJECTG_FRIMAGENOTICEDLG_H
#define PG_CLIENT_PROJECTG_FRIMAGENOTICEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;
class FrArea;

// Notice dialog built from a 3x3 grid of image areas (left/center/right by
// top/middle/bottom), with an optional OK and close button; its size
// (0x140) is the `new` of FrImageNoticeDlgMakeInstance.
class FrImageNoticeDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frimagenoticedlg.h:16

	FrImageNoticeDlg(void);
	virtual ~FrImageNoticeDlg(void);

	bool SetOkButton(WRect& rect, bool bRelative);
	bool SetCloseButton(WRect& rect, bool bRelative);
	void SetDlgSize(WRect& rect);
	bool SetBackGroundImage(int x, int y, WRect& rect, const char* name, bool bRelative);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitCloseButton(intptr_t param);
	void OnInitOkButton(intptr_t param);
	void OnLBDownOkButton(void);
	void OnInitLeftTop(intptr_t param);
	void OnInitCenterTop(intptr_t param);
	void OnInitRightTop(intptr_t param);
	void OnInitLeftMiddle(intptr_t param);
	void OnInitCenterMiddle(intptr_t param);
	void OnInitRightMiddle(intptr_t param);
	void OnInitLeftBottom(intptr_t param);
	void OnInitCenterBottom(intptr_t param);
	void OnInitRightBottom(intptr_t param);

private:
	FrButton* m_pCloseButton;          // 0x110 ("cancel")
	FrButton* m_pOkButton;             // 0x114 ("ok")
	FrArea* m_pArea[3][3];             // 0x118 ([y][x], "lefttop" .. "rightbottom")
	UNKNOWN(0x13c, 0x140);
};
BEGIN_LAYOUT_CHECK(FrImageNoticeDlg)
	CHECK_FIELD(FrImageNoticeDlg, m_pCloseButton, 0x110);
	CHECK_FIELD(FrImageNoticeDlg, m_pOkButton, 0x114);
	CHECK_FIELD(FrImageNoticeDlg, m_pArea, 0x118);
END_LAYOUT_CHECK
CHECK_SIZE(FrImageNoticeDlg, 0x140);

#endif // PG_CLIENT_PROJECTG_FRIMAGENOTICEDLG_H
