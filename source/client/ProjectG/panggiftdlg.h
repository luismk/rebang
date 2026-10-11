// Original: d:\build\custom\temp\code\client\projectg\panggiftdlg.h
#ifndef PG_CLIENT_PROJECTG_PANGGIFTDLG_H
#define PG_CLIENT_PROJECTG_PANGGIFTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrArea;
class FrEdit;

// The gift a pang gift dialog shows (original header not identified yet;
// only the members this unit uses are named).
struct sGiftInfo {
	UNKNOWN(0x0, 0x8);
	unsigned short m_pang;           // 0x8 (pang amount of the gift)
};
CHECK_FIELD(sGiftInfo, m_pang, 0x8);

// Pang gift dialog; its size (0x128) is the `new` of
// FrPangGiftDlgMakeInstance.
class FrPangGiftDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // panggiftdlg.h:12

	FrPangGiftDlg(void);

	sGiftInfo* GetItemInfo(void);    // panggiftdlg.h:17 (defined in RealMyRoomMainUI.cpp)
	void SetItemInfo(sGiftInfo* pInfo);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnMyPangInit(intptr_t param);
	void OnSendPangInit(intptr_t param);
	void OnTotalPangInit(intptr_t param);
	void OnItemInit(intptr_t param);
	void OnItemOwnerDraw(int param);

private:
	FrArea* m_pItemArea;             // 0x110 (OnItemInit)
	FrEdit* m_pMyPangEdit;           // 0x114 (OnMyPangInit)
	FrEdit* m_pSendPangEdit;         // 0x118 (OnSendPangInit)
	FrEdit* m_pTotalPangEdit;        // 0x11c (OnTotalPangInit)
	sGiftInfo* m_itemInfo;           // 0x120
	UNKNOWN(0x124, 0x128);
};
BEGIN_LAYOUT_CHECK(FrPangGiftDlg)
	CHECK_FIELD(FrPangGiftDlg, m_pItemArea, 0x110);
	CHECK_FIELD(FrPangGiftDlg, m_pMyPangEdit, 0x114);
	CHECK_FIELD(FrPangGiftDlg, m_pSendPangEdit, 0x118);
	CHECK_FIELD(FrPangGiftDlg, m_pTotalPangEdit, 0x11c);
	CHECK_FIELD(FrPangGiftDlg, m_itemInfo, 0x120);
END_LAYOUT_CHECK
CHECK_SIZE(FrPangGiftDlg, 0x128);

#endif // PG_CLIENT_PROJECTG_PANGGIFTDLG_H
