// Original: d:\build\custom\temp\code\client\projectg\itembuffactor.cpp
#include <atlbase.h>	// stand-in for the precompiled header: _AtlWinModule initializer
#include "itembuffactor.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "commonutil.h"
#include "../shared/ItemManager.h"
#include "SharedDoc.h"
#include "ToolTip.h"
#include "frlistbox.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "frform.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "Packet.h"
#include "cardsystem.h"
#include "TaskManager.h"
#include "UccManager.h"
#include "CardDlg.h"

#include <algorithm>

extern unsigned long g_CurrentTime;

class FrForm;

namespace {

FrForm* OpenResultDlg(unsigned long typeId, FrCmdTarget* pTarget);
FrForm* OpenConfirmDlg(unsigned long typeId, FrCmdTarget* pTarget, FRESH_RESULT_CALLBACK callback);

// The name of each buff ability (specialcardavility), "" where it has none
// (entries 0 and 1 are null: the original's initializer leaves them zero).
// Entry 2 through a const variable: the table is initialized by code, as in
// the original (docs/tough_problems/010).
const char* const s_expGainName = "\xC8\xB9\xB5\xE6 \xB0\xE6\xC7\xE8\xC4\xA1";

const char* s_abilityName[] = {
	0,
	0,
	s_expGainName,
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
};

// itembuffactor.cpp:62
const char* MakeBuffMessage(const char* name, int period, specialcardavility ability, int value) {
	char* periodText = MakeStr("\xB1\xE2\xB0\xA3 : %d\xBA\xD0", period);
	char* effectText = MakeStr("\xC8\xBF\xB0\xFA : %s +%d%%", s_abilityName[ability], value);
	return MakeStr("%s\n\\c0xff0000ff\\c%s\n%s\\c0xff000000\\c", name, periodText, effectText);
}

// itembuffactor.cpp:71
FrForm* OpenResultDlg(unsigned long typeId, FrCmdTarget* pTarget) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), pTarget, "notify_item", 0);

	WRect rect = pForm->GetRect();
	rect.w += 55.0f;
	rect.h += 40.0f;

	pForm->SetRect(rect);

	FrButton* pOk = DYNAMIC_CAST(FrButton, pForm->FindChildByName("ok"));
	if (pOk) {
		WRect okRect = pOk->GetRect();
		okRect.x = (rect.w - okRect.w) * 0.5f;
		okRect.y = rect.h - 80.0f;
		pOk->SetClientRect(okRect);
	}

	FrEdit* pMessage = DYNAMIC_CAST(FrEdit, pForm->FindChildByName("message"));
	if (pMessage) {
		WRect messageRect = pMessage->GetRect();
		messageRect.w += 55.0f;
		messageRect.h += 40.0f;
		pMessage->SetRect(messageRect);
	}

	IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(typeId);

	specialcardavility ability;
	int value;
	int period;
	int unused;
	const char* message = "";

	if (ItemManager()->GetBuffProperty(typeId, ability, value, period, unused)) {
		// "You used the item and got the following effect!"
		message = MakeBuffMessage("\xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBB \xBB\xE7\xBF\xEB\xC7\xCF\xBF\xA9 \xB4\xD9\xC0\xBD \xC8\xBF\xB0\xFA\xB8\xA6 \xBE\xF2\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9!", period, ability, value);
	}

	FrArea* pIcon = DYNAMIC_CAST(FrArea, pForm->FindChildByName("itemicon"));
	FrEdit* pName = DYNAMIC_CAST(FrEdit, pForm->FindChildByName("itemname"));

	if (pIcon) {
		if (pItem) {
			pIcon->SetBgImg(pItem->m_icon);
		} else {
			pIcon->SetBgImg("hide");
		}
	}

	if (pName) {
		if (pItem) {
			pName->AddText(pItem->m_name, false, true);
		} else {
			pName->AddText("none", false, true);
		}
		pName->AddText("\n", false, true);
	}

	pForm->SetMessage(message, false);
	pForm->Open(0, 1);

	return pForm;
}

// itembuffactor.cpp:159
FrForm* OpenConfirmDlg(unsigned long typeId, FrCmdTarget* pTarget, FRESH_RESULT_CALLBACK callback) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), pTarget, "notify_item_yesno", 0);

	WRect rect = pForm->GetRect();
	rect.w += 55.0f;
	rect.h += 40.0f;

	pForm->SetRect(rect);

	FrButton* pYes = DYNAMIC_CAST(FrButton, pForm->FindChildByName("yes"));
	if (pYes) {
		WRect yesRect = pYes->GetRect();
		yesRect.x = 55.0f;
		yesRect.y = rect.h - 90.0f;
		pYes->SetClientRect(yesRect);
	}

	FrButton* pNo = DYNAMIC_CAST(FrButton, pForm->FindChildByName("no"));
	if (pNo) {
		WRect noRect = pNo->GetRect();
		noRect.x = rect.w - noRect.w - 55.0f;
		noRect.y = rect.h - 90.0f;
		pNo->SetClientRect(noRect);
	}

	FrEdit* pMessage = DYNAMIC_CAST(FrEdit, pForm->FindChildByName("message"));
	if (pMessage) {
		WRect messageRect = pMessage->GetRect();
		messageRect.w += 55.0f;
		messageRect.h += 40.0f;
		pMessage->SetRect(messageRect);
	}

	IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(typeId);

	specialcardavility ability;
	int value;
	int period;
	int unused;
	const char* message = "";

	if (ItemManager()->GetBuffProperty(typeId, ability, value, period, unused)) {
		// "Do you want to use this item?"
		message = MakeBuffMessage("\xC0\xCC \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBB \xBB\xE7\xBF\xEB\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", period, ability, value);
	}

	FrArea* pIcon = DYNAMIC_CAST(FrArea, pForm->FindChildByName("itemicon"));
	FrEdit* pName = DYNAMIC_CAST(FrEdit, pForm->FindChildByName("itemname"));

	if (pIcon) {
		if (pItem) {
			pIcon->SetBgImg(pItem->m_icon);
		} else {
			pIcon->SetBgImg("hide");
		}
	}

	if (pName) {
		if (pItem) {
			pName->AddText(pItem->m_name, false, true);
		} else {
			pName->AddText("none", false, true);
		}
		pName->AddText("\n", false, true);
	}

	pForm->SetMessage(message, false);
	pForm->Open(callback, 1);

	return pForm;
}

}

// itembuffactor.cpp:253 (run-time type and factory registration of CItemBuff)
const WRTTI CItemBuff::m_RTTI("CItemBuff", &IActor::m_RTTI);

IObject* CItemBuffMakeInstance(void) {
	return new CItemBuff;
}

struct __impCItemBuff {
	__impCItemBuff(void) {
		ObjectFactory().AddObjectFunctor(CItemBuffMakeInstance, "CItemBuff");
		ActorFactory().AddActorClass("CItemBuff");
	}
};
static __impCItemBuff s_registerCItemBuff;

// itembuffactor.cpp:256
std::list<sSCardAvilityPeriodInfo> CItemBuff::ms_BuffItemInfoList;

// itembuffactor.cpp:259
CItemBuff::CItemBuff(void) {
	m_pButtonOver = 0;
	m_pButtonNormal = 0;
	m_useItemTypeId = 0;
}

// itembuffactor.cpp:265
void CItemBuff::OnLoad(void) {
	m_pButtonNormal = g_pFresh->RegisterBitmap("trade_item_edit_bn_normal");
	m_pButtonOver = g_pFresh->RegisterBitmap("trade_item_edit_bn_over");
	m_buttonRect = WRect(103.0f, 66.0f, m_pButtonNormal->Width(), m_pButtonNormal->Height());
}

// itembuffactor.cpp:272
void CItemBuff::HandleMsg(const MsgObject& msg) {
	if ((unsigned long)msg.m_reserved > g_CurrentTime) {
		StoreDelayedMsg(msg);
		return;
	}

	switch (msg.m_msg) {
	case 0x246:
		HandleBuffItemSubMsg(msg);
		break;
	case 0x26:
		OpenResultDlg(msg.m_param1, this);
		break;
	}
}

// itembuffactor.cpp:302
void CItemBuff::HandleBuffItemSubMsg(const MsgObject& msg) {
	switch (msg.m_param1) {
	case 0: {
		bool* pIsBuff = (bool*)msg.m_param2;
		if (ItemManager()->IsItemBuff(msg.m_param5)) {
			*pIsBuff = true;
		}
		if (*pIsBuff) {
			DrawMiniButton((FrListItem*)msg.m_param3, *(const WPoint*)msg.m_param4);
		}
		break;
	}
	case 1: {
		bool* pIsBuff = (bool*)msg.m_param2;
		if (ItemManager()->IsItemBuff(msg.m_param5)) {
			*pIsBuff = true;
		}
		if (*pIsBuff) {
			ButtonDown((FrListItem*)msg.m_param3, *(const WPoint*)msg.m_param4);
		}
		break;
	}
	case 2: {
		bool* pHandled = (bool*)msg.m_param2;
		CIconToolTip* pToolTip = CIconToolTip::GetInstance();
		if (pToolTip->GetCurTipIndex() == 0x46) {
			*pHandled = true;
		}
		break;
	}
	}
}

// itembuffactor.cpp:357
void CItemBuff::DrawMiniButton(FrListItem* pItem, const WPoint& pt) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pItem->underCursor) {
		WRect rc(pItem->pos.x + m_buttonRect.x, pItem->pos.y + m_buttonRect.y, m_buttonRect.w, m_buttonRect.h);
		if (pt.x >= rc.x && pt.x <= rc.x + rc.w && pt.y >= rc.y && rc.y + rc.h >= pt.y) {
			pGDI->DrawTexture(m_pButtonOver, WRect(m_buttonRect.x + pItem->pos.x, pItem->pos.y + m_buttonRect.y, m_buttonRect.w, m_buttonRect.h), 0xFFFFFFFF, 0);
			CIconToolTip::GetInstance()->SetToolTip((eBarButton)0x46);
			return;
		}
	}

	pGDI->DrawTexture(m_pButtonNormal, WRect(pItem->pos.x + m_buttonRect.x, pItem->pos.y + m_buttonRect.y, m_buttonRect.w, m_buttonRect.h), 0xFFFFFFFF, 0);
}

// itembuffactor.cpp:369
void CItemBuff::ButtonDown(FrListItem* pItem, const WPoint& pt) {
	if (pItem) {
		WRect rc(pItem->pos.x + m_buttonRect.x, pItem->pos.y + m_buttonRect.y, m_buttonRect.w, m_buttonRect.h);
		if (pt.x >= rc.x && pt.x <= rc.x + rc.w && pt.y >= rc.y && rc.y + rc.h >= pt.y) {
			UseBuffItem((sItemInfo*)pItem->pData);
		}
	}
}

// itembuffactor.cpp:378
void CItemBuff::UseBuffItem(sItemInfo* pItem) {
	if (pItem == 0) {
		return;
	}
	IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(pItem->m_typeId);
	if (pCommon == 0) {
		return;
	}

	m_useItemTypeId = pItem->m_typeId;

	bool compatible = true;
	unsigned long replaceTypeId = 0;
	IsCompatibleBuff(m_useItemTypeId, ms_BuffItemInfoList, compatible, replaceTypeId);
	IsCompatibleBuff(m_useItemTypeId, CSharedDoc::Instance()->m_cardPeriodList[0], compatible, replaceTypeId);

	if (!compatible) {
		IFF_ITEM_COMMON* pReplace = ItemManager()->FindCommonItem(replaceTypeId);
		const char* text;
		if (pReplace) {
			text = MakeStr("%s\n%s", "\xB4\xD9\xC0\xBD \xBE\xC6\xC0\xCC\xC5\xDB\xB0\xFA \xC7\xD4\xB2\xB2 \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", pReplace->m_name);
		} else {
			text = MakeStr("%s", "\xC7\xD4\xB2\xB2 \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xCC \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.");
		}
		GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)text, 0, 0, 0, 0);
		m_useItemTypeId = 0;
	} else {
		OpenConfirmDlg(pItem->m_typeId, this, static_cast<FRESH_RESULT_CALLBACK>(&CItemBuff::OnUseBuffItemDlgResult));
	}
}

// itembuffactor.cpp:420
void CItemBuff::IsCompatibleBuff(unsigned long typeId, const std::list<sSCardAvilityPeriodInfo>& buffList, bool& compatible, unsigned long& replaceTypeId) {
	for (std::list<sSCardAvilityPeriodInfo>::const_iterator it = buffList.begin(); compatible && it != buffList.end(); ++it) {
		if (!ItemManager()->IsCompatibleBuff(typeId, it->typeId)) {
			replaceTypeId = it->typeId;
			compatible = false;
		}
	}
}

// itembuffactor.cpp:433
bool CItemBuff::OnUseBuffItemDlgResult(int ret, FrForm* pForm) {
	if (ret == 1) {
		if (m_useItemTypeId != 0) {
			GetActor("RealMyRoom") << MsgObject(0, 1, 0, 0, 0, 0, 0);

			WSendPacket packet((enumClientPacket)0xDA);
			packet.Encode4(m_useItemTypeId);
			packet.Send(SEND_TO_DEFAULT);
		}
	}
	m_useItemTypeId = 0;

	return true;
}

// itembuffactor.cpp:452
void CItemBuff::ResetBuffList(void) {
	ms_BuffItemInfoList.clear();
}

// itembuffactor.cpp:457
void CItemBuff::ProcessPacket(WReceivedPacket& packet) {
	if (packet.Decode2() == 0x186) {
		switch (packet.Decode4()) {
		case 0: {
			int count = packet.Decode1();
			for (int i = 0; i < count; i++) {
				sSCardAvilityPeriodInfo info;
				packet.DecodeBuffer(&info, sizeof(info));
				AddBuffItem(info);
			}
			break;
		}
		case 1:
			RemoveBuffItem(packet.Decode4());
			break;
		case 2: {
			AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 2, 0, 0, 0, 0, 0);

			unsigned long typeId = 0;
			if (packet.Decode4() == 1) {
				packet.Decode4();

				sSCardAvilityPeriodInfo info;
				packet.DecodeBuffer(&info, sizeof(info));
				AddBuffItem(info);
				typeId = info.typeId;

				CCardManager::Instance()->SetSPCardAlarm(true);
				AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x219, typeId, 1, 0, 0, 0);
			}
			AfxGetTask()->GetActor("ItemBuff") << MsgObject(0, 0x26, typeId, 0, 0, 0, 0);
			break;
		}
		}
	}
}

// itembuffactor.cpp:520
struct sAbilityFinder {
	sAbilityFinder(unsigned long typeId);
	bool operator()(sSCardAvilityPeriodInfo info);

	unsigned long m_typeId;   // 0x0
};

sAbilityFinder::sAbilityFinder(unsigned long typeId) {
	m_typeId = typeId;
}

// itembuffactor.cpp:526
bool sAbilityFinder::operator()(sSCardAvilityPeriodInfo info) {
	return m_typeId == info.typeId;
}

// itembuffactor.cpp:536
void CItemBuff::AddBuffItem(sSCardAvilityPeriodInfo& info) {
	if (!ItemManager()->IsItemBuff(info.typeId)) {
		return;
	}

	std::list<sSCardAvilityPeriodInfo>::iterator it = std::find_if(ms_BuffItemInfoList.begin(), ms_BuffItemInfoList.end(), sAbilityFinder(info.typeId));
	if (it != ms_BuffItemInfoList.end()) {
		it->endTime = info.endTime;
		it->startTime.wSecond = HIWORD(g_CurrentTime);
		it->startTime.wMilliseconds = LOWORD(g_CurrentTime);
	} else {
		info.startTime.wSecond = HIWORD(g_CurrentTime);
		info.startTime.wMilliseconds = LOWORD(g_CurrentTime);
		ms_BuffItemInfoList.push_back(info);
	}

	CCardManager::Instance()->UpdateCardBuffDlg();
}

// itembuffactor.cpp:561
void CItemBuff::RemoveBuffItem(unsigned long typeId) {
	std::list<sSCardAvilityPeriodInfo>::iterator it = ms_BuffItemInfoList.begin();
	while (it != ms_BuffItemInfoList.end()) {
		if (it->typeId == typeId) {
			it = ms_BuffItemInfoList.erase(it);
		} else {
			++it;
		}
	}

	CCardManager::Instance()->UpdateCardBuffDlg();
}

// itembuffactor.cpp:580
void CItemBuff::UpdateBuffDlg(FrSPCardBuffDlg* dlg) {
	if (dlg) {
		dlg->SetBuffItem(CSharedDoc::Instance()->m_cardPeriodList[0], true);
		dlg->SetBuffItem(ms_BuffItemInfoList, false);
	}
}
