// Original: d:\build\custom\temp\code\client\projectg\finalrankdlg.h
#ifndef PG_CLIENT_PROJECTG_FINALRANKDLG_H
#define PG_CLIENT_PROJECTG_FINALRANKDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrListBox;

// Final ranking dialog: two list boxes ("list_left", "list_right") showing
// the players of the finished game, sorted by FinalRankCompare; its size
// (0x118) is the `new` of FrFinalRankDlgMakeInstance.
class FrFinalRankDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // finalrankdlg.h:8

	FrFinalRankDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnRankLeftInit(intptr_t param);
	void OnRankLeftOwnerDraw(intptr_t param);
	void OnRankRightInit(intptr_t param);
	void OnRankRightOwnerDraw(intptr_t param);

private:
	FrListBox* m_pListLeft;            // 0x110 (OnRankLeftInit)
	FrListBox* m_pListRight;           // 0x114 (OnRankRightInit)
};
BEGIN_LAYOUT_CHECK(FrFinalRankDlg)
	CHECK_FIELD(FrFinalRankDlg, m_pListLeft, 0x110);
	CHECK_FIELD(FrFinalRankDlg, m_pListRight, 0x114);
END_LAYOUT_CHECK
CHECK_SIZE(FrFinalRankDlg, 0x118);

#endif // PG_CLIENT_PROJECTG_FINALRANKDLG_H
