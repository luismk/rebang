// Original: d:\build\custom\temp\code\shared\itemmanager.cpp
#include <stdio.h>
#include <algorithm>
#include <sstream>

#include "ItemManager.h"
#include "../client/ProjectG/UccManager.h"
#include "../client/ProjectG/cardsystem.h"
#include "../client/ProjectG/commonutil.h"
#include "client/Wangreal/source/XUnzip.h"
#include "client/Wangreal/source/XZip.h"
#ifdef PG_IFF_US
#include "iff_us.h"
#endif
#include "classdefine.h"
#include "Localize.h"
#include "TikiMagicBoxTable.h"

// The recycle recipe tables (RecycleDlg.h, recycletable.h), freed by Reset.
#include "../client/ProjectG/RecycleDlg.h"
extern int g_NumIndexRecycleItem;
extern sRecycleItem** g_ArrayTidMixItem;

// A replacement the item names go through (CheckInvalidItemName).
struct ChangeWordPair {
	std::string m_from;   // 0x0
	std::string m_to;     // 0x1C
};

// itemmanager.cpp:43
void DevFinalStockPrice(IFF_ITEM_COMMON &item) {
	if (item.m_active && item.m_price >= 1000000) {
		unsigned char stockType = item.m_flags0 >> 5;
		if (stockType != 0 && stockType != 4) {
			char text[128] = "";
			// "Shop display item price anomaly (%s)"
			sprintf(text, "\xbb\xf3\xc1\xa1 \xc1\xf8\xbf\xad\xbb\xf3\xc7\xb0 \xb0\xa1\xb0\xdd\xc0\xcc\xbb\xf3(%s)", item.m_name);
			MessageBoxA(NULL, text, "PangYa", MB_OK);

			item.m_flags0 &= 1;
		}
	}
}

// itemmanager.cpp:76
void DevRentalPreventSale(IFF_STRUCT::sPart &part) {
	if (part.m_active && (part.m_flags & 0x1E)) {
		if (part.m_rentalPrice > 0) {
			char text[128] = "";
			// "Found a tradable rental part (%s)"
			sprintf(text, "\xb0\xc5\xb7\xa1\xb0\xa1 \xb5\xc7\xb4\xc2 \xb7\xa3\xc5\xbb \xc6\xc4\xc3\xf7 \xb9\xdf\xb0\xdf(%s)", part.m_name);
			MessageBoxA(NULL, text, "PangYa", MB_OK);

			part.m_rentalPrice = 0;
		}
	}
}

// itemmanager.cpp:105
CItemManager::CItemManager(void) {
	m_loaded = false;
}

// itemmanager.cpp:115
void CItemManager::Reload(void) {
	_client::_private::CLock<_client::CCriticalSection> lock(m_cs);

	DWORD tick = GetTickCount();

	m_loaded = true;
	Reset();
	Load();

	m_loaded = false;
}

// itemmanager.cpp:133
void CItemManager::Reset(void) {
	m_charMap.clear();
	m_partMap.clear();
	m_clubMap.clear();
	m_clubSetMap.clear();
	m_ballMap.clear();
	m_itemMap.clear();
	m_caddieMap.clear();
	m_cadItemMap.clear();
	m_setItemMap.clear();
	m_courseMap.clear();
	m_matchMap.clear();
	m_titleMap.clear();
	m_enchantMap.clear();
	m_skinMap.clear();
	m_hairStyleMap.clear();
	m_childItemMap.clear();
	m_mascotMap.clear();
	m_descMap.clear();
	m_auxPartMap.clear();
	m_questDropMap.clear();
	m_furnitureMap.clear();
	m_cardMap.clear();
	m_offlineShopMap.clear();
	m_randomRecycleMap.clear();
	m_cadieMagicBoxList.clear();
	m_furnitureAbilityList.clear();

	m_cutinInformationMap.clear();

	std::map<unsigned int, IFF_STRUCT::sQuest>::iterator it;
	for (it = m_questMap.begin(); it != m_questMap.end(); ++it) {
		IFF_STRUCT::sQuest *quest = &it->second;

		if (quest && quest->m_extraData) {
			delete[] quest->m_extraData;
			quest->m_extraData = NULL;
		}
	}

	m_questMap.clear();

	m_loaded = false;

	if (g_ArrayTidMixItem) {
		for (int i = 0; i < g_NumIndexRecycleItem; i++) {
			if (g_ArrayTidMixItem[i]) {
				delete[] g_ArrayTidMixItem[i];
				g_ArrayTidMixItem[i] = NULL;
			}
		}

		delete[] g_ArrayTidMixItem;
		g_ArrayTidMixItem = NULL;
	}

	if (g_ArrayRecycleItem) {
		delete[] g_ArrayRecycleItem;
		g_ArrayRecycleItem = NULL;
	}
}

// itemmanager.cpp:239
ChangeWordPair ChangeWordList[] = {
	{ "'", "`" },
};
int ChangeWordListCount = sizeof(ChangeWordList) / sizeof(ChangeWordList[0]);

// itemmanager.cpp:246 (the records that embed the common item head)
template <class T>
void CheckCommonItemName(T &item) {
	std::string name(item.second.m_name);

	for (int i = 0; i < ChangeWordListCount; i++) {
		if (str_Replace(name, ChangeWordList[i].m_from, ChangeWordList[i].m_to) == true) {
			if (name.length() >= sizeof(item.second.m_name) - 1) {
				strncpy(item.second.m_name, name.c_str(), sizeof(item.second.m_name) - 1);
				item.second.m_name[sizeof(item.second.m_name) - 1] = 0;
			} else {
				strcpy(item.second.m_name, name.c_str());
			}
		}
	}
}

// itemmanager.cpp:273 (the records with a name of their own)
template <class T>
void CheckItemName(T &item) {
	std::string name(item.second.m_name);

	for (int i = 0; i < ChangeWordListCount; i++) {
		if (str_Replace(name, ChangeWordList[i].m_from, ChangeWordList[i].m_to) == true) {
			if (name.length() >= sizeof(item.second.m_name) - 1) {
				strncpy(item.second.m_name, name.c_str(), sizeof(item.second.m_name) - 1);
				item.second.m_name[sizeof(item.second.m_name) - 1] = 0;
			} else {
				strcpy(item.second.m_name, name.c_str());
			}
		}
	}
}

// itemmanager.cpp:301
void CItemManager::CheckInvalidItemName(void) {
	std::for_each(m_charMap.begin(), m_charMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sChar> >);
	std::for_each(m_partMap.begin(), m_partMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sPart> >);
	std::for_each(m_clubMap.begin(), m_clubMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sClub> >);
	std::for_each(m_clubSetMap.begin(), m_clubSetMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sClubSet> >);
	std::for_each(m_ballMap.begin(), m_ballMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sBall> >);
	std::for_each(m_itemMap.begin(), m_itemMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sItem> >);
	std::for_each(m_caddieMap.begin(), m_caddieMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sCaddie> >);
	std::for_each(m_cadItemMap.begin(), m_cadItemMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sCadItem> >);
	std::for_each(m_setItemMap.begin(), m_setItemMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sSetItem> >);
	std::for_each(m_skinMap.begin(), m_skinMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sSkin> >);
	std::for_each(m_hairStyleMap.begin(), m_hairStyleMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sHairStyle> >);
	std::for_each(m_mascotMap.begin(), m_mascotMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sMascot> >);
	std::for_each(m_auxPartMap.begin(), m_auxPartMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sAuxPart> >);
	std::for_each(m_questDropMap.begin(), m_questDropMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sQuestDrop> >);
	std::for_each(m_furnitureMap.begin(), m_furnitureMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sFurniture> >);
	std::for_each(m_cardMap.begin(), m_cardMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sCard> >);
	// itemmanager.cpp:319 walks m_offlineShopMap the same way; IFF_STRUCT::sOfflineShop is not defined yet.
	std::for_each(m_courseMap.begin(), m_courseMap.end(), CheckCommonItemName<std::pair<const unsigned int, IFF_STRUCT::sCourse> >);

	std::for_each(m_matchMap.begin(), m_matchMap.end(), CheckItemName<std::pair<const unsigned int, IFF_STRUCT::sMatch> >);
	std::for_each(m_titleMap.begin(), m_titleMap.end(), CheckItemName<std::pair<const unsigned int, IFF_STRUCT::sTitle> >);
	std::for_each(m_childItemMap.begin(), m_childItemMap.end(), CheckItemName<std::pair<const unsigned int, IFF_STRUCT::sChildItem> >);
}

// itemmanager.cpp:328
bool CItemManager::Load(void) {
	std::stringstream name;

#ifdef PG_IFF_US
	// The CMake build reads a later client's tables (CMake PG_IFF_US).
	name << PG_IFF_US_FILE;
#else
	name << "PangYa";
#endif
	name << ".iff";

	WFile file(name.str().c_str());

	int length = file.Length();
	char *data = new char[length];

	file.Read(data, length);
	file.Close();

	HZIP zip = OpenZipU(data, length, ZIP_MEMORY);
	if (!zip) {
		delete[] data;
		return false;
	}

	Reset();

	ZIPENTRY entry;
	GetZipItemA(zip, -1, &entry);
	int count = entry.index;

	for (int i = 0; i < count; i++) {
		GetZipItemA(zip, i, &entry);
		char *item = new char[entry.unc_size];
		UnzipItem(zip, i, item, entry.unc_size, ZIP_MEMORY);
#ifdef PG_IFF_US
		item = PgConvertIff(entry.name, item, &entry.unc_size);
#endif

		if (_stricmp(entry.name, "Character.iff") == 0) {
			MakeCharMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Part.iff") == 0) {
			MakePartMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Club.iff") == 0) {
			MakeClubMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "ClubSet.iff") == 0) {
			MakeClubSetMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Ball.iff") == 0) {
			MakeBallMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Item.iff") == 0) {
			MakeItemMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Caddie.iff") == 0) {
			MakeCaddieMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "CaddieItem.iff") == 0) {
			MakeCadItemMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "SetItem.iff") == 0) {
			MakeSetItemMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Course.iff") == 0) {
			MakeCourseMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Match.iff") == 0) {
			MakeMatchMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Title.iff") == 0) {
			MakeTitleMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Enchant.iff") == 0) {
			MakeEnchantMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Skin.iff") == 0) {
			MakeSkinMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "HairStyle.iff") == 0) {
			MakeHairStyleMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "ChildItem.iff") == 0) {
			MakeChildItemMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Mascot.iff") == 0) {
			MakeMascotMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "AuxPart.iff") == 0) {
			MakeAuxPartMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "QuestDrop.iff") == 0) {
			MakeQuestDropMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Quest.iff") == 0) {
			MakeQuestMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Card.iff") == 0) {
			if (IsLocalContent((localContentType_t)0x51))
				MakeCardMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Furniture.iff") == 0) {
			if (IsLocalContent((localContentType_t)0x56))
				MakeFurnitureMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "Desc.iff") == 0) {
			MakeDescMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "OfflineShop.iff") == 0) {
			if (IsLocalContent((localContentType_t)0x66))
				MakeOfflineMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "CadieMagicBox.iff") == 0) {
			MakeCadieMagicBoxMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "CadieMagicBoxRandom.iff") == 0) {
			MakeRandomRecycleMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "FurnitureAbility.iff") == 0) {
			MakeFurnitureAbilityList(item, entry.unc_size);
		} else if (_stricmp(entry.name, "TikiRecipe.iff") == 0) {
			MakeTikiRecipeMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "TikiPointTable.iff") == 0) {
			MakeTikiPointTableMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "TikiSpecialTable.iff") == 0) {
			MakeTikiSpecialTableMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "CutinInfomation.iff") == 0) {
			MakeCutinInforMationTableMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "SpecialPrizeItem.iff") == 0) {
			MakeSpecialPrizeItemMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "RandomBox.sff") == 0) {
			MakeRandomBoxMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "NonVisibleItemTable.iff") == 0) {
			MakeNonVisibleItemMap(item, entry.unc_size);
		} else if (_stricmp(entry.name, "SubscriptionItemTable.iff") == 0) {
			MakeSubscriptionItemMap(item, entry.unc_size);
		}

		delete[] item;
	}

	CloseZipU(zip);
	delete[] data;

	MakeItemBuffMap();

	CheckInvalidItemName();

	return true;
}

// The head of an IFF table file (the Make*Map functions read the records
// after it).
struct sIffHeader {
	unsigned short m_count;   // 0x0, the number of records
	unsigned short m_bindingId; // 0x2, the table's binding id (not read here); a scalar, not a
	                          // byte array, so /GS does not place the header beside the cookie
	unsigned long m_version;  // 0x4
};
CHECK_SIZE(sIffHeader, 0x8);

// itemmanager.cpp:600
void CItemManager::MakeCharMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sChar character = *(IFF_STRUCT::sChar *)record;
		record += sizeof(IFF_STRUCT::sChar);

		// TODO(classdefine.h): sChar does not embed IFF_ITEM_COMMON in its
		// read-only header yet, and neither has the sale start (0x70, a
		// _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)character;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&character + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&character + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (character.m_active) {
			m_charMap.insert(std::pair<const unsigned int, IFF_STRUCT::sChar>(character.m_typeId, character));
		}
	}
}

// itemmanager.cpp:641
void CItemManager::MakePartMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sPart part = *(IFF_STRUCT::sPart *)record;
		record += sizeof(IFF_STRUCT::sPart);

		// TODO(classdefine.h): sPart does not embed IFF_ITEM_COMMON in its
		// read-only header yet, and neither has the sale start (0x70, a
		// _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)part;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&part + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&part + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (part.m_active) {
			DevFinalStockPrice(common);
			DevRentalPreventSale(part);
			m_partMap.insert(std::pair<const unsigned int, IFF_STRUCT::sPart>(part.m_typeId, part));
		}
	}
}

// itemmanager.cpp:690
void CItemManager::MakeClubMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sClub club = *(IFF_STRUCT::sClub *)record;
		record += sizeof(IFF_STRUCT::sClub);

		// TODO(classdefine.h): sClub does not embed IFF_ITEM_COMMON in its
		// read-only header yet, and neither has the sale start (0x70, a
		// _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)club;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&club + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&club + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (club.m_active) {
			DevFinalStockPrice(common);
			m_clubMap.insert(std::pair<const unsigned int, IFF_STRUCT::sClub>(club.m_typeId, club));
		}
	}
}

// itemmanager.cpp:730
void CItemManager::MakeClubSetMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sClubSet clubSet = *(IFF_STRUCT::sClubSet *)record;
		record += sizeof(IFF_STRUCT::sClubSet);

		// TODO(classdefine.h): sClubSet does not embed IFF_ITEM_COMMON in its
		// read-only header yet, and neither has the sale start (0x70, a
		// _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)clubSet;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&clubSet + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&clubSet + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (clubSet.m_active) {
			m_clubSetMap.insert(std::pair<const unsigned int, IFF_STRUCT::sClubSet>(clubSet.m_typeId, clubSet));
		}
	}
}

// itemmanager.cpp:769
void CItemManager::MakeBallMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sBall ball = *(IFF_STRUCT::sBall *)record;
		record += sizeof(IFF_STRUCT::sBall);

		// TODO(classdefine.h): sBall does not embed IFF_ITEM_COMMON in its
		// read-only header yet, and neither has the sale start (0x70, a
		// _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)ball;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&ball + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&ball + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (ball.m_active) {
			m_ballMap.insert(std::pair<const unsigned int, IFF_STRUCT::sBall>(ball.m_typeId, ball));
		}
	}
}

// itemmanager.cpp:808
void CItemManager::MakeItemMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sItem item = *(IFF_STRUCT::sItem *)record;
		record += sizeof(IFF_STRUCT::sItem);

		// TODO(classdefine.h): sItem does not embed IFF_ITEM_COMMON in its
		// read-only header yet, and neither has the sale start (0x70, a
		// _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)item;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&item + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&item + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (item.m_active) {
			DevFinalStockPrice(common);
			m_itemMap.insert(std::pair<const unsigned int, IFF_STRUCT::sItem>(item.m_typeId, item));
		}
	}
}

// itemmanager.cpp:848
void CItemManager::MakeCaddieMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sCaddie caddie = *(IFF_STRUCT::sCaddie *)record;
		record += sizeof(IFF_STRUCT::sCaddie);

		// TODO(classdefine.h): sCaddie does not embed IFF_ITEM_COMMON in its
		// read-only header yet, and neither has the sale start (0x70, a
		// _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)caddie;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&caddie + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&caddie + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (caddie.m_active) {
			m_caddieMap.insert(std::pair<const unsigned int, IFF_STRUCT::sCaddie>(caddie.m_typeId, caddie));
		}
	}
}

// itemmanager.cpp:887
void CItemManager::MakeCadItemMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sCadItem cadItem = *(IFF_STRUCT::sCadItem *)record;
		record += sizeof(IFF_STRUCT::sCadItem);

		// TODO(classdefine.h): sCadItem does not embed IFF_ITEM_COMMON in its
		// read-only header yet, and neither has the sale start (0x70, a
		// _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)cadItem;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&cadItem + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&cadItem + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (cadItem.m_active) {
			m_cadItemMap.insert(std::pair<const unsigned int, IFF_STRUCT::sCadItem>(cadItem.m_typeId, cadItem));
		}
	}
}

// itemmanager.cpp:926
void CItemManager::MakeSetItemMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sSetItem setItem = *(IFF_STRUCT::sSetItem *)record;
		record += sizeof(IFF_STRUCT::sSetItem);

		// TODO(classdefine.h): sSetItem does not embed IFF_ITEM_COMMON in its
		// read-only header yet, and neither has the sale start (0x70, a
		// _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)setItem;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&setItem + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&setItem + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (setItem.m_active) {
			DevFinalStockPrice(common);
			m_setItemMap.insert(std::pair<const unsigned int, IFF_STRUCT::sSetItem>(setItem.m_typeId, setItem));
		}
	}
}

// itemmanager.cpp:966
void CItemManager::MakeCourseMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);
	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sCourse course = *(IFF_STRUCT::sCourse *)record;
		record += sizeof(IFF_STRUCT::sCourse);

		// TODO(classdefine.h): sCourse does not embed IFF_ITEM_COMMON in its
		// read-only header yet, and neither has the sale start (0x70, a
		// _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)course;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&course + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&course + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (course.m_active) {
			m_courseMap.insert(std::pair<const unsigned int, IFF_STRUCT::sCourse>(course.m_typeId, course));
		}
	}

	// The built-in courses the IFF does not list.
	IFF_STRUCT::sCourse course;
	memset(&course, 0, sizeof(course));

	course.courseFlags = (course.courseFlags & 0xF0) | 1;
	strcpy(course.m_name, "TUTORIAL");
	strcpy(course.m_mapName, "tuto");

	strcpy(course.m_propertyFile, "tuto_property.xml");

	course.m_typeId = 0x280000FD;
	course.m_rate = 1.1f;
	m_courseMap.insert(std::pair<const unsigned int, IFF_STRUCT::sCourse>(course.m_typeId, course));

	memset(&course, 0, sizeof(course));
	course.courseFlags = (course.courseFlags & 0xF0) | 1;
	strcpy(course.m_name, "TEST");
	strcpy(course.m_mapName, "test");

	strcpy(course.m_propertyFile, "test_property.xml");
	course.m_typeId = 0x280000FE;
	course.m_rate = 1.1f;
	m_courseMap.insert(std::pair<const unsigned int, IFF_STRUCT::sCourse>(course.m_typeId, course));

	memset(&course, 0, sizeof(course));

	course.courseFlags = (course.courseFlags & 0xF0) | 1;
	strcpy(course.m_name, "RANDOM");
	strcpy(course.m_mapName, "random");

	strcpy(course.m_propertyFile, "test_property.xml");
	strcpy(course.m_icon, "map_random");
	course.m_typeId = 0x2800007F;
	course.m_active = 1;
	course.m_rate = 1.1f;
	m_courseMap.insert(std::pair<const unsigned int, IFF_STRUCT::sCourse>(course.m_typeId, course));
}

// itemmanager.cpp:1043
void CItemManager::MakeMatchMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sMatch match = *(IFF_STRUCT::sMatch *)record;
		record += sizeof(IFF_STRUCT::sMatch);
		if (match.m_active) {
			m_matchMap.insert(std::pair<const unsigned int, IFF_STRUCT::sMatch>(match.m_typeId, match));
		}
	}
}

// itemmanager.cpp:1067
void CItemManager::MakeTitleMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sTitle title = *(IFF_STRUCT::sTitle *)record;
		record += sizeof(IFF_STRUCT::sTitle);
		if (title.m_active) {
			m_titleMap.insert(std::pair<const unsigned int, IFF_STRUCT::sTitle>(title.m_typeId, title));
		}
	}
}

// itemmanager.cpp:1091
void CItemManager::MakeEnchantMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sEnchant enchant = *(IFF_STRUCT::sEnchant *)record;
		record += sizeof(IFF_STRUCT::sEnchant);
		if (enchant.m_active) {
			m_enchantMap.insert(std::pair<const unsigned int, IFF_STRUCT::sEnchant>(enchant.m_typeId, enchant));
		}
	}
}

// itemmanager.cpp:1115
void CItemManager::MakeSkinMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sSkin skin = *(IFF_STRUCT::sSkin *)record;
		record += sizeof(IFF_STRUCT::sSkin);

		// TODO(classdefine.h): the sale start (0x70, a _SYSTEMTIME) and end
		// (0x80) and the shop flags (0x69) are not members of sSkin in its
		// read-only header yet.
		if (((_SYSTEMTIME *)((char *)&skin + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&skin + 0x80))->wYear != 0) {
			*((unsigned char *)&skin + 0x69) &= ~3;
		}

		if (skin.m_active) {
			m_skinMap.insert(std::pair<const unsigned int, IFF_STRUCT::sSkin>(skin.m_typeId, skin));
		}
	}
}

// itemmanager.cpp:1145
void CItemManager::MakeCardMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sCard card = *(IFF_STRUCT::sCard *)record;
		record += sizeof(IFF_STRUCT::sCard);

		// TODO(classdefine.h): sCard does not embed IFF_ITEM_COMMON in its
		// read-only header yet, and neither has the sale start (0x70, a
		// _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)card;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&card + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&card + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (card.m_active) {
			m_cardMap.insert(std::pair<const unsigned int, IFF_STRUCT::sCard>(card.m_typeId, card));
		}
	}
}

// itemmanager.cpp:1181
void CItemManager::MakeFurnitureMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sFurniture furniture = *(IFF_STRUCT::sFurniture *)record;
		record += sizeof(IFF_STRUCT::sFurniture);

		// sFurniture starts with IFF_ITEM_COMMON; its sale start (0x70) and
		// end (0x80) are _SYSTEMTIMEs.
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)furniture;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&furniture + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&furniture + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (furniture.m_active) {
			DevFinalStockPrice(common);
			m_furnitureMap.insert(std::pair<const unsigned int, IFF_STRUCT::sFurniture>(furniture.m_typeId, furniture));
		}
	}
}

// itemmanager.cpp:1218
void CItemManager::MakeOfflineMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sOfflineShop shop = *(IFF_STRUCT::sOfflineShop *)record;
		record += sizeof(IFF_STRUCT::sOfflineShop);

		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)shop;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (shop.m_saleStartYear != 0 || shop.m_saleEndYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (shop.m_active) {
			m_offlineShopMap.insert(std::pair<const unsigned int, IFF_STRUCT::sOfflineShop>(shop.m_typeId, shop));
		}
	}
}

// itemmanager.cpp:1427
void CItemManager::MakeTikiRecipeMap(char *data, int count) {
	IFF_STRUCT::sTikiOutputTable table;
	memset(&table, 0, sizeof(table));

	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		table = *(IFF_STRUCT::sTikiOutputTable *)record;
		record += sizeof(IFF_STRUCT::sTikiOutputTable);

		if (WSingleton<CTikiMagicBoxDoc>::Instance()) {
			WSingleton<CTikiMagicBoxDoc>::Instance()->InsertOutput(table);
		}
	}
}

// itemmanager.cpp:1459
void CItemManager::MakeTikiPointTableMap(char *data, int count) {
	IFF_STRUCT::sTikiPointTable table;
	memset(&table, 0, sizeof(table));

	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		table = *(IFF_STRUCT::sTikiPointTable *)record;
		record += sizeof(IFF_STRUCT::sTikiPointTable);

		if (WSingleton<CTikiMagicBoxDoc>::Instance()) {
			WSingleton<CTikiMagicBoxDoc>::Instance()->InsertPointTable(table);
		}
	}
}

// itemmanager.cpp:1490
void CItemManager::MakeTikiSpecialTableMap(char *data, int count) {
	IFF_STRUCT::sTikiSpecialRecipe recipe;
	memset(&recipe, 0, sizeof(recipe));

	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		recipe = *(IFF_STRUCT::sTikiSpecialRecipe *)record;
		record += sizeof(IFF_STRUCT::sTikiSpecialRecipe);

		if (WSingleton<CTikiMagicBoxDoc>::Instance()) {
			WSingleton<CTikiMagicBoxDoc>::Instance()->InsertSpecialRecipe(recipe);
		}
	}
}

// The key the random box lookups search with.
struct stFindRandomBox {
	stFindRandomBox(unsigned long typeId, unsigned long index);
	bool operator()(const std::pair<unsigned long, _private::sRandomBoxItem> &item) const;

	unsigned long m_typeId;   // 0x0
	unsigned long m_index;    // 0x4
};

// itemmanager.cpp:1576
stFindRandomBox::stFindRandomBox(unsigned long typeId, unsigned long index) {
	m_typeId = typeId;
	m_index = index;
}

// itemmanager.cpp:1580
bool stFindRandomBox::operator()(const std::pair<unsigned long, _private::sRandomBoxItem> &item) const {
	return item.second.m_box.m_itemTypeId == m_typeId && item.second.m_box.m_index == (m_index != 0 ? m_index : item.second.m_box.m_index);
}

// itemmanager.cpp:1682
void CItemManager::MakeNonVisibleItemMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	m_nonVisibleItemMap.clear();

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sNonVisibleItem item = *(IFF_STRUCT::sNonVisibleItem *)record;
		record += sizeof(IFF_STRUCT::sNonVisibleItem);
		if (item.m_active) {
			m_nonVisibleItemMap.insert(std::pair<const unsigned int, IFF_STRUCT::sNonVisibleItem>(item.m_typeId, item));
		}
	}
}

// itemmanager.cpp:1714
void CItemManager::MakeSubscriptionItemMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	m_subscriptionItemMap.clear();

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sSubscriptionItem item = *(IFF_STRUCT::sSubscriptionItem *)record;
		record += sizeof(IFF_STRUCT::sSubscriptionItem);

		// The record's first byte is the active flag, its dword at 0x8 the type id.
		if (*(char *)&item != 0) {
			unsigned int typeId = *(unsigned int *)((char *)&item + 0x8);
			m_subscriptionItemMap.insert(std::pair<const unsigned int, IFF_STRUCT::sSubscriptionItem>(typeId, item));
		}
	}
}

// itemmanager.cpp:1756
void CItemManager::MakeHairStyleMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sHairStyle hairStyle = *(IFF_STRUCT::sHairStyle *)record;
		record += sizeof(IFF_STRUCT::sHairStyle);

		// TODO(classdefine.h): sHairStyle does not embed IFF_ITEM_COMMON in its
		// read-only header yet, and neither has the sale start (0x70, a
		// _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)hairStyle;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&hairStyle + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&hairStyle + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (hairStyle.m_active) {
			m_hairStyleMap.insert(std::pair<const unsigned int, IFF_STRUCT::sHairStyle>(hairStyle.m_typeId, hairStyle));
		}
	}
}

// itemmanager.cpp:1803
void CItemManager::MakeChildItemMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sChildItem child = *(IFF_STRUCT::sChildItem *)record;
		record += sizeof(IFF_STRUCT::sChildItem);

		if (child.m_active) {
			m_childItemMap.insert(std::pair<const unsigned int, IFF_STRUCT::sChildItem>(i, child));
		}
	}
}

// itemmanager.cpp:1827
void CItemManager::MakeAuxPartMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sAuxPart auxPart = *(IFF_STRUCT::sAuxPart *)record;
		record += sizeof(IFF_STRUCT::sAuxPart);

		// TODO(classdefine.h): sAuxPart does not embed IFF_ITEM_COMMON yet, nor
		// the sale start (0x70, a _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)auxPart;
		if (((_SYSTEMTIME *)((char *)&auxPart + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&auxPart + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (auxPart.m_active) {
			DevFinalStockPrice(common);
			m_auxPartMap.insert(std::pair<const unsigned int, IFF_STRUCT::sAuxPart>(auxPart.m_typeId, auxPart));
		}
	}
}

// itemmanager.cpp:1861
void CItemManager::MakeQuestDropMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sQuestDrop drop = *(IFF_STRUCT::sQuestDrop *)record;
		record += sizeof(IFF_STRUCT::sQuestDrop);

		// TODO(classdefine.h): the sale start (0x70, a _SYSTEMTIME) and end
		// (0x80) and the shop flags (0x69) are not members of sQuestDrop in
		// its read-only header yet.
		if (((_SYSTEMTIME *)((char *)&drop + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&drop + 0x80))->wYear != 0) {
			*((unsigned char *)&drop + 0x69) &= ~3;
		}

		if (drop.m_active) {
			m_questDropMap.insert(std::pair<const unsigned int, IFF_STRUCT::sQuestDrop>(drop.m_typeId, drop));
		}
	}
}

// itemmanager.cpp:1894
void CItemManager::MakeQuestMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sQuest quest = *(IFF_STRUCT::sQuest *)record;
		record += sizeof(IFF_STRUCT::sQuest);

		quest.m_clearedA = 0;
		quest.m_clearedB = 0;
		quest.m_extraData = 0;

		if (quest.m_active) {
			m_questMap.insert(std::pair<const unsigned int, IFF_STRUCT::sQuest>(quest.m_typeId, quest));
		}
	}
}

// itemmanager.cpp:1923
void CItemManager::MakeMascotMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sMascot mascot = *(IFF_STRUCT::sMascot *)record;
		record += sizeof(IFF_STRUCT::sMascot);

		// TODO(classdefine.h): sMascot does not embed IFF_ITEM_COMMON in its
		// read-only header yet, and neither has the sale start (0x70, a
		// _SYSTEMTIME) and end (0x80).
		IFF_ITEM_COMMON &common = (IFF_ITEM_COMMON &)mascot;
		if (common.m_discountPrice == 0) {
			common.m_discountPrice = common.m_price;
		}
		if (common.m_refundPrice >= common.m_discountPrice) {
			common.m_refundPrice = 0;
		}

		if (((_SYSTEMTIME *)((char *)&mascot + 0x70))->wYear != 0 || ((_SYSTEMTIME *)((char *)&mascot + 0x80))->wYear != 0) {
			common.m_flags1 &= ~3;
		}

		if (mascot.m_active) {
			DevFinalStockPrice(common);
			m_mascotMap.insert(std::pair<const unsigned int, IFF_STRUCT::sMascot>(mascot.m_typeId, mascot));
		}
	}
}

// itemmanager.cpp:1964
void CItemManager::MakeDescMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sDesc desc = *(IFF_STRUCT::sDesc *)record;
		record += sizeof(IFF_STRUCT::sDesc);
		m_descMap.insert(std::pair<const unsigned int, IFF_STRUCT::sDesc>(desc.m_typeId, desc));
	}
}

// itemmanager.cpp:1986
void CItemManager::MakeFurnitureAbilityList(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sFurnitureAbility ability = *(IFF_STRUCT::sFurnitureAbility *)record;
		record += sizeof(IFF_STRUCT::sFurnitureAbility);

		if (ability.m_active) {
			m_furnitureAbilityList.push_back(ability);
		}
	}
}

// itemmanager.cpp:2021
IFF_STRUCT::sChar *CItemManager::FindChar(unsigned long typeId) {
	IFF_STRUCT::sChar *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sChar>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_charMap.find(typeId);

		if (it != m_charMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_charMap.find(typeId);

		if (it != m_charMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2055
IFF_STRUCT::sPart *CItemManager::FindPart(unsigned long typeId) {
	IFF_STRUCT::sPart *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sPart>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_partMap.find(typeId);

		if (it != m_partMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_partMap.find(typeId);

		if (it != m_partMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2088
IFF_STRUCT::sClub *CItemManager::FindClub(unsigned long typeId) {
	IFF_STRUCT::sClub *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sClub>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_clubMap.find(typeId);

		if (it != m_clubMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_clubMap.find(typeId);

		if (it != m_clubMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2122
IFF_STRUCT::sClubSet *CItemManager::FindClubSet(unsigned long typeId) {
	IFF_STRUCT::sClubSet *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sClubSet>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_clubSetMap.find(typeId);

		if (it != m_clubSetMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_clubSetMap.find(typeId);

		if (it != m_clubSetMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2157
IFF_STRUCT::sBall *CItemManager::FindBall(unsigned long typeId) {
	IFF_STRUCT::sBall *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sBall>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_ballMap.find(typeId);

		if (it != m_ballMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_ballMap.find(typeId);

		if (it != m_ballMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2191
IFF_STRUCT::sItem *CItemManager::FindItem(unsigned long typeId) {
	IFF_STRUCT::sItem *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sItem>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_itemMap.find(typeId);

		if (it != m_itemMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_itemMap.find(typeId);

		if (it != m_itemMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2225
IFF_STRUCT::sCaddie *CItemManager::FindCaddie(unsigned long typeId) {
	IFF_STRUCT::sCaddie *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sCaddie>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_caddieMap.find(typeId);

		if (it != m_caddieMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_caddieMap.find(typeId);

		if (it != m_caddieMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2260
IFF_STRUCT::sCadItem *CItemManager::FindCadItem(unsigned long typeId) {
	IFF_STRUCT::sCadItem *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sCadItem>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_cadItemMap.find(typeId);

		if (it != m_cadItemMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_cadItemMap.find(typeId);

		if (it != m_cadItemMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2294
IFF_STRUCT::sSetItem *CItemManager::FindSetItem(unsigned long typeId) {
	IFF_STRUCT::sSetItem *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sSetItem>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_setItemMap.find(typeId);

		if (it != m_setItemMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_setItemMap.find(typeId);

		if (it != m_setItemMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2394
IFF_STRUCT::sCourse *CItemManager::FindCourse(unsigned long typeId) {
	IFF_STRUCT::sCourse *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sCourse>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_courseMap.find(typeId);

		if (it != m_courseMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_courseMap.find(typeId);

		if (it != m_courseMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2429
IFF_STRUCT::sMatch *CItemManager::FindMatch(unsigned long typeId) {
	IFF_STRUCT::sMatch *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sMatch>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_matchMap.find(typeId);

		if (it != m_matchMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_matchMap.find(typeId);

		if (it != m_matchMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2462
IFF_STRUCT::sTitle *CItemManager::FindTitle(unsigned long typeId) {
	IFF_STRUCT::sTitle *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sTitle>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_titleMap.find(typeId);

		if (it != m_titleMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_titleMap.find(typeId);

		if (it != m_titleMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2496
IFF_STRUCT::sEnchant *CItemManager::FindEnchant(unsigned long typeId) {
	IFF_STRUCT::sEnchant *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sEnchant>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_enchantMap.find(typeId);

		if (it != m_enchantMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_enchantMap.find(typeId);

		if (it != m_enchantMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2530
IFF_STRUCT::sSkin *CItemManager::FindSkin(unsigned long typeId) {
	IFF_STRUCT::sSkin *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sSkin>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_skinMap.find(typeId);

		if (it != m_skinMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_skinMap.find(typeId);

		if (it != m_skinMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2596
IFF_STRUCT::sRandomBox *CItemManager::FindRandomBox(unsigned long typeId) {
	IFF_STRUCT::sRandomBox *result = NULL;
	_private::sRandomBox *box = findRandomBox(typeId);

	if (box != NULL) {
		result = &box->m_box;
	}

	return result;
}

// itemmanager.cpp:2618
void CItemManager::MakeCutinInforMationTableMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::S5::sCutinInformation info = *(IFF_STRUCT::S5::sCutinInformation *)record;
		record += sizeof(IFF_STRUCT::S5::sCutinInformation);
		if (info.m_active) {
			m_cutinInformationMap.insert(std::pair<const unsigned int, IFF_STRUCT::S5::sCutinInformation>(info.m_typeId, info));
		}
	}
}

// itemmanager.cpp:2652
IFF_STRUCT::S5::sCutinInformation *CItemManager::FindCutinInformation(unsigned long typeId) {
	IFF_STRUCT::S5::sCutinInformation *result = NULL;
	std::map<unsigned int, IFF_STRUCT::S5::sCutinInformation>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_cutinInformationMap.find(typeId);

		if (it != m_cutinInformationMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_cutinInformationMap.find(typeId);

		if (it != m_cutinInformationMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2740
IFF_STRUCT::sHairStyle *CItemManager::FindHairStyle(unsigned long typeId) {
	IFF_STRUCT::sHairStyle *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sHairStyle>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_hairStyleMap.find(typeId);

		if (it != m_hairStyleMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_hairStyleMap.find(typeId);

		if (it != m_hairStyleMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2833
IFF_STRUCT::sChildItem *CItemManager::FindChildItem(unsigned long typeId, int index) {
	IFF_STRUCT::sChildItem *child = NULL;
	int count = 0;
	std::map<unsigned int, IFF_STRUCT::sChildItem>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		for (it = m_childItemMap.begin(); it != m_childItemMap.end(); ++it) {
			child = &it->second;

			if (child) {
				if (child->m_typeId == typeId) {
					if (index == count) {
						break;
					}
					count++;
				}
			}
		}
		m_cs.unlock();
	} else {
		for (it = m_childItemMap.begin(); it != m_childItemMap.end(); ++it) {
			child = &it->second;

			if (child) {
				if (child->m_typeId == typeId) {
					if (index == count) {
						return child;
					}
					count++;
				}
			}
		}
	}

	return child;
}

// itemmanager.cpp:2931
IFF_STRUCT::sAuxPart *CItemManager::FindAuxPart(unsigned long typeId) {
	IFF_STRUCT::sAuxPart *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sAuxPart>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_auxPartMap.find(typeId);

		if (it != m_auxPartMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_auxPartMap.find(typeId);

		if (it != m_auxPartMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2965
IFF_STRUCT::sQuestDrop *CItemManager::FindQuestDrop(unsigned long typeId) {
	IFF_STRUCT::sQuestDrop *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sQuestDrop>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_questDropMap.find(typeId);

		if (it != m_questDropMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_questDropMap.find(typeId);

		if (it != m_questDropMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:2999
IFF_STRUCT::sQuest *CItemManager::FindQuest(unsigned long typeId) {
	IFF_STRUCT::sQuest *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sQuest>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_questMap.find(typeId);

		if (it != m_questMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_questMap.find(typeId);

		if (it != m_questMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:3034
IFF_STRUCT::sMascot *CItemManager::FindMascot(unsigned long typeId) {
	std::map<unsigned int, IFF_STRUCT::sMascot>::iterator it;

	it = m_mascotMap.find(typeId);

	if (it != m_mascotMap.end()) {
		return &it->second;
	}

	return NULL;
}

// itemmanager.cpp:3053
IFF_STRUCT::sCard *CItemManager::FindCard(unsigned long typeId) {
	IFF_STRUCT::sCard *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sCard>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_cardMap.find(typeId);

		if (it != m_cardMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_cardMap.find(typeId);

		if (it != m_cardMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:3089
IFF_STRUCT::sFurniture *CItemManager::FindFurniture(unsigned long typeId) {
	if (!IsLocalContent((localContentType_t)0x56)) {
		return NULL;
	}

	IFF_STRUCT::sFurniture *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sFurniture>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_furnitureMap.find(typeId);

		if (it != m_furnitureMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_furnitureMap.find(typeId);

		if (it != m_furnitureMap.end()) {
			result = &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:3167
IFF_STRUCT::sDesc *CItemManager::FindDesc(unsigned long typeId) {
	std::map<unsigned int, IFF_STRUCT::sDesc>::iterator it;

	it = m_descMap.find(typeId);

	if (it == m_descMap.end()) {
		return NULL;
	}

	return &it->second;
}

// itemmanager.cpp:3181
IFF_ITEM_COMMON *CItemManager::FindCommonItem(unsigned long typeId) {
	switch (typeId >> 26) {
	case 1: {
		IFF_STRUCT::sChar *record = FindChar(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 2: {
		IFF_STRUCT::sPart *record = FindPart(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 3: {
		IFF_STRUCT::sClub *record = FindClub(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 4: {
		IFF_STRUCT::sClubSet *record = FindClubSet(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 5: {
		IFF_STRUCT::sBall *record = FindBall(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 6: {
		IFF_STRUCT::sItem *record = FindItem(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 7: {
		IFF_STRUCT::sCaddie *record = FindCaddie(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 8: {
		IFF_STRUCT::sCadItem *record = FindCadItem(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 9: {
		IFF_STRUCT::sSetItem *record = FindSetItem(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 10: {
		IFF_STRUCT::sCourse *record = FindCourse(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 14: {
		IFF_STRUCT::sSkin *record = FindSkin(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 15: {
		IFF_STRUCT::sHairStyle *record = FindHairStyle(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 16: {
		IFF_STRUCT::sMascot *record = FindMascot(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 28: {
		IFF_STRUCT::sAuxPart *record = FindAuxPart(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 29: {
		IFF_STRUCT::sQuestDrop *record = FindQuestDrop(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 31: {
		IFF_STRUCT::sCard *record = FindCard(typeId);
		if (record) {
			return (IFF_ITEM_COMMON *)record;
		}
		break;
	}
	case 18:
		if (IsLocalContent((localContentType_t)0x56)) {
			IFF_STRUCT::sFurniture *record = FindFurniture(typeId);
			if (record) {
				return (IFF_ITEM_COMMON *)record;
			}
		}
		break;
	case 19:
		if (IsLocalContent((localContentType_t)0x66)) {
			IFF_STRUCT::sOfflineShop *record = FindOfflineShop(typeId);
			if (record) {
				return (IFF_ITEM_COMMON *)record;
			}
		}
		break;
	}

	return NULL;
}

// itemmanager.cpp:3470
bool CItemManager::GetDefCombo(unsigned long typeId, unsigned long *combo) {
	if (combo) {
		IFF_STRUCT::sChar *character = FindChar(typeId);

		if (character) {
			for (int i = 0; i < 24; i++) {
				// The default part of slot i: 0x08000400 | (typeId << 18) | (i << 13).
				unsigned long partTypeId = (((typeId << 5) | i) << 13) | 0x8000400;

				combo[i] = FindPart(partTypeId) ? partTypeId : 0;
			}

			return true;
		}
	}

	return false;
}

// itemmanager.cpp:3548
unsigned int CItemManager::GetItemPrice(unsigned long typeId) {
	IFF_ITEM_COMMON *item = FindCommonItem(typeId);

	if (item) {
		return item->m_price;
	}

	return 0xFFFFFFFF;
}

// itemmanager.cpp:3565
int CItemManager::CheckStockType(unsigned long typeId, unsigned int stockType) {
	int result = 0;
	IFF_ITEM_COMMON *item = FindCommonItem(typeId);
	if (item) {
		if ((unsigned int)(item->m_flags0 >> 5) == stockType) {
			result = 1;
		}
	}

	return result;
}

// itemmanager.cpp:3580
bool CItemManager::IsValidBuyItemCount(unsigned long typeId, int count) {
	if ((typeId & 0xFC000000) == 0x18000000) {
		IFF_STRUCT::sItem *item = FindItem(typeId);

		if (item == NULL) {
			return false;
		}
		if ((item->m_flags & 1) && item->quantity > 0 && count != item->quantity) {
			return false;
		}
	}

	return true;
}

// itemmanager.cpp:3868
unsigned int CItemManager::GetChildItemPrice(unsigned long typeId, int index) {
	IFF_STRUCT::sChildItem *child = FindChildItem(typeId, index);

	if (child) {
		if (child->m_hasPrice) {
			return child->m_price;
		} else if (child->m_hasAltPrice) {
			return child->m_altPrice;
		}
	}

	return 0xFFFFFFFF;
}

// itemmanager.cpp:4127
bool CItemManager::IsCashChildItem(unsigned long typeId, int index) {
	IFF_STRUCT::sChildItem *child = FindChildItem(typeId, index);

	if (child) {
		return child->isCash;
	}

	return true;
}

// itemmanager.cpp:4139
bool CItemManager::IsTimeLimit(unsigned long typeId) {
	unsigned long group = typeId >> 26;

	switch (group) {
	case 6: {
		IFF_STRUCT::sItem *item = FindItem(typeId);
		if (item) {
			if (item->m_timeType != 0) {
				return true;
			}
		}
		break;
	}
	case 14: {
		IFF_STRUCT::sSkin *skin = FindSkin(typeId);
		if (skin) {
			if (skin->m_timeType == 1 || skin->m_timeType == 2) {
				return true;
			}
		}
		break;
	}
	}

	return false;
}

// itemmanager.cpp:4195
const char *CItemManager::GetItemName(unsigned long typeId) {
	if (typeId != 0) {
		switch (typeId >> 26) {
		case 1: {
			IFF_STRUCT::sChar *record = FindChar(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 2: {
			IFF_STRUCT::sPart *record = FindPart(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 3: {
			IFF_STRUCT::sClub *record = FindClub(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 4: {
			IFF_STRUCT::sClubSet *record = FindClubSet(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 5: {
			IFF_STRUCT::sBall *record = FindBall(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 6: {
			IFF_STRUCT::sItem *record = FindItem(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 7: {
			IFF_STRUCT::sCaddie *record = FindCaddie(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 8: {
			IFF_STRUCT::sCadItem *record = FindCadItem(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 9: {
			IFF_STRUCT::sSetItem *record = FindSetItem(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 10: {
			IFF_STRUCT::sCourse *record = FindCourse(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 11: {
			IFF_STRUCT::sMatch *record = FindMatch(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 12: {
			IFF_STRUCT::sTitle *record = FindTitle(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 14: {
			IFF_STRUCT::sSkin *record = FindSkin(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 15: {
			IFF_STRUCT::sHairStyle *record = FindHairStyle(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 16: {
			IFF_STRUCT::sMascot *record = FindMascot(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 28: {
			IFF_STRUCT::sAuxPart *record = FindAuxPart(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 29: {
			IFF_STRUCT::sQuestDrop *record = FindQuestDrop(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 30: {
			IFF_STRUCT::sQuest *record = FindQuest(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 31: {
			IFF_STRUCT::sCard *record = FindCard(typeId);
			if (record) {
				return record->m_name;
			}
			break;
		}
		case 18:
			if (IsLocalContent((localContentType_t)0x56)) {
				IFF_STRUCT::sFurniture *record = FindFurniture(typeId);
				if (record) {
					return record->m_name;
				}
			}
			break;
		}
	}

	return "unknown";
}

// itemmanager.cpp:4364
bool CItemManager::HasSalePeriod(unsigned long typeId, _SYSTEMTIME **begin, _SYSTEMTIME **end) {
	*begin = NULL;
	*end = NULL;
	IFF_ITEM_COMMON *item = FindCommonItem(typeId);

	if (item) {
		// The sale start (0x70) and end (0x80) of IFF_ITEM_COMMON are not
		// members of its read-only header yet (client/ProjectG/UccManager.h).
		*begin = (_SYSTEMTIME *)((char *)item + 0x70);
		*end = (_SYSTEMTIME *)((char *)item + 0x80);
	}

	return *begin ? true : false;
}

// itemmanager.cpp:4386
bool CItemManager::IsInSalePeriod(unsigned long typeId, _SYSTEMTIME &time) {
	_SYSTEMTIME *begin;
	_SYSTEMTIME *end;

	if (!HasSalePeriod(typeId, &begin, &end)) {
		return true;
	}

	return IsBetween(begin, end, time);
}

// itemmanager.cpp:4400
int CItemManager::CompareSystemTime(_SYSTEMTIME &a, _SYSTEMTIME &b) {
	if (a.wYear < b.wYear) return -1;
	if (a.wYear > b.wYear) return 1;

	if (a.wMonth < b.wMonth) return -1;
	if (a.wMonth > b.wMonth) return 1;

	if (a.wDay < b.wDay) return -1;
	if (a.wDay > b.wDay) return 1;

	if (a.wHour < b.wHour) return -1;
	if (a.wHour > b.wHour) return 1;

	if (a.wMinute < b.wMinute) return -1;
	if (a.wMinute > b.wMinute) return 1;

	if (a.wSecond < b.wSecond) return -1;
	if (a.wSecond > b.wSecond) return 1;

	if (a.wMilliseconds < b.wMilliseconds) return -1;
	if (a.wMilliseconds > b.wMilliseconds) return 1;

	return 0;
}

// itemmanager.cpp:4427
int CItemManager::GetCouponKind(unsigned long typeId) {
	int kind = 0;

	if (typeId & 0x2000000) {
		switch (typeId & 0x1FFFFFF) {
		case 0x15: case 0x16: case 0x17:
		case 0x1D: case 0x1E:
		case 0x2B: case 0x2C: case 0x2D: case 0x2E:
		case 0x3A: case 0x3C: case 0x4E:
			kind = 1;
			break;
		case 0x28: case 0x29: case 0x2A:
			kind = 5;
			break;
		case 0x30: case 0x33: case 0x34: case 0xA3:
			kind = 2;
			break;
		case 0x36:
		case 0x63: case 0x64: case 0x65: case 0x66: case 0x67: case 0x68: case 0x69:
		case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6E: case 0x6F: case 0x70:
		case 0x71: case 0x72: case 0x73: case 0x74: case 0x75:
		case 0xD2:
			kind = 3;
			break;
		case 0x3D: case 0x3F: case 0x53:
			kind = 4;
			break;
		default:
			kind = 0;
			break;
		}
	}

	return kind;
}

// itemmanager.cpp:4515
bool CItemManager::IsDisableDuplicateItem(unsigned long typeId) {
	if (typeId & 0x2000000) {
		switch (typeId & 0x1FFFFFF) {
		case 0xF:
			return true;
		default:
			return false;
		}
	}

	return false;
}

// itemmanager.cpp:4555
bool CItemManager::IsDisplayItemNumber(unsigned long typeId) {
	bool result = false;
	switch (typeId >> 26) {
	case 6:
		if ((typeId & 0x2000000) && (typeId & 0x1FFFFFF) == 6) {
			break;
		}
		result = true;
		break;
	case 5:
		if (IsUnlimitAztec(typeId)) {
			break;
		}
		result = true;
		break;
	case 0x1C: {
		IFF_STRUCT::sAuxPart *auxPart = FindAuxPart(typeId);
		if (auxPart == NULL || auxPart->quantity == 0) {
			break;
		}
		result = true;
		break;
	}
	case 0x1F:
		result = true;
		break;
	}

	return result;
}

// itemmanager.cpp:4930
unsigned long CItemManager::GetMascotBonusPang(unsigned long typeId, bool flag) {
	unsigned long bonus = 0;

	if ((typeId & 0xFC000000) == 0x40000000) {
		switch (typeId & 0x3FFFFFF) {
		case 0:
		case 1:
		case 2:
		case 3:
			bonus = flag ? 10 : 0;
			break;
		case 6:
		case 14:
		case 18:
			bonus = flag ? 10 : 5;
			break;
		case 4:
		case 13:
		case 16:
		case 19:
			bonus = flag ? 15 : 5;
			break;
		case 9:
		case 10:
		case 11:
		case 12:
			bonus = flag ? 15 : 0;
			break;
		}
	}

	return bonus;
}

// itemmanager.cpp:4987
bool CItemManager::IsBetween(_SYSTEMTIME *begin, _SYSTEMTIME *end, _SYSTEMTIME &time) {
	if (begin && end) {
		if (begin->wYear > 0) {
			if (end->wYear > 0) {
				if (CompareSystemTime(*begin, time) <= 0 && CompareSystemTime(time, *end) <= 0) {
					return true;
				}

				return false;
			}

			return CompareSystemTime(*begin, time) <= 0 ? true : false;
		}

		if (end->wYear > 0) {
			return CompareSystemTime(time, *end) <= 0 ? true : false;
		}
	} else if (begin) {
		return CompareSystemTime(*begin, time) <= 0 ? true : false;
	} else if (end) {
		return CompareSystemTime(time, *end) <= 0 ? true : false;
	}

	return true;
}

// itemmanager.cpp:5267
int CItemManager::GetCourseNum(void) {
	int num;
	if (m_loaded) {
		m_cs.lock();
		num = m_courseMap.size() - 3;
		m_cs.unlock();
	} else {
		num = m_courseMap.size() - 3;
	}

	return num;
}

// itemmanager.cpp:5511
unsigned char CItemManager::GetItemLevel(eLvlType type, const unsigned long *parts, const unsigned long *cards) {
	if (parts == NULL) {
		return 0;
	}

	IFF_STRUCT::sPart *partList[24];
	IFF_STRUCT::sAuxPart *auxPartList[5];
	unsigned char level = 0;

	for (int i = 0; i < 24; i++) {
		if (parts[i] == 0) {
			partList[i] = NULL;
		} else {
			partList[i] = FindPart(parts[i]);
		}

		if (partList[i] != NULL) {
			level += partList[i]->m_baseStat[type];
		}
	}

	if (cards != NULL && type < 5) {
		for (int i = 0; i < 5; i++) {
			if (cards[i] == 0) {
				auxPartList[i] = NULL;
			} else {
				auxPartList[i] = FindAuxPart(cards[i]);
			}

			if (auxPartList[i] != NULL && auxPartList[i]->m_levels[type] != 99) {
				level += auxPartList[i]->m_levels[type];
			}
		}
	}

	return level;
}

// itemmanager.cpp:5559
unsigned char CItemManager::GetCharCapacity(eLvlType type, unsigned long charTypeId, const unsigned long *parts, const unsigned long *cards, unsigned char extra) {
	IFF_STRUCT::sPart *partRecords[24];
	IFF_STRUCT::sAuxPart *cardRecords[5];
	unsigned char capacity;

	if (charTypeId != 0) {
		IFF_STRUCT::sChar *character = FindChar(charTypeId);

		if (character) {
			capacity = (unsigned char)character->m_capacity[type];
		} else {
			capacity = 0;
		}
	} else {
		capacity = 0;
	}

	if (type == 0 && extra >= 6) {
		capacity += (extra - 1) / 5;
	}

	if (parts) {
		for (int i = 0; i < 24; i++) {
			if (parts[i] == 0) {
				partRecords[i] = NULL;
			} else {
				partRecords[i] = FindPart(parts[i]);
			}

			if (partRecords[i]) {
				capacity += (unsigned char)partRecords[i]->m_slotStat[type];
			}
		}
	}

	if (cards && type < 5) {
		for (int i = 0; i < 5; i++) {
			if (cards[i] == 0) {
				cardRecords[i] = NULL;
			} else {
				cardRecords[i] = FindAuxPart(cards[i]);
			}

			if (cardRecords[i]) {
				unsigned char value = cardRecords[i]->m_capacity[type];

				if (value != 99) {
					capacity += value;
				}
			}
		}
	}

	if (IsLocalContent((localContentType_t)0x51)) {
		capacity += WSingleton<CCardManager>::Instance()->GetCardStatusSlot(type);
	}

	return capacity;
}

// itemmanager.cpp:5659
unsigned char CItemManager::GetCapacity(eLvlType type, unsigned long charTypeId, const char *levels, const unsigned long *parts, unsigned long clubTypeId, const short *clubLevels, unsigned long ballTypeId, unsigned char extra) {
	IFF_STRUCT::sPart *partRecords[24];
	unsigned char capacity;

	IFF_STRUCT::sChar *character = FindChar(charTypeId);
	IFF_STRUCT::sClubSet *clubSet = FindClubSet(clubTypeId);

	if (character) {
		if (type == 0) {
			capacity = (unsigned char)character->m_capacity[0];

			if (extra >= 6) {
				capacity += (extra - 1) / 5;
			}
		} else if (type > 0 && type <= 4) {
			capacity = (unsigned char)character->m_capacity[type];
		}
	} else {
		capacity = 0;
	}

	if (parts) {
		for (int i = 0; i < 24; i++) {
			if (parts[i] == 0) {
				partRecords[i] = NULL;
			} else {
				partRecords[i] = FindPart(parts[i]);
			}

			if (partRecords[i]) {
				capacity += (unsigned char)(partRecords[i]->m_slotStat[type] + partRecords[i]->m_baseStat[type]);
			}
		}
	}

	if (clubSet) {
		capacity += (unsigned char)clubSet->m_maxStat[type];
	}

	if ((ballTypeId & 0xFC000000) == 0x20000000) {
		ballTypeId = ((ballTypeId >> 21) & 0x1F) | 0x1C000000;
	}

	IFF_STRUCT::sCaddie *caddie = FindCaddie(ballTypeId);
	unsigned char caddieValue;

	if (caddie && (caddie->m_level & 0x7F) <= extra) {
		caddieValue = (unsigned char)caddie->m_levels[type];
	} else {
		caddieValue = 0;
	}
	capacity += caddieValue;

	if (IsLocalContent((localContentType_t)0x51)) {
		capacity += WSingleton<CCardManager>::Instance()->GetCardPeriodStatus(type);
		capacity += WSingleton<CCardManager>::Instance()->GetCardStatusSlot(type);
	}

	return capacity;
}

// itemmanager.cpp:5636
unsigned char CItemManager::GetCharLevel(eLvlType type, unsigned long charTypeId, const char *levels) {
	if (charTypeId == 0)
		return 0;

	IFF_STRUCT::sChar *chr = FindChar(charTypeId);

	if (chr == 0)
		return 0;

	unsigned char level = chr->m_baseLevels[type];
	if (levels)
		level += levels[type];

	return level;
}

// itemmanager.cpp:5935
int CalcPowerPenalty(int power) {
	if (power >= 66) {
		return 13;
	} else if (power >= 61) {
		return 12;
	} else if (power >= 56) {
		return 11;
	} else if (power >= 51) {
		return 10;
	} else if (power >= 46) {
		return 9;
	} else if (power >= 41) {
		return 8;
	} else if (power >= 36) {
		return 7;
	} else if (power >= 31) {
		return 6;
	} else if (power >= 26) {
		return 5;
	} else if (power >= 21) {
		return 4;
	} else if (power >= 16) {
		return 3;
	} else if (power >= 11) {
		return 2;
	} else if (power >= 6) {
		return 1;
	}

	return 0;
}

// itemmanager.cpp:5966
status_t CItemManager::GetLevelWithPenalty(eLvlType type, unsigned long charTypeId, const char *levels, const unsigned long *parts, const unsigned long *cards, unsigned long clubTypeId, const short *clubLevels, unsigned long ballTypeId, unsigned char extra) {
	unsigned char penalty = 0;
	unsigned char level = GetLevel(type, charTypeId, levels, parts, cards, clubTypeId, clubLevels, ballTypeId, extra);

	if (type == 1 || type == 2) {
		int base = GetLevel((eLvlType)0, charTypeId, levels, parts, cards, clubTypeId, clubLevels, ballTypeId, extra);
		int excess = base - CalcPowerPenalty(extra) - 20;
		penalty = excess > 0 ? excess : 0;

		if (level <= penalty) {
			level = 0;
		} else {
			level -= penalty;
		}
	}

	status_t status;
	status.m_level = level;
	status.m_penalty = penalty;
	return status;
}

// itemmanager.cpp:5998
bool CItemManager::isTrade(unsigned long typeId) {
	IFF_ITEM_COMMON *item = FindCommonItem(typeId);
	if (!item) {
		return false;
	}

	return (item->m_flags0 & 0x1E) == 2 ? true : false;
}

// itemmanager.cpp:6007
bool CItemManager::IsCanOverlapped(unsigned long typeId, unsigned char a, unsigned char b) {
	bool result = false;

	IFF_ITEM_COMMON *item = FindCommonItem(typeId);
	if (!item)
		return false;

	switch (typeId >> 26) {
	case 1:
		if (b == 1)
			result = true;
		break;
	case 7:
		if (a == 1) {
			if (typeId == 0x1C000001 || typeId == 0x1C000002 || typeId == 0x1C000003 || typeId == 0x1C000007)
				result = true;
		}
		break;
	case 2: {
		unsigned char kind = (item->m_flags0 >> 1) & 0xF;
		if (kind == 1 || kind == 3)
			result = true;
		else
			result = false;
		break;
	}
	case 4:
	case 8:
	case 0xE:
	case 0xF:
		result = false;
		break;
	case 9: {
		unsigned long group = (typeId >> 21) & 0x1F;
		if (group == 6 || group == 5)
			result = true;
		else
			result = false;
		break;
	}
	case 0x1C:
		if (!(typeId & 0x1F0000))
			result = true;
		break;
	default:
		result = true;
		break;
	}

	return result;
}

// itemmanager.cpp:6084
bool CItemManager::IsPartAttachCard(unsigned long cardTypeId, unsigned long partTypeId, int slot) {
	int cardKind = (cardTypeId >> 22) & 0xF;
	bool result = false;
	IFF_STRUCT::sPart *part = FindPart(partTypeId);

	if (part) {
		if (cardKind == 0) {
			if (part->m_charCardSlots >= slot && slot > 0 && slot < 5)
				result = true;
		} else if (cardKind == 1) {
			slot -= 4;
			if (part->m_caddieCardSlots >= slot && slot > 0 && slot < 5)
				result = true;
		}
	}

	return result;
}

// itemmanager.cpp:6108
bool CItemManager::GetCardSlotNum(unsigned long typeId, unsigned short &charSlots, unsigned short &caddieSlots) {
	charSlots = 0;
	caddieSlots = 0;

	switch (typeId >> 26) {
	case 2: {
		IFF_STRUCT::sPart *part = FindPart(typeId);
		if (part) {
			charSlots = part->m_charCardSlots;
			caddieSlots = part->m_caddieCardSlots;
			return true;
		}
		break;
	}
	}

	return false;
}

// itemmanager.cpp:6137
bool CItemManager::IsEquipCard(unsigned long typeId) {
	if ((typeId & 0xFC000000) != 0x7C000000) {
		return false;
	}

	if ((typeId & 0x3C00000) <= 0x400000) {
		return true;
	}

	return false;
}

// itemmanager.cpp:6155
COUNTING_TYPE CItemManager::GetItemCountingType(unsigned long typeId, unsigned char kind) {
	COUNTING_TYPE type = (COUNTING_TYPE)0;

	switch (typeId >> 26) {
	case 2:
	case 4:
		type = (COUNTING_TYPE)((kind != 0) + 1);
		break;
	case 5:
	case 6:
	case 0x1C:
		type = (COUNTING_TYPE)3;
		break;
	case 7:
		type = (COUNTING_TYPE)4;
		break;
	case 0x1F:
		if (IsLocalContent((localContentType_t)0x51))
			type = (COUNTING_TYPE)5;
		break;
	}

	return type;
}

// itemmanager.cpp:6363
void CItemManager::MakeSpecialPrizeItemMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sSpecialPrizeItem item = *(IFF_STRUCT::sSpecialPrizeItem *)record;
		record += sizeof(IFF_STRUCT::sSpecialPrizeItem);
		m_specialPrizeItemMap[item.m_type].insert(std::pair<const unsigned int, IFF_STRUCT::sSpecialPrizeItem>(item.m_typeId, item));
	}
}

// itemmanager.cpp:6385
IFF_STRUCT::sSpecialPrizeItem *CItemManager::FindSPItem(unsigned long typeId, eSPAVILITYTYPE type) {
	IFF_STRUCT::sSpecialPrizeItem *result = NULL;
	std::map<unsigned int, IFF_STRUCT::sSpecialPrizeItem>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_specialPrizeItemMap[type].find(typeId);

		if (it != m_specialPrizeItemMap[type].end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_specialPrizeItemMap[type].find(typeId);

		if (it != m_specialPrizeItemMap[type].end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:6419
void CItemManager::MakeItemBuffMap(void) {
	sItemBuffHelper helper;
	std::map<unsigned long, sItemBuffHelper>::iterator it;

	m_itemBuffMap.clear();

	m_itemBuffMap.insert(std::make_pair(0x1A0000B3, helper));
	it = m_itemBuffMap.find(0x1A0000B3);
	if (it != m_itemBuffMap.end()) {
		it->second.m_value2 = 1;
		it->second.m_ability = (specialcardavility)2;
		it->second.m_value1 = 120;
		it->second.m_value0 = 5;
		std::set<unsigned long> &compatible = it->second.m_compatible;
		compatible.insert(0x1A0000B4);
	}

	m_itemBuffMap.insert(std::make_pair(0x1A0000B4, helper));
	it = m_itemBuffMap.find(0x1A0000B4);
	if (it != m_itemBuffMap.end()) {
		it->second.m_value2 = 1;
		it->second.m_ability = (specialcardavility)2;
		it->second.m_value1 = 120;
		it->second.m_value0 = 50;
		std::set<unsigned long> &compatible = it->second.m_compatible;
		compatible.insert(0x1A0000B3);
	}
}

// itemmanager.cpp:6575
bool CItemManager::IsItemBuff(unsigned long typeId) {
	std::map<unsigned long, sItemBuffHelper>::iterator it = m_itemBuffMap.find(typeId);
	if (it != m_itemBuffMap.end()) {
		return true;
	}

	return false;
}

// itemmanager.cpp:6584
bool CItemManager::IsCompatibleBuff(unsigned long typeId, unsigned long otherTypeId) {
	std::map<unsigned long, sItemBuffHelper>::iterator it = m_itemBuffMap.find(typeId);
	if (it == m_itemBuffMap.end()) {
		return false;
	}

	const std::set<unsigned long> &compatible = it->second.m_compatible;
	std::set<unsigned long>::const_iterator found = compatible.find(otherTypeId);
	return found == compatible.end() ? true : false;
}

// itemmanager.cpp:6601
bool CItemManager::GetBuffProperty(unsigned long typeId, specialcardavility &ability, int &value0, int &value1, int &value2) {
	std::map<unsigned long, sItemBuffHelper>::iterator it = m_itemBuffMap.find(typeId);
	if (it == m_itemBuffMap.end()) {
		return false;
	}

	ability = it->second.m_ability;
	value0 = it->second.m_value0;
	value1 = it->second.m_value1;
	value2 = it->second.m_value2;
	return true;
}

// itemmanager.cpp:6621
bool S5::ISCutinItem(unsigned long typeId) {
	if ((typeId & 0xFC000000) == 0x38000000 && (typeId & 0x3C00000) == 0x1400000) {
		return true;
	}

	return false;
}

// itemmanager.cpp:6632
const char *CItemManager::GetIconName(unsigned long typeId) {
	if ((typeId & 0xFC000000) != 0x2C000000) {
		IFF_ITEM_COMMON *item = FindCommonItem(typeId);
		if (item)
			return item->m_icon;
	} else {
		IFF_STRUCT::sMatch *match = FindMatch(typeId);
		if (match)
			return match->m_icon;
	}

	return "";
}

// itemmanager.cpp:6652
bool CItemManager::IsUnlimitAztec(unsigned long typeId) {
	if ((typeId & 0xFC000000) == 0x14000000) {
		IFF_STRUCT::sBall *ball = FindBall(typeId);
		if (ball && ball->quantity == 0) {
			return true;
		}
	}

	return false;
}

// itemmanager.cpp:6668
bool CItemManager::IsBasicAztecFamily(unsigned long typeId) {
	if ((typeId & 0xFC000000) == 0x14000000) {
		if (typeId == (typeId & 0xFC000000))
			return true;

		IFF_STRUCT::sBall *ball = FindBall(typeId);
		if (ball && ball->m_price == 0 && ball->quantity == 0)
			return true;
	}

	return false;
}

// itemmanager.cpp:6686
const char *CItemManager::ChangePartsPosmask(unsigned long typeId, unsigned long posmask) {
	IFF_STRUCT::sPart *part = FindPart(typeId);
	if (part) {
		m_cs.lock();

		part->hideMask = posmask;

		m_cs.unlock();
		return "The posmask is changed";
	}

	return "No exist";
}

// itemmanager.cpp:6700
const char *CItemManager::ChangePartsPetName(unsigned long typeId, const char *name) {
	IFF_STRUCT::sPart *part = FindPart(typeId);
	if (part) {
		m_cs.lock();

		char oldName[256];
		strcpy(oldName, part->model);
		strcpy(part->model, name);

		m_cs.unlock();
		return "The petname is changed";
	}

	return "No exist";
}

// itemmanager.cpp:6717
const char *CItemManager::ChangePartsTexName(unsigned long typeId, unsigned long texType, unsigned long index, const char *name) {
	if (texType != 0 && texType != 1) {
		return "Invalid texture type.";
	}

	if (index <= 2) {
		IFF_STRUCT::sPart *part = FindPart(typeId);
		if (part) {
			m_cs.lock();

			if (texType == 0) {
				strcpy(part->texture[index], name);
			} else if (texType == 1) {
				strcpy(part->texture2[index], name);
			}

			m_cs.unlock();
			return "The texname is changed";
		}

		return "No exist";
	}

	return "Invalid texture.";
}

// itemmanager.cpp:6748
_private::sRandomBox *CItemManager::findRandomBox(unsigned long typeId) {
	_private::sRandomBox *result = NULL;
	std::map<unsigned int, _private::sRandomBox>::iterator it;

	if (m_loaded) {
		m_cs.lock();

		it = m_randomBoxMap.find(typeId);

		if (it != m_randomBoxMap.end()) {
			result = &it->second;
		}
		m_cs.unlock();
	} else {
		it = m_randomBoxMap.find(typeId);

		if (it != m_randomBoxMap.end()) {
			return &it->second;
		}
	}

	return result;
}

// itemmanager.cpp:6777
IFF_STRUCT::sRandomBox *CItemManager::GetRandomBoxInfo(unsigned long typeId, unsigned long kind, unsigned long itemTypeId) {
	_private::sRandomBox *box = findRandomBox(typeId);

	if (box == NULL) {
		return NULL;
	}

	std::map<unsigned long, _private::sRandomBoxItem>::iterator it = box->m_items.find(itemTypeId);
	if (it == box->m_items.end()) {
		return NULL;
	}

	return &it->second.m_box;
}

// itemmanager.cpp:6796
bool CItemManager::GetEnumRandomBoxStuff(unsigned long typeId, std::vector<IFF_STRUCT::sRandomBox> &stuffs) {
	_private::sRandomBox *box = findRandomBox(typeId);
	if (box == NULL) {
		return false;
	}

	std::vector<_private::sRandomBoxStuff>::iterator it = box->m_stuffs.begin();
	std::vector<_private::sRandomBoxStuff>::iterator end = box->m_stuffs.end();
	for (; it != end; ++it) {
		stuffs.push_back(*it);
	}

	return true;
}

// itemmanager.cpp:6867
bool CItemManager::IsNonVisibleItem(unsigned long typeId) {
	std::map<unsigned int, IFF_STRUCT::sSubscriptionItem>::iterator coupon;
	std::map<unsigned int, IFF_STRUCT::sNonVisibleItem>::iterator it;

	coupon = m_subscriptionItemMap.find(typeId);
	if (coupon != m_subscriptionItemMap.end() && coupon->second.isCoupon) {
		return true;
	}

	it = m_nonVisibleItemMap.find(typeId);
	if (it == m_nonVisibleItemMap.end()) {
		return false;
	}

	return it->second.isNonVisible;
}

// itemmanager.cpp:6891
bool CItemManager::IsSubScriptionCoupon(unsigned long typeId) {
	std::map<unsigned int, IFF_STRUCT::sSubscriptionItem>::iterator it;

	it = m_subscriptionItemMap.find(typeId);
	if (it == m_subscriptionItemMap.end()) {
		return false;
	}

	return it->second.isCoupon;
}

// itemmanager.cpp:1253
void CItemManager::MakeCadieMagicBoxMap(char *data, int count) {
	sIffHeader header;
	IFF_STRUCT::sCadieMagicBox record;

	memcpy(&header, data, sizeof(sIffHeader));
	char *p = data + sizeof(sIffHeader);

	m_cadieMagicBoxList.clear();

	for (int i = 0; i < header.m_count; i++) {
		memcpy(&record, p, sizeof(IFF_STRUCT::sCadieMagicBox));
		p += sizeof(IFF_STRUCT::sCadieMagicBox);

		m_cadieMagicBoxList.push_back(record);

		switch (record.m_category) {
		case 0:
			g_NumLowRecycleItem++;
			break;
		case 1:
			g_NumMidRecycleItem++;
			break;
		case 2:
			g_NumHighRecycleItem++;
			break;
		case 3:
			g_NumSpecialRecycleItem++;
			break;
		case 4:
			g_NumEventRecycleItem++;
			break;
		}

		g_NumIndexRecycleItem++;
	}

	if (g_ArrayTidMixItem) {
		for (int i = 0; i < 4; i++) {
			if (g_ArrayTidMixItem[i]) {
				delete[] g_ArrayTidMixItem[i];
				g_ArrayTidMixItem[i] = NULL;
			}
		}

		delete[] g_ArrayTidMixItem;
		g_ArrayTidMixItem = NULL;
	}

	if (g_ArrayRecycleItem) {
		delete[] g_ArrayRecycleItem;
		g_ArrayRecycleItem = NULL;
	}

	g_ArrayTidMixItem = new sRecycleItem*[g_NumIndexRecycleItem];
	if (g_ArrayTidMixItem) {
		for (int i = 0; i < g_NumIndexRecycleItem; i++) {
			g_ArrayTidMixItem[i] = new sRecycleItem[4];
		}
	}

	g_ArrayRecycleItem = new sRecycleItem[g_NumIndexRecycleItem];

	std::list<IFF_STRUCT::sCadieMagicBox>::iterator it = m_cadieMagicBoxList.begin();
	std::list<IFF_STRUCT::sCadieMagicBox>::iterator end = m_cadieMagicBoxList.end();
	for (; it != end; ++it) {
		memset(&g_ArrayRecycleItem[it->m_number - 1], 0, sizeof(sRecycleItem));

		g_ArrayRecycleItem[it->m_number - 1].tid = it->m_output;
		g_ArrayRecycleItem[it->m_number - 1].count = (unsigned short)it->m_outputCount;
		g_ArrayRecycleItem[it->m_number - 1].level = (unsigned char)it->m_level;
		g_ArrayRecycleItem[it->m_number - 1].character = (short)it->m_character;

		if (it->m_randSeq > 0) {
			g_ArrayRecycleItem[it->m_number - 1].singleRecycle = it->m_randSeq;
			strncpy(g_ArrayRecycleItem[it->m_number - 1].randName, it->m_randName, 39);
			g_ArrayRecycleItem[it->m_number - 1].randName[39] = 0;
		}

		for (int j = 0; j < 4; j++) {
			memset(&g_ArrayTidMixItem[it->m_number - 1][j], 0, sizeof(sRecycleItem));
			g_ArrayTidMixItem[it->m_number - 1][j].tid = it->m_element[j];
			g_ArrayTidMixItem[it->m_number - 1][j].count = (unsigned short)it->m_elementCount[j];
			g_ArrayTidMixItem[it->m_number - 1][j].level = (unsigned char)it->m_level;
		}
	}

	m_cadieMagicBoxList.clear();
}

// itemmanager.cpp:1368
void CItemManager::MakeRandomRecycleMap(char *data, int count) {
	sIffHeader header = *(sIffHeader *)data;
	char *record = data + sizeof(sIffHeader);

	std::set<unsigned long> seqNumberSet;
	for (int i = 0; i < header.m_count; i++) {
		IFF_STRUCT::sRandomRecycle recycle = *(IFF_STRUCT::sRandomRecycle *)record;
		record += sizeof(IFF_STRUCT::sRandomRecycle);

		if (seqNumberSet.find(recycle.m_recycleId) == seqNumberSet.end()) {
			seqNumberSet.insert(recycle.m_recycleId);
		}

		m_randomRecycleMap.insert(std::pair<const unsigned int, IFF_STRUCT::sRandomRecycle>(recycle.m_recycleId, recycle));
	}

	for (std::set<unsigned long>::const_iterator it = seqNumberSet.begin(); it != seqNumberSet.end(); ++it) {
		std::multimap<unsigned int, IFF_STRUCT::sRandomRecycle>::const_iterator lower = m_randomRecycleMap.lower_bound(*it);
		std::multimap<unsigned int, IFF_STRUCT::sRandomRecycle>::const_iterator upper = m_randomRecycleMap.upper_bound(*it);
		while (lower != upper) {
			++lower;
		}
	}
}
