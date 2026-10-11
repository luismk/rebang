// Original: d:\build\custom\temp\code\client\projectg\uccsetinfodlg.cpp
// This object also emits hatmanager.h:38-43 (the FilteringNick reasons).
#define PG_HATMANAGER_FILE_STRINGS
#include <atlbase.h>	// stand-in for the precompiled header: _AtlWinModule initializer
#include <string.h>
#include <string>

#include "UccSetInfoDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "hatmanager.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "actor.h"
#include "UccDrawDlg.h"
#include "UccBase.h"
#include "UccLibrary.h"
#include "commonutil.h"
#include "../shared/globalgamedefine.h"
#include "../shared/ItemManager.h"
#include "../shared/classdefine.h"
#include "frgraphicinterface.h"
#include "fresh.h"

static const char* const s_nickReasonEmpty = "\xB4\xEB\xC8\xAD\xB8\xED\xC0\xBB \xC0\xD4\xB7\xC2\xC7\xD8 \xC1\xD6\xBC\xBC\xBF\xE4. ";
static const char* const s_nickReasonLength = "\xB4\xEB\xC8\xAD\xB8\xED\xC0\xBA \\c0xffff0000\\c\xBF\xB5\xB9\xAE 4 - 16 \xB1\xDB\xC0\xDA\\c0xff000000\\c \xB6\xC7\xB4\xC2 \\c0xffff0000\\c\xC7\xD1\xB1\xDB 2 - 8\xB1\xDB\xC0\xDA\\c0xff000000\\c\xC0\xC7 \xB1\xE6\xC0\xCC\xB8\xA6 \xB0\xA1\xC1\xAE\xBE\xDF \xC7\xD5\xB4\xCF\xB4\xD9.";
static const char* const s_nickReasonSpace = "\xB4\xEB\xC8\xAD\xB8\xED\xC0\xBA \\c0xffff0000\\c\xB0\xF8\xB9\xE9\xB9\xAE\xC0\xDA\\c0xff000000\\c\xB8\xA6 \xC6\xF7\xC7\xD4\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.";
static const char* const s_nickReasonBadChar = "\xB4\xEB\xC8\xAD\xB8\xED\xBF\xA1\xBC\xAD \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xB9\xAE\xC0\xDA\xB8\xA6 \xC6\xF7\xC7\xD4\xC7\xCF\xB0\xED \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.";
static const char* const s_nickReasonBadWord = "\xB4\xEB\xC8\xAD\xB8\xED\xC0\xB8\xB7\xCE \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xB4\xDC\xBE\xEE\xC0\xD4\xB4\xCF\xB4\xD9.";
static const char* const s_nickReasonHasBadWord = "\xB4\xEB\xC8\xAD\xB8\xED\xC0\xB8\xB7\xCE \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xB4\xDC\xBE\xEE\xB8\xA6 \xC6\xF7\xC7\xD4\xC7\xCF\xB0\xED \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.";

// uccsetinfodlg.cpp:10 (run-time type and factory registration of FrUccSetInfoDlg)
const WRTTI FrUccSetInfoDlg::m_RTTI("FrUccSetInfoDlg", &FrForm::m_RTTI);

IObject* FrUccSetInfoDlgMakeInstance(void) {
	return new FrUccSetInfoDlg;
}

struct __sFrUccSetInfoDlg {
	__sFrUccSetInfoDlg(void) {
		ObjectFactory().AddObjectFunctor(FrUccSetInfoDlgMakeInstance, "FrUccSetInfoDlg");
	}
};
static __sFrUccSetInfoDlg s_registerFrUccSetInfoDlg;

// uccsetinfodlg.cpp:12
const sFRESH_MSGMAP* FrUccSetInfoDlg::GetMessageMap(void) const {
	return &FrUccSetInfoDlg::_MsgMap;
}

sFRESH_MSGMAP FrUccSetInfoDlg::_MsgMap = { &FrForm::_MsgMap, &FrUccSetInfoDlg::_MsgEntries[0] };

sFRESH_ENTRY FrUccSetInfoDlg::_MsgEntries[] = {
	// uccsetinfodlg.cpp:14
	{ "name", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccSetInfoDlg::OnNameInit },
	{ "name", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrUccSetInfoDlg::OnNameEnterKey },
	{ "ok_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccSetInfoDlg::OnOkBtnInit },
	{ "ok_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUccSetInfoDlg::OnOkBtnUp },
	{ "cancel_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccSetInfoDlg::OnCancelBtnInit },
	{ "cancel_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUccSetInfoDlg::OnCancelBtnUp },
	{ "icon_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccSetInfoDlg::OnIconAreaInit },
	{ "icon_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccSetInfoDlg::OnIconAreaOwnerDraw },

	// uccsetinfodlg.cpp:23
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// uccsetinfodlg.cpp:26
FrUccSetInfoDlg::FrUccSetInfoDlg(void) {
	m_pParent = 0;
}

// uccsetinfodlg.cpp:31
void FrUccSetInfoDlg::OnNameInit(intptr_t param) {
	m_pName = DYNAMIC_CAST(FrEdit, param);
}

// uccsetinfodlg.cpp:35
bool FrUccSetInfoDlg::OnNameEnterKey(int param) {
	OnOkBtnUp();
	return true;
}

// uccsetinfodlg.cpp:41
void FrUccSetInfoDlg::OnOkBtnInit(intptr_t param) {
	m_pOkBtn = DYNAMIC_CAST(FrButton, param);
}

// uccsetinfodlg.cpp:46
void FrUccSetInfoDlg::OnOkBtnUp(void) {
	if (m_pParent == 0)
		return;

	char name[208];
	strcpy(name, m_pName->GetLine(1, false));

	const char* reason = WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringNick(name);

	if (reason) {
		if (strcmp(reason, s_nickReasonEmpty) == 0) {
			AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)"\xC0\xC7\xBB\xF3 \xC0\xCC\xB8\xA7\xC0\xBB \xC0\xD4\xB7\xC2\xC7\xD8\xC1\xD6\xBC\xBC\xBF\xE4.", 0, 0, 0, 0);
		} else if (strcmp(reason, s_nickReasonLength) == 0) {
			AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)"\xC0\xC7\xBB\xF3 \xC0\xCC\xB8\xA7\xC0\xBA \\c0xffff0000\\c\xBF\xB5\xB9\xAE 4 - 16 \xB1\xDB\xC0\xDA\\c0xff000000\\c \xB6\xC7\xB4\xC2 \\c0xffff0000\\c\xC7\xD1\xB1\xDB 2 - 8\xB1\xDB\xC0\xDA\\c0xff000000\\c\xC0\xC7 \xB1\xE6\xC0\xCC\xB8\xA6 \xB0\xA1\xC1\xAE\xBE\xDF \xC7\xD5\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
		} else if (strcmp(reason, s_nickReasonSpace) == 0) {
			AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)"\xC0\xC7\xBB\xF3 \xC0\xCC\xB8\xA7\xC0\xBA \\c0xffff0000\\c\xB0\xF8\xB9\xE9\xB9\xAE\xC0\xDA\\c0xff000000\\c\xB8\xA6 \xC6\xF7\xC7\xD4\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
		} else if (strcmp(reason, s_nickReasonBadChar) == 0) {
			AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)"\xC0\xC7\xBB\xF3 \xC0\xCC\xB8\xA7\xC0\xB8\xB7\xCE \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xB9\xAE\xC0\xDA\xB8\xA6 \xC6\xF7\xC7\xD4\xC7\xCF\xB0\xED \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
		} else if (strcmp(reason, s_nickReasonBadWord) == 0 || strcmp(reason, s_nickReasonHasBadWord) == 0) {
			AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)"\xC0\xC7\xBB\xF3 \xC0\xCC\xB8\xA7\xBF\xA1 \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xC7\xA5\xC7\xF6\xC0\xCC \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
		}
		return;
	}

	std::string strName = m_pName->GetLine(1, false);
	strName = strName.substr(0, (int)strName.size() > 40 ? 40 : strName.size());
	m_pParent->SetName(strName.c_str());

	OnOK();
}

// uccsetinfodlg.cpp:98
void FrUccSetInfoDlg::OnCancelBtnInit(intptr_t param) {
	m_pCancelBtn = DYNAMIC_CAST(FrButton, param);
}

// uccsetinfodlg.cpp:103
void FrUccSetInfoDlg::OnCancelBtnUp(void) {
	OnCancel();
}

// uccsetinfodlg.cpp:108
void FrUccSetInfoDlg::OnIconAreaInit(intptr_t param) {
	m_pIconArea = DYNAMIC_CAST(FrArea, param);
}

// uccsetinfodlg.cpp:113
void FrUccSetInfoDlg::OnIconAreaOwnerDraw(int param) {
	FrGraphicInterface* pGDI = GetGDI();
	if (pGDI == 0)
		return;

	WPoint pos(m_rect.x, m_rect.y);

	if (m_pParent) {
		sItemInfo* pItemInfo = m_pParent->m_pItemInfo;

		if (pItemInfo) {
			IFF_STRUCT::sChar* pChar = ItemManager()->FindChar(((pItemInfo->m_typeId >> 18) & 0xFF) | 0x4000000);
			if (pChar) {
				const Bitmap* pBitmap = g_pFresh->GetBitmap(MakeStr("s_%s", pChar->m_icon));

				if (pBitmap) {
					pGDI->DrawTexture(pBitmap, WRect(pos.x + 60.0f, pos.y + 68.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
				}
			}
		}
	}

	if (m_icon.Width() > 0 && m_icon.Height() > 0) {
		pGDI->DrawTexture(&m_icon, WRect(pos.x + 66.0f, pos.y + 70.0f, m_icon.Width(), m_icon.Height()), 0xFFFFFFFF, 0);
	}
}

// uccsetinfodlg.cpp:163
void FrUccSetInfoDlg::SetParent(FrUccDrawDlg* pParent) {
	m_pParent = pParent;

	Bitmap* pTexture = pParent->m_pUccBase[0]->GetTexture();
	Bitmap* pMask = pParent->m_pUccBase[0]->GetMask();

	m_icon.Create(64, 84, 32);
	CreateItemThumbnail(pTexture, pMask, &m_icon);
}
