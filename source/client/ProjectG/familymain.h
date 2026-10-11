// Original: d:\build\custom\temp\code\client\projectg\familymain.h
#ifndef PG_CLIENT_PROJECTG_FAMILYMAIN_H
#define PG_CLIENT_PROJECTG_FAMILYMAIN_H

#include <vector>
#include "..\..\defines.h"
#include "..\..\padding.h"
#include "TaskMain.h"
#include "RTTI.h"
#include "../client/Wangreal/include/wtypes.h"
#include "AnotherHand.h"
#include "../shared/globalgamedefine.h"

class FrForm;
class FrArea;
class FrButton;
class FrListBox;
class FrTreasureCourse;
class CExhibition;
class CStatBar;
class CPartTidList;
struct FrListItem;

// The family (pet) task. Only the members the transcribed handlers touch
// are named; its full size is not known yet.
class CFamilyMain : public CTaskMain {
	LAYOUT_CHECK_FRIEND;
public:
	// One pet of the family screen, owned by CFamilyMain::m_families. Its
	// members sit at byte offsets (sItemInfo at 0x385, the slots at 0x43D):
	// the layout is packed.
#pragma pack(push, 1)
	class cFamily {
		LAYOUT_CHECK_FRIEND;
	public:
		cFamily(void);
		~cFamily(void);

		void Init(const WRect& rect, std::vector<int>& petList, unsigned char petKind);
		void Render(void);
		void Process(float dt);
		const WPoint& GetFramePos(void) const;
		const sUserEquip& GetUserEquip(void) const;
		void SetItemSlot(unsigned long* pItemSlot);

	private:
		void SetStatus(void);
		void PetRebuild(void);
		void MakePet(std::vector<int>& petList);

		WRect m_rect;                   // 0x0, the pet's area
		WPoint m_framePos;              // 0x10, "family_info_frm" centred at the bottom of m_rect
		WPoint m_modelPos;              // 0x18, m_framePos + (45, 50)
		CExhibition* m_exhibition;      // 0x20
		CAnotherHand m_anotherHand;     // 0x24
		CPartTidList* m_partTidList;     // 0x1AC, the pet's parts (PetRebuild)
		sCharacterInfo m_charInfo;      // 0x1B0
		sCaddieInfo m_caddieInfo;       // 0x36C
		sItemInfo m_itemInfo;           // 0x385
		sUserEquip m_userEquip;         // 0x42D, the pet's equipment (GetUserEquip)
		unsigned char m_unknown499[0x3F];   // 0x499, cleared by the constructor
		CStatBar* m_statBar;            // 0x4D8
		unsigned char m_petKind;        // 0x4DC, Init's last argument
	};
#pragma pack(pop)

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; } // familymain.h:10

	static unsigned char m_mapIndex;
	static unsigned long ms_itemslotBackup[4][10];   // per pet, first dimension assumed

	CFamilyMain(void);
	virtual ~CFamilyMain(void);

	virtual void OnPreLoadInit(void);
	virtual void OnLoad(void);
	virtual void OnInit(void);
	virtual void OnDisplay(void);
	virtual void OnProcess(float dt);
	virtual void OnDestroy(void);
	virtual void HandleMsg(const MsgObject& msg);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void TurnOffControls(void);
	void OnInitAreaPet1(int param);
	void OnInitAreaPet2(int param);
	void OnInitAreaPet3(int param);
	void OnInitAreaPet4(int param);
	void OnInitAreaGlobal(int param);
	void OnInitBtnStart(int param);
	void OnInitBtnSelMap(int param);
	void OnInitBtnHoleType(int param);
	void OnInitBtnHoleNum(int param);
	void OnInitBtnTimeLimit(int param);
	void OnInitCreatePet3(int param);
	void OnInitCreatePet4(int param);
	void OnInitDeletePet3(int param);
	void OnInitDeletePet4(int param);
	void OnInitListEquip1(int param);
	void OnInitListEquip2(int param);
	void OnInitListEquip3(int param);
	void OnInitListEquip4(int param);
	void OnDownDeletePet3(void);
	void OnDownDeletePet4(void);
	void OnDrawAreaPet1(void);
	void OnDrawAreaPet2(void);
	void OnDrawAreaPet3(void);
	void OnDrawAreaPet4(void);
	void OnDrawAreaGlobal(void);
	void OnDownBtnSelMap(void);
	bool OnResultSelMap(int, FrForm*);
	void OnDownBtnHoleType(void);
	void OnDownBtnHoleNum(void);
	void OnDrawBtnSelMap(void);
	void OnDrawBtnHoleType(void);
	void OnDrawBtnHoleNum(void);
	void OnDrawBtnTimeLimit(void);
	bool OnItemEquipResult(int result, FrForm* pForm);
	void OnDownBtnTimeLimit(void);
	void OnDrawBtnStart(void);
	void OnDrawEquipListBox(FrListItem* pItem);
	void OnDrawListEquip1(int);
	void OnDrawListEquip2(int);
	void OnDrawListEquip3(int);
	void OnDrawListEquip4(int);
	void OnDownEquipListBox(int index);
	void OnDownListEquip1(int);
	void OnDownListEquip2(int);
	void OnDownListEquip3(int);
	void OnDownListEquip4(int);
	void ReFresh(bool);

private:
	std::vector<cFamily*> m_families;   // 0x1E0, one per pet slot
	std::vector<FrListBox*> m_equipLists;  // 0x1F0, the four equipment list boxes
	std::vector<FrArea*> m_areas;           // 0x200, the four pet areas, then the global one
	std::vector<FrButton*> m_buttons;       // 0x210, start, map, hole type/count, time limit, create/delete pet 3-4
	unsigned long m_timeLimit;          // 0x220, cycled by OnDownBtnTimeLimit
	unsigned char m_holeNum;            // 0x224, cycled by OnDownBtnHoleNum
	unsigned char m_holeType;           // 0x225
	PADDING(0x226, 0x228);
	std::vector<int> m_petList;         // 0x228, element type not confirmed
	FrTreasureCourse* m_selMapDlg;      // 0x238, cleared by OnResultSelMap
	void* m_unknown23C;                 // 0x23C, cleared by the constructor
	int m_curPet;                       // 0x240, the pet whose equipment is edited
	PADDING(0x244, 0x248);
	bool m_unknown248;                  // 0x248, cleared by the constructor
	PADDING(0x249, 0x24C);
};

IObject* CFamilyMainMakeInstance(void);

CHECK_SIZE(CFamilyMain, 0x24C);

BEGIN_LAYOUT_CHECK(CFamilyMain)
	CHECK_FIELD(CFamilyMain, m_families, 0x1E0);
	CHECK_FIELD(CFamilyMain, m_equipLists, 0x1F0);
	CHECK_FIELD(CFamilyMain, m_areas, 0x200);
	CHECK_FIELD(CFamilyMain, m_buttons, 0x210);
	CHECK_FIELD(CFamilyMain, m_timeLimit, 0x220);
	CHECK_FIELD(CFamilyMain, m_holeNum, 0x224);
	CHECK_FIELD(CFamilyMain, m_holeType, 0x225);
	CHECK_FIELD(CFamilyMain, m_petList, 0x228);
	CHECK_FIELD(CFamilyMain, m_selMapDlg, 0x238);
	CHECK_FIELD(CFamilyMain, m_unknown23C, 0x23C);
	CHECK_FIELD(CFamilyMain, m_curPet, 0x240);
	CHECK_FIELD(CFamilyMain, m_unknown248, 0x248);
END_LAYOUT_CHECK

BEGIN_LAYOUT_CHECK(CFamilyMain::cFamily)
	CHECK_FIELD(CFamilyMain::cFamily, m_rect, 0x0);
	CHECK_FIELD(CFamilyMain::cFamily, m_framePos, 0x10);
	CHECK_FIELD(CFamilyMain::cFamily, m_modelPos, 0x18);
	CHECK_FIELD(CFamilyMain::cFamily, m_exhibition, 0x20);
	CHECK_FIELD(CFamilyMain::cFamily, m_anotherHand, 0x24);
	CHECK_FIELD(CFamilyMain::cFamily, m_partTidList, 0x1AC);
	CHECK_FIELD(CFamilyMain::cFamily, m_charInfo, 0x1B0);
	CHECK_FIELD(CFamilyMain::cFamily, m_caddieInfo, 0x36C);
	CHECK_FIELD(CFamilyMain::cFamily, m_itemInfo, 0x385);
	CHECK_FIELD(CFamilyMain::cFamily, m_userEquip, 0x42D);
	CHECK_FIELD(CFamilyMain::cFamily, m_unknown499, 0x499);
	CHECK_FIELD(CFamilyMain::cFamily, m_statBar, 0x4D8);
	CHECK_FIELD(CFamilyMain::cFamily, m_petKind, 0x4DC);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_FAMILYMAIN_H
