// Original: d:\build\custom\temp\code\client\projectg\emoticondlg.cpp
#include <windows.h>
#include <mmsystem.h>
#include <string.h>

#include "EmoticonDlg.h"
#include "ObjectFactory.h"
#include "InputManager.h"
#include "Localize.h"
#include "commonutil.h"
#include "fremoticon.h"
#include "fresh.h"
#include "frframe.h"
#include "frlistbox.h"
#include "frwndmanager.h"

// Emoticon code delimiters of the IsLocalCountry(30) build ("-" marks the
// flipped icon, drawn when shift is held). Reading them from a const variable
// makes the static locals below dynamically initialized (guard bits), as in
// the original.
static const char* const EMOTICON_OPEN = "(";
static const char* const EMOTICON_CLOSE = ")";
static const char* const EMOTICON_FLIP = "-";

// emoticondlg.cpp:19 (run-time type and factory registration of FrEmoticonDlg)
const WRTTI FrEmoticonDlg::m_RTTI("FrEmoticonDlg", &FrForm::m_RTTI);

IObject* FrEmoticonDlgMakeInstance(void) {
	return new FrEmoticonDlg;
}

struct __sFrEmoticonDlg {
	__sFrEmoticonDlg(void) {
		ObjectFactory().AddObjectFunctor(FrEmoticonDlgMakeInstance, "FrEmoticonDlg");
	}
};
static __sFrEmoticonDlg s_registerFrEmoticonDlg;

// emoticondlg.cpp:21
const sFRESH_MSGMAP* FrEmoticonDlg::GetMessageMap(void) const {
	return &FrEmoticonDlg::_MsgMap;
}

sFRESH_MSGMAP FrEmoticonDlg::_MsgMap = { &FrForm::_MsgMap, &FrEmoticonDlg::_MsgEntries[0] };

sFRESH_ENTRY FrEmoticonDlg::_MsgEntries[] = {
	// emoticondlg.cpp:23
	{ "list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEmoticonDlg::OnListInit },
	{ "list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEmoticonDlg::OnListOwnerDraw },
	{ "list", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrEmoticonDlg::OnListBtnUp },

	// emoticondlg.cpp:27
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// emoticondlg.cpp:33
FrEmoticonDlg::FrEmoticonDlg(void) {
	m_pList = 0;
	m_szSelectedIcon[0] = 0;
}

// emoticondlg.cpp:40
FrEmoticonDlg::~FrEmoticonDlg(void) {
}

// emoticondlg.cpp:45
void FrEmoticonDlg::OnOK(void) {
	FrEmoticon* pEmoticon = g_pFresh->GetManager()->GetEmoticon();

	if (pEmoticon && m_pList->GetItemUnderCursor()) {
		const char* name = pEmoticon->GetIconName((intptr_t)m_pList->GetItemUnderCursor()->pData);
		if (name) {
			if (IsLocalCountry(30)) {
				static const char* szOpen = EMOTICON_OPEN;
				static const char* szClose = EMOTICON_CLOSE;

				static const char* szFlip = EMOTICON_FLIP;

				strcpy(m_szSelectedIcon, szOpen);
				if (g_input->Get("LSHIFT", false) || g_input->Get("RSHIFT", false))
					strcat(m_szSelectedIcon, szFlip);
				strcat(m_szSelectedIcon, name);
				strcat(m_szSelectedIcon, szClose);
			} else {
				strcpy(m_szSelectedIcon, "(");
				if (g_input->Get("LSHIFT", false) || g_input->Get("RSHIFT", false))
					strcat(m_szSelectedIcon, "-");
				strcat(m_szSelectedIcon, name);
				strcat(m_szSelectedIcon, ")");
			}
		}
	}

	FrForm::OnOK();
}

// emoticondlg.cpp:83
bool FrEmoticonDlg::OnInit(void) {
	if (m_pBaseFrm)
		m_pBaseFrm->UseEmoAtDesc(false);

	return true;
}

// emoticondlg.cpp:92
void FrEmoticonDlg::OnListInit(intptr_t param) {
	m_pList = DYNAMIC_CAST(FrListBox, param);
	if (!m_pList)
		return;

	FrEmoticon* pEmoticon = g_pFresh->GetManager()->GetEmoticon();
	if (!pEmoticon)
		return;

	int count = pEmoticon->GetIconNum();

	for (int i = 0; i < count; i++)
		m_pList->AddItem((void*)i);

	m_dwOpenTime = timeGetTime();

	m_pList->SetKeyFocus(true);
}

// emoticondlg.cpp:114
void FrEmoticonDlg::OnListOwnerDraw(int param) {
	FrEmoticon* pEmoticon = g_pFresh->GetManager()->GetEmoticon();
	if (!pEmoticon)
		return;

	FrListItem* pItem = (FrListItem*)param;
	if (!pItem)
		return;

	pEmoticon->SetAnim(true);
	pEmoticon->SetAnimTime(m_dwOpenTime);

	if (m_pList->GetItemUnderCursor() == pItem) {
		pEmoticon->Draw((intptr_t)pItem->pData, WRect(pItem->pos.x - 3.0f, pItem->pos.y - 3.0f, (float)pEmoticon->GetWidth() + 6.0f, (float)pEmoticon->GetHeight() + 6.0f), 0xffffffff, g_input->Get("LSHIFT", false) || g_input->Get("RSHIFT", false));

		const char* name = pEmoticon->GetIconName((intptr_t)pItem->pData);
		if (name) {
			if (IsLocalCountry(30)) {
				static const char* szOpen = EMOTICON_OPEN;
				static const char* szClose = EMOTICON_CLOSE;

				static const char* szFlip = EMOTICON_FLIP;

				if (g_input->Get("LSHIFT", false) || g_input->Get("RSHIFT", false))
					SetDesc(MakeStr("%s%s%s%s", szOpen, szFlip, name, szClose));
				else
					SetDesc(MakeStr("%s%s%s", szOpen, name, szClose));
			} else {
				if (g_input->Get("LSHIFT", false) || g_input->Get("RSHIFT", false))
					SetDesc(MakeStr("(-%s)", name));
				else
					SetDesc(MakeStr("(%s)", name));
			}
		}
	} else {
		pEmoticon->Draw((intptr_t)pItem->pData, pItem->pos.x, pItem->pos.y, 0xffffffff, g_input->Get("LSHIFT", false) || g_input->Get("RSHIFT", false));
	}

	pEmoticon->SetAnim(false);
}

// emoticondlg.cpp:172
void FrEmoticonDlg::OnListBtnUp(void) {
	FrEmoticon* pEmoticon = g_pFresh->GetManager()->GetEmoticon();
	if (!pEmoticon)
		return;

	FrListItem* pItem = m_pList->GetItemUnderCursor();
	if (!pItem)
		return;

	const char* name = pEmoticon->GetIconName((intptr_t)pItem->pData);
	if (name) {
		if (IsLocalCountry(30)) {
			static const char* szOpen = EMOTICON_OPEN;
			static const char* szClose = EMOTICON_CLOSE;

			static const char* szFlip = EMOTICON_FLIP;

			strcpy(m_szSelectedIcon, szOpen);
			if (g_input->Get("LSHIFT", false) || g_input->Get("RSHIFT", false))
				strcat(m_szSelectedIcon, szFlip);
			strcat(m_szSelectedIcon, name);
			strcat(m_szSelectedIcon, szClose);
		} else {
			strcpy(m_szSelectedIcon, "(");
			if (g_input->Get("LSHIFT", false) || g_input->Get("RSHIFT", false))
				strcat(m_szSelectedIcon, "-");
			strcat(m_szSelectedIcon, name);
			strcat(m_szSelectedIcon, ")");
		}
	}

	Close(FrOK, true);
}
