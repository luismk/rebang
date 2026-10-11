// Original: d:\build\custom\temp\code\client\projectg\christamseventdlg.h
#ifndef PG_CLIENT_PROJECTG_CHRISTAMSEVENTDLG_H
#define PG_CLIENT_PROJECTG_CHRISTAMSEVENTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "../client/Wangreal/include/wfont.h"

class FrButton;
class FrArea;

// Christmas event dialog with falling snow; its size (0x150) is the `new`
// of FrChristmasEventDlgMakeInstance.
class FrChristmasEventDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	FrChristmasEventDlg(void);
	virtual ~FrChristmasEventDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnCloseBtnInit(intptr_t param);
	void OnCloseBtnUp(void);
	void OnCaptionInit(intptr_t param);
	void OnXmasBtn1Init(intptr_t param);
	void OnXmasBtn2Init(intptr_t param);
	void OnXmasBtn3Init(intptr_t param);
	void OnXmasEx1Init(intptr_t param);
	void OnXmasEx2Init(intptr_t param);
	void OnXmasEx3Init(intptr_t param);
	void OnSocks1AreaInit(intptr_t param);
	void OnSocks2AreaInit(intptr_t param);
	void OnSnowFallDraw(void);

private:
	virtual bool OnInit(void);
	virtual void OnDraw(void);
	virtual void OnProc(const float dt);

	tagWTITLEFONT m_titleFontInfo;     // 0x110, "[font_hole_skins_red.jpg", 16x16
	WTitleFont* m_pTitleFont;          // 0x130
	FrButton* m_pXmasBtn1;             // 0x134
	FrButton* m_pXmasBtn2;             // 0x138
	FrButton* m_pXmasBtn3;             // 0x13c
	FrArea* m_pXmasEx1;                // 0x140
	FrArea* m_pXmasEx2;                // 0x144
	FrArea* m_pXmasEx3;                // 0x148
	bool m_bXmasEx1Shown;              // 0x14c, m_pXmasEx1 shown while m_pXmasBtn1 is hovered
	bool m_bXmasEx2Shown;              // 0x14d
	bool m_bXmasEx3Shown;              // 0x14e
	UNKNOWN(0x14f, 0x150);
};

CHECK_SIZE(FrChristmasEventDlg, 0x150);
BEGIN_LAYOUT_CHECK(FrChristmasEventDlg)
	CHECK_FIELD(FrChristmasEventDlg, m_titleFontInfo, 0x110);
	CHECK_FIELD(FrChristmasEventDlg, m_pTitleFont, 0x130);
	CHECK_FIELD(FrChristmasEventDlg, m_pXmasBtn1, 0x134);
	CHECK_FIELD(FrChristmasEventDlg, m_pXmasEx3, 0x148);
	CHECK_FIELD(FrChristmasEventDlg, m_bXmasEx1Shown, 0x14c);
	CHECK_FIELD(FrChristmasEventDlg, m_bXmasEx2Shown, 0x14d);
	CHECK_FIELD(FrChristmasEventDlg, m_bXmasEx3Shown, 0x14e);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CHRISTAMSEVENTDLG_H
