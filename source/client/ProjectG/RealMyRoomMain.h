// Original: d:\build\custom\temp\code\client\projectg\realmyroommain.h
#ifndef PG_CLIENT_PROJECTG_REALMYROOMMAIN_H
#define PG_CLIENT_PROJECTG_REALMYROOMMAIN_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "RTTI.h"
#include "frcmdtarget.h"
#include "TaskMain.h"
#include "scenemanager.h"
#include "polysoup.h"
#include "AnotherHand.h"
#include "../client/Wangreal/include/wmath.h"
#include "../client/Wangreal/include/wtypes.h"
#include <list>
#include <map>

class CFxSequence;
class FrWnd;
class FrNoteDlg;
class FrFortuneDlg;
class FrEdit;
class FrGroupBox;
class FrArea;
class FrViewer;
class FrStatic;
class FrGaugeBar;
class WOverlay;
class FrForm;
class FrEnchantDlg;
class FrMascotInfoDlg;
class FrPangGiftDlg;
class FrReportDlg;
class CCardBook;
class FrItemStorageNotice;
class FrListBox;
class FrButton;
class FrTabButton;
class FrUccDrawDlg;
class FrGraphicInterface;
class FrItemStorageAccessPanel;
class CExhibition;
class Bitmap;
class CPost_SendDlg;
class WOverlay;
struct sItemInfo;
struct sCharacterInfo;
class WTitleFont;
struct sGiftInfo;
struct sCaddieInfo;
struct FrListItem;

// The real my-room task (registered as "CRealMyRoomMain"). Bases from the
// constructor's vtable stores: CTaskMain at 0x0, CRenderFuncPtr at 0x1e0,
// CHandOwner at 0x1e4. Only the members the transcribed functions touch are
// declared.
// The PC bang item ReinitializePC re-equips: 1 the caddie, 2 the mascot.
// The original header is not known.
enum ePCBangItem {
	PCBANG_ITEM_CADDIE = 1,
	PCBANG_ITEM_MASCOT = 2
};

class CRealMyRoomMain : public CTaskMain, public CRenderFuncPtr, public CHandOwner {
public:
	CRealMyRoomMain(void);
	virtual ~CRealMyRoomMain(void);

	static const WRTTI m_RTTI;

	virtual const WRTTI* GetRTTI(void) const;   // realmyroommain.h:45

	virtual void OnInit(void);
	virtual void OnPreLoadInit(void);
	virtual void DoReLoad(void);
	virtual void Display(void);
	virtual void OnDisplay(void);
	virtual void OnDestroy(void);

	// Nested helper struct; members not recovered yet.
	struct miniBtn_t {
		miniBtn_t(void);
	};

	// The tabs of the item list; the values are not recovered yet.
	enum selTab_t {
	};

	// The main tabs; the values are not recovered yet.
	enum mainTab_t {
	};

	// The tabs of the stat window (OnStatPartUp selects 0, OnStatAllUp 1).
	enum statTab_t {
		STAT_TAB_PART = 0,
		STAT_TAB_ALL = 1
	};

	// The sub tabs of the item list; the values are not recovered yet.
	enum subTab_t {
	};

	// The main tabs of the furniture list; the values are not recovered yet.
	enum eFurniture_Tab {
	};

	bool IsItemStorageVisible(void) const;   // realmyroommain.h:1404
	void SetUIState(unsigned long state);
	void ChangeFlags(unsigned long flag);
	bool GetFlags(unsigned long flag);
	void OpenItemStorageAccessPanel(unsigned char type);
	void OpenItemStorageNotice(void);
	bool OnResultItemStorageNotice(int result, FrForm* pForm);
	bool OnResultItemStorageAccessPanel(int result, FrForm* pForm);
	void SetItemStorageVisible(bool visible);
	void OpenInformation(unsigned long* pTypeId, sGiftInfo* pGiftInfo, unsigned long* pSerial);
	void OpenInformation(FrListItem* pItem);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // realmyroommainui.cpp:171

	virtual void OnAnotherHand(int nWork, void* pData);
	virtual void OnUnderBar_MyRoomUp(void);
	virtual void CloseAllDialogs(void);

	void OpenLayout(const char* layout);
	void OpenEventGiftBox(sItemInfo* pItemInfo);
	void OpenS4EventGiftBox(sItemInfo* pItemInfo);
	bool OnOpenEventGiftDlgResult(int result, FrForm* pForm);
	bool OnOpenS4EventGiftDlgResult(int result, FrForm* pForm);
	void OnUnderBar_GiftUp(void);
	void OpenCaddieInfo(const sCaddieInfo& info);
	void OnMiddleUIInit(int param);
	void OnTabTxt1Init(int param);
	void OnTabTxt2Init(int param);
	void OnTabTxt3Init(int param);
	void OnSelectedNameInit(int param);
	void OnPetInit(int param);
	void OnSkinPetInit(int param);
	void OnFurnitureArrangeInit(int param);
	void OnItemDescBgInit2(int param);
	void OnItemNameInit(int param);
	void OnGiftDescInit(int param);
	void OnUpgradeInit(int param);
	void OnDowngradeInit(int param);
	void SetItemDesc(void);
	void OnItemListMouseMove(void);
	void OnSelItemInit(int param);
	void OnInitItemStoragePang(int param);
	void OnInitItemStorageIndexMsg(int param);
	void OnInitItemStorageDragDrop(int param);
	void OnFurniturePupInit(int param);
	void OnFurniturePdownInit(int param);
	void OnFurnitureListMaxNumInit(int param);
	void OnFurniture_BtnListNumInit(int param);
	void OnFurnitureResetInit(int param);
	void OnFurnitureBackInit(int param);
	void OnFurnitureSaveInit(int param);
	void OnFurnitureOutInit(int param);
	void OnFurnitureBackGroundInit1(int param);
	void OnFurnitureBackGroundInit2(int param);
	void OnFurnitureBackGroundInit3(int param);
	void OnFurnitureBackGroundInit4(int param);
	void OnFurnitureBackGroundInit5(int param);
	void OnFurnitureBackGroundInit6(int param);
	void OnFurnitureBackGroundInit7(int param);
	void OnFurnitureBackGroundInit8(int param);
	void OnFurnitureScrollInit1(int param);
	void OnFurnitureScrollInit2(int param);
	void OnFurnitureScrollInit3(int param);
	void DrawStatusCard(FrGraphicInterface* pGI, FrListItem* pItem, short* const cardStatus, float x);
	void DrawStatus2(FrGraphicInterface* pGI, FrListItem* pItem, short* const status, short* const slotStatus, float x);
	void OnSkinPet2Init(int param);
	void OnItemDescBgInit(int param);
	void OnItemSlotInit(intptr_t param);
	void OnStatFrameInit(int param);
	void OnStatTextInit(int param);
	void OnSelFrameInit(int param);
	void OnStatBoxInit(int param);
	void OnSelItemIconInit(int param);
	void OnStatPartInit(int param);
	void OnStatAllInit(int param);
	void OnTabSubInit(int param);
	void OnPowerEditInit(int param);
	void OnControlEditInit(int param);
	void OnImpactEditInit(int param);
	void OnSpinEditInit(int param);
	void OnCurveEditInit(int param);
	void OnPowerBarInit(intptr_t param);
	void OnControlBarInit(intptr_t param);
	void OnImpactBarInit(intptr_t param);
	void OnSpinBarInit(intptr_t param);
	void OnCurveBarInit(intptr_t param);
	bool UnselectItemFromItemList(unsigned long typeId);
	void Furniture_ShowUI_Left(bool bShow);
	void OnDBClickItemStorageList(void);
	void OnStatAllOwnerDraw(int param);
	void DrawCutinFrame(void);
	WOverlay* GetCutinImg(unsigned long typeId);
	WOverlay* GetCardImg(unsigned long typeId);
	void DrawCard(void);
	void OpenEventXmasGiftBox(sItemInfo* pItemInfo);
	void OnBtnMenuInit(int param);
	void OnEffectViewInit(int param);
	void OnFurnitureListInit(int param);
	void SafeLoadPet(unsigned long* pTypeId, bool bForce);
	void OnEquipResetUp(void);
	void OnFurniturePupButtonUp(void);
	void OnFurniturePdownButtonUp(void);
	bool SelectItemList(FrListItem* pItem);
	bool SelectItemList(unsigned long typeId);
	void UnselectItemList(FrListItem* pItem);
	bool UnselectItemList(unsigned long typeId);
	void SetGiftDescString(const char* pSender, const char* pMessage, const char* pDate, const char* pTime);
	bool OnNoteDlgResult(int result, FrForm* pForm);

	void DrawMouseCursor(void);
	void RefreshAllUccClothes(void);
	void CheckUIPosition(void);
	void CalculationCameraMat(WMatrix& mat, WVector at, WVector dir, WVector pos, float targetFov, float curFov, float speed);
	void SetCameraDisplayType(int type);
	void ToggleTopViewMode(void);
	void InitMyRoomControls(void);
	void InitMyRoomDialogs(void);
	void InitMyRoomVariables(void);
	void ChangeSelTab(selTab_t tab);
	void TurnOnStat(void);
	void TurnOffStat(void);
	void OnSelItemOwnerDraw(int param);
	void OnSelItemUp(void);
	void OnCardPackListDown(void);
	void OnCardPackListRbUp(void);
	void BuildCardPackList(void);
	void OnCardPackListInit(int param);
	void OnTabCardPackInit(int param);
	void OnTabCardVolume1Init(int param);
	void OnTabCardVolume2Init(int param);
	void OnTabCardVolume3Init(int param);
	void OnTabCardVolume4Init(int param);
	void OnRightMenuInit(int param);
	void OnCardBookInit(int param);
	void OnCardBookIndexInit(int param);
	void OnCardUseInit(int param);
	void OnCardPropertyInit(int param);
	void OnCardBookOwnerDraw(int param);
	void OnCardBookIndexLBDown(void);
	void OnCardBookIndexMouseOver(void);
	void OnTabCardPackLBDown(void);
	void OnTabCardVolume1LBDown(void);
	void OnTabCardVolume2LBDown(void);
	void OnTabCardVolume3LBDown(void);
	void OnTabCardVolume4LBDown(void);
	bool OnCardBoxDlgResult(int result, FrForm* pForm);
	void CardPackBehavior(unsigned char slot, unsigned char button);
	void OnCardPackLBDown1(void);
	void OnCardPackLBDown2(void);
	void OnCardPackLBDown3(void);
	void OnCardPackLBDown4(void);
	void OnCardPackLBDown5(void);
	void OnCardPackLBDown6(void);
	void OnCardPackLBDown7(void);
	void OnCardPackLBDown8(void);
	void OnCardPackRBDown1(void);
	void OnCardPackRBDown2(void);
	void OnCardPackRBDown3(void);
	void OnCardPackRBDown4(void);
	void OnCardPackRBDown5(void);
	void OnCardPackRBDown6(void);
	void OnCardPackRBDown7(void);
	void OnCardPackRBDown8(void);
	void CardBehavior(unsigned char slot, unsigned char button);
	void OnCardLBDown1(void);
	void OnCardLBDown2(void);
	void OnCardLBDown3(void);
	void OnCardLBDown4(void);
	void OnCardLBDown5(void);
	void OnCardLBDown6(void);
	void OnCardLBDown7(void);
	void OnCardLBDown8(void);
	void OnCardLBDown9(void);
	void OnCardLBDown10(void);
	void OnCardLBDown11(void);
	void OnCardLBDown12(void);
	void OnCardRBDown1(void);
	void OnCardRBDown2(void);
	void OnCardRBDown3(void);
	void OnCardRBDown4(void);
	void OnCardRBDown5(void);
	void OnCardRBDown6(void);
	void OnCardRBDown7(void);
	void OnCardRBDown8(void);
	void OnCardRBDown9(void);
	void OnCardRBDown10(void);
	void OnCardRBDown11(void);
	void OnCardRBDown12(void);
	bool OnCardRemoveDlgResult(int result, FrForm* pForm);
	bool OnEnchantDlgResult(int result, FrForm* pForm);
	bool OnMascotInfoDlgResult(int result, FrForm* pForm);
	bool OnUpgradeDlgResult(int result, FrForm* pForm);
	bool OnFunritureSaveMsgdlgResult(int result, FrForm* pForm);
	void OOnBtnMenuButtonUP(void);
	bool OnResultDlg_Report(int result, FrForm* pForm);
	bool OnOpenEventXmasGiftDlgResult(int result, FrForm* pForm);
	void OnOwnerDrawItemStoragePang(int param);
	void OnItemSlotBtnDown(void);
	void OnBtn_MenuListRBtnDown(void);
	void OnBtn_MenuListMouseMove(void);
	void OnBtn_MenuListDClick(void);
	void SetLevelBar(void);
	void ChangeStatTab(statTab_t tab);
	void OnStatPartUp(void);
	void OnStatAllUp(void);
	void OnFurniture_BtnListNumBtnUp(void);
	bool OnFurnitureListEnterKey(int param);
	void OnOwnerDrawItemStorageBGI(int param);
	void OnMouseMoveItemStorageList(void);
	void OnOwnerDrawItemStorageDragDrop(int param);
	void SelectMenu(unsigned char menu);
	bool OnAuthDlgFaileResult(int result, FrForm* pForm);
	void ControlUiHideMode(int mode);
	void OnFurnitureArrangeButtonUP(void);
	void SetMiddleUIControl(bool bShow);
	void BuildDisplayStand(subTab_t tab);
	bool OnReplayDelOpenDlgResult(int result, FrForm* pForm);
	void SetOverAndUnderBarVisible(bool bVisible);
	void UIDisplayControl(bool bShow);
	void SetListMenu(void);
	bool OnPostDlgResult(int result, FrForm* pForm);
	void OnUnderBar_ItemStorageUp(void);
	void OnLBDownItemStoragePangMinus(void);
	void OnLBDownItemStoragePangPlus(void);
	void OnOwnerDrawItemStorageList(int param);
	void OnOwnerDrawItemStorageIndexMsg(int param);
	void OnLBDownItemStorageIndexMsg(void);
	void OnLBDownItemStoragePageNext(void);
	void OnLBDownItemStoragePagePrev(void);
	void TurnOnItemSelect(void);
	void TurnOffItemSelect(void);
	void BoundCardPackPage(FrButton* pButton, unsigned char slot);
	void OnCardPackInit1(int param);
	void OnCardPackInit2(int param);
	void OnCardPackInit3(int param);
	void OnCardPackInit4(int param);
	void OnCardPackInit5(int param);
	void OnCardPackInit6(int param);
	void OnCardPackInit7(int param);
	void OnCardPackInit8(int param);
	void SetCardBookVisible(bool bVisible);
	void BoundCardPage(FrButton* pButton, unsigned char slot);
	void OnCardInit1(int param);
	void OnCardInit2(int param);
	void OnCardInit3(int param);
	void OnCardInit4(int param);
	void OnCardInit5(int param);
	void OnCardInit6(int param);
	void OnCardInit7(int param);
	void OnCardInit8(int param);
	void OnCardInit9(int param);
	void OnCardInit10(int param);
	void OnCardInit11(int param);
	void OnCardInit12(int param);
	void OnFurnitureListNumBtnUp(void);
	bool OnUccCopyDlgResult(int result, FrForm* pForm);
	void ShowPet(unsigned long typeId);
	void CloseLayout(void);
	void ChangeSubTab(subTab_t tab, bool bForce);
	void OnTabSubButtonUp(void);
	void TurnOnItemStatus(void);
	void TurnOffItemStatus(void);
	void AutoScroll(void);
	void ChangeFMainTab(eFurniture_Tab tab, bool bForce);
	void BuildDisplayStandFurniture(void);
	void ReinitializePC(ePCBangItem item);
	bool ChangeCaddie(unsigned long id);
	void ChangeMascot(unsigned long id);
	void GetCaddieStatus(unsigned char* const pStatus, unsigned char* const pUnused);
	void OnUnderBar_BackUp(void);
	void OnFurnitureTabMainButtonUp(void);
	void OnRightMenuRBtnDown(void);
	bool OnUccDrawDlgResult(int result, FrForm* pForm);
	const Bitmap* GetItemIcon(unsigned long typeId);
	void DrawSelItem(FrGraphicInterface* pGI, unsigned long typeId);
	bool OnOpenNewYearMoneyDlgResult(int result, FrForm* pForm);
	void OpenNewYearMoney(sItemInfo* pItemInfo);
	void OnBtn_MenuListBtnDown(void);
	void HidePet(bool bWait);
	void SetPetAreaRect(void);
	void Reinitialize(bool bForce);
	void OnMyRoomFinish(void);
	void UnequipItem(unsigned long* pItemId);
	void OnItemSlotDClick(void);
	bool IsInItemList(unsigned long typeId);
	void OnEffectViewOnwnerDraw(int param);
	void OnMyRoomInit(int param);
	void Net_StartPeriodItem(unsigned long serial);
	bool OnOfflineShopEditMsgDlgResult(int result, FrForm* pForm);
	bool OnPostSendDlgResult(int result, FrForm* pForm);
	bool OnPostSendDlg2Result(int result, FrForm* pForm);
	void PostDlgCreate(void);
	void Net_EquipCharacter(void);
	void Net_EquipItem(void);
	bool OnFortuneDlgResult(int result, FrForm* pForm);
	void CharacterViewControl(bool bShow);
	void ChangeMainTab(mainTab_t tab, bool bForce);
	void SetMiddleUIVisible(bool bVisible);
	void OnGiftBgInit(int param);
	void OnSelItemPrevInit(int param);
	void OnSelItemNextInit(int param);
	void Net_EquipCaddie(void);
	void Net_EquipGear(void);
	void UpdateRightMenu(void);
	void PopupRightMenu(int x, int y);
	void OnFurnitureListNumInit(int param);
	void OnInitItemStoragePageNext(int param);
	void OnInitItemStoragePagePrev(int param);
	void OnLBDownItemStorageClose(void);
	void OnInitItemStoragePangMinus(int param);
	void OnInitItemStoragePangPlus(int param);
	void OnInitItemStorageClose(int param);
	void OnItemSlotBoxInit(int param);
	void OnEquipCancelInit(int param);
	void OnEquipResetInit(int param);

	// Declared from the PDB publics, in source-line order (the original
	// declaration order is not known).
	void LoadMyRoomImages(void);
	void LoadItemImages(void);
	void ChangeCharacter(unsigned long typeId);
	int ChangeClubset(unsigned long typeId);
	void ChangeAztec(unsigned long typeId);
	void DrawCharacterList(FrGraphicInterface* pGI, unsigned long typeId);
	void DrawCaddieList(FrGraphicInterface* pGI, unsigned long typeId);
	void DrawMascotList(FrGraphicInterface* pGI, unsigned long typeId);
	void DrawAztecList(FrGraphicInterface* pGI, unsigned long typeId);
	void DrawClubList(FrGraphicInterface* pGI, unsigned long typeId);
	void DrawSkinFrame(void);
	void DrawSkinSlot(void);
	void OnPetOwnerDraw(int param);
	void OnItemDescBgOwnerDraw2(int param);
	void OnItemDescBgOwnerDraw(int param);
	void OnItemDescInit2(int param);
	void OnItemDescInit(int param);
	void OnEquipCancelUp(void);
	void OnItemSlotOwnerDraw(int param);
	void OnSelItemIconOwnerDraw(int param);
	void OnSelItemPrevUp(void);
	void OnSelItemNextUp(void);
	void OnStatPartOwnerDraw(int param);
	void OnTabMainInit(int param);
	void OnTabMainButtonUp(void);
	void OnGiftPageOwnerDraw(int param);
	void OnGiftPageLBDown(void);
	void OnCardPackListDraw(int param);
	void OnCardUseLBDown(void);
	void OnItemListInit(intptr_t param);
	void DrawStatus(FrGraphicInterface* pGI, FrListItem* pItem, short* const status, short* const slotStatus);
	void OnItemListOwnerDraw(intptr_t param);
	void MoveGift(sGiftInfo* pGiftInfo);
	void OpenTikiReport(sItemInfo* pItemInfo);
	void OnItemListBtnDown(void);
	void OnItemListRBtnDown(void);
	void OnItemListDClick(void);
	void OnUpgradeUp(void);
	void OnDowngradeUp(void);
	void OnTooltipHoverOn(int param);
	void OnTooltipHoverOff(int param);
	void GetTotalStatus(unsigned char* const status, unsigned char* const slotStatus, unsigned char* const cardStatus);
	void GetCharacterStatus(unsigned char* const status, unsigned char* const slotStatus);
	void GetClubSetStatus(unsigned char* const status, unsigned char* const slotStatus);
	void ShowMainModel(void);
	void DrawSelectItemList(FrListItem* pItem);
	int GetItemNumInMyPocket(unsigned long typeId);
	void Net_EquipDresserParts(void);
	void Net_EquipSkin(void);
	void Net_EquipMascot(void);
	bool OnPangGiftDlgResult(int result, FrForm* pForm);
	bool OnReturnGiftDlgResult(int result, FrForm* pForm);
	bool OnMoveGiftDlgResult(int result, FrForm* pForm);
	bool OnFurnitrueListDeleteDlgResult(int result, FrForm* pForm);
	bool OnCaddieInfoDlgResult(int result, FrForm* pForm);
	bool OnOpenTikiReport(int result, FrForm* pForm);
	void EquipItem(sItemInfo* pItemInfo);
	bool OnFortuneOpenDlgResult(int result, FrForm* pForm);
	bool OnReplayModeOpenDlgResult(int result, FrForm* pForm);
	void OnFurnitureResetButtonUp(void);
	void OnFurnitureBackButtonUp(void);
	void OnFurnitureSaveButtonUp(void);
	void OnFurnitureOutButtonUp(void);
	void OnFurnitureTabMainInit(int param);
	void OnFurnitureListOwnerDraw(int param);
	void OnFurnitureListBtnDown(void);
	void OnBtn_MenuListInit(int param);
	void OnBtn_MenuListOwnerDraw(int param);
	void FurnitureDisplayControl(bool bShow);
	void Furniture_ShowUI(bool bShow);
	void OnRightMenuLBtnDown(void);
	void FurnitureUse(void);
	void FurnitureRun(void);
	void FurnitureFunctionChange(void);
	bool OnFurniture_AbilityDlgResult(int result, FrForm* pForm);
	bool OnAuthDlgResult(int result, FrForm* pForm);
	void LoadCardImages(void);
	void DeleteCardImages(void);
	void DrawPartsSlot(FrGraphicInterface* pGI, FrListItem* pItem);
	bool OnCardDlgResult(int result, FrForm* pForm);
	bool OnCardPackDlgResult(int result, FrForm* pForm);
	bool OnItemAttachCardDlgResult(int result, FrForm* pForm);
	void OpenNewRandomBox(sItemInfo* pItemInfo);
	bool OnOpenNewRandomBoxDlgResult(int result, FrForm* pForm);
	void OnInitItemStorageBGI(int param);
	void OnInitItemStorageList(int param);
	void OnLBDownItemStorageList(void);
	void OnRBDownItemStorageList(void);
	void DeleteCutinImages(void);
	bool OnRentalExtensionDlgResult(int result, FrForm* pForm);

public:
	bool GetArrangement(unsigned long typeId, unsigned long serial);
	void RMR_UpdateDB(void);

private:
	void ExecuteMiniButtonDown(sItemInfo* pItemInfo);
	friend class CRealMyRoom_Avatar;   // RealMyRoom_Avatar Render reads m_shadowTexture

private:
	UNKNOWN(0x1E8, 0x209);             // WTL::CString at 0x204: LoadBg's map name
	bool m_decalShown;                 // 0x209, Display draws the decal under the avatar
	UNKNOWN(0x20A, 0x20C);
	int m_decalTexture;                // 0x20c, the texture of that decal
	FrWnd* m_pRightMenuWnd;            // 0x210, hidden by OnRightMenuRBtnDown
	UNKNOWN(0x214, 0x220);
	WVector m_avatarViewPos;           // 0x220, the local avatar's position raised 4.6 and moved back 46 (OnInit)
	UNKNOWN(0x22C, 0x258);
	bool m_topViewMode;                // 0x258, toggled by ToggleTopViewMode
	UNKNOWN(0x259, 0x25C);
	float m_cameraFov;                 // 0x25c, 70.0 when the top view / type 3 is set
	UNKNOWN(0x260, 0x264);
	WVector m_cameraOffset;            // 0x264, WVector::ZERO for the top view
	float m_decalPulse;                // 0x270, the decal's pulse phase (scaled by cos(phase * 10))
	UNKNOWN(0x274, 0x288);
	WTitleFont* m_pTitleFont;          // 0x288, released by the destructor
	UNKNOWN(0x28C, 0x2BC);
	int m_shadowTexture;               // 0x2bc, the shadow texture handle OnLoad loads
public:
	CFxSequence* m_pRoomFx;            // 0x2c0, the room's effect sequence OnLoad opens; avatars pause it when hidden
private:
	UNKNOWN(0x2C4, 0x2D4);
	WVector m_eyePos;                  // 0x2d4, the eye position SetCameraDisplayType picks
	float m_eyeFov;                    // 0x2e0, the field of view going with it
	FrWnd* m_pMiddleUIWnd;             // 0x2e4, faded by SetMiddleUIVisible
	FrStatic* m_pSelectedName;         // 0x2e8
	FrWnd* m_pItemStatusWnd;           // 0x2ec, enabled by TurnOnItemStatus
	FrViewer* m_pStatAllViewer;        // 0x2f0, the viewer of the all-stat tab
	UNKNOWN(0x2F4, 0x2F8);
	FrTabButton* m_pSubTab;            // 0x2f8, read by OnTabSubButtonUp
	UNKNOWN(0x2FC, 0x304);
	FrListBox* m_pUccCopyHideWnd;      // 0x304, shown again when the UCC copy dialog closes; IsInItemList searches its items
	FrArea* m_pTabTxt[3];              // 0x308, the text areas of the three tabs
	FrWnd* m_pPetArea;                 // 0x314, one of m_pPetAreaWnd (SetPetAreaRect)
	UNKNOWN(0x318, 0x31C);
	FrArea* m_pSelFrame;               // 0x31c
	FrWnd* m_pSelItemWnd;              // 0x320, its rect frames DrawSelItem's icon
	FrButton* m_pItemSelectWnd;        // 0x324, blinks (OnSelItemInit)
	FrWnd* m_pStatWnd;                 // 0x328
	FrGaugeBar* m_pPowerBar;           // 0x32c
	FrGaugeBar* m_pControlBar;         // 0x330
	FrGaugeBar* m_pImpactBar;          // 0x334
	FrGaugeBar* m_pSpinBar;            // 0x338
	FrGaugeBar* m_pCurveBar;           // 0x33c
	FrStatic* m_pPowerEdit;            // 0x340
	FrStatic* m_pControlEdit;          // 0x344
	FrStatic* m_pImpactEdit;           // 0x348
	FrStatic* m_pSpinEdit;             // 0x34c
	FrStatic* m_pCurveEdit;            // 0x350
	FrButton* m_pSelItemPrevButton;    // 0x354, push delay 0.1 (OnSelItemPrevInit)
	FrButton* m_pSelItemNextButton;    // 0x358, push delay 0.1 (OnSelItemNextInit)
	FrArea* m_pItemDescBg;             // 0x35c
	FrEdit* m_pItemDescEdit;           // 0x360, cleared by SetItemDesc
	FrViewer* m_pItemDescBg2;          // 0x364, hidden at init (OnItemDescBgInit2)
	FrEdit* m_pItemDescEdit2;          // 0x368, hidden at init (OnItemDescInit2)
	FrEdit* m_pItemNameEdit;           // 0x36c, hidden at init (OnItemNameInit)
	FrWnd* m_pPetAreaWnd[3];           // 0x370, the pet areas SetPetAreaRect chooses from
	FrListBox* m_pItemSlotList;        // 0x37c, its items carry the equipped item ids (OnItemSlotDClick)
	FrButton* m_pFurnitureArrangeButton;   // 0x380, hidden at init (OnFurnitureArrangeInit)
	FrGroupBox* m_pItemSlotBox;        // 0x384, hidden at init, an item storage background UI
	FrButton* m_pEquipCancelButton;    // 0x388, hover enabled at init, an item storage background UI
	FrButton* m_pEquipResetButton;     // 0x38c, hover enabled at init, an item storage background UI
	FrButton* m_pUpgradeButton;        // 0x390, hidden at init, hover enabled
	FrButton* m_pDowngradeButton;      // 0x394, hidden at init, hover enabled
	FrWnd* m_pCaddieInfoWnd;           // 0x398, re-enabled when the caddie info dialog closes with 2, disabled again when the caddie's level is above the player's
	FrViewer* m_pGiftBgViewer;         // 0x39c, hidden and without mouse events at init (OnGiftBgInit)
	FrEdit* m_pGiftDescEdit;           // 0x3a0, the description of the selected gift (SetGiftDescString)
	FrArea* m_pStatFrame;              // 0x3a4
	FrArea* m_pStatText;               // 0x3a8
	FrTabButton* m_pFurnitureMainTab;  // 0x3ac, read by OnFurnitureTabMainButtonUp
	FrListBox* m_pFurnitureList;       // 0x3b0, multi-select except on furniture main tabs 1-3 (ChangeFMainTab)
	FrListBox* m_pMenuList;            // 0x3b4, its items carry the menu number + 1
	FrButton* m_pFurniturePageUpButton;     // 0x3b8, hidden at init
	FrButton* m_pFurniturePageDownButton;   // 0x3bc, hidden at init
	FrEdit* m_pFurnitureListNumWnd;    // 0x3c0, the current furniture page
	FrEdit* m_pFurniturePageCountWnd;  // 0x3c4, the furniture page count
	FrButton* m_pFurnitureListNumButton;    // 0x3c8, hidden at init
	FrButton* m_pFurnitureResetButton;      // 0x3cc, hidden at init
	FrButton* m_pFurnitureBackButton;       // 0x3d0, hidden at init
	FrButton* m_pFurnitureSaveButton;       // 0x3d4, hidden at init
	FrButton* m_pFurnitureOutButton;        // 0x3d8, hidden at init
	FrArea* m_pFurnitureBackGround[8];      // 0x3dc, hidden at init
	FrArea* m_pFurnitureScroll[3];          // 0x3fc, hidden at init
	UNKNOWN(0x408, 0x420);
	FrButton* m_pBtnMenu;              // 0x420
	UNKNOWN(0x424, 0x428);
	FrEnchantDlg* m_pEnchantDlg;       // 0x428
	FrForm* m_pUpgradeDlg;             // 0x42c
	UNKNOWN(0x430, 0x434);
	FrPangGiftDlg* m_pPangGiftDlg;     // 0x434, cleared by OnPangGiftDlgResult
	FrMascotInfoDlg* m_pMascotInfoDlg; // 0x438
	FrNoteDlg* m_pNoteDlg;             // 0x43c
	FrFortuneDlg* m_pFortuneDlg;       // 0x440, cleared by OnFortuneDlgResult
	UNKNOWN(0x444, 0x448);
	FrForm* m_pReplayDelDlg;           // 0x448
	UNKNOWN(0x44C, 0x45C);
	FrUccDrawDlg* m_pUccDrawDlg;       // 0x45c
	FrForm* m_pUccCopyDlg;             // 0x460
	FrReportDlg* m_pReportDlg;         // 0x464
	FrForm* m_pCardRemoveDlg;          // 0x468
	UNKNOWN(0x46C, 0x474);
	FrForm* m_pPostDlg;                // 0x474
	CPost_SendDlg* m_pPostSendDlg;     // 0x478, the "write_post" form OnPostSendDlg2Result opens
	UNKNOWN(0x47C, 0x504);
	const Bitmap* m_pStatAllBitmap[3]; // 0x504, OnStatAllOwnerDraw's tab image: normal, hovered, selected
	UNKNOWN(0x510, 0x670);
	FrWnd* m_pFurnitureLeftWnd[5];     // 0x670, enabled together by Furniture_ShowUI_Left
	FrWnd* m_pEffectViewWnd;           // 0x684, framed by OnEffectViewOnwnerDraw
	UNKNOWN(0x688, 0x694);
	FrWnd* m_pLayoutWnd;               // 0x694, closed by CloseLayout
	WPoint m_infoDlgPos;               // 0x698, the clicked item's position minus (300, 450): where OpenInformation(FrListItem*) puts the dialog
	WPoint m_itemDescEdit2InitPos;     // 0x6a0, m_itemDescEdit2Pos as OnItemDescInit2 found it
	std::map<unsigned long, const Bitmap*> m_itemIconMap;   // 0x6a8, GetItemIcon's cache of registered icons by type id
	std::map<unsigned int, WOverlay*> m_cardImgMap;    // 0x6b4, GetCardImg's cache of card images by type id
	std::map<unsigned int, WOverlay*> m_cutinImgMap;   // 0x6c0, GetCutinImg's cache of skin cut-in images by type id
	UNKNOWN(0x6CC, 0x6D8);
	bool m_selItemOn;                  // 0x6d8
	UNKNOWN(0x6D9, 0x6DC);
	const char* m_pCharLevels;        // 0x6dc, the shown character's stat levels (GetCharacterStatus)
	const unsigned long* m_pCharParts; // 0x6e0, the shown character's equipped parts (GetCharacterStatus)
	UNKNOWN(0x6E4, 0x744);
	const unsigned long* m_pCharCards; // 0x744, the shown character's equipped cards (GetCharacterStatus)
	unsigned long m_charTypeId;        // 0x748, the shown character's type id (GetCharacterStatus)
	UNKNOWN(0x74C, 0x750);
	unsigned long m_caddieTypeId;      // 0x750, the caddie whose stat bonus GetCaddieStatus adds
	UNKNOWN(0x754, 0x764);
	unsigned int m_clubSerial;         // 0x764, the shown club set's item serial (GetClubSetStatus looks up its upgrade levels)
	unsigned long m_clubTypeId;        // 0x768, the shown club set's type id
	const short* m_pClubLevels;        // 0x76c, the shown club set's upgrade levels (GetTotalStatus)
	unsigned long m_aztecTypeId;       // 0x770, the shown aztec (ball) type id (ChangeAztec)
	statTab_t m_statTab;               // 0x774
	mainTab_t m_mainTab;               // 0x778, reselected by SetMiddleUIVisible
	int m_roomMode;                    // 0x77c, 13-15 and 16 select their own pet area (SetPetAreaRect)
	selTab_t m_selTab;                 // 0x780
	UNKNOWN(0x784, 0x788);
	WPoint m_itemDescPos;              // 0x788, set per room mode by SetItemDesc
	WPoint m_itemDescEditPos;          // 0x790, the bottom-left corner of m_pItemDescEdit (OnItemDescInit)
	WPoint m_itemDescEdit2Pos;         // 0x798, the bottom-left corner of m_pItemDescEdit2 (OnItemDescInit2)
	eFurniture_Tab m_furnitureMainTab; // 0x7a0, ChangeFMainTab's tab
	int m_furniturePage;               // 0x7a4, reset to 0 when the furniture main tab changes
	int m_furniturePageCount;          // 0x7a8, the furniture page is clamped below it
	UNKNOWN(0x7AC, 0x7B0);
	unsigned long m_newYearMoneySerial;   // 0x7b0, the serial OpenNewYearMoney opens
	unsigned long m_eventGiftBoxSerial;   // 0x7b4, the serial OpenEventGiftBox opens
	unsigned long m_xmasGiftBoxSerial;   // 0x7b8, the serial OpenEventXmasGiftBox opens
	unsigned long m_s4EventGiftBoxTypeId; // 0x7bc, the type OpenS4EventGiftBox opens
	UNKNOWN(0x7C0, 0x7DC);
	CExhibition* m_pPetExhibition;     // 0x7dc, deleted by HidePet
	CAnotherHand m_petLoader;          // 0x7e0, HidePet waits for its work
	std::map<unsigned int, sCharacterInfo> m_characterMap;   // 0x968, a copy of CSharedDoc's owned characters (OnInit)
	unsigned long m_caddieId;         // 0x974, the caddie Net_EquipCaddie equips when it is not the current one
	unsigned long m_characterId;       // 0x978, the character Net_EquipCharacter equips when it is not the current one
	unsigned long m_clubId;           // 0x97c, the club set Net_EquipGear sends
	unsigned long m_ballId;           // 0x980, the ball Net_EquipGear sends
	unsigned long m_equipItem[10];     // 0x984, the edited equipped items, sent by Net_EquipItem when they differ from CSharedDoc's
	UNKNOWN(0x9AC, 0x9C4);
	unsigned long m_equipSkin[6];      // 0x9c4, the edited equipped skins, sent by Net_EquipSkin when they differ from CSharedDoc's
	unsigned long m_mascotId;          // 0x9dc, the mascot Net_EquipMascot equips when it is not the current one
	UNKNOWN(0x9E0, 0xB68);
	std::list<unsigned long> m_initList;   // 0xb68, element type not recovered; emptied by OnMyRoomInit
	int m_statSource;                  // 0xb74, what SetLevelBar showed last: 0 the character, 1 the club set, 2 the caddie
	unsigned long m_displaySkin[6];    // 0xb78, copy of CSharedDoc::m_equipSkin made by Reinitialize
	int m_skinSlotA;                   // 0xb90, cleared by Reinitialize
	int m_skinSlotB;                   // 0xb94, cleared by Reinitialize
	int m_skinParamA;                  // 0xb98, the skin's byte 0xB8
	int m_skinParamB;                  // 0xb9c, the skin's byte 0xB9
	unsigned long m_cardTypeId;        // 0xba0, the card DrawCard shows
	UNKNOWN(0xBA4, 0xC50);
	// The named cameras of the room; SetCameraDisplayType cycles 2..5.
	WPolySoup::sCamera* m_pCameras[8]; // 0xc50
	UNKNOWN(0xC70, 0xC78);
	// Two debug segments OnDisplay draws in white.
	WVector m_debugLineFrom;           // 0xc78
	WVector m_debugLineTo;             // 0xc84
	UNKNOWN(0xC90, 0xC9C);
	WVector m_debugLine2From;          // 0xc9c
	WVector m_debugLine2To;            // 0xca8
	UNKNOWN(0xCB4, 0xCE8);
	int m_cameraDisplayType;           // 0xce8, the type SetCameraDisplayType stores
	UNKNOWN(0xCEC, 0xD0C);
	WMatrix m_savedCamera;             // 0xd0c, the camera SetCameraDisplayType(0) restores
	float m_savedFov;                  // 0xd3c, the field of view restored with it
	UNKNOWN(0xD40, 0xD45);
	bool m_bHideMiddleUI;              // 0xd45, set by ControlUiHideMode
	UNKNOWN(0xD46, 0xD4C);
	unsigned long m_uiState;           // 0xd4c, the UI state bits SetUIState toggles
	unsigned long m_prevUiState;       // 0xd50, the m_uiState ControlUiHideMode replaced
	// The UI flags of ChangeFlags/GetFlags: flag 1, 2, 4 and 8 select
	// elements 0-3.
	bool m_flags[5];                   // 0xd54
	UNKNOWN(0xD59, 0xD5C);
	int m_offlineShopEditState;        // 0xd5c, 1 once the offline shop edit message is confirmed
	WOverlay* m_pGenderIcons[6];       // 0xd60, the male/female icons OnLoad loads ("i_male.tga", ...)
	WOverlay* m_pMannerIcons[2];       // 0xd78, "i_male_manner.tga", "i_female_manner.tga"
	WOverlay* m_pAngelIcons[2];        // 0xd80, "i_male_angel.tga", ...
	bool m_cardBookVisible;            // 0xd88, the state SetCardBookVisible gave the card book
	UNKNOWN(0xD89, 0xD8C);
	CCardBook* m_pCardBook;            // 0xd8c
	FrItemStorageNotice* m_pItemStorageNotice;   // 0xd90
	FrItemStorageAccessPanel* m_pItemStorageAccessPanel;   // 0xd94, cleared by OnResultItemStorageAccessPanel
	bool m_itemStorageVisible;         // 0xd98
	UNKNOWN(0xD99, 0xDA8);
	FrArea* m_pItemStoragePang;        // 0xda8, a front UI of the item storage
	FrButton* m_pItemStoragePangMinus; // 0xdac, a front UI of the item storage
	FrButton* m_pItemStoragePangPlus; // 0xdb0, a front UI of the item storage
	FrListBox* m_pItemStorageList;   // 0xdb4
	FrListBox* m_pItemStorageIndexMsg; // 0xdb8, a front UI of the item storage
	FrButton* m_pItemStoragePageNext; // 0xdbc, push delay 0.1, hidden at init
	FrButton* m_pItemStoragePagePrev; // 0xdc0, push delay 0.1, hidden at init
	FrButton* m_pItemStorageClose;    // 0xdc4, a front UI of the item storage
	FrArea* m_pItemStorageDragDrop;    // 0xdc8, a front UI of the item storage
	FrListBox* m_pCardPackListBox;     // 0xdcc, shown only on the card pack tab of the card book

	LAYOUT_CHECK_FRIEND;
};
BEGIN_LAYOUT_CHECK(CRealMyRoomMain)
	CHECK_FIELD(CRealMyRoomMain, m_pRightMenuWnd, 0x210);
	CHECK_FIELD(CRealMyRoomMain, m_avatarViewPos, 0x220);
	CHECK_FIELD(CRealMyRoomMain, m_pTitleFont, 0x288);
	CHECK_FIELD(CRealMyRoomMain, m_topViewMode, 0x258);
	CHECK_FIELD(CRealMyRoomMain, m_cameraFov, 0x25C);
	CHECK_FIELD(CRealMyRoomMain, m_cameraOffset, 0x264);
	CHECK_FIELD(CRealMyRoomMain, m_pRoomFx, 0x2C0);
	CHECK_FIELD(CRealMyRoomMain, m_pMiddleUIWnd, 0x2E4);
	CHECK_FIELD(CRealMyRoomMain, m_pItemStatusWnd, 0x2EC);
	CHECK_FIELD(CRealMyRoomMain, m_pSubTab, 0x2F8);
	CHECK_FIELD(CRealMyRoomMain, m_pUccCopyHideWnd, 0x304);
	CHECK_FIELD(CRealMyRoomMain, m_pPetArea, 0x314);
	CHECK_FIELD(CRealMyRoomMain, m_pSelItemWnd, 0x320);
	CHECK_FIELD(CRealMyRoomMain, m_pItemSelectWnd, 0x324);
	CHECK_FIELD(CRealMyRoomMain, m_pStatWnd, 0x328);
	CHECK_FIELD(CRealMyRoomMain, m_pSelItemPrevButton, 0x354);
	CHECK_FIELD(CRealMyRoomMain, m_pSelItemNextButton, 0x358);
	CHECK_FIELD(CRealMyRoomMain, m_pPetAreaWnd, 0x370);
	CHECK_FIELD(CRealMyRoomMain, m_pItemSlotList, 0x37C);
	CHECK_FIELD(CRealMyRoomMain, m_pFurnitureListNumWnd, 0x3C0);
	CHECK_FIELD(CRealMyRoomMain, m_pFurnitureMainTab, 0x3AC);
	CHECK_FIELD(CRealMyRoomMain, m_pMenuList, 0x3B4);
	CHECK_FIELD(CRealMyRoomMain, m_pEnchantDlg, 0x428);
	CHECK_FIELD(CRealMyRoomMain, m_pUpgradeDlg, 0x42C);
	CHECK_FIELD(CRealMyRoomMain, m_pPangGiftDlg, 0x434);
	CHECK_FIELD(CRealMyRoomMain, m_pMascotInfoDlg, 0x438);
	CHECK_FIELD(CRealMyRoomMain, m_pReplayDelDlg, 0x448);
	CHECK_FIELD(CRealMyRoomMain, m_pUccDrawDlg, 0x45C);
	CHECK_FIELD(CRealMyRoomMain, m_pUccCopyDlg, 0x460);
	CHECK_FIELD(CRealMyRoomMain, m_pReportDlg, 0x464);
	CHECK_FIELD(CRealMyRoomMain, m_pCardRemoveDlg, 0x468);
	CHECK_FIELD(CRealMyRoomMain, m_pPostDlg, 0x474);
	CHECK_FIELD(CRealMyRoomMain, m_pPostSendDlg, 0x478);
	CHECK_FIELD(CRealMyRoomMain, m_pEffectViewWnd, 0x684);
	CHECK_FIELD(CRealMyRoomMain, m_pLayoutWnd, 0x694);
	CHECK_FIELD(CRealMyRoomMain, m_selItemOn, 0x6D8);
	CHECK_FIELD(CRealMyRoomMain, m_pCharLevels, 0x6DC);
	CHECK_FIELD(CRealMyRoomMain, m_pCharParts, 0x6E0);
	CHECK_FIELD(CRealMyRoomMain, m_pCharCards, 0x744);
	CHECK_FIELD(CRealMyRoomMain, m_charTypeId, 0x748);
	CHECK_FIELD(CRealMyRoomMain, m_infoDlgPos, 0x698);
	CHECK_FIELD(CRealMyRoomMain, m_cardImgMap, 0x6B4);
	CHECK_FIELD(CRealMyRoomMain, m_cutinImgMap, 0x6C0);
	CHECK_FIELD(CRealMyRoomMain, m_pCaddieInfoWnd, 0x398);
	CHECK_FIELD(CRealMyRoomMain, m_statTab, 0x774);
	CHECK_FIELD(CRealMyRoomMain, m_mainTab, 0x778);
	CHECK_FIELD(CRealMyRoomMain, m_roomMode, 0x77C);
	CHECK_FIELD(CRealMyRoomMain, m_selTab, 0x780);
	CHECK_FIELD(CRealMyRoomMain, m_furnitureMainTab, 0x7A0);
	CHECK_FIELD(CRealMyRoomMain, m_furniturePage, 0x7A4);
	CHECK_FIELD(CRealMyRoomMain, m_caddieTypeId, 0x750);
	CHECK_FIELD(CRealMyRoomMain, m_clubSerial, 0x764);
	CHECK_FIELD(CRealMyRoomMain, m_clubTypeId, 0x768);
	CHECK_FIELD(CRealMyRoomMain, m_pClubLevels, 0x76c);
	CHECK_FIELD(CRealMyRoomMain, m_aztecTypeId, 0x770);
	CHECK_FIELD(CRealMyRoomMain, m_pFurnitureList, 0x3B0);
	CHECK_FIELD(CRealMyRoomMain, m_newYearMoneySerial, 0x7B0);
	CHECK_FIELD(CRealMyRoomMain, m_eventGiftBoxSerial, 0x7B4);
	CHECK_FIELD(CRealMyRoomMain, m_s4EventGiftBoxTypeId, 0x7BC);
	CHECK_FIELD(CRealMyRoomMain, m_pPetExhibition, 0x7DC);
	CHECK_FIELD(CRealMyRoomMain, m_petLoader, 0x7E0);
	CHECK_FIELD(CRealMyRoomMain, m_characterId, 0x978);
	CHECK_FIELD(CRealMyRoomMain, m_characterMap, 0x968);
	CHECK_FIELD(CRealMyRoomMain, m_caddieId, 0x974);
	CHECK_FIELD(CRealMyRoomMain, m_clubId, 0x97C);
	CHECK_FIELD(CRealMyRoomMain, m_ballId, 0x980);
	CHECK_FIELD(CRealMyRoomMain, m_equipSkin, 0x9C4);
	CHECK_FIELD(CRealMyRoomMain, m_mascotId, 0x9DC);
	CHECK_FIELD(CRealMyRoomMain, m_pGiftBgViewer, 0x39C);
	CHECK_FIELD(CRealMyRoomMain, m_initList, 0xB68);
	CHECK_FIELD(CRealMyRoomMain, m_statSource, 0xB74);
	CHECK_FIELD(CRealMyRoomMain, m_bHideMiddleUI, 0xD45);
	CHECK_FIELD(CRealMyRoomMain, m_prevUiState, 0xD50);
	CHECK_FIELD(CRealMyRoomMain, m_displaySkin, 0xB78);
	CHECK_FIELD(CRealMyRoomMain, m_skinSlotA, 0xB90);
	CHECK_FIELD(CRealMyRoomMain, m_skinSlotB, 0xB94);
	CHECK_FIELD(CRealMyRoomMain, m_skinParamA, 0xB98);
	CHECK_FIELD(CRealMyRoomMain, m_skinParamB, 0xB9C);
	CHECK_FIELD(CRealMyRoomMain, m_debugLineFrom, 0xC78);
	CHECK_FIELD(CRealMyRoomMain, m_debugLineTo, 0xC84);
	CHECK_FIELD(CRealMyRoomMain, m_debugLine2From, 0xC9C);
	CHECK_FIELD(CRealMyRoomMain, m_debugLine2To, 0xCA8);
	CHECK_FIELD(CRealMyRoomMain, m_cameraDisplayType, 0xCE8);
	CHECK_FIELD(CRealMyRoomMain, m_decalShown, 0x209);
	CHECK_FIELD(CRealMyRoomMain, m_decalTexture, 0x20C);
	CHECK_FIELD(CRealMyRoomMain, m_decalPulse, 0x270);
	CHECK_FIELD(CRealMyRoomMain, m_eyePos, 0x2D4);
	CHECK_FIELD(CRealMyRoomMain, m_eyeFov, 0x2E0);
	CHECK_FIELD(CRealMyRoomMain, m_pCameras, 0xC50);
	CHECK_FIELD(CRealMyRoomMain, m_savedCamera, 0xD0C);
	CHECK_FIELD(CRealMyRoomMain, m_savedFov, 0xD3C);
	CHECK_FIELD(CRealMyRoomMain, m_uiState, 0xD4C);
	CHECK_FIELD(CRealMyRoomMain, m_flags, 0xD54);
	CHECK_FIELD(CRealMyRoomMain, m_offlineShopEditState, 0xD5C);
	CHECK_FIELD(CRealMyRoomMain, m_shadowTexture, 0x2BC);
	CHECK_FIELD(CRealMyRoomMain, m_pGenderIcons, 0xD60);
	CHECK_FIELD(CRealMyRoomMain, m_pMannerIcons, 0xD78);
	CHECK_FIELD(CRealMyRoomMain, m_pAngelIcons, 0xD80);
	CHECK_FIELD(CRealMyRoomMain, m_cardBookVisible, 0xD88);
	CHECK_FIELD(CRealMyRoomMain, m_pCardBook, 0xD8C);
	CHECK_FIELD(CRealMyRoomMain, m_pItemStorageNotice, 0xD90);
	CHECK_FIELD(CRealMyRoomMain, m_pItemStorageAccessPanel, 0xD94);
	CHECK_FIELD(CRealMyRoomMain, m_itemStorageVisible, 0xD98);
	CHECK_FIELD(CRealMyRoomMain, m_pItemStorageList, 0xDB4);
	CHECK_FIELD(CRealMyRoomMain, m_pItemStoragePangMinus, 0xDAC);
	CHECK_FIELD(CRealMyRoomMain, m_pItemStoragePangPlus, 0xDB0);
	CHECK_FIELD(CRealMyRoomMain, m_pItemStorageClose, 0xDC4);
	CHECK_FIELD(CRealMyRoomMain, m_pItemSlotBox, 0x384);
	CHECK_FIELD(CRealMyRoomMain, m_pEquipCancelButton, 0x388);
	CHECK_FIELD(CRealMyRoomMain, m_pEquipResetButton, 0x38C);
	CHECK_FIELD(CRealMyRoomMain, m_pItemStoragePageNext, 0xDBC);
	CHECK_FIELD(CRealMyRoomMain, m_pItemStoragePagePrev, 0xDC0);
	CHECK_FIELD(CRealMyRoomMain, m_pCardPackListBox, 0xDCC);
	CHECK_FIELD(CRealMyRoomMain, m_pSelectedName, 0x2E8);
	CHECK_FIELD(CRealMyRoomMain, m_pStatAllViewer, 0x2F0);
	CHECK_FIELD(CRealMyRoomMain, m_pTabTxt, 0x308);
	CHECK_FIELD(CRealMyRoomMain, m_pSelFrame, 0x31C);
	CHECK_FIELD(CRealMyRoomMain, m_pPowerBar, 0x32C);
	CHECK_FIELD(CRealMyRoomMain, m_pCurveBar, 0x33C);
	CHECK_FIELD(CRealMyRoomMain, m_pFurnitureLeftWnd, 0x670);
	CHECK_FIELD(CRealMyRoomMain, m_pStatAllBitmap, 0x504);
	CHECK_FIELD(CRealMyRoomMain, m_cardTypeId, 0xBA0);
	CHECK_FIELD(CRealMyRoomMain, m_pPowerEdit, 0x340);
	CHECK_FIELD(CRealMyRoomMain, m_pControlEdit, 0x344);
	CHECK_FIELD(CRealMyRoomMain, m_pImpactEdit, 0x348);
	CHECK_FIELD(CRealMyRoomMain, m_pSpinEdit, 0x34C);
	CHECK_FIELD(CRealMyRoomMain, m_pCurveEdit, 0x350);
	CHECK_FIELD(CRealMyRoomMain, m_pItemDescBg, 0x35C);
	CHECK_FIELD(CRealMyRoomMain, m_pStatFrame, 0x3A4);
	CHECK_FIELD(CRealMyRoomMain, m_pStatText, 0x3A8);
	CHECK_FIELD(CRealMyRoomMain, m_pBtnMenu, 0x420);
	CHECK_FIELD(CRealMyRoomMain, m_itemIconMap, 0x6A8);
	CHECK_FIELD(CRealMyRoomMain, m_pFurniturePageCountWnd, 0x3C4);
	CHECK_FIELD(CRealMyRoomMain, m_furniturePageCount, 0x7A8);
	CHECK_FIELD(CRealMyRoomMain, m_pGiftDescEdit, 0x3A0);
	CHECK_FIELD(CRealMyRoomMain, m_pItemDescBg2, 0x364);
	CHECK_FIELD(CRealMyRoomMain, m_pItemDescEdit, 0x360);
	CHECK_FIELD(CRealMyRoomMain, m_itemDescPos, 0x788);
	CHECK_FIELD(CRealMyRoomMain, m_equipItem, 0x984);
	CHECK_FIELD(CRealMyRoomMain, m_pFortuneDlg, 0x440);
	CHECK_FIELD(CRealMyRoomMain, m_pItemStoragePang, 0xDA8);
	CHECK_FIELD(CRealMyRoomMain, m_pItemStorageIndexMsg, 0xDB8);
	CHECK_FIELD(CRealMyRoomMain, m_pItemStorageDragDrop, 0xDC8);
	CHECK_FIELD(CRealMyRoomMain, m_pFurniturePageUpButton, 0x3B8);
	CHECK_FIELD(CRealMyRoomMain, m_pFurniturePageDownButton, 0x3BC);
	CHECK_FIELD(CRealMyRoomMain, m_pFurnitureListNumButton, 0x3C8);
	CHECK_FIELD(CRealMyRoomMain, m_pFurnitureResetButton, 0x3CC);
	CHECK_FIELD(CRealMyRoomMain, m_pFurnitureBackButton, 0x3D0);
	CHECK_FIELD(CRealMyRoomMain, m_pFurnitureSaveButton, 0x3D4);
	CHECK_FIELD(CRealMyRoomMain, m_pFurnitureOutButton, 0x3D8);
	CHECK_FIELD(CRealMyRoomMain, m_pFurnitureBackGround, 0x3DC);
	CHECK_FIELD(CRealMyRoomMain, m_pFurnitureScroll, 0x3FC);
	CHECK_FIELD(CRealMyRoomMain, m_pItemNameEdit, 0x36C);
	CHECK_FIELD(CRealMyRoomMain, m_pItemDescEdit2, 0x368);
	CHECK_FIELD(CRealMyRoomMain, m_itemDescEdit2InitPos, 0x6A0);
	CHECK_FIELD(CRealMyRoomMain, m_itemDescEditPos, 0x790);
	CHECK_FIELD(CRealMyRoomMain, m_itemDescEdit2Pos, 0x798);
	CHECK_FIELD(CRealMyRoomMain, m_pFurnitureArrangeButton, 0x380);
	CHECK_FIELD(CRealMyRoomMain, m_pUpgradeButton, 0x390);
	CHECK_FIELD(CRealMyRoomMain, m_pDowngradeButton, 0x394);
	CHECK_FIELD(CRealMyRoomMain, m_pNoteDlg, 0x43C);
END_LAYOUT_CHECK
CHECK_SIZE(CRealMyRoomMain, 0xDD0);

#endif // PG_CLIENT_PROJECTG_REALMYROOMMAIN_H
