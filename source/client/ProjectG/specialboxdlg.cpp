// Original: d:\build\custom\temp\code\client\projectg\specialboxdlg.cpp
#include "SpecialBoxDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "fredit.h"
#include "frgaugebar.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "bitmap.h"
#include "TaskManager.h"
#include "actor.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "UccManager.h"
#include "MessageManager.h"

namespace _gateway {

// specialboxdlg.cpp:11 (run-time type and factory registration of FrSpecialBoxDlg)
const WRTTI FrSpecialBoxDlg::m_RTTI("FrSpecialBoxDlg", &FrForm::m_RTTI);

IObject* FrSpecialBoxDlgMakeInstance(void) {
	return new FrSpecialBoxDlg;
}

struct __sFrSpecialBoxDlg {
	__sFrSpecialBoxDlg(void) {
		ObjectFactory().AddObjectFunctor(FrSpecialBoxDlgMakeInstance, "FrSpecialBoxDlg");
	}
};
static __sFrSpecialBoxDlg s_registerFrSpecialBoxDlg;

// specialboxdlg.cpp:14
const sFRESH_MSGMAP* FrSpecialBoxDlg::GetMessageMap(void) const {
	return &FrSpecialBoxDlg::_MsgMap;
}

sFRESH_MSGMAP FrSpecialBoxDlg::_MsgMap = { &FrForm::_MsgMap, &FrSpecialBoxDlg::_MsgEntries[0] };

sFRESH_ENTRY FrSpecialBoxDlg::_MsgEntries[] = {
	// specialboxdlg.cpp:16
	{ "notify_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSpecialBoxDlg::OnNotifyAreaInit },
	// specialboxdlg.cpp:17
	{ "notify_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSpecialBoxDlg::OnNotifyAreaOwnerdraw },

	// specialboxdlg.cpp:19
	{ "explane", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSpecialBoxDlg::OnExplaneEditInit },
	// specialboxdlg.cpp:20
	{ "progressbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSpecialBoxDlg::OnProgressBarInit },

	// specialboxdlg.cpp:22
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// specialboxdlg.cpp:26
FrSpecialBoxDlg::FrSpecialBoxDlg(void) {
	m_pNotifyArea = 0;
	m_pExplaneEdit = 0;
	m_pProgressBar = 0;
	m_pBoxBitmap = 0;

	m_bDisable = false;

	m_limit = 0;
	m_elapsed = 0;
}

// specialboxdlg.cpp:39
bool FrSpecialBoxDlg::OnInit(void) {
	IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(0x1A00015C);


	m_pBoxBitmap = g_pFresh->GetBitmap(pItem->m_icon);





	m_limit = 3.0f;


	return FrForm::OnInit();
}

// specialboxdlg.cpp:56
void FrSpecialBoxDlg::OnProc(const float dt) {
	if (m_bDisable) {
		return;
	}




	m_elapsed += dt;


	if (m_elapsed >= m_limit) {
		m_pProgressBar->SetPos(100);
		m_bDisable = true;
		WSendPacket packet((enumClientPacket)0xF1);
		packet.Encode4(0x1A00015B);
		packet.Send(SEND_TO_DEFAULT);

		CTaskManager* pTaskManager = CTaskManager::Instance();
		(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
		Close(FrOK, true);
		return;
	}


	m_pProgressBar->SetPos((int)(m_elapsed / m_limit * 100.0f));

}

// specialboxdlg.cpp:82
void FrSpecialBoxDlg::SetMessageType(int type) {
	const char* message = WSingleton<_systemmsg::CMessageManager>::Instance()->GetMsg((_systemmsg::eMsgIdentifier)type);


	m_pExplaneEdit->AddText(message, false, true);
}

// specialboxdlg.cpp:90
void FrSpecialBoxDlg::SetMessage(const char* message) {


	m_pExplaneEdit->AddText(message, false, true);
}

// specialboxdlg.cpp:97
void FrSpecialBoxDlg::DisbleProgressBar(void) {
	m_bDisable = true;
	if (m_pProgressBar) {

		m_pProgressBar->SetVisible(false);
	}
}

// specialboxdlg.cpp:106
void FrSpecialBoxDlg::OnNotifyAreaInit(intptr_t param) {
	m_pNotifyArea = DYNAMIC_CAST(FrArea, param);
}

// specialboxdlg.cpp:111
void FrSpecialBoxDlg::OnNotifyAreaOwnerdraw(int param) {





	g_pFresh->GetManager()->GetGDI()->DrawTexture(m_pBoxBitmap, WRect(m_rect.x + 150.0f, m_rect.y + 20.0f, m_pBoxBitmap->Width(), m_pBoxBitmap->Height()), 0xffffffff, 0);
}

// specialboxdlg.cpp:121
void FrSpecialBoxDlg::OnExplaneEditInit(intptr_t param) {
	m_pExplaneEdit = DYNAMIC_CAST(FrEdit, param);
}

// specialboxdlg.cpp:126
void FrSpecialBoxDlg::OnProgressBarInit(intptr_t param) {
	m_pProgressBar = DYNAMIC_CAST(FrGaugeBar, param);


	m_pProgressBar->SetRange(0, 100, 0);
}

}
