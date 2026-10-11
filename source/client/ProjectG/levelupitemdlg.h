// Original: d:\build\custom\temp\code\client\projectg\levelupitemdlg.h
#ifndef PG_CLIENT_PROJECTG_LEVELUPITEMDLG_H
#define PG_CLIENT_PROJECTG_LEVELUPITEMDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "../shared/globalgamedefine.h"

class Bitmap;
class FrArea;
class FrEdit;
class FrListBox;
class WTitleFont;

// Level-up reward dialog; its size (0x200) is the `new` of
// FrLevelupItemFormMakeInstance.
class FrLevelupItemForm : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	// A small icon drawn over a reward item (OnItemListInit).
	class MiniBtn {
	public:
		MiniBtn(void);

		WRect rect;               // 0x0
		const Bitmap* pBitmap;    // 0x10
		UNKNOWN(0x14, 0x18);
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // levelupitemdlg.h:219

	FrLevelupItemForm(void);
	virtual ~FrLevelupItemForm(void);

	virtual bool Close(eFormRet ret, bool bDestroy);
	bool SetData(sLevelUpDone data);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	void OnLevelUpItemInit(int);
	void OnCloseUp(void);
	void OnLevelIcon(intptr_t param);
	void OnFrame2Init(intptr_t param);
	void OnFrame3Init(intptr_t param);
	void OnItemSendInit(intptr_t param);
	void OnItemListInit(intptr_t param);
	void OnItemListOwnerDraw(intptr_t param);
	void OnItemExplaneInit(intptr_t param);

private:
	void SetItemInfo(void);

protected:
	FrArea* m_pLevelIcon;           // 0x110, "level"
	FrArea* m_pFrame2;              // 0x114, "frame2"
	FrArea* m_pFrame3;              // 0x118, "frame3"
	FrEdit* m_pItemSend;            // 0x11c, "itemsend"
	FrListBox* m_pItemList;         // 0x120, "leveluplist"
	FrEdit* m_pItemExplane;         // 0x124, "itemexplane"
	WTitleFont* m_pTitleFont;       // 0x128
	PADDING(0x12c, 0x174);          // FrLevelupItemForm members
	MiniBtn m_deleteBtn;            // 0x174, "delete" icon of the reward items
	MiniBtn m_clockBtn;             // 0x18c, "clock" icon of the reward items
	PADDING(0x1a4, 0x1ec);          // FrLevelupItemForm members
	unsigned char m_level;          // 0x1ec
	unsigned char m_rewardReceived; // 0x1ed
	PADDING(0x1ee, 0x1f0);
	unsigned long m_rewardTypeId;   // 0x1f0, item data of the reward list
	unsigned long m_bonusTypeId;    // 0x1f4, second reward item (level 1)
	bool m_hasReward;               // 0x1f8
	PADDING(0x1f9, 0x200);          // FrLevelupItemForm members
};

CHECK_SIZE(FrLevelupItemForm::MiniBtn, 0x18);
CHECK_FIELD(FrLevelupItemForm::MiniBtn, rect, 0x0);
CHECK_FIELD(FrLevelupItemForm::MiniBtn, pBitmap, 0x10);
CHECK_SIZE(FrLevelupItemForm, 0x200);
BEGIN_LAYOUT_CHECK(FrLevelupItemForm)
	CHECK_FIELD(FrLevelupItemForm, m_pLevelIcon, 0x110);
	CHECK_FIELD(FrLevelupItemForm, m_pFrame2, 0x114);
	CHECK_FIELD(FrLevelupItemForm, m_pFrame3, 0x118);
	CHECK_FIELD(FrLevelupItemForm, m_pItemSend, 0x11c);
	CHECK_FIELD(FrLevelupItemForm, m_pItemList, 0x120);
	CHECK_FIELD(FrLevelupItemForm, m_pItemExplane, 0x124);
	CHECK_FIELD(FrLevelupItemForm, m_pTitleFont, 0x128);
	CHECK_FIELD(FrLevelupItemForm, m_deleteBtn, 0x174);
	CHECK_FIELD(FrLevelupItemForm, m_clockBtn, 0x18c);
	CHECK_FIELD(FrLevelupItemForm, m_level, 0x1ec);
	CHECK_FIELD(FrLevelupItemForm, m_rewardReceived, 0x1ed);
	CHECK_FIELD(FrLevelupItemForm, m_rewardTypeId, 0x1f0);
	CHECK_FIELD(FrLevelupItemForm, m_bonusTypeId, 0x1f4);
	CHECK_FIELD(FrLevelupItemForm, m_hasReward, 0x1f8);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_LEVELUPITEMDLG_H
