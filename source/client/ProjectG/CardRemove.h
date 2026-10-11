// Original: d:\build\custom\temp\code\client\projectg\cardremove.h
#ifndef PG_CLIENT_PROJECTG_CARDREMOVE_H
#define PG_CLIENT_PROJECTG_CARDREMOVE_H

#include <list>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class FrArea;
class FrListBox;

// "Card remover" form of My Room: lists the parts that carry cards and,
// once one is chosen, uses a card remover item on it. Its size (0x138) is
// the `new` of FrCardRemoveMakeInstance.
class FrCardRemove : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // cardremove.h:24

	FrCardRemove(void);
	virtual ~FrCardRemove(void);

	static bool IsCardRemoveItem(unsigned long itemTypeId);                    // cardremove.cpp:169
	void SetCardRemoveItem(unsigned long itemTypeId, unsigned long itemId);   // cardremove.cpp:314

protected:
	void DrawInDlg(const char* bitmap, float x, float y);
	void PrintInDlg(const char* text, float x, float y);
	void DrawTexture(const char* bitmap, float x, float y, unsigned long diffuse);
	void PrintText(const char* text, float x, float y, unsigned long color);
	bool CheckEquiped(unsigned long itemId);
	bool SetCurPartUid(unsigned long partUid);
	void BuildCurPartCardList(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnDrawAreaInit(intptr_t param);
	void OnDrawAreaOwnerDraw(int param);
	void OnItemListInit(intptr_t param);
	void OnItemListOwnerDraw(intptr_t param);
	void OnItemListDBClick(void);
	void OnItemListRBDown(void);
	void OnConfirmInit(intptr_t param);
	void OnConfirmLBDown(void);
	bool OnUseCardRemoveConfirm(int result, FrForm* pForm);
	void OnCancelInit(intptr_t param);
	void OnCancelLBDown(void);

	FrArea* m_pDrawArea;                          // 0x110, "draw_area"
	FrListBox* m_pItemList;                       // 0x114, "item_list"
	unsigned long m_cardRemoveItemTypeId;         // 0x118, card remover item in use (SetCardRemoveItem)
	unsigned long m_cardRemoveItemId;             // 0x11C
	unsigned long m_curPartTypeId;                // 0x120, part chosen in the list
	unsigned long m_curPartUid;                   // 0x124
	std::list<unsigned long> m_curPartCardList;   // 0x128, cards of the chosen part (BuildCurPartCardList)
	UNKNOWN(0x134, 0x138);
};

CHECK_SIZE(FrCardRemove, 0x138);
BEGIN_LAYOUT_CHECK(FrCardRemove)
	CHECK_FIELD(FrCardRemove, m_pDrawArea, 0x110);
	CHECK_FIELD(FrCardRemove, m_pItemList, 0x114);
	CHECK_FIELD(FrCardRemove, m_cardRemoveItemTypeId, 0x118);
	CHECK_FIELD(FrCardRemove, m_cardRemoveItemId, 0x11C);
	CHECK_FIELD(FrCardRemove, m_curPartTypeId, 0x120);
	CHECK_FIELD(FrCardRemove, m_curPartUid, 0x124);
	CHECK_FIELD(FrCardRemove, m_curPartCardList, 0x128);
END_LAYOUT_CHECK

// "Use the card remover?" confirmation (size 0x118, the `new` of
// FrCardRemoveConfirmMakeInstance).
class FrCardRemoveConfirm : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // cardremove.h:82

	FrCardRemoveConfirm(void);
	virtual ~FrCardRemoveConfirm(void);

	void SetPart(unsigned long partTypeId, unsigned long partId) {   // cardremove.h:104
		m_partTypeId = partTypeId;
		m_partId = partId;
	}

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnDrawAreaOwnerDraw(int param);
	void OnConfirmInit(intptr_t param);
	void OnConfirmLBDown(void);
	void OnCancelInit(intptr_t param);
	void OnCancelLBDown(void);

	unsigned long m_partTypeId;  // 0x110, part whose card is removed (SetPart)
	unsigned long m_partId;      // 0x114
};

CHECK_SIZE(FrCardRemoveConfirm, 0x118);
BEGIN_LAYOUT_CHECK(FrCardRemoveConfirm)
	CHECK_FIELD(FrCardRemoveConfirm, m_partTypeId, 0x110);
	CHECK_FIELD(FrCardRemoveConfirm, m_partId, 0x114);
END_LAYOUT_CHECK

// Result of a card removal: the removed cards slide out from under the
// remover's cover mask (size 0x150, the `new` of
// FrCardRemoveResultMakeInstance).
class FrCardRemoveResult : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	// The enumerators are not named yet; 0 and 3 show the result without
	// the slide animation.
	enum eRESULT {
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // cardremove.h:114

	FrCardRemoveResult(void);
	virtual ~FrCardRemoveResult(void);

	void SetRemover(unsigned long typeId) { m_removerTypeId = typeId; }   // cardremove.h:157
	void SetResult(eRESULT result) {                                      // cardremove.h:158
		m_result = result;
	}
	void PushList(unsigned long cardTypeId) {                             // cardremove.h:163
		m_cardList.push_back(cardTypeId);
	}

protected:
	virtual bool OnInit(void);
	virtual void OnProc(float dt);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnDrawAreaOwnerDraw(int param);
	void OnConfirmInit(intptr_t param);
	void OnConfirmLBDown(void);

	eRESULT m_result;                     // 0x110
	std::list<unsigned long> m_cardList;  // 0x114, removed cards (PushList)
	const Bitmap* m_pCardBitmap;          // 0x120, "<remover icon>_for_resultdlg"
	const Bitmap* m_pCoverMask;           // 0x124, "card_remover_covermask"
	float m_cardX;                        // 0x128
	float m_cardY;                        // 0x12C
	float m_cardWidth;                    // 0x130
	float m_cardHeight;                   // 0x134
	float m_maskX;                        // 0x138
	float m_maskY;                        // 0x13C
	float m_maskWidth;                    // 0x140
	float m_maskOffset;                   // 0x144, slides from 0 to 40
	int m_state;                          // 0x148, 0 grow the form, 1 slide, 2 done
	unsigned long m_removerTypeId;        // 0x14C (SetRemover)
};

CHECK_SIZE(FrCardRemoveResult, 0x150);
BEGIN_LAYOUT_CHECK(FrCardRemoveResult)
	CHECK_FIELD(FrCardRemoveResult, m_result, 0x110);
	CHECK_FIELD(FrCardRemoveResult, m_cardList, 0x114);
	CHECK_FIELD(FrCardRemoveResult, m_pCardBitmap, 0x120);
	CHECK_FIELD(FrCardRemoveResult, m_pCoverMask, 0x124);
	CHECK_FIELD(FrCardRemoveResult, m_cardX, 0x128);
	CHECK_FIELD(FrCardRemoveResult, m_cardY, 0x12C);
	CHECK_FIELD(FrCardRemoveResult, m_cardWidth, 0x130);
	CHECK_FIELD(FrCardRemoveResult, m_cardHeight, 0x134);
	CHECK_FIELD(FrCardRemoveResult, m_maskX, 0x138);
	CHECK_FIELD(FrCardRemoveResult, m_maskY, 0x13C);
	CHECK_FIELD(FrCardRemoveResult, m_maskWidth, 0x140);
	CHECK_FIELD(FrCardRemoveResult, m_maskOffset, 0x144);
	CHECK_FIELD(FrCardRemoveResult, m_state, 0x148);
	CHECK_FIELD(FrCardRemoveResult, m_removerTypeId, 0x14C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CARDREMOVE_H
