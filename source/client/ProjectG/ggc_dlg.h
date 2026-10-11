// Original: d:\build\custom\temp\code\client\projectg\ggc_dlg.h
#ifndef PG_CLIENT_PROJECTG_GGC_DLG_H
#define PG_CLIENT_PROJECTG_GGC_DLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrEdit;

// GameGuard (GGC) critical-error dialog: shows the first critical error
// message of GGC_Helper in its "message" edit box; its size (0x118) is the
// `new` of GGC_DlgMakeInstance.
class GGC_Dlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ggc_dlg.h:16

	GGC_Dlg(void) {   // ggc_dlg.h:19
		m_pMessageEdit = 0;
	}

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnMessage_Init(intptr_t param);

private:
	FrEdit* m_pMessageEdit;   // 0x110 ("message")
	UNKNOWN(0x114, 0x118);
};
CHECK_SIZE(GGC_Dlg, 0x118);
BEGIN_LAYOUT_CHECK(GGC_Dlg)
	CHECK_FIELD(GGC_Dlg, m_pMessageEdit, 0x110);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GGC_DLG_H
