// Original: d:\build\custom\temp\code\client\projectg\frreadytoall.h
#ifndef PG_CLIENT_PROJECTG_FRREADYTOALL_H
#define PG_CLIENT_PROJECTG_FRREADYTOALL_H

#include "..\..\defines.h"
#include "frform.h"

// The ready form of a room member ("ReadyStart" / "WaitCancel" buttons); no
// members of its own: FrReadyToAllMakeInstance news FrForm's size (0x110).
class FrReadyToAll : public FrForm {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frreadytoall.h:7

	FrReadyToAll(void);
	virtual ~FrReadyToAll(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnReadyBtnUp(void);
	void OnWaitBtnUp(void);
};
CHECK_SIZE(FrReadyToAll, 0x110);

// The room master's ready form (start / exit buttons); no members of its
// own: FrReadyToMasterMakeInstance news FrForm's size (0x110).
class FrReadyToMaster : public FrForm {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frreadytoall.h:23

	FrReadyToMaster(void);
	virtual ~FrReadyToMaster(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnStartBtnUp(void);
	void OnExitBtnUp(void);
	virtual void OnProc(const float dt);
};
CHECK_SIZE(FrReadyToMaster, 0x110);

#endif // PG_CLIENT_PROJECTG_FRREADYTOALL_H
