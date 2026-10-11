// Original: d:\build\custom\temp\code\client\projectg\tikimagicboxdlg.h
#ifndef PG_CLIENT_PROJECTG_TIKIMAGICBOXDLG_H
#define PG_CLIENT_PROJECTG_TIKIMAGICBOXDLG_H

#include <list>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;
class FrListBox;
struct sTikiMagicBoxMtr;

// Tiki's magic box (item mixing) dialog; its size (0x150) is the `new` of
// FrTikiMagicBoxDlgMakeInstance.
class FrTikiMagicBoxDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // tikimagicboxdlg.h:10

	FrTikiMagicBoxDlg(void);
	virtual ~FrTikiMagicBoxDlg(void);

	int InsertMaterial(const sTikiMagicBoxMtr& mtr);
	int DeleteMaterial(const sTikiMagicBoxMtr& mtr);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnBackImageOwnerDraw(int param);
	void OnMyItemListInit(intptr_t param);
	void OnMyItemListOwnerDraw(int param);
	void OnMyItemListDBClick(void);
	void OnMyItemListLBDown(void);
	void OnMyItemListLBUp(void);
	void OnMtrListInit(int param);
	void OnMtrListOwnerDraw(int param);
	void OnMtrListDBClick(void);
	void OnMtrListLBDown(void);
	void OnMtrListLBUp(void);
	void OnCategoryListInit(int param);
	void OnCategoryListOwnerDraw(int param);
	void OnCategoryListLBDown(void);
	void OnCharacterListInit(int param);
	void OnCharacterListOwnerDraw(int param);
	void OnCharacterListLBDown(void);
	void OnHelpBtnInit(intptr_t param);
	void OnHelpBtnLBUp(void);
	void OnCloseBtnInit(intptr_t param);
	void OnCloseBtnLBUp(void);
	void OnCharTabInit(int param);
	void OnCharTabLBUp(void);
	void OnItemTabInit(int param);
	void OnItemTabLBUp(void);
	void OnInsertBtnInit(int param);
	void OnInsertBtnLBUp(void);
	void OnMixBtnInit(int param);
	void OnMixBtnLBUp(void);
	void OnCancelInit(intptr_t param);
	void OnCancelLBUp(void);

	void BindButton(FrButton*& pButton, int param);
	void BindListBox(FrListBox*& pListBox, int param);
	void DrawBitmapOnDlg(float x, float y, const char* name);
	void PrintTextOnDlg(float x, float y, const char* text, unsigned long color);

	int m_mtrIndex;                    // 0x110, zero-based slot the "insert the Nth material" hint shows
	std::list<FrWnd*> m_bindList;      // 0x114, controls bound by BindButton/BindListBox
	FrListBox* m_pMyItemList;          // 0x120
	FrListBox* m_pMtrList;             // 0x124
	FrListBox* m_pCategoryList;        // 0x128
	FrListBox* m_pCharacterList;       // 0x12c
	FrButton* m_pHelpBtn;              // 0x130
	FrButton* m_pCloseBtn;             // 0x134
	FrButton* m_pCharTab;              // 0x138
	FrButton* m_pItemTab;              // 0x13c
	FrButton* m_pInsertBtn;            // 0x140
	FrButton* m_pMixBtn;               // 0x144
	FrButton* m_pCancel;               // 0x148
	UNKNOWN(0x14c, 0x150);
};

CHECK_SIZE(FrTikiMagicBoxDlg, 0x150);
BEGIN_LAYOUT_CHECK(FrTikiMagicBoxDlg)
	CHECK_FIELD(FrTikiMagicBoxDlg, m_mtrIndex, 0x110);
	CHECK_FIELD(FrTikiMagicBoxDlg, m_bindList, 0x114);
	CHECK_FIELD(FrTikiMagicBoxDlg, m_pMyItemList, 0x120);
	CHECK_FIELD(FrTikiMagicBoxDlg, m_pMtrList, 0x124);
	CHECK_FIELD(FrTikiMagicBoxDlg, m_pCategoryList, 0x128);
	CHECK_FIELD(FrTikiMagicBoxDlg, m_pCharacterList, 0x12c);
	CHECK_FIELD(FrTikiMagicBoxDlg, m_pHelpBtn, 0x130);
	CHECK_FIELD(FrTikiMagicBoxDlg, m_pCloseBtn, 0x134);
	CHECK_FIELD(FrTikiMagicBoxDlg, m_pCharTab, 0x138);
	CHECK_FIELD(FrTikiMagicBoxDlg, m_pItemTab, 0x13c);
	CHECK_FIELD(FrTikiMagicBoxDlg, m_pInsertBtn, 0x140);
	CHECK_FIELD(FrTikiMagicBoxDlg, m_pMixBtn, 0x144);
	CHECK_FIELD(FrTikiMagicBoxDlg, m_pCancel, 0x148);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_TIKIMAGICBOXDLG_H
