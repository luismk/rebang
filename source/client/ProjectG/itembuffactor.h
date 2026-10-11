// Original: d:\build\custom\temp\code\client\projectg\itembuffactor.h
#ifndef PG_CLIENT_PROJECTG_ITEMBUFFACTOR_H
#define PG_CLIENT_PROJECTG_ITEMBUFFACTOR_H

#include <list>

#include "..\..\defines.h"
#include "actor.h"
#include "frcmdtarget.h"
#include "../client/Wangreal/include/wtypes.h"

class Bitmap;
class FrForm;
class FrSPCardBuffDlg;
class MsgObject;
class WPoint;
class WReceivedPacket;
struct FrListItem;
struct sItemInfo;
struct sSCardAvilityPeriodInfo;

// The buff item actor: keeps the list of the active buff (special card)
// items, draws the "use" mini button of the buff items in the item lists and
// uses the item on a click. Its size (0x38) is the `new` of
// CItemBuffMakeInstance: an IActor with a Fresh command target at 0x18.
class CItemBuff : public IActor, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

protected:
	CItemBuff(void);
	friend IObject* CItemBuffMakeInstance(void);   // the factory creates it

public:
	virtual void OnLoad(void);
	virtual void HandleMsg(const MsgObject& msg);

	static void UpdateBuffDlg(FrSPCardBuffDlg* dlg);
	static void ResetBuffList(void);
	static void RemoveBuffItem(unsigned long typeId);
	static void AddBuffItem(sSCardAvilityPeriodInfo& info);
	static void ProcessPacket(WReceivedPacket& packet);

protected:
	bool OnUseBuffItemDlgResult(int ret, FrForm* pForm);
	void DrawMiniButton(FrListItem* pItem, const WPoint& pt);
	void IsCompatibleBuff(unsigned long typeId, const std::list<sSCardAvilityPeriodInfo>& buffList, bool& compatible, unsigned long& replaceTypeId);
	void UseBuffItem(sItemInfo* pItem);
	void ButtonDown(FrListItem* pItem, const WPoint& pt);
	void HandleBuffItemSubMsg(const MsgObject& msg);

private:
	static std::list<sSCardAvilityPeriodInfo> ms_BuffItemInfoList;

	const Bitmap* m_pButtonNormal;   // 0x1c, "trade_item_edit_bn_normal"
	const Bitmap* m_pButtonOver;     // 0x20, "trade_item_edit_bn_over"
	WRect m_buttonRect;              // 0x24, the mini button, relative to the list item
	unsigned long m_useItemTypeId;   // 0x34, the item UseBuffItem asks to use
};

CHECK_SIZE(CItemBuff, 0x38);
BEGIN_LAYOUT_CHECK(CItemBuff)
	CHECK_FIELD(CItemBuff, m_pButtonNormal, 0x1c);
	CHECK_FIELD(CItemBuff, m_pButtonOver, 0x20);
	CHECK_FIELD(CItemBuff, m_buttonRect, 0x24);
	CHECK_FIELD(CItemBuff, m_useItemTypeId, 0x34);
END_LAYOUT_CHECK

#endif
