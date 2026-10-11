// Original: d:\build\custom\temp\code\client\projectg\mailboxdlg.cpp
#include <string>

#include "MailBoxDlg.h"
#include "ObjectFactory.h"
#include "NoteDlg.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "commonutil.h"
#include "fresh.h"
#include "fremoticon.h"
#include "frgraphicinterface.h"
#include "frlistbox.h"
#include "frwndmanager.h"

// mailboxdlg.cpp:19 (run-time type and factory registration of FrMailBoxDlg)
const WRTTI FrMailBoxDlg::m_RTTI("FrMailBoxDlg", &FrForm::m_RTTI);

IObject* FrMailBoxDlgMakeInstance(void) {
	return new FrMailBoxDlg;
}

struct __sFrMailBoxDlg {
	__sFrMailBoxDlg(void) {
		ObjectFactory().AddObjectFunctor(FrMailBoxDlgMakeInstance, "FrMailBoxDlg");
	}
};
static __sFrMailBoxDlg s_registerFrMailBoxDlg;

// mailboxdlg.cpp:21
const sFRESH_MSGMAP* FrMailBoxDlg::GetMessageMap(void) const {
	return &FrMailBoxDlg::_MsgMap;
}

sFRESH_MSGMAP FrMailBoxDlg::_MsgMap = { &FrForm::_MsgMap, &FrMailBoxDlg::_MsgEntries[0] };

sFRESH_ENTRY FrMailBoxDlg::_MsgEntries[] = {
	// mailboxdlg.cpp:23
	{ "note", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMailBoxDlg::OnNoteInit },
	// mailboxdlg.cpp:24
	{ "note", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMailBoxDlg::OnNoteOwnerDraw },
	// mailboxdlg.cpp:25
	{ "note", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMailBoxDlg::OnNoteLBtnUp },
	// mailboxdlg.cpp:26
	{ "note", FRCMD_RBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMailBoxDlg::OnNoteRBtnUp },
	// mailboxdlg.cpp:27
	{ "note", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&FrMailBoxDlg::OnNoteRBtnUp },
	// mailboxdlg.cpp:28
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMailBoxDlg::OnCancelBtnUp },

	// mailboxdlg.cpp:30
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// mailboxdlg.cpp:33
void FrMailBoxDlg::OnNoteInit(intptr_t param) {
	m_noteList = DYNAMIC_CAST(FrListBox, param);
	if (!m_noteList)
		return;

	m_noteList->UseRightButton(true);

	for (std::list<sNoteInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_noteList.begin();
		it != WSingleton<CSharedDoc>::Instance()->m_noteList.end(); ++it) {
		m_noteList->AddItem(&*it);
	}
}

// mailboxdlg.cpp:48
void FrMailBoxDlg::OnNoteOwnerDraw(int param) {
	if (!m_noteList)
		return;

	FrListItem* pItem = (FrListItem*)param;
	if (!pItem)
		return;

	sNoteInfo* pNote = (sNoteInfo*)pItem->pData;
	if (!pNote)
		return;

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (!pGDI)
		return;

	if (pNote == m_pSelNote)
		pGDI->Box(WRect(pItem->pos.x + 5.0f, pItem->pos.y, 560.0f, 20.0f), 0x999B9DFF, 0, 0.0f);

	pGDI->SetTextStyle(0);

	if (pNote->read)
		pGDI->SetTextColor(0xFF808080, 0xFFFFFFFF);
	else
		pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);

	FrEmoticon* pEmoticon = g_pFresh->GetManager()->GetEmoticon();
	if (pEmoticon) {
		pEmoticon->PrintText(WPoint(pItem->pos.x + 8.0f, pItem->pos.y + 3.0f), 0, pNote->nick, 0xFFFFFFFF);
		pEmoticon->PrintText(WPoint(pItem->pos.x + 118.0f, pItem->pos.y + 3.0f), 0, pNote->text, 0xFFFFFFFF);
		pEmoticon->PrintText(WPoint(pItem->pos.x + 467.0f, pItem->pos.y + 3.0f), 0, pNote->date, 0xFFFFFFFF);
	} else {
		pGDI->PrintText(WPoint(pItem->pos.x + 8.0f, pItem->pos.y + 3.0f), 0, pNote->nick, 0);
		pGDI->PrintText(WPoint(pItem->pos.x + 118.0f, pItem->pos.y + 3.0f), 0, pNote->text, 0);
		pGDI->PrintText(WPoint(pItem->pos.x + 467.0f, pItem->pos.y + 3.0f), 0, pNote->date, 0);
	}
}

// mailboxdlg.cpp:91
void FrMailBoxDlg::OnNoteLBtnUp(void) {
	if (!m_noteList)
		return;

	FrListItem* pItem = m_noteList->GetItemUnderCursor();
	if (!pItem)
		return;

	sNoteInfo* pNote = (sNoteInfo*)pItem->pData;
	if (!pNote)
		return;

	m_pSelNote = pNote;
}

// mailboxdlg.cpp:107
void FrMailBoxDlg::OnNoteRBtnUp(void) {
	if (!m_noteList)
		return;

	FrListItem* pItem = m_noteList->GetItemUnderCursor();
	if (!pItem)
		return;

	sNoteInfo* pNote = (sNoteInfo*)pItem->pData;
	if (!pNote)
		return;

	m_pSelNote = pNote;

	if (pNote->read)
		return;

	if (WSingleton<CSharedDoc>::Instance()->m_myPang < 10) {
		// "You need %d pang to send a note."
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 0x23, (intptr_t)MakeStr("\xC2\xCA\xC1\xF6\xB8\xA6 \xBA\xB8\xB3\xBB\xB1\xE2 \xC0\xA7\xC7\xD8\xBC\xAD\xB4\xC2 %d\xC6\xCE\xC0\xCC \xC7\xCA\xBF\xE4\xC7\xD5\xB4\xCF\xB4\xD9.", 10), 0, 0, 0, 0);
		return;
	}

	if (pNote->nick[0] == '@') {
		// "You cannot send a note to an operator."
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 0x23, (intptr_t)MakeStr("\xBF\xEE\xBF\xB5\xC0\xDA\xBF\xA1\xB0\xD4\xB4\xC2 \xC2\xCA\xC1\xF6\xB8\xA6 \xBA\xB8\xB3\xBE \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 10), 0, 0, 0, 0);
		return;
	}

	if (!m_pNoteDlg) {
		m_pNoteDlg = CreateForm<FrNoteDlg>(g_pFresh->GetManager(), this, "note", 0);
		m_pNoteDlg->SetNick(pNote->nick, m_pSelNote->text);
		m_pNoteDlg->Open((FRESH_RESULT_CALLBACK)&FrMailBoxDlg::OnNoteDlgResult, 1);
	}
}

// mailboxdlg.cpp:145
bool FrMailBoxDlg::OnNoteDlgResult(int result, FrForm*) {
	if (result == 1) {
		m_pSelNote->read = true;

		WSendPacket packet((enumClientPacket)0x3C);
		packet.Encode2(0x111);

		packet.Encode4(m_pSelNote->id);
		packet.EncodeStr(std::string(m_pNoteDlg->GetNote()));
		packet.Encode1(0);
		packet.Send(SEND_TO_DEFAULT);
	}

	m_pNoteDlg = NULL;
	return true;
}

// mailboxdlg.cpp:165
void FrMailBoxDlg::OnCancelBtnUp(void) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
	// "Closing the mail box deletes every note.\nClose the mail box?"
	pForm->SetMessage("\xC2\xCA\xC1\xF6\xC7\xD4\xC0\xBB \xB4\xDD\xC0\xB8\xB8\xE9 \xC2\xCA\xC1\xF6\xB0\xA1 \xB8\xF0\xB5\xCE \xBB\xE7\xB6\xF3\xC1\xFD\xB4\xCF\xB4\xD9.\n\xC2\xCA\xC1\xF6\xC7\xD4\xC0\xBB \xB4\xDD\xC0\xB8\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
	pForm->Open((FRESH_RESULT_CALLBACK)&FrMailBoxDlg::OnCloseDlgResult, 0);
}

// mailboxdlg.cpp:172
bool FrMailBoxDlg::OnCloseDlgResult(int result, FrForm*) {
	if (result == 1) {
		WSingleton<CSharedDoc>::Instance()->m_noteList.clear();
		Close(true);
	}

	return true;
}
