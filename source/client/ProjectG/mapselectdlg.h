// Original: d:\build\custom\temp\code\client\projectg\mapselectdlg.h
#ifndef PG_CLIENT_PROJECTG_MAPSELECTDLG_H
#define PG_CLIENT_PROJECTG_MAPSELECTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrListBox;

// Course selection dialog: a list box of the courses the current channel
// allows; its size (0x120) is the `new` of FrMapSelectDlgMakeInstance.
class FrMapSelectDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // mapselectdlg.h:8

	FrMapSelectDlg(void);
	virtual ~FrMapSelectDlg(void);

	void SetSelectMap(int map);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnSelectInit(intptr_t param);
	void OnSelectBtnUp(void);
	void OnSelectOwnerDraw(intptr_t param);

private:
	FrListBox* m_pSelect;              // 0x110 (OnSelectInit)
	int m_selectMap;                   // 0x114, type id of the selected course
	bool m_sendOnSelect;               // 0x118, in the game room layouts (GAMEROOM, GAMEROOM_EXT, AVATARCHAT_MAIN) OnSelectBtnUp sends the course to the server
	bool m_refuseCourse7F;             // 0x119, OnSelectBtnUp deselects course 0x7F when set
	UNKNOWN(0x11A, 0x120);
};
BEGIN_LAYOUT_CHECK(FrMapSelectDlg)
	CHECK_FIELD(FrMapSelectDlg, m_pSelect, 0x110);
	CHECK_FIELD(FrMapSelectDlg, m_selectMap, 0x114);
	CHECK_FIELD(FrMapSelectDlg, m_sendOnSelect, 0x118);
	CHECK_FIELD(FrMapSelectDlg, m_refuseCourse7F, 0x119);
END_LAYOUT_CHECK
CHECK_SIZE(FrMapSelectDlg, 0x120);

#endif // PG_CLIENT_PROJECTG_MAPSELECTDLG_H
