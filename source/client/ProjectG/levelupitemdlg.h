#pragma once

#include "frform.h"
#include "../../shared/globalgamedefine.h"

class FrArea;
class FrEdit;
class FrListBox;
class WTitleFont;

class FrLevelupItemForm : public FrForm
{
public:
	DECLARE_OBJECT(FrLevelupItemForm)

	FrLevelupItemForm();
	virtual ~FrLevelupItemForm();

	struct MiniBtn
	{
		WRect rect;
		const Bitmap* pBitmap;
		int index;
	};

	virtual bool Close(eFormRet ret, bool bSound);
	bool SetData(sLevelUpDone data);

protected:
	virtual bool OnInit();
	virtual void OnProc(const float delta);

	void OnLevelUpItemInit(int param);
	void OnCloseUp();
	void OnLevelIcon(int param);
	void OnFrame2Init(int param);
	void OnFrame3Init(int param);
	void OnItemSendInit(int param);
	void OnItemListInit(int param);
	void OnItemListOwnerDraw(int param);
	void OnItemExplaneInit(int param);

	FrArea* m_pLevelIcon;
	FrArea* m_pFrame2;
	FrArea* m_pFrame3;
	FrEdit* m_pItemSend;
	FrListBox* m_pItemList;
	FrEdit* m_pItemExplane;
	WTitleFont* m_pFont;
	MiniBtn m_unusedBtn[3];
	MiniBtn m_deleteIcon;
	MiniBtn m_clockIcon;
	MiniBtn m_unusedBtn2[3];
	unsigned char m_level;
	unsigned char m_state;
	unsigned long m_itemTypeId;
	unsigned long m_itemTypeId2;
	bool m_bResult;

private:
	void SetItemInfo();

	DECLARE_FRESH_MSGMAP()
};
