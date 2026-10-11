// Original: d:\build\custom\temp\code\client\projectg\guidebook.h
#ifndef PG_CLIENT_PROJECTG_GUIDEBOOK_H
#define PG_CLIENT_PROJECTG_GUIDEBOOK_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "actor.h"

class FrArea;
class FrButton;
class FrTextButton;
class MsgObject;
class WOverlay;

// The guide book dialog ("guide_book"): nine chapter tabs, previous/next
// page buttons and the page image "<chapter>_<page>.tga" drawn into the
// "content" area. Its size (0x150) is the `new` of FrGuideBookDlgMakeInstance.
class FrGuideBookDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // guidebook.h:20

	FrGuideBookDlg(void);
	virtual ~FrGuideBookDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OpenChapter(int chapter);

	void OnContentInit(intptr_t param);
	void OnContentOwnerDraw(int param);
	void OnPrevInit(intptr_t param);
	void OnPrevBtnUp(void);
	void OnNextInit(intptr_t param);
	void OnNextBtnUp(void);
	void OnTip1Init(intptr_t param);
	void OnTip2Init(intptr_t param);
	void OnTip3Init(intptr_t param);
	void OnTip4Init(intptr_t param);
	void OnTip5Init(intptr_t param);
	void OnTip6Init(intptr_t param);
	void OnTip7Init(intptr_t param);
	void OnTip8Init(intptr_t param);
	void OnTip9Init(intptr_t param);
	void OnTip1BtnUp(void);
	void OnTip2BtnUp(void);
	void OnTip3BtnUp(void);
	void OnTip4BtnUp(void);
	void OnTip5BtnUp(void);
	void OnTip6BtnUp(void);
	void OnTip7BtnUp(void);
	void OnTip8BtnUp(void);
	void OnTip9BtnUp(void);

private:
	int m_chapter;              // 0x110 (1-9, the selected tip)
	int m_page;                 // 0x114 (1-based page of the chapter)
	FrArea* m_pContent;         // 0x118 ("content")
	FrButton* m_pPrev;          // 0x11c ("prev")
	FrButton* m_pNext;          // 0x120 ("next")
	FrTextButton* m_pTip[9];    // 0x124 ("tip1".."tip9")
	WOverlay* m_pPage;          // 0x148 (image of the current page)
	PADDING(0x14c, 0x150);
};

CHECK_SIZE(FrGuideBookDlg, 0x150);
BEGIN_LAYOUT_CHECK(FrGuideBookDlg)
	CHECK_FIELD(FrGuideBookDlg, m_chapter, 0x110);
	CHECK_FIELD(FrGuideBookDlg, m_page, 0x114);
	CHECK_FIELD(FrGuideBookDlg, m_pContent, 0x118);
	CHECK_FIELD(FrGuideBookDlg, m_pPrev, 0x11c);
	CHECK_FIELD(FrGuideBookDlg, m_pNext, 0x120);
	CHECK_FIELD(FrGuideBookDlg, m_pTip, 0x124);
	CHECK_FIELD(FrGuideBookDlg, m_pPage, 0x148);
END_LAYOUT_CHECK

// The actor that opens the guide book dialog on message 0x12a, closes it on
// 0x12b and reports whether it is open on 0x12c (size 0x20, the `new` of
// CGuideBookMakeInstance): an IActor with a Fresh command target at 0x18.
class CGuideBook : public IActor, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // guidebook.h:75

protected:
	CGuideBook(void);
	friend IObject* CGuideBookMakeInstance(void);   // the factory creates it

public:
	virtual ~CGuideBook(void) {}   // guidebook.h:79

	virtual void HandleMsg(const MsgObject& msg);

protected:
	bool OnDlgResult(int ret, FrForm* pForm);

private:
	FrGuideBookDlg* m_pDlg;   // 0x1c
};

CHECK_SIZE(CGuideBook, 0x20);
BEGIN_LAYOUT_CHECK(CGuideBook)
	CHECK_FIELD(CGuideBook, m_pDlg, 0x1c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GUIDEBOOK_H
