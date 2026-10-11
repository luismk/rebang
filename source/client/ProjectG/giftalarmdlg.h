// Original: d:\build\custom\temp\code\client\projectg\giftalarmdlg.h
#ifndef PG_CLIENT_PROJECTG_GIFTALARMDLG_H
#define PG_CLIENT_PROJECTG_GIFTALARMDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include <map>

class Bitmap;
class FrListBox;
class FrStatic;

// Messenger gift-arrival dialog: the list of received gifts and a blinking
// "page" caption. Size 0x138, the `new` of FrGiftAlarmDlgMakeInstance.
class FrGiftAlarmDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // giftalarmdlg.h:8

	FrGiftAlarmDlg(void);
	virtual ~FrGiftAlarmDlg(void);

	virtual void OnProc(const float dt);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void MakeSenderMsg(const char* pSender, char* pLines[]);
	void OnInfoMsgInit(intptr_t param);
	void OnGiftListOwnerDraw(int param);
	void OnGiftListInit(intptr_t param);

	std::map<unsigned long, const Bitmap*> m_bitmapMap;   // 0x110
	FrListBox* m_pGiftList;                               // 0x11C ("giftlist")
	FrStatic* m_pInfoMsg;                                 // 0x120 ("infomsg")
	int m_curGift;                                        // 0x124
	int m_giftCount;                                      // 0x128
	float m_blinkTime;                                    // 0x12C
	const Bitmap* m_pArrivalMailImage;                    // 0x130 "rmr_Arrival_mail"
	const Bitmap* m_pOpenMailImage;                       // 0x134 "rmr_Open_mail"
};

CHECK_SIZE(FrGiftAlarmDlg, 0x138);
BEGIN_LAYOUT_CHECK(FrGiftAlarmDlg)
	CHECK_FIELD(FrGiftAlarmDlg, m_bitmapMap, 0x110);
	CHECK_FIELD(FrGiftAlarmDlg, m_pGiftList, 0x11C);
	CHECK_FIELD(FrGiftAlarmDlg, m_pInfoMsg, 0x120);
	CHECK_FIELD(FrGiftAlarmDlg, m_curGift, 0x124);
	CHECK_FIELD(FrGiftAlarmDlg, m_giftCount, 0x128);
	CHECK_FIELD(FrGiftAlarmDlg, m_blinkTime, 0x12C);
	CHECK_FIELD(FrGiftAlarmDlg, m_pArrivalMailImage, 0x130);
	CHECK_FIELD(FrGiftAlarmDlg, m_pOpenMailImage, 0x134);
END_LAYOUT_CHECK

// Treasure-hunt gift-arrival dialog. Size 0x128, the `new` of
// CTreasureAlarmDlgMakeInstance.
class CTreasureAlarmDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // giftalarmdlg.h:50

	CTreasureAlarmDlg(void);
	virtual ~CTreasureAlarmDlg(void);

	virtual void OnProc(const float dt);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnListBox_Gift_Init(int param);
	void OnListBox_Gift_OwnerDraw(int param);
	void OnStatic_Msg_Init(intptr_t param);

	FrListBox* m_pGiftList;   // 0x110 ("giftlist")
	FrStatic* m_pMsg;         // 0x114 ("infomsg")
	UNKNOWN(0x118, 0x11C);
	int m_giftCount;          // 0x11C
	float m_blinkTime;        // 0x120
	UNKNOWN(0x124, 0x128);
};

CHECK_SIZE(CTreasureAlarmDlg, 0x128);
BEGIN_LAYOUT_CHECK(CTreasureAlarmDlg)
	CHECK_FIELD(CTreasureAlarmDlg, m_pGiftList, 0x110);
	CHECK_FIELD(CTreasureAlarmDlg, m_pMsg, 0x114);
	CHECK_FIELD(CTreasureAlarmDlg, m_giftCount, 0x11C);
	CHECK_FIELD(CTreasureAlarmDlg, m_blinkTime, 0x120);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GIFTALARMDLG_H
