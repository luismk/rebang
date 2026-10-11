// Original: d:\build\custom\temp\code\client\projectg\trainingdlg.cpp
#include "TrainingDlg.h"
#include "ObjectFactory.h"
#include "bitmap.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frlistbox.h"
#include "frwndmanager.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "guidebook.h"

// The original calls the CRT's POSIX close() of <io.h> where FrForm::Close was
// meant.
#include <io.h>

// One button of the training list: its kind, the "ICONS" bitmap id and the
// caption (12 bytes; the list items' data, read by OnTrainingOwnerDraw).
struct sButtonInfo {
	unsigned char type;   // 0x0
	char icon[4];         // 0x1
	const char* name;     // 0x8
};
CHECK_FIELD(sButtonInfo, type, 0x0);
CHECK_FIELD(sButtonInfo, icon, 0x1);
CHECK_FIELD(sButtonInfo, name, 0x8);
CHECK_SIZE(sButtonInfo, 0xc);

// trainingdlg.cpp:21 (run-time type and factory registration of FrTrainingDlg)
const WRTTI FrTrainingDlg::m_RTTI("FrTrainingDlg", &FrForm::m_RTTI);

IObject* FrTrainingDlgMakeInstance(void) {
	return new FrTrainingDlg;
}

struct __sFrTrainingDlg {
	__sFrTrainingDlg(void) {
		ObjectFactory().AddObjectFunctor(FrTrainingDlgMakeInstance, "FrTrainingDlg");
	}
};
static __sFrTrainingDlg s_registerFrTrainingDlg;

// trainingdlg.cpp:23
const sFRESH_MSGMAP* FrTrainingDlg::GetMessageMap(void) const {
	return &FrTrainingDlg::_MsgMap;
}

sFRESH_MSGMAP FrTrainingDlg::_MsgMap = { &FrForm::_MsgMap, &FrTrainingDlg::_MsgEntries[0] };

sFRESH_ENTRY FrTrainingDlg::_MsgEntries[] = {
	// trainingdlg.cpp:25
	{ "training", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingDlg::OnTrainingInit },
	// trainingdlg.cpp:26
	{ "training", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingDlg::OnTrainingOwnerDraw },
	// trainingdlg.cpp:27
	{ "training", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTrainingDlg::OnTrainingBtnUp },
	// trainingdlg.cpp:28
	{ "close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTrainingDlg::OnTrainingCloseBtnUp },

	// trainingdlg.cpp:30
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// The button captions ("learn with the caddie", "practice alone", "caddie
// book", "special shot practice"). They are named const pointers, not literals
// in the initializer: cl 13.10 then makes buttonList dynamic from the first
// caption on (the original's initializer sub_009d7b90), folding their values.
static const char* const s_learnWithCaddieName = "\xc4\xb3\xb5\xf0\xb6\xfb \xb9\xe8\xbf\xec\xb1\xe2";
static const char* const s_practiceAloneName = "\xc8\xa5\xc0\xda \xbf\xac\xbd\xc0\xc7\xcf\xb1\xe2";
static const char* const s_caddieBookName = "\xc4\xb3\xb5\xf0\xba\xcf";
static const char* const s_specialShotPracticeName = "\xc6\xaf\xbc\xf6\xbc\xa6 \xbf\xac\xbd\xc0\xc7\xcf\xb1\xe2";

// trainingdlg.cpp:39
sButtonInfo buttonList[] = {
	// trainingdlg.cpp:40
	{ 0, "i61", s_learnWithCaddieName },
	// trainingdlg.cpp:41
	{ 1, "i62", s_practiceAloneName },
	// trainingdlg.cpp:42
	{ 2, "i65", s_caddieBookName },
	{ 3, "i64", s_specialShotPracticeName }
};

// trainingdlg.cpp:46
void FrTrainingDlg::OnTrainingInit(intptr_t param) {
	m_pListBox = DYNAMIC_CAST(FrListBox, param);
	m_pListBox->ClearItem();

	m_pListBox->AddItem(&buttonList[0]);
	m_pListBox->AddItem(&buttonList[1]);
	m_pListBox->AddItem(&buttonList[2]);





	m_optionState = 0;
}

// trainingdlg.cpp:62
void FrTrainingDlg::OnTrainingBtnUp(void) {
	if (m_pListBox == 0) {
		return;
	}

	FrListItem* pItem = m_pListBox->GetItemUnderCursor();
	if (pItem == 0) {
		return;
	}

	sButtonInfo* pInfo = (sButtonInfo*)pItem->pData;
	if (pInfo == 0) {
		return;
	}

	switch (pInfo->type) {
	case 0:
		// Learn with the caddie: the tutorial course, one hole.
		CSharedDoc::Instance()->m_gameMode = 0;

		SetCurMap(0xFD);
		SetGameType(12);
		CSharedDoc::Instance()->m_holes = 1;

		for (unsigned char i = 0; i < 18; i++) {
			CSharedDoc::Instance()->m_holeOrder[i] = 0;
		}
		CSharedDoc::Instance()->m_holeOrder[0] = 1;
		break;

	case 1:
		// Practice alone: 18 holes of the first course, random pins.
		CSharedDoc::Instance()->m_gameMode = 2;

		SetCurMap(0);
		SetGameType(0);
		CSharedDoc::Instance()->m_holes = 18;
		CSharedDoc::Instance()->m_gameKind = 0;

		for (unsigned char i = 0; i < 18; i++) {
			CSharedDoc::Instance()->m_holeOrder[i] = i + 1;
			CSharedDoc::Instance()->m_holePin[i] = rand() % 3;
		}

		{
			WSendPacket packet((enumClientPacket)0x5A);
			packet.Encode1(0);
			packet.Send(SEND_TO_DEFAULT);
		}
		break;

	case 2:
		{
			// The caddie book.
			FrGuideBookDlg* pDlg = CreateForm<FrGuideBookDlg>(g_pFresh->GetManager(), this, "guide_book", 0);
			if (pDlg) {
				pDlg->Open(0, 1);
			}

			Close(FrCANCEL, true);
			return;
		}

	case 3:
		// Special shot practice: the tutorial course's second hole.
		CSharedDoc::Instance()->m_gameMode = 3;

		SetCurMap(0xFD);
		SetGameType(0);
		CSharedDoc::Instance()->m_holes = 1;

		for (unsigned char i = 0; i < 18; i++) {
			CSharedDoc::Instance()->m_holeOrder[i] = 0;
		}
		CSharedDoc::Instance()->m_holeOrder[0] = 2;
		break;
	}

	Close(FrOK, true);
}

// trainingdlg.cpp:156
void FrTrainingDlg::OnTrainingOwnerDraw(int param) {
	if (m_pListBox == 0) {
		return;
	}

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0) {
		return;
	}

	sButtonInfo* pInfo = (sButtonInfo*)pItem->pData;
	if (pInfo == 0) {
		return;
	}

	const Bitmap* pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS", "ch_base");
	if (pBitmap) {
		pGDI->DrawTexture(pBitmap, WRect(pItem->pos.x, pItem->pos.y, (float)pBitmap->Width(), (float)pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pBitmap = g_pFresh->GetManager()->GetBitmap("ICONS", pInfo->icon);
	if (pBitmap) {
		pGDI->DrawTexture(pBitmap, WRect(pItem->pos.x + 17.0f, pItem->pos.y + 5.0f, (float)pBitmap->Width(), (float)pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	if (pItem->underCursor) {

		pGDI->SetTextColor(0xFFFFFFFF, 0xFF808080);
		pGDI->SetTextStyle(2);

	} else {

		pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
		pGDI->SetTextStyle(0);
	}

	g_pFresh->GetManager()->PrintText(WPoint(pItem->pos.x + 80.0f, pItem->pos.y + 22.0f), 0, pInfo->name, -1.0f, 0xFFFFFFFF);
}

// trainingdlg.cpp:194
void FrTrainingDlg::OnTrainingCloseBtnUp(void) {
	Close(FrCANCEL, true);
}

// trainingdlg.cpp:199
bool FrTrainingDlg::OnNewTrainingOptionDlgResult(int result, FrForm* pForm) {
	m_optionState = 0;
	if (result == FrOK) {
		WSendPacket packet((enumClientPacket)0x5A);
		packet.Encode1(0);
		packet.Send(SEND_TO_DEFAULT);

		close(FrOK);
	} else {

		close(FrCANCEL);
	}

	return true;
}
