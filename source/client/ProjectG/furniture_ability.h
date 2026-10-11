// Original: d:\build\custom\temp\code\client\projectg\furniture_ability.h
#ifndef PG_CLIENT_PROJECTG_FURNITURE_ABILITY_H
#define PG_CLIENT_PROJECTG_FURNITURE_ABILITY_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class FrArea;
class FrButton;
class FrEdit;
class FrListBox;
class WTitleFont;
struct sItemInfo;

// Furniture ability dialog of the room editor; its size (0x150) is the `new`
// of CFurniture_AbilityDlgMakeInstance. Only the members this unit defines so
// far are declared.
class CFurniture_AbilityDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // furniture_ability.h:23

	CFurniture_AbilityDlg(void);
	virtual ~CFurniture_AbilityDlg(void);

	void OnFurnitureAbility_ItemListInit(intptr_t param);       // furniture_ability.cpp:136
	void OnFurnitureAbility_ItemListOwnerDraw(intptr_t param);  // furniture_ability.cpp:150
	void OnFurnitureAbility_ItemListBtnDown(void);         // furniture_ability.cpp:232
	void OnFurnitureAbility_ItemListMouseMove(void);       // furniture_ability.cpp:263
	void OnFurnitureAbility_ImgSelect(intptr_t param);          // furniture_ability.cpp:274
	void OnFurnitureAbility_SelectListInit(intptr_t param);     // furniture_ability.cpp:289
	void OnFurnitureAbility_EditArrayInit(intptr_t param);      // furniture_ability.cpp:304
	void OnFurnitureAbility_BtnArrayInit(intptr_t param);       // furniture_ability.cpp:320
	void OnFurnitureAbility_TextImg(intptr_t param);            // furniture_ability.cpp:338
	void GetItemInfo(sItemInfo *pInfo);               // furniture_ability.cpp:380

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // furniture_ability.cpp:33

	void SetList(void);

	FrListBox* m_pItemList;        // 0x110, OnFurnitureAbility_ItemListInit
	FrListBox* m_pSelectList;      // 0x114, OnFurnitureAbility_SelectListInit
	FrArea* m_pImgSelect;          // 0x118, OnFurnitureAbility_ImgSelect
	unsigned long m_value11c;      // 0x11c
	sItemInfo* m_pSelectedItem;    // 0x120, OnFurnitureAbility_ItemListBtnDown
	FrButton* m_pBtnArray;         // 0x124, OnFurnitureAbility_BtnArrayInit
	FrEdit* m_pEditArray;          // 0x128, OnFurnitureAbility_EditArrayInit
	FrArea* m_pTextImg;            // 0x12c, OnFurnitureAbility_TextImg
	WTitleFont* m_pTitleFont;      // 0x130
	WTitleFont* m_pNumberFont;     // 0x134
	const Bitmap* m_pSelectNormal; // 0x138, "rmr_select_n"
	const Bitmap* m_pSelectDown;   // 0x13c, "rmr_select_d"
	const Bitmap* m_pLine;         // 0x140, "rmr_line"
	PADDING(0x144, 0x150);
};

CHECK_SIZE(CFurniture_AbilityDlg, 0x150);
BEGIN_LAYOUT_CHECK(CFurniture_AbilityDlg)
	CHECK_FIELD(CFurniture_AbilityDlg, m_pItemList, 0x110);
	CHECK_FIELD(CFurniture_AbilityDlg, m_pSelectList, 0x114);
	CHECK_FIELD(CFurniture_AbilityDlg, m_pImgSelect, 0x118);
	CHECK_FIELD(CFurniture_AbilityDlg, m_value11c, 0x11c);
	CHECK_FIELD(CFurniture_AbilityDlg, m_pSelectedItem, 0x120);
	CHECK_FIELD(CFurniture_AbilityDlg, m_pBtnArray, 0x124);
	CHECK_FIELD(CFurniture_AbilityDlg, m_pEditArray, 0x128);
	CHECK_FIELD(CFurniture_AbilityDlg, m_pTextImg, 0x12c);
	CHECK_FIELD(CFurniture_AbilityDlg, m_pTitleFont, 0x130);
	CHECK_FIELD(CFurniture_AbilityDlg, m_pNumberFont, 0x134);
	CHECK_FIELD(CFurniture_AbilityDlg, m_pSelectNormal, 0x138);
	CHECK_FIELD(CFurniture_AbilityDlg, m_pSelectDown, 0x13c);
	CHECK_FIELD(CFurniture_AbilityDlg, m_pLine, 0x140);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_FURNITURE_ABILITY_H
