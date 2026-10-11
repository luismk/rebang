// Original: d:\build\custom\temp\code\client\projectg\detaildlg.h
#ifndef PG_CLIENT_PROJECTG_DETAILDLG_H
#define PG_CLIENT_PROJECTG_DETAILDLG_H

#include "..\..\defines.h"
#include "frform.h"

class FrViewer;

// Detail (notice) dialog: a close button, a caption and an image viewer;
// its size (0x118) is the `new` of NtDetailDlgMakeInstance.
class NtDetailDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // detaildlg.h:16

	NtDetailDlg(void);
	virtual ~NtDetailDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnCloseBtnInit(intptr_t param);
	void OnCloseBtnUp(void);
	void OnCaptionInit(intptr_t param);
	void OnViewInit(intptr_t param);

private:
	FrViewer* m_pView;   // 0x110 ("view"); 0x114: tail padding to FrForm's 8-byte alignment
};

CHECK_SIZE(NtDetailDlg, 0x118);
BEGIN_LAYOUT_CHECK(NtDetailDlg)
	CHECK_FIELD(NtDetailDlg, m_pView, 0x110);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_DETAILDLG_H
