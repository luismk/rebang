// Original: d:\build\custom\temp\code\client\projectg\guildnoticedlg.cpp
#include "GuildNoticeDlg.h"
#include "ObjectFactory.h"
#include "frbutton.h"
#include "fredit.h"
#include "frscrollbar.h"
#include "NewsReader.h"
#include "UWIN.h"
#include "commonutil.h"
#include <string>

// guildnoticedlg.cpp:18 (run-time type and factory registration of FrGuildNoticeDlg)
const WRTTI FrGuildNoticeDlg::m_RTTI("FrGuildNoticeDlg", &FrForm::m_RTTI);

IObject* FrGuildNoticeDlgMakeInstance(void) {
	return new FrGuildNoticeDlg;
}

struct __sFrGuildNoticeDlg {
	__sFrGuildNoticeDlg(void) {
		ObjectFactory().AddObjectFunctor(FrGuildNoticeDlgMakeInstance, "FrGuildNoticeDlg");
	}
};
static __sFrGuildNoticeDlg s_registerFrGuildNoticeDlg;

// guildnoticedlg.cpp:20
const sFRESH_MSGMAP* FrGuildNoticeDlg::GetMessageMap(void) const {
	return &FrGuildNoticeDlg::_MsgMap;
}

sFRESH_MSGMAP FrGuildNoticeDlg::_MsgMap = { &FrForm::_MsgMap, &FrGuildNoticeDlg::_MsgEntries[0] };

sFRESH_ENTRY FrGuildNoticeDlg::_MsgEntries[] = {
	// guildnoticedlg.cpp:22
	{ "close", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuildNoticeDlg::OnClose_Init },
	// guildnoticedlg.cpp:23
	{ "index", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuildNoticeDlg::OnIndex_Init },
	// guildnoticedlg.cpp:24
	{ "article", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuildNoticeDlg::OnArticle_Init },
	// guildnoticedlg.cpp:25
	{ "prev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuildNoticeDlg::OnPrev_Init },
	// guildnoticedlg.cpp:26
	{ "next", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuildNoticeDlg::OnNext_Init },
	// guildnoticedlg.cpp:27
	{ "close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuildNoticeDlg::OnClose_LBtnUp },
	// guildnoticedlg.cpp:28
	{ "prev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuildNoticeDlg::OnPrev_LBtnUp },
	// guildnoticedlg.cpp:29
	{ "next", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuildNoticeDlg::OnNext_LBtnUp },

	// guildnoticedlg.cpp:31
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// guildnoticedlg.cpp:35
FrGuildNoticeDlg::FrGuildNoticeDlg(void) {
	m_pClose = 0;
	m_pIndex = 0;
	m_pArticle = 0;
	m_pPrev = 0;
	m_pNext = 0;
	m_curArticle = (unsigned int)-1;
}

// guildnoticedlg.cpp:45
FrGuildNoticeDlg::~FrGuildNoticeDlg(void) {
}

// guildnoticedlg.cpp:49
void FrGuildNoticeDlg::DataBind(const NewsReader* pNewsReader, unsigned int index) {
	if (pNewsReader == 0) {
		return;
	}




	m_pNewsReader = pNewsReader;
	SetArticle(index);
}

// guildnoticedlg.cpp:61
void FrGuildNoticeDlg::SetArticle(unsigned int index) {
	if (m_pNewsReader == 0 || m_pClose == 0 || m_pArticle == 0 || m_pPrev == 0 || m_pNext == 0) {
		return;
	}




	if (m_pNewsReader->GetNumArticles() == 0) {
		m_pIndex->ClearLine();
		m_pArticle->AddLine("\xB1\xE6\xB5\xE5 \xB0\xF8\xC1\xF6\xBB\xE7\xC7\xD7\xC0\xBB \xBE\xF2\xBE\xEE\xBF\xC0\xC1\xF6 \xB8\xF8\xC7\xDF\xBD\xC0\xB4\xCF\xB4\xD9.", 0, false);
		m_pPrev->SetVisible(false);
		m_pNext->SetVisible(false);
		return;
	}


	if (index >= m_pNewsReader->GetNumArticles()) {
		return;
	}
	if (m_curArticle == index) {
		return;
	}


	m_curArticle = index;
	m_pPrev->Enable(index != 0);
	m_pNext->Enable(index != m_pNewsReader->GetNumArticles() - 1);


	m_pIndex->ClearLine();
	m_pIndex->AddLine(MakeStr("%d / %d", index + 1, m_pNewsReader->GetNumArticles()), 0, false);


	std::string str;
	m_pArticle->ClearLine();
	const NewsReader::sArticle* pArticle = m_pNewsReader->GetArticle(index);


	str = "\\cT\\c\\c0xffff0000\\c" + pArticle->m_title + "\\c0xff000000\\c";
	m_pArticle->AddText(str.c_str(), false, true);


	str = "\\cT\\c" + pArticle->m_userId + " / " + pArticle->m_regDate;
	m_pArticle->AddText(str.c_str(), false, true);


	str = pArticle->m_content;
	UWIN::AlterChar(str, ';', '\n');
	str = "\n\\cN\\c" + str;
	m_pArticle->AddText(str.c_str(), false, true);


	m_pArticle->GetScrollBar()->ScrollToFirst();
}

// guildnoticedlg.cpp:118
void FrGuildNoticeDlg::Prev(void) {
	SetArticle(m_curArticle - 1);
}

// guildnoticedlg.cpp:123
void FrGuildNoticeDlg::Next(void) {
	SetArticle(m_curArticle + 1);
}

// guildnoticedlg.cpp:131
void FrGuildNoticeDlg::OnClose_Init(intptr_t param) {
	m_pClose = DYNAMIC_CAST(FrButton, param);
}

// guildnoticedlg.cpp:137
void FrGuildNoticeDlg::OnIndex_Init(intptr_t param) {
	m_pIndex = DYNAMIC_CAST(FrEdit, param);
}

// guildnoticedlg.cpp:143
void FrGuildNoticeDlg::OnArticle_Init(intptr_t param) {
	m_pArticle = DYNAMIC_CAST(FrEdit, param);
}

// guildnoticedlg.cpp:149
void FrGuildNoticeDlg::OnPrev_Init(intptr_t param) {
	m_pPrev = DYNAMIC_CAST(FrButton, param);
}

// guildnoticedlg.cpp:155
void FrGuildNoticeDlg::OnNext_Init(intptr_t param) {
	m_pNext = DYNAMIC_CAST(FrButton, param);
}

// guildnoticedlg.cpp:164
void FrGuildNoticeDlg::OnClose_LBtnUp(void) {
	Close(FrNONE, true);
}

// guildnoticedlg.cpp:169
void FrGuildNoticeDlg::OnPrev_LBtnUp(void) {
	SetArticle(m_curArticle - 1);
}

// guildnoticedlg.cpp:174
void FrGuildNoticeDlg::OnNext_LBtnUp(void) {
	SetArticle(m_curArticle + 1);
}
