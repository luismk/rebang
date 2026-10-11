// Original: d:\build\custom\temp\code\client\projectg\notifyform.h
#ifndef PG_CLIENT_PROJECTG_NOTIFYFORM_H
#define PG_CLIENT_PROJECTG_NOTIFYFORM_H

#include "..\..\defines.h"
#include "frform.h"

// A plain notification form: its "ok_btn", "cancel_btn", "yes_btn" and
// "no_btn" buttons close it with the matching result. Adds no members (the
// `new` of FrNotifyFormMakeInstance is FrForm's 0x110); its four handlers are
// new virtuals (vtable slots 37-40).
class FrNotifyForm : public FrForm {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // notifyform.h:8

	FrNotifyForm(void);
	virtual ~FrNotifyForm(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	// vtable slots 37-40
	virtual void OnFreshOkay(void);
	virtual void OnFreshCancel(void);
	virtual void OnFreshYes(void);
	virtual void OnFreshNo(void);
};

CHECK_SIZE(FrNotifyForm, 0x110);

#endif // PG_CLIENT_PROJECTG_NOTIFYFORM_H
