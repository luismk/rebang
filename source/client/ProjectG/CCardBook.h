// Original: d:\build\custom\temp\code\client\projectg\ccardbook.h
#ifndef PG_CLIENT_PROJECTG_CCARDBOOK_H
#define PG_CLIENT_PROJECTG_CCARDBOOK_H

#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "../client/Fresh/include/frcmdtarget.h"
#include "../client/Wangreal/include/wtypes.h"
#include "../client/Wangreal/include/wfont.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Fresh/include/frbutton.h"
#include "../client/Fresh/include/frarea.h"
#include "../client/Fresh/include/frlistbox.h"

class Bitmap;
class FrButton;
class FrCardUseDlg;
class FrForm;
class FrItemAttachCardDlg;
class FrOpenCardPackDlg;
class FrSpCardUseDlg;
class FrGraphicInterface;
struct sCards;
struct IFF_ITEM_COMMON;
class WOverlay;

namespace IFF_STRUCT {
struct sCard;
}

// One card slot of the card book; 0x3c bytes (the stride of
// ICardBookPage::BoundSlotControl). Only the members the units built so far
// use are declared.
class CCardSlot : public FrCmdTarget {
public:
	CCardSlot(void);
	virtual ~CCardSlot(void);

	// ccardbook.h:41
	unsigned char IsPushed(void) const {
		return m_pushed;
	}

	// ccardbook.h:53
	void Show(bool show) {
		if (m_pButton != NULL) {
			if (m_pCardInfo != NULL && m_pCommonItem != NULL) {
				m_pButton->SetVisible(show);
			}
		}
	}

	// ccardbook.h:64
	WRect GetPosition(void) const {
		return m_rect;
	}

	sCards* GetCards(void);          // ccardbook.h:69

	// ccardbook.h:74
	IFF_STRUCT::sCard* GetCardInfo(void) {
		return m_pCardInfo;
	}

	void SetPosition(WRect& rect);
	void Select(unsigned char pushed);
	void SetCard(unsigned long typeId);
	void UseCard(void);
	bool BoundControl(FrButton* pButton);
	void Draw(FrGraphicInterface* pGraphic);

protected:
	bool OnCardPackDlgResult(int result, FrForm* pForm);         // ccardbook.cpp:175
	bool OnSpCardDlgResult(int result, FrForm* pForm);           // ccardbook.cpp:198
	bool OnCardDlgResult(int result, FrForm* pForm);             // ccardbook.cpp:217
	bool OnItemAttachCardDlgResult(int result, FrForm* pForm);   // ccardbook.cpp:253

private:
	unsigned char m_pushed;          // 0x4
	WRect m_rect;                    // 0x8
	FrButton* m_pButton;             // 0x18
	IFF_STRUCT::sCard* m_pCardInfo;  // 0x1c
	IFF_ITEM_COMMON* m_pCommonItem;  // 0x20, the card's common record (SetCard); its icon names the button images
	sCards* m_pCards;                // 0x24
	WOverlay* m_pCardOverlay;        // 0x28, the card picture "<name>.png" (Draw), released by the destructor
	FrOpenCardPackDlg* m_pOpenCardPackDlg;      // 0x2c, "cardpack_open" (UseCard of a card pack)
	FrCardUseDlg* m_pCardUseDlg;                // 0x30, "usecard" (UseCard of a card)
	FrItemAttachCardDlg* m_pItemAttachCardDlg;  // 0x34, "itemattachcarddlg" (OnCardDlgResult)
	FrSpCardUseDlg* m_pSpCardUseDlg;            // 0x38, "usecard" (UseCard of a special card)

	LAYOUT_CHECK_FRIEND;
};
BEGIN_LAYOUT_CHECK(CCardSlot)
	CHECK_FIELD(CCardSlot, m_pushed, 0x4);
	CHECK_FIELD(CCardSlot, m_rect, 0x8);
	CHECK_FIELD(CCardSlot, m_pButton, 0x18);
	CHECK_FIELD(CCardSlot, m_pCardInfo, 0x1c);
	CHECK_FIELD(CCardSlot, m_pCommonItem, 0x20);
	CHECK_FIELD(CCardSlot, m_pCards, 0x24);
	CHECK_FIELD(CCardSlot, m_pCardOverlay, 0x28);
	CHECK_FIELD(CCardSlot, m_pOpenCardPackDlg, 0x2c);
	CHECK_FIELD(CCardSlot, m_pCardUseDlg, 0x30);
	CHECK_FIELD(CCardSlot, m_pItemAttachCardDlg, 0x34);
	CHECK_FIELD(CCardSlot, m_pSpCardUseDlg, 0x38);
END_LAYOUT_CHECK
CHECK_SIZE(CCardSlot, 0x3C);

// Base of the card book's pages (card pack page, card page). Virtual order
// from ??_7CCardPage@@6B@.
class ICardBookPage {
public:
	// ccardbook.h:93
	ICardBookPage(void) {
		m_pCardSlots = NULL;
		m_pFont = NULL;

		static char* strBonusNumber = "[font_wind.jpg";

		tagWTITLEFONT info;
		memset(&info, 0, sizeof(info));
		info.filename = &strBonusNumber;
		info.fontw = 16;
		info.fonth = 16;
		info.numPages = 1;
		info.texw = 128;
		info.texh = 64;
		info.pCharSet = "1234567890ym%-.?/:";

		m_pFont = g_resrcmng->GetTitleFont();
		m_pFont->Create(&info);
	}

	// ccardbook.h:119
	virtual ~ICardBookPage(void) {
		if (m_pCardSlots != NULL) {
			delete[] m_pCardSlots;
			m_pCardSlots = NULL;
		}

		m_pFont->Erase();
		if (g_resrcmng != NULL && m_pFont != NULL) { g_resrcmng->Release(m_pFont); m_pFont = NULL; }
	}

	virtual bool Init(void) = 0;
	virtual void Draw(FrGraphicInterface* pGraphic) = 0;

	// ccardbook.h:148
	virtual void SetCard(unsigned char slot, unsigned long typeId) {
	}

	virtual void ShowSlotControl(bool show) = 0;
	virtual void SelectSlot(unsigned char control, unsigned char slot) = 0;
	virtual bool IsSelectedSlot(void) = 0;

	bool BoundSlotControl(unsigned char slot, FrButton* pButton);   // ccardbook.h:130
	CCardSlot* GetCardSlot(void);                                   // ccardbook.h:140

protected:
	CCardSlot* m_pCardSlots;         // 0x4
	WTitleFont* m_pFont;             // 0x8

	LAYOUT_CHECK_FRIEND;
};
BEGIN_LAYOUT_CHECK(ICardBookPage)
	CHECK_FIELD(ICardBookPage, m_pCardSlots, 0x4);
END_LAYOUT_CHECK

// The page listing the card packs.
class CCardPackPage : public ICardBookPage {
public:
	// ccardbook.h:162
	CCardPackPage(void) {
		m_pCardPackList = NULL;
	}

	// ccardbook.h:168
	virtual ~CCardPackPage(void) {
		if (m_pCardPackList != NULL) {
			delete m_pCardPackList;
			m_pCardPackList = NULL;
		}
	}

	virtual bool Init(void);
	virtual void Draw(FrGraphicInterface* pGraphic);
	virtual void SetCard(unsigned char slot, unsigned long typeId);
	virtual void ShowSlotControl(bool show);
	virtual void SelectSlot(unsigned char control, unsigned char slot);

	// ccardbook.h:181
	std::vector<unsigned long>* GetCardPackList(void) {
		return m_pCardPackList;
	}

	// ccardbook.h:190
	virtual bool IsSelectedSlot(void) {
		bool result = false;

		for (int i = 0; i < 8; i++) {
			if (m_pCardSlots != NULL) {
				if (m_pCardSlots[i].IsPushed() == 1) {
					result = true;
					break;
				}
			}
		}

		return result;
	}

private:
	std::vector<unsigned long>* m_pCardPackList;   // 0xc

	LAYOUT_CHECK_FRIEND;
};
BEGIN_LAYOUT_CHECK(CCardPackPage)
	CHECK_FIELD(CCardPackPage, m_pCardPackList, 0xc);
END_LAYOUT_CHECK

// The page listing the cards, one list per card type (1..4).
class CCardPage : public ICardBookPage {
public:
	CCardPage(void);
	virtual ~CCardPage(void);
	virtual bool Init(void);
	virtual void Draw(FrGraphicInterface* pGraphic);
	virtual void SetCard(unsigned char slot, unsigned long typeId);
	virtual void ShowSlotControl(bool show);
	virtual void SelectSlot(unsigned char control, unsigned char slot);

	// ccardbook.h:244
	virtual bool IsSelectedSlot(void) {
		bool result = false;

		for (int i = 0; i < 12; i++) {
			if (m_pCardSlots != NULL) {
				if (m_pCardSlots[i].IsPushed() == 1) {
					result = true;
					break;
				}
			}
		}

		return result;
	}

	// No out-of-line copy: its line in ccardbook.h is unknown.
	void SetCardType(unsigned char type) {
		m_cardType = type;
	}

	// No out-of-line copy: its line in ccardbook.h is unknown.
	void SetCurPage(unsigned char page) {
		m_curPage = page;
	}

	std::vector<unsigned long>* GetCardList(unsigned char type);
	unsigned long GetPageCount(unsigned char type);

protected:
	void PageSeparate(std::vector<unsigned long>*& rpList, unsigned char type);
	void BuildCardList(void);

private:
	unsigned char m_cardType;                     // 0xc
	unsigned char m_curPage;                      // 0xd, the page shown (CCardBook::BuildCardPage)
	std::vector<unsigned long>* m_pCardList1;     // 0x10 (card type 1)
	std::vector<unsigned long>* m_pCardList2;     // 0x14 (card type 2)
	std::vector<unsigned long>* m_pCardList3;     // 0x18 (card type 3)
	std::vector<unsigned long>* m_pCardList4;     // 0x1c (card type 4)
	const Bitmap* m_pSlotBackGround;              // 0x20, drawn behind each filled slot (Draw)

	LAYOUT_CHECK_FRIEND;
};
BEGIN_LAYOUT_CHECK(CCardPage)
	CHECK_FIELD(CCardPage, m_cardType, 0xc);
	CHECK_FIELD(CCardPage, m_curPage, 0xd);
	CHECK_FIELD(CCardPage, m_pCardList1, 0x10);
	CHECK_FIELD(CCardPage, m_pCardList4, 0x1c);
	CHECK_FIELD(CCardPage, m_pSlotBackGround, 0x20);
END_LAYOUT_CHECK

// The card book of the my-room screen. Only the members the units built so
// far use are declared (size unknown).
class CCardBook {
public:
	CCardBook(void);
	virtual ~CCardBook(void);

	void ShowSlotControl(bool show);
	void PageIndexBehavior(const WPoint& pt, unsigned char type);
	void ButtonTabControl(unsigned char tab);
	void UseCard(void);
	void BuildCardPackTab(void);
	void BuildCardPage(unsigned long page);
	void BuildCardTab(unsigned long tab);
	void ReBuildCurrentPage(void);
	void CardSlotControl(unsigned char page, unsigned char slot, unsigned char control);
	void Draw(void);
	void Show(bool show);
	void DrawCardPackHelp(unsigned long typeId);

	// Stores pControl in rControl and hides it; true when it was found.
	template <class T>
	bool BoundControl(T& rControl, T pControl) {    // ccardbook.h:313
		rControl = pControl;

		pControl->SetVisible(false);

		return rControl ? true : false;
	}

	CCardPackPage* GetCardPackPage(void);           // ccardbook.h:324
	CCardPage* GetCardPage(void);                   // ccardbook.h:329
	void SetSelectedSlot(unsigned char slot);       // ccardbook.h:334
	bool IsShow(void) const;                        // ccardbook.h:350

protected:
	void ClearVariables(void);
	void LoadImages(void);
	void ShowCardProperty(bool show);
	void DrawBackGround(FrGraphicInterface* pGraphic);
	void DrawCardPack(FrGraphicInterface* pGraphic);
	void DrawCard(FrGraphicInterface* pGraphic);
	void DrawProperty(FrGraphicInterface* pGraphic);
	void DrawPageIndex(FrGraphicInterface* pGraphic);

private:
	bool m_show;                     // 0x4
	bool m_showProperty;             // 0x5
	unsigned char m_tab;             // 0x6, the selected tab (ButtonTabControl): 0 card packs, 1-3 card volumes
	unsigned char m_pageCount;       // 0x7, the pages of the current card list (BuildCardTab)
	unsigned char m_curPage;         // 0x8, the page shown (BuildCardPage)
	unsigned char m_hoverPage;       // 0x9, the page index under the cursor (PageIndexBehavior)
	unsigned char m_selectedSlot;    // 0xa
	UNKNOWN(0xB, 0xC);
	// Element type unknown: only the constructor, the destructor and
	// ClearVariables touch it.
	std::vector<unsigned long> m_reservedList;     // 0xc
	std::vector<unsigned long>* m_pCurCardList;    // 0x1c, the card list of the current tab (BuildCardTab)
	std::vector<WRect> m_pageIndexRects;           // 0x20, one rectangle per page index (BuildCardTab)
	// The two pages, indexed by page (0 card pack, 1 card).
	union {
		struct {
			CCardPackPage* m_pCardPackPage;  // 0x30
			CCardPage* m_pCardPage;          // 0x34
		};
		ICardBookPage* m_pPages[2];          // 0x30
	};
	const Bitmap* m_pBackGround[4];  // 0x38, "cardbook_bg1".."4"
	const Bitmap* m_pCardPack[4];    // 0x48, "cardpack1".."4"
	const Bitmap* m_pCollection[4];  // 0x58, "collection1".."4"

public:
	// Bound by CRealMyRoomMain's OnCard*Init handlers (BoundControl).
	FrArea* m_pBookWnd;              // 0x68, the "card book" area, shown with the book (Show; OnCardBookInit)
	FrArea* m_pPropertyWnd;          // 0x6c, the card property area (Show; OnCardPropertyInit)
	FrButton* m_pTabButtons[5];      // 0x70, the tabs: card packs, volumes 1-4 (OnTabCard*Init; ButtonTabControl)
	FrButton* m_pPageWnd;            // 0x84, the "use card" button (OnCardUseInit)
	FrListBox* m_pSubWnd;            // 0x88, the card book index list, shown and hidden with the book (Show; OnCardBookIndexInit)

private:

	LAYOUT_CHECK_FRIEND;
};
BEGIN_LAYOUT_CHECK(CCardBook)
	CHECK_FIELD(CCardBook, m_show, 0x4);
	CHECK_FIELD(CCardBook, m_showProperty, 0x5);
	CHECK_FIELD(CCardBook, m_tab, 0x6);
	CHECK_FIELD(CCardBook, m_pageCount, 0x7);
	CHECK_FIELD(CCardBook, m_curPage, 0x8);
	CHECK_FIELD(CCardBook, m_hoverPage, 0x9);
	CHECK_FIELD(CCardBook, m_selectedSlot, 0xA);
	CHECK_FIELD(CCardBook, m_reservedList, 0xC);
	CHECK_FIELD(CCardBook, m_pCurCardList, 0x1C);
	CHECK_FIELD(CCardBook, m_pageIndexRects, 0x20);
	CHECK_FIELD(CCardBook, m_pCardPackPage, 0x30);
	CHECK_FIELD(CCardBook, m_pCardPage, 0x34);
	CHECK_FIELD(CCardBook, m_pBackGround, 0x38);
	CHECK_FIELD(CCardBook, m_pCardPack, 0x48);
	CHECK_FIELD(CCardBook, m_pCollection, 0x58);
	CHECK_FIELD(CCardBook, m_pBookWnd, 0x68);
	CHECK_FIELD(CCardBook, m_pPropertyWnd, 0x6C);
	CHECK_FIELD(CCardBook, m_pTabButtons, 0x70);
	CHECK_FIELD(CCardBook, m_pPageWnd, 0x84);
	CHECK_FIELD(CCardBook, m_pSubWnd, 0x88);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CCARDBOOK_H
