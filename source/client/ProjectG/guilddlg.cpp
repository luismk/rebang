// Original: d:\build\custom\temp\code\client\projectg\guilddlg.cpp
#include <stdio.h>

#include "GuildDlg.h"
#include "ObjectFactory.h"
#include "cHttp.h"
#include "fredit.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frlistbox.h"
#include "frwndmanager.h"
#include "tinyxml.h"

// guilddlg.cpp:24 (run-time type and factory registration of FrGuildDlg)
const WRTTI FrGuildDlg::m_RTTI("FrGuildDlg", &FrForm::m_RTTI);

IObject* FrGuildDlgMakeInstance(void) {
	return new FrGuildDlg;
}

struct __sFrGuildDlg {
	__sFrGuildDlg(void) {
		ObjectFactory().AddObjectFunctor(FrGuildDlgMakeInstance, "FrGuildDlg");
	}
};
static __sFrGuildDlg s_registerFrGuildDlg;

// guilddlg.cpp:26
const sFRESH_MSGMAP* FrGuildDlg::GetMessageMap(void) const {
	return &FrGuildDlg::_MsgMap;
}

sFRESH_MSGMAP FrGuildDlg::_MsgMap = { &FrForm::_MsgMap, &FrGuildDlg::_MsgEntries[0] };

sFRESH_ENTRY FrGuildDlg::_MsgEntries[] = {
	// guilddlg.cpp:28
	{ "board", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuildDlg::OnBoardInit },
	// guilddlg.cpp:29
	{ "board", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuildDlg::OnBoardBtnUp },
	// guilddlg.cpp:30
	{ "board", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrGuildDlg::OnBoardBtnDown },
	// guilddlg.cpp:31
	{ "board", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuildDlg::OnBoardOwnerDraw },
	// guilddlg.cpp:32
	{ "view", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuildDlg::OnViewInit },
	// guilddlg.cpp:33
	{ "view", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuildDlg::OnViewBtnUp },

	// guilddlg.cpp:35
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// guilddlg.cpp:38
FrGuildDlg::FrGuildDlg(void)
	: m_pBoard(0),
	  m_pView(0) {
}

// guilddlg.cpp:44
FrGuildDlg::~FrGuildDlg(void) {
	UnloadRSS();
}

// guilddlg.cpp:49
bool FrGuildDlg::OnInit(void) {
	if (!m_pBoard) {
		return FrForm::OnInit();
	}

	// "index2.xml": the RSS feed of the guild board
	LoadRSS("index2.xml");

	// select the first entry and show its text
	m_pBoard->SelectItem(m_pBoard->m_itemList.front(), true);
	SelectTitle(m_pBoard->m_itemList.front());

	return FrForm::OnInit();
}

// guilddlg.cpp:65
void FrGuildDlg::OnBoardInit(intptr_t param) {
	m_pBoard = DYNAMIC_CAST(FrListBox, param);
}

// guilddlg.cpp:70
void FrGuildDlg::OnBoardBtnUp(void) {
}

// guilddlg.cpp:74
void FrGuildDlg::OnBoardOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (!pItem)
		return;

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (!pGDI)
		return;

	sGuildRSSItem* pRSS = (sGuildRSSItem*)pItem->pData;
	if (m_pBoard->GetSelected() == pItem) {
		// highlight bar of the selected entry, as wide as the list
		WRect rect;
		m_pBoard->GetClientRect(rect);
		pGDI->Box(WRect(pItem->pos.x + 4.0f, pItem->pos.y, rect.w, 16.0f), 0x999B9DFF, 0, 0.0f);
	}

	if (pItem->underCursor) {
		pGDI->SetTextColor(0xFFFFFFFF, 0xFF808080);
		pGDI->SetTextStyle(2);
	} else {
		pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
		pGDI->SetTextStyle(0);
	}

	pGDI->Print(pItem->pos, 0, pRSS->title);
}

// guilddlg.cpp:109
void FrGuildDlg::OnBoardBtnDown(void) {
	FrListItem* pItem = m_pBoard->GetItemUnderCursor();
	if (pItem) {
		m_pBoard->SelectItem(pItem, true);
		SelectTitle(pItem);
	} else {
		m_pBoard->SelectItem(0, true);
	}
}

// guilddlg.cpp:123
void FrGuildDlg::OnViewInit(intptr_t param) {
	m_pView = DYNAMIC_CAST(FrEdit, param);
}

// guilddlg.cpp:128
void FrGuildDlg::OnViewBtnUp(void) {
}

// guilddlg.cpp:133
bool FrGuildDlg::DownLoad(const char* fileName, const char* url, const char* localDir, const char* urlBase) {
	char fullUrl[MAX_PATH];
	char localPath[MAX_PATH];
	DWORD available;
	DWORD read;

	sprintf(fullUrl, "%s%s", urlBase, url);
	sprintf(localPath, "%s%s", localDir, fileName);

	HINTERNET hInternet = InternetOpenA("cHttp DownLoad", 0, NULL, NULL, 0);
	if (hInternet == NULL)
		return false;

	HINTERNET hUrl = InternetOpenUrlA(hInternet, fullUrl, NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if (hUrl == NULL) {
		InternetCloseHandle(hInternet);
		return false;
	}

	FILE* fp = fopen(localPath, "wb");
	if (fp == NULL)
		return false;

	InternetQueryDataAvailable(hUrl, &available, 0, 0);
	char* buffer = new char[available];

	do {
		InternetReadFile(hUrl, buffer, available, &read);
		fwrite(buffer, read, 1, fp);
	} while (read != 0);

	delete[] buffer;
	fclose(fp);

	InternetCloseHandle(hUrl);
	InternetCloseHandle(hInternet);

	return true;
}

// guilddlg.cpp:176
void FrGuildDlg::UnloadRSS(void) {
	for (FrListBox::ITEM_LIST::iterator i = m_pBoard->m_itemList.begin(); i != m_pBoard->m_itemList.end(); ++i) {
		FrListItem* pItem = *i;
		sGuildRSSItem* pRSS = (sGuildRSSItem*)pItem->pData;
		delete[] pRSS->title;
		delete[] pRSS->description;
		delete pRSS;
	}
}

// guilddlg.cpp:191
void FrGuildDlg::SelectTitle(FrListItem* pItem) {
	m_pView->ClearLine();

	m_pView->AddText(((sGuildRSSItem*)pItem->pData)->description, false, true);
}

// guilddlg.cpp:198
void FrGuildDlg::ParseRSS(TiXmlDocument& doc) {
	TiXmlNode* pRss = doc.FirstChild("rss");
	if (!pRss)
		return;

	TiXmlNode* pChannel = pRss->FirstChild("channel");
	if (!pChannel)
		return;

	TiXmlNode* pItem = pChannel->FirstChild("item");
	while (pItem) {
		sGuildRSSItem* pRSS = new sGuildRSSItem;

		TiXmlNode* pTitle = pItem->FirstChild("title");
		const char* title = pTitle->FirstChild()->Value();

		int len = strlen(title);
		pRSS->title = new char[len + 1];
		strcpy(pRSS->title, title);

		TiXmlNode* pDescription = pItem->FirstChild("description");
		const char* description = pDescription->FirstChild()->Value();

		len = strlen(description);
		pRSS->description = new char[len + 1];
		strcpy(pRSS->description, description);

		m_pBoard->AddItem(pRSS);

		pItem = pItem->NextSibling("item");
	}
}

// guilddlg.cpp:248
void FrGuildDlg::LoadRSS(const char* fileName) {
	TiXmlDocument doc;

	DownLoad("index.xml", "http://kwanny.ntreev.net/tt/index.xml", "", "");

	FILE* fp = fopen("index.xml", "rb");
	if (!fp)
		return;

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	char* buffer = new char[size + 1];
	fseek(fp, 0, SEEK_SET);
	fread(buffer, size, 1, fp);

	if (doc.LoadFileFromMemoryEx(buffer))
		ParseRSS(doc);

	delete[] buffer;
}
