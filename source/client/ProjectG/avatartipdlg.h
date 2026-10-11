// Original: d:\build\custom\temp\code\client\projectg\avatartipdlg.h
#ifndef PG_CLIENT_PROJECTG_AVATARTIPDLG_H
#define PG_CLIENT_PROJECTG_AVATARTIPDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrViewer;
class FrButton;

// Avatar tip dialog: a viewer paging through "tip1.tga".."tip6.tga" with the
// "prev"/"next" buttons; its size (0x128) is the `new` of
// FrAvatarTipDlgMakeInstance.
class FrAvatarTipDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // avatartipdlg.h:8

	FrAvatarTipDlg(void);
	virtual ~FrAvatarTipDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnTipInit(intptr_t param);
	void OnPrevInit(intptr_t param);
	void OnPrevBtnUp(void);
	void OnNextInit(intptr_t param);
	void OnNextBtnUp(void);
	void OnCloseInit(intptr_t param);
	void OnCloseBtnUp(void);

private:
	FrViewer* m_pTip;     // 0x110 ("tip")
	FrButton* m_pPrev;    // 0x114 ("prev")
	FrButton* m_pNext;    // 0x118 ("next")
	FrButton* m_pClose;   // 0x11c ("cancel")
	int m_tipIndex;       // 0x120 (0..5, shows "tip%d.tga" of m_tipIndex + 1)
	UNKNOWN(0x124, 0x128);
};

CHECK_SIZE(FrAvatarTipDlg, 0x128);
BEGIN_LAYOUT_CHECK(FrAvatarTipDlg)
	CHECK_FIELD(FrAvatarTipDlg, m_pTip, 0x110);
	CHECK_FIELD(FrAvatarTipDlg, m_pPrev, 0x114);
	CHECK_FIELD(FrAvatarTipDlg, m_pNext, 0x118);
	CHECK_FIELD(FrAvatarTipDlg, m_pClose, 0x11c);
	CHECK_FIELD(FrAvatarTipDlg, m_tipIndex, 0x120);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_AVATARTIPDLG_H
