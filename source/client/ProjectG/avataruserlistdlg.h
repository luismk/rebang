// Original: d:\build\custom\temp\code\client\projectg\avataruserlistdlg.h
#ifndef PG_CLIENT_PROJECTG_AVATARUSERLISTDLG_H
#define PG_CLIENT_PROJECTG_AVATARUSERLISTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class FrListBox;

// User list of the avatar lobby; its size (0x148) is the `new` of
// FrAvatarUserListDlgMakeInstance. Only the members this unit defines so far
// are declared.
class FrAvatarUserListDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // avataruserlistdlg.h:7

	FrAvatarUserListDlg(void);
	virtual ~FrAvatarUserListDlg(void);

	void UpdateUserList(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // avataruserlistdlg.cpp:15

	void OnUserListInit(intptr_t param);
	void OnUserListOwnerDraw(int param);
	void OnUserListLBtnDown(void);
	void OnUserListRBtnUp(void);

	virtual void OnProc(const float dt);

	int m_selected;               // 0x110
	FrListBox* m_pUserList;       // 0x114
	float m_alpha;                // 0x118
	const Bitmap* m_pBitmaps[11]; // 0x11c, set by OnUserListInit
};

CHECK_SIZE(FrAvatarUserListDlg, 0x148);
BEGIN_LAYOUT_CHECK(FrAvatarUserListDlg)
	CHECK_FIELD(FrAvatarUserListDlg, m_selected, 0x110);
	CHECK_FIELD(FrAvatarUserListDlg, m_pUserList, 0x114);
	CHECK_FIELD(FrAvatarUserListDlg, m_alpha, 0x118);
	CHECK_FIELD(FrAvatarUserListDlg, m_pBitmaps, 0x11c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_AVATARUSERLISTDLG_H
