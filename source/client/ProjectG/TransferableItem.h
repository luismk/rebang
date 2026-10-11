// Original: d:\build\custom\temp\code\client\projectg\transferableitem.h
#ifndef PG_CLIENT_PROJECTG_TRANSFERABLEITEM_H
#define PG_CLIENT_PROJECTG_TRANSFERABLEITEM_H

#include <list>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "../shared/globalgamedefine.h"
#include "../shared/globalnetworkdefine.h"

// The items the player can hand over (trade, mail, personal shop) and the
// ones registered for the current transfer (the WSingleton<CTransferableItem>
// instance).
class CTransferableItem : public WSingleton<CTransferableItem> {
	LAYOUT_CHECK_FRIEND;
public:
	CTransferableItem(void);
	virtual ~CTransferableItem(void);

	void OnLoad(void);
	void OnDestory(void);
	void Process(float fElapsed);
	void Render(void);
	void Reset(void);
	std::list<sTradeItem> *GetList_TransferableItem(void);
	void ClearTransferableItemList(void);
	void SetTransferableItemList(void);
	void SetTransferableEtcItemList(void);
	bool GetTransferableItem(sItemInfo &itemInfo, sTradeItem &tradeItem, bool useCount);
	void SetListEquipParts(void);
	bool IsEquipParts(unsigned long typeId);
	bool IsUccEquipped(sItemInfo *pItemInfo);
	int GetNumEquipItem(unsigned long typeId);
	void Set_RegistrationListSize(int size);
	bool SetRegistrationItem(unsigned long guid);
	bool SetRegistrationItem(unsigned long guid, int count);
	bool IsTransferableItem(unsigned long guid, sTradeItem &tradeItem, int count);
	bool IsRegisterationItem(unsigned long guid, sTradeItem &tradeItem);
	bool IsRegisterationItem(unsigned long guid);
	std::list<sTradeItem> *GetList_RegistrationItem(void);
	void ResetRegistrationItemList(void);
	int Get_RegistrationListSize(void);
	bool DelRegistrationItem(unsigned long guid);
	std::list<sTradeItem>::iterator DelRegistrationItem_itor_return(unsigned long guid);
	void SetMailInfoPage(sMailBoxListForPage &page);
	void SetMailInfo(sMailInfo &info);
	sMailBoxListForPage *GetMailBoxPageList(void);
	sMailInfo *GetMailInfo(void);
	int GetNumTradingItem(unsigned long guid);
	bool IsEquipParts(unsigned long typeId, unsigned long partTypeId);
	bool IsCheckCardParts(unsigned long typeId, unsigned long cardTypeId);

protected:
	std::list<sTradeItem> m_listTransferableItem;    // 0x28
	std::list<unsigned long> m_listEquipParts;       // 0x34, type ids of the equipped parts
	std::list<sTradeItem> m_listRegistrationItem;    // 0x40
	int m_nRegistrationListSize;                     // 0x4C, most registered items
	sMailBoxListForPage m_mailBoxPageList;           // 0x50
	sMailInfo m_mailInfo;                            // 0x68
};

BEGIN_LAYOUT_CHECK(CTransferableItem)
	CHECK_SIZE(CTransferableItem, 0x3D0);
	CHECK_FIELD(CTransferableItem, m_listTransferableItem, 0x28);
	CHECK_FIELD(CTransferableItem, m_listEquipParts, 0x34);
	CHECK_FIELD(CTransferableItem, m_listRegistrationItem, 0x40);
	CHECK_FIELD(CTransferableItem, m_nRegistrationListSize, 0x4C);
	CHECK_FIELD(CTransferableItem, m_mailBoxPageList, 0x50);
	CHECK_FIELD(CTransferableItem, m_mailInfo, 0x68);
END_LAYOUT_CHECK

#endif
