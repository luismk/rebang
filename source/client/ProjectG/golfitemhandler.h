// Original: d:\build\custom\temp\code\client\projectg\golfitemhandler.h
#ifndef PG_CLIENT_PROJECTG_GOLFITEMHANDLER_H
#define PG_CLIENT_PROJECTG_GOLFITEMHANDLER_H

#include "..\..\defines.h"
#include "gatewayinterface.h"

class FrForm;
class FrNoticeDlg;

namespace _gateway {

class CGolfItemDoc;

bool IsSpecialBoxArea(void);

// Handler of the golf-item gateway content: CGatewayHandler (0x1C bytes),
// then the WizCity notice dialog.
class CGolfItemHandler : public CGatewayHandler {
	LAYOUT_CHECK_FRIEND;

public:
	CGolfItemHandler(void);
	virtual ~CGolfItemHandler(void);

	virtual bool Initialize(void);
	virtual bool Execute(const MsgObject& msg);

protected:
	bool OnCloseWizCityNoticeResult(int result, FrForm* pForm);

private:
	void OnRequestResetCalipers(void);
	void OnGetFlagCalipers(intptr_t param);
	void OnSetFlagCalipersEnable(void);
	void OnSetFlagCalipersDisable(void);
	void OnGetCountCalipers(intptr_t param);
	void OnSetCountCalipers(intptr_t param);
	void OnDecreaseCalipers(void);
	void OnSetEnableItemTID(intptr_t param);
	void OnGetEnableTimeBooster(intptr_t param);
	void OnIncSpecialBox(void);
	void OnResetSpecialBox(void);
	void OnGetSpecialBox(intptr_t param);
	void OnCheckSomeMoreGetSpecialBox(void);
	void OnRequestWizCityNoticeDlg(void);
	int CalcRemainMoreGetSpecialBoxCount(int count);

	FrNoticeDlg* m_pNoticeDlg; // 0x1C, set by OnRequestWizCityNoticeDlg
};

CHECK_SIZE(CGolfItemHandler, 0x20);

}

BEGIN_LAYOUT_CHECK(_gateway::CGolfItemHandler)
	CHECK_FIELD(_gateway::CGolfItemHandler, m_pNoticeDlg, 0x1C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GOLFITEMHANDLER_H
