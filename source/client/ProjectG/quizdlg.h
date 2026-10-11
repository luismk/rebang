// Original: d:\build\custom\temp\code\client\projectg\quizdlg.h
#ifndef PG_CLIENT_PROJECTG_QUIZDLG_H
#define PG_CLIENT_PROJECTG_QUIZDLG_H

#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;
class FrListBox;

class AnswerCard {
	LAYOUT_CHECK_FRIEND;
public:
	enum eCheckNum {
		eCheck1 = 1,
		eCheck2 = 2,
		eCheck3 = 4,
		eCheck4 = 8,
	};

	AnswerCard();
	~AnswerCard();
	void SetMode(bool mode);
	bool IsChecked(eCheckNum num) const;
	int ISAllCheck() const;
	unsigned char GetAnswerCard() const;
	void DeleteAnswer(eCheckNum num);
	bool WriteAnswer(eCheckNum num);

private:
	bool m_bMode;           // 0x0
	unsigned char m_check;  // 0x1
};

CHECK_SIZE(AnswerCard, 0x2);
BEGIN_LAYOUT_CHECK(AnswerCard)
	CHECK_FIELD(AnswerCard, m_bMode, 0x0);
	CHECK_FIELD(AnswerCard, m_check, 0x1);
END_LAYOUT_CHECK

// One answer of a quiz: whether it is the right one, and its text.
struct sQuizCase {
	bool correct;        // 0x0
	std::string text;    // 0x4
};

CHECK_SIZE(sQuizCase, 0x20);

// A quiz question with its five answers (the unit's static table holds two).
struct sQuiz {
	std::string question;   // 0x0
	sQuizCase cases[5];     // 0x1c
};

CHECK_SIZE(sQuiz, 0xbc);

// The quiz dialog; its size (0x148) is the `new` of FrQuizDlgMakeInstance.
// Its destructor is compiler-generated.
class FrQuizDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // quizdlg.h:57

	FrQuizDlg(void);                     // quizdlg.cpp:108

	void SetupQuiz(int);
	const char* GetQuestion(void);       // quizdlg.cpp:307
	bool IsCorrectAnswer(void);
	void ProcessChkBox(void);

	virtual void OnProc(const float dt); // quizdlg.cpp:342

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // quizdlg.cpp:89

	void OnBQuiz1Init(intptr_t param);        // quizdlg.cpp:124
	void OnBQuiz1BtnDown(void);          // quizdlg.cpp:136
	void OnBQuiz2Init(intptr_t param);        // quizdlg.cpp:142
	void OnBQuiz2BtnDown(void);          // quizdlg.cpp:154
	void OnBQuiz3Init(intptr_t param);        // quizdlg.cpp:160
	void OnBQuiz3BtnDown(void);          // quizdlg.cpp:172
	void OnBQuiz4Init(intptr_t param);        // quizdlg.cpp:178
	void OnBQuiz4BtnDown(void);          // quizdlg.cpp:190
	void OnCasesInit(intptr_t param);         // quizdlg.cpp:197
	void OnCasesOwnerDraw(intptr_t param);    // quizdlg.cpp:204
	void OnCasesBtnDown(void);           // quizdlg.cpp:239

	FrListBox* m_pCaseList;          // 0x110
	sQuiz* m_pQuiz;                  // 0x114
	int m_caseOrder[4];              // 0x118, filled by SetupQuiz
	int m_correctAnswer;             // 0x128, -1 until SetupQuiz; OnCasesBtnDown stores the picked case
	int m_reserved12c;               // 0x12c, cleared by the constructor only
	bool m_bUpdateChkBox;            // 0x130, OnProc runs ProcessChkBox when set
	FrButton* m_pChkBox[4];          // 0x134
	AnswerCard m_answerCard;         // 0x144
};

CHECK_SIZE(FrQuizDlg, 0x148);
BEGIN_LAYOUT_CHECK(FrQuizDlg)
	CHECK_FIELD(FrQuizDlg, m_pCaseList, 0x110);
	CHECK_FIELD(FrQuizDlg, m_pQuiz, 0x114);
	CHECK_FIELD(FrQuizDlg, m_caseOrder, 0x118);
	CHECK_FIELD(FrQuizDlg, m_correctAnswer, 0x128);
	CHECK_FIELD(FrQuizDlg, m_reserved12c, 0x12c);
	CHECK_FIELD(FrQuizDlg, m_bUpdateChkBox, 0x130);
	CHECK_FIELD(FrQuizDlg, m_pChkBox, 0x134);
	CHECK_FIELD(FrQuizDlg, m_answerCard, 0x144);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_QUIZDLG_H
