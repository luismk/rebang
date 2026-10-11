// Original: d:\build\custom\temp\code\client\projectg\post_recivedlg.h
#ifndef PG_CLIENT_PROJECTG_POST_RECIVEDLG_H
#define PG_CLIENT_PROJECTG_POST_RECIVEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "ToolTipManager.h"
#include "../shared/globalgamedefine.h"
#include <list>

class FrArea;
class FrGraphicInterface;
class CPost_Contents;
class FrButton;
class FrEdit;
class FrListBox;
struct FrListItem;
class Bitmap;
class WTitleFont;
struct sMailInfo;

// Mail box dialog; its size (0x200) is the `new` of
// CPost_ReciveDlgMakeInstance. Only the members this unit defines so far are
// declared.
class CPost_ReciveDlg : public FrForm {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // post_recivedlg.h:28

	CPost_ReciveDlg(void);
	virtual ~CPost_ReciveDlg(void);

	void OnLoadPostList(void);                                // post_recivedlg.cpp:1652
	void OnReset(void);                                       // post_recivedlg.cpp:758
	void ToServerSendData(FrListItem *pItem, unsigned long);  // post_recivedlg.cpp
	bool OnPostQuestionResult(int ret, FrForm *pForm);        // post_recivedlg.cpp:885
	bool OnPostMessageResult(int ret, FrForm *pForm);         // post_recivedlg.cpp:968
	void EnableMiniBtns(bool bEnable);                        // post_recivedlg.cpp:1488
	void Question(int type);                                  // post_recivedlg.cpp:866
	void OnPostDescDlgCreate(void);                           // post_recivedlg.cpp:952
	void BuildIndexPage(int page);                            // post_recivedlg.cpp
	bool OnDeleteAllLetterResult(int ret, FrForm *pForm);     // post_recivedlg.cpp:221
	bool OnMoveGiftDlgResult(int ret, FrForm *pForm);         // post_recivedlg.cpp:1314
	bool OnReturnGiftDlgResult(int ret, FrForm *pForm);       // post_recivedlg.cpp:1294
	void SetLoadPostList(void);                               // post_recivedlg.cpp:1664
	void SetLoadPostList(std::list<sMailInfoBrief>* pList);   // post_recivedlg.cpp:1716

	// Two-element array at 0x140: the gift-move and delete buttons drawn on
	// the post list (OnLoadImage).
	struct miniBtn_t {
		miniBtn_t(void);

		WRect m_rect;                  // 0x00, excluded from the post list's right-button area
		const Bitmap* m_pNormalImg;    // 0x10
		const Bitmap* m_pOverImg;      // 0x14
		bool m_enabled;                // 0x18
		UNKNOWN(0x19, 0x1C);
	};

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // post_recivedlg.cpp:35

	void OnRecivePostInit(intptr_t param);       // post_recivedlg.cpp:152
	void OnRecivePostBtnUp(void);     // post_recivedlg.cpp:159
	void OnSendPostInit(intptr_t param);         // post_recivedlg.cpp:164
	void OnSendPostBtnUp(void);             // post_recivedlg.cpp:171
	void OnDeleteAllLetterInit(intptr_t param);  // post_recivedlg.cpp:178
	void OnWriteLetterInit(intptr_t param);      // post_recivedlg.cpp:249
	void OnWriteLetterBtnUp(void);          // post_recivedlg.cpp:256
	void OnPageListInit(intptr_t param);         // post_recivedlg.cpp:275
	void OnPostListInit(intptr_t param);         // post_recivedlg.cpp:374
	void OnPageDownInit(intptr_t param);         // post_recivedlg.cpp:985
	void OnPageDownBtnUp(void);             // post_recivedlg.cpp:993
	void OnPageUpInit(intptr_t param);           // post_recivedlg.cpp:1055
	void OnPageUpBtnUp(void);               // post_recivedlg.cpp:1061
	void OnToolTipInit(intptr_t param);          // post_recivedlg.cpp:1375
	void OnToolTipOnwerDraw(int param);     // post_recivedlg.cpp:1381
	void OnIndexAreaInit(intptr_t param);        // post_recivedlg.cpp:1502
	void OnIndexListInit(intptr_t param);        // post_recivedlg.cpp:1562
	void OnPageListBtnDown(void);     // post_recivedlg.cpp:349
	void OnPostListRBtnDown(void);    // post_recivedlg.cpp:849
	void OnPostListMouseMove(void);   // post_recivedlg.cpp:948
	void OnPostListDClick(void);      // post_recivedlg.cpp:1123
	const char* MakeStatusString(int statusType, int statusValue, int slotValue);   // post_recivedlg.cpp:777
	bool OnEditPageEndterKey(int);    // post_recivedlg.cpp:1129
	void OnDeleteItem(FrListItem *pItem); // post_recivedlg.cpp:1774
	void OnLoadImage(void);                 // post_recivedlg.cpp
	void DrawStatus2(FrGraphicInterface* pGI, FrListItem* pItem, short status[5], short slot[5], float x);   // post_recivedlg.cpp:764
	void OnPostListOwnerDraw(intptr_t param);    // post_recivedlg.cpp
	void OnPostListBtnDown(void);           // post_recivedlg.cpp
	void OnPageListOwnerDraw(intptr_t param);    // post_recivedlg.cpp
	void OnDeleteAllLetterBtnUp(void);      // post_recivedlg.cpp
	void OnIndexAreaOwnerDraw(void);        // post_recivedlg.cpp
	void OnIndexListLBDown(void);           // post_recivedlg.cpp
	void OnIndexListMouseMove(void);        // post_recivedlg.cpp

private:
	FrButton* m_pRecivePostBtn;       // 0x110
	FrButton* m_pSendPostBtn;         // 0x114
	FrButton* m_pDeleteAllLetterBtn;  // 0x118
	FrButton* m_pWriteLetterBtn;      // 0x11C
	FrListBox* m_pPostList;           // 0x120
	FrListBox* m_pPageList;           // 0x124, one item per page, the page number as its data
	PADDING(0x128, 0x130);
	FrButton* m_pPageDownBtn;         // 0x130
	FrButton* m_pPageUpBtn;           // 0x134
	PADDING(0x138, 0x13C);
	FrArea* m_pToolTipArea;           // 0x13C
	miniBtn_t m_miniBtns[2];          // 0x140
	const Bitmap* m_pItemBaseNormalImg;   // 0x178
	const Bitmap* m_pItemBaseOverImg;     // 0x17C
	const Bitmap* m_pItemBaseDownImg;     // 0x180
	const Bitmap* m_pItemBaseEmptyImg;    // 0x184
	const Bitmap* m_pArrivalMailImg;      // 0x188
	const Bitmap* m_pOpenMailImg;         // 0x18C
	CToolTipManager m_toolTipManager; // 0x190
	const unsigned long* m_pMoveGiftIds;   // 0x1A0, the two ids sent with the move-gift request (0x24)
	PADDING(0x1A4, 0x1A8);
	int m_curPage;                    // 0x1A8, initialised to the same value as 0x1CC (the page sent to the server); reset to 1 by OnReset
	std::list<sMailInfoBrief> m_mailList;   // 0x1AC
	PADDING(0x1B8, 0x1C4);
	CPost_Contents* m_pContentsDlg;   // 0x1C4, the open letter (OnPostDescDlgCreate)
	int m_totalPage;                  // 0x1C8, the last page OnPageListBtnDown accepts
	int m_requestPage;                // 0x1CC, the page OnLoadPostList asks the server for
	int m_pageListStep;               // 0x1D0, the page list's item i holds i times this value
	FrListItem* m_pQuestionItem;      // 0x1D4, the letter a yes/no question is asked about
	int m_questionType;               // 0x1D8, non-zero: answering yes sends 0xBE, zero: 0xBF
	WTitleFont* m_pTitleFont;         // 0x1DC
	PADDING(0x1E0, 0x1E4);
	FrArea* m_pIndexArea;             // 0x1E4
	FrListBox* m_pIndexList;          // 0x1E8
	unsigned int m_indexPageCount;    // 0x1EC
	unsigned int m_indexPage;         // 0x1F0, the index page shown (BuildIndexPage)
	unsigned char m_indexSelected;    // 0x1F4, the index entry last clicked (OnIndexListLBDown), cleared when another index page is requested
	unsigned char m_indexHover;       // 0x1F5, the index entry under the cursor, 0xFF for none
	unsigned char m_indexItemCount;   // 0x1F6, entries on the index page shown (at most 5)
	PADDING(0x1F7, 0x1F8);
	WRect* m_pIndexRects;             // 0x1F8, one rect per index entry (left, top, right, bottom in x, y, w, h), BuildIndexPage
	int m_questionIndex;              // 0x1FC, -1 once a question is answered no
	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(CPost_ReciveDlg::miniBtn_t, 0x1C);
CHECK_FIELD(CPost_ReciveDlg::miniBtn_t, m_pNormalImg, 0x10);
CHECK_FIELD(CPost_ReciveDlg::miniBtn_t, m_pOverImg, 0x14);
CHECK_FIELD(CPost_ReciveDlg::miniBtn_t, m_enabled, 0x18);
CHECK_SIZE(CPost_ReciveDlg, 0x200);
BEGIN_LAYOUT_CHECK(CPost_ReciveDlg)
	CHECK_FIELD(CPost_ReciveDlg, m_pRecivePostBtn, 0x110);
	CHECK_FIELD(CPost_ReciveDlg, m_pSendPostBtn, 0x114);
	CHECK_FIELD(CPost_ReciveDlg, m_pDeleteAllLetterBtn, 0x118);
	CHECK_FIELD(CPost_ReciveDlg, m_pWriteLetterBtn, 0x11C);
	CHECK_FIELD(CPost_ReciveDlg, m_pPostList, 0x120);
	CHECK_FIELD(CPost_ReciveDlg, m_pPageList, 0x124);
	CHECK_FIELD(CPost_ReciveDlg, m_pPageDownBtn, 0x130);
	CHECK_FIELD(CPost_ReciveDlg, m_pPageUpBtn, 0x134);
	CHECK_FIELD(CPost_ReciveDlg, m_pToolTipArea, 0x13C);
	CHECK_FIELD(CPost_ReciveDlg, m_miniBtns, 0x140);
	CHECK_FIELD(CPost_ReciveDlg, m_pItemBaseNormalImg, 0x178);
	CHECK_FIELD(CPost_ReciveDlg, m_pItemBaseOverImg, 0x17C);
	CHECK_FIELD(CPost_ReciveDlg, m_pItemBaseDownImg, 0x180);
	CHECK_FIELD(CPost_ReciveDlg, m_pItemBaseEmptyImg, 0x184);
	CHECK_FIELD(CPost_ReciveDlg, m_pArrivalMailImg, 0x188);
	CHECK_FIELD(CPost_ReciveDlg, m_pOpenMailImg, 0x18C);
	CHECK_FIELD(CPost_ReciveDlg, m_toolTipManager, 0x190);
	CHECK_FIELD(CPost_ReciveDlg, m_pMoveGiftIds, 0x1A0);
	CHECK_FIELD(CPost_ReciveDlg, m_curPage, 0x1A8);
	CHECK_FIELD(CPost_ReciveDlg, m_mailList, 0x1AC);
	CHECK_FIELD(CPost_ReciveDlg, m_pContentsDlg, 0x1C4);
	CHECK_FIELD(CPost_ReciveDlg, m_totalPage, 0x1C8);
	CHECK_FIELD(CPost_ReciveDlg, m_requestPage, 0x1CC);
	CHECK_FIELD(CPost_ReciveDlg, m_pageListStep, 0x1D0);
	CHECK_FIELD(CPost_ReciveDlg, m_pQuestionItem, 0x1D4);
	CHECK_FIELD(CPost_ReciveDlg, m_questionType, 0x1D8);
	CHECK_FIELD(CPost_ReciveDlg, m_pTitleFont, 0x1DC);
	CHECK_FIELD(CPost_ReciveDlg, m_pIndexArea, 0x1E4);
	CHECK_FIELD(CPost_ReciveDlg, m_pIndexList, 0x1E8);
	CHECK_FIELD(CPost_ReciveDlg, m_indexPageCount, 0x1EC);
	CHECK_FIELD(CPost_ReciveDlg, m_indexPage, 0x1F0);
	CHECK_FIELD(CPost_ReciveDlg, m_indexSelected, 0x1F4);
	CHECK_FIELD(CPost_ReciveDlg, m_indexHover, 0x1F5);
	CHECK_FIELD(CPost_ReciveDlg, m_indexItemCount, 0x1F6);
	CHECK_FIELD(CPost_ReciveDlg, m_pIndexRects, 0x1F8);
	CHECK_FIELD(CPost_ReciveDlg, m_questionIndex, 0x1FC);
END_LAYOUT_CHECK

// Contents of a letter; its size (0x130) is the `new` of
// CPost_ContentsMakeInstance.
class CPost_Contents : public FrForm {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // post_recivedlg.h:209

	CPost_Contents(void);
	virtual ~CPost_Contents(void);

	void SetData(sMailInfo* pMailInfo);                      // post_recivedlg.cpp:2428
	void SetData(std::list<sMailIncludeItem>* pItemList);
	void OnPostMessage_ReciveUserInit(intptr_t param);       // post_recivedlg.cpp:2055
	void OnPostMessage_SendUserInit(intptr_t param);         // post_recivedlg.cpp:2065
	void OnPostMessage_LetterContentsInit(intptr_t param);   // post_recivedlg.cpp:2073
	void OnPostMessage_LetterDateInit(intptr_t param);       // post_recivedlg.cpp:2081
	void OnPostListInit(intptr_t param);                     // post_recivedlg.cpp:2090
	void OnPostListOwnerDraw(intptr_t param);                // post_recivedlg.cpp
	void OnPostListBtnDown(void);     // post_recivedlg.cpp:2412
	void OnPostListRBtnDown(void);    // post_recivedlg.cpp:2416
	void OnPostListMouseMove(void);   // post_recivedlg.cpp:2420
	void OnPostListDClick(void);      // post_recivedlg.cpp:2424

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // post_recivedlg.cpp:1996

private:
	FrEdit* m_pSendUserEdit;         // 0x110
	FrEdit* m_pReciveUserEdit;       // 0x114
	FrEdit* m_pLetterContentsEdit;   // 0x118
	FrEdit* m_pLetterDateEdit;       // 0x11C
	FrListBox* m_pPostList;          // 0x120
	WTitleFont* m_pTitleFont;   // 0x124
	PADDING(0x128, 0x130);
	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(CPost_Contents, 0x130);
BEGIN_LAYOUT_CHECK(CPost_Contents)
	CHECK_FIELD(CPost_Contents, m_pSendUserEdit, 0x110);
	CHECK_FIELD(CPost_Contents, m_pReciveUserEdit, 0x114);
	CHECK_FIELD(CPost_Contents, m_pLetterContentsEdit, 0x118);
	CHECK_FIELD(CPost_Contents, m_pLetterDateEdit, 0x11C);
	CHECK_FIELD(CPost_Contents, m_pPostList, 0x120);
	CHECK_FIELD(CPost_Contents, m_pTitleFont, 0x124);
END_LAYOUT_CHECK

// Letter message dialog; its size (0x138) is the `new` of
// CPost_MessgeDlgMakeInstance.
class CPost_MessgeDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // post_recivedlg.h:252

	CPost_MessgeDlg(void);
	virtual ~CPost_MessgeDlg(void);

	void OnPostMessage_ReciveUserInit(intptr_t param);       // post_recivedlg.cpp:1908
	void OnPostMessage_LetterContentsInit(intptr_t param);   // post_recivedlg.cpp:1917
	void OnPostMessage_LetterDateInit(intptr_t param);       // post_recivedlg.cpp:1926
	void OnPostDescFrame(bool bShow);   // post_recivedlg.cpp:1938
	void SetGiftDescString(const char* pSender, const char* pContents, const char* pItemName, const char* pItemDesc);   // post_recivedlg.cpp:1951

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // post_recivedlg.cpp:1813

	virtual void OnOK(void);   // post_recivedlg.cpp:1976

private:
	PADDING(0x110, 0x128);
	FrEdit* m_pReciveUserEdit;       // 0x128
	FrEdit* m_pLetterContentsEdit;   // 0x12C
	FrEdit* m_pLetterDateEdit;       // 0x130
	PADDING(0x134, 0x138);
};

CHECK_SIZE(CPost_MessgeDlg, 0x138);
BEGIN_LAYOUT_CHECK(CPost_MessgeDlg)
	CHECK_FIELD(CPost_MessgeDlg, m_pReciveUserEdit, 0x128);
	CHECK_FIELD(CPost_MessgeDlg, m_pLetterContentsEdit, 0x12C);
	CHECK_FIELD(CPost_MessgeDlg, m_pLetterDateEdit, 0x130);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_POST_RECIVEDLG_H
