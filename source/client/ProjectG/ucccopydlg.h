// Original: d:\build\custom\temp\code\client\projectg\ucccopydlg.h
#ifndef PG_CLIENT_PROJECTG_UUCCOPYDLG_H
#define PG_CLIENT_PROJECTG_UUCCOPYDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class FrArea;
class FrButton;
class FrListBox;
struct sItemInfo;

// UCC (self-designed item) copy dialog; its size (0x138) is the `new` of
// FrUccCopyDlgMakeInstance. Only the members this unit defines so far are
// declared.
class FrUccCopyDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ucccopydlg.h:13

	FrUccCopyDlg(void);
	virtual ~FrUccCopyDlg(void);

	void Init(void);
	void SetItem(sItemInfo* pItem);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnCharIconInit(intptr_t param);
	void OnItemListInit(intptr_t param);
	void OnItemListOwnerDraw(intptr_t param);
	void OnItemListBtnDown(void);   // ucccopydlg.cpp:124
	void OnOkBtnInit(intptr_t param);
	void OnOkBtnUp(void);
	void OnCancelBtnInit(intptr_t param);
	void OnCancelBtnUp(void);       // ucccopydlg.cpp:163
	void OnEtcOwnerDraw(int param);
	bool OnUccCopyConfirmDlgResult(int ret, FrForm* pForm);
	void BuildItemList(void);

	const Bitmap* m_pItemBaseNormal;   // 0x110
	const Bitmap* m_pItemBaseOver;     // 0x114
	const Bitmap* m_pItemBaseDown;     // 0x118
	sItemInfo* m_pItem;                // 0x11c
	FrArea* m_pCharIcon;               // 0x120
	FrListBox* m_pItemList;            // 0x124
	FrButton* m_pOkBtn;                // 0x128
	FrArea* m_pCancelBtn;              // 0x12c
	FrForm* m_pConfirmDlg;             // 0x130
	PADDING(0x134, 0x138);             // FrUccCopyDlg members
};

CHECK_SIZE(FrUccCopyDlg, 0x138);
BEGIN_LAYOUT_CHECK(FrUccCopyDlg)
	CHECK_FIELD(FrUccCopyDlg, m_pItemBaseNormal, 0x110);
	CHECK_FIELD(FrUccCopyDlg, m_pItemBaseOver, 0x114);
	CHECK_FIELD(FrUccCopyDlg, m_pItemBaseDown, 0x118);
	CHECK_FIELD(FrUccCopyDlg, m_pItem, 0x11c);
	CHECK_FIELD(FrUccCopyDlg, m_pCharIcon, 0x120);
	CHECK_FIELD(FrUccCopyDlg, m_pItemList, 0x124);
	CHECK_FIELD(FrUccCopyDlg, m_pOkBtn, 0x128);
	CHECK_FIELD(FrUccCopyDlg, m_pCancelBtn, 0x12c);
	CHECK_FIELD(FrUccCopyDlg, m_pConfirmDlg, 0x130);
END_LAYOUT_CHECK

// Confirmation step of the UCC copy (size 0x128).
class FrUccCopyConfirmDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ucccopydlg.h:61

	FrUccCopyConfirmDlg(void);
	virtual ~FrUccCopyConfirmDlg(void);

	void Init(sItemInfo* pSrcItem, sItemInfo* pDestItem);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnIconInit(intptr_t param);
	void OnOkBtnInit(intptr_t param);
	void OnOkBtnUp(void);
	void OnCancelBtnInit(intptr_t param);
	void OnCancelBtnUp(void);
	void OnEtcOwnerDraw(int param);

	sItemInfo* m_pSrcItem;      // 0x110
	sItemInfo* m_pDestItem;     // 0x114
	FrArea* m_pIcon;            // 0x118
	FrButton* m_pOkBtn;         // 0x11c
	FrButton* m_pCancelBtn;     // 0x120
	PADDING(0x124, 0x128);      // FrUccCopyConfirmDlg members
};

CHECK_SIZE(FrUccCopyConfirmDlg, 0x128);
BEGIN_LAYOUT_CHECK(FrUccCopyConfirmDlg)
	CHECK_FIELD(FrUccCopyConfirmDlg, m_pSrcItem, 0x110);
	CHECK_FIELD(FrUccCopyConfirmDlg, m_pDestItem, 0x114);
	CHECK_FIELD(FrUccCopyConfirmDlg, m_pIcon, 0x118);
	CHECK_FIELD(FrUccCopyConfirmDlg, m_pOkBtn, 0x11c);
	CHECK_FIELD(FrUccCopyConfirmDlg, m_pCancelBtn, 0x120);
END_LAYOUT_CHECK

// Last step of the UCC copy, showing the result (size 0x128).
class FrUccCopyCompletedDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ucccopydlg.h:97

	FrUccCopyCompletedDlg(void);
	virtual ~FrUccCopyCompletedDlg(void);

	void Init(unsigned long typeId, const char* name);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnOkBtnInit(intptr_t param);
	void OnOkBtnUp(void);
	void OnEtcOwnerDraw(int param);

	unsigned long m_typeId;     // 0x110
	char m_name[10];            // 0x114
	FrButton* m_pOkBtn;         // 0x120
	PADDING(0x124, 0x128);      // FrUccCopyCompletedDlg members
};

CHECK_SIZE(FrUccCopyCompletedDlg, 0x128);
BEGIN_LAYOUT_CHECK(FrUccCopyCompletedDlg)
	CHECK_FIELD(FrUccCopyCompletedDlg, m_typeId, 0x110);
	CHECK_FIELD(FrUccCopyCompletedDlg, m_name, 0x114);
	CHECK_FIELD(FrUccCopyCompletedDlg, m_pOkBtn, 0x120);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_UUCCOPYDLG_H
