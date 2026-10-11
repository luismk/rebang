// Original: d:\build\custom\temp\code\shared\itemmanager.h
#ifndef PG_SHARED_ITEMMANAGER_H
#define PG_SHARED_ITEMMANAGER_H

#include <windows.h>
#include <string.h>
#include <list>
#include <map>
#include <set>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "../client/ProjectG/lock.hpp"
#include "classdefine.h"

// The IFF item records the manager returns. Their definitions live with
// IFF_STRUCT (classdefine.h); only what the code built so far uses is
// transcribed there.
namespace IFF_STRUCT {
struct sChar;
struct sPart;
struct sClub;
struct sClubSet;
struct sBall;
struct sItem;
struct sCaddie;
struct sCadItem;
struct sSetItem;
struct sCourse;
struct sMatch;
struct sTitle;
struct sEnchant;
struct sSkin;
struct sHairStyle;
struct sChildItem;
struct sAuxPart;
struct sQuestDrop;
struct sQuest;
struct sMascot;
struct sCard;
struct sFurniture;
struct sOfflineShop;
struct sDesc;
struct sRandomBox;
struct sSpecialPrizeItem;
struct sRandomRecycle;
struct sCadieMagicBox;
struct sFurnitureAbility;
struct sTikiPointTable;
struct sTikiSpecialRecipe;
struct sNonVisibleItem;
struct sSubscriptionItem;
namespace S5 {
struct sCutinInformation;
}
}

namespace _private {

// One item of a random box with the records that can replace it.
struct sRandomBoxItem {
	// itemmanager.h:145
	sRandomBoxItem() {
		memset(&m_box, 0, sizeof(IFF_STRUCT::sRandomBox));
		m_items.clear();
	}

	// itemmanager.h:151
	~sRandomBoxItem() {
		m_items.clear();
	}

	IFF_STRUCT::sRandomBox m_box;               // 0x0
	std::vector<IFF_STRUCT::sRandomBox> m_items; // 0xE4
};
CHECK_SIZE(sRandomBoxItem, 0xF4);
CHECK_FIELD(sRandomBoxItem, m_box, 0x0);
CHECK_FIELD(sRandomBoxItem, m_items, 0xE4);

// A random box stuff entry: a random box record (GetEnumRandomBoxStuff
// copies it out as one).
struct sRandomBoxStuff : IFF_STRUCT::sRandomBox {
	// itemmanager.h:160
	sRandomBoxStuff() {
		memset(this, 0, sizeof(sRandomBoxStuff));
	}
};
CHECK_SIZE(sRandomBoxStuff, 0xE4);

// A random box: its record, its items by type id and its stuff.
struct sRandomBox {
	// itemmanager.h:172
	sRandomBox() {
		memset(&m_box, 0, sizeof(IFF_STRUCT::sRandomBox));
		m_items.clear();
		m_stuffs.clear();
	}

	// itemmanager.h:179
	~sRandomBox() {
		m_items.clear();
		m_stuffs.clear();
	}

	IFF_STRUCT::sRandomBox m_box;                           // 0x0
	std::map<unsigned long, sRandomBoxItem> m_items;        // 0xE4
	std::vector<sRandomBoxStuff> m_stuffs;                  // 0xF0
};
CHECK_SIZE(sRandomBox, 0x100);
CHECK_FIELD(sRandomBox, m_box, 0x0);
CHECK_FIELD(sRandomBox, m_items, 0xE4);
CHECK_FIELD(sRandomBox, m_stuffs, 0xF0);

} // namespace _private

struct IFF_ITEM_COMMON;

// The level GetLevelWithPenalty returns with the power penalty it took off.
struct status_t {
	unsigned char m_level;   // 0x0
	unsigned char m_penalty; // 0x1
};
CHECK_SIZE(status_t, 0x2);
CHECK_FIELD(status_t, m_level, 0x0);
CHECK_FIELD(status_t, m_penalty, 0x1);

enum COUNTING_TYPE;
enum PANGYA_ITEM_GROUP;
enum eLvlType;
enum eSPAVILITYTYPE;
enum specialcardavility;

// The item database: the IFF tables loaded into maps keyed by type id.
// Only the members the functions built so far use are named.
class CItemManager {
public:
	// The buff properties of an item (MakeItemBuffMap, GetBuffProperty).
	struct sItemBuffHelper {
		specialcardavility m_ability;       // 0x0, the GetBuffProperty values
		int m_value0;                       // 0x4
		int m_value1;                       // 0x8
		int m_value2;                       // 0xC
		std::set<unsigned long> m_compatible; // 0x10
	};

	CItemManager(void);
	virtual ~CItemManager(void);

	int CompareSystemTime(_SYSTEMTIME &a, _SYSTEMTIME &b);
	int GetCouponKind(unsigned long typeId);
	bool IsDisableDuplicateItem(unsigned long typeId);
	unsigned long GetIndexAbilityItem(unsigned long typeId);
	unsigned long GetMascotBonusPang(unsigned long typeId, bool flag);
	bool IsBetween(_SYSTEMTIME *begin, _SYSTEMTIME *end, _SYSTEMTIME &time);
	bool IsEquipCard(unsigned long typeId);
	COUNTING_TYPE GetItemCountingType(unsigned long typeId, unsigned char kind);
	int GetNumItems(PANGYA_ITEM_GROUP group);
	int GetCourseNum(void);
	void Reset(void);
	IFF_STRUCT::sChar *FindChar(unsigned long typeId);
	IFF_STRUCT::sPart *FindPart(unsigned long typeId);
	IFF_STRUCT::sClub *FindClub(unsigned long typeId);
	IFF_STRUCT::sClubSet *FindClubSet(unsigned long typeId);
	IFF_STRUCT::sBall *FindBall(unsigned long typeId);
	IFF_STRUCT::sItem *FindItem(unsigned long typeId);
	IFF_STRUCT::sCaddie *FindCaddie(unsigned long typeId);
	IFF_STRUCT::sCadItem *FindCadItem(unsigned long typeId);
	IFF_STRUCT::sSetItem *FindSetItem(unsigned long typeId);
	IFF_STRUCT::sSetItem *FindSetItemAtConstruct(unsigned long typeId);
	IFF_STRUCT::sCourse *FindCourse(unsigned long typeId);
	IFF_STRUCT::sMatch *FindMatch(unsigned long typeId);
	IFF_STRUCT::sTitle *FindTitle(unsigned long typeId);
	IFF_STRUCT::sEnchant *FindEnchant(unsigned long typeId);
	IFF_STRUCT::sSkin *FindSkin(unsigned long typeId);
	IFF_STRUCT::S5::sCutinInformation *FindCutinInformation(unsigned long typeId);
	IFF_STRUCT::sHairStyle *FindHairStyle(unsigned long typeId);
	IFF_STRUCT::sHairStyle *FindHairStyle(unsigned char style, unsigned char color);
	IFF_STRUCT::sChildItem *FindChildItem(unsigned long typeId, int index);
	int FindChildItemNumber(unsigned long typeId);
	IFF_STRUCT::sAuxPart *FindAuxPart(unsigned long typeId);
	IFF_STRUCT::sQuestDrop *FindQuestDrop(unsigned long typeId);
	IFF_STRUCT::sQuest *FindQuest(unsigned long typeId);
	IFF_STRUCT::sMascot *FindMascot(unsigned long typeId);
	IFF_STRUCT::sCard *FindCard(unsigned long typeId);
	IFF_STRUCT::sFurniture *FindFurniture(unsigned long typeId);
	IFF_STRUCT::sOfflineShop *FindOfflineShop(unsigned long typeId);
	IFF_STRUCT::sDesc *FindDesc(unsigned long typeId);
	IFF_ITEM_COMMON *FindCommonItem(unsigned long typeId);
	bool GetSetItemElem(IFF_STRUCT::sSetItem *setItem, int index, unsigned long &typeId, unsigned long &count, unsigned long &extra);
	bool GetDefCombo(unsigned long typeId, unsigned long *combo);
	unsigned int GetItemPrice(unsigned long typeId);
	int CheckStockType(unsigned long typeId, unsigned int count);
	bool IsValidBuyItemCount(unsigned long typeId, int count);
	unsigned int GetItemSalePrice(unsigned long typeId, int count, unsigned long option);
	unsigned int GetChildItemPrice(unsigned long typeId, int index);
	bool IsCashItem(unsigned long typeId);
	bool IsCashChildItem(unsigned long typeId, int index);
	bool IsTimeLimit(unsigned long typeId);
	const char *GetItemName(unsigned long typeId);
	bool HasSalePeriod(unsigned long typeId, _SYSTEMTIME **begin, _SYSTEMTIME **end);
	bool IsInSalePeriod(unsigned long typeId, _SYSTEMTIME &time);
	unsigned char GetItemLevel(eLvlType type, const unsigned long *parts, const unsigned long *cards);
	unsigned char GetCharCapacity(eLvlType type, unsigned long charTypeId, const unsigned long *parts, const unsigned long *cards, unsigned char extra);
	unsigned char GetCharLevel(eLvlType type, unsigned long charTypeId, const char *levels);
	unsigned char GetCapacity(eLvlType type, unsigned long charTypeId, const char *levels, const unsigned long *parts, unsigned long clubTypeId, const short *clubLevels, unsigned long ballTypeId, unsigned char extra);
	unsigned char GetLevel(eLvlType type, unsigned long charTypeId, const char *levels, const unsigned long *parts, const unsigned long *cards, unsigned long clubTypeId, const short *clubLevels, unsigned long ballTypeId, unsigned char extra);
	status_t GetLevelWithPenalty(eLvlType type, unsigned long charTypeId, const char *levels, const unsigned long *parts, const unsigned long *cards, unsigned long clubTypeId, const short *clubLevels, unsigned long ballTypeId, unsigned char extra);
	bool isTrade(unsigned long typeId);
	bool IsCanOverlapped(unsigned long typeId, unsigned char a, unsigned char b);
	bool IsPartAttachCard(unsigned long partTypeId, unsigned long cardTypeId, int slot);
	bool GetCardSlotNum(unsigned long typeId, unsigned short &first, unsigned short &count);
	IFF_STRUCT::sSpecialPrizeItem *FindSPItem(unsigned long typeId, eSPAVILITYTYPE type);
	bool IsItemBuff(unsigned long typeId);
	bool IsCompatibleBuff(unsigned long typeId, unsigned long otherTypeId);
	bool GetBuffProperty(unsigned long typeId, specialcardavility &ability, int &value0, int &value1, int &value2);
	const char *GetIconName(unsigned long typeId);
	bool IsUnlimitAztec(unsigned long typeId);
	bool IsBasicAztecFamily(unsigned long typeId);
	const char *ChangePartsPosmask(unsigned long typeId, unsigned long posmask);
	const char *ChangePartsPetName(unsigned long typeId, const char *name);
	const char *ChangePartsTexName(unsigned long typeId, unsigned long a, unsigned long b, const char *name);
	bool IsSubScriptionCoupon(unsigned long typeId);
	IFF_STRUCT::sRandomBox *FindRandomBox(unsigned long typeId);
	unsigned int IsValidBuyItem(unsigned long typeId);
	bool IsDisplayItemNumber(unsigned long typeId);
	IFF_STRUCT::sRandomBox *GetRandomBoxInfo(unsigned long typeId, unsigned long kind, unsigned long itemTypeId);
	bool IsNonVisibleItem(unsigned long typeId);
	bool Load(void);
	void Reload(void);
	bool GetEnumRandomBoxStuff(unsigned long typeId, std::vector<IFF_STRUCT::sRandomBox> &stuffs);

private:
	void MakeTikiRecipeMap(char *data, int count);
	void MakeTikiPointTableMap(char *data, int count);
	void MakeTikiSpecialTableMap(char *data, int count);
	void CheckInvalidItemName(void);
	_private::sRandomBox *findRandomBox(unsigned long typeId);
	void MakeCharMap(char *data, int count);
	void MakePartMap(char *data, int count);
	void MakeClubMap(char *data, int count);
	void MakeClubSetMap(char *data, int count);
	void MakeBallMap(char *data, int count);
	void MakeItemMap(char *data, int count);
	void MakeCaddieMap(char *data, int count);
	void MakeCadItemMap(char *data, int count);
	void MakeSetItemMap(char *data, int count);
	void MakeCourseMap(char *data, int count);
	void MakeMatchMap(char *data, int count);
	void MakeTitleMap(char *data, int count);
	void MakeEnchantMap(char *data, int count);
	void MakeSkinMap(char *data, int count);
	void MakeCardMap(char *data, int count);
	void MakeFurnitureMap(char *data, int count);
	void MakeOfflineMap(char *data, int count);
	void MakeCadieMagicBoxMap(char *data, int count);
	void MakeNonVisibleItemMap(char *data, int count);
	void MakeSubscriptionItemMap(char *data, int count);
	void MakeHairStyleMap(char *data, int count);
	void MakeChildItemMap(char *data, int count);
	void MakeAuxPartMap(char *data, int count);
	void MakeQuestDropMap(char *data, int count);
	void MakeQuestMap(char *data, int count);
	void MakeMascotMap(char *data, int count);
	void MakeDescMap(char *data, int count);
	void MakeFurnitureAbilityList(char *data, int count);
	void MakeCutinInforMationTableMap(char *data, int count);
	void MakeSpecialPrizeItemMap(char *data, int count);
	void MakeRandomRecycleMap(char *data, int count);
	void MakeItemBuffMap(void);
	void MakeRandomBoxMap(char *data, int count);

	LAYOUT_CHECK_FRIEND;

	// 0x0: vftable
	_client::CCriticalSection m_cs;                              // 0x4, guards the maps once loaded
	bool m_loaded;                                               // 0x20, cleared by the constructor; the Find/Get functions lock m_cs when set
	UNKNOWN(0x21, 0x24);

public:
	// The IFF tables by type id (Make*Map). The dialogs read some of them
	// directly, without m_cs (MapSelectDlg, NewRecordDlg: m_courseMap;
	// FrSPCardBuffDlg::OnInit: m_cardMap, public in place below).
	std::map<unsigned int, IFF_STRUCT::sChar> m_charMap;         // 0x24
	std::map<unsigned int, IFF_STRUCT::sPart> m_partMap;         // 0x30
	std::map<unsigned int, IFF_STRUCT::sClub> m_clubMap;         // 0x3C
	std::map<unsigned int, IFF_STRUCT::sClubSet> m_clubSetMap;   // 0x48
	std::map<unsigned int, IFF_STRUCT::sBall> m_ballMap;         // 0x54
	std::map<unsigned int, IFF_STRUCT::sItem> m_itemMap;         // 0x60
	std::map<unsigned int, IFF_STRUCT::sCaddie> m_caddieMap;     // 0x6C
	std::map<unsigned int, IFF_STRUCT::sCadItem> m_cadItemMap;   // 0x78
	std::map<unsigned int, IFF_STRUCT::sSetItem> m_setItemMap;   // 0x84
	std::map<unsigned int, IFF_STRUCT::sCourse> m_courseMap;     // 0x90

private:
	// The constructor builds every container below in this order (map
	// key/value types from its _Buynode calls).
	std::map<unsigned int, IFF_STRUCT::sMatch> m_matchMap;       // 0x9C
	std::map<unsigned int, IFF_STRUCT::sTitle> m_titleMap;       // 0xA8
	std::map<unsigned int, IFF_STRUCT::sEnchant> m_enchantMap;   // 0xB4 (MakeEnchantMap)
	std::map<unsigned int, IFF_STRUCT::sDesc> m_descMap;         // 0xC0 (MakeDescMap)
	std::map<unsigned int, IFF_STRUCT::sSkin> m_skinMap;         // 0xCC
	std::map<unsigned int, IFF_STRUCT::sHairStyle> m_hairStyleMap; // 0xD8
	std::map<unsigned int, IFF_STRUCT::sChildItem> m_childItemMap; // 0xE4

public:
	std::map<unsigned int, IFF_STRUCT::sMascot> m_mascotMap;     // 0xF0 (MakeMascotMap; CShopMain::Build_Mascot)

private:
	std::map<unsigned int, IFF_STRUCT::sAuxPart> m_auxPartMap;   // 0xFC
	std::map<unsigned int, IFF_STRUCT::sQuestDrop> m_questDropMap; // 0x108 (MakeQuestDropMap)
	std::map<unsigned int, IFF_STRUCT::sQuest> m_questMap;       // 0x114 (MakeQuestMap)
	std::map<unsigned int, IFF_STRUCT::sFurniture> m_furnitureMap; // 0x120

public:
	std::map<unsigned int, IFF_STRUCT::sCard> m_cardMap;         // 0x12C (CSharedDoc+0x314; FrSPCardBuffDlg::OnInit: m_cardMap)

private:
	std::map<unsigned int, IFF_STRUCT::sOfflineShop> m_offlineShopMap; // 0x138
	std::multimap<unsigned int, IFF_STRUCT::sRandomRecycle> m_randomRecycleMap; // 0x144
	std::list<IFF_STRUCT::sCadieMagicBox> m_cadieMagicBoxList;   // 0x150
	std::list<IFF_STRUCT::sFurnitureAbility> m_furnitureAbilityList; // 0x15C (MakeFurnitureAbilityList)
	std::list<IFF_STRUCT::sTikiPointTable> m_tikiPointTableList; // 0x168
	std::list<IFF_STRUCT::sTikiSpecialRecipe> m_tikiSpecialRecipeList; // 0x174
	std::map<unsigned int, _private::sRandomBox> m_randomBoxMap; // 0x180 (findRandomBox)
	std::map<unsigned int, IFF_STRUCT::sNonVisibleItem> m_nonVisibleItemMap; // 0x18C (IsNonVisibleItem)
	std::map<unsigned int, IFF_STRUCT::sSubscriptionItem> m_subscriptionItemMap; // 0x198 (IsSubScriptionCoupon)
	std::map<unsigned int, IFF_STRUCT::S5::sCutinInformation> m_cutinInformationMap; // 0x1A4
	std::map<unsigned int, IFF_STRUCT::sSpecialPrizeItem> m_specialPrizeItemMap[5]; // 0x1B0 (FindSPItem, by eSPAVILITYTYPE)
	std::map<unsigned long, sItemBuffHelper> m_itemBuffMap;      // 0x1EC
};

CHECK_SIZE(CItemManager::sItemBuffHelper, 0x1C);
CHECK_FIELD(CItemManager::sItemBuffHelper, m_ability, 0x0);
CHECK_FIELD(CItemManager::sItemBuffHelper, m_value0, 0x4);
CHECK_FIELD(CItemManager::sItemBuffHelper, m_value1, 0x8);
CHECK_FIELD(CItemManager::sItemBuffHelper, m_value2, 0xC);
CHECK_FIELD(CItemManager::sItemBuffHelper, m_compatible, 0x10);
// The constructor's last member is m_itemBuffMap (0x1EC + 0xC); trailing
// members it leaves uninitialised are not excluded, so no CHECK_SIZE yet.
BEGIN_LAYOUT_CHECK(CItemManager)
	CHECK_FIELD(CItemManager, m_cs, 0x4);
	CHECK_FIELD(CItemManager, m_loaded, 0x20);
	CHECK_FIELD(CItemManager, m_charMap, 0x24);
	CHECK_FIELD(CItemManager, m_partMap, 0x30);
	CHECK_FIELD(CItemManager, m_clubMap, 0x3C);
	CHECK_FIELD(CItemManager, m_clubSetMap, 0x48);
	CHECK_FIELD(CItemManager, m_ballMap, 0x54);
	CHECK_FIELD(CItemManager, m_itemMap, 0x60);
	CHECK_FIELD(CItemManager, m_caddieMap, 0x6C);
	CHECK_FIELD(CItemManager, m_cadItemMap, 0x78);
	CHECK_FIELD(CItemManager, m_setItemMap, 0x84);
	CHECK_FIELD(CItemManager, m_courseMap, 0x90);
	CHECK_FIELD(CItemManager, m_matchMap, 0x9C);
	CHECK_FIELD(CItemManager, m_titleMap, 0xA8);
	CHECK_FIELD(CItemManager, m_enchantMap, 0xB4);
	CHECK_FIELD(CItemManager, m_descMap, 0xC0);
	CHECK_FIELD(CItemManager, m_skinMap, 0xCC);
	CHECK_FIELD(CItemManager, m_hairStyleMap, 0xD8);
	CHECK_FIELD(CItemManager, m_childItemMap, 0xE4);
	CHECK_FIELD(CItemManager, m_mascotMap, 0xF0);
	CHECK_FIELD(CItemManager, m_auxPartMap, 0xFC);
	CHECK_FIELD(CItemManager, m_questDropMap, 0x108);
	CHECK_FIELD(CItemManager, m_questMap, 0x114);
	CHECK_FIELD(CItemManager, m_furnitureMap, 0x120);
	CHECK_FIELD(CItemManager, m_cardMap, 0x12C);
	CHECK_FIELD(CItemManager, m_offlineShopMap, 0x138);
	CHECK_FIELD(CItemManager, m_randomRecycleMap, 0x144);
	CHECK_FIELD(CItemManager, m_cadieMagicBoxList, 0x150);
	CHECK_FIELD(CItemManager, m_furnitureAbilityList, 0x15C);
	CHECK_FIELD(CItemManager, m_tikiPointTableList, 0x168);
	CHECK_FIELD(CItemManager, m_tikiSpecialRecipeList, 0x174);
	CHECK_FIELD(CItemManager, m_randomBoxMap, 0x180);
	CHECK_FIELD(CItemManager, m_nonVisibleItemMap, 0x18C);
	CHECK_FIELD(CItemManager, m_subscriptionItemMap, 0x198);
	CHECK_FIELD(CItemManager, m_cutinInformationMap, 0x1A4);
	CHECK_FIELD(CItemManager, m_specialPrizeItemMap, 0x1B0);
	CHECK_FIELD(CItemManager, m_itemBuffMap, 0x1EC);
END_LAYOUT_CHECK

namespace S5 {
bool ISCutinItem(unsigned long typeId);
}

#endif // PG_SHARED_ITEMMANAGER_H
