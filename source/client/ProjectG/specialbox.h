// Original: d:\build\custom\temp\code\client\projectg\specialbox.h
#ifndef PG_CLIENT_PROJECTG_SPECIALBOX_H
#define PG_CLIENT_PROJECTG_SPECIALBOX_H

#include "..\..\defines.h"
#include "gatewayinterface.h"

class FrForm;

namespace _gateway {

class FrSpecialBoxDlg;

// Handler of the special-box gateway content: CGatewayHandler (0x1C bytes),
// then the box dialog it opens (cleared by the dialog's close callback).
class CSpecialBoxHandler : public CGatewayHandler {
	LAYOUT_CHECK_FRIEND;

public:
	// specialbox.h:14
	CSpecialBoxHandler(void) {}
	// specialbox.h:15
	virtual ~CSpecialBoxHandler(void) {}

	virtual bool Initialize(void);
	virtual bool Execute(const MsgObject& msg);

protected:
	bool OnSpecialBoxCloseCallback(int result, FrForm* pForm);

private:
	void OnReqOpenBox(void);
	void OnReqWarningMsg(intptr_t param);
	bool VerifyHaveKeyItem(void);

	FrSpecialBoxDlg* m_pSpecialBoxDlg; // 0x1C, set by OnReqOpenBox and OnReqWarningMsg
};

CHECK_SIZE(CSpecialBoxHandler, 0x20);

}

BEGIN_LAYOUT_CHECK(_gateway::CSpecialBoxHandler)
	CHECK_FIELD(_gateway::CSpecialBoxHandler, m_pSpecialBoxDlg, 0x1C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SPECIALBOX_H
