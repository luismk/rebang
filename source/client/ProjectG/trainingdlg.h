// Original: d:\build\custom\temp\code\client\projectg\trainingdlg.h
#ifndef PG_CLIENT_PROJECTG_TRAININGDLG_H
#define PG_CLIENT_PROJECTG_TRAININGDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrListBox;

// Training mode selection dialog: an owner-drawn list box of the training
// buttons (buttonList), each starting a practice mode. Its size (0x118) is
// the `new` of FrTrainingDlgMakeInstance.
class FrTrainingDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // trainingdlg.h:10

	FrTrainingDlg(void) {                                           // trainingdlg.h:13
		m_pListBox = 0;
	}

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnTrainingInit(intptr_t param);
	void OnTrainingBtnUp(void);
	void OnTrainingOwnerDraw(int param);
	void OnTrainingCloseBtnUp(void);
	bool OnNewTrainingOptionDlgResult(int result, FrForm* pForm);

private:
	FrListBox* m_pListBox;                // 0x110 (OnTrainingInit, the "training" list box)
	int m_optionState;                    // 0x114 (cleared by OnTrainingInit and OnNewTrainingOptionDlgResult)
};
BEGIN_LAYOUT_CHECK(FrTrainingDlg)
	CHECK_FIELD(FrTrainingDlg, m_pListBox, 0x110);
	CHECK_FIELD(FrTrainingDlg, m_optionState, 0x114);
END_LAYOUT_CHECK
CHECK_SIZE(FrTrainingDlg, 0x118);

#endif // PG_CLIENT_PROJECTG_TRAININGDLG_H
