// Original: d:\build\custom\temp\code\client\projectg\msnusersnapshot.h
#ifndef PG_CLIENT_PROJECTG_MSNUSERSNAPSHOT_H
#define PG_CLIENT_PROJECTG_MSNUSERSNAPSHOT_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrStatic;

// Messenger user snapshot dialog; its size (0x128) is the `new` of
// ntMsnUserSnapshotDlgMakeInstance. The statics are bound by the On*Init
// handlers.
class ntMsnUserSnapshotDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // msnusersnapshot.h:16

	ntMsnUserSnapshotDlg(void);
	virtual ~ntMsnUserSnapshotDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // msnusersnapshot.cpp:24

	void OnIdInit(intptr_t param);
	void OnGenderInit(intptr_t param);
	void OnLevelInit(intptr_t param);
	void OnGuildInit(intptr_t param);
	void OnSchoolInit(intptr_t param);
	void OnStatusInit(intptr_t param);
	void OnDetailClick(void);

	FrStatic* m_pId;       // 0x110, OnIdInit
	FrStatic* m_pGender;   // 0x114, OnGenderInit
	FrStatic* m_pLevel;    // 0x118, OnLevelInit
	FrStatic* m_pGuild;    // 0x11c, OnGuildInit
	FrStatic* m_pSchool;   // 0x120, OnSchoolInit
	FrStatic* m_pStatus;   // 0x124, OnStatusInit
};

CHECK_SIZE(ntMsnUserSnapshotDlg, 0x128);
BEGIN_LAYOUT_CHECK(ntMsnUserSnapshotDlg)
	CHECK_FIELD(ntMsnUserSnapshotDlg, m_pId, 0x110);
	CHECK_FIELD(ntMsnUserSnapshotDlg, m_pGender, 0x114);
	CHECK_FIELD(ntMsnUserSnapshotDlg, m_pLevel, 0x118);
	CHECK_FIELD(ntMsnUserSnapshotDlg, m_pGuild, 0x11c);
	CHECK_FIELD(ntMsnUserSnapshotDlg, m_pSchool, 0x120);
	CHECK_FIELD(ntMsnUserSnapshotDlg, m_pStatus, 0x124);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_MSNUSERSNAPSHOT_H
