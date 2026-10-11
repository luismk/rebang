// Original: d:\build\custom\temp\code\client\projectg\ghostmain.h
#ifndef PG_CLIENT_PROJECTG_GHOSTMAIN_H
#define PG_CLIENT_PROJECTG_GHOSTMAIN_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "TaskMain.h"
#include "RTTI.h"
#include "AnotherHand.h"
#include "../client/Wangreal/include/wtypes.h"

#include <string>
#include <vector>

class IObject;
class IActor;
class CStatBar;
class CExhibition;
class CPartTidList;
class FrArea;
class FrListBox;
class FrForm;
struct FrListItem;
class MsgObject;

// The ghost (replay) task. Its size (0x434) is the `new` of
// CGhostMainMakeInstance. Only the members the transcribed handlers touch
// are named.
class CGhostMain : public CTaskMain {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI *GetRTTI(void) const { return &m_RTTI; } // ghostmain.h:12

	class cImgBinder {
	public:
		struct sImgInfo {
			sImgInfo(const std::string &strImage, const WRect &rect); // ghostmain.cpp:296

			std::string m_strImage; // 0x00
			WRect m_rect;           // 0x1C
		};

		cImgBinder(int); // ghostmain.cpp:303
		~cImgBinder(void); // ghostmain.cpp:306

		void AddImage(std::string strImage, const WPoint &pos); // ghostmain.cpp:308

		WPoint GetPos(void);                 // ghostmain.cpp:317
		void SetPosX(float x);               // ghostmain.cpp:335
		void AddPos(float dx, float dy);     // ghostmain.cpp:344
		void AddPos(const WPoint &delta);    // ghostmain.cpp:356
		void Render(unsigned char alpha);    // ghostmain.cpp:368

		std::vector<sImgInfo> m_vecImage; // 0x00
	};

	CGhostMain(void);
	virtual ~CGhostMain(void); // ghostmain.cpp:398

	virtual void OnPreLoadInit(void); // ghostmain.cpp:614
	virtual void OnLoad(void);        // ghostmain.cpp:618
	virtual void OnInit(void);        // ghostmain.cpp:622
	virtual void OnDestroy(void);     // ghostmain.cpp:627
	virtual void OnProcess(float dt); // ghostmain.cpp:646
	virtual void OnDisplay(void);     // ghostmain.cpp:753
	virtual void HandleMsg(const MsgObject &msg); // ghostmain.cpp:1485

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP *GetMessageMap(void) const; // ghostmain.cpp:51

	void OnInitStart(int); // ghostmain.cpp:833
	CExhibition *MakePet(FrArea *area, CPartTidList *parts, int);
	void OnExit(void);
	void HidePet(bool wait); // ghostmain.cpp:606
	void OnDownSelectCourse(void); // ghostmain.cpp:945
	void OnDrawTipZone(void); // ghostmain.cpp:1344
	void OnInitGhostMain(int); // ghostmain.cpp:1477
	void OnInitPlayerZone(int); // ghostmain.cpp:895
	void OnDrawPlayerZone(void); // ghostmain.cpp:906
	void OnInitUserEquip(int); // ghostmain.cpp:951
	void OnDrawUserEquip(int); // ghostmain.cpp:957
	void OnInitGhostEquip(int); // ghostmain.cpp:1241
	void OnInitGhostZone(int); // ghostmain.cpp:1297
	void OnDrawGhostZone(void); // ghostmain.cpp:1308
	void OnInitTipZone(int); // ghostmain.cpp:1336
	void OnInitAreaUserStat(int); // ghostmain.cpp:1399
	void OnDrawAreaUserStat(void); // ghostmain.cpp:1405
	void OnInitAreaGhostStat(int); // ghostmain.cpp:1435

private:
	virtual bool OnResultSubEquipBar(int, FrForm *); // ghostmain.cpp:1063
	void OnProcTip(float dt);
	void RefreshAllUccClothes(void);
	void Initialize(void);
	void StatProcess(float dt); // ghostmain.cpp:557
	void UserPetRebuild(void); // ghostmain.cpp:1199
	bool CallBackEquipItemResult(int result, FrForm *form); // ghostmain.cpp:1209
	bool IsFocusToMain(void); // ghostmain.cpp:1231
	void SetUserEquip(PANGYA_ITEM_GROUP group, unsigned long id);
	void SendPacketEquipResult(PANGYA_ITEM_GROUP group, unsigned long typeId); // ghostmain.cpp:1546

	CAnotherHand m_hand;     // 0x1E0, builds the pets in the background
	CExhibition *m_userPet;  // 0x368, deleted by HidePet
	CExhibition *m_ghostPet; // 0x36C, deleted by HidePet
	CPartTidList *m_userParts; // 0x370, the user's pet parts
	CPartTidList *m_ghostParts; // 0x374, the ghost's pet parts
	float m_ghostAlpha;      // 0x378, alpha of the ghost pet, raised by OnProcess
	bool m_ghostVisible;     // 0x37C, set once m_ghostAlpha passes 32; OnDrawGhostZone draws the ghost pet only then
	PADDING(0x37D, 0x380);
	std::vector<FrArea *> m_areas; // 0x380, [0] player zone, [1] ghost zone, [2] user stat, [3] ghost stat, [4] tip zone
	cImgBinder *m_userStatImage;  // 0x390, drawn by OnDrawAreaUserStat, deleted by OnDestroy
	cImgBinder *m_ghostStatImage; // 0x394, deleted by OnDestroy
	float m_userStatAlpha;   // 0x398, alpha of m_userStatImage
	float m_userStatStopX;  // 0x39C, x where m_userStatImage stops
	float m_ghostStatStopX; // 0x3A0, x where m_ghostStatImage stops
	float m_userStatSpeed;  // 0x3A4, slide speed of m_userStatImage
	float m_ghostStatSpeed; // 0x3A8, slide speed of m_ghostStatImage
	bool m_showUserStat;     // 0x3AC, OnDrawAreaUserStat draws the stat bar, level and name only when set
	PADDING(0x3AD, 0x3B0);
	float m_statTime;        // 0x3B0, grows to 3 while the stat images slide in
	float m_statSpeedTime;   // 0x3B4, grows to 4 (at 1.2/s) while the stat images slide in
	FrListBox *m_userEquip;  // 0x3B8
	FrListBox *m_ghostEquip; // 0x3BC
	FrListItem *m_selectedIcon; // 0x3C0, list item whose icon OnResultSubEquipBar resets
	std::vector<std::string> m_userIcons;  // 0x3C4, destroyed by ~CGhostMain
	std::vector<std::string> m_ghostIcons; // 0x3D4, destroyed by ~CGhostMain
	float m_equipAlpha;      // 0x3E4, alpha of the equip icons drawn by OnDrawUserEquip
	FrForm *m_equipDlg;      // 0x3E8, the equip dialog while CallBackEquipItemResult waits for it
	CStatBar *m_userStat;  // 0x3EC, advanced by StatProcess
	CStatBar *m_ghostStat; // 0x3F0, advanced by StatProcess
	PADDING(0x3F4, 0x404);
	int m_unknown404;        // 0x404, cleared by the constructor
	PADDING(0x408, 0x424);
	cImgBinder *m_tipImage; // 0x424, drawn by OnDrawTipZone
	float m_tipAlpha;       // 0x428
	PADDING(0x42C, 0x434);
};

IObject *CGhostMainMakeInstance(void);

CHECK_SIZE(CGhostMain::cImgBinder::sImgInfo, 0x2C);
CHECK_FIELD(CGhostMain::cImgBinder::sImgInfo, m_strImage, 0x00);
CHECK_FIELD(CGhostMain::cImgBinder::sImgInfo, m_rect, 0x1C);
CHECK_SIZE(CGhostMain::cImgBinder, 0x10);
CHECK_FIELD(CGhostMain::cImgBinder, m_vecImage, 0x00);
CHECK_SIZE(CGhostMain, 0x434);
BEGIN_LAYOUT_CHECK(CGhostMain)
	CHECK_FIELD(CGhostMain, m_hand, 0x1E0);
	CHECK_FIELD(CGhostMain, m_userPet, 0x368);
	CHECK_FIELD(CGhostMain, m_ghostPet, 0x36C);
	CHECK_FIELD(CGhostMain, m_userParts, 0x370);
	CHECK_FIELD(CGhostMain, m_ghostParts, 0x374);
	CHECK_FIELD(CGhostMain, m_areas, 0x380);
	CHECK_FIELD(CGhostMain, m_userStatImage, 0x390);
	CHECK_FIELD(CGhostMain, m_ghostStatImage, 0x394);
	CHECK_FIELD(CGhostMain, m_userStatAlpha, 0x398);
	CHECK_FIELD(CGhostMain, m_showUserStat, 0x3AC);
	CHECK_FIELD(CGhostMain, m_userEquip, 0x3B8);
	CHECK_FIELD(CGhostMain, m_ghostEquip, 0x3BC);
	CHECK_FIELD(CGhostMain, m_userIcons, 0x3C4);
	CHECK_FIELD(CGhostMain, m_ghostIcons, 0x3D4);
	CHECK_FIELD(CGhostMain, m_userStat, 0x3EC);
	CHECK_FIELD(CGhostMain, m_ghostStat, 0x3F0);
	CHECK_FIELD(CGhostMain, m_tipImage, 0x424);
	CHECK_FIELD(CGhostMain, m_tipAlpha, 0x428);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GHOSTMAIN_H
