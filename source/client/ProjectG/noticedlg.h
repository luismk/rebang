// Original: d:\build\custom\temp\code\client\projectg\noticedlg.h
#ifndef PG_CLIENT_PROJECTG_NOTICEDLG_H
#define PG_CLIENT_PROJECTG_NOTICEDLG_H

#include <vector>

#include "..\..\defines.h"
#include "frform.h"
#include "NoticeImage.h"

class FrButton;
class FrListBox;
class FrViewer;

// Notice dialog: pages through the images of a notice file in a viewer, with
// prev/next buttons and a page-number list box. Size 0x148 (the `new` of
// FrNoticeDlgMakeInstance).
class FrNoticeDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // noticedlg.h:19

	FrNoticeDlg(void);
	virtual ~FrNoticeDlg(void);

	unsigned long GetTypeId(void) const;   // noticedlg.h:29

	void LoadOnly(const char* fileName);
	void LoadInitFile(const char* fileName);
	void Prev(void);
	void Next(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);

	void OnPrevInit(intptr_t wnd);
	void OnNextInit(intptr_t wnd);
	void SetScrollBar(void);
	void OnCancelInit(intptr_t wnd);
	void OnViewInit(intptr_t wnd);
	void OnNumInit(intptr_t wnd);
	void OnNumOwnerDraw(intptr_t param);
	void OnViewBtnDown(void);
	void OpenImage(void);
	void OnNumLBtnUp(void);
	void OnPrevBtnUp(void);
	void OnNextBtnUp(void);

private:
	FrButton* m_pCancel;                   // 0x110 ("cancel")
	FrViewer* m_pView;                     // 0x114 ("view")
	std::vector<sNoticeImage> m_images;    // 0x118
	unsigned char m_page;                  // 0x128, index of the shown image
	FrButton* m_pPrev;                     // 0x12c ("prev")
	FrButton* m_pNext;                     // 0x130 ("next")
	FrListBox* m_pNum;                     // 0x134 ("num")
	int m_openedPage;                      // 0x138, page OpenImage last opened
	bool m_bScroll;                        // 0x13c, the image is taller than the viewer
	unsigned long m_typeId;                // 0x140
	int m_startPage;                       // 0x144, -1: none
};
CHECK_SIZE(FrNoticeDlg, 0x148);
BEGIN_LAYOUT_CHECK(FrNoticeDlg)
	CHECK_FIELD(FrNoticeDlg, m_pCancel, 0x110);
	CHECK_FIELD(FrNoticeDlg, m_pView, 0x114);
	CHECK_FIELD(FrNoticeDlg, m_images, 0x118);
	CHECK_FIELD(FrNoticeDlg, m_page, 0x128);
	CHECK_FIELD(FrNoticeDlg, m_pPrev, 0x12c);
	CHECK_FIELD(FrNoticeDlg, m_pNext, 0x130);
	CHECK_FIELD(FrNoticeDlg, m_pNum, 0x134);
	CHECK_FIELD(FrNoticeDlg, m_openedPage, 0x138);
	CHECK_FIELD(FrNoticeDlg, m_bScroll, 0x13c);
	CHECK_FIELD(FrNoticeDlg, m_typeId, 0x140);
	CHECK_FIELD(FrNoticeDlg, m_startPage, 0x144);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_NOTICEDLG_H
