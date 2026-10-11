// Original: d:\build\custom\temp\code\client\projectg\christmassockeventdlg.h
#ifndef PG_CLIENT_PROJECTG_CHRISTMASSOCKEVENTDLG_H
#define PG_CLIENT_PROJECTG_CHRISTMASSOCKEVENTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrViewer;

// The Christmas sock event description window: one image viewer showing the
// event's popup picture. Its size (0x118) is the `new` of
// FrChristmasSockEventDescDlgMakeInstance.
class FrChristmasSockEventDescDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // christmassockeventdlg.h:6

	FrChristmasSockEventDescDlg(void);
	virtual ~FrChristmasSockEventDescDlg(void);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitDescView(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

	FrViewer* m_pDescView;    // 0x110 (OnInitDescView)
	UNKNOWN(0x114, 0x118);
};

CHECK_SIZE(FrChristmasSockEventDescDlg, 0x118);
BEGIN_LAYOUT_CHECK(FrChristmasSockEventDescDlg)
	CHECK_FIELD(FrChristmasSockEventDescDlg, m_pDescView, 0x110);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CHRISTMASSOCKEVENTDLG_H
