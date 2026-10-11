// Original: d:\build\custom\temp\code\client\projectg\quizdlg.cpp
#include <stdlib.h>
#include <string.h>

#include "QuizDlg.h"
#include "ObjectFactory.h"
#include "fresh.h"
#include "frbutton.h"
#include "frlistbox.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"

// quizdlg.cpp:11
// The tutorial's two quizzes; nothing but the initializer and its atexit
// destructor references them.
static sQuiz s_quizList[] = {
	// quizdlg.cpp:13
	{ "\xC0\xCC\xC1\xDF\xBF\xA1\xBC\xAD \xB0\xA1\xC0\xE5 \xC1\xC1\xC0\xBA \xC1\xA1\xBC\xF6\xB0\xA1 \xB9\xAB\xBE\xF9\xC0\xCF\xB1\xEE?", { { true, "Albatross." }, { false, "Boggie" }, { false, "Par" }, { false, "Dobble Boggie" }, { false, "Eagle" } } },
	// quizdlg.cpp:14
	{ "\xC0\xCC\xC1\xDF\xBF\xA1\xBC\xAD \xBC\xA6\xC0\xBB \xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xC1\xF6\xC7\xFC\xC0\xBA \xB9\xAB\xBE\xF9\xC0\xCF\xB1\xEE?", { { true, "O.B." }, { false, "Ice" }, { false, "Bunker" }, { false, "Fairway" }, { false, "Rough" } } },
};

// quizdlg.h:34
AnswerCard::AnswerCard() : m_bMode(false), m_check(0) {
}

// quizdlg.h:35
AnswerCard::~AnswerCard() {
}

// quizdlg.h:42
void AnswerCard::SetMode(bool mode) {
	m_bMode = mode;
}

// quizdlg.cpp:20
bool AnswerCard::WriteAnswer(eCheckNum num) {
	if (m_bMode == true) {
		m_check ^= num;
	} else {
		m_check = num;
	}

	return false;
}

// quizdlg.cpp:38
bool AnswerCard::IsChecked(eCheckNum num) const {
	bool checked = false;
	if (m_check & num) {
		checked = true;
	}

	return checked;
}

// quizdlg.cpp:51
int AnswerCard::ISAllCheck() const {
	if (m_check & eCheck1) {
		return 1;
	} else if (m_check & eCheck2) {
		return 2;
	} else if (m_check & eCheck3) {
		return 3;
	} else if (m_check & eCheck4) {
		return 4;
	}

	return 0;
}

// quizdlg.cpp:66
unsigned char AnswerCard::GetAnswerCard() const {
	return m_check;
}

// quizdlg.cpp:72
void AnswerCard::DeleteAnswer(eCheckNum num) {
	if (num == 0) {
		m_check = 0;
	} else {
		if (m_check & num) {
			m_check ^= num;
		}
	}
}

// quizdlg.cpp:87 (run-time type and factory registration of FrQuizDlg)
const WRTTI FrQuizDlg::m_RTTI("FrQuizDlg", &FrForm::m_RTTI);

IObject* FrQuizDlgMakeInstance(void) {
	return new FrQuizDlg;
}

struct __sFrQuizDlg {
	__sFrQuizDlg(void) {
		ObjectFactory().AddObjectFunctor(FrQuizDlgMakeInstance, "FrQuizDlg");
	}
};
static __sFrQuizDlg s_registerFrQuizDlg;

// quizdlg.cpp:89
const sFRESH_MSGMAP* FrQuizDlg::GetMessageMap(void) const {
	return &FrQuizDlg::_MsgMap;
}

sFRESH_MSGMAP FrQuizDlg::_MsgMap = { &FrForm::_MsgMap, &FrQuizDlg::_MsgEntries[0] };

sFRESH_ENTRY FrQuizDlg::_MsgEntries[] = {
	// quizdlg.cpp:91
	{ "cases", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuizDlg::OnCasesInit },
	// quizdlg.cpp:92
	{ "cases", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuizDlg::OnCasesOwnerDraw },
	// quizdlg.cpp:93
	{ "cases", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrQuizDlg::OnCasesBtnDown },

	// quizdlg.cpp:95
	{ "button_quiz1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuizDlg::OnBQuiz1Init },
	// quizdlg.cpp:96
	{ "button_quiz1", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrQuizDlg::OnBQuiz1BtnDown },
	// quizdlg.cpp:97
	{ "button_quiz2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuizDlg::OnBQuiz2Init },
	// quizdlg.cpp:98
	{ "button_quiz2", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrQuizDlg::OnBQuiz2BtnDown },
	// quizdlg.cpp:99
	{ "button_quiz3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuizDlg::OnBQuiz3Init },
	// quizdlg.cpp:100
	{ "button_quiz3", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrQuizDlg::OnBQuiz3BtnDown },
	// quizdlg.cpp:101
	{ "button_quiz4", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuizDlg::OnBQuiz4Init },
	// quizdlg.cpp:102
	{ "button_quiz4", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrQuizDlg::OnBQuiz4BtnDown },

	// quizdlg.cpp:105
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// quizdlg.cpp:108
FrQuizDlg::FrQuizDlg(void) {
	m_pCaseList = 0;
	m_pQuiz = 0;




	m_reserved12c = 0;
	m_bUpdateChkBox = false;
	m_correctAnswer = -1;
	memset(m_pChkBox, 0, sizeof(m_pChkBox));
	m_answerCard.SetMode(false);
	m_answerCard.DeleteAnswer((AnswerCard::eCheckNum)0);
}

// quizdlg.cpp:124
void FrQuizDlg::OnBQuiz1Init(intptr_t param) {
	m_pChkBox[0] = DYNAMIC_CAST(FrButton, param);

	if (m_pChkBox[0]) {
		m_pChkBox[0]->SetPushDelay(0.0f);
	}
}

// quizdlg.cpp:136
void FrQuizDlg::OnBQuiz1BtnDown(void) {
	m_answerCard.WriteAnswer(AnswerCard::eCheck1);
	m_bUpdateChkBox = true;
}

// quizdlg.cpp:142
void FrQuizDlg::OnBQuiz2Init(intptr_t param) {
	m_pChkBox[1] = DYNAMIC_CAST(FrButton, param);

	if (m_pChkBox[1]) {
		m_pChkBox[1]->SetPushDelay(0.0f);
	}
}

// quizdlg.cpp:154
void FrQuizDlg::OnBQuiz2BtnDown(void) {
	m_answerCard.WriteAnswer(AnswerCard::eCheck2);
	m_bUpdateChkBox = true;
}

// quizdlg.cpp:160
void FrQuizDlg::OnBQuiz3Init(intptr_t param) {
	m_pChkBox[2] = DYNAMIC_CAST(FrButton, param);

	if (m_pChkBox[2]) {
		m_pChkBox[2]->SetPushDelay(0.0f);
	}
}

// quizdlg.cpp:172
void FrQuizDlg::OnBQuiz3BtnDown(void) {
	m_answerCard.WriteAnswer(AnswerCard::eCheck3);
	m_bUpdateChkBox = true;
}

// quizdlg.cpp:178
void FrQuizDlg::OnBQuiz4Init(intptr_t param) {
	m_pChkBox[3] = DYNAMIC_CAST(FrButton, param);

	if (m_pChkBox[3]) {
		m_pChkBox[3]->SetPushDelay(0.0f);
	}
}

// quizdlg.cpp:190
void FrQuizDlg::OnBQuiz4BtnDown(void) {
	m_answerCard.WriteAnswer(AnswerCard::eCheck4);
	m_bUpdateChkBox = true;
}

// quizdlg.cpp:197
void FrQuizDlg::OnCasesInit(intptr_t param) {
	m_pCaseList = DYNAMIC_CAST(FrListBox, param);
}

// quizdlg.cpp:204
void FrQuizDlg::OnCasesOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (!pItem)
		return;

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (!pGDI)
		return;

	if (!m_pQuiz)
		return;

	int caseIdx = (intptr_t)pItem->pData;

	pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);

	pGDI->SetTextStyle(1);

	if (pItem->selected) {
		pGDI->SetTextColor(0xFFFF0000, 0xFFFFFFFF);
	} else if (pItem->underCursor) {
		pGDI->SetTextStyle(4);
		pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
	}

	const char* text = m_pQuiz->cases[caseIdx].text.c_str();

	pGDI->Print(WPoint(pItem->pos.x + 10.0f, pItem->pos.y + 7.0f), 0, "%d. %s", pItem->idx, text);
	pGDI->SetTextStyle(0);
}

// quizdlg.cpp:239
void FrQuizDlg::OnCasesBtnDown(void) {
	if (!m_pCaseList)
		return;

	FrListItem* pItem = m_pCaseList->GetItemUnderCursor();
	if (!pItem)
		return;

	m_correctAnswer = (intptr_t)pItem->pData;
}

// quizdlg.cpp:251
void FrQuizDlg::SetupQuiz(int quiz) {
	m_correctAnswer = (quiz == 0) ? 3 : 1;

	if (m_pQuiz == 0)
		return;

	for (int i = 0; i < 4; i++) m_caseOrder[i] = i + 1;

	int blank = rand() % 4;
	m_caseOrder[blank] = 0;

	if (m_pCaseList) {
		for (int i = 0; i < 4; i++) {
			m_pCaseList->AddItem((void*)m_caseOrder[i]);
		}
	}
}

// quizdlg.cpp:307
const char* FrQuizDlg::GetQuestion(void) {
	if (m_pQuiz != 0) {
		return m_pQuiz->question.c_str();
	}

	return 0;
}

// quizdlg.cpp:317
bool FrQuizDlg::IsCorrectAnswer(void) {
	int answer = m_answerCard.ISAllCheck();

	return (m_correctAnswer == answer) ? true : false;
}

// quizdlg.cpp:342
void FrQuizDlg::OnProc(const float dt) {
	if (m_bUpdateChkBox) {
		ProcessChkBox();
		m_bUpdateChkBox = false;
	}
}

// quizdlg.cpp:351
void FrQuizDlg::ProcessChkBox(void) {
	if (m_answerCard.IsChecked(AnswerCard::eCheck1))
		m_pChkBox[0]->SetButtonImg("tutorial_btn_check_d", FrButton::NORMAL);
	else
		m_pChkBox[0]->SetButtonImg("tutorial_btn_check_n", FrButton::NORMAL);
	if (m_answerCard.IsChecked(AnswerCard::eCheck2))
		m_pChkBox[1]->SetButtonImg("tutorial_btn_check_d", FrButton::NORMAL);
	else
		m_pChkBox[1]->SetButtonImg("tutorial_btn_check_n", FrButton::NORMAL);
	if (m_answerCard.IsChecked(AnswerCard::eCheck3))
		m_pChkBox[2]->SetButtonImg("tutorial_btn_check_d", FrButton::NORMAL);
	else
		m_pChkBox[2]->SetButtonImg("tutorial_btn_check_n", FrButton::NORMAL);
	if (m_answerCard.IsChecked(AnswerCard::eCheck4))
		m_pChkBox[3]->SetButtonImg("tutorial_btn_check_d", FrButton::NORMAL);
	else
		m_pChkBox[3]->SetButtonImg("tutorial_btn_check_n", FrButton::NORMAL);
}
