// Original: d:\build\custom\temp\code\client\projectg\transferableitem.cpp
#include "TransferableItem.h"

#include <string.h>

#include "SharedDoc.h"
#include "UccLibrary.h"
#include "UccManager.h"

// transferableitem.cpp:28
CTransferableItem::CTransferableItem(void) {
}

// transferableitem.cpp:33
CTransferableItem::~CTransferableItem(void) {
}

// transferableitem.cpp:39
bool CompareTradeItem(const sTradeItem &left, const sTradeItem &right) {
	return left.typeId < right.typeId;
}

// transferableitem.cpp:45
void CTransferableItem::OnLoad(void) {
	Reset();
}

// transferableitem.cpp:52
void CTransferableItem::OnDestory(void) {
	m_listTransferableItem.clear();
	m_listRegistrationItem.clear();
	m_listEquipParts.clear();

	m_nRegistrationListSize = 0;
}

// transferableitem.cpp:61
void CTransferableItem::Process(float fElapsed) {
}

// transferableitem.cpp:66
void CTransferableItem::Render() {
}

// transferableitem.cpp:71
void CTransferableItem::Reset(void) {
	m_listTransferableItem.clear();
	m_listRegistrationItem.clear();
	m_listEquipParts.clear();

	m_nRegistrationListSize = 0;
}

// transferableitem.cpp:80
std::list<sTradeItem> *CTransferableItem::GetList_TransferableItem(void) {
	return &m_listTransferableItem;
}

// transferableitem.cpp:86
void CTransferableItem::ClearTransferableItemList(void) {
	m_listTransferableItem.clear();
}

// transferableitem.cpp:91
void CTransferableItem::SetTransferableItemList(void) {
	m_listTransferableItem.clear();

	sTradeItem tradeItem;

	// Parts.
	std::list<sItemInfo>::iterator itPart;

	std::list<sItemInfo> &partsList = CSharedDoc::Instance()->m_myPartsList;
	for (itPart = partsList.begin(); itPart != partsList.end(); ++itPart) {
		IFF_ITEM_COMMON *pCommon = ItemManager()->FindCommonItem(itPart->m_typeId);

		if (pCommon == NULL)
			continue;

		unsigned char tradeKind = (pCommon->m_flags0 >> 1) & 0x0F;
		if (tradeKind == 1 || tradeKind == 3) {
			tradeItem.guid = itPart->m_serial;

			tradeItem.typeId = itPart->m_typeId;
			tradeItem.count = 1 - GetNumTradingItem(itPart->m_serial);
			tradeItem.price = 0;

			if (IsLocalContent((localContentType_t)0x51)) {
				IFF_STRUCT::sPart *pPart = ItemManager()->FindPart(itPart->m_typeId);
				if (pPart != NULL) {
					// TODO: sTradeItem card slots (+0x67/+0x69 = pPart->m_charCardSlots /
					// m_caddieCardSlots), the card type ids (unsigned long[12] at +0x37,
					// cleared) filled from m_cardPeriodList[0] (lines 123-136):
					// globalnetworkdefine.h needs those members.
				}
			}

			m_listTransferableItem.push_back(tradeItem);
		}
	}

	if (IsLocalContent((localContentType_t)0x57)) {
		std::list<sItemInfo>::iterator itItem;

		std::list<sItemInfo> &itemList = CSharedDoc::Instance()->m_myItemList;
		for (itItem = itemList.begin(); itItem != itemList.end(); ++itItem) {
			IFF_ITEM_COMMON *pCommon = ItemManager()->FindCommonItem(itItem->m_typeId);

			if (pCommon == NULL)
				continue;

			unsigned char tradeKind = (pCommon->m_flags0 >> 1) & 0x0F;
			if (tradeKind == 1 || tradeKind == 3) {
				// A part worn by a character cannot be traded.
				bool bEquipped = false;
				std::map<unsigned int, sCharacterInfo>::iterator itChar;

				for (itChar = CSharedDoc::Instance()->m_characterMap.begin(); itChar != CSharedDoc::Instance()->m_characterMap.end(); ++itChar) {
					if (itChar->second.typeId == (((itItem->m_typeId >> 18) & 0xFF) | 0x4000000)) {
						int slot = (itItem->m_typeId >> 13) & 0x1F;

						if (itChar->second.partIds[slot] == itItem->m_serial) {
							bEquipped = true;
							break;
						}
					}
				}

				if (bEquipped)
					continue;

				tradeItem.guid = itItem->m_serial;
				tradeItem.typeId = itItem->m_typeId;
				strncpy(tradeItem.uccIdx, itItem->m_uccKey, 9);
				tradeItem.uccSeq = itItem->m_uccSeq;

				tradeItem.count = 1;
				tradeItem.price = 0;
				tradeItem.uccStatus = itItem->m_uccStatus;
				if (IsLocalContent((localContentType_t)0x51)) {
					IFF_STRUCT::sPart *pPart = ItemManager()->FindPart(itItem->m_typeId);
					if (pPart != NULL) {
						// TODO: card slots and card type ids, as for the parts above
						// (lines 219-232).
					}
				}

				m_listTransferableItem.push_back(tradeItem);
			}
		}
	}

	// The items of the third extra list, less those registered.
	std::list<sItemInfo>::iterator itExtra;

	std::list<sItemInfo> &extraList = CSharedDoc::Instance()->m_myExtraItemList3;
	for (itExtra = extraList.begin(); itExtra != extraList.end(); ++itExtra) {
		IFF_ITEM_COMMON *pCommon = ItemManager()->FindCommonItem(itExtra->m_typeId);

		if (pCommon == NULL)
			continue;

		unsigned char tradeKind = (pCommon->m_flags0 >> 1) & 0x0F;
		if (tradeKind == 1 || tradeKind == 3) {
			tradeItem.guid = itExtra->m_serial;
			tradeItem.typeId = itExtra->m_typeId;
			tradeItem.count = itExtra->m_count - GetNumTradingItem(itExtra->m_serial);

			tradeItem.price = 0;
			m_listTransferableItem.push_back(tradeItem);
		}
	}

	SetListEquipParts();

	// Drop the items worn as parts.
	std::list<sTradeItem>::iterator it;

	for (it = m_listTransferableItem.begin(); it != m_listTransferableItem.end(); ) {
		if (IsEquipParts(it->typeId, it->guid)) {
			it = m_listTransferableItem.erase(it);
			continue;
		}
		++it;
	}

	// Stackable items, less those registered and equipped.
	std::list<sItemInfo>::iterator itInfo;

	std::list<sItemInfo> &infoList = CSharedDoc::Instance()->m_itemInfoList;
	for (itInfo = infoList.begin(); itInfo != infoList.end(); ++itInfo) {
		IFF_ITEM_COMMON *pCommon = ItemManager()->FindCommonItem(itInfo->m_typeId);

		if (pCommon == NULL)
			continue;

		unsigned char tradeKind = (pCommon->m_flags0 >> 1) & 0x0F;
		if (tradeKind == 1 || tradeKind == 3) {
			tradeItem.guid = itInfo->m_serial;
			tradeItem.typeId = itInfo->m_typeId;
			int count = itInfo->m_count - GetNumTradingItem(itInfo->m_serial) - GetNumEquipItem(itInfo->m_typeId);
			tradeItem.count = count;
			tradeItem.price = 0;

			if (count > 0)
				m_listTransferableItem.push_back(tradeItem);
		}
	}

	// Gear, but the equipped ball.
	std::list<sItemInfo>::iterator itGear;

	std::list<sItemInfo> &gearList = CSharedDoc::Instance()->m_myGearList;
	for (itGear = gearList.begin(); itGear != gearList.end(); ++itGear) {
		if (itGear->m_typeId == CSharedDoc::Instance()->m_myBallId)
			continue;

		IFF_ITEM_COMMON *pCommon = ItemManager()->FindCommonItem(itGear->m_typeId);

		if (pCommon == NULL)
			continue;

		unsigned char tradeKind = (pCommon->m_flags0 >> 1) & 0x0F;
		if (tradeKind == 1 || tradeKind == 3) {
			tradeItem.guid = itGear->m_serial;

			tradeItem.typeId = itGear->m_typeId;
			tradeItem.count = itGear->m_count - GetNumTradingItem(itGear->m_serial);

			tradeItem.price = 0;
			m_listTransferableItem.push_back(tradeItem);
		}
	}

	// Caddies, but the equipped one.
	std::map<unsigned int, sCaddieInfo>::iterator itCaddie;

	std::map<unsigned int, sCaddieInfo> &caddieMap = CSharedDoc::Instance()->m_caddieMap;
	for (itCaddie = caddieMap.begin(); itCaddie != caddieMap.end(); ++itCaddie) {
		if (itCaddie->second.id == CSharedDoc::Instance()->m_myCaddieId)
			continue;

		IFF_ITEM_COMMON *pCommon = ItemManager()->FindCommonItem(itCaddie->second.typeId);

		if (pCommon == NULL)
			continue;

		unsigned char tradeKind = (pCommon->m_flags0 >> 1) & 0x0F;
		if (tradeKind == 1 || tradeKind == 3) {
			tradeItem.guid = itCaddie->second.id;
			tradeItem.typeId = itCaddie->second.typeId;
			tradeItem.count = 1 - GetNumTradingItem(itCaddie->second.id);

			tradeItem.price = 0;
			m_listTransferableItem.push_back(tradeItem);
		}
	}

	// Club sets, but the equipped one, with their upgrade enchant value.
	std::map<unsigned int, sItemInfo>::iterator itClub;

	std::map<unsigned int, sItemInfo> &clubMap = CSharedDoc::Instance()->m_myItemMap;
	for (itClub = clubMap.begin(); itClub != clubMap.end(); ++itClub) {
		if (itClub->second.m_serial == CSharedDoc::Instance()->m_myClubId)
			continue;

		IFF_ITEM_COMMON *pCommon = ItemManager()->FindCommonItem(itClub->second.m_typeId);

		if (pCommon == NULL)
			continue;

		unsigned char tradeKind = (pCommon->m_flags0 >> 1) & 0x0F;
		if (tradeKind == 1 || tradeKind == 3) {
			memset(&tradeItem, 0, sizeof(tradeItem));

			tradeItem.guid = itClub->second.m_serial;
			tradeItem.typeId = itClub->second.m_typeId;
			tradeItem.count = 1 - GetNumTradingItem(itClub->second.m_serial);
			tradeItem.price = 0;

			tradeItem.period = 0;
			memcpy(tradeItem.stats, itClub->second.m_clubStatLevel, sizeof(tradeItem.stats));

			for (int stat = 0; stat < 5; stat++) {
				for (int level = 0; level < (short)tradeItem.stats[stat]; level++) {
					IFF_STRUCT::sEnchant *pEnchant = ItemManager()->FindEnchant(((stat | 0x340) << 20) | level);

					if (pEnchant != NULL)
						tradeItem.period += pEnchant->m_value;
				}
			}

			m_listTransferableItem.push_back(tradeItem);
		}
	}

	// Cards.
	if (IsLocalContent((localContentType_t)0x51)) {
		std::list<sCards>::iterator itCard;

		std::list<sCards> &cardList = CSharedDoc::Instance()->m_myCardList;
		for (itCard = cardList.begin(); itCard != cardList.end(); ++itCard) {
			IFF_ITEM_COMMON *pCommon = ItemManager()->FindCommonItem(itCard->m_typeId);

			if (pCommon == NULL)
				continue;

			unsigned char tradeKind = (pCommon->m_flags0 >> 1) & 0x0F;
			if (tradeKind == 1 || tradeKind == 3) {
				tradeItem.guid = itCard->m_guid;
				tradeItem.typeId = itCard->m_typeId;
				tradeItem.count = itCard->m_count;

				tradeItem.price = 0;
				m_listTransferableItem.push_back(tradeItem);
			}
		}
	}
}

// transferableitem.cpp:428
bool CTransferableItem::GetTransferableItem(sItemInfo &itemInfo, sTradeItem &tradeItem, bool useCount) {
	IFF_ITEM_COMMON *pCommon = ItemManager()->FindCommonItem(itemInfo.m_typeId);

	int count;
	if (useCount)
		count = itemInfo.m_count;
	else
		count = 1;

	if (pCommon == NULL)
		return false;

	unsigned char tradeKind = (pCommon->m_flags0 >> 1) & 0x0F;
	if (tradeKind == 1 || tradeKind == 3) {
		if (count == 0)
			return false;

		tradeItem.count = count;

		tradeItem.guid = itemInfo.m_serial;
		tradeItem.typeId = itemInfo.m_typeId;
		tradeItem.price = 0;

		if (IsLocalContent((localContentType_t)0x57) && IsUccClothes(itemInfo.m_typeId)) {
			strncpy(tradeItem.uccIdx, itemInfo.m_uccKey, 8);
			tradeItem.uccSeq = itemInfo.m_uccSeq;
			tradeItem.uccStatus = itemInfo.m_uccStatus;
		}
	} else {
		return false;
	}

	return true;
}

// transferableitem.cpp:477
void CTransferableItem::SetTransferableEtcItemList(void) {
	sTradeItem tradeItem;

	std::map<unsigned int, sCaddieInfo> &caddieMap = CSharedDoc::Instance()->m_caddieMap;
	std::map<unsigned int, sCaddieInfo>::iterator itCaddie;

	for (itCaddie = caddieMap.begin(); itCaddie != caddieMap.end(); ++itCaddie) {
		// The equipped caddie cannot be traded.
		if (itCaddie->second.id != CSharedDoc::Instance()->m_myCaddieId) {
			memset(&tradeItem, 0, sizeof(tradeItem));
			IFF_ITEM_COMMON *pCommon = ItemManager()->FindCommonItem(itCaddie->second.typeId);

			if (pCommon != NULL) {
				unsigned char tradeKind = (pCommon->m_flags0 >> 1) & 0x0F;
				if (tradeKind == 1 || tradeKind == 3) {
					tradeItem.guid = itCaddie->second.id;
					tradeItem.typeId = itCaddie->second.typeId;
					tradeItem.price = 0;
					tradeItem.count = 1;

					m_listTransferableItem.push_back(tradeItem);
				}
			}
		}
	}

	std::map<unsigned int, sItemInfo> &itemMap = CSharedDoc::Instance()->m_myItemMap;
	std::map<unsigned int, sItemInfo>::iterator itItem;

	for (itItem = itemMap.begin(); itItem != itemMap.end(); ++itItem) {
		// The equipped club set cannot be traded.
		if (itItem->second.m_serial != CSharedDoc::Instance()->m_myClubId) {
			memset(&tradeItem, 0, sizeof(tradeItem));
			IFF_ITEM_COMMON *pCommon = ItemManager()->FindCommonItem(itItem->second.m_typeId);

			if (pCommon != NULL) {
				unsigned char tradeKind = (pCommon->m_flags0 >> 1) & 0x0F;
				if (tradeKind == 1 || tradeKind == 3) {
					memset(&tradeItem, 0, sizeof(tradeItem));

					tradeItem.typeId = itItem->second.m_typeId;
					tradeItem.guid = itItem->second.m_serial;
					tradeItem.count = 1;
					tradeItem.price = 0;
					tradeItem.period = 0;
					memcpy(tradeItem.stats, itItem->second.m_clubStatLevel, sizeof(tradeItem.stats));

					// Sum the enchant value of every upgrade level of the five stats.
					for (int stat = 0; stat < 5; stat++) {
						for (int level = 0; level < (short)tradeItem.stats[stat]; level++) {
							IFF_STRUCT::sEnchant *pEnchant = ItemManager()->FindEnchant(((stat | 0x340) << 20) | level);

							if (pEnchant != NULL)
								tradeItem.period += pEnchant->m_value;
						}
					}

					m_listTransferableItem.push_back(tradeItem);
				}
			}
		}
	}
}

// transferableitem.cpp:560
void CTransferableItem::SetListEquipParts(void) {
	m_listEquipParts.clear();

	std::map<unsigned int, sCharacterInfo>::iterator it;

	for (it = CSharedDoc::Instance()->m_characterMap.begin(); it != CSharedDoc::Instance()->m_characterMap.end(); ++it) {
		for (int i = 0; i < 24; i++) {
			if (it->second.partTypeIds[i] != 0)
				m_listEquipParts.push_back(it->second.id);
		}
	}
}

// transferableitem.cpp:603
bool CTransferableItem::IsEquipParts(unsigned long typeId) {
	bool bFound = false;
	std::list<unsigned long>::iterator it;

	for (it = m_listEquipParts.begin(); it != m_listEquipParts.end(); ++it) {
		if (typeId == *it) {
			bFound = true;
			break;
		}
	}

	return bFound;
}

// transferableitem.cpp:625
bool CTransferableItem::IsUccEquipped(sItemInfo *pItemInfo) {
	bool bEquipped = false;
	std::map<unsigned int, sCharacterInfo>::iterator it;

	for (it = CSharedDoc::Instance()->m_characterMap.begin(); it != CSharedDoc::Instance()->m_characterMap.end(); ++it) {
		sCharacterInfo *pCharacter = &it->second;

		if (pCharacter == NULL)
			break;

		bEquipped = true;
		break;
	}

	return bEquipped;
}

// transferableitem.cpp:654
int CTransferableItem::GetNumEquipItem(unsigned long typeId) {
	int num = 0;

	for (int i = 0; i < 10; i++) {
		if (typeId == CSharedDoc::Instance()->m_equipItem[i])
			num++;
	}

	return num;
}

// transferableitem.cpp:679
void CTransferableItem::Set_RegistrationListSize(int size) {
	m_nRegistrationListSize = size;
}

// transferableitem.cpp:684
bool CTransferableItem::SetRegistrationItem(unsigned long guid) {
	if (m_listRegistrationItem.size() >= (unsigned int)m_nRegistrationListSize)
		return false;

	std::list<sTradeItem>::iterator it;

	for (it = m_listTransferableItem.begin(); it != m_listTransferableItem.end(); ++it) {
		if (it->guid == guid) {
			sTradeItem item = *it;
			m_listRegistrationItem.push_back(item);
			return true;
		}
	}

	return false;
}

// transferableitem.cpp:707
bool CTransferableItem::SetRegistrationItem(unsigned long guid, int count) {
	char itemBuffer[sizeof(sTradeItem)];
	sTradeItem &item = *(sTradeItem *)itemBuffer;
	memset(&item, 0, sizeof(sTradeItem));

	if (m_listRegistrationItem.size() < (unsigned int)m_nRegistrationListSize) {
		if (IsTransferableItem(guid, item, count)) {
			item.count = count;
			m_listRegistrationItem.push_back(item);
			return true;
		}
	}

	return false;
}

// transferableitem.cpp:725
bool CTransferableItem::IsTransferableItem(unsigned long guid, sTradeItem &tradeItem, int count) {
	std::list<sTradeItem>::iterator it;

	for (it = m_listTransferableItem.begin(); it != m_listTransferableItem.end(); ++it) {
		if (it->guid == guid) {
			if (count != 0) {
				int remain = it->count - count;
				if (remain < 0)
					return false;

				it->count -= count;
			}

			tradeItem = *it;
			return true;
		}
	}

	return false;
}

// transferableitem.cpp:777
bool CTransferableItem::IsRegisterationItem(unsigned long guid, sTradeItem &tradeItem) {
	std::list<sTradeItem>::iterator it;

	for (it = m_listRegistrationItem.begin(); it != m_listRegistrationItem.end(); ++it) {
		if (it->guid == guid) {
			std::list<sTradeItem>::iterator itTransferable;

			for (itTransferable = m_listTransferableItem.begin(); itTransferable != m_listTransferableItem.end(); ++itTransferable) {
				if (itTransferable->guid == guid) {
					itTransferable->count += it->count;
					break;
				}
			}

			tradeItem = *it;
			m_listRegistrationItem.erase(it);
			return true;
		}
	}

	return false;
}

// transferableitem.cpp:807
bool CTransferableItem::IsRegisterationItem(unsigned long guid) {
	std::list<sTradeItem>::iterator it;

	for (it = m_listRegistrationItem.begin(); it != m_listRegistrationItem.end(); ++it) {
		if (it->guid == guid)
			return true;
	}

	return false;
}

// transferableitem.cpp:823
std::list<sTradeItem> *CTransferableItem::GetList_RegistrationItem(void) {
	return &m_listRegistrationItem;
}

// transferableitem.cpp:828
void CTransferableItem::ResetRegistrationItemList(void) {
	m_listRegistrationItem.clear();

	SetTransferableItemList();
}

// transferableitem.cpp:835
int CTransferableItem::Get_RegistrationListSize(void) {
	return m_listRegistrationItem.size();
}

// transferableitem.cpp:840
bool CTransferableItem::DelRegistrationItem(unsigned long guid) {
	char itemBuffer[sizeof(sTradeItem)];
	sTradeItem &item = *(sTradeItem *)itemBuffer;

	if (m_listRegistrationItem.size() == 0)
		return false;

	if (IsRegisterationItem(guid, item)) {
		m_listTransferableItem.sort(CompareTradeItem);
		return true;
	}

	return false;
}

// transferableitem.cpp:861
std::list<sTradeItem>::iterator CTransferableItem::DelRegistrationItem_itor_return(unsigned long guid) {
	if (m_listRegistrationItem.size() == 0)
		return NULL;

	std::list<sTradeItem>::iterator it;

	for (it = m_listRegistrationItem.begin(); it != m_listRegistrationItem.end(); ++it) {
		if (it->guid == guid) {
			std::list<sTradeItem>::iterator itTransferable;

			for (itTransferable = m_listTransferableItem.begin(); itTransferable != m_listTransferableItem.end(); ++itTransferable) {
				if (itTransferable->guid == guid) {
					itTransferable->count += it->count;
					break;
				}
			}

			return m_listRegistrationItem.erase(it);
		}
	}

	return NULL;
}

// transferableitem.cpp:888
void CTransferableItem::SetMailInfoPage(sMailBoxListForPage &page) {
	m_mailBoxPageList.mailList.clear();
	m_mailBoxPageList = page;
}

// transferableitem.cpp:895
void CTransferableItem::SetMailInfo(sMailInfo &info) {
	m_mailInfo.itemList.clear();
	m_mailInfo = info;
}

// transferableitem.cpp:902
sMailBoxListForPage *CTransferableItem::GetMailBoxPageList(void) {
	return &m_mailBoxPageList;
}

// transferableitem.cpp:907
sMailInfo *CTransferableItem::GetMailInfo(void) {
	return &m_mailInfo;
}

// transferableitem.cpp:915
int CTransferableItem::GetNumTradingItem(unsigned long guid) {
	int num = 0;
	std::list<sTradeItem>::iterator it;

	for (it = m_listRegistrationItem.begin(); it != m_listRegistrationItem.end(); ++it) {
		if (it->guid == guid)
			num += it->count;
	}

	return num;
}

// transferableitem.cpp:934
bool CTransferableItem::IsEquipParts(unsigned long typeId, unsigned long partTypeId) {
	bool bFound = false;
	std::list<unsigned long>::iterator it;

	for (it = m_listEquipParts.begin(); it != m_listEquipParts.end(); ++it) {
		if (*it != 0 && partTypeId == *it) {
			m_listEquipParts.erase(it);
			bFound = true;
			break;
		}
	}

	return bFound;
}

// transferableitem.cpp:960
bool CTransferableItem::IsCheckCardParts(unsigned long partsId, unsigned long partsTypeId) {
	std::list<sSCardAvilityPeriodInfo>::iterator it;

	for (it = CSharedDoc::Instance()->m_cardPeriodList[0].begin(); it != CSharedDoc::Instance()->m_cardPeriodList[0].end(); ++it) {
		if (it->partsTypeId == partsTypeId && it->partsId == partsId)
			break;
	}

	return it == CSharedDoc::Instance()->m_cardPeriodList[0].end() ? true : false;
}
