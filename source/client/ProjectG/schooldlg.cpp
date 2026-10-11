// Original: d:\build\custom\temp\code\client\projectg\schooldlg.cpp
#include "SchoolDlg.h"
#include "ObjectFactory.h"
#include "frbutton.h"
#include "frcombobox.h"
#include "fredit.h"
#include "frlistbox.h"
#include "frstatic.h"
#include "binstr.h"
#include "westpak.h"
#include "wresrcmng.h"

// westpak.cpp (not declared in westpak.h yet)
void __cdecl CloseCFile(cFile* file);

// schooldlg.cpp:21
std::string szJpSchoolType[4] = {
	"\xC1\xDF\xC7\xD0",
	"\xB0\xED\xB1\xB3",
	"\xB4\xDC\xB4\xEB",
	"\xB4\xEB\xC7\xD0"
};

// schooldlg.cpp:28 (run-time type and factory registration of FrSchoolDlg)
const WRTTI FrSchoolDlg::m_RTTI("FrSchoolDlg", &FrForm::m_RTTI);

IObject* FrSchoolDlgMakeInstance(void) {
	return new FrSchoolDlg;
}

struct __sFrSchoolDlg {
	__sFrSchoolDlg(void) {
		ObjectFactory().AddObjectFunctor(FrSchoolDlgMakeInstance, "FrSchoolDlg");
	}
};
static __sFrSchoolDlg s_registerFrSchoolDlg;

// schooldlg.cpp:30
const sFRESH_MSGMAP* FrSchoolDlg::GetMessageMap(void) const {
	return &FrSchoolDlg::_MsgMap;
}

sFRESH_MSGMAP FrSchoolDlg::_MsgMap = { &FrForm::_MsgMap, &FrSchoolDlg::_MsgEntries[0] };

sFRESH_ENTRY FrSchoolDlg::_MsgEntries[] = {
	// schooldlg.cpp:32
	{ "like", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSchoolDlg::OnLikeInit },
	// schooldlg.cpp:33
	{ "like", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrSchoolDlg::OnLikeEnterKey },
	// schooldlg.cpp:34
	{ "school", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSchoolDlg::OnSchoolInit },
	// schooldlg.cpp:35
	{ "school", FRCMD_LBUTTONDOWN, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSchoolDlg::OnSchoolBtnDown },
	// schooldlg.cpp:36
	{ "search", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrSchoolDlg::OnSearchBtnUp },
	// schooldlg.cpp:37
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrSchoolDlg::OnOkBtnUp },

	// schooldlg.cpp:39
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// schooldlg.cpp:45
FrSchoolDlg::FrSchoolDlg(void) {
	m_schoolNo = 0;
	m_pTypeCombo = 0;
	m_pDist0Combo = 0;
	m_pDist1Combo = 0;
	m_pDist1Static = 0;
	m_typeNo = 0;
	m_dist0No = 0;
	m_dist1No = 0;
}

// schooldlg.cpp:57
FrSchoolDlg::~FrSchoolDlg(void) {
}

// schooldlg.cpp:63
void FrSchoolDlg::OnLikeInit(intptr_t param) {
	m_pLikeEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pLikeEdit) {
		m_pLikeEdit->SetKeyFocus(true);
	}
}

// schooldlg.cpp:76
bool FrSchoolDlg::OnLikeEnterKey(int param) {
	OnSearchBtnUp();
	return false;
}

// schooldlg.cpp:83
void FrSchoolDlg::OnSchoolInit(intptr_t param) {
	m_pSchoolCombo = DYNAMIC_CAST(FrComboBox, param);
}

// schooldlg.cpp:89
void FrSchoolDlg::OnSchoolBtnDown(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	std::string* pName = (std::string*)pItem->pData;
	m_schoolNo = m_schoolNos[pItem->no];
	m_schoolName = *pName;
}

// schooldlg.cpp:98
void FrSchoolDlg::OnSearchBtnUp(void) {
	if (m_pLikeEdit == 0 || m_pSchoolCombo == 0) {
		return;
	}

	const char* like = m_pLikeEdit->GetLine(1, false);
	if (strlen(like) < 4) {
		return;
	}

	m_pSchoolCombo->ClearList();
	m_pSchoolCombo->ClearLine();




	cFile* pFile = g_resrcmng->GetCFile("school.txt", 0xffff);

	if (pFile == 0) {

		m_pSchoolCombo->AddString(like);
		m_pSchoolCombo->SetLine(1, like, 0, false, 0);
		return;
	}

	char line[0x2000];
	int count = 0;
	char number[64];
	char name[128];
	while (pFile->Scan("%n", line)) {

		int len = strlen(line);
		cTokenV token;
		token.Init(line, len);


		token.GetToken(number, " ", 1);


		token.GetToken(name, "\r\n", 2);

		if (strstr(name, like)) {

			if (count >= 10) {

				m_schoolNo = 0;
				m_schoolName = "";
				m_pSchoolCombo->ClearList();
				m_pSchoolCombo->SetLine(1, "\xC1\xBB\xB4\xF5 \xC0\xDA\xBC\xBC\xC7\xCF\xB0\xD4 \xB3\xD6\xBE\xEE\xC1\xD6\xBC\xBC\xBF\xE4", 0, false, 0);
				break;
			}



			m_schoolNos[count++] = atoi(number);

			m_pSchoolCombo->AddString(name);
			if (m_pSchoolCombo->GetLineNum() == 0) {

				m_schoolNo = atoi(number);
				m_schoolName = name;
				m_pSchoolCombo->SetLine(1, name, 0, false, 0);
			}
		}
	}

	CloseCFile(pFile);


	if (m_pSchoolCombo->GetLineNum() == 0) {
		m_pSchoolCombo->SetLine(1, "\xB8\xF1\xB7\xCF\xBF\xA1\xBC\xAD \xC3\xA3\xC0\xBB \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9", 0, false, 0);
	}
}

// schooldlg.cpp:170
void FrSchoolDlg::OnOkBtnUp(void) {
	if (m_schoolNo == 0) {

		m_schoolName = "";
		m_pSchoolCombo->ClearList();
	}




	OnFreshOkay();
}

// schooldlg.cpp:183
void FrSchoolDlg::OnExampleInit(int param) {
	FrStatic* pExample = DYNAMIC_CAST(FrStatic, param);

	if (pExample) {
		pExample->SetVisible(false);
	}
}

// schooldlg.cpp:192
void FrSchoolDlg::OnTypeInit(int param) {
	m_pTypeCombo = DYNAMIC_CAST(FrComboBox, param);
}

// schooldlg.cpp:198
void FrSchoolDlg::OnTypeListLBtnDown(int param) {
	const char* text = m_pTypeCombo->GetLine(1, false);

	if (strcmp(text, m_typeName.c_str()) == 0) {
		return;
	}




	m_typeNo = 5;

	for (int i = 0; i < 4; ++i) {

		if (strcmp(text, szJpSchoolType[i].c_str()) == 0) {

			m_typeNo = i + 1;
			m_typeName = text;
			break;
		}
	}

	if (m_typeNo > 4) {
		return;
	}


	m_dist0Name = "";
	m_dist1Name = "";
	m_schoolName = "";

	m_dist0No = 0;
	m_dist1No = 0;
	m_schoolNo = 0;
	if (m_pDist0Combo) {

		m_pDist0Combo->SetLine(1, "", 0, false, 0);
		m_pDist0Combo->ClearList();



		for (std::list<sJpSchool>::iterator it = m_schoolListA.begin(); it != m_schoolListA.end(); ++it) {

			if (((*it).no >> 28 & 7) == m_typeNo) {
				m_pDist0Combo->AddString((*it).name.c_str());
			}
		}
	}

	if (m_pDist1Combo) {

		m_pDist1Combo->SetLine(1, "", 0, false, 0);
		m_pDist1Combo->ClearList();
	}

	if (m_pSchoolCombo) {

		m_pSchoolCombo->SetLine(1, "", 0, false, 0);
		m_pSchoolCombo->ClearList();
	}

	m_dist1Name = "";
	m_schoolName = "";

	m_dist1No = 0;
	m_schoolNo = 0;

	switch (m_typeNo) {
	case 3:
	case 4:
		if (m_pDist1Combo) {
			m_pDist1Combo->SetVisible(false);
		}
		if (m_pDist1Static) {
			m_pDist1Static->SetVisible(false);
		}
		break;

	default:
		if (m_pDist1Combo) {
			m_pDist1Combo->SetVisible(true);
		}
		if (m_pDist1Static) {
			m_pDist1Static->SetVisible(true);
		}
		break;
	}
}

// schooldlg.cpp:279
void FrSchoolDlg::OnDist0Init(int param) {
	m_pDist0Combo = DYNAMIC_CAST(FrComboBox, param);
}

// schooldlg.cpp:285
void FrSchoolDlg::OnDist0ListLBtnDown(int param) {
	const char* text = m_pDist0Combo->GetLine(1, false);

	if (strcmp(text, m_dist0Name.c_str()) == 0) {
		return;
	}




	std::list<sJpSchool>::iterator it;
	for (it = m_schoolListA.begin(); it != m_schoolListA.end(); ++it) {




		if (((*it).no >> 28 & 7) == m_typeNo && strcmp(text, (*it).name.c_str()) == 0) {

			m_dist0No = (*it).no;
			m_dist0Name = text;
			break;
		}
	}

	if (it == m_schoolListA.end()) {
		return;
	}


	switch (m_typeNo) {
	case 3:
	case 4:
		m_schoolName = "";
		m_schoolNo = 0;

		if (m_pSchoolCombo) {

			m_pSchoolCombo->SetLine(1, "", 0, false, 0);
			m_pSchoolCombo->ClearList();

			for (it = m_schoolListC.begin(); it != m_schoolListC.end(); ++it) {


				if (((*it).no >> 28 & 7) == m_typeNo && (((*it).no ^ m_dist0No) & 0xfe00000) == 0) {

					m_pSchoolCombo->AddString((*it).name.c_str());
				}
			}
		}
		break;

	default:
		m_dist1Name = "";
		m_schoolName = "";

		m_dist1No = 0;
		m_schoolNo = 0;
		if (m_pDist1Combo) {

			m_pDist1Combo->SetLine(1, "", 0, false, 0);
			m_pDist1Combo->ClearList();

			for (it = m_schoolListB.begin(); it != m_schoolListB.end(); ++it) {


				if (((*it).no >> 28 & 7) == m_typeNo && (((*it).no ^ m_dist0No) & 0xfe00000) == 0) {

					m_pDist1Combo->AddString((*it).name.c_str());
				}
			}
		}

		if (m_pSchoolCombo) {

			m_pSchoolCombo->SetLine(1, "", 0, false, 0);
			m_pSchoolCombo->ClearList();
		}
		break;
	}
}

// schooldlg.cpp:364
void FrSchoolDlg::OnDist1Init(int param) {
	m_pDist1Combo = DYNAMIC_CAST(FrComboBox, param);
}

// schooldlg.cpp:370
void FrSchoolDlg::OnDist1ListLBtnDown(int param) {
	const char* text = m_pDist1Combo->GetLine(1, false);

	if (strcmp(text, m_dist1Name.c_str()) == 0) {
		return;
	}




	std::list<sJpSchool>::iterator it;
	for (it = m_schoolListB.begin(); it != m_schoolListB.end(); ++it) {





		if (((*it).no >> 28 & 7) == m_typeNo && (((*it).no ^ m_dist0No) & 0xfe00000) == 0
			&& strcmp(text, (*it).name.c_str()) == 0) {

			m_dist1No = (*it).no;
			m_dist1Name = text;
			break;
		}
	}

	// The original tests the end of m_schoolListA, not of the list searched.
	if (it == m_schoolListA.end()) {
		return;
	}


	m_schoolName = "";
	m_schoolNo = 0;

	if (m_pSchoolCombo) {

		m_pSchoolCombo->SetLine(1, "", 0, false, 0);
		m_pSchoolCombo->ClearList();

		for (it = m_schoolListC.begin(); it != m_schoolListC.end(); ++it) {

			if (((*it).no >> 28 & 7) == m_typeNo && (((*it).no ^ m_dist0No) & 0xfe00000) == 0
				&& (((*it).no ^ m_dist1No) & 0x1fe000) == 0) {

				m_pSchoolCombo->AddString((*it).name.c_str());
			}
		}
	}
}

// schooldlg.cpp:421
void FrSchoolDlg::OnStaticDist1Init(int param) {
	m_pDist1Static = DYNAMIC_CAST(FrStatic, param);
}

// schooldlg.cpp:427
void FrSchoolDlg::OnSearchInit(int param) {
	FrButton* pSearch = DYNAMIC_CAST(FrButton, param);

	if (pSearch) {
		pSearch->SetVisible(false);
	}
}

// schooldlg.cpp:436
bool FrSchoolDlg::OnInit(void) {


	if (m_pTypeCombo) {

		for (int i = 0; i < 4; ++i) {
			m_pTypeCombo->AddString(szJpSchoolType[i].c_str());
		}
	}

	cFile* pFile = g_resrcmng->GetCFile("school.txt", 0xffff);
	if (pFile == 0) {
		return true;
	}




	sJpSchool school;

	m_schoolListA.clear();

	int no;
	char name[1024];
	while (pFile->Scan("%d %s", &no, name)) {

		school.no = no;
		school.name = name;

		if (no & 0x1fff) {
			m_schoolListC.push_back(school);
		} else if (no & 0x1fe000) {
			m_schoolListB.push_back(school);
		} else if (no & 0xfe00000) {
			m_schoolListA.push_back(school);
		}
	}

	CloseCFile(pFile);

	return true;
}
