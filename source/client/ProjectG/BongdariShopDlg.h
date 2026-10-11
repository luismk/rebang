// Original: d:\build\custom\temp\code\client\projectg\bongdarishopdlg.h
#ifndef PG_CLIENT_PROJECTG_BONGDARISHOPDLG_H
#define PG_CLIENT_PROJECTG_BONGDARISHOPDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "NoticeImage.h"

#include <list>

class CExhibition;
class FrArea;
class FrButton;
class FrEdit;
class FrListBox;
class FrTextButton;
class FrViewer;
class Bitmap;
class WTitleFont;
class CSimpleUI;

// A bonus ball of the Bongdari shop (0x14 bytes: SetBonusBall copies
// count * 0x14). The header that declares it originally is not known; only
// the fields the transcribed functions read are named.
struct sBonusBall {
	int m_index;              // 0x0, negative for an empty ball (SetClosedBonusBall)
	unsigned long m_typeId;   // 0x4, the item's type id (FindCommonItem, OpenInformation)
	UNKNOWN(0x8, 0xC);
	int m_count;              // 0xC, how many were won (SetOpeningBonusBallInfo)
	UNKNOWN(0x10, 0x11);
	bool m_bUnitCount;        // 0x11, totals go to m_unitCount instead of m_count (AddListTotalItem)
	unsigned short m_unitCount;   // 0x12
};

CHECK_SIZE(sBonusBall, 0x14);
CHECK_FIELD(sBonusBall, m_typeId, 0x4);
CHECK_FIELD(sBonusBall, m_count, 0xC);
CHECK_FIELD(sBonusBall, m_bUnitCount, 0x11);
CHECK_FIELD(sBonusBall, m_unitCount, 0x12);

// One of the 9 item silhouettes shown in the Bongdari machine (0x14 bytes,
// array at FrBongdariShopDlg+0x114, reset by InitBongdariShop). The
// original type name is not known.
struct sItemSilhouette {
	int m_slot;               // 0x0, -1 when unassigned
	const Bitmap* m_pImage;   // 0x4, silhouette image drawn (DisplayItemSilhouette)
	unsigned long m_slotParam;   // 0x8, item type id, cleared by InitBongdariShop
	unsigned char m_grade;    // 0xC, picks the silhouette's tint
	unsigned char m_state;    // 0xD, 0 = default silhouette, 1/2 = revealed
	float m_scale;            // 0x10, 10.0 after InitBongdariShop
};

CHECK_SIZE(sItemSilhouette, 0x14);
CHECK_FIELD(sItemSilhouette, m_pImage, 0x4);
CHECK_FIELD(sItemSilhouette, m_grade, 0xC);
CHECK_FIELD(sItemSilhouette, m_state, 0xD);
CHECK_FIELD(sItemSilhouette, m_scale, 0x10);

// The Bongdari (gacha machine) shop; its size (0x3f0) is the `new` of
// FrBongdariShopDlgMakeInstance. Only the members this unit defines so far
// are declared.
class FrBongdariShopDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // bongdarishopdlg.h:18

	// Motion of a bonus ball (0x34 bytes, 5 at +0x2ac).
	struct sMovingData {
		sMovingData(void);

		UNKNOWN(0x0, 0x10);
		float m_shadowY;    // 0x10, shadow anchor (DrawShadow)
		UNKNOWN(0x14, 0x18);
		float m_shadowX;    // 0x18, shadow anchor (DrawShadow)
		float m_height;     // 0x1c, compared with m_shadowY for the shadow's fade
		UNKNOWN(0x20, 0x24);
		float m_distance;   // 0x24, subtracted from the camera distance (DrawBonusBall)
		UNKNOWN(0x28, 0x34);
	};

	FrBongdariShopDlg(void);
	virtual ~FrBongdariShopDlg(void);

	void SetUsingBonusBall(bool b) { m_bUsingBonusBall = b; }   // bongdarishopdlg.h:192
	void InitBongdariShop(void);   // bongdarishopdlg.cpp:835
	void SetBongdariShop(void);   // bongdarishopdlg.cpp:859
	void SetBonusBall(int count, sBonusBall* pBalls);   // bongdarishopdlg.cpp:871
	void SetDataFee(int fee);   // bongdarishopdlg.cpp:1055
	void UpdateUsableTimes(void);   // bongdarishopdlg.cpp:1066
	void InitCouponBongdariShop(void);   // bongdarishopdlg.cpp:1039
	void EnableExitButton(bool enable);   // bongdarishopdlg.cpp:2786
	void Clear(void);
	void ShowDiscountUsableTimes(void);   // bongdarishopdlg.cpp:1074

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // bongdarishopdlg.cpp:78

	void OnBongdariShopInit(int);   // bongdarishopdlg.cpp:258
	void OnAboutShopBnInit(intptr_t);   // bongdarishopdlg.cpp:262
	void OnAboutShopBnUp(void);   // bongdarishopdlg.cpp:267
	void OnTotalItemBnInit(intptr_t);   // bongdarishopdlg.cpp:273
	void OnTotalItemBnUp(void);   // bongdarishopdlg.cpp:280
	void OnBongdarishopNoticeBnInit(intptr_t);   // bongdarishopdlg.cpp:286
	void OnBongdarishopNoticeBnUp(void);   // bongdarishopdlg.cpp:291
	void OnCloseBnInit(intptr_t);   // bongdarishopdlg.cpp:297
	void OnCloseBnUp(void);   // bongdarishopdlg.cpp:302
	void OnBongdariInit(intptr_t);   // bongdarishopdlg.cpp:308
	void OnBonusBall01BnInit(intptr_t);   // bongdarishopdlg.cpp:353
	void OnBonusBall02BnInit(intptr_t);   // bongdarishopdlg.cpp:369
	void OnBonusBall03BnInit(intptr_t);   // bongdarishopdlg.cpp:385
	void OnBonusBall04BnInit(intptr_t);   // bongdarishopdlg.cpp:401
	void OnBonusBall05BnInit(intptr_t);   // bongdarishopdlg.cpp:417
	void OnBonusBall01OwnerDraw(void);   // bongdarishopdlg.cpp:364
	void OnBonusBall02OwnerDraw(void);   // bongdarishopdlg.cpp:380
	void OnBonusBall03OwnerDraw(void);   // bongdarishopdlg.cpp:396
	void OnBonusBall04OwnerDraw(void);   // bongdarishopdlg.cpp:412
	void OnBonusBall05OwnerDraw(void);   // bongdarishopdlg.cpp:428
	virtual bool OnInit(void);   // bongdarishopdlg.cpp:442
	virtual bool OnLButtonUp(const WPoint& pt);   // bongdarishopdlg.cpp:620
	virtual void OnCancel(void);   // bongdarishopdlg.cpp:662
	bool OnAboutShopDlgResult(int, FrForm*);   // bongdarishopdlg.cpp:764
	bool OnTotalItemDlgResult(int, FrForm*);   // bongdarishopdlg.cpp:771
	bool OnBongdarishopNoticeDlgResult(int, FrForm*);   // bongdarishopdlg.cpp:778
	bool OnBongdarishopRareItemDlgResult(int, FrForm*);   // bongdarishopdlg.cpp:800
	bool OnBongdarishopWarningDlgResult(int, FrForm*);   // bongdarishopdlg.cpp:807

	void DrawBonusBall(unsigned int index);
	void ShowBongdari(void);
	void SetRandomDataItemSilhouette(void);
	void SetPosMovingClickText(float dt);   // bongdarishopdlg.cpp:1459
	void SetOpeningBonusBallInfo(sBonusBall* pBall);   // bongdarishopdlg.cpp:1887
	void DeletePetBonusBall(void);   // bongdarishopdlg.cpp:1966
	void SetStartAnimationBongdari(void);   // bongdarishopdlg.cpp:1977

	void OpenAboutShopDlg(void);   // bongdarishopdlg.cpp:669
	void OpenTotalItemDlg(void);   // bongdarishopdlg.cpp:681
	void OpenBongdarishopWarningDlg(int, int);   // bongdarishopdlg.cpp:734
	void OpenClosedBonusBallResultDlg(void);   // bongdarishopdlg.cpp:747
	void OpenBongdarishopNoticeDlg(bool);   // bongdarishopdlg.cpp:694
	void OpenBongdarishopRareItemDlg(int index);   // bongdarishopdlg.cpp:717
	void ControlNoticeDlg(void);   // bongdarishopdlg.cpp:814
	void OnBongdariOwnerDraw(void);   // bongdarishopdlg.cpp:316
	void SendReBuildMyInventory(void);
	void SendDataClosedBonusBall(void);
	void DrawShadow(void);
	void SendStateBongdariShop(bool bState);
	void UpdateListMyItem(sBonusBall* pBall);
	void DisplayItemSilhouette(void);
	void DrawUserInfo(void);
	void DrawTextBongdariClick(void);   // bongdarishopdlg.cpp:1641
	void DrawTextBonusBallClick(void);
	void ShuffleRandomDataItemSilhouette(void);   // bongdarishopdlg.cpp:2252
	unsigned long SelectDifferentTypeId(int type);   // bongdarishopdlg.cpp:2473
	void AddListTotalItem(sBonusBall* pBall);   // bongdarishopdlg.cpp:2540

private:
	int m_shopState;                   // 0x110, 1 once SetBongdariShop ran
	sItemSilhouette m_silhouettes[9];  // 0x114
	sBonusBall m_bonusBalls[5];        // 0x1c8, set by SetBonusBall
	int m_bonusBallCount;              // 0x22c, at most 5
	CExhibition* m_pPetBonusBall[5];   // 0x230, freed by DeletePetBonusBall
	CExhibition* m_pBongdari;          // 0x244, the machine model
	FrButton* m_pExitBn;               // 0x248
	FrArea* m_pBongdariArea;           // 0x24c, the machine's area (OnBongdariInit)
	FrArea* m_pBonusBallArea[5];       // 0x250, bonus ball areas 01-05 (hidden at init)
	const Bitmap* m_pDefaultSilhouetteImage;   // 0x264, silhouette of a slot in state 0
	const Bitmap* m_pShadowImage;      // 0x268, shadow under the machine and the balls
	const Bitmap* m_pClickTextImage;   // 0x26c, "click" text drawn over the machine
	const Bitmap* m_pBonusClickTextImage;   // 0x270, "click" text drawn over the selected bonus ball
	PADDING(0x274, 0x27c);
	float m_clickTextY;                // 0x27c, offset of the moving "click" text
	bool m_clickTextDown;              // 0x280, the text moves back up when set
	bool m_bBongdariPressed;           // 0x281, a button press on the machine waits for its release (OnLButtonUp)
	bool m_bUsingBonusBall;            // 0x282
	bool m_bAnimating;                 // 0x283, set when the machine's motion starts
	WPoint m_ballPos;                  // 0x284, moved by the window origin in OnInit
	PADDING(0x28c, 0x294);
	int m_selectedBall;                // 0x294, -1 = none; drawn closer
	PADDING(0x298, 0x29c);
	WTitleFont* m_pTitleFont29c;       // 0x29c
	WTitleFont* m_pTitleFont2a0;       // 0x2a0
	WTitleFont* m_pTitleFont2a4;       // 0x2a4
	CSimpleUI* m_pUserInfoUI;          // 0x2a8
	sMovingData m_movingData[5];       // 0x2ac
	bool m_bSubDlgOpen;                // 0x3b0, cleared when a child dialog closes
	PADDING(0x3b1, 0x3b4);
	std::list<sBonusBall> m_totalItemList;   // 0x3b4, shown by FrTotalItemDlg
	int m_couponCount;                 // 0x3c0, coupons usable in the shop (InitCouponBongdariShop)
	short m_usableTimes[3];            // 0x3c4, Bongdari plays usable, bonus usable, bonus remaining (UpdateUsableTimes)
	PADDING(0x3ca, 0x3cc);
	int m_dataFee;                     // 0x3cc, highest fee set by SetDataFee
	int m_dataFee3d0;                  // 0x3d0, cleared by SetDataFee
	int m_dataFee3d4;                  // 0x3d4, set to 0x200 by SetDataFee
	bool m_bShowDiscount;              // 0x3d8, Bongdari plays left (ShowDiscountUsableTimes)
	bool m_bShowBonusDiscount;         // 0x3d9, bonus plays left
	PADDING(0x3da, 0x3dc);
	int m_discountTime;                // 0x3dc, cleared when the discount shows
	int m_discountState;               // 0x3e0, set to 0x200 when the discount shows
	PADDING(0x3e4, 0x3e8);
	FrButton* m_pTotalItemBn;          // 0x3e8, set by OnTotalItemBnInit
	float m_animTime;                  // 0x3ec, exit button follows only from 8.0
};

CHECK_SIZE(FrBongdariShopDlg, 0x3f0);
BEGIN_LAYOUT_CHECK(FrBongdariShopDlg)
	CHECK_FIELD(FrBongdariShopDlg, m_shopState, 0x110);
	CHECK_FIELD(FrBongdariShopDlg, m_silhouettes, 0x114);
	CHECK_FIELD(FrBongdariShopDlg, m_pDefaultSilhouetteImage, 0x264);
	CHECK_FIELD(FrBongdariShopDlg, m_pBonusBallArea, 0x250);
	CHECK_FIELD(FrBongdariShopDlg, m_pBongdariArea, 0x24c);
	CHECK_FIELD(FrBongdariShopDlg, m_pTotalItemBn, 0x3e8);
	CHECK_FIELD(FrBongdariShopDlg, m_selectedBall, 0x294);
	CHECK_FIELD(FrBongdariShopDlg, m_movingData, 0x2ac);
	CHECK_FIELD(FrBongdariShopDlg, m_bonusBalls, 0x1c8);
	CHECK_FIELD(FrBongdariShopDlg, m_bonusBallCount, 0x22c);
	CHECK_FIELD(FrBongdariShopDlg, m_clickTextY, 0x27c);
	CHECK_FIELD(FrBongdariShopDlg, m_clickTextDown, 0x280);
	CHECK_FIELD(FrBongdariShopDlg, m_bUsingBonusBall, 0x282);
	CHECK_FIELD(FrBongdariShopDlg, m_bAnimating, 0x283);
	CHECK_FIELD(FrBongdariShopDlg, m_totalItemList, 0x3b4);
	CHECK_FIELD(FrBongdariShopDlg, m_couponCount, 0x3c0);
	CHECK_FIELD(FrBongdariShopDlg, m_usableTimes, 0x3c4);
	CHECK_FIELD(FrBongdariShopDlg, m_pPetBonusBall, 0x230);
	CHECK_FIELD(FrBongdariShopDlg, m_pBongdari, 0x244);
	CHECK_FIELD(FrBongdariShopDlg, m_pUserInfoUI, 0x2a8);
	CHECK_FIELD(FrBongdariShopDlg, m_pExitBn, 0x248);
	CHECK_FIELD(FrBongdariShopDlg, m_ballPos, 0x284);
	CHECK_FIELD(FrBongdariShopDlg, m_bSubDlgOpen, 0x3b0);
	CHECK_FIELD(FrBongdariShopDlg, m_dataFee, 0x3cc);
	CHECK_FIELD(FrBongdariShopDlg, m_dataFee3d0, 0x3d0);
	CHECK_FIELD(FrBongdariShopDlg, m_dataFee3d4, 0x3d4);
	CHECK_FIELD(FrBongdariShopDlg, m_animTime, 0x3ec);
	CHECK_FIELD(FrBongdariShopDlg, m_pClickTextImage, 0x26c);
	CHECK_FIELD(FrBongdariShopDlg, m_pBonusClickTextImage, 0x270);
	CHECK_FIELD(FrBongdariShopDlg, m_bBongdariPressed, 0x281);
	CHECK_FIELD(FrBongdariShopDlg, m_bShowDiscount, 0x3d8);
	CHECK_FIELD(FrBongdariShopDlg, m_bShowBonusDiscount, 0x3d9);
	CHECK_FIELD(FrBongdariShopDlg, m_discountTime, 0x3dc);
	CHECK_FIELD(FrBongdariShopDlg, m_discountState, 0x3e0);
END_LAYOUT_CHECK

// Help pages of the Bongdari shop; its size (0x148) is the `new` of
// FrAboutShopDlgMakeInstance.
class FrAboutShopDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // bongdarishopdlg.h:246

	FrAboutShopDlg(void);   // bongdarishopdlg.cpp:2855
	virtual ~FrAboutShopDlg(void);   // bongdarishopdlg.cpp:2869

	void SetImgAboutIndex(int index);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // bongdarishopdlg.cpp:2821

	void OnAboutShopInit(int);   // bongdarishopdlg.cpp:2873
	void OnAboutViewerInit(intptr_t);   // bongdarishopdlg.cpp:2877
	void OnIndex01Init(intptr_t);   // bongdarishopdlg.cpp:2883
	void OnIndex02Init(intptr_t);   // bongdarishopdlg.cpp:2893
	void OnIndex03Init(intptr_t);   // bongdarishopdlg.cpp:2903
	void OnIndex04Init(intptr_t);   // bongdarishopdlg.cpp:2913
	void OnIndex05Init(intptr_t);   // bongdarishopdlg.cpp:2923
	void OnIndex06Init(intptr_t);   // bongdarishopdlg.cpp:2933
	void OnIndex07Init(intptr_t);   // bongdarishopdlg.cpp:2943
	void OnIndex08Init(intptr_t);   // bongdarishopdlg.cpp:2953
	void OnIndex09Init(intptr_t);   // bongdarishopdlg.cpp:2963
	void OnIndex10Init(intptr_t);   // bongdarishopdlg.cpp:2973
	void OnIndex01BnUp(void);   // bongdarishopdlg.cpp:2888
	void OnIndex02BnUp(void);   // bongdarishopdlg.cpp:2898
	void OnIndex03BnUp(void);   // bongdarishopdlg.cpp:2908
	void OnIndex04BnUp(void);   // bongdarishopdlg.cpp:2918
	void OnIndex05BnUp(void);   // bongdarishopdlg.cpp:2928
	void OnIndex06BnUp(void);   // bongdarishopdlg.cpp:2938
	void OnIndex07BnUp(void);   // bongdarishopdlg.cpp:2948
	void OnIndex08BnUp(void);   // bongdarishopdlg.cpp:2958
	void OnIndex09BnUp(void);   // bongdarishopdlg.cpp:2968
	void OnIndex10BnUp(void);   // bongdarishopdlg.cpp:2978
	void OnBackwardBnInit(intptr_t);   // bongdarishopdlg.cpp:2983
	void OnBackwardBnUp(void);   // bongdarishopdlg.cpp:2988
	void OnForwardBnInit(intptr_t);   // bongdarishopdlg.cpp:2994
	void OnForwardBnUp(void);   // bongdarishopdlg.cpp:2999
	void OnOkBnInit(intptr_t);   // bongdarishopdlg.cpp:3005
	void OnOkBnUp(void);   // bongdarishopdlg.cpp:3010
	virtual bool OnInit(void);   // bongdarishopdlg.cpp:3015

private:
	FrTextButton* m_pIndexBn[10];      // 0x110, page buttons 01-10
	int m_curIndex;                    // 0x138, page shown
	FrButton* m_pBackwardBn;           // 0x13c, set by OnBackwardBnInit
	FrButton* m_pForwardBn;            // 0x140
	FrViewer* m_pAboutViewer;          // 0x144, the page image
};

CHECK_SIZE(FrAboutShopDlg, 0x148);
BEGIN_LAYOUT_CHECK(FrAboutShopDlg)
	CHECK_FIELD(FrAboutShopDlg, m_pIndexBn, 0x110);
	CHECK_FIELD(FrAboutShopDlg, m_curIndex, 0x138);
	CHECK_FIELD(FrAboutShopDlg, m_pBackwardBn, 0x13c);
	CHECK_FIELD(FrAboutShopDlg, m_pForwardBn, 0x140);
	CHECK_FIELD(FrAboutShopDlg, m_pAboutViewer, 0x144);
END_LAYOUT_CHECK

// List of every item of the shop; its size (0x1e8) is the `new` of
// FrTotalItemDlgMakeInstance. Only the members this unit defines so far are
// declared.
class FrTotalItemDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // bongdarishopdlg.h:309

	class MiniBtn {
	public:
		MiniBtn(void);

		WRect rect;               // 0x0
		const Bitmap* pBitmap;    // 0x10
		UNKNOWN(0x14, 0x18);
	};

	FrTotalItemDlg(void);
	virtual ~FrTotalItemDlg(void);

	void SetListItem(std::list<sBonusBall>* pList);   // bongdarishopdlg.cpp:3330

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // bongdarishopdlg.cpp:3080

	void OnTotalItemInit(int);   // bongdarishopdlg.cpp:3120
	void OnTotalItemListInit(intptr_t);   // bongdarishopdlg.cpp:3124
	void OnTotalItemListOwnerDraw(int);   // bongdarishopdlg.cpp:3135
	void OnTotalItemListRBnDown(void);   // bongdarishopdlg.cpp:3213
	void OpenInformation(unsigned long typeId);
	void OnOkBnInit(intptr_t);   // bongdarishopdlg.cpp:3314
	void OnOkBnUp(void);   // bongdarishopdlg.cpp:3319
	virtual bool OnInit(void);   // bongdarishopdlg.cpp:3324

private:
	FrListBox* m_pTotalItemList;       // 0x110, one line per bonus ball
	const Bitmap* m_pIconBase;         // 0x114, "shop_icon_base02"
	WTitleFont* m_pTitleFont;          // 0x118, digits font
	PADDING(0x11c, 0x164);
	MiniBtn m_deleteBtn;               // 0x164, "delete" icon of the items
	MiniBtn m_clockBtn;                // 0x17c, "clock" icon of the items
	PADDING(0x194, 0x1dc);
	std::list<sBonusBall> m_bonusBallList;   // 0x1dc, copied by SetListItem
};

CHECK_SIZE(FrTotalItemDlg, 0x1e8);
BEGIN_LAYOUT_CHECK(FrTotalItemDlg)
	CHECK_FIELD(FrTotalItemDlg, m_pTotalItemList, 0x110);
	CHECK_FIELD(FrTotalItemDlg, m_pIconBase, 0x114);
	CHECK_FIELD(FrTotalItemDlg, m_deleteBtn, 0x164);
	CHECK_FIELD(FrTotalItemDlg, m_clockBtn, 0x17c);
	CHECK_FIELD(FrTotalItemDlg, m_pTitleFont, 0x118);
	CHECK_FIELD(FrTotalItemDlg, m_bonusBallList, 0x1dc);
END_LAYOUT_CHECK

// Notice pages of the Bongdari shop; its size (0x148) is the `new` of
// FrBongdarishopNoticeDlgMakeInstance.
class FrBongdarishopNoticeDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
	friend class FrBongdariShopDlg;   // sets m_flag (OpenBongdarishopNoticeDlg)
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // bongdarishopdlg.h:371

	FrBongdarishopNoticeDlg(void);   // bongdarishopdlg.cpp:3382
	virtual ~FrBongdarishopNoticeDlg(void);   // bongdarishopdlg.cpp:3395

	void LoadInitFile(const char* fileName);
	void SetImgNotice(int index);
	void SetButtonType(bool b);   // bongdarishopdlg.cpp:3595
	void PrevIndex(void);   // bongdarishopdlg.cpp:3652
	void NextIndex(void);   // bongdarishopdlg.cpp:3658
	unsigned long GetMoveLayoutTid(void) { return m_moveLayoutTid; }   // bongdarishopdlg.h:418

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // bongdarishopdlg.cpp:3360

	void OnBongdarishopNoticeInit(int);   // bongdarishopdlg.cpp:3399
	void OnNoticeViewerInit(intptr_t);   // bongdarishopdlg.cpp:3403
	void OnNoticeViewerBnUp(void);   // bongdarishopdlg.cpp:3408
	void OnListIndexInit(intptr_t);   // bongdarishopdlg.cpp:3434
	void OnListIndexLBnUp(void);   // bongdarishopdlg.cpp:3439
	void OnListIndexOwnerDraw(intptr_t);   // bongdarishopdlg.cpp:3453
	void OnBackwardBnInit(intptr_t);   // bongdarishopdlg.cpp:3483
	void OnBackwardBnUp(void);   // bongdarishopdlg.cpp:3488
	void OnForwardBnInit(intptr_t);   // bongdarishopdlg.cpp:3494
	void OnForwardBnUp(void);   // bongdarishopdlg.cpp:3499
	void OnInBnInit(intptr_t);   // bongdarishopdlg.cpp:3505
	void OnOutBnInit(intptr_t);   // bongdarishopdlg.cpp:3515
	void OnOutBnUp(void);   // bongdarishopdlg.cpp:3520
	void OnOkBnInit(intptr_t);   // bongdarishopdlg.cpp:3534
	void OnInBnUp(void);   // bongdarishopdlg.cpp:3510
	void OnOkBnUp(void);   // bongdarishopdlg.cpp:3539
	virtual bool OnInit(void);   // bongdarishopdlg.cpp:3544

private:
	std::vector<sNoticeImage> m_noticeImages;   // 0x110
	FrListBox* m_pListIndex;           // 0x120, page list (OnListIndexLBnUp)
	int m_curIndex;                    // 0x124, page shown
	unsigned long m_moveLayoutTid;     // 0x128, set by OnNoticeViewerBnUp
	FrButton* m_pBackwardBn;           // 0x12c, set by OnBackwardBnInit
	FrButton* m_pForwardBn;            // 0x130
	FrViewer* m_pNoticeViewer;         // 0x134, set by OnNoticeViewerInit
	// FrButton controls; FrButton is not transcribed yet, the code only
	// uses their FrWnd virtuals.
	FrWnd* m_pInBn;                    // 0x138
	FrWnd* m_pOutBn;                   // 0x13c
	FrWnd* m_pOkBn;                    // 0x140
	PADDING(0x144, 0x148);
};

CHECK_SIZE(FrBongdarishopNoticeDlg, 0x148);
BEGIN_LAYOUT_CHECK(FrBongdarishopNoticeDlg)
	CHECK_FIELD(FrBongdarishopNoticeDlg, m_noticeImages, 0x110);
	CHECK_FIELD(FrBongdarishopNoticeDlg, m_pListIndex, 0x120);
	CHECK_FIELD(FrBongdarishopNoticeDlg, m_curIndex, 0x124);
	CHECK_FIELD(FrBongdarishopNoticeDlg, m_moveLayoutTid, 0x128);
	CHECK_FIELD(FrBongdarishopNoticeDlg, m_pBackwardBn, 0x12c);
	CHECK_FIELD(FrBongdarishopNoticeDlg, m_pForwardBn, 0x130);
	CHECK_FIELD(FrBongdarishopNoticeDlg, m_pNoticeViewer, 0x134);
	CHECK_FIELD(FrBongdarishopNoticeDlg, m_pInBn, 0x138);
	CHECK_FIELD(FrBongdarishopNoticeDlg, m_pOutBn, 0x13c);
	CHECK_FIELD(FrBongdarishopNoticeDlg, m_pOkBn, 0x140);
END_LAYOUT_CHECK

// Rare item announcement of the Bongdari shop; its size (0x120) is the `new`
// of FrBongdarishopRareItemDlgMakeInstance.
class FrBongdarishopRareItemDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // bongdarishopdlg.h:434

	FrBongdarishopRareItemDlg(void);   // bongdarishopdlg.cpp:3677
	virtual ~FrBongdarishopRareItemDlg(void);   // bongdarishopdlg.cpp:3684

	void SetRareItem(sBonusBall* pBall);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // bongdarishopdlg.cpp:3665

	void OnBongdarishopRareItemInit(int);   // bongdarishopdlg.cpp:3688
	void OnRareItemIconInit(intptr_t);   // bongdarishopdlg.cpp:3692
	void OnRareItemNameInit(intptr_t);   // bongdarishopdlg.cpp:3697
	void OnRareItemAbilityInit(intptr_t);   // bongdarishopdlg.cpp:3702
	void OnOkBnInit(intptr_t);   // bongdarishopdlg.cpp:3707
	void OnOkBnUp(void);   // bongdarishopdlg.cpp:3712
	virtual bool OnInit(void);   // bongdarishopdlg.cpp:3717

private:
	FrArea* m_pRareItemIcon;           // 0x110
	FrEdit* m_pRareItemName;           // 0x114
	FrEdit* m_pRareItemAbility;        // 0x118
	PADDING(0x11c, 0x120);
};

CHECK_SIZE(FrBongdarishopRareItemDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrBongdarishopRareItemDlg)
	CHECK_FIELD(FrBongdarishopRareItemDlg, m_pRareItemIcon, 0x110);
	CHECK_FIELD(FrBongdarishopRareItemDlg, m_pRareItemAbility, 0x118);
END_LAYOUT_CHECK

// Warning of the Bongdari shop; its size (0x120) is the `new` of
// FrBongdarishopWarningDlgMakeInstance.
class FrBongdarishopWarningDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // bongdarishopdlg.h:472

	FrBongdarishopWarningDlg(void);   // bongdarishopdlg.cpp:3876
	virtual ~FrBongdarishopWarningDlg(void);   // bongdarishopdlg.cpp:3899

	void SetImgWarningBack(int, int);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // bongdarishopdlg.cpp:3865

	void OnBongdarishopWarningInit(int);   // bongdarishopdlg.cpp:3909
	void OnWarningBackInit(intptr_t);   // bongdarishopdlg.cpp:3913
	void OnOkBnInit(intptr_t);   // bongdarishopdlg.cpp:3918
	void OnOkBnUp(void);   // bongdarishopdlg.cpp:3923
	virtual bool OnInit(void);   // bongdarishopdlg.cpp:3937
	void OnWarningBackOwnerDraw(int);   // bongdarishopdlg.cpp:3964

private:
	FrArea* m_pWarningBack;            // 0x110, set by OnWarningBackInit
	WTitleFont* m_pTitleFont;          // 0x114, digits font
	unsigned long m_bonusPrice;        // 0x118, printed on the "bonus" warning (type 2)
	int m_backType;                    // 0x11c, 2 = bonus warning (SetImgWarningBack)
};

CHECK_SIZE(FrBongdarishopWarningDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrBongdarishopWarningDlg)
	CHECK_FIELD(FrBongdarishopWarningDlg, m_pWarningBack, 0x110);
	CHECK_FIELD(FrBongdarishopWarningDlg, m_pTitleFont, 0x114);
	CHECK_FIELD(FrBongdarishopWarningDlg, m_bonusPrice, 0x118);
	CHECK_FIELD(FrBongdarishopWarningDlg, m_backType, 0x11c);
END_LAYOUT_CHECK

// Result of opening the closed bonus balls; its size (0x250) is the `new` of
// FrClosedBonusBallResultDlgMakeInstance.
class FrClosedBonusBallResultDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // bongdarishopdlg.h:510

	class MiniBtn {
	public:
		MiniBtn(void);

		WRect rect;               // 0x0
		const Bitmap* pBitmap;    // 0x10
		UNKNOWN(0x14, 0x18);
	};

	FrClosedBonusBallResultDlg(void);   // bongdarishopdlg.cpp:3995
	virtual ~FrClosedBonusBallResultDlg(void);   // bongdarishopdlg.cpp:4008

	void SetClosedBonusBall(sBonusBall* pBalls);   // bongdarishopdlg.cpp:4135
	void SetEditDataClosedBonusBall(int, int);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // bongdarishopdlg.cpp:3980

	void OnClosedBonusBallResultInit(int);   // bongdarishopdlg.cpp:4012
	void OnClosedBonusBallListInit(intptr_t);   // bongdarishopdlg.cpp:4016
	void OnClosedBonusBallListOwnerDraw(int);   // bongdarishopdlg.cpp:4026
	void OnClosedBonusBallName01Init(intptr_t);   // bongdarishopdlg.cpp:4078
	void OnClosedBonusBallAbility01Init(intptr_t);   // bongdarishopdlg.cpp:4083
	void OnClosedBonusBallName02Init(intptr_t);   // bongdarishopdlg.cpp:4088
	void OnClosedBonusBallAbility02Init(intptr_t);   // bongdarishopdlg.cpp:4093
	void OnOkBnInit(intptr_t);   // bongdarishopdlg.cpp:4098
	void OnOkBnUp(void);   // bongdarishopdlg.cpp:4103
	virtual bool OnInit(void);   // bongdarishopdlg.cpp:4108
	virtual void OnProc(const float dt);   // bongdarishopdlg.cpp:4113

private:
	FrListBox* m_pClosedBonusBallList;   // 0x110, set by OnClosedBonusBallListInit
	FrEdit* m_pNameEdit[2];              // 0x114, "name01", "name02"
	FrEdit* m_pAbilityEdit[2];           // 0x11c, "ability01", "ability02"
	int m_selected;                      // 0x124, -1 = none
	PADDING(0x128, 0x170);
	MiniBtn m_deleteBtn;                 // 0x170, "delete" icon of the items
	MiniBtn m_clockBtn;                  // 0x188, "clock" icon of the items
	PADDING(0x1a0, 0x1e8);
	sBonusBall m_closedBalls[5];         // 0x1e8, zeroed by the constructor, set by SetClosedBonusBall
	PADDING(0x24c, 0x250);
};

CHECK_SIZE(FrClosedBonusBallResultDlg, 0x250);
BEGIN_LAYOUT_CHECK(FrClosedBonusBallResultDlg)
	CHECK_FIELD(FrClosedBonusBallResultDlg, m_pClosedBonusBallList, 0x110);
	CHECK_FIELD(FrClosedBonusBallResultDlg, m_pNameEdit, 0x114);
	CHECK_FIELD(FrClosedBonusBallResultDlg, m_pAbilityEdit, 0x11c);
	CHECK_FIELD(FrClosedBonusBallResultDlg, m_selected, 0x124);
	CHECK_FIELD(FrClosedBonusBallResultDlg, m_deleteBtn, 0x170);
	CHECK_FIELD(FrClosedBonusBallResultDlg, m_clockBtn, 0x188);
	CHECK_FIELD(FrClosedBonusBallResultDlg, m_closedBalls, 0x1e8);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_BONGDARISHOPDLG_H
