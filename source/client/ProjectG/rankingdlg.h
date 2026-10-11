// Original: d:\build\custom\temp\code\client\projectg\rankingdlg.h
#ifndef PG_CLIENT_PROJECTG_RANKINGDLG_H
#define PG_CLIENT_PROJECTG_RANKINGDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "frcmdtarget.h"
#include "singleton.h"
#include "AnotherHand.h"
#include "fredit.h"

#include <string>
#include <list>

class FrTextButton;
class FrButton;
class FrViewer;
class FrWnd;
class FrStatic;
class FrListBox;
class FrComboBox;
class FrEdit;
class FrArea;
class FrGaugeBar;
class Bitmap;
class WPoint;
class CExhibition;
class FrEmoticonDlg;
struct FrListItem;

namespace ranktype {

// One line of the rank list (the data of the rank list box's items): the
// player's UID and nickname (layout partial; the rest is not used yet).
struct sListInfo {
	unsigned long uid;      // 0x0
	UNKNOWN(0x4, 0x14);
	std::string nickname;   // 0x14
};

CHECK_FIELD(sListInfo, uid, 0x0);
CHECK_FIELD(sListInfo, nickname, 0x14);

} // namespace ranktype

// One entry of the ranking dialog's type lists: the value sent to the rank
// server and the name the list shows (8 bytes; the tables are walked by
// rankingdlg.cpp's list initializers).
struct sTypeName {
	int m_type;           // 0x0
	const char* m_name;   // 0x4
};

CHECK_SIZE(sTypeName, 0x8);

// Ranking dialog; its size (0xae0) is the `new` of FrRankingDlgMakeInstance.
// CHandOwner (0x110) receives the pet model loaded on the worker thread
// (OnAnotherHand adjusts `this` by -0x110).
class FrRankingDlg : public FrForm, public CHandOwner {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	FrRankingDlg(void);
	virtual ~FrRankingDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);
	virtual void OnCancel(void);
	virtual void OnProc(const float dt);
	virtual void OnAnotherHand(int nWork, void* pData);

	void ResetControl(void);
	void SetVisibleUserInfo(bool bVisible);
	void OnRankList_LBtnDown(void);
	void OnRankList_MyInfo_LBtnDown(void);
	void OnEmoticonBtnUp(void);
	bool OnEmoticonResult(int ret, FrForm* pForm);
	void SendRequestRank(bool bFirst);
	void SendRequestUserInfo(unsigned long uid, const char* nickname);
	void HidePet(bool bHide);
	void OnTermType_S5_BtnUp(void);
	void OnRankList_Init(int param);
	void OnRankList_OwnerDraw(int param);
	void OnRankList_PrevPageBtnUp(void);
	void OnAboutBtnInit(int param);
	void OnRankTypeViewerDown(void);
	void OnGeneralSubTypeViewerDown(void);
	void OnCourseTypeViewerDown(void);
	void OnRecordTypeViewerDown(void);
	void OnSearchTypeListLDown(void);
	void OnSearchTypeViewerDown(void);
	void OnGhostBtnUp(void);
	void SetListItemInfo(FrListItem* pItem);
	void SetPetCharacter(void);
	void OnRankList_MyInfo_OwnerDraw(int param);
	void SetVisibleByRankType(void);
	void DrawUserInfoLine(int type, int line, WPoint& pos);
	void ShowUserInfo(void);
	void OnRankList_MyInfo_Init(int param);
	void OnAboutBtnUp(void);
	void OnClassType_ComboInit(int param);
	void OnClassType_ComboDown(int param);
	void SendRequestSearchUser(const char* nickname);
	void OnRankList_SearchUserBtnUp(void);
	void SynchronizeButtons(void);
	void OnRankTypeListInit(int param);
	void OnGeneralSubTypeListInit(int param);
	void OnCourseTypeListInit(int param);
	void OnRecordTypeListInit(int param);
	void CloseAllComboBtns(void);
	void OnSearchTypeListInit(int param);
	void OnRankTypeListLDown(void);
	void OnRankTypeListOwnerDraw(int param);
	void OnGeneralSubTypeListOwnerDraw(int param);
	void OnCourseTypeListOwnerDraw(int param);
	void OnRecordTypeListOwnerDraw(int param);
	void OnSearchTypeListOwnerDraw(int param);
	void OnGeneralSubTypeListLDown(void);
	void OnCourseTypeListLDown(void);
	void OnRecordTypeListLDown(void);
	void OnPetCharacterOwnerDraw(void);
	void OnRankTypeViewerOwnerDraw(void);
	void OnGeneralSubTypeViewerOwnerDraw(void);
	void OnCourseTypeViewerOwnerDraw(void);
	void OnRecordTypeViewerOwnerDraw(void);
	void OnSearchTypeViewerOwnerDraw(void);

	void OnCourseAreaOwnerDraw(int param);
	void OnRankList_NextPageBtnUp(void);
	void OnRankList_UserIdEditDown(void);
	void OnCloseBtnUp(void);
	bool OnAboutDlgResult(int ret, FrForm* pForm);
	void SetAllControlsEnable(bool bEnable);
	void OnTermType_S5_BtnInit(int param);
	void OnCourseAreaInit(int param);
	void OnRankList_PrevPageInit(int param);
	void OnRankList_NextPageInit(int param);
	void OnRankList_CurrentPageInit(int param);
	void OnRankList_TotalPageInit(int param);
	void OnRankList_NoInRank_Init(int param);
	void OnRankList_SearchUserInit(int param);
	void OnRankList_UserIdEditInit(int param);
	void OnEmoticonInit(intptr_t param);
	void OnBottomMessageInit(int param);
	void OnPetCharacterInit(int param);
	void OnPetLoadInit(int param);
	void OnUpperMiniInfoInit(int param);
	void OnUserRankEditInit(int param);
	void OnGhostBtnInit(int param);
	void OnLanguageInit(intptr_t param);
	void OnRankTypeViewerInit(int param);
	void OnGeneralSubTypeViewerInit(int param);
	void OnCourseTypeViewerInit(int param);
	void OnRecordTypeViewerInit(int param);
	void OnSearchTypeViewerInit(int param);

	static int s_iRankType;
	static int s_iSubType;
	static int s_iTermType;
	static int s_iClassType;
	static int s_iPageNum;
	static int s_iPageTotal;
	static int s_iSearchType;

	static const sTypeName s_aRankType[];
	static const sTypeName s_aClassType[];
	static const sTypeName s_aGenSubType[];
	static const sTypeName s_aCourseType[];
	static const sTypeName s_aRecordType[];
	static const sTypeName s_aTermType[];
	static const sTypeName s_aSearchType[];

public:
	static void SetRankTypes(unsigned char rankType, unsigned char subType, unsigned char termType,
		unsigned char classType, int pageNum, int pageTotal);
	void UpdateRankList(void);
	void UpdateUserInfo(void);
	void DisconnectRankServer(void);
	void UpdateMyInfo(void);
	void SetBottomMsg(const char* msg);
	void OnCustomProc(float dt);
	void OnNotify(const char* msg, unsigned long param);
	void ClearRankList(void);
	void SelectRankList(int index);

private:
	std::list<ranktype::sListInfo> m_rankList;   // 0x114, lines of the current rank page
	int m_myRank;               // 0x120, first member of the local player's rank entry (-1: unranked)
	UNKNOWN(0x124, 0x16c);
	unsigned long m_myUID;      // 0x16c
	char m_myNickname[1];       // 0x170, (length unknown)
	UNKNOWN(0x171, 0x35c);
	unsigned long m_userRank;   // 0x35c, shown in m_pUserRankEdit (UpdateUserInfo; 0: unranked)
	UNKNOWN(0x360, 0x398);
	FrLine m_courseTypeLine[17];     // 0x398, list items of m_pCourseTypeList
	FrLine m_searchTypeLine[2];      // 0x640, list items of m_pSearchTypeList (OnSearchTypeListInit)
	FrLine m_recordTypeLine[5];      // 0x690, list items of m_pRecordTypeList (OnRecordTypeListInit)
	FrLine m_rankTypeLine[3];        // 0x758, list items of m_pRankTypeList (OnRankTypeListInit)
	FrLine m_genSubTypeLine[5];      // 0x7d0, list items of m_pGenSubTypeList (OnGeneralSubTypeListInit)
	FrButton* m_pTermS5Btn;     // 0x898
	FrComboBox* m_pClassTypeCombo;   // 0x89c (OnClassType_ComboInit)
	FrListBox* m_pRankList;     // 0x8a0 (OnRankList_Init)
	FrListBox* m_pMyInfoList;   // 0x8a4, the local player's rank line
	FrArea* m_pCourseArea;      // 0x8a8 (OnCourseAreaInit)
	FrWnd* m_pNoMyRankWnd;      // 0x8ac, shown when the local player is unranked
	FrButton* m_pSearchUserBtn; // 0x8b0 (OnRankList_SearchUserInit)
	FrEdit* m_pUserIdEdit;      // 0x8b4, nickname to search (OnRankList_UserIdEditInit)
	FrButton* m_pEmoticonBtn;   // 0x8b8 (OnEmoticonInit)
	FrArea* m_pLanguageArea;    // 0x8bc, IME language indicator (OnLanguageInit)
	FrEdit* m_pCurPageEdit;     // 0x8c0, current page number (OnRankList_CurrentPageInit)
	FrEdit* m_pTotalPageEdit;   // 0x8c4, page count (OnRankList_TotalPageInit)
	FrEdit* m_pUserRankEdit;    // 0x8c8 (OnUserRankEditInit)
	FrGaugeBar* m_pPetLoadGauge; // 0x8cc, pet loading progress (OnPetLoadInit)
	FrStatic* m_pBottomMsg;   // 0x8d0, "bottom_message" (OnBottomMessageInit)
	FrEmoticonDlg* m_pEmoticonDlg;   // 0x8d4, the open emoticon picker (OnEmoticonBtnUp)
	FrForm* m_pAboutDlg;   // 0x8d8, the open FrRankingAboutDlg (cleared by its result callback)
	FrListBox* m_pRankTypeList;      // 0x8dc
	FrViewer* m_pRankTypeViewer;     // 0x8e0
	const Bitmap* m_pRankTypeViewerImg[3];    // 0x8e4, normal/over/down
	FrListBox* m_pGenSubTypeList;    // 0x8f0
	FrViewer* m_pGenSubTypeViewer;   // 0x8f4
	const Bitmap* m_pGenSubTypeViewerImg[3];  // 0x8f8
	FrListBox* m_pCourseTypeList;    // 0x904
	FrViewer* m_pCourseTypeViewer;   // 0x908
	const Bitmap* m_pCourseTypeViewerImg[3];  // 0x90c
	FrListBox* m_pRecordTypeList;    // 0x918
	FrViewer* m_pRecordTypeViewer;   // 0x91c
	const Bitmap* m_pRecordTypeViewerImg[3];  // 0x920
	FrListBox* m_pSearchTypeList;    // 0x92c
	FrViewer* m_pSearchTypeViewer;   // 0x930
	const Bitmap* m_pSearchTypeViewerImg[3];  // 0x934
	FrArea* m_pPetCharacterArea;     // 0x940 (OnPetCharacterInit)
	CExhibition* m_pExhibition;      // 0x944, the pet shown (HidePet)
	char* m_pPetData;                // 0x948, released with the pet (type unknown)
	FrButton* m_pGhostBtn;           // 0x94c (OnGhostBtnInit)
	FrArea* m_pUpperMiniInfo;        // 0x950, area the user info lines are drawn in (OnUpperMiniInfoInit)
	CAnotherHand m_anotherHand;      // 0x954, loads the pet model (OnAnotherHand)
	float m_userInfoDelay;           // 0xadc, set by SendRequestUserInfo; clicks wait for it to run out
};

CHECK_SIZE(FrRankingDlg, 0xae0);
BEGIN_LAYOUT_CHECK(FrRankingDlg)
	CHECK_FIELD(FrRankingDlg, m_rankList, 0x114);
	CHECK_FIELD(FrRankingDlg, m_myRank, 0x120);
	CHECK_FIELD(FrRankingDlg, m_myUID, 0x16c);
	CHECK_FIELD(FrRankingDlg, m_myNickname, 0x170);
	CHECK_FIELD(FrRankingDlg, m_userRank, 0x35c);
	CHECK_FIELD(FrRankingDlg, m_pTermS5Btn, 0x898);
	CHECK_FIELD(FrRankingDlg, m_courseTypeLine, 0x398);
	CHECK_FIELD(FrRankingDlg, m_searchTypeLine, 0x640);
	CHECK_FIELD(FrRankingDlg, m_recordTypeLine, 0x690);
	CHECK_FIELD(FrRankingDlg, m_rankTypeLine, 0x758);
	CHECK_FIELD(FrRankingDlg, m_genSubTypeLine, 0x7d0);
	CHECK_FIELD(FrRankingDlg, m_pClassTypeCombo, 0x89c);
	CHECK_FIELD(FrRankingDlg, m_pUserIdEdit, 0x8b4);
	CHECK_FIELD(FrRankingDlg, m_pCurPageEdit, 0x8c0);
	CHECK_FIELD(FrRankingDlg, m_pTotalPageEdit, 0x8c4);
	CHECK_FIELD(FrRankingDlg, m_pGenSubTypeViewer, 0x8f4);
	CHECK_FIELD(FrRankingDlg, m_pCourseTypeViewer, 0x908);
	CHECK_FIELD(FrRankingDlg, m_pRecordTypeViewer, 0x91c);
	CHECK_FIELD(FrRankingDlg, m_pUpperMiniInfo, 0x950);
	CHECK_FIELD(FrRankingDlg, m_pEmoticonDlg, 0x8d4);
	CHECK_FIELD(FrRankingDlg, m_anotherHand, 0x954);
	CHECK_FIELD(FrRankingDlg, m_userInfoDelay, 0xadc);
	CHECK_FIELD(FrRankingDlg, m_pRankList, 0x8a0);
	CHECK_FIELD(FrRankingDlg, m_pMyInfoList, 0x8a4);
	CHECK_FIELD(FrRankingDlg, m_pNoMyRankWnd, 0x8ac);
	CHECK_FIELD(FrRankingDlg, m_pBottomMsg, 0x8d0);
	CHECK_FIELD(FrRankingDlg, m_pAboutDlg, 0x8d8);
	CHECK_FIELD(FrRankingDlg, m_pRankTypeList, 0x8dc);
	CHECK_FIELD(FrRankingDlg, m_pGenSubTypeList, 0x8f0);
	CHECK_FIELD(FrRankingDlg, m_pCourseTypeList, 0x904);
	CHECK_FIELD(FrRankingDlg, m_pRecordTypeList, 0x918);
	CHECK_FIELD(FrRankingDlg, m_pSearchTypeList, 0x92c);
	CHECK_FIELD(FrRankingDlg, m_pExhibition, 0x944);
	CHECK_FIELD(FrRankingDlg, m_pPetData, 0x948);
	CHECK_FIELD(FrRankingDlg, m_pCourseArea, 0x8a8);
	CHECK_FIELD(FrRankingDlg, m_pSearchUserBtn, 0x8b0);
	CHECK_FIELD(FrRankingDlg, m_pEmoticonBtn, 0x8b8);
	CHECK_FIELD(FrRankingDlg, m_pUserRankEdit, 0x8c8);
	CHECK_FIELD(FrRankingDlg, m_pPetLoadGauge, 0x8cc);
	CHECK_FIELD(FrRankingDlg, m_pRankTypeViewer, 0x8e0);
	CHECK_FIELD(FrRankingDlg, m_pSearchTypeViewer, 0x930);
	CHECK_FIELD(FrRankingDlg, m_pPetCharacterArea, 0x940);
	CHECK_FIELD(FrRankingDlg, m_pGhostBtn, 0x94c);
	CHECK_FIELD(FrRankingDlg, m_pLanguageArea, 0x8bc);
	CHECK_FIELD(FrRankingDlg, m_pRankTypeViewerImg, 0x8e4);
	CHECK_FIELD(FrRankingDlg, m_pGenSubTypeViewerImg, 0x8f8);
	CHECK_FIELD(FrRankingDlg, m_pCourseTypeViewerImg, 0x90c);
	CHECK_FIELD(FrRankingDlg, m_pRecordTypeViewerImg, 0x920);
	CHECK_FIELD(FrRankingDlg, m_pSearchTypeViewerImg, 0x934);
END_LAYOUT_CHECK

// Help pages of the ranking dialog: four index buttons, backward/forward
// buttons and a viewer showing help_ranking_NN.jpg.
class FrRankingAboutDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	FrRankingAboutDlg(void);
	virtual ~FrRankingAboutDlg(void);

	void SetImgAboutIndex(int index);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);

	void OnIndex01BnUp(void);
	void OnIndex02BnUp(void);
	void OnIndex03BnUp(void);
	void OnIndex04BnUp(void);
	void OnBackwardBnUp(void);
	void OnForwardBnUp(void);
	void OnOkBnInit(intptr_t param);
	void OnAboutViewerInit(intptr_t param);
	void OnIndex01Init(intptr_t param);
	void OnIndex02Init(intptr_t param);
	void OnIndex03Init(intptr_t param);
	void OnIndex04Init(intptr_t param);
	void OnBackwardBnInit(intptr_t param);
	void OnForwardBnInit(intptr_t param);
	void OnOkBnUp(void);

private:
	FrTextButton* m_pIndexBtn[4];   // 0x110 ("index01".."index04")
	int m_aboutIndex;               // 0x120, page shown
	FrButton* m_pBackwardBtn;       // 0x124
	FrButton* m_pForwardBtn;        // 0x128
	FrViewer* m_pAboutViewer;       // 0x12c
};

CHECK_SIZE(FrRankingAboutDlg, 0x130);
BEGIN_LAYOUT_CHECK(FrRankingAboutDlg)
	CHECK_FIELD(FrRankingAboutDlg, m_pIndexBtn, 0x110);
	CHECK_FIELD(FrRankingAboutDlg, m_aboutIndex, 0x120);
	CHECK_FIELD(FrRankingAboutDlg, m_pBackwardBtn, 0x124);
	CHECK_FIELD(FrRankingAboutDlg, m_pForwardBtn, 0x128);
	CHECK_FIELD(FrRankingAboutDlg, m_pAboutViewer, 0x12c);
END_LAYOUT_CHECK

// Owner of the ranking dialog: opens it, forwards the frame and the
// notifications to it and closes it.
class CRankingInfo : public WSingleton<CRankingInfo>, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;

public:
	CRankingInfo(void);
	virtual ~CRankingInfo(void);

	bool Open(void);
	void Close(void);
	void Process(float dt);
	void OnNotify(const char* msg, unsigned long param);

	// rankingdlg.h:267
	FrRankingDlg* GetDlg(void) { return m_pRankingDlg; }

protected:
	bool OnRankingDlgResult(int ret, FrForm* pForm);

private:
	FrRankingDlg* m_pRankingDlg;   // 0x2c
};

BEGIN_LAYOUT_CHECK(CRankingInfo)
	CHECK_FIELD(CRankingInfo, m_pRankingDlg, 0x2c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_RANKINGDLG_H
