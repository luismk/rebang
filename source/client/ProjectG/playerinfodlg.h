// Original: d:\build\custom\temp\code\client\projectg\playerinfodlg.h
#ifndef PG_CLIENT_PROJECTG_PLAYERINFODLG_H
#define PG_CLIENT_PROJECTG_PLAYERINFODLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrEdit;

// Player information dialog: three text boxes filled by
// WritePlayerInfoDlg; its size (0x120) is the `new` of
// FrPlayerInfoDlgMakeInstance.
class FrPlayerInfoDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // playerinfodlg.h:13

	FrPlayerInfoDlg(void);
	virtual ~FrPlayerInfoDlg(void);

	void WritePlayerInfoDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnUserInfoInit(intptr_t param);
	void OnCharInfoInit(intptr_t param);
	void OnCadInfoInit(intptr_t param);

private:
	FrEdit* m_pUserInfo;    // 0x110 ("user")
	FrEdit* m_pCharInfo;    // 0x114 ("character")
	FrEdit* m_pCadInfo;     // 0x118 ("caddie")
	PADDING(0x11c, 0x120);
};

CHECK_SIZE(FrPlayerInfoDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrPlayerInfoDlg)
	CHECK_FIELD(FrPlayerInfoDlg, m_pUserInfo, 0x110);
	CHECK_FIELD(FrPlayerInfoDlg, m_pCharInfo, 0x114);
	CHECK_FIELD(FrPlayerInfoDlg, m_pCadInfo, 0x118);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_PLAYERINFODLG_H
