// Original: d:\build\custom\temp\code\client\projectg\specialbox.cpp
#include "specialbox.h"
#include "SpecialBoxDlg.h"
#include "MessageManager.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "actor.h"
#include "commonutil.h"
#include "fresh.h"
#include "UccManager.h"

// Target actor of the "key item missing" warning.
static const char* s_gatewayActorName = "Gateway";

namespace _gateway {

// Type id of the special box's key item.
static const unsigned long SPECIAL_BOX_KEY_ITEM = 0x1A00015C;

// specialbox.cpp:13
bool CSpecialBoxHandler::Initialize(void) {
	AddFunctionType(0, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CSpecialBoxHandler::OnReqOpenBox);
	AddFunctionType(1, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CSpecialBoxHandler::*)(void))&CSpecialBoxHandler::OnReqWarningMsg);

	return true;
}

// specialbox.cpp:21
bool CSpecialBoxHandler::Execute(const MsgObject& msg) {
	SetFunctionType(msg.m_param1);
	// The message travels through Exec's int parameter (the original's
	// 32-bit pointer-in-int; not portable to 64-bit).
	return Exec(this, (intptr_t)&msg);
}

// specialbox.cpp:28
bool CSpecialBoxHandler::OnSpecialBoxCloseCallback(int result, FrForm* pForm) {
	m_pSpecialBoxDlg = 0;

	return true;
}

// specialbox.cpp:35
bool CSpecialBoxHandler::VerifyHaveKeyItem(void) {
	CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
	std::list<sItemInfo>::iterator it;
	for (it = pDoc->m_itemInfoList.begin(); it != pDoc->m_itemInfoList.end() && (*it).m_typeId != SPECIAL_BOX_KEY_ITEM; ++it);
	if (it != pDoc->m_itemInfoList.end()) {
		return true;
	}

	return false;
}

// specialbox.cpp:47
void CSpecialBoxHandler::OnReqOpenBox(void) {
	if (VerifyHaveKeyItem() == false) {
		CTaskManager::Instance()->PostMsg(0, s_gatewayActorName, 0x276, 1, 0x200B3C, 0, 0);
		return;
	}

	m_pSpecialBoxDlg = CreateForm<FrSpecialBoxDlg>(g_pFresh->GetManager(), this, "specialBoxOpen", 0);

	const char* format = WSingleton<_systemmsg::CMessageManager>::Instance()->GetMsg((_systemmsg::eMsgIdentifier)0xCB);

	IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(SPECIAL_BOX_KEY_ITEM);

	m_pSpecialBoxDlg->SetMessage(MakeStr(format, pItem->m_name));
	m_pSpecialBoxDlg->Open((FRESH_RESULT_CALLBACK)&CSpecialBoxHandler::OnSpecialBoxCloseCallback, 1);
}

// specialbox.cpp:68
// param is the MsgObject's address; m_param2 the warning kind
void CSpecialBoxHandler::OnReqWarningMsg(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;
	int messageType = 0;
	switch (pMsg->m_param2) {
	case 0x200B3B:
		messageType = 0xC8;
		break;
	case 0x200B3C:
		messageType = 0xC9;
		break;
	case 0x200B3D:
		messageType = 0xCA;
		break;
	}

	m_pSpecialBoxDlg = CreateForm<FrSpecialBoxDlg>(g_pFresh->GetManager(), this, "specialBoxOpen", 0);
	m_pSpecialBoxDlg->SetMessageType(messageType);
	m_pSpecialBoxDlg->DisbleProgressBar();
	m_pSpecialBoxDlg->Open((FRESH_RESULT_CALLBACK)&CSpecialBoxHandler::OnSpecialBoxCloseCallback, 1);
}

}
