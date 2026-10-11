// Original: d:\build\custom\temp\code\client\projectg\frquickmovedlg.h
#ifndef PG_CLIENT_PROJECTG_FRQUICKMOVEDLG_H
#define PG_CLIENT_PROJECTG_FRQUICKMOVEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;
class FrViewer;

// Quick-move dialog: pages through notice images (prev/next buttons) and
// moves to the place the shown page advertises (the shop). Its size (0x168)
// is the `new` of FrQuickMoveDlgMakeInstance.
class FrQuickMoveDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frquickmovedlg.h:13

	FrQuickMoveDlg(void);
	virtual ~FrQuickMoveDlg(void);

	void SetDescImage(const char* fileName);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitDescViewer(intptr_t param);
	void OnLBDownViewer(void);
	void OnInitBtnPrev(intptr_t param);
	void OnInitBtnNext(intptr_t param);
	void OnInitBtnMove(intptr_t param);
	void OnLBDownBtnPrev(void);
	void OnLBDownBtnNext(void);
	void OnLBDownBtnMove(void);

private:
	FrViewer* m_pDescViewer;           // 0x110 (OnInitDescViewer)
	char m_descImage[64];              // 0x114 (file shown by the viewer)
	unsigned int m_page;               // 0x154 (current page, 1 to 4)
	FrButton* m_pBtnPrev;              // 0x158 (OnInitBtnPrev)
	FrButton* m_pBtnNext;              // 0x15c (OnInitBtnNext)
	FrButton* m_pBtnMove;              // 0x160 (OnInitBtnMove)
	UNKNOWN(0x164, 0x168);
};
BEGIN_LAYOUT_CHECK(FrQuickMoveDlg)
	CHECK_FIELD(FrQuickMoveDlg, m_pDescViewer, 0x110);
	CHECK_FIELD(FrQuickMoveDlg, m_descImage, 0x114);
	CHECK_FIELD(FrQuickMoveDlg, m_page, 0x154);
	CHECK_FIELD(FrQuickMoveDlg, m_pBtnPrev, 0x158);
	CHECK_FIELD(FrQuickMoveDlg, m_pBtnNext, 0x15c);
	CHECK_FIELD(FrQuickMoveDlg, m_pBtnMove, 0x160);
END_LAYOUT_CHECK
CHECK_SIZE(FrQuickMoveDlg, 0x168);

#endif // PG_CLIENT_PROJECTG_FRQUICKMOVEDLG_H
