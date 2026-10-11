// Original: d:\build\custom\temp\code\client\projectg\golfitemhandler.cpp
#include "GolfItemHandler.h"
#include "GatewayDoc.h"
#include "actor.h"
#include "SharedDoc.h"
#include "fresh.h"
#include "frform.h"
#include "noticedlg.h"
#include "TaskManager.h"
#include "GolfTask.h"
#include "ItemManager.h"
#include "UccManager.h"
#include "MessageManager.h"
#include "ChatMsg.h"
#include "commonutil.h"
#include <vector>

namespace _gateway {

// golfitemhandler.cpp:33
CGolfItemHandler::CGolfItemHandler(void)
	: m_pNoticeDlg(0) {
}

// golfitemhandler.cpp:38
CGolfItemHandler::~CGolfItemHandler(void) {
}

// golfitemhandler.cpp:43
bool CGolfItemHandler::Initialize(void) {
	AddFunctionType(0, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CGolfItemHandler::OnRequestResetCalipers);
	AddFunctionType(1, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CGolfItemHandler::*)(void))&CGolfItemHandler::OnGetFlagCalipers);
	AddFunctionType(2, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CGolfItemHandler::OnSetFlagCalipersEnable);
	AddFunctionType(3, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CGolfItemHandler::OnSetFlagCalipersDisable);
	AddFunctionType(5, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CGolfItemHandler::*)(void))&CGolfItemHandler::OnSetCountCalipers);
	AddFunctionType(4, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CGolfItemHandler::*)(void))&CGolfItemHandler::OnGetCountCalipers);
	AddFunctionType(6, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CGolfItemHandler::OnDecreaseCalipers);

	AddFunctionType(7, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CGolfItemHandler::*)(void))&CGolfItemHandler::OnSetEnableItemTID);
	AddFunctionType(8, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CGolfItemHandler::*)(void))&CGolfItemHandler::OnGetEnableTimeBooster);

	AddFunctionType(9, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CGolfItemHandler::OnIncSpecialBox);
	AddFunctionType(10, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CGolfItemHandler::OnResetSpecialBox);
	AddFunctionType(11, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CGolfItemHandler::*)(void))&CGolfItemHandler::OnGetSpecialBox);
	AddFunctionType(12, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CGolfItemHandler::OnCheckSomeMoreGetSpecialBox);

	AddFunctionType(13, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CGolfItemHandler::OnRequestWizCityNoticeDlg);

	return true;
}

// golfitemhandler.cpp:69
bool CGolfItemHandler::Execute(const MsgObject& msg) {
	SetFunctionType(msg.m_param1);
	// The message travels through Exec's int parameter (the original's
	// 32-bit pointer-in-int; not portable to 64-bit).
	return Exec(this, (intptr_t)&msg);
}

// golfitemhandler.cpp:76
void CGolfItemHandler::OnRequestResetCalipers(void) {
	CGolfItemDoc* pDoc = GetDocument<CGolfItemDoc*>();
	if (pDoc == 0) {
		return;
	}

	pDoc->ResetCalipersVariable();
}

// golfitemhandler.cpp:88
// param is the MsgObject's address; m_param2 the address of the bool result
// (pointers in ints, as the original: not portable to 64-bit).
void CGolfItemHandler::OnGetFlagCalipers(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;

	CGolfItemDoc* pDoc = GetDocument<CGolfItemDoc*>();
	if (pDoc == 0) {
		return;
	}

	*(bool*)pMsg->m_param2 = pDoc->GetEnableCalipers();
}

// golfitemhandler.cpp:103
void CGolfItemHandler::OnSetFlagCalipersEnable(void) {
	CGolfItemDoc* pDoc = GetDocument<CGolfItemDoc*>();
	if (pDoc == 0) {
		return;
	}

	pDoc->EnableCalipers(true);
}

// golfitemhandler.cpp:115
void CGolfItemHandler::OnSetFlagCalipersDisable(void) {
	CGolfItemDoc* pDoc = GetDocument<CGolfItemDoc*>();
	if (pDoc == 0) {
		return;
	}

	pDoc->EnableCalipers(false);
}

// golfitemhandler.cpp:127
// param is the MsgObject's address; m_param2 the address of the int result
// (pointers in ints, as the original: not portable to 64-bit).
void CGolfItemHandler::OnGetCountCalipers(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;

	CGolfItemDoc* pDoc = GetDocument<CGolfItemDoc*>();
	if (pDoc == 0) {
		return;
	}

	*(int*)pMsg->m_param2 = pDoc->GetCalipersCount();
}

// golfitemhandler.cpp:142
// param is the MsgObject's address (pointer in an int: not portable to 64-bit).
void CGolfItemHandler::OnSetCountCalipers(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;
	CGolfItemDoc* pDoc = GetDocument<CGolfItemDoc*>();
	if (pDoc == 0) {
		return;
	}

	pDoc->SetCalipersCount(pMsg->m_param2);
}

// golfitemhandler.cpp:156
void CGolfItemHandler::OnDecreaseCalipers(void) {
	CGolfItemDoc* pDoc = GetDocument<CGolfItemDoc*>();
	if (pDoc == 0) {
		return;
	}

	pDoc->DeceaseCalipersCount();
}

// golfitemhandler.cpp:168
// param is the MsgObject's address; m_param2 the address of the list of
// usable item type ids (pointers in ints, as the original: not portable to
// 64-bit).
void CGolfItemHandler::OnSetEnableItemTID(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;
	std::vector<unsigned long>* pItemList = (std::vector<unsigned long>*)pMsg->m_param2;

	CGolfItemDoc* pDoc = GetDocument<CGolfItemDoc*>();
	if (pDoc == 0) {
		return;
	}

	pDoc->ResetUsableItemList();

	int count = pItemList->size();
	for (int i = 0; i < count; i++) {
		switch (pItemList->at(i)) {
		case 0x1A000011:
			pDoc->m_usableItemList[0] = 1;
			break;
		case 0x1A000040:
			pDoc->m_usableItemList[1] = 1;
			break;
		}
	}
}

// golfitemhandler.cpp:200
// param is the MsgObject's address; m_param2 the address of the bool result
// (pointers in ints, as the original: not portable to 64-bit).
void CGolfItemHandler::OnGetEnableTimeBooster(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;

	CGolfItemDoc* pDoc = GetDocument<CGolfItemDoc*>();
	if (pDoc == 0) {
		return;
	}

	*(bool*)pMsg->m_param2 = pDoc->GetEnableTimeBooster();
}

// golfitemhandler.cpp:216
void CGolfItemHandler::OnIncSpecialBox(void) {
	CGolfItemDoc* pDoc = GetDocument<CGolfItemDoc*>();

	pDoc->IncreaseSpecialBoxCount();
}

// golfitemhandler.cpp:224
void CGolfItemHandler::OnResetSpecialBox(void) {
	CGolfItemDoc* pDoc = GetDocument<CGolfItemDoc*>();

	pDoc->ResetSpecialBoxCount();

	CSharedDoc* pShared = WSingleton<CSharedDoc>::Instance();
	std::list<sItemInfo>::iterator it = pShared->m_itemInfoList.begin();
	while (it != pShared->m_itemInfoList.end() && (*it).m_typeId != 0x1A00015B) ++it;
	if (it == pShared->m_itemInfoList.end()) {
		pDoc->SetOwnSpecialBoxCount(0);
	} else {
		pDoc->SetOwnSpecialBoxCount((*it).m_count);
	}
}

// golfitemhandler.cpp:244
// param is the MsgObject's address; m_param2 the address of the int result
// (pointers in ints, as the original: not portable to 64-bit).
void CGolfItemHandler::OnGetSpecialBox(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;

	CGolfItemDoc* pDoc = GetDocument<CGolfItemDoc*>();
	*(int*)pMsg->m_param2 = pDoc->GetSpecialBoxCount();
}

// golfitemhandler.cpp:259
bool IsSpecialBoxArea(void) {
	unsigned char curMap = GetCurMap();
	if (curMap == 19) {
		return true;
	}
	return false;
}

// golfitemhandler.cpp:272
void CGolfItemHandler::OnCheckSomeMoreGetSpecialBox(void) {
	CTask* pTask = AfxGetTask();
	CGolfTask* pGolfTask = pTask->IsKindOf(&CGolfTask::m_RTTI) ? (CGolfTask*)pTask : 0;
	if (pGolfTask == 0) {
		return;
	}

	if (!pGolfTask->DetermineWhetherToAddActor_GroundItemMan()) {
		return;
	}

	if (GetCurMap() != 19) {
		return;
	}

	CGolfItemDoc* pDoc = GetDocument<CGolfItemDoc*>();

	int count = pDoc->GetOwnSpecialBoxCount() + pDoc->GetSpecialBoxCount();

	int remain = CalcRemainMoreGetSpecialBoxCount(count);
	IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(0x1A00015B);

	char* msg = MakeStr(WSingleton<_systemmsg::CMessageManager>::Instance()->GetMsg((_systemmsg::eMsgIdentifier)0x12C), pItem->m_name, 50);
	WSingleton<CChatMsg>::Instance()->AddChatMsg(msg, 0xFFFF3030, true, false);

	if (remain < 1) {
		msg = MakeStr(WSingleton<_systemmsg::CMessageManager>::Instance()->GetMsg((_systemmsg::eMsgIdentifier)0x12D), pItem->m_name);
		WSingleton<CChatMsg>::Instance()->AddChatMsg(msg, 0xFFFF3030, true, false);
	} else {
		float percent = (float)remain * 2.0f;
		if (80.0f <= percent) {
			msg = MakeStr(WSingleton<_systemmsg::CMessageManager>::Instance()->GetMsg((_systemmsg::eMsgIdentifier)0x12E), pDoc->GetOwnSpecialBoxCount(), remain);
			WSingleton<CChatMsg>::Instance()->AddChatMsg(msg, 0xFFFF3030, true, false);
		}
	}
}

// golfitemhandler.cpp:325
void CGolfItemHandler::OnRequestWizCityNoticeDlg(void) {
	m_pNoticeDlg = CreateForm<FrNoticeDlg>(g_pFresh->GetManager(), this, "notice_popup", 0);
	m_pNoticeDlg->LoadOnly("hat_event_explanation.jpg");
	m_pNoticeDlg->Open((FRESH_RESULT_CALLBACK)&CGolfItemHandler::OnCloseWizCityNoticeResult, 0x11);
}

// golfitemhandler.cpp:333
int CGolfItemHandler::CalcRemainMoreGetSpecialBoxCount(int count) {
	return 50 - count;
}

// golfitemhandler.cpp:340
bool CGolfItemHandler::OnCloseWizCityNoticeResult(int result, FrForm* pForm) {
	m_pNoticeDlg = 0;

	return true;
}

}
