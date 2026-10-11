// Original: d:\build\custom\temp\code\client\projectg\returngiftdlg.h
#ifndef PG_CLIENT_PROJECTG_RETURNGIFTDLG_H
#define PG_CLIENT_PROJECTG_RETURNGIFTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

// Return-gift dialog; its size (0x118) is the `new` of
// FrReturnGiftDlgMakeInstance. Only the members this unit defines so far are
// declared.
class FrReturnGiftDlg : public FrForm {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // returngiftdlg.h:20

	FrReturnGiftDlg(void);

	// The two ids sent with the return-gift request (packet 0x4D) when the
	// dialog is answered yes (CPost_ReciveDlg/CRealMyRoomMain::OnReturnGiftDlgResult).
	unsigned long m_requestIds[2];   // 0x110
};

CHECK_SIZE(FrReturnGiftDlg, 0x118);
CHECK_FIELD(FrReturnGiftDlg, m_requestIds, 0x110);

#endif // PG_CLIENT_PROJECTG_RETURNGIFTDLG_H
