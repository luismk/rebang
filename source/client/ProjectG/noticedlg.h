#pragma once

#include <vector>
#include "frform.h"
#include "noticeimage.h"

class FrButton;
class FrViewer;
class FrListBox;

class FrNoticeDlg : public FrForm
{
	DECLARE_OBJECT(FrNoticeDlg)

	FrNoticeDlg();
	virtual ~FrNoticeDlg();

	void LoadInitFile(const char* filename);
	void LoadOnly(const char* filename);
	void Prev();
	void Next();

	unsigned long GetTypeId() const { return m_typeId; }

protected:
	virtual bool OnInit();

	void OnCancelInit(int param);
	void OnViewInit(int param);
	void OnViewBtnDown();
	void OnNumInit(int param);
	void OnNumLBtnUp();
	void OnNumOwnerDraw(int param);
	void OnPrevInit(int param);
	void OnPrevBtnUp();
	void OnNextInit(int param);
	void OnNextBtnUp();

	void SetScrollBar();
	void OpenImage();

	FrButton* m_pCancel;
	FrViewer* m_pViewer;
	std::vector<sNoticeImage> m_images;
	unsigned char m_curIndex;
	FrButton* m_pPrev;
	FrButton* m_pNext;
	FrListBox* m_pNumList;
	int m_openedIndex;
	bool m_bScrollBar;
	unsigned long m_typeId;
	int m_startIndex;

	DECLARE_FRESH_MSGMAP()
};
