// Original: d:\build\custom\temp\code\client\projectg\rankingdlg.cpp
#include "NetworkSystem.h"
#include <windows.h>
#include <atlbase.h>	// stand-in for the precompiled header: _AtlWinModule initializer
#include "RankingDlg.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "frstatic.h"
#include "frbutton.h"
#include "frlistbox.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "SharedDoc.h"
#include "SoundManager.h"
#include "frcombobox.h"
#include "fredit.h"
#include "frviewer.h"
#include "frarea.h"
#include "frtextbutton.h"
#include "frgaugebar.h"
#include "winput.h"
#include "InputManager.h"

// The IME input device (public ?g_ime@@3PAVWInputDev@@A).
extern WInputDev* g_ime;
#include "binstr.h"
#include "exhibition.h"
#include "EmoticonDlg.h"
#include "TaskManager.h"
#include "avatar_task.h"
#include "avatar_avatar.h"
#include "commonutil.h"
#include "frgraphicinterface.h"
#include "wview.h"
#include "woverlay.h"

// Names of the first entries, read through constants: the tables are
// initialized by code (dynamic initializers that store each member).
static const char* const RANK_TYPE_TOTAL_NAME = "\xc1\xbe\xc7\xd5 \xb7\xa9\xc5\xb7";
static const char* const CLASS_TYPE_ALL_NAME = "\xc0\xfc\xc3\xbc";
static const char* const GEN_SUB_TYPE_TOTAL_NAME = "\xc1\xbe\xc7\xd5 \xc1\xa1\xbc\xf6";
static const char* const COURSE_TYPE_BLUE_LAGOON_NAME = "\xba\xed\xb7\xe7\xb6\xf3\xb1\xba";
static const char* const RECORD_TYPE_ALBATROSS_NAME = "\xbe\xcb\xb9\xd9\xc6\xae\xb7\xce\xbd\xba";
static const char* const TERM_TYPE_SEASON5_NAME = "\xbd\xc3\xc1\xf0" "5";
static const char* const SEARCH_TYPE_NICKNAME_NAME = "\xb4\xd0\xb3\xd7\xc0\xd3";

// rankingdlg.cpp:49
const sTypeName FrRankingDlg::s_aRankType[] = {
	{ 0, RANK_TYPE_TOTAL_NAME },
	{ 1, "\xc4\xda\xbd\xba\xba\xb0 \xb7\xa9\xc5\xb7" },
	{ 2, "\xb1\xe2\xb7\xcf \xb7\xa9\xc5\xb7" }
};

// rankingdlg.cpp:56
const sTypeName FrRankingDlg::s_aClassType[] = {
	{ 0, CLASS_TYPE_ALL_NAME },
	{ 1, "\xc3\xca\xb1\xde" },
	{ 2, "\xc1\xdf\xb1\xde" },
	{ 3, "\xbb\xf3\xb1\xde" }
};

// rankingdlg.cpp:65
const sTypeName FrRankingDlg::s_aGenSubType[] = {
	{ 0, GEN_SUB_TYPE_TOTAL_NAME },
	{ 1, "\xbd\xba\xc4\xda\xbe\xee \xc1\xa1\xbc\xf6" },
	{ 2, "\xc6\xae\xb7\xce\xc7\xc7 \xc1\xa1\xbc\xf6" },
	{ 3, "\xc8\xb9\xb5\xe6\xc6\xce \xc1\xa1\xbc\xf6" },
	{ 4, "\xc7\xc3\xb7\xb9\xc0\xcc\xc8\xa6 \xc1\xa1\xbc\xf6" }
};

// rankingdlg.cpp:176
const sTypeName FrRankingDlg::s_aCourseType[] = {
	{ 0, COURSE_TYPE_BLUE_LAGOON_NAME },
	{ 1, "\xba\xed\xb7\xe7\xbf\xf6\xc5\xcd" },
	{ 2, "\xbc\xbc\xc7\xc7\xbe\xc6\xc0\xa9\xb5\xe5" },
	{ 3, "\xc0\xa9\xb5\xe5\xc8\xfa" },
	{ 4, "\xc0\xa7\xc1\xee\xc0\xa7\xc1\xee" },
	{ 5, "\xbf\xfe\xbd\xba\xc6\xae\xc0\xa7\xc1\xee" },
	{ 6, "\xba\xed\xb7\xe7\xb9\xae" },
	{ 7, "\xbd\xc7\xba\xf1\xbe\xc6\xc4\xb3\xb3\xed" },
	{ 8, "\xbe\xc6\xc0\xcc\xbd\xba\xc4\xb3\xb3\xed" },
	{ 9, "\xc8\xad\xc0\xcc\xc6\xae\xc0\xa7\xc1\xee" },
	{ 10, "\xbb\xfe\xc0\xcc\xb4\xd7\xbb\xf7\xb5\xe5" },
	{ 11, "\xc7\xce\xc5\xa9\xc0\xa9\xb5\xe5" },
	{ 12, "\xb5\xf6\xc0\xce\xc6\xe4\xb8\xa3\xb3\xeb" },
	{ 13, "\xbe\xc6\xc0\xcc\xbd\xba \xbd\xba\xc6\xc4" },
	{ 14, "\xb7\xce\xbd\xba\xc6\xae \xbd\xc3\xbf\xfe\xc0\xcc" },
	{ 15, "\xc0\xcc\xbd\xba\xc5\xcf \xba\xa7\xb8\xae" },

	{ 16, "\xc0\xa7\xc1\xee \xbd\xc3\xc6\xbc" }
};

// rankingdlg.cpp:199
const sTypeName FrRankingDlg::s_aRecordType[] = {
	{ 0, RECORD_TYPE_ALBATROSS_NAME },
	{ 1, "\xc8\xa6\xc0\xce\xbf\xf8" },
	{ 2, "\xb0\xad\xc1\xbe\xc0\xb2" },
	{ 3, "\xb7\xb9\xba\xa7" },
	{ 4, "\xc3\xd1\xc5\xb8\xb1\xb8\xba\xf1\xb0\xc5\xb8\xae" }
};

// rankingdlg.cpp:209
const sTypeName FrRankingDlg::s_aTermType[] = {
	{ 0, TERM_TYPE_SEASON5_NAME }
};

// rankingdlg.cpp:214
const sTypeName FrRankingDlg::s_aSearchType[] = {
	{ 0, SEARCH_TYPE_NICKNAME_NAME },





	{ 1, "\xbc\xf8\xc0\xa7" }
};

// rankingdlg.cpp:235
long GetRealCourseType(long course) {
	if (course == 12) {
		return 13;
	} else if (course == 13) {
		return 14;
	} else if (course == 14) {
		return 15;
	} else if (course == 15) {
		return 16;
	}

	if (course == 16) {
		return 19;
	}

	return course;
}

// rankingdlg.cpp:265 (run-time type and factory registration of FrRankingDlg)
const WRTTI FrRankingDlg::m_RTTI("FrRankingDlg", &FrForm::m_RTTI);

IObject* FrRankingDlgMakeInstance(void) {
	return new FrRankingDlg;
}

struct __impFrRankingDlg {
	__impFrRankingDlg(void) {
		ObjectFactory().AddObjectFunctor(FrRankingDlgMakeInstance, "FrRankingDlg");
		ActorFactory().AddActorClass("FrRankingDlg");
	}
};
static __impFrRankingDlg s_impFrRankingDlg;

// rankingdlg.cpp:267
const sFRESH_MSGMAP* FrRankingDlg::GetMessageMap(void) const {
	return &FrRankingDlg::_MsgMap;
}

// rankingdlg.cpp:409
bool FrRankingDlg::OnInit(void) {
	FrForm::OnInit();

	SendRequestRank(true);
	SendRequestUserInfo(MyUID(), CSharedDoc::Instance()->GetPangyaUserInfo().id);

	return true;
}

// rankingdlg.cpp:419
void FrRankingDlg::OnCancel(void) {
	Close(true);
}

// rankingdlg.cpp:424
void FrRankingDlg::OnProc(const float dt) {
	if (m_userInfoDelay > 0.0f) {
		m_userInfoDelay -= dt;

		if (m_userInfoDelay <= 0.0f) {
			SetVisibleUserInfo(true);
			SetAllControlsEnable(true);
		}
	}

	if (g_input->GetButton(LEFT_BUTTON) || g_input->GetButton(RIGHT_BUTTON)) {
		CloseAllComboBtns();
	}

	if (g_input->GetDown("ENTER", true) || g_input->GetDown("PADENTER", true)) {
		if (m_pUserIdEdit->IsViewFocused()) {
			OnRankList_SearchUserBtnUp();
			g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
		}
	}

	if (m_pLanguageArea) {
		// IME mode the indicator shows (data 0xABE7A0, no symbol)
		static bool s_bAlphaNumericMode;

		if (g_ime->IsAlphaNumericMode() != s_bAlphaNumericMode) {
			s_bAlphaNumericMode = g_ime->IsAlphaNumericMode();

			if (s_bAlphaNumericMode) {
				m_pLanguageArea->SetBgImg("chat_eng");
			} else {
				m_pLanguageArea->SetBgImg("chat_kor");
			}
		}
	}

	if ((g_input->Get("LCONTROL", false) || g_input->Get("RCONTROL", false)) && g_input->GetDown("EMOTICON", false)) {
		OnEmoticonBtnUp();
	}
}

// rankingdlg.cpp:541
void FrRankingDlg::ResetControl(void) {
	m_pTermS5Btn = 0;

	m_pRankList = 0;
	m_pMyInfoList = 0;
	m_pClassTypeCombo = 0;
	m_pCourseArea = 0;
	m_pNoMyRankWnd = 0;
	m_pPetCharacterArea = 0;
	m_pSearchUserBtn = 0;
	m_pEmoticonBtn = 0;
	m_pUserIdEdit = 0;
	m_pCurPageEdit = 0;
	m_pTotalPageEdit = 0;
	m_pUserRankEdit = 0;
	m_pRankTypeList = 0;
	m_pRankTypeViewer = 0;
	m_pGenSubTypeList = 0;
	m_pGenSubTypeViewer = 0;
	m_pCourseTypeList = 0;
	m_pCourseTypeViewer = 0;
	m_pRecordTypeList = 0;
	m_pRecordTypeViewer = 0;
	m_pSearchTypeList = 0;
	m_pSearchTypeViewer = 0;
	m_pPetLoadGauge = 0;
	m_pBottomMsg = 0;
	m_pEmoticonDlg = 0;
	m_pAboutDlg = 0;
	m_pGhostBtn = 0;

	memset(m_pRankTypeViewerImg, 0, sizeof(m_pRankTypeViewerImg));
	memset(m_pGenSubTypeViewerImg, 0, sizeof(m_pGenSubTypeViewerImg));
	memset(m_pCourseTypeViewerImg, 0, sizeof(m_pCourseTypeViewerImg));
	memset(m_pRecordTypeViewerImg, 0, sizeof(m_pRecordTypeViewerImg));
	memset(m_pSearchTypeViewerImg, 0, sizeof(m_pSearchTypeViewerImg));
}

// rankingdlg.cpp:580
void FrRankingDlg::SetVisibleByRankType(void) {
	if (m_pGenSubTypeViewer) {
		m_pGenSubTypeViewer->SetVisible(s_iRankType == 0);
	}

	if (m_pCourseTypeViewer) {
		m_pCourseTypeViewer->SetVisible(s_iRankType == 1);
	}

	if (m_pRecordTypeViewer) {
		m_pRecordTypeViewer->SetVisible(s_iRankType == 2);
	}
}

// rankingdlg.cpp:597
void FrRankingDlg::SetVisibleUserInfo(bool bVisible) {
	if (m_pPetCharacterArea) {
		m_pPetCharacterArea->SetVisible(bVisible);
	}
	if (m_pUpperMiniInfo) {
		m_pUpperMiniInfo->SetVisible(bVisible);
	}
	if (m_pPetLoadGauge) {
		// the gauge shows while the pet loads
		m_pPetLoadGauge->SetVisible(!bVisible);
		m_pPetLoadGauge->SetPos(0);
		m_pPetLoadGauge->SetDestPos(150, bVisible);
	}

	if (m_pUserRankEdit) {
		m_pUserRankEdit->SetVisible(bVisible);
	}
}

// rankingdlg.cpp:616
void FrRankingDlg::UpdateRankList(void) {
	if (m_pRankList) {
		m_pRankList->ClearItem();

		int count = 0;
		std::list<ranktype::sListInfo>::iterator it;
		for (it = m_rankList.begin(); it != m_rankList.end(); ++it) {
			if (count >= 12) {
				break;
			}

			m_pRankList->AddItem(&*it);
			count++;
		}
	}

	SynchronizeButtons();

	char text[32];
	itoa(s_iPageNum + 1, text, 10);
	m_pCurPageEdit->SetLine(1, text, 0, false, 0);
	m_pCurPageEdit->SetVisible(true);
}

// rankingdlg.cpp:652
void FrRankingDlg::ClearRankList(void) {
	if (m_pRankList) {
		m_pRankList->ClearItem();
	}

	char text[32];
	s_iPageNum = 0;
	itoa(s_iPageNum, text, 10);
	m_pCurPageEdit->SetLine(1, text, 0, false, 0);

	s_iPageTotal = 0;
	itoa(s_iPageTotal, text, 10);
	m_pTotalPageEdit->SetLine(1, text, 0, false, 0);

	SynchronizeButtons();
}

// rankingdlg.cpp:676
void FrRankingDlg::UpdateMyInfo(void) {
	if (m_pMyInfoList) {
		m_pMyInfoList->ClearItem();

		if (m_myRank != -1) {
			m_pMyInfoList->AddItem(&m_myRank);
			m_pMyInfoList->SetVisible(true);
			m_pNoMyRankWnd->SetVisible(false);
		} else {
			m_pMyInfoList->SetVisible(false);
			m_pNoMyRankWnd->SetVisible(true);
		}
	}
}

// rankingdlg.cpp:696
void FrRankingDlg::UpdateUserInfo(void) {
	m_anotherHand.Work(this, 0, 0, true, -1);

	if (m_pPetCharacterArea) {
		m_pPetCharacterArea->SetVisible(true);
	}

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	pGDI->SetTextStyle(1);

	char text[32];
	if (m_userRank <= 0) {
		sprintf(text, "- \xc0\xa7");
	} else {
		sprintf(text, "%d \xc0\xa7", m_userRank);
	}
	m_pUserRankEdit->SetLine(1, text, 0xffffffff, false, 0);

	pGDI->SetTextStyle(0);
}

// rankingdlg.cpp:473
void FrRankingDlg::OnAnotherHand(int nWork, void* pData) {
	if (nWork == 0) {
		SetPetCharacter();
	}
}

// rankingdlg.cpp:845
void FrRankingDlg::ShowUserInfo(void) {
	if (m_myUID == 0xffffffff) {
		return;
	}

	WPoint pt = m_pUpperMiniInfo->GetRect().TopLeft();

	WPoint pos(pt.x, pt.y + 40.0f);
	DrawUserInfoLine(1, 0, pos);
	DrawUserInfoLine(2, 1, pos);
	DrawUserInfoLine(3, 2, pos);
	DrawUserInfoLine(4, 3, pos);
	pos.y += 7.0f;
	DrawUserInfoLine(0, 4, pos);
}

// rankingdlg.cpp:931
void FrRankingDlg::DisconnectRankServer(void) {
	if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_RANK)) {
		WNetworkSystem::Instance()->ForceShutDown(WNetworkSystem::NET_UNIT_RANK);
	}
}

// rankingdlg.cpp:939
void FrRankingDlg::SetRankTypes(unsigned char rankType, unsigned char subType, unsigned char termType,
	unsigned char classType, int pageNum, int pageTotal) {
	s_iRankType = rankType;
	s_iSubType = subType;
	s_iTermType = termType;
	s_iClassType = classType;
	s_iPageNum = pageNum;
	s_iPageTotal = pageTotal;
}

// rankingdlg.cpp:949
void FrRankingDlg::SelectRankList(int index) {
	if (m_pRankList) {
		FrListItem* pItem = m_pRankList->GetItem(index);
		if (!pItem) {
			return;
		}

		m_pRankList->SelectItem(pItem, true);
	}

	FrListItem* pSelected = m_pRankList->GetSelected();
	ranktype::sListInfo* pInfo = (ranktype::sListInfo*)pSelected->pData;
	if (pInfo) {
		SendRequestUserInfo(pInfo->uid, pInfo->nickname.c_str());
	}
}

// rankingdlg.cpp:967
void FrRankingDlg::SynchronizeButtons(void) {
	if (m_pRankTypeList) {
		const char* name = ((FrLine*)m_pRankTypeList->GetSelected()->pData)->text.c_str();
		if (strcmpi(s_aRankType[s_iRankType].m_name, name) == 0) {
			m_pRankTypeList->SelectItem(m_pRankTypeList->GetItem(s_iRankType), true);
			m_pRankTypeList->SetVisible(false);
			SetVisibleByRankType();
		}
	}

	switch (s_iRankType) {
	case 0:
		if (m_pGenSubTypeList) {
			m_pGenSubTypeList->SelectItem(m_pGenSubTypeList->GetItem(s_iSubType), true);
			m_pGenSubTypeList->SetVisible(false);
		}
		break;

	case 1:
		if (m_pCourseTypeList) {
			m_pCourseTypeList->SelectItem(m_pCourseTypeList->GetItem(s_iSubType), true);
			m_pCourseTypeList->SetVisible(false);
		}
		break;

	case 2:
		if (m_pRecordTypeList) {
			m_pRecordTypeList->SelectItem(m_pRecordTypeList->GetItem(s_iSubType), true);
			m_pRecordTypeList->SetVisible(false);
		}
		break;
	}
}

// rankingdlg.cpp:1110
void FrRankingDlg::SetAllControlsEnable(bool bEnable) {
	m_pTermS5Btn->Enable(bEnable);

	m_pRankList->Enable(bEnable);
	m_pMyInfoList->Enable(bEnable);
	m_pSearchUserBtn->Enable(bEnable);
	m_pRankTypeList->Enable(bEnable);
	m_pRankTypeViewer->Enable(bEnable);
	m_pGenSubTypeList->Enable(bEnable);
	m_pGenSubTypeViewer->Enable(bEnable);
	m_pCourseTypeList->Enable(bEnable);
	m_pCourseTypeViewer->Enable(bEnable);
	m_pRecordTypeList->Enable(bEnable);
	m_pRecordTypeViewer->Enable(bEnable);
	m_pSearchTypeList->Enable(bEnable);
	m_pSearchTypeViewer->Enable(bEnable);
}

// rankingdlg.cpp:1130
void FrRankingDlg::HidePet(bool bHide) {
	if (m_pExhibition) {
		delete m_pExhibition;
		m_pExhibition = 0;
	}
	if (m_pPetData) {
		delete m_pPetData;
		m_pPetData = 0;
	}
}

// rankingdlg.cpp:1144
void FrRankingDlg::SendRequestRank(bool bFirst) {
	if (!WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_RANK)) {
		return;
	}

	WSendPacket packet(0);

	packet.Encode4(MyUID());
	packet.EncodeStr(std::string(CSharedDoc::Instance()->GetPangyaUserInfo().id));
	packet.Encode1(s_iRankType);
	packet.Encode1(s_iSubType);
	packet.Encode1(s_iTermType);
	packet.Encode1(s_iClassType);
	packet.Encode4(s_iPageNum);
	packet.Encode1(bFirst);
	packet.Send(SEND_TO_RANK);

	m_userInfoDelay = 1.0f;
	SetAllControlsEnable(false);

	CRankingInfo::Instance()->GetDlg()->SetBottomMsg("");
}

// rankingdlg.cpp:1170
void FrRankingDlg::SendRequestUserInfo(unsigned long uid, const char* nickname) {
	if (!WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_RANK)) {
		return;
	}

	if (uid == 0xffffffff) {
		return;
	}

	WSendPacket packet(1);
	packet.Encode4(uid);
	packet.EncodeStr(nickname);
	packet.Encode1(s_iTermType);
	packet.Send(SEND_TO_RANK);

	m_userInfoDelay = 1.0f;
	SetVisibleUserInfo(false);
	SetAllControlsEnable(false);

	CRankingInfo::Instance()->GetDlg()->SetBottomMsg("");
}

// rankingdlg.cpp:1257
void FrRankingDlg::OnTermType_S5_BtnInit(int param) {
	m_pTermS5Btn = DYNAMIC_CAST(FrButton, param);
	m_pTermS5Btn->SetPushDelay(1.0f);

	m_pTermS5Btn->SetStatus(FrButton::PRESSED);
}

// rankingdlg.cpp:1265
void FrRankingDlg::OnTermType_S5_BtnUp(void) {
	s_iTermType = 0;

	m_pTermS5Btn->SetStatus(FrButton::PRESSED);

	s_iPageNum = 0;
	SendRequestRank(true);
	SendRequestUserInfo(m_myUID, m_myNickname);
}

// rankingdlg.cpp:1283
void FrRankingDlg::OnCourseAreaInit(int param) {
	m_pCourseArea = DYNAMIC_CAST(FrArea, param);
}

// rankingdlg.cpp:1288
void FrRankingDlg::OnCourseAreaOwnerDraw(int param) {
}

// rankingdlg.cpp:1315
void FrRankingDlg::OnRankList_Init(int param) {
	if (m_pRankList) {
		m_pRankList->ClearItem();
	} else {
		m_pRankList = DYNAMIC_CAST(FrListBox, param);
	}
}

// rankingdlg.cpp:1321
void FrRankingDlg::OnRankList_OwnerDraw(int param) {
	if (m_rankList.empty()) {
		return;
	}

	if (!param) {
		return;
	}

	SetListItemInfo((FrListItem*)param);
}

// rankingdlg.cpp:1333
void FrRankingDlg::OnRankList_LBtnDown(void) {
	if (!(m_userInfoDelay > 0.0f) && !m_anotherHand.IsWorking(0)) {
		FrListItem* pItem = m_pRankList->GetItemUnderCursor();
		if (!pItem) {
			return;
		}

		ranktype::sListInfo* pInfo = (ranktype::sListInfo*)pItem->pData;
		if (!pInfo) {
			return;
		}

		if (strcmpi(pInfo->nickname.c_str(), m_myNickname) == 0) {
			return;
		}

		SendRequestUserInfo(pInfo->uid, pInfo->nickname.c_str());
	}
}

// rankingdlg.cpp:1353
void FrRankingDlg::OnRankList_PrevPageInit(int param) {
	FrButton* pBtn = DYNAMIC_CAST(FrButton, param);
	pBtn->SetPushDelay(1.0f);
}

// rankingdlg.cpp:1359
void FrRankingDlg::OnRankList_PrevPageBtnUp(void) {
	if (s_iPageNum > 0) {
		s_iPageNum--;
		SendRequestRank(false);
	} else {
		// "There are no more ranking records."
		m_pBottomMsg->SetCaption("\xB4\xF5 \xC0\xCC\xBB\xF3 \xB7\xA9\xC5\xB7 \xB1\xE2\xB7\xCF\xC0\xCC \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."); m_pBottomMsg->SetVisible(true);
	}
}

// rankingdlg.cpp:1372
void FrRankingDlg::OnRankList_NextPageInit(int param) {
	FrButton* pBtn = DYNAMIC_CAST(FrButton, param);
	pBtn->SetPushDelay(1.0f);
}

// rankingdlg.cpp:1378
void FrRankingDlg::OnRankList_NextPageBtnUp(void) {
	s_iPageNum++;
	SendRequestRank(false);
}

// rankingdlg.cpp:1384
void FrRankingDlg::OnRankList_CurrentPageInit(int param) {
	m_pCurPageEdit = DYNAMIC_CAST(FrEdit, param);
}

// rankingdlg.cpp:1389
void FrRankingDlg::OnRankList_TotalPageInit(int param) {
	m_pTotalPageEdit = DYNAMIC_CAST(FrEdit, param);
}

// rankingdlg.cpp:1394
void FrRankingDlg::OnRankList_MyInfo_Init(int param) {
	if (m_pMyInfoList) {
		m_pMyInfoList->ClearItem();
	} else {
		m_pMyInfoList = DYNAMIC_CAST(FrListBox, param);
	}

	m_pMyInfoList->UseRightButton(true);
}

// rankingdlg.cpp:1402
void FrRankingDlg::OnRankList_MyInfo_LBtnDown(void) {
	if (!(m_userInfoDelay > 0.0f) && !m_anotherHand.IsWorking(0)) {
		FrListItem* pItem = m_pMyInfoList->GetItemUnderCursor();
		if (!pItem) {
			return;
		}

		ranktype::sListInfo* pInfo = (ranktype::sListInfo*)pItem->pData;
		if (!pInfo) {
			return;
		}

		if (strcmpi(pInfo->nickname.c_str(), m_myNickname) == 0) {
			return;
		}

		SendRequestUserInfo(pInfo->uid, pInfo->nickname.c_str());
	}
}

// rankingdlg.cpp:1422
void FrRankingDlg::OnRankList_MyInfo_OwnerDraw(int param) {
	if (param) {
		SetListItemInfo((FrListItem*)param);
	}
}

// rankingdlg.cpp:1433
void FrRankingDlg::OnRankList_NoInRank_Init(int param) {
	m_pNoMyRankWnd = DYNAMIC_CAST(FrStatic, param);
	m_pNoMyRankWnd->SetVisible(false);
}

// rankingdlg.cpp:1439
void FrRankingDlg::OnRankList_SearchUserInit(int param) {
	m_pSearchUserBtn = DYNAMIC_CAST(FrButton, param);
	m_pSearchUserBtn->SetPushDelay(0.1f);
}

// rankingdlg.cpp:1445
void FrRankingDlg::OnRankList_SearchUserBtnUp(void) {
	char nickname[260];
	nickname[0] = 0;

	if (m_pUserIdEdit) {
		strcpy(nickname, m_pUserIdEdit->GetLine(1, false));
		TrimA(nickname, " \t\n\r");
	}

	SendRequestSearchUser(nickname);
}

// rankingdlg.cpp:1456
void FrRankingDlg::OnRankList_UserIdEditInit(int param) {
	m_pUserIdEdit = DYNAMIC_CAST(FrEdit, param);
	m_pUserIdEdit->SetCharLimit(0x20, true);
}

// rankingdlg.cpp:1462
void FrRankingDlg::OnRankList_UserIdEditDown(void) {
}

// rankingdlg.cpp:1467
void FrRankingDlg::OnEmoticonInit(intptr_t param) {
	m_pEmoticonBtn = DYNAMIC_CAST(FrButton, param);
}

// rankingdlg.cpp:1472
void FrRankingDlg::OnEmoticonBtnUp(void) {
	if (m_pEmoticonBtn && !m_pEmoticonDlg) {
		m_pEmoticonDlg = CreateForm<FrEmoticonDlg>(g_pFresh->GetManager(), this, "emoticon", 0);
		if (!m_pEmoticonDlg) {
			return;
		}

		FrButton* pBtn = m_pEmoticonBtn;
		WPoint pos(0.0f, 0.0f);
		if (pBtn) {
			pos.x = pBtn->GetRect().x - 200.0f;
			pos.y = pBtn->GetRect().y - 260.0f;
		}

		m_pEmoticonDlg->Open((FRESH_RESULT_CALLBACK)&FrRankingDlg::OnEmoticonResult, pos, 3);
	}
}

// rankingdlg.cpp:1493
void FrRankingDlg::OnLanguageInit(intptr_t param) {
	m_pLanguageArea = DYNAMIC_CAST(FrArea, param);
	if (m_pLanguageArea) {
		if (g_ime->IsAlphaNumericMode())
			m_pLanguageArea->SetBgImg("chat_eng");
		else
			m_pLanguageArea->SetBgImg("chat_kor");
	}
}

// rankingdlg.cpp:1506
void FrRankingDlg::OnAboutBtnInit(int param) {
	if ((__rtti_obj = (void*)param) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&FrButton::m_RTTI)) {
	}
}

// rankingdlg.cpp:1512
void FrRankingDlg::OnAboutBtnUp(void) {
	m_pAboutDlg = CreateForm<FrRankingAboutDlg>(g_pFresh->GetManager(), this, "ranking_about", 0);

	if (m_pAboutDlg) {
		m_pAboutDlg->Open((FRESH_RESULT_CALLBACK)&FrRankingDlg::OnAboutDlgResult, 3);
	}
}

// rankingdlg.cpp:1522
void FrRankingDlg::OnCloseBtnUp(void) {
	Close(true);
}

// rankingdlg.cpp:1528
void FrRankingDlg::OnBottomMessageInit(int param) {
	m_pBottomMsg = DYNAMIC_CAST(FrStatic, param);
	m_pBottomMsg->SetVisible(false);
}

// rankingdlg.cpp:1534
void FrRankingDlg::OnPetCharacterInit(int param) {
	m_pPetCharacterArea = DYNAMIC_CAST(FrArea, param);
}

// rankingdlg.cpp:1539
void FrRankingDlg::OnPetCharacterOwnerDraw(void) {
	if (m_pPetLoadGauge && m_pPetLoadGauge->IsVisible()) {
		return;
	}

	if (!m_anotherHand.IsWorking(0)) {
		m_anotherHand.Enter();
		if (m_pExhibition) {
			m_pExhibition->Display(0.0f, 0.0f);
		}
		m_anotherHand.Leave();
	}
}

// rankingdlg.cpp:1553
void FrRankingDlg::OnPetLoadInit(int param) {
	m_pPetLoadGauge = DYNAMIC_CAST(FrGaugeBar, param);
	if (m_pPetLoadGauge) {
		m_pPetLoadGauge->SetRange(0, 150, 0);
		m_pPetLoadGauge->SetVisible(false);
	}
}

// rankingdlg.cpp:1563
void FrRankingDlg::OnUpperMiniInfoInit(int param) {
	m_pUpperMiniInfo = DYNAMIC_CAST(FrArea, param);
}

// rankingdlg.cpp:1595
void FrRankingDlg::OnClassType_ComboInit(int param) {
	m_pClassTypeCombo = DYNAMIC_CAST(FrComboBox, param);

	for (int i = 0; i < 4; i++) {
		m_pClassTypeCombo->AddString(s_aClassType[i].m_name);
	}

	m_pClassTypeCombo->SetLine(1, s_aClassType[s_iClassType].m_name, 0, false, 0);
	m_pClassTypeCombo->SetVisible(false);
}

// rankingdlg.cpp:1608
void FrRankingDlg::OnClassType_ComboDown(int param) {
	const char* text = m_pClassTypeCombo->GetLine(1, false);

	int prevClassType = s_iClassType;
	s_iClassType = m_pClassTypeCombo->FindString(text) - 1;
	if (s_iClassType < 0) {
		s_iClassType = 0;
	} else if (s_iClassType > 3) {
		s_iClassType = 3;
	}

	if (s_iClassType != prevClassType) {
		SendRequestRank(true);
	}
}

// rankingdlg.cpp:1622
void FrRankingDlg::OnRankTypeListInit(int param) {
	m_pRankTypeList = DYNAMIC_CAST(FrListBox, param);

	if (m_pRankTypeList) {
		for (int i = 0; i < 3; i++) {
			m_rankTypeLine[i].text = std::string(s_aRankType[i].m_name);
			m_pRankTypeList->AddItem(&m_rankTypeLine[i]);
		}

		m_pRankTypeList->SelectItem(m_pRankTypeList->FindItem(&m_rankTypeLine[0]), true);
		m_pRankTypeList->SetVisible(false);
		m_pRankTypeList->EnableScrollBar(false);
	}
}

// rankingdlg.cpp:1642
void FrRankingDlg::OnRankTypeListLDown(void) {
	FrListItem* pItem = m_pRankTypeList->GetItemUnderCursor();

	if (pItem) {
		m_pRankTypeList->SelectItem(pItem, true);
		m_pRankTypeList->SetVisible(false);

		s_iRankType = pItem->idx;
		if (s_iRankType < 0) {
			s_iRankType = 0;
		} else if (s_iRankType > 2) {
			s_iRankType = 2;
		}

		s_iSubType = 0;
		s_iPageNum = 0;
		SendRequestRank(true);
		SetVisibleByRankType();
	}
}

// rankingdlg.cpp:1662
void FrRankingDlg::OnRankTypeListOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		FrListItem* pItem = (FrListItem*)param;
		if (pItem) {
			FrLine* pLine = (FrLine*)pItem->pData;
			WRect rect(pItem->pos.x, pItem->pos.y, 82.0f, 22.0f);

			WOverlay::DrawBox(g_view, rect, 0, 0xAA0E4F75, 0.0f);

			if (pItem->underCursor) {
				rect.x = pItem->pos.x + 1.0f;
				rect.y = pItem->pos.y + 1.0f;
				rect.w = 80.0f;
				rect.h = 20.0f;
				pGDI->Box(rect, 0x80082B46, 0, 0.0f);
				pGDI->SetTextStyle(1);
			} else {
				pGDI->SetTextStyle(0);
			}

			pGDI->SetTextColor(0xffffffff, 0xffffffff);
			pGDI->Print(WPoint(pItem->pos.x + 10.0f, pItem->pos.y + 6.0f), 0, pLine->text.c_str());
		}
	}
}

// rankingdlg.cpp:1691
void FrRankingDlg::OnRankTypeViewerInit(int param) {
	m_pRankTypeViewer = DYNAMIC_CAST(FrViewer, param);

	m_pRankTypeViewerImg[0] = g_pFresh->RegisterBitmap("btn_drop01_n");
	m_pRankTypeViewerImg[1] = g_pFresh->RegisterBitmap("btn_drop01_o");
	m_pRankTypeViewerImg[2] = g_pFresh->RegisterBitmap("btn_drop01_d");
}

// rankingdlg.cpp:1700
void FrRankingDlg::OnRankTypeViewerDown(void) {
	if (m_pRankTypeList) {
		m_pRankTypeList->SetVisible(!m_pRankTypeList->IsVisible());
		g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}
}

// rankingdlg.cpp:1709
void FrRankingDlg::OnRankTypeViewerOwnerDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		FrViewer* pViewer = m_pRankTypeViewer;
		if (pViewer) {
			WRect rect;
			int state = 0;
			if (m_pRankTypeList->IsVisible()) {
				state = 2;
			} else {
				rect = pViewer->GetRect();

				if (rect.IsInRect(g_pFresh->GetManager()->GetMousePos())) {
					state = 1;
				}
			}

			pGDI->DrawTexture(m_pRankTypeViewerImg[state], pViewer->GetRect(), 0xffffffff, 0);

			const WRect& viewerRect = m_pRankTypeViewer->GetRect();
			WPoint pt(viewerRect.x, viewerRect.y);
			FrLine* pLine = (FrLine*)m_pRankTypeList->GetSelected()->pData;
			WRect viewRect(pt, viewerRect.w, viewerRect.h);

			WOverlay::DrawBox(g_view, viewRect, 0, 0x800E4F75, 0.0f);
			pGDI->SetTextStyle(1);

			pGDI->SetTextColor(0xffffffff, 0xffffffff);
			pGDI->Print(WPoint(pt.x + 4.0f, pt.y + 5.0f), 0, pLine->text.c_str());
		}
	}
}

// rankingdlg.cpp:1750
void FrRankingDlg::OnGeneralSubTypeListInit(int param) {
	m_pGenSubTypeList = DYNAMIC_CAST(FrListBox, param);

	if (m_pGenSubTypeList) {
		for (int i = 0; i < 5; i++) {
			m_genSubTypeLine[i].text = std::string(s_aGenSubType[i].m_name);
			m_pGenSubTypeList->AddItem(&m_genSubTypeLine[i]);
		}

		m_pGenSubTypeList->SelectItem(m_pGenSubTypeList->FindItem(&m_genSubTypeLine[0]), true);
		m_pGenSubTypeList->SetVisible(false);
		m_pGenSubTypeList->EnableScrollBar(false);
	}
}

// rankingdlg.cpp:1770
void FrRankingDlg::OnGeneralSubTypeListLDown(void) {
	FrListItem* pItem = m_pGenSubTypeList->GetItemUnderCursor();

	if (pItem) {
		m_pGenSubTypeList->SelectItem(pItem, true);
		m_pGenSubTypeList->SetVisible(false);

		s_iSubType = pItem->idx;
		if (s_iSubType < 0) {
			s_iSubType = 0;
		} else if (s_iSubType > 4) {
			s_iSubType = 4;
		}

		s_iPageNum = 0;
		SendRequestRank(true);
	}
}

// rankingdlg.cpp:1786
void FrRankingDlg::OnGeneralSubTypeListOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		FrListItem* pItem = (FrListItem*)param;
		if (pItem) {
			FrLine* pLine = (FrLine*)pItem->pData;
			WRect rect(pItem->pos.x, pItem->pos.y, 82.0f, 22.0f);

			WOverlay::DrawBox(g_view, rect, 0, 0xAA0E4F75, 0.0f);

			if (pItem->underCursor) {
				rect.x = pItem->pos.x + 1.0f;
				rect.y = pItem->pos.y + 1.0f;
				rect.w = 80.0f;
				rect.h = 20.0f;
				pGDI->Box(rect, 0x80082B46, 0, 0.0f);
				pGDI->SetTextStyle(1);
			} else {
				pGDI->SetTextStyle(0);
			}

			pGDI->SetTextColor(0xffffffff, 0xffffffff);
			pGDI->Print(WPoint(pItem->pos.x + 10.0f, pItem->pos.y + 6.0f), 0, pLine->text.c_str());
		}
	}
}

// rankingdlg.cpp:1815
void FrRankingDlg::OnGeneralSubTypeViewerInit(int param) {
	m_pGenSubTypeViewer = DYNAMIC_CAST(FrViewer, param);

	m_pGenSubTypeViewerImg[0] = g_pFresh->RegisterBitmap("btn_drop02_n");
	m_pGenSubTypeViewerImg[1] = g_pFresh->RegisterBitmap("btn_drop02_o");
	m_pGenSubTypeViewerImg[2] = g_pFresh->RegisterBitmap("btn_drop02_d");
}

// rankingdlg.cpp:1824
void FrRankingDlg::OnGeneralSubTypeViewerDown(void) {
	if (m_pGenSubTypeList) {
		m_pGenSubTypeList->SetVisible(!m_pGenSubTypeList->IsVisible());
		g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}
}

// rankingdlg.cpp:1833
void FrRankingDlg::OnGeneralSubTypeViewerOwnerDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		FrViewer* pViewer = m_pGenSubTypeViewer;
		if (pViewer) {
			WRect rect;
			int state = 0;
			if (m_pGenSubTypeList->IsVisible()) {
				state = 2;
			} else {
				rect = pViewer->GetRect();

				if (rect.IsInRect(g_pFresh->GetManager()->GetMousePos())) {
					state = 1;
				}
			}

			pGDI->DrawTexture(m_pGenSubTypeViewerImg[state], pViewer->GetRect(), 0xffffffff, 0);

			const WRect& viewerRect = m_pGenSubTypeViewer->GetRect();
			WPoint pt(viewerRect.x, viewerRect.y);
			FrLine* pLine = (FrLine*)m_pGenSubTypeList->GetSelected()->pData;
			WRect viewRect(pt, viewerRect.w, viewerRect.h);

			WOverlay::DrawBox(g_view, viewRect, 0, 0x800E4F75, 0.0f);
			pGDI->SetTextStyle(1);

			pGDI->SetTextColor(0xffffffff, 0xffffffff);
			pGDI->Print(WPoint(pt.x + 4.0f, pt.y + 5.0f), 0, pLine->text.c_str());
		}
	}
}

// rankingdlg.cpp:1874
void FrRankingDlg::OnCourseTypeListInit(int param) {
	m_pCourseTypeList = DYNAMIC_CAST(FrListBox, param);

	if (m_pCourseTypeList) {
		for (int i = 0; i < 17; i++) {
			IFF_STRUCT::sCourse* pCourse = ItemManager()->FindCourse(0x28000000 | GetRealCourseType(i));
			if (pCourse) {
				m_courseTypeLine[i].text = std::string(s_aCourseType[i].m_name);
				m_pCourseTypeList->AddItem(&m_courseTypeLine[i]);
			}
		}

		const WRect& rect = m_pCourseTypeList->GetRect();

		m_pCourseTypeList->SetRect(WRect(rect.x, rect.y, 107.0f, 374.0f));

		m_pCourseTypeList->SelectItem(m_pCourseTypeList->FindItem(&m_courseTypeLine[0]), true);
		m_pCourseTypeList->SetVisible(false);
		m_pCourseTypeList->EnableScrollBar(false);
	}
}

// rankingdlg.cpp:1905
void FrRankingDlg::OnCourseTypeListLDown(void) {
	FrListItem* pItem = m_pCourseTypeList->GetItemUnderCursor();

	if (pItem) {
		m_pCourseTypeList->SelectItem(pItem, true);
		m_pCourseTypeList->SetVisible(false);

		s_iSubType = pItem->idx;
		if (s_iSubType < 0) {
			s_iSubType = 0;
		} else if (s_iSubType > 16) {
			s_iSubType = 16;
		}

		s_iPageNum = 0;
		SendRequestRank(true);
	}
}

// rankingdlg.cpp:1921
void FrRankingDlg::OnCourseTypeListOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		FrListItem* pItem = (FrListItem*)param;
		if (pItem) {
			FrLine* pLine = (FrLine*)pItem->pData;
			WRect rect(pItem->pos.x, pItem->pos.y, 82.0f, 22.0f);

			WOverlay::DrawBox(g_view, rect, 0, 0xAA0E4F75, 0.0f);

			if (pItem->underCursor) {
				rect.x = pItem->pos.x + 1.0f;
				rect.y = pItem->pos.y + 1.0f;
				rect.w = 80.0f;
				rect.h = 20.0f;
				pGDI->Box(rect, 0x80082B46, 0, 0.0f);
				pGDI->SetTextStyle(1);
			} else {
				pGDI->SetTextStyle(0);
			}

			pGDI->SetTextColor(0xffffffff, 0xffffffff);
			pGDI->Print(WPoint(pItem->pos.x + 10.0f, pItem->pos.y + 6.0f), 0, pLine->text.c_str());
		}
	}
}

// rankingdlg.cpp:1950
void FrRankingDlg::OnCourseTypeViewerInit(int param) {
	m_pCourseTypeViewer = DYNAMIC_CAST(FrViewer, param);

	m_pCourseTypeViewerImg[0] = g_pFresh->RegisterBitmap("btn_drop02_n");
	m_pCourseTypeViewerImg[1] = g_pFresh->RegisterBitmap("btn_drop02_o");
	m_pCourseTypeViewerImg[2] = g_pFresh->RegisterBitmap("btn_drop02_d");
}

// rankingdlg.cpp:1959
void FrRankingDlg::OnCourseTypeViewerDown(void) {
	if (m_pCourseTypeList) {
		m_pCourseTypeList->SetVisible(!m_pCourseTypeList->IsVisible());
		g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}
}

// rankingdlg.cpp:1968
void FrRankingDlg::OnCourseTypeViewerOwnerDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		FrViewer* pViewer = m_pCourseTypeViewer;
		if (pViewer) {
			WRect rect;
			int state = 0;
			if (m_pCourseTypeList->IsVisible()) {
				state = 2;
			} else {
				rect = pViewer->GetRect();

				if (rect.IsInRect(g_pFresh->GetManager()->GetMousePos())) {
					state = 1;
				}
			}

			pGDI->DrawTexture(m_pCourseTypeViewerImg[state], pViewer->GetRect(), 0xffffffff, 0);

			const WRect& viewerRect = m_pCourseTypeViewer->GetRect();
			WPoint pt(viewerRect.x, viewerRect.y);
			FrLine* pLine = (FrLine*)m_pCourseTypeList->GetSelected()->pData;
			WRect viewRect(pt, viewerRect.w, viewerRect.h);

			WOverlay::DrawBox(g_view, viewRect, 0, 0x800E4F75, 0.0f);
			pGDI->SetTextStyle(1);

			pGDI->SetTextColor(0xffffffff, 0xffffffff);
			pGDI->Print(WPoint(pt.x + 4.0f, pt.y + 5.0f), 0, pLine->text.c_str());
		}
	}
}

// rankingdlg.cpp:2010
void FrRankingDlg::OnRecordTypeListInit(int param) {
	m_pRecordTypeList = DYNAMIC_CAST(FrListBox, param);

	if (m_pRecordTypeList) {
		for (int i = 0; i < 5; i++) {
			m_recordTypeLine[i].text = std::string(s_aRecordType[i].m_name);
			m_pRecordTypeList->AddItem(&m_recordTypeLine[i]);
		}

		m_pRecordTypeList->SelectItem(m_pRecordTypeList->FindItem(&m_recordTypeLine[0]), true);
		m_pRecordTypeList->SetVisible(false);
		m_pRecordTypeList->EnableScrollBar(false);
	}
}

// rankingdlg.cpp:2034
void FrRankingDlg::OnRecordTypeListLDown(void) {
	FrListItem* pItem = m_pRecordTypeList->GetItemUnderCursor();

	if (pItem) {
		m_pRecordTypeList->SelectItem(pItem, true);
		m_pRecordTypeList->SetVisible(false);

		s_iSubType = pItem->idx;
		if (s_iSubType < 0) {
			s_iSubType = 0;
		} else if (s_iSubType > 4) {
			s_iSubType = 4;
		}

		s_iPageNum = 0;
		SendRequestRank(true);
	}
}

// rankingdlg.cpp:2058
void FrRankingDlg::OnRecordTypeListOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		FrListItem* pItem = (FrListItem*)param;
		if (pItem) {
			FrLine* pLine = (FrLine*)pItem->pData;
			WRect rect(pItem->pos.x, pItem->pos.y, 82.0f, 22.0f);

			WOverlay::DrawBox(g_view, rect, 0, 0xAA0E4F75, 0.0f);

			if (pItem->underCursor) {
				rect.x = pItem->pos.x + 1.0f;
				rect.y = pItem->pos.y + 1.0f;
				rect.w = 80.0f;
				rect.h = 20.0f;
				pGDI->Box(rect, 0x80082B46, 0, 0.0f);
				pGDI->SetTextStyle(1);
			} else {
				pGDI->SetTextStyle(0);
			}

			pGDI->SetTextColor(0xffffffff, 0xffffffff);
			pGDI->Print(WPoint(pItem->pos.x + 10.0f, pItem->pos.y + 6.0f), 0, pLine->text.c_str());
		}
	}
}

// rankingdlg.cpp:2087
void FrRankingDlg::OnRecordTypeViewerInit(int param) {
	m_pRecordTypeViewer = DYNAMIC_CAST(FrViewer, param);

	m_pRecordTypeViewerImg[0] = g_pFresh->RegisterBitmap("btn_drop02_n");
	m_pRecordTypeViewerImg[1] = g_pFresh->RegisterBitmap("btn_drop02_o");
	m_pRecordTypeViewerImg[2] = g_pFresh->RegisterBitmap("btn_drop02_d");
}

// rankingdlg.cpp:2096
void FrRankingDlg::OnRecordTypeViewerDown(void) {
	if (m_pRecordTypeList) {
		m_pRecordTypeList->SetVisible(!m_pRecordTypeList->IsVisible());
		g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}
}

// rankingdlg.cpp:2105
void FrRankingDlg::OnRecordTypeViewerOwnerDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		FrViewer* pViewer = m_pRecordTypeViewer;
		if (pViewer) {
			WRect rect;
			int state = 0;
			if (m_pRecordTypeList->IsVisible()) {
				state = 2;
			} else {
				rect = pViewer->GetRect();

				if (rect.IsInRect(g_pFresh->GetManager()->GetMousePos())) {
					state = 1;
				}
			}

			pGDI->DrawTexture(m_pRecordTypeViewerImg[state], pViewer->GetRect(), 0xffffffff, 0);

			const WRect& viewerRect = m_pRecordTypeViewer->GetRect();
			WPoint pt(viewerRect.x, viewerRect.y);
			FrLine* pLine = (FrLine*)m_pRecordTypeList->GetSelected()->pData;
			WRect viewRect(pt, viewerRect.w, viewerRect.h);

			WOverlay::DrawBox(g_view, viewRect, 0, 0x800E4F75, 0.0f);
			pGDI->SetTextStyle(1);

			pGDI->SetTextColor(0xffffffff, 0xffffffff);
			pGDI->Print(WPoint(pt.x + 4.0f, pt.y + 5.0f), 0, pLine->text.c_str());
		}
	}
}

// rankingdlg.cpp:2146
void FrRankingDlg::OnSearchTypeListInit(int param) {
	m_pSearchTypeList = DYNAMIC_CAST(FrListBox, param);

	if (m_pSearchTypeList) {
		for (int i = 0; i < 2; i++) {
			m_searchTypeLine[i].text = std::string(s_aSearchType[i].m_name);
			m_pSearchTypeList->AddItem(&m_searchTypeLine[i]);
		}

		m_pSearchTypeList->SelectItem(m_pSearchTypeList->FindItem(&m_searchTypeLine[0]), true);
		m_pSearchTypeList->SetVisible(false);
		m_pSearchTypeList->EnableScrollBar(false);
	}
}

// rankingdlg.cpp:2166
void FrRankingDlg::OnSearchTypeListLDown(void) {
	FrListItem* pItem = m_pSearchTypeList->GetItemUnderCursor();

	if (pItem) {
		m_pSearchTypeList->SelectItem(pItem, true);
		m_pSearchTypeList->SetVisible(false);

		s_iSearchType = pItem->idx;
	}
}

// rankingdlg.cpp:2177
void FrRankingDlg::OnSearchTypeListOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		FrListItem* pItem = (FrListItem*)param;
		if (pItem) {
			FrLine* pLine = (FrLine*)pItem->pData;
			WRect rect(pItem->pos.x, pItem->pos.y, 82.0f, 22.0f);

			WOverlay::DrawBox(g_view, rect, 0, 0xAA0E4F75, 0.0f);

			if (pItem->underCursor) {
				rect.x = pItem->pos.x + 1.0f;
				rect.y = pItem->pos.y + 1.0f;
				rect.w = 80.0f;
				rect.h = 20.0f;
				pGDI->Box(rect, 0x80082B46, 0, 0.0f);
				pGDI->SetTextStyle(1);
			} else {
				pGDI->SetTextStyle(0);
			}

			pGDI->SetTextColor(0xffffffff, 0xffffffff);
			pGDI->Print(WPoint(pItem->pos.x + 10.0f, pItem->pos.y + 6.0f), 0, pLine->text.c_str());
		}
	}
}

// rankingdlg.cpp:2206
void FrRankingDlg::OnSearchTypeViewerInit(int param) {
	m_pSearchTypeViewer = DYNAMIC_CAST(FrViewer, param);

	m_pSearchTypeViewerImg[0] = g_pFresh->RegisterBitmap("btn_drop03_n");
	m_pSearchTypeViewerImg[1] = g_pFresh->RegisterBitmap("btn_drop03_o");
	m_pSearchTypeViewerImg[2] = g_pFresh->RegisterBitmap("btn_drop03_d");
}

// rankingdlg.cpp:2215
void FrRankingDlg::OnSearchTypeViewerDown(void) {
	if (m_pSearchTypeList) {
		m_pSearchTypeList->SetVisible(!m_pSearchTypeList->IsVisible());
		g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}
}

// rankingdlg.cpp:2224
void FrRankingDlg::OnSearchTypeViewerOwnerDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		FrViewer* pViewer = m_pSearchTypeViewer;
		if (pViewer) {
			WRect rect;
			int state = 0;
			if (m_pSearchTypeList->IsVisible()) {
				state = 2;
			} else {
				rect = pViewer->GetRect();

				if (rect.IsInRect(g_pFresh->GetManager()->GetMousePos())) {
					state = 1;
				}
			}

			pGDI->DrawTexture(m_pSearchTypeViewerImg[state], pViewer->GetRect(), 0xffffffff, 0);

			const WRect& viewerRect = m_pSearchTypeViewer->GetRect();
			WPoint pt(viewerRect.x, viewerRect.y);
			FrLine* pLine = (FrLine*)m_pSearchTypeList->GetSelected()->pData;
			WRect viewRect(pt, viewerRect.w, viewerRect.h);

			WOverlay::DrawBox(g_view, viewRect, 0, 0x800E4F75, 0.0f);
			pGDI->SetTextStyle(1);

			pGDI->SetTextColor(0xffffffff, 0xffffffff);
			pGDI->Print(WPoint(pt.x + 4.0f, pt.y + 5.0f), 0, pLine->text.c_str());
		}
	}
}

// rankingdlg.cpp:2264
void FrRankingDlg::OnUserRankEditInit(int param) {
	m_pUserRankEdit = DYNAMIC_CAST(FrEdit, param);
	m_pUserRankEdit->SetVisible(false);
}

// rankingdlg.cpp:2270
void FrRankingDlg::OnNotify(const char* msg, unsigned long param) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
	pForm->SetTimeLimit(1.0f);
	pForm->SetMessage(msg, false);
	pForm->Open((FRESH_RESULT_CALLBACK)0, param);

	m_pBottomMsg->SetVisible(false);
}

// rankingdlg.cpp:2280
void FrRankingDlg::SetBottomMsg(const char* msg) {
	m_pBottomMsg->SetCaption(msg);
	m_pBottomMsg->SetVisible(true);
}

// rankingdlg.cpp:2286
void FrRankingDlg::OnCustomProc(float dt) {
	if (m_anotherHand.IsWorking(0)) {
		if (m_pGhostBtn && m_pGhostBtn->IsVisible()) {
			m_pGhostBtn->SetVisible(false);
		}

		Sleep(10);
		return;
	}

	m_anotherHand.Enter();

	if (m_pExhibition && m_pPetLoadGauge && !m_pPetLoadGauge->IsVisible()) {
		WRect rect = m_pPetCharacterArea->GetRect();
		m_pExhibition->SetCenter(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f);
		m_pExhibition->Process(dt, true);
	}

	m_anotherHand.Leave();
}

// rankingdlg.cpp:2331
bool FrRankingDlg::OnEmoticonResult(int ret, FrForm* pForm) {
	if (m_pEmoticonDlg && ret == 1) {
		FrEmoticonDlg* pDlg = DYNAMIC_CAST(FrEmoticonDlg, pForm);
		if (pDlg) {
			const char* icon = pDlg->GetSelectedIcon();
			if (icon) {
				FrEdit* pEdit = m_pUserIdEdit;

				if (pEdit) {
					const char* text = pEdit->GetEditText_Front();
					pEdit->SetLine(1, MakeStr("%s%s", text, icon), 0, false, 0);
					pEdit->SetKeyFocus(true);
				}
			}
		}
	}

	m_pEmoticonDlg = 0;
	return true;
}

// rankingdlg.cpp:2357
bool FrRankingDlg::OnAboutDlgResult(int ret, FrForm* pForm) {
	m_pAboutDlg = 0;
	return true;
}

// rankingdlg.cpp:2363
void FrRankingDlg::OnGhostBtnInit(int param) {
	m_pGhostBtn = DYNAMIC_CAST(FrButton, param);
	m_pGhostBtn->SetVisible(false);
	m_pGhostBtn->Enable(false);
}

// rankingdlg.cpp:2370
void FrRankingDlg::OnGhostBtnUp(void) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), 0, "notify", 0);
	pForm->SetTimeLimit(5.0f);
	// "The service is being prepared."
	pForm->SetMessage("\xBC\xAD\xBA\xF1\xBD\xBA \xC1\xD8\xBA\xF1 \xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.", false);
	pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
}

// rankingdlg.cpp:2396
CRankingInfo::CRankingInfo(void) {
	m_pRankingDlg = 0;
}

// rankingdlg.cpp:2401
bool CRankingInfo::Open(void) {
	if (!m_pRankingDlg) {
		m_pRankingDlg = CreateForm<FrRankingDlg>(g_pFresh->GetManager(), this, "rankingdlg", 0);
		m_pRankingDlg->Open((FRESH_RESULT_CALLBACK)&CRankingInfo::OnRankingDlgResult, 1);
		m_pRankingDlg->EnableDrag(false);

		if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false) {
			ntAvatarManager::Instance()->SetActive(false);
		}
	}

	return true;
}

// rankingdlg.cpp:2415
void CRankingInfo::Close(void) {
	if (m_pRankingDlg) {
		m_pRankingDlg->Close(true);
	}
}

// rankingdlg.cpp:2422
bool CRankingInfo::OnRankingDlgResult(int ret, FrForm* pForm) {
	if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_RANK)) WNetworkSystem::Instance()->ForceShutDown(WNetworkSystem::NET_UNIT_RANK);
	m_pRankingDlg = 0;

	if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false) {
		ntAvatarManager::Instance()->SetActive(true);
	}
	return true;
}

// rankingdlg.cpp:2432
void CRankingInfo::Process(float dt) {
	if (m_pRankingDlg) {
		m_pRankingDlg->OnCustomProc(dt);
	}
}

// rankingdlg.cpp:2438
void CRankingInfo::OnNotify(const char* msg, unsigned long param) {
	if (m_pRankingDlg) {
		m_pRankingDlg->OnNotify(msg, param);
	} else {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		pForm->SetTimeLimit(3.0f);
		pForm->SetMessage(msg, false);
		pForm->Open((FRESH_RESULT_CALLBACK)0, param);
	}
}

// rankingdlg.cpp:2452 (run-time type and factory registration of FrRankingAboutDlg)
const WRTTI FrRankingAboutDlg::m_RTTI("FrRankingAboutDlg", &FrForm::m_RTTI);

IObject* FrRankingAboutDlgMakeInstance(void) {
	return new FrRankingAboutDlg;
}

struct __sFrRankingAboutDlg {
	__sFrRankingAboutDlg(void) {
		ObjectFactory().AddObjectFunctor(FrRankingAboutDlgMakeInstance, "FrRankingAboutDlg");
	}
};
static __sFrRankingAboutDlg s_registerFrRankingAboutDlg;

// rankingdlg.cpp:2454
const sFRESH_MSGMAP* FrRankingAboutDlg::GetMessageMap(void) const {
	return &FrRankingAboutDlg::_MsgMap;
}

// rankingdlg.cpp:2475
FrRankingAboutDlg::FrRankingAboutDlg(void) {
	for (int i = 0; i < 4; i++) {
		m_pIndexBtn[i] = 0;
	}
	m_aboutIndex = 0;
	m_pBackwardBtn = 0;
	m_pForwardBtn = 0;
}

// rankingdlg.cpp:2489
FrRankingAboutDlg::~FrRankingAboutDlg(void) {
}

// rankingdlg.cpp:2493
void FrRankingAboutDlg::OnAboutViewerInit(intptr_t param) {
	m_pAboutViewer = DYNAMIC_CAST(FrViewer, param);
}

// rankingdlg.cpp:2499
void FrRankingAboutDlg::OnIndex01Init(intptr_t param) {
	m_pIndexBtn[0] = DYNAMIC_CAST(FrTextButton, param);
}

// rankingdlg.cpp:2504
void FrRankingAboutDlg::OnIndex01BnUp(void) {
	SetImgAboutIndex(0);
}

// rankingdlg.cpp:2509
void FrRankingAboutDlg::OnIndex02Init(intptr_t param) {
	m_pIndexBtn[1] = DYNAMIC_CAST(FrTextButton, param);
}

// rankingdlg.cpp:2514
void FrRankingAboutDlg::OnIndex02BnUp(void) {
	SetImgAboutIndex(1);
}

// rankingdlg.cpp:2519
void FrRankingAboutDlg::OnIndex03Init(intptr_t param) {
	m_pIndexBtn[2] = DYNAMIC_CAST(FrTextButton, param);
}

// rankingdlg.cpp:2524
void FrRankingAboutDlg::OnIndex03BnUp(void) {
	SetImgAboutIndex(2);
}

// rankingdlg.cpp:2529
void FrRankingAboutDlg::OnIndex04Init(intptr_t param) {
	m_pIndexBtn[3] = DYNAMIC_CAST(FrTextButton, param);
}

// rankingdlg.cpp:2534
void FrRankingAboutDlg::OnIndex04BnUp(void) {
	SetImgAboutIndex(3);
}

// rankingdlg.cpp:2539
void FrRankingAboutDlg::OnBackwardBnInit(intptr_t param) {
	m_pBackwardBtn = DYNAMIC_CAST(FrButton, param);
}

// rankingdlg.cpp:2544
void FrRankingAboutDlg::OnBackwardBnUp(void) {
	SetImgAboutIndex(--m_aboutIndex);
	m_pBackwardBtn->SetPushDelay(0.0f);
}

// rankingdlg.cpp:2550
void FrRankingAboutDlg::OnForwardBnInit(intptr_t param) {
	m_pForwardBtn = DYNAMIC_CAST(FrButton, param);
}

// rankingdlg.cpp:2555
void FrRankingAboutDlg::OnForwardBnUp(void) {
	SetImgAboutIndex(++m_aboutIndex);
	m_pForwardBtn->SetPushDelay(0.0f);
}

// rankingdlg.cpp:2561
void FrRankingAboutDlg::OnOkBnInit(intptr_t param) {
	if ((__rtti_obj = (void*)param) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&FrButton::m_RTTI)) {
	}
}

// rankingdlg.cpp:2566
void FrRankingAboutDlg::OnOkBnUp(void) {
	OnFreshOkay();
}

// rankingdlg.cpp:2571
bool FrRankingAboutDlg::OnInit(void) {
	SetImgAboutIndex(0);
	return true;
}

// rankingdlg.cpp:2579
void FrRankingAboutDlg::SetImgAboutIndex(int index) {
	for (int i = 0; i < 4; i++) {
		m_pIndexBtn[i]->Unselect(0);
	}

	if (m_pBackwardBtn) {
		m_pBackwardBtn->Enable(true);
	}

	if (m_pForwardBtn) {
		m_pForwardBtn->Enable(true);
	}

	if (index <= 0) {
		index = 0;
		m_pIndexBtn[0]->Select(0);

		if (m_pBackwardBtn) {
			m_pBackwardBtn->Enable(false);
		}
	} else if (index >= 3) {
		index = 3;
		m_pIndexBtn[3]->Select(0);

		if (m_pForwardBtn) {
			m_pForwardBtn->Enable(false);
		}
	}

	m_pIndexBtn[index]->Select(0);

	m_aboutIndex = index;
	if (m_pAboutViewer) {
		m_pAboutViewer->Open(MakeStr("help_ranking_%02d.jpg", index + 1));
	}
}
