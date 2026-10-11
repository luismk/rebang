// Original: d:\build\custom\temp\code\client\projectg\user_info.cpp
#include <algorithm>
#include "NetworkSystem.h"
#include "user_info.h"
#include "AddFriendDlg.h"
#include "ObjectFactory.h"
#include "commonutil.h"
#include "frarea.h"
#include "fredit.h"
#include "frlistbox.h"
#include "frstatic.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "SharedDoc.h"
#include "exhibition.h"
#include "partsinfo.h"
#include "UccManager.h"
#include "parttidlist.h"
#include "frgaugebar.h"
#include "frgraphicinterface.h"
#include "wresrcmng.h"
#include "../client/Wangreal/include/wfont.h"
#include "../shared/Localize.h"
#include "Packet.h"
#include "TaskManager.h"
#include "actor.h"
#include "GolfTask.h"

extern unsigned long g_CurrentTime;

// user_info.cpp:40 (run-time type and factory registration of FrUserInfoForm)
const WRTTI FrUserInfoForm::m_RTTI("FrUserInfoForm", &FrForm::m_RTTI);

IObject* FrUserInfoFormMakeInstance(void) {
	return new FrUserInfoForm;
}

struct __sFrUserInfoForm {
	__sFrUserInfoForm(void) {
		ObjectFactory().AddObjectFunctor(FrUserInfoFormMakeInstance, "FrUserInfoForm");
	}
};
static __sFrUserInfoForm s_registerFrUserInfoForm;

// user_info.cpp:42
const sFRESH_MSGMAP* FrUserInfoForm::GetMessageMap(void) const {
	return &FrUserInfoForm::_MsgMap;
}

// user_info.cpp:263
FrUserInfoForm::FrUserInfoForm(void) {
	m_seasonTab = SEASON_CURRENT;

	memset(&m_pCurSeasonBtn, 0, sizeof(FrButton*) * 2);

	memset(m_pTabBtn, 0, sizeof(m_pTabBtn));
	memset(&m_pGeneralBack, 0, sizeof(FrArea*) * 7);
	memset(&m_pIdEdit, 0, sizeof(FrEdit*) * 8);
	memset(m_pSpecialInfoWnd, 0, sizeof(m_pSpecialInfoWnd));

	memset(&m_pRecordNormalBtn, 0, sizeof(FrButton*) * 4);

	memset(m_pSpecialInfoWnd, 0, sizeof(m_pSpecialInfoWnd));
	memset(m_pRecordEdit, 0, sizeof(m_pRecordEdit));
	memset(&m_pTrophyNormalBtn, 0, sizeof(FrButton*) * 2);
	memset(m_pGuildInfoEdit, 0, sizeof(m_pGuildInfoEdit));
	memset(&m_pRecTotalBack01Bmp, 0, sizeof(const Bitmap*) * 3);
	memset(m_pRecAwardWnd, 0, sizeof(m_pRecAwardWnd));

	m_pRecTotalList = NULL;
	m_pLevelArea = NULL;
	m_pClubArea = NULL;
	m_pDisconnQuestionMark = NULL;
	m_trophyListState = 0;
	m_pTrophySpecialList = NULL;
	m_pTrophyNormalList = NULL;
	m_pNoTrophyStatic = NULL;
	m_pTrophyNormalNum = NULL;
	m_pTrophySpecialNum = NULL;
	m_pGuildNoTrophyStatic = NULL;
	m_recordType = 0;
	m_trophyType = 0;
	m_pGuildTrophyList = NULL;

	m_seasonRecord.Clear();
	m_totalRecord.Clear();

	memset(&m_userInfo, 0, sizeof(sUserInfo));
	m_userInfo.m_guid = 0xFFFFFFFF;
	m_userInfo.m_uid = 0xFFFFFFFF;

	m_pFriendBtn = NULL;
	m_pWhisperBtn = NULL;
	m_pIgnoreBtn = NULL;
	m_pPetArea = NULL;
	m_pPetExhibition = NULL;
	m_pPetLoadGauge = NULL;
	m_pPetTids = NULL;
	m_pCommentEdit = NULL;
	m_pNickArea = NULL;

	m_pTitleIconWnd = NULL;
	m_pTitleWnd = NULL;
	m_pTitleTextWnd = NULL;
	m_pTitleBackWnd = NULL;

	m_pScoreList = NULL;
	m_pTitleFont = NULL;
	m_pMyGuildArea = NULL;
	m_pGuildMarkArea = NULL;
	m_pGuildTrophyBmp = NULL;
	m_bOption = false;
	m_bMsnControl = false;
	m_bControlOff = false;
	m_pGhostWnd = NULL;
	m_pGhostListWnd = NULL;
	memset(m_friendExtra.data, 0, sizeof(m_friendExtra.data));
	m_friendExtra.index = 0xFF;
	m_friendExtra.code = 0xFFFF;
	m_friendExtra.id1 = 0xFFFFFFFF;
	m_friendExtra.id2 = 0xFFFFFFFF;

	m_bPetShown = false;

	// clears 0x58C bytes from here, through 0x768 inside m_userInfo
	memset(m_pMatchHistoryBtn, 0, 0x58C);

	m_pSpecList = NULL;
	m_bMatchHistoryRequested = false;
	m_bFriendRequested = false;
	m_bCheckTime = false;
	m_lastElapsed = 0;
	m_requestedSeason = 0;
	m_pRmrVisitorBtn = NULL;
	m_recvSeasonFlags = 0;
	m_seasonPending = 0;
	memset(m_classicMapStats, 0, sizeof(m_classicMapStats));
	for (int i = 0; i < 20; i++) {
		m_classicMapStats[i].m_bestScore = 0x7F;
	}
	memset(m_seasonClassicMapStats, 0, sizeof(m_seasonClassicMapStats));
	for (int j = 0; j < 20; j++) {
		m_seasonClassicMapStats[j].m_bestScore = 0x7F;
	}
	memset(m_totalClassicMapStats, 0, sizeof(m_totalClassicMapStats));
	for (int k = 0; k < 20; k++) {
		m_totalClassicMapStats[k].m_bestScore = 0x7F;
	}
	memset(m_matchHistoryState, 0, sizeof(m_matchHistoryState));

	m_addFriendDlg = NULL;
}

// user_info.cpp:372
FrUserInfoForm::~FrUserInfoForm(void) {
	Clear();
}

// user_info.cpp:386
void FrUserInfoForm::Clear(void) {
	if (CUserInfo::Instance()) {
		CUserInfo::Instance()->m_dlg = NULL;
	}

	g_resrcmng->UnlockAllByThread(m_anotherHand.GetThreadID());
	m_anotherHand.BreakWorkingThread();
	HidePet(true);

	if (g_resrcmng && m_pTitleFont) {
		g_resrcmng->Release(m_pTitleFont);
		m_pTitleFont = NULL;
	}
}

// user_info.cpp:404
void FrUserInfoForm::OnTitleSeasonBtnInit(int param) {
	m_pCurSeasonBtn = DYNAMIC_CAST(FrButton, param);

	if (m_seasonTab == SEASON_CURRENT) {
		m_pCurSeasonBtn->SetStatus(FrButton::PRESSED);
	} else {
		m_pCurSeasonBtn->SetStatus(FrButton::NORMAL);
	}
}

// user_info.cpp:414
void FrUserInfoForm::OnTitleSeasonBtnUp(void) {
	if (!(m_recvSeasonFlags & 4)) {
		RequestSeasonData(5);
	}
	OnTitleSeasonVisible();
}

// user_info.cpp:423
void FrUserInfoForm::OnTitleTotalBtnInit(int param) {
	m_pTotalSeasonBtn = DYNAMIC_CAST(FrButton, param);

	if (m_seasonTab == SEASON_TOTAL) {
		m_pTotalSeasonBtn->SetStatus(FrButton::PRESSED);
	} else {
		m_pTotalSeasonBtn->SetStatus(FrButton::NORMAL);
	}
}

// user_info.cpp:433
void FrUserInfoForm::OnTitleTotalBtnUp(void) {
	if (!(m_recvSeasonFlags & 2)) {
		RequestSeasonData(0);
	}

	m_seasonTab = SEASON_TOTAL;
	m_pCurSeasonBtn->SetStatus(FrButton::NORMAL);
	m_pTotalSeasonBtn->SetStatus(FrButton::PRESSED);

	SetContent(true);
	SetTrophyStatic();

	SetSpecListBoxItem(SEASON_TOTAL);
}

// user_info.cpp:451
void FrUserInfoForm::SetTrophyStatic(void) {
	if (m_trophyType == 1) {
		if (m_pNoTrophyStatic) {
			m_pNoTrophyStatic->SetVisible(m_curTab == TAB_TROPHY_INFO && m_pTrophySpecialList->m_itemList.size() == 0);
		}
	} else if (m_trophyType == 0) {
		if (m_pNoTrophyStatic) {
			m_pNoTrophyStatic->SetVisible(m_curTab == TAB_TROPHY_INFO && m_pTrophyNormalList->m_itemList.size() == 0);
		}
	}

	if (m_pGuildNoTrophyStatic) {
		m_pGuildNoTrophyStatic->SetVisible(m_curTab == TAB_GUILD_INFO && m_pGuildTrophyList->m_itemList.size() == 0);
	}
}

// user_info.cpp:471
void FrUserInfoForm::OnGeneralInfoInit(int param) {
	m_pTabBtn[TAB_GENERAL_INFO] = DYNAMIC_CAST(FrButton, param);
}

// user_info.cpp:476
void FrUserInfoForm::OnGeneralInfoDown(void) {
	SetFormA(TAB_GENERAL_INFO);
}

// user_info.cpp:481
void FrUserInfoForm::OnSpecialInfoInit(int param) {
	m_pTabBtn[TAB_SPECIAL_INFO] = DYNAMIC_CAST(FrButton, param);
}

// user_info.cpp:486
void FrUserInfoForm::OnSpecialInfoDown(void) {
	SetFormA(TAB_SPECIAL_INFO);
}

// user_info.cpp:491
void FrUserInfoForm::OnRecordInfoInit(int param) {
	m_pTabBtn[TAB_RECORD_INFO] = DYNAMIC_CAST(FrButton, param);
}

// user_info.cpp:496
void FrUserInfoForm::OnRecordInfoDown(void) {
	SetFormA(TAB_RECORD_INFO);
}

// user_info.cpp:501
void FrUserInfoForm::OnTrophyInfoInit(int param) {
	m_pTabBtn[TAB_TROPHY_INFO] = DYNAMIC_CAST(FrButton, param);
}

// user_info.cpp:506
void FrUserInfoForm::OnTrophyInfoDown(void) {
	SetFormA(TAB_TROPHY_INFO);
}

// user_info.cpp:511
void FrUserInfoForm::OnGuildInfoInit(int param) {
	m_pTabBtn[TAB_GUILD_INFO] = DYNAMIC_CAST(FrButton, param);
}

// user_info.cpp:516
void FrUserInfoForm::OnGuildInfoDown(void) {
	SetFormA(TAB_GUILD_INFO);
}

// user_info.cpp:545
void FrUserInfoForm::OnGeneralBackInit(int param) {
	m_pGeneralBack = DYNAMIC_CAST(FrArea, param);
}

// user_info.cpp:550
void FrUserInfoForm::OnRecCourseBackInit(int param) {
	m_pRecCourseBack = DYNAMIC_CAST(FrArea, param);
}

// user_info.cpp:555
void FrUserInfoForm::OnTroNormalBackInit(int param) {
	m_pTroNormalBack = DYNAMIC_CAST(FrArea, param);
}

// user_info.cpp:560
void FrUserInfoForm::OnTroSpecialBackInit(int param) {
	m_pTroSpecialBack = DYNAMIC_CAST(FrArea, param);
}

// user_info.cpp:565
void FrUserInfoForm::OnGuildBackInit(int param) {
	m_pGuildBack = DYNAMIC_CAST(FrArea, param);
}

// user_info.cpp:607
void FrUserInfoForm::OnEditInit_GI_ID(int param) { m_pIdEdit = DYNAMIC_CAST(FrEdit, param); m_pIdEdit->EnableHover(true, 0.0f); }

// user_info.cpp:608
void FrUserInfoForm::OnEditInit_GI_SCHOOL(int param) { m_pSchoolEdit = DYNAMIC_CAST(FrEdit, param); m_pSchoolEdit->EnableHover(true, 0.0f); }

// user_info.cpp:609
void FrUserInfoForm::OnEditInit_GI_CLUB(int param) { m_pClubEdit = DYNAMIC_CAST(FrEdit, param); m_pClubEdit->EnableHover(true, 0.0f); }

// user_info.cpp:610
void FrUserInfoForm::OnEditInit_GI_EXP(int param) { m_pExpEdit = DYNAMIC_CAST(FrEdit, param); m_pExpEdit->EnableHover(true, 0.0f); }

// user_info.cpp:611
void FrUserInfoForm::OnEditInit_GI_CONNSTATUS(int param) { m_pConnStatusEdit = DYNAMIC_CAST(FrEdit, param); m_pConnStatusEdit->EnableHover(true, 0.0f); }

// user_info.cpp:615
void FrUserInfoForm::OnEditInit_GI_DISCONN(int param) { m_pDisconnEdit = DYNAMIC_CAST(FrEdit, param); m_pDisconnEdit->EnableHover(true, 0.0f); }

// user_info.cpp:617
void FrUserInfoForm::OnEditInit_GI_COMBO(int param) { m_pComboEdit = DYNAMIC_CAST(FrEdit, param); m_pComboEdit->EnableHover(true, 0.0f); }

// user_info.cpp:621
void FrUserInfoForm::OnEditInit_GI_QUIZLEVEL(int param) { m_pQuizLevelEdit = DYNAMIC_CAST(FrEdit, param); m_pQuizLevelEdit->EnableHover(true, 0.0f); }

// user_info.cpp:623
void FrUserInfoForm::OnEditInit_SI_TROPHY_GOLD(int param) { m_pTrophyGoldEdit = DYNAMIC_CAST(FrEdit, param); m_pTrophyGoldEdit->EnableHover(true, 0.0f); }

// user_info.cpp:624
void FrUserInfoForm::OnEditInit_SI_TROPHY_SILVER(int param) { m_pTrophySilverEdit = DYNAMIC_CAST(FrEdit, param); m_pTrophySilverEdit->EnableHover(true, 0.0f); }

// user_info.cpp:625
void FrUserInfoForm::OnEditInit_SI_TROPHY_BRONZE(int param) { m_pTrophyBronzeEdit = DYNAMIC_CAST(FrEdit, param); m_pTrophyBronzeEdit->EnableHover(true, 0.0f); }

// user_info.cpp:626
void FrUserInfoForm::OnEditInit_SI_MATCH_WIN_PERCENTAGE(int param) { m_pMatchWinPercentEdit = DYNAMIC_CAST(FrEdit, param); m_pMatchWinPercentEdit->EnableHover(true, 0.0f); }

// user_info.cpp:627
void FrUserInfoForm::OnEditInit_SI_AVG_SCORE(int param) { m_pAvgScoreEdit = DYNAMIC_CAST(FrEdit, param); m_pAvgScoreEdit->EnableHover(true, 0.0f); }

// user_info.cpp:628
void FrUserInfoForm::OnEditInit_SI_MEAN_SHOTTIME(int param) { m_pMeanShotTimeEdit = DYNAMIC_CAST(FrEdit, param); m_pMeanShotTimeEdit->EnableHover(true, 0.0f); }

// user_info.cpp:629
void FrUserInfoForm::OnEditInit_SI_PANGYA_PERCENTAGE(int param) { m_pPangyaPercentEdit = DYNAMIC_CAST(FrEdit, param); m_pPangyaPercentEdit->EnableHover(true, 0.0f); }

// user_info.cpp:630
void FrUserInfoForm::OnEditInit_SI_FAIRWAY_PERCENTAGE(int param) { m_pFairwayPercentEdit = DYNAMIC_CAST(FrEdit, param); m_pFairwayPercentEdit->EnableHover(true, 0.0f); }

// user_info.cpp:631
void FrUserInfoForm::OnEditInit_SI_PUTT_PERCENTAGE(int param) { m_pPuttPercentEdit = DYNAMIC_CAST(FrEdit, param); m_pPuttPercentEdit->EnableHover(true, 0.0f); }

// user_info.cpp:632
void FrUserInfoForm::OnEditInit_SI_OB_PERCENTAGE(int param) { m_pObPercentEdit = DYNAMIC_CAST(FrEdit, param); m_pObPercentEdit->EnableHover(true, 0.0f); }

// user_info.cpp:633
void FrUserInfoForm::OnEditInit_SI_TOTAL_SKINS_PANG(int param) { m_pTotalSkinsPangEdit = DYNAMIC_CAST(FrEdit, param); m_pTotalSkinsPangEdit->EnableHover(true, 0.0f); }

// user_info.cpp:644
void FrUserInfoForm::OnEditInit_RI_TOT_PLAYTIME(int param) { m_pRecordEdit[0] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[0]->EnableHover(true, 0.0f); }

// user_info.cpp:645
void FrUserInfoForm::OnEditInit_RI_TOT_SHOTS(int param) { m_pRecordEdit[1] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[1]->EnableHover(true, 0.0f); }

// user_info.cpp:646
void FrUserInfoForm::OnEditInit_RI_TOT_HOLES(int param) { m_pRecordEdit[2] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[2]->EnableHover(true, 0.0f); }

// user_info.cpp:647
void FrUserInfoForm::OnEditInit_RI_TOT_DRIVE(int param) { m_pRecordEdit[4] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[4]->EnableHover(true, 0.0f); }

// user_info.cpp:648
void FrUserInfoForm::OnEditInit_RI_EARTH_ROUND(int param) { m_pRecordEdit[3] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[3]->EnableHover(true, 0.0f); }

// user_info.cpp:649
void FrUserInfoForm::OnEditInit_RI_BEST_SCORE_5(int param) { m_pRecordEdit[5] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[5]->EnableHover(true, 0.0f); }

// user_info.cpp:650
void FrUserInfoForm::OnEditInit_RI_BEST_SCORE_4(int param) { m_pRecordEdit[6] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[6]->EnableHover(true, 0.0f); }

// user_info.cpp:651
void FrUserInfoForm::OnEditInit_RI_BEST_SCORE_3(int param) { m_pRecordEdit[7] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[7]->EnableHover(true, 0.0f); }

// user_info.cpp:652
void FrUserInfoForm::OnEditInit_RI_BEST_SCORE_2(int param) { m_pRecordEdit[8] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[8]->EnableHover(true, 0.0f); }

// user_info.cpp:653
void FrUserInfoForm::OnEditInit_RI_BEST_SCORE_1(int param) { m_pRecordEdit[9] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[9]->EnableHover(true, 0.0f); }

// user_info.cpp:654
void FrUserInfoForm::OnEditInit_RI_TOT_HOLEINONES(int param) { m_pRecordEdit[10] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[10]->EnableHover(true, 0.0f); }

// user_info.cpp:655
void FrUserInfoForm::OnEditInit_RI_TOT_ALBATROSS(int param) { m_pRecordEdit[11] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[11]->EnableHover(true, 0.0f); }

// user_info.cpp:656
void FrUserInfoForm::OnEditInit_RI_LONGEST_DRIVE(int param) { m_pRecordEdit[12] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[12]->EnableHover(true, 0.0f); }

// user_info.cpp:657
void FrUserInfoForm::OnEditInit_RI_LONGEST_PUTT(int param) { m_pRecordEdit[13] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[13]->EnableHover(true, 0.0f); }

// user_info.cpp:658
void FrUserInfoForm::OnEditInit_RI_LONGEST_CHIPIN(int param) { m_pRecordEdit[14] = DYNAMIC_CAST(FrEdit, param); m_pRecordEdit[14]->EnableHover(true, 0.0f); }

// user_info.cpp:660
void FrUserInfoForm::OnEditInit_GUILD_NAME(int param) { m_pGuildNameEdit = DYNAMIC_CAST(FrEdit, param); m_pGuildNameEdit->EnableHover(true, 0.0f); }

// user_info.cpp:661
void FrUserInfoForm::OnEditInit_GUILD_PANG(int param) { m_pGuildPangEdit = DYNAMIC_CAST(FrEdit, param); m_pGuildPangEdit->EnableHover(true, 0.0f); }

// user_info.cpp:662
void FrUserInfoForm::OnEditInit_GUILD_MYPERCENT(int param) { m_pGuildMyPercentEdit = DYNAMIC_CAST(FrEdit, param); m_pGuildMyPercentEdit->EnableHover(true, 0.0f); }

// user_info.cpp:668
void FrUserInfoForm::OnAreaInit_GI_Level(int param) {
	m_pLevelArea = DYNAMIC_CAST(FrArea, param);
	if (m_pLevelArea) {
		m_pLevelArea->SetVisible(false);
	}
}

// user_info.cpp:675
void FrUserInfoForm::OnAreaInit_GI_ClUB(int param) {
	m_pClubArea = DYNAMIC_CAST(FrArea, param);
	if (m_pClubArea) {
		m_pClubArea->SetVisible(false);
	}
}

// user_info.cpp:682
void FrUserInfoForm::OnAreaInit_Question_mark(int param) {
	m_pDisconnQuestionMark = DYNAMIC_CAST(FrArea, param);

	if (m_pDisconnQuestionMark) {
		// "The disconnect rate shows once you reach Beginner E or play more than 50 games."
		m_pDisconnQuestionMark->SetToolTipText(std::string("\xb0\xad\xc1\xa6\xc1\xbe\xb7\xe1\xc0\xb2\xc0\xba \xba\xf1\xb1\xe2\xb3\xca" "E\xb0\xa1 \xb5\xc7\xb0\xc5\xb3\xaa \xb0\xd4\xc0\xd3\xc8\xbd\xbc\xf6 50\xc8\xb8\xb8\xa6 \xc3\xca\xb0\xfa\xc7\xd8\xbe\xdf \xba\xb8\xc0\xcc\xb0\xd4 \xb5\xcb\xb4\xcf\xb4\xd9."));
	}
	const Bitmap* pBmp = g_pFresh->GetManager()->GetBitmap("USER_INFO", "user_info_question_mark");
	if (pBmp) {
		if (m_pDisconnQuestionMark) {
			m_pDisconnQuestionMark->SetBgImg(pBmp);
			m_pDisconnQuestionMark->SetVisible(false);
		}
	}
}

// user_info.cpp:729
void FrUserInfoForm::OnRecordNormalBtnInit(int param) {
	m_pRecordNormalBtn = DYNAMIC_CAST(FrButton, param);

	if (m_recordType == 0) {
		m_pRecordNormalBtn->SetStatus(FrButton::PRESSED);
	} else {
		m_pRecordNormalBtn->SetStatus(FrButton::NORMAL);
	}
}

// user_info.cpp:739
void FrUserInfoForm::OnRecordNormalBtnUp(void) {
	m_recordType = 0;
	m_pRecordNormalBtn->SetStatus(FrButton::PRESSED);
	m_pRecordCourseBtn->SetStatus(FrButton::NORMAL);
	m_pRecordClassicBtn->SetStatus(FrButton::NORMAL);

	ChangeRecordType();
}

// user_info.cpp:778
void FrUserInfoForm::OnRecordCourseBtnInit(int param) {
	m_pRecordCourseBtn = DYNAMIC_CAST(FrButton, param);

	if (m_recordType == 2) {
		m_pRecordCourseBtn->SetStatus(FrButton::PRESSED);
	} else {
		m_pRecordCourseBtn->SetStatus(FrButton::NORMAL);
	}
}

// user_info.cpp:788
void FrUserInfoForm::OnRecordCourseBtnUp(void) {
	m_recordType = 2;
	m_pRecordCourseBtn->SetStatus(FrButton::PRESSED);
	m_pRecordNormalBtn->SetStatus(FrButton::NORMAL);
	m_pRecordClassicBtn->SetStatus(FrButton::NORMAL);

	ChangeRecordType();
	UpdateEditMapStatistics(m_seasonTab, false);
}

// user_info.cpp:804
void FrUserInfoForm::OnRecordClassicBtnInit(int param) {
	m_pRecordClassicBtn = DYNAMIC_CAST(FrButton, param);

	if (m_recordType == 3) {
		m_pRecordClassicBtn->SetStatus(FrButton::PRESSED);
	} else {
		m_pRecordClassicBtn->SetStatus(FrButton::NORMAL);
	}
}

// user_info.cpp:818
void FrUserInfoForm::OnRecordClassicBtnUp(void) {
	m_recordType = 3;
	m_pRecordClassicBtn->SetStatus(FrButton::PRESSED);
	m_pRecordCourseBtn->SetStatus(FrButton::NORMAL);
	m_pRecordNormalBtn->SetStatus(FrButton::NORMAL);

	ChangeRecordType();
	UpdateEditMapStatistics(m_seasonTab, true);
}

// user_info.cpp:838
void FrUserInfoForm::OnEditInit_SI_MATCH_HISTORY1(int param) { m_pMatchHistoryEdit[0] = DYNAMIC_CAST(FrEdit, param); m_pMatchHistoryEdit[0]->EnableHover(true, 0.0f); }

// user_info.cpp:839
void FrUserInfoForm::OnEditInit_SI_MATCH_HISTORY2(int param) { m_pMatchHistoryEdit[1] = DYNAMIC_CAST(FrEdit, param); m_pMatchHistoryEdit[1]->EnableHover(true, 0.0f); }

// user_info.cpp:840
void FrUserInfoForm::OnEditInit_SI_MATCH_HISTORY3(int param) { m_pMatchHistoryEdit[2] = DYNAMIC_CAST(FrEdit, param); m_pMatchHistoryEdit[2]->EnableHover(true, 0.0f); }

// user_info.cpp:841
void FrUserInfoForm::OnEditInit_SI_MATCH_HISTORY4(int param) { m_pMatchHistoryEdit[3] = DYNAMIC_CAST(FrEdit, param); m_pMatchHistoryEdit[3]->EnableHover(true, 0.0f); }

// user_info.cpp:842
void FrUserInfoForm::OnEditInit_SI_MATCH_HISTORY5(int param) { m_pMatchHistoryEdit[4] = DYNAMIC_CAST(FrEdit, param); m_pMatchHistoryEdit[4]->EnableHover(true, 0.0f); }

// user_info.cpp:845
void FrUserInfoForm::OnSpecListboxInit(int param) {
	m_pSpecList = DYNAMIC_CAST(FrListBox, param);
	m_pSpecList->SetVisible(false);

	m_pSpecBackBmp = g_pFresh->RegisterBitmap("special_back");
	m_pSpecBack00Bmp = g_pFresh->RegisterBitmap("special_back00");

	m_pSpecList->AddItem((void*)m_pSpecBackBmp);
	m_pSpecList->AddItem((void*)m_pSpecBack00Bmp);
}

// user_info.cpp:856
void FrUserInfoForm::OnMatchHistoryBtn1Init(int param) { m_pMatchHistoryBtn[0] = DYNAMIC_CAST(FrButton, param); }

// user_info.cpp:857
void FrUserInfoForm::OnMatchHistoryBtn2Init(int param) { m_pMatchHistoryBtn[1] = DYNAMIC_CAST(FrButton, param); }

// user_info.cpp:858
void FrUserInfoForm::OnMatchHistoryBtn3Init(int param) { m_pMatchHistoryBtn[2] = DYNAMIC_CAST(FrButton, param); }

// user_info.cpp:859
void FrUserInfoForm::OnMatchHistoryBtn4Init(int param) { m_pMatchHistoryBtn[3] = DYNAMIC_CAST(FrButton, param); }

// user_info.cpp:860
void FrUserInfoForm::OnMatchHistoryBtn5Init(int param) { m_pMatchHistoryBtn[4] = DYNAMIC_CAST(FrButton, param); }

// user_info.cpp:862
void FrUserInfoForm::OnMatchHistoryBtn1Up(void) { MatchHistoryUserBtnUp(0); }
// user_info.cpp:863
void FrUserInfoForm::OnMatchHistoryBtn2Up(void) { MatchHistoryUserBtnUp(1); }
// user_info.cpp:864
void FrUserInfoForm::OnMatchHistoryBtn3Up(void) { MatchHistoryUserBtnUp(2); }
// user_info.cpp:865
void FrUserInfoForm::OnMatchHistoryBtn4Up(void) { MatchHistoryUserBtnUp(3); }
// user_info.cpp:866
void FrUserInfoForm::OnMatchHistoryBtn5Up(void) { MatchHistoryUserBtnUp(4); }

// user_info.cpp:881
void FrUserInfoForm::MatchHistoryUserBtnUp(int index) {
	WSendPacket packet((enumClientPacket)0x2F);
	packet.Encode4(m_matchHistory[index].uid);
	packet.Encode1(5);
	packet.Send(SEND_TO_DEFAULT);

	m_bMatchHistoryRequested = true;
	m_pMatchHistoryBtn[index]->SetStatus(FrButton::NORMAL);
	CUserInfo::Instance()->m_nick = m_matchHistory[index].nickname;
}

// user_info.cpp:899
void FrUserInfoForm::MsnDisconExeception(float elapsed, FrListItem *pItem) {
	static int s_blinkCount;      // the notice's blinks so far
	static float s_blinkTime;     // time into the current blink

	if (!m_bMatchHistoryRequested)
		return;

	// The notice shows for the first 0.7 s of every second, three times.
	s_blinkTime += elapsed;
	if (s_blinkTime < 0.7f) {
		FrGraphicInterface *pGDI = g_pFresh->GetManager()->GetGDI();
		if (pGDI == NULL || pItem == NULL)
			return;

		pGDI->SetTextColor(0xFFC0125C, 0xFFFFFFFF);
		pGDI->SetTextStyle(2);

		g_pFresh->GetManager()->PrintText(WPoint(pItem->pos.x + 37.0f, pItem->pos.y + 212.0f), 0, "\xbc\xad\xb9\xf6\xc0\xc7\x20\xc0\xdb\xbe\xf7\xb7\xae\xc0\xcc\x20\xb8\xb9\xbd\xc0\xb4\xcf\xb4\xd9\x2e", -1.0f, 0xFFFFFFFF);
		g_pFresh->GetManager()->PrintText(WPoint(pItem->pos.x + 37.0f, pItem->pos.y + 228.0f), 0, "\xc0\xe1\xbd\xc3\x20\xc8\xc4\x20\xb4\xd9\xbd\xc3\x20\xbd\xc3\xb5\xb5\xc7\xd8\x20\xc1\xd6\xbd\xca\xbd\xc3\xbf\xc0\x2e", -1.0f, 0xFFFFFFFF);
	}

	if (s_blinkTime > 1.0f) {
		s_blinkTime = 0.0f;
		s_blinkCount++;

		if (s_blinkCount > 3) {
			s_blinkCount = 0;
			m_bMatchHistoryRequested = false;
		}
	}
}

// user_info.cpp:942
void FrUserInfoForm::SetMatchHistoryContents(void) {
	char text[128] = "";

	if (CMatchHistory::Instance()->IsUpdated() == true) {
		for (int i = 0; i < 5; i++) {
			if (m_matchHistory[i].uid != 0)
				m_pMatchHistoryEdit[i]->SetLine(1, m_matchHistory[i].nickname, 0xFF000000, false, 0);
			else
				m_pMatchHistoryEdit[i]->SetLine(1, "\xb1\xe2\xb7\xcf\x20\xbe\xf8\xc0\xbd", 0xFF000000, false, 0);
		}
	} else {
		const char *unavailable = "\xc8\xae\xc0\xce\x20\xba\xd2\xb0\xa1\x2e";
		strcpy(text, unavailable);
		for (int i = 0; i < 5; i++)
			m_pMatchHistoryEdit[i]->SetLine(1, text, 0xFF000000, false, 0);
	}
}

// user_info.cpp:974
void FrUserInfoForm::SetSpecListBoxItem(eSeasonType season) {
	if (strcmp(m_userInfo.m_name, WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().nickname) == 0) {
		if (m_pTabBtn[TAB_RECORD_INFO]->GetStatus() == FrButton::PRESSED) {
			if (m_pRecordClassicBtn) {
				m_pRecordClassicBtn->SetVisible(true);
			}
		}
	}
}

// user_info.cpp:1216
void FrUserInfoForm::OnRecTotalListboxInit(int param) {
	m_pRecTotalList = DYNAMIC_CAST(FrListBox, param);
	m_pRecTotalList->SetVisible(false);

	m_pRecTotalBack01Bmp = g_pFresh->RegisterBitmap("special_back01");
	m_pRecTotalBack02Bmp = g_pFresh->RegisterBitmap("special_back02");
	m_pRecTotalBack03Bmp = g_pFresh->RegisterBitmap("special_back03");
	m_pRecTotalBack02NdBmp = g_pFresh->RegisterBitmap("special_back02_nd");

	m_pRecTotalList->AddItem((void*)m_pRecTotalBack01Bmp);
	m_pRecTotalList->AddItem((void*)m_pRecTotalBack02Bmp);
	m_pRecTotalList->AddItem((void*)m_pRecTotalBack03Bmp);
}

// user_info.cpp:1391
void FrUserInfoForm::OnTrophyNormalNumInit(int param) {
	m_pTrophyNormalNum = DYNAMIC_CAST(FrEdit, param);
	m_pTrophyNormalNum->SetVisible(false);
}

// user_info.cpp:1397
void FrUserInfoForm::OnTrophySpecialNumInit(int param) {
	m_pTrophySpecialNum = DYNAMIC_CAST(FrEdit, param);
	m_pTrophySpecialNum->SetVisible(false);
}

// user_info.cpp:1403
void FrUserInfoForm::OnTrophyNormalBtnInit(int param) {
	m_pTrophyNormalBtn = DYNAMIC_CAST(FrButton, param);

	if (m_trophyType == 0) {
		m_pTrophyNormalBtn->SetStatus(FrButton::PRESSED);
	} else {
		m_pTrophyNormalBtn->SetStatus(FrButton::NORMAL);
	}
}

// user_info.cpp:1413
void FrUserInfoForm::OnTrophyNormalBtnUp(void) {
	m_trophyType = 0;
	m_pTrophyNormalBtn->SetStatus(FrButton::PRESSED);
	m_pTrophySpecialBtn->SetStatus(FrButton::NORMAL);

	ChangeTrophyType();
}

// user_info.cpp:1423
void FrUserInfoForm::OnTrophySpecialBtnInit(int param) {
	m_pTrophySpecialBtn = DYNAMIC_CAST(FrButton, param);

	if (m_trophyType == 1) {
		m_pTrophySpecialBtn->SetStatus(FrButton::PRESSED);
	} else {
		m_pTrophySpecialBtn->SetStatus(FrButton::NORMAL);
	}
}

// user_info.cpp:1433
void FrUserInfoForm::OnTrophySpecialBtnUp(void) {
	m_trophyType = 1;
	m_pTrophySpecialBtn->SetStatus(FrButton::PRESSED);
	m_pTrophyNormalBtn->SetStatus(FrButton::NORMAL);

	ChangeTrophyType();
}

// user_info.cpp:1443
void FrUserInfoForm::ChangeRecordType(void) {
	int i;

	if (m_recordType == 0) {
		if (m_pRecCourseBack)
			m_pRecCourseBack->SetVisible(false);

		m_pRecTotalList->SetVisible(true);
		m_pScoreList->SetVisible(false);
	} else if (m_recordType == 2 || m_recordType == 3) {
		if (m_pRecCourseBack)
			m_pRecCourseBack->SetVisible(true);

		for (i = 0; i < 15; i++) {
			if (m_pRecordEdit[i])
				m_pRecordEdit[i]->SetVisible(false);
		}

		m_pRecTotalList->SetVisible(false);
		m_pScoreList->SetVisible(true);
	} else if (m_recordType == 1) {
		if (m_pRecCourseBack)
			m_pRecCourseBack->SetVisible(false);
		if (m_pRecAwardBack)
			m_pRecAwardBack->SetVisible(true);

		for (i = 0; i < 6; i++) {
			if (m_pRecAwardWnd[i])
				m_pRecAwardWnd[i]->SetVisible(true);
		}
		for (i = 0; i < 15; i++) {
			if (m_pRecordEdit[i])
				m_pRecordEdit[i]->SetVisible(false);
		}

		m_pRecTotalList->SetVisible(false);
		m_pScoreList->SetVisible(false);
	}
}

// user_info.cpp:1492
void FrUserInfoForm::ChangeTrophyType(void) {
	if (m_trophyType == 1) {
		m_pTrophyNormalNum->SetVisible(false);
		m_pTrophySpecialNum->SetVisible(true);

		if (m_pTroNormalBack)
			m_pTroNormalBack->SetVisible(false);
		if (m_pTroSpecialBack)
			m_pTroSpecialBack->SetVisible(true);

		if (m_pTrophySpecialList) {
			m_pTrophySpecialList->SetVisible(true);

			if (m_pNoTrophyStatic)
				m_pNoTrophyStatic->SetVisible(m_pTrophySpecialList->m_itemList.size() == 0);
		}

		if (m_pTrophyNormalList)
			m_pTrophyNormalList->SetVisible(false);
	} else if (m_trophyType == 0) {
		m_pTrophyNormalNum->SetVisible(true);
		m_pTrophySpecialNum->SetVisible(false);

		if (m_pTroNormalBack)
			m_pTroNormalBack->SetVisible(true);
		if (m_pTroSpecialBack)
			m_pTroSpecialBack->SetVisible(false);

		if (m_pTrophySpecialList)
			m_pTrophySpecialList->SetVisible(false);

		if (m_pTrophyNormalList) {
			m_pTrophyNormalList->SetVisible(true);

			if (m_pNoTrophyStatic)
				m_pNoTrophyStatic->SetVisible(m_pTrophyNormalList->m_itemList.size() == 0);
		}
	}
}

// user_info.cpp:1538
void FrUserInfoForm::OnListBoxInit_TRI_SPECIALBOX(int param) {
	m_pTrophySpecialList = DYNAMIC_CAST(FrListBox, param);
}

// user_info.cpp:1609
void FrUserInfoForm::OnListBoxInit_TRI_NORMALBOX(int param) {
	m_pTrophyNormalList = DYNAMIC_CAST(FrListBox, param);
}

// user_info.cpp:1680
void FrUserInfoForm::OnStaticInit_TRI_NOTROPHY(int param) {
	m_pNoTrophyStatic = DYNAMIC_CAST(FrStatic, param);
	if (m_pNoTrophyStatic) {
		m_pNoTrophyStatic->SetVisible(false);
	}
}

// user_info.cpp:1703
void FrUserInfoForm::OnMyGuildInit(int param) {
	m_pMyGuildArea = DYNAMIC_CAST(FrArea, param);
	m_pGuildTrophyBmp = g_pFresh->GetManager()->GetBitmap("TROPHY", "guild");
}

// user_info.cpp:1751
void FrUserInfoForm::OnGuildTrophyListInit(int param) {
	m_pGuildTrophyList = DYNAMIC_CAST(FrListBox, param);
	if (m_pGuildTrophyList) {
		m_pGuildTrophyList->SetVisible(false);
	}
}

// user_info.cpp:1830
void FrUserInfoForm::OnStaticInit_GUILD_NOTROPHY(int param) {
	m_pGuildNoTrophyStatic = DYNAMIC_CAST(FrStatic, param);
	if (m_pGuildNoTrophyStatic) {
		m_pGuildNoTrophyStatic->SetVisible(false);
	}
}

// user_info.cpp:1842
void FrUserInfoForm::OnWhisperInit(int param) {
	m_pWhisperBtn = DYNAMIC_CAST(FrButton, param);
}

// user_info.cpp:1847
void FrUserInfoForm::OnNickInit(intptr_t param) {
	m_pNickArea = DYNAMIC_CAST(FrArea, param);
	m_pMaleBmp = g_pFresh->RegisterBitmap("i_male");
	m_pFemaleBmp = g_pFresh->RegisterBitmap("i_female");
}

// user_info.cpp:1934
void FrUserInfoForm::OnFriendInit(int param) {
	m_pFriendBtn = DYNAMIC_CAST(FrButton, param);
}

// user_info.cpp:1954
void FrUserInfoForm::OnIgnoreInit(intptr_t param) {
	m_pIgnoreBtn = DYNAMIC_CAST(FrButton, param);
}

// user_info.cpp:1959
void FrUserInfoForm::OnIgnoreUp(void) {
	if (m_userInfo.m_uid == 0xFFFFFFFF || m_userInfo.m_uid == MyUID())
		return;

	bool bIgnored = false;
	std::list<std::string>::iterator it = std::find(WSingleton<CSharedDoc>::Instance()->m_ignoreList.begin(), WSingleton<CSharedDoc>::Instance()->m_ignoreList.end(), m_userInfo.m_name);
	if (it == WSingleton<CSharedDoc>::Instance()->m_ignoreList.end()) {
		WSingleton<CSharedDoc>::Instance()->m_ignoreList.push_back(std::string(m_userInfo.m_name));
		bIgnored = true;
	} else {
		WSingleton<CSharedDoc>::Instance()->m_ignoreList.erase(it);
	}

	SetIgnoreButtonImg(bIgnored);
}

// user_info.cpp:1997
void FrUserInfoForm::SetIgnoreButtonImg(bool bIgnored) {
	if (bIgnored) {
		m_pIgnoreBtn->SetButtonImg("btn_i02_02_n", FrButton::NORMAL);
		m_pIgnoreBtn->SetButtonImg("btn_i02_02_o", FrButton::OVER);
	} else {
		m_pIgnoreBtn->SetButtonImg("btn_i02_n", FrButton::NORMAL);
		m_pIgnoreBtn->SetButtonImg("btn_i02_o", FrButton::OVER);
	}
	if (bIgnored) m_pIgnoreBtn->SetButtonImg("btn_i02_02_d", FrButton::PRESSED); else m_pIgnoreBtn->SetButtonImg("btn_i02_d", FrButton::PRESSED);
}

// user_info.cpp:2015
void FrUserInfoForm::OnCloseUp(void) {
	Close(FrNONE, true);
}

// user_info.cpp:2020
bool FrUserInfoForm::Close(eFormRet ret, bool bDestroy) {
	if (m_anotherHand.IsWorking(0))
		return false;

	return FrForm::Close(ret, bDestroy);
}

// user_info.cpp:2028
void FrUserInfoForm::OnProc(const float dt) {
	if (IsLocalContent((localContentType_t)0x57)) {
		RefreshAllUccClothes();
	}

	m_lastElapsed = dt;
	if (m_petLoadTime < 3.0f) {
		m_petLoadTime += dt;

		if (m_petLoadTime >= 3.0f || !m_anotherHand.IsWorking(0)) {
			m_pPetLoadGauge->SetVisible(false);
			m_petLoadTime = 3.0f;
		}
	}
}

// user_info.cpp:2048
void FrUserInfoForm::OnAnotherHand(int nWork, void *pData) {
	if (nWork == 0) {
		ShowPet();
	}
}

// user_info.cpp:2071
void FrUserInfoForm::RefreshAllUccClothes(void) {
	if (m_pPetTids && m_pPetExhibition && m_pPetExhibition->GetPetFrame()) {
		sUccRefreshItem item;

		for (int i = 0; i < 10; i++) {
			if (!UccManager()->GetRefreshItem(item)) {
				break;
			}

			unsigned long typeId = item.m_typeId;

			unsigned char slot = (unsigned char)((typeId >> 13) & 0x1F);
			if (m_pPetTids->m_partTids[slot] != typeId) {
				continue;
			}
			unsigned long serial = m_pPetTids->m_partIds[slot];

			sUccClothes *pClothes = UccManager()->FindClothes(serial);

			if (pClothes != 0 && strcmpi(pClothes->m_uccKey, item.m_uccKey) == 0) {
				if (m_pPetExhibition && m_pPetExhibition->GetPetFrame()) {
					m_pPetExhibition->GetPetFrame()->SetPart(m_pPetTids, pClothes->m_typeId, pClothes->m_serial);
					m_pPetExhibition->GetPetFrame()->SetPart(m_pPetTids, pClothes->m_typeId, pClothes->m_serial);
				}
			}
		}
	}
}

// user_info.cpp:2128
bool FrUserInfoForm::OnInit(void) {
	m_disconnEditRect = m_pDisconnEdit->GetRect();

	// Every page starts hidden.
	m_curTab = TAB_NONE;
	SetVisible(TAB_GENERAL_INFO, false);
	// One source line each in the original (user_info.cpp:2134, 2137): the
	// special and guild pages hidden by hand.
	SetVisible(m_pSpecialInfoWnd, 16, false);
	for (int i = 0; i < 5; i++) {
		m_pMatchHistoryBtn[i]->SetVisible(false);
	}
	m_pSpecList->SetVisible(false);
	SetVisible(TAB_RECORD_INFO, false);
	SetVisible(TAB_TROPHY_INFO, false);
	SetVisible(m_pGuildInfoEdit, 3, false);
	m_pMyGuildArea->SetVisible(false);
	if (m_pGuildBack) {
		m_pGuildBack->SetVisible(false);
	}
	if (m_pGuildTrophyList) {
		m_pGuildTrophyList->SetVisible(false);
	}
	m_pGuildNoTrophyStatic->SetVisible(false);

	m_petLoadTime = 0.0f;
	m_pIdEdit->HidePrivacy(true);

	m_pIdEdit->DisableEditStyle(FrEdit::ES_EMOTICON);

	return true;
}

// user_info.cpp:2158
void FrUserInfoForm::SetFormA(eTabType tab) {
	if (m_curTab != tab) {
		SetVisible(m_curTab, false);
		m_curTab = tab;
		SetVisible(tab, true);

		for (int i = 0; i < 5; i++) {
			if (i == m_curTab) {
				if (m_pTabBtn[i]) {
					m_pTabBtn[i]->SetStatus(FrButton::PRESSED);
				}
			} else {
				if (m_pTabBtn[i]) {
					m_pTabBtn[i]->SetStatus(FrButton::NORMAL);
				}
			}
		}
	}
}

// user_info.cpp:2182
void FrUserInfoForm::HidePet(bool bWait) {
	if (bWait) {
		m_anotherHand.WaitUntilWorkDone(9999);
	}

	if (m_pPetExhibition) {
		delete m_pPetExhibition;
		m_pPetExhibition = NULL;
	}
	if (m_pPetTids) {
		delete m_pPetTids;
		m_pPetTids = NULL;
	}
}

// user_info.cpp:2351
bool ScoreListCompare(const void *a, const void *b) {
	const sMapStatistics *pA = (const sMapStatistics *)a;
	const sMapStatistics *pB = (const sMapStatistics *)b;

	return pA->m_bestScore < pB->m_bestScore;
}

// user_info.cpp:2382
void FrUserInfoForm::SelectContents(eSeasonType season, sPangYaUserStatistics **ppStatistics,
		sTrophyStatistics **ppTrophy, std::list<sSpecialTrophy> **ppSpecialTrophies,
		std::list<sGuildTrophy> **ppGuildTrophies) {
	switch (season) {
	default:
		// sUserInfo does not type its statistics block (0x10F) yet.
		*ppStatistics = (sPangYaUserStatistics *)((char *)&m_userInfo + 0x10F);
		*ppTrophy = &m_userInfo.m_trophy;
		*ppSpecialTrophies = &m_specialTrophies;
		*ppGuildTrophies = &m_guildTrophies;
		break;

	case SEASON_TOTAL:
		*ppStatistics = &m_totalRecord.m_statistics;
		*ppTrophy = &m_totalRecord.m_trophy;
		*ppSpecialTrophies = &m_totalRecord.m_specialTrophies;
		*ppGuildTrophies = &m_totalRecord.m_guildTrophies;
		break;
	}
}

// user_info.cpp:2412
unsigned long FrUserInfoForm::GetSeasonTextColor(eSeasonType season) {
	return 0xFF000000;
}

// user_info.cpp:2448
void FrUserInfoForm::UpdateLevelTextImg(unsigned char level) {
	const Bitmap* pBitmap = g_pFresh->GetBitmap(MakeStr("level_%03d", (level + 1 < 71) ? level + 1 : 71));
	if (pBitmap) {
		m_pLevelArea->SetBgImg(pBitmap);
	}
}

// user_info.cpp:2465
bool IsNothingRecord(const sPangYaUserStatistics *pStatistics) {
	if (!pStatistics) return false;
	return (pStatistics->level == 0 || pStatistics->gamesPlayed == 0) ? 1 : 0;
}

// user_info.cpp:2475
bool IsMyRecord(const sUserInfo *pInfo) {
	if (!pInfo) return false;
	return (pInfo->m_uid == MyUID() || (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().identity >> 2) & 1) ? true : false;
}

// user_info.cpp:2485
bool IsHideDisconnectRate(const sPangYaUserStatistics *pStatistics) {
	return (pStatistics->gamesPlayed <= 50 || pStatistics->level < 6) ? 1 : 0;
}

// user_info.cpp:2652
void FrUserInfoForm::UpdateEditMapStatistics(eSeasonType season, bool bClassic) {
	sMapStatistics *pStats;
	switch (season) {
	case SEASON_CURRENT:
		pStats = bClassic ? m_classicMapStats : m_userInfo.m_mapStatistics;
		break;

	case SEASON_TOTAL:
		pStats = bClassic ? m_totalClassicMapStats : m_totalRecord.m_mapStats;
		break;

	default:
		pStats = bClassic ? m_classicMapStats : m_userInfo.m_mapStatistics;
		break;
	}

	if (pStats == NULL)
		return;

	if (m_pScoreList == NULL)
		return;

	m_pScoreList->ClearItem();

	for (int i = 0; i < 20; i++, pStats++) {
		if (pStats->m_course == 0x11)
			continue;

		IFF_STRUCT::sCourse *pCourse = ItemManager()->FindCourse(0x28000000
				| ((pStats->m_course > 0x7F && pStats->m_course != 0xFD) ? pStats->m_course - 0x80 : pStats->m_course));
		if (pCourse == NULL || pCourse->m_active == 0)
			continue;

		m_pScoreList->AddItem(pStats);
	}

	m_pScoreList->SortItem(ScoreListCompare);
}

// user_info.cpp:2701
void FrUserInfoForm::SetDisconntextAlign(void) {
	if (m_pDisconnQuestionMark == NULL)
		return;

	WRect rect;
	rect = m_pDisconnEdit->GetRect();

	if (m_pDisconnQuestionMark->IsVisible()) {
		if (rect.x == m_disconnEditRect.x) {
			rect.x -= m_pDisconnQuestionMark->GetRect().w;
			m_pDisconnEdit->SetRect(rect);
		}
	} else {
		if (rect.x != m_disconnEditRect.x) {
			rect.x += m_pDisconnQuestionMark->GetRect().w;
			m_pDisconnEdit->SetRect(rect);
		}
	}
}

// user_info.cpp:2726
void FrUserInfoForm::SetDisconnQuestionMark(bool bVisible) {
	if (m_pDisconnQuestionMark) {
		m_pDisconnQuestionMark->SetVisible(bVisible);
		SetDisconntextAlign();
	}
}

// user_info.cpp:2736
void SortMapStat(sMapStatistics *ppBest[], sMapStatistics *pStats) {
	for (int i = 0; i < 20; i++, pStats++) {
		if (i == 0x11)
			continue;

		IFF_STRUCT::sCourse *pCourse = ItemManager()->FindCourse(0x28000000
				| ((pStats->m_course > 0x7F && pStats->m_course != 0xFD) ? pStats->m_course - 0x80 : pStats->m_course));
		if (pCourse == NULL || pCourse->m_active == 0)
			continue;

		unsigned char kind = pCourse->courseFlags;
		sMapStatistics *&pBest = ppBest[(kind & 0x0F) - 1];

		if (pBest != NULL) {
			if (pStats->m_bestScore == 0x7F)
				continue;

			if (pStats->m_bestScore > pBest->m_bestScore)
				continue;

			if (pStats->m_bestScore == pBest->m_bestScore) {
				// sMapStatistics does not name its 64-bit value at 0x1E yet.
				__int64 value = *(__int64 *)((char *)pStats + 0x1E);
				if (value == 0 || value <= *(__int64 *)((char *)pBest + 0x1E))
					continue;
			}
		}
		pBest = pStats;
	}
}

// user_info.cpp:3235
unsigned long FrUserInfoForm::GetBestScoreColor(unsigned char course) {
	if (m_userInfo.m_uid == WSingleton<CSharedDoc>::Instance()->m_myUID)
		return 0xFF000000;

	// sUserInfo and CSharedDoc do not type their statistics blocks (0x10F,
	// 0x4EF) yet.
	const sPangYaUserStatistics *pStatistics = (const sPangYaUserStatistics *)((const char *)&m_userInfo + 0x10F);
	const sPangYaUserStatistics *pMyStatistics = (const sPangYaUserStatistics *)&WSingleton<CSharedDoc>::Instance()->m_myStatShots;
	bool bBetter;
	if (pStatistics->bestScore[course] == 0x7F)
		bBetter = false;
	else if (pMyStatistics->bestScore[course] == 0x7F)
		bBetter = true;
	else
		bBetter = pStatistics->bestScore[course] < pMyStatistics->bestScore[course];

	return bBetter ? 0xFFFFA045 : 0xFF000000;
}

// user_info.cpp:3261
unsigned long FrUserInfoForm::GetMaxPangColor(unsigned char course) {
	if (m_userInfo.m_uid == WSingleton<CSharedDoc>::Instance()->m_myUID)
		return 0xFFFFFFFF;

	// sUserInfo and CSharedDoc do not type their statistics blocks (0x10F,
	// 0x4EF) yet.
	const sPangYaUserStatistics *pStatistics = (const sPangYaUserStatistics *)((const char *)&m_userInfo + 0x10F);
	const sPangYaUserStatistics *pMyStatistics = (const sPangYaUserStatistics *)&WSingleton<CSharedDoc>::Instance()->m_myStatShots;
	bool bBetter = pStatistics->maxPang[course] > pMyStatistics->maxPang[course];

	return bBetter ? 0xFFFFA045 : 0xFF000000;
}

// user_info.cpp:3278
unsigned long FrUserInfoForm::Get30sRecordColor(int medal) {
	if (m_userInfo.m_uid == WSingleton<CSharedDoc>::Instance()->m_myUID)
		return 0xFF000000;

	const sTrophyStatistics *pTrophy = &m_userInfo.m_trophy;
	const sTrophyStatistics *pMyTrophy = &WSingleton<CSharedDoc>::Instance()->m_myTrophy;
	int myCount = 0, count = 0;

	for (int i = 0; i < 13; i++) {
		myCount += pMyTrophy->m_medals[i][medal];
		count += pTrophy->m_medals[i][medal];
	}

	return count > myCount ? 0xFFFFA045 : 0;
}

// user_info.cpp:3487
void FrUserInfoForm::OnPetInit(int param) {
	m_pPetArea = DYNAMIC_CAST(FrArea, param);
}

// user_info.cpp:3604
void FrUserInfoForm::OnPetLoadInit(int param) {
	m_pPetLoadGauge = DYNAMIC_CAST(FrGaugeBar, param);
	if (m_pPetLoadGauge) {
		m_pPetLoadGauge->SetRange(0, 0x78, 0);
		m_pPetLoadGauge->SetPos(0);
		m_pPetLoadGauge->SetDestPos(0x78, false);
		m_pPetLoadGauge->SetVisible(true);
	}
}

// user_info.cpp:3724
void FrUserInfoForm::SetVisible(FrWnd* pWnds[], int count, bool bVisible) {
	for (int i = 0; i < count; i++) {
		if (pWnds[i]) {
			pWnds[i]->SetVisible(bVisible);
		}
	}
}

// user_info.cpp:3895
void FrUserInfoForm::OnCommentInit(int param) {
	m_pCommentEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pCommentEdit) {
		m_pCommentEdit->SetVisible(false);
	}
}

// user_info.cpp:3902
void FrUserInfoForm::OnEditInit_HoverOn(int param) {
	if (!m_pCommentEdit)
		return;

	FrEdit* pEdit = DYNAMIC_CAST(FrEdit, param);
	if (!pEdit)
		return;

	const char* comment = "";
	int len = strlen(comment);
	if (len) {
		m_pCommentEdit->ClearLine();
		m_pCommentEdit->AddText(comment, false, false);
		m_pCommentEdit->MoveWindow(pEdit->GetRect().TopLeft());
		m_pCommentEdit->SetVisible(true);
	}
}

// user_info.cpp:3942
void FrUserInfoForm::OnEditInit_HoverOff(int param) {
	if (m_pCommentEdit) {
		m_pCommentEdit->SetVisible(false);
	}
}

// Texture of the score list's title font (data_a97bd8).
static char* s_scoreFontFile = "[font_wind.jpg";

// user_info.cpp:3948
void FrUserInfoForm::OnScoreListInit(int param) {
	m_pScoreList = DYNAMIC_CAST(FrListBox, param);
	if (m_pScoreList) {
		m_pScoreList->SetVisible(false);

		tagWTITLEFONT info;
		memset(&info, 0, sizeof(info));
		info.filename = &s_scoreFontFile;
		info.fontw = 16;
		info.fonth = 16;
		info.numPages = 1;
		info.texw = 128;
		info.texh = 64;
		info.pCharSet = "1234567890ym%-.?/:";
		m_pTitleFont = g_resrcmng->GetTitleFont();
		m_pTitleFont->Create(&info);
	}
}

// user_info.cpp:4045
void FrUserInfoForm::OnTitleSeasonVisible(void) {
	m_seasonTab = SEASON_CURRENT;
	m_pCurSeasonBtn->SetStatus(FrButton::PRESSED);
	m_pTotalSeasonBtn->SetStatus(FrButton::NORMAL);

	SetContent(true);
	SetTrophyStatic();

	SetSpecListBoxItem(SEASON_CURRENT);
}

// user_info.cpp:4058
void FrUserInfoForm::SetRequestFriendData(const char *nick, unsigned long uid) {
	if (nick == NULL) {
		return;
	}

	if (m_addFriendDlg) {
		m_addFriendDlg->SetNick(nick);
		m_addFriendDlg->SetFindUID(uid);
	}
}

// user_info.cpp:4075
CUserInfo::CUserInfo(void) : m_dlg(NULL), m_guid(0xFFFFFFFF), m_uid(0xFFFFFFFF), m_bRequest(false), m_bHasInfo(false) {
}

// user_info.cpp:4079
CUserInfo::~CUserInfo(void) {
}

// user_info.cpp:4083
void CUserInfo::Clear(void) {
	if (m_dlg) {
		m_dlg->Clear();
	}
}

// user_info.cpp:4089
bool CUserInfo::IsVisible(void) {
	return m_dlg ? true : false;
}

// user_info.cpp:4109
void CUserInfo::SetInfo(unsigned long uid, unsigned long guid, bool bRequest, bool bSendPacket, bool bMsnControl,
		bool bControlOff, std::string nick) {
	if (!OnlinePlay())
		return;

	if (uid == 0xFFFFFFFF)
		return;

	if (m_dlg == NULL) {
		m_dlg = CreateForm<FrUserInfoForm>(g_pFresh->GetManager(), this, "userinfo", 0);
		if (m_dlg == NULL)
			return;

		m_dlg->Open(static_cast<FRESH_RESULT_CALLBACK>(&CUserInfo::OnUserInfoFormResult), 1);
		m_dlg->EnableDrag(false);
		m_dlg->SetTopmost(bControlOff);
		m_dlg->SetMsnControl(bMsnControl);
		m_dlg->SetControlOff(bControlOff);
		m_dlg->FindNextTopFocus(false);

		m_nick = nick;
	}

	m_guid = guid;
	m_uid = uid;
	m_bHasInfo = true;
	m_bRequest = bRequest;

	std::map<unsigned long, sUserInfoTime>::iterator it;
	if (WSingleton<CSharedDoc>::Instance()->FindUserInfoTimeUID(uid, it)) {
		if (!bRequest || g_CurrentTime - it->second.m_time < 60000) {
			if (WSingleton<CSharedDoc>::Instance()->m_myUID == m_uid) {
				std::map<unsigned int, sCharacterInfo>::iterator itChar = WSingleton<CSharedDoc>::Instance()->m_characterMap.find(WSingleton<CSharedDoc>::Instance()->m_myCharacterId);
				if (itChar != WSingleton<CSharedDoc>::Instance()->m_characterMap.end())
					it->second.m_userInfo.m_characterInfo = itChar->second;

				if (!((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)))
					it->second.m_userInfo.m_pang = WSingleton<CSharedDoc>::Instance()->m_myPang;

				if (IsLocalContent((localContentType_t)0x51)) {
					it->second.m_userInfo.m_value155 = WSingleton<CSharedDoc>::Instance()->m_myExp;
					it->second.m_userInfo.m_level = WSingleton<CSharedDoc>::Instance()->m_myLevel;
				}

				memcpy(&it->second.m_userInfo.m_equipHead, &WSingleton<CSharedDoc>::Instance()->m_myCaddieId, 0x6C);
			}

			if (bMsnControl)
				it->second.m_userInfo.m_slot = 0xFFFF;
			m_dlg->SetInfo(&it->second);
			m_bHasInfo = false;
			return;
		}
	}

	if (bSendPacket) {
		if (nick.size()) {
			WSendPacket packet((enumClientPacket)0x2F);
			packet.Encode4(uid);
			packet.Encode1(5);
			packet.Send(SEND_TO_DEFAULT);
		}
	}
}

// user_info.cpp:4214
void CUserInfo::Refresh(void) {
	if (m_dlg) {
		m_dlg->SetContent(true);
	}
}

// user_info.cpp:4220
void CUserInfo::Process(float elapsed) {
	if (m_bHasInfo) {
		SetInfo(m_uid, m_guid, m_bRequest, false, false, false, "");

		if (m_bHasInfo && WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN)) {
			SetInfo(m_uid, m_guid, m_bRequest, false, true, false, "");
		}
	}

	if (m_dlg) {
		m_dlg->OnCustomProc(elapsed);
	}
}

// user_info.cpp:4237
bool CUserInfo::OnUserInfoFormResult(int ret, FrForm *pForm) {
	m_guid = 0xFFFFFFFF;
	m_uid = 0xFFFFFFFF;
	m_bHasInfo = false;

	m_dlg = 0;
	return true;
}

// user_info.cpp:4247
void CUserInfo::Close(void) {
	if (m_dlg) {
		m_dlg->Close(FrNONE, true);
	}
}

// user_info.cpp:4253
bool CUserInfo::IsRecvedData(unsigned char season) {
	if (m_dlg) {
		unsigned long flag = 0;
		if (season == 0) {
			flag = 2;
		} else if (season == 5) {
			flag = 4;
		}
		m_dlg->RecvedSeasonData(flag);
		if (m_dlg->IsTotalTab()) {
			if (!m_dlg->IsRecvSeason(2)) {
				m_dlg->RequestSeasonData(0);
				return false;
			}
			if (!m_dlg->IsRecvSeason(4)) {
				m_dlg->RequestSeasonData(5);
				return false;
			}
		}
		return true;
	}
	return false;
}

// user_info.cpp:4286
void FrUserInfoForm::OnRecAwardBackInit(int param) {
	m_pRecAwardBack = DYNAMIC_CAST(FrArea, param);
}

// user_info.cpp:4311
void FrUserInfoForm::OnGhostdUp(void) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), 0, "notify", 0);
	pForm->SetTimeLimit(5.0f);
	// "The service is being prepared."
	pForm->SetMessage("\xBC\xAD\xBA\xF1\xBD\xBA \xC1\xD8\xBA\xF1 \xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.", false);
	pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
}

// user_info.cpp:4325
void FrUserInfoForm::OnBtn_RmrVisitorInit(int param) {
	m_pRmrVisitorBtn = DYNAMIC_CAST(FrButton, param);
}

// user_info.cpp:4338
void FrUserInfoForm::OnBtn_RmrVisitorBtnUp(void) {
	m_anotherHand.WaitUntilWorkDone(9999);

	WSingleton<CSharedDoc>::Instance()->m_myRoomOwnerUID = m_userInfo.m_uid;

	unsigned char mode;
	if (MyUID() == WSingleton<CSharedDoc>::Instance()->m_myRoomOwnerUID) {
		mode = 1;
	} else {
		mode = 5;
	}

	WSingleton<CSharedDoc>::Instance()->SetRmrMode(mode); CTaskManager* pTaskManager = CTaskManager::Instance(); (!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 0x222, 0, 0, 0, 0, 0));
}

// user_info.cpp:4357
void FrUserInfoForm::RequestSeasonData(unsigned char season) {
	if (m_seasonPending == 0) {
		WSendPacket packet((enumClientPacket)0x2F);
		packet.Encode4(m_userInfo.m_uid);
		packet.Encode1(season);
		packet.Send(SEND_TO_DEFAULT);

		m_seasonPending = 1;
	}
}

// user_info.cpp:4371
void FrUserInfoForm::RecvedSeasonData(unsigned long season) {
	m_recvSeasonFlags |= season;
	m_seasonPending = 0;
}

// user_info.cpp:4377
int FrUserInfoForm::IsTotalTab(void) {
	return m_seasonTab == 1;
}
