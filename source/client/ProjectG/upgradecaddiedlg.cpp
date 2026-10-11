// Original: d:\build\custom\temp\code\client\projectg\upgradecaddiedlg.cpp
#include <string.h>
#include "UpgradeCaddieDlg.h"
#include "ObjectFactory.h"
#include "fredit.h"
#include "commonutil.h"
#include "Packet.h"
#include "TaskManager.h"
#include "actor.h"

// upgradecaddiedlg.cpp:5 (run-time type and factory registration of FrUpgradeCaddieDlg)
const WRTTI FrUpgradeCaddieDlg::m_RTTI("FrUpgradeCaddieDlg", &FrForm::m_RTTI);

IObject* FrUpgradeCaddieDlgMakeInstance(void) {
	return new FrUpgradeCaddieDlg;
}

struct __sFrUpgradeCaddieDlg {
	__sFrUpgradeCaddieDlg(void) {
		ObjectFactory().AddObjectFunctor(FrUpgradeCaddieDlgMakeInstance, "FrUpgradeCaddieDlg");
	}
};
static __sFrUpgradeCaddieDlg s_registerFrUpgradeCaddieDlg;

// upgradecaddiedlg.cpp:7
const sFRESH_MSGMAP* FrUpgradeCaddieDlg::GetMessageMap(void) const {
	return &FrUpgradeCaddieDlg::_MsgMap;
}

sFRESH_MSGMAP FrUpgradeCaddieDlg::_MsgMap = { &FrForm::_MsgMap, &FrUpgradeCaddieDlg::_MsgEntries[0] };

sFRESH_ENTRY FrUpgradeCaddieDlg::_MsgEntries[] = {
	// upgradecaddiedlg.cpp:9
	{ "message", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUpgradeCaddieDlg::OnMessageInit },
	// upgradecaddiedlg.cpp:10
	{ "Yes", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUpgradeCaddieDlg::OnYesBtnUp },
	// upgradecaddiedlg.cpp:11
	{ "No", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUpgradeCaddieDlg::OnNoBtnUp },

	// upgradecaddiedlg.cpp:13
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// upgradecaddiedlg.cpp:16
FrUpgradeCaddieDlg::FrUpgradeCaddieDlg(void) {
	m_pMessage = 0;
	m_caddieId = 0;
	memset(m_stats, 0, sizeof(m_stats));
	m_value12c = m_value130 = 0;
}

// upgradecaddiedlg.cpp:24
void FrUpgradeCaddieDlg::OnMessageInit(intptr_t param) {
	m_pMessage = DYNAMIC_CAST(FrEdit, param);
}

// upgradecaddiedlg.cpp:30
void FrUpgradeCaddieDlg::OnYesBtnUp(void) {
	WSendPacket packet((enumClientPacket)0xEC);
	packet.Encode4(m_caddieId);
	packet.Send(SEND_TO_DEFAULT);
	CTaskManager* pTaskManager = CTaskManager::Instance();
	(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
	Close(FrOK, true);
}

// upgradecaddiedlg.cpp:39
void FrUpgradeCaddieDlg::OnNoBtnUp(void) {
	Close(FrCANCEL, true);
}

// upgradecaddiedlg.cpp:45
void FrUpgradeCaddieDlg::SetUpgradeInfo(unsigned long caddieId, int* stats, int beforePang, int afterPang) {
	m_caddieId = caddieId;
	memcpy(m_stats, stats, sizeof(m_stats));
	if (!m_pMessage) {
		return;
	}











	m_pMessage->AddText(MakeStr("\xc4\xb3\xb5\xf0 \xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5\xb8\xa6 \xc1\xf8\xc7\xe0\xc7\xd5\xb4\xcf\xb4\xd9.\n\xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc \xc0\xcc\xc8\xc4 \xba\xaf\xb0\xe6\xbb\xe7\xc7\xd7\xc0\xba \xbe\xc6\xb7\xa1\xbf\xcd \xb0\xb0\xbd\xc0\xb4\xcf\xb4\xd9.\n\n\\c0xffff0000\\c\xc0\xe7\xb0\xed\xbf\xeb \xba\xf1\xbf\xeb : %s\n\\c0xff000000\\c\xc6\xc4\xbf\xf6 : %s\n\xc4\xc1\xc6\xae\xb7\xd1 : %s\n\xc1\xa4\xc8\xae\xb5\xb5 : %s\n\xbd\xba\xc7\xc9 : %s\n\xc4\xbf\xba\xea : %s\n",
		afterPang - beforePang == 0 ? "\xc7\xd8\xb4\xe7\xbb\xe7\xc7\xd7 \xbe\xf8\xc0\xbd" : MakeStr("%d\xc6\xce -> %d\xc6\xce", beforePang, afterPang),
		m_stats[0] > 0 ? MakeStr("+%d", m_stats[0]) : MakeStr("%d", m_stats[0]),
		m_stats[1] > 0 ? MakeStr("+%d", m_stats[1]) : MakeStr("%d", m_stats[1]),
		m_stats[2] > 0 ? MakeStr("+%d", m_stats[2]) : MakeStr("%d", m_stats[2]),
		m_stats[3] > 0 ? MakeStr("+%d", m_stats[3]) : MakeStr("%d", m_stats[3]),
		m_stats[4] > 0 ? MakeStr("+%d", m_stats[4]) : MakeStr("%d", m_stats[4])), false, true);
}
