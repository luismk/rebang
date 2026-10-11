// Original: d:\build\custom\temp\code\client\projectg\schooldlg.h
#ifndef PG_CLIENT_PROJECTG_SCHOOLDLG_H
#define PG_CLIENT_PROJECTG_SCHOOLDLG_H

#include <list>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrComboBox;
class FrEdit;
class FrStatic;

// Japanese school types (four EUC-KR names, schooldlg.cpp:21-26).
extern std::string szJpSchoolType[4];

// School search dialog: a search edit ("like"), the school combo box and the
// type / district combo boxes that narrow the search; its size (0x250) is the
// `new` of FrSchoolDlgMakeInstance.
class FrSchoolDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // schooldlg.h:20

	// One school of a search result (implicit constructors and destructor).
	struct sJpSchool {
		unsigned int no;    // 0x0 (bits 28-30: school type, see OnTypeListLBtnDown)
		std::string name;   // 0x4
	};

	FrSchoolDlg(void);
	virtual ~FrSchoolDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);

	void OnLikeInit(intptr_t param);
	bool OnLikeEnterKey(int param);
	void OnSchoolInit(intptr_t param);
	void OnSchoolBtnDown(intptr_t param);
	void OnSearchBtnUp(void);
	void OnOkBtnUp(void);
	void OnExampleInit(int param);
	void OnTypeInit(int param);
	void OnDist0Init(int param);
	void OnDist1Init(int param);
	void OnStaticDist1Init(int param);
	void OnSearchInit(int param);
	void OnTypeListLBtnDown(int param);
	void OnDist0ListLBtnDown(int param);
	void OnDist1ListLBtnDown(int param);

private:
	int m_schoolNo;                       // 0x110 (number of the chosen school, 0 = none)
	std::string m_schoolName;             // 0x114 (name of the chosen school)
	FrEdit* m_pLikeEdit;                  // 0x130 ("like")
	FrComboBox* m_pSchoolCombo;           // 0x134 ("school")
	FrComboBox* m_pTypeCombo;             // 0x138 (OnTypeInit)
	FrComboBox* m_pDist0Combo;            // 0x13c (OnDist0Init)
	FrComboBox* m_pDist1Combo;            // 0x140 (OnDist1Init)
	FrStatic* m_pDist1Static;             // 0x144 (OnStaticDist1Init)
	std::string m_typeName;               // 0x148 (selection of the type combo, presumably)
	std::string m_dist0Name;              // 0x164 (selection of the first district combo, presumably)
	std::string m_dist1Name;              // 0x180 (selection of the second district combo, presumably)
	unsigned int m_typeNo;                // 0x19c (1-4: index + 1 in szJpSchoolType, 5: none)
	unsigned int m_dist0No;               // 0x1a0 (sJpSchool::no of the chosen district)
	unsigned int m_dist1No;               // 0x1a4 (sJpSchool::no of the chosen district)
	std::list<sJpSchool> m_schoolListA;   // 0x1a8
	std::list<sJpSchool> m_schoolListB;   // 0x1b4
	std::list<sJpSchool> m_schoolListC;   // 0x1c0
	int m_schoolNos[33];                  // 0x1cc (school number of each row of the school combo)
};

CHECK_SIZE(FrSchoolDlg::sJpSchool, 0x20);
CHECK_SIZE(FrSchoolDlg, 0x250);
BEGIN_LAYOUT_CHECK(FrSchoolDlg)
	CHECK_FIELD(FrSchoolDlg, m_schoolNo, 0x110);
	CHECK_FIELD(FrSchoolDlg, m_schoolName, 0x114);
	CHECK_FIELD(FrSchoolDlg, m_pLikeEdit, 0x130);
	CHECK_FIELD(FrSchoolDlg, m_pSchoolCombo, 0x134);
	CHECK_FIELD(FrSchoolDlg, m_pTypeCombo, 0x138);
	CHECK_FIELD(FrSchoolDlg, m_pDist1Static, 0x144);
	CHECK_FIELD(FrSchoolDlg, m_typeName, 0x148);
	CHECK_FIELD(FrSchoolDlg, m_dist1Name, 0x180);
	CHECK_FIELD(FrSchoolDlg, m_typeNo, 0x19c);
	CHECK_FIELD(FrSchoolDlg, m_dist1No, 0x1a4);
	CHECK_FIELD(FrSchoolDlg, m_schoolListA, 0x1a8);
	CHECK_FIELD(FrSchoolDlg, m_schoolListC, 0x1c0);
	CHECK_FIELD(FrSchoolDlg, m_schoolNos, 0x1cc);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SCHOOLDLG_H
