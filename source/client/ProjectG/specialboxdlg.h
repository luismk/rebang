// Original: d:\build\custom\temp\code\client\projectg\specialboxdlg.h
#ifndef PG_CLIENT_PROJECTG_SPECIALBOXDLG_H
#define PG_CLIENT_PROJECTG_SPECIALBOXDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class FrArea;
class FrEdit;
class FrGaugeBar;

namespace _gateway {

// Special box dialog: a notify area drawing the box item's icon, an
// explanation edit and a progress bar that fills over m_limit seconds before
// the box is opened (packet 0xF1); its size (0x130) is the `new` of
// FrSpecialBoxDlgMakeInstance.
class FrSpecialBoxDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // specialboxdlg.h:11

	FrSpecialBoxDlg(void);
	virtual ~FrSpecialBoxDlg(void) {                                 // specialboxdlg.h:15
	}

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	void SetMessageType(int type);
	void SetMessage(const char* message);
	void DisbleProgressBar(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnNotifyAreaInit(intptr_t param);
	void OnNotifyAreaOwnerdraw(int param);
	void OnExplaneEditInit(intptr_t param);
	void OnProgressBarInit(intptr_t param);

private:
	FrArea* m_pNotifyArea;         // 0x110
	FrEdit* m_pExplaneEdit;        // 0x114
	FrGaugeBar* m_pProgressBar;    // 0x118
	const Bitmap* m_pBoxBitmap;    // 0x11c (icon of item 0x1A00015C)
	bool m_bDisable;               // 0x120 (progress finished or disabled)
	float m_limit;                 // 0x124 (seconds until the box opens)
	float m_elapsed;               // 0x128
	UNKNOWN(0x12c, 0x130);
};

}

CHECK_SIZE(_gateway::FrSpecialBoxDlg, 0x130);
BEGIN_LAYOUT_CHECK(_gateway::FrSpecialBoxDlg)
	CHECK_FIELD(_gateway::FrSpecialBoxDlg, m_pNotifyArea, 0x110);
	CHECK_FIELD(_gateway::FrSpecialBoxDlg, m_pExplaneEdit, 0x114);
	CHECK_FIELD(_gateway::FrSpecialBoxDlg, m_pProgressBar, 0x118);
	CHECK_FIELD(_gateway::FrSpecialBoxDlg, m_pBoxBitmap, 0x11c);
	CHECK_FIELD(_gateway::FrSpecialBoxDlg, m_bDisable, 0x120);
	CHECK_FIELD(_gateway::FrSpecialBoxDlg, m_limit, 0x124);
	CHECK_FIELD(_gateway::FrSpecialBoxDlg, m_elapsed, 0x128);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SPECIALBOXDLG_H
