// Original: d:\build\custom\temp\code\client\projectg\s5\rentalextensiondlg.cpp
#include "s5/RentalExtensionDlg.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "UccManager.h"
#include "../shared/classdefine.h"
#include "../shared/globalgamedefine.h"
#include "bitmap.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "commonutil.h"
#include "fredit.h"
#include "frbutton.h"
#include "../shared/ItemManager.h"

namespace S5 {

// rentalextensiondlg.cpp:15 (run-time type and factory registration of FrRentalExtensionDlg)
const WRTTI FrRentalExtensionDlg::m_RTTI("FrRentalExtensionDlg", &FrForm::m_RTTI);

IObject* FrRentalExtensionDlgMakeInstance(void) {
	return new FrRentalExtensionDlg;
}

struct __sFrRentalExtensionDlg {
	__sFrRentalExtensionDlg(void) {
		ObjectFactory().AddObjectFunctor(FrRentalExtensionDlgMakeInstance, "FrRentalExtensionDlg");
	}
};
static __sFrRentalExtensionDlg s_registerFrRentalExtensionDlg;

// rentalextensiondlg.cpp:18
const sFRESH_MSGMAP* FrRentalExtensionDlg::GetMessageMap(void) const {
	return &FrRentalExtensionDlg::_MsgMap;
}

sFRESH_MSGMAP FrRentalExtensionDlg::_MsgMap = { &FrForm::_MsgMap, &FrRentalExtensionDlg::_MsgEntries[0] };

sFRESH_ENTRY FrRentalExtensionDlg::_MsgEntries[] = {
	// rentalextensiondlg.cpp:20
	{ "ar_dialog", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrRentalExtensionDlg::OnDialogAreaDraw },
	// rentalextensiondlg.cpp:21
	{ "bt_extension", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrRentalExtensionDlg::OnExtensionBtnUp },
	// rentalextensiondlg.cpp:22
	{ "bt_destroy", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrRentalExtensionDlg::OnDestroyBtnUp },
	// rentalextensiondlg.cpp:23
	{ "bt_cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrRentalExtensionDlg::OnCancelBtnUp },

	// rentalextensiondlg.cpp:25
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// rentalextensiondlg.cpp:29
FrRentalExtensionDlg::FrRentalExtensionDlg(void) {
	m_itemInfo = 0;
}

// rentalextensiondlg.cpp:41
FrRentalExtensionDlg::~FrRentalExtensionDlg(void) {
	if (m_itemInfo) { delete m_itemInfo; m_itemInfo = 0; }
}

// rentalextensiondlg.cpp:46
void FrRentalExtensionDlg::Initialize(const sItemInfo* itemInfo) {
	m_itemInfo = new sItemInfo;
	*m_itemInfo = *itemInfo;

	FrEdit* pName = DYNAMIC_CAST(FrEdit, FindChildByName("ed_name"));
	if (pName) {
		pName->AddText(ItemManager()->GetItemName(m_itemInfo->m_typeId), false, true);
	}

	FrEdit* pDesc = DYNAMIC_CAST(FrEdit, FindChildByName("ed_desc"));
	if (pDesc) {
		pDesc->AddText("\\c0xffff0000\\c\xc7\xd8\xb4\xe7 \xc0\xc7\xbb\xf3\xc0\xc7 \xbb\xe7\xbf\xeb\xb1\xe2\xb0\xa3\xc0\xcc \xb8\xb8\xb7\xe1\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9.", false, true);

		pDesc->AddText(MakeStr("\\c0xff000000\\c- \xbb\xe7\xbf\xeb \xb1\xe2\xb0\xa3\xc0\xbb \xbf\xac\xc0\xe5\xc7\xcf\xbd\xc7 \xb0\xe6\xbf\xec \xc1\xef\xbd\xc3 %s \xc2\xf7\xb0\xa8 \xb5\xcb\xb4\xcf\xb4\xd9.", "\xc6\xce\xc0\xcc"), false, true);
	}

	IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(m_itemInfo->m_typeId);
	FrEdit* pPrice = DYNAMIC_CAST(FrEdit, FindChildByName("ed_price"));
	if (pPrice && pPart) {
		pPrice->AddText(MakeStr("\xb1\xe2\xb0\xa3 \xbf\xac\xc0\xe5 \xba\xf1\xbf\xeb : %d%s(%d\xc0\xcf)", pPart->m_rentalPrice, "\xc6\xce", 7), false, true);
	}

	FrButton* pExtension = DYNAMIC_CAST(FrButton, FindChildByName("bt_extension"));
	FrButton* pDestroy = DYNAMIC_CAST(FrButton, FindChildByName("bt_destroy"));
	if (pPart == 0) {
		if (pExtension) {
			pExtension->Enable(false);
			pExtension->SetToolTipText("\xb4\xeb\xbf\xa9 \xb1\xe2\xb4\xc9 \xbb\xe7\xbf\xeb \xba\xd2\xb0\xa1");
		}
		if (pDestroy) {
			pDestroy->Enable(false);
			pDestroy->SetToolTipText("\xb4\xeb\xbf\xa9 \xb1\xe2\xb4\xc9 \xbb\xe7\xbf\xeb \xba\xd2\xb0\xa1");
		}
	} else {
		if (pExtension) {
			if (WSingleton<CSharedDoc>::Instance()->m_myPang < pPart->m_rentalPrice) {
				pExtension->Enable(false);
				pExtension->SetToolTipText("\xb1\xe2\xb0\xa3 \xbf\xac\xc0\xe5\xbf\xa1 \xc7\xca\xbf\xe4\xc7\xd1 \xc6\xce\xc0\xcc \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.");
				pExtension->EnableToolTip(true);
			}
		}
		if (pDestroy) {
			for (int i = 0; i < 12; i++) {
				if (m_itemInfo->m_cardSlots[i] != 0) {
					pDestroy->Enable(false);
					pDestroy->SetToolTipText("\xc4\xab\xb5\xe5 \xc3\xdf\xc3\xe2\xbe\xd7\xc0\xb8\xb7\xce \xc4\xab\xb5\xe5\xb8\xa6 \xb8\xd5\xc0\xfa \xc3\xdf\xc3\xe2\xc7\xd8\xbe\xdf \xc7\xd5\xb4\xcf\xb4\xd9.");
					pDestroy->EnableToolTip(true);
					break;
				}
			}
		}
	}
}

// rentalextensiondlg.cpp:142
std::string FrRentalExtensionDlg::SetChildWindowPos(const tweaker_param& param) {
	FrWnd* pWnd = FindChildByName(param[0].to_cstr());
	if (pWnd) {
		float x = param[1].to_float();
		float y = param[2].to_float();

		WRect rect(m_rect.x + x, m_rect.y + y, pWnd->GetRect().w, pWnd->GetRect().h);

		pWnd->SetRect(rect);

		return "changed child window position.";
	}
	return "not found child window";
}

// rentalextensiondlg.cpp:159
std::string FrRentalExtensionDlg::GetChildWindowPos(const tweaker_param& param) {
	FrWnd* pWnd = FindChildByName(param[0].to_cstr());
	if (pWnd) {
		WRect rect = pWnd->GetRect();
		return MakeStr("x : %f, y : %f", rect.x - m_rect.x, rect.y - m_rect.y);
	}
	return "not found child window";
}

// rentalextensiondlg.cpp:174
void FrRentalExtensionDlg::OnDialogAreaDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(m_itemInfo->m_typeId);
	if (pItem) {
		const Bitmap* pBitmap = g_pFresh->GetBitmap(pItem->m_icon);
		pGDI->DrawTexture(pBitmap, WRect(m_rect.x + 35.0f, m_rect.y + 25.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}
}

// rentalextensiondlg.cpp:185
void FrRentalExtensionDlg::OnExtensionBtnUp(void) {
	FrForm::Close(FrYES, true);
}

// rentalextensiondlg.cpp:191
void FrRentalExtensionDlg::OnDestroyBtnUp(void) {
	FrForm::Close(FrNO, true);
}

// rentalextensiondlg.cpp:196
void FrRentalExtensionDlg::OnCancelBtnUp(void) {
	FrForm::Close(FrCANCEL, true);
}

} // namespace S5
