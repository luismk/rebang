// Original: d:\build\custom\temp\code\client\projectg\valentineeventdlg.cpp
#include "ValentineEventDlg.h"
#include "ObjectFactory.h"
#include "wresrcmng.h"
#include "DetailDlg.h"
#include "SharedDoc.h"
#include "frgraphicinterface.h"
#include "../client/Wangreal/include/bitmap.h"
#include "Packet.h"
#include "commonutil.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "../client/Wangreal/include/wview.h"

// valentineeventdlg.cpp:32 (run-time type and factory registration of FrValEventDlg)
const WRTTI FrValEventDlg::m_RTTI("FrValEventDlg", &FrForm::m_RTTI);

IObject* FrValEventDlgMakeInstance(void) {
	return new FrValEventDlg;
}

struct __sFrValEventDlg {
	__sFrValEventDlg(void) {
		ObjectFactory().AddObjectFunctor(FrValEventDlgMakeInstance, "FrValEventDlg");
	}
};
static __sFrValEventDlg s_registerFrValEventDlg;

// valentineeventdlg.cpp:34
const sFRESH_MSGMAP* FrValEventDlg::GetMessageMap(void) const {
	return &FrValEventDlg::_MsgMap;
}

sFRESH_MSGMAP FrValEventDlg::_MsgMap = { &FrForm::_MsgMap, &FrValEventDlg::_MsgEntries[0] };

sFRESH_ENTRY FrValEventDlg::_MsgEntries[] = {
	// valentineeventdlg.cpp:36
	{ "back_img", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrValEventDlg::OnBackImgInit },

	// valentineeventdlg.cpp:37
	{ "back_img", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrValEventDlg::OnBackImgOwnerDraw },

	// valentineeventdlg.cpp:38
	{ "gauge_img", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrValEventDlg::OnGaugeImgOwnerDraw },

	// valentineeventdlg.cpp:40
	{ "nuri_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrValEventDlg::OnNuriBtnInit },

	// valentineeventdlg.cpp:41
	{ "nuri_btn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrValEventDlg::OnNuriBtnLBDown },

	// valentineeventdlg.cpp:43
	{ "azer_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrValEventDlg::OnAzerBtnInit },

	// valentineeventdlg.cpp:44
	{ "azer_btn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrValEventDlg::OnAzerBtnLBDown },

	// valentineeventdlg.cpp:46
	{ "max_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrValEventDlg::OnMaxBtnInit },

	// valentineeventdlg.cpp:47
	{ "max_btn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrValEventDlg::OnMaxBtnLBDown },

	// valentineeventdlg.cpp:49
	{ "kaz_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrValEventDlg::OnKazBtnInit },

	// valentineeventdlg.cpp:50
	{ "kaz_btn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrValEventDlg::OnKazBtnLBDown },

	// valentineeventdlg.cpp:52
	{ "hana_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrValEventDlg::OnHanaBtnInit },

	// valentineeventdlg.cpp:53
	{ "hana_btn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrValEventDlg::OnHanaBtnLBDown },

	// valentineeventdlg.cpp:55
	{ "cecil_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrValEventDlg::OnCecilBtnInit },

	// valentineeventdlg.cpp:56
	{ "cecil_btn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrValEventDlg::OnCecilBtnLBDown },

	// valentineeventdlg.cpp:58
	{ "kooh_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrValEventDlg::OnKoohBtnInit },

	// valentineeventdlg.cpp:59
	{ "kooh_btn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrValEventDlg::OnKoohBtnLBDown },

	// valentineeventdlg.cpp:61
	{ "arin_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrValEventDlg::OnArinBtnInit },

	// valentineeventdlg.cpp:62
	{ "arin_btn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrValEventDlg::OnArinBtnLBDown },

	// valentineeventdlg.cpp:64
	{ "lucia_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrValEventDlg::OnLuciaBtnInit },

	// valentineeventdlg.cpp:65
	{ "lucia_btn", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrValEventDlg::OnLuciaBtnLBDown },

	// valentineeventdlg.cpp:67
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// valentineeventdlg.cpp:71
FrValEventDlg::FrValEventDlg(void) {
	m_nuriCount = 0;
	m_azerCount = 0;
	m_maxCount = 0;
	m_kazCount = 0;
	m_hanaCount = 0;
	m_cecilCount = 0;
	m_koohCount = 0;
	m_arinCount = 0;
	m_luciaCount = 0;
}

// valentineeventdlg.cpp:85
FrValEventDlg::~FrValEventDlg(void) {
}

// valentineeventdlg.cpp:89
bool FrValEventDlg::OnInit(void) {
	SetCaption("White Day Event");

	return FrForm::OnInit();
}

// valentineeventdlg.cpp:100
void FrValEventDlg::BindButton(FrButton*& button, int id) {
	button = DYNAMIC_CAST(FrButton, id);

	if (button) {

		button->SetPushDelay(0.1f);
		button->SetVisible(false);
	}
}

// valentineeventdlg.cpp:112
void FrValEventDlg::BuildHeartCount(void) {
	std::list<sItemInfo>& items = WSingleton<CSharedDoc>::Instance()->m_itemInfoList;
	std::list<sItemInfo>::iterator it = items.begin();
	std::list<sItemInfo>::iterator itEnd = items.end();

	for (; it != itEnd; ++it) {
		switch (it->m_typeId) {
		case 0x1A0000BE:
			m_nuriCount = (unsigned char)it->m_clubStatLevel[0];
			if (m_nuriCount >= 10)
				m_nuriCount = 10;
			break;

		case 0x1A0000BF:
			m_azerCount = (unsigned char)it->m_clubStatLevel[0];
			if (m_azerCount >= 10)
				m_azerCount = 10;
			break;

		case 0x1A0000C0:
			m_maxCount = (unsigned char)it->m_clubStatLevel[0];
			if (m_maxCount >= 10)
				m_maxCount = 10;
			break;

		case 0x1A0000C1:
			m_kazCount = (unsigned char)it->m_clubStatLevel[0];
			if (m_kazCount >= 10)
				m_kazCount = 10;
			break;

		case 0x1A0000B6:
			m_hanaCount = (unsigned char)it->m_clubStatLevel[0];
			if (m_hanaCount >= 10)
				m_hanaCount = 10;
			break;

		case 0x1A0000B7:
			m_cecilCount = (unsigned char)it->m_clubStatLevel[0];
			if (m_cecilCount >= 10)
				m_cecilCount = 10;
			break;

		case 0x1A0000B8:
			m_koohCount = (unsigned char)it->m_clubStatLevel[0];
			if (m_koohCount >= 10)
				m_koohCount = 10;
			break;

		case 0x1A0000B9:
			m_arinCount = (unsigned char)it->m_clubStatLevel[0];
			if (m_arinCount >= 10)
				m_arinCount = 10;
			break;

		case 0x1A0000BA:
			m_luciaCount = (unsigned char)it->m_clubStatLevel[0];
			if (m_luciaCount >= 10)
				m_luciaCount = 10;
			break;

		case 0x1A0000BB:
			m_nellCount = (unsigned char)it->m_clubStatLevel[0];
			if (m_nellCount >= 10)
				m_nellCount = 10;
			break;
		}
	}
}

// valentineeventdlg.cpp:197
void FrValEventDlg::DrawInDlg(const char* name, WPoint& pt) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {

		float x = m_rect.x + 2.0f;
		float y = m_rect.y + 23.0f;

		const Bitmap* bmp = g_pFresh->GetBitmap(name);

		if (bmp) {

			pGDI->DrawTexture(bmp, WRect(x + pt.x, y + pt.y, bmp->Width(), bmp->Height()), 0xFFFFFFFF, 0);
		}
	}
}

// valentineeventdlg.cpp:214
void FrValEventDlg::DrawGauge(WPoint& pt, unsigned char count) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {

		const Bitmap* bmp = g_pFresh->GetBitmap("Heart_Gage");

		WPoint pos;
		pos.x = m_rect.x + 2.0f;
		pos.y = m_rect.y + 23.0f;

		float height = (float)bmp->Height();

		float offset = (10 - count) * height * 0.1f;
		pGDI->DrawTexture(bmp, WRect(pos.x + pt.x, offset + pt.y + pos.y, bmp->Width(), bmp->Height() - offset), 0xFFFFFFFF, 0);
	}
}

// valentineeventdlg.cpp:236
void FrValEventDlg::OnBackImgInit(intptr_t id) {
	BuildHeartCount();
}

// valentineeventdlg.cpp:243
void FrValEventDlg::OnBackImgOwnerDraw(int param) {
	if (g_pFresh->GetManager()->GetGDI()) {

		DrawInDlg("Frame_WhiteDay_01", WPoint(0.0f, 0.0f));
		DrawInDlg("Frame_WhiteDay_02", WPoint(256.0f, 0.0f));
		DrawInDlg("Frame_WhiteDay_03", WPoint(512.0f, 0.0f));
		DrawInDlg("Frame_WhiteDay_04", WPoint(0.0f, 256.0f));
		DrawInDlg("Frame_WhiteDay_05", WPoint(256.0f, 256.0f));
		DrawInDlg("Frame_WhiteDay_06", WPoint(512.0f, 256.0f));
	}
}

// valentineeventdlg.cpp:266
void FrValEventDlg::OnGaugeImgOwnerDraw(int param) {
	if (g_pFresh->GetManager()->GetGDI()) {

		if (m_giftFlag & 0x20) {

			DrawGauge(WPoint(25.0f, 373.0f), 10);
		} else if (m_nuriCount < 10) {

			DrawGauge(WPoint(25.0f, 373.0f), m_nuriCount);
			DrawInDlg("Character_Nuri", WPoint(25.0f, 267.0f));
		}

		if (m_giftFlag & 0x40) {

			DrawGauge(WPoint(227.0f, 373.0f), 10);
		} else if (m_azerCount < 10) {

			DrawGauge(WPoint(227.0f, 373.0f), m_azerCount);
			DrawInDlg("Character_Azer", WPoint(227.0f, 267.0f));
		}

		if (m_giftFlag & 0x80) {

			DrawGauge(WPoint(126.0f, 373.0f), 10);
		} else if (m_maxCount < 10) {

			DrawGauge(WPoint(126.0f, 373.0f), m_maxCount);
			DrawInDlg("Character_Max", WPoint(126.0f, 267.0f));
		}

		if (m_giftFlag & 0x100) {

			DrawGauge(WPoint(328.0f, 373.0f), 10);
		} else if (m_kazCount < 10) {

			DrawGauge(WPoint(328.0f, 373.0f), m_kazCount);
			DrawInDlg("Character_Kaz", WPoint(328.0f, 267.0f));
		}

		DrawInDlg("Frame_Candy_01", WPoint(18.0f, 366.0f));
		DrawInDlg("Frame_Candy_02", WPoint(274.0f, 366.0f));
	}
}

// valentineeventdlg.cpp:394
void FrValEventDlg::CheckButtonClick(void) {
	if (m_nuriCount >= 10) {

		if (m_giftFlag & 0x20) {

			m_pNuriBtn->SetVisible(false);
			m_pNuriBtn->Enable(false);
		} else {

			m_pNuriBtn->SetVisible(true);
			m_pNuriBtn->Enable(true);
		}
	}

	if (m_azerCount >= 10) {

		if (m_giftFlag & 0x40) {

			m_pAzerBtn->SetVisible(false);
			m_pAzerBtn->Enable(false);
		} else {

			m_pAzerBtn->SetVisible(true);
			m_pAzerBtn->Enable(true);
		}
	}

	if (m_maxCount >= 10) {

		if (m_giftFlag & 0x80) {

			m_pMaxBtn->SetVisible(false);
			m_pMaxBtn->Enable(false);
		} else {

			m_pMaxBtn->SetVisible(true);
			m_pMaxBtn->Enable(true);
		}
	}

	if (m_kazCount >= 10) {

		if (m_giftFlag & 0x100) {

			m_pKazBtn->SetVisible(false);
			m_pKazBtn->Enable(false);
		} else {

			m_pKazBtn->SetVisible(true);
			m_pKazBtn->Enable(true);
		}
	}
}

// valentineeventdlg.cpp:454
void FrValEventDlg::OnNuriBtnInit(intptr_t id) {
	BindButton(m_pNuriBtn, id);
}

// valentineeventdlg.cpp:466
void FrValEventDlg::OnNuriBtnLBDown(void) {
	if (m_pNuriBtn) {

		m_pNuriBtn->SetVisible(false);
	}

	WSendPacket packet(0xDD);
	packet.Encode4(0);
	packet.Send(SEND_TO_DEFAULT);

	m_giftFlag |= 0x20;
}

// valentineeventdlg.cpp:485
void FrValEventDlg::OnAzerBtnInit(intptr_t id) {
	BindButton(m_pAzerBtn, id);
}

// valentineeventdlg.cpp:497
void FrValEventDlg::OnAzerBtnLBDown(void) {
	if (m_pAzerBtn) {

		m_pAzerBtn->SetVisible(false);
	}

	WSendPacket packet(0xDD);
	packet.Encode4(2);
	packet.Send(SEND_TO_DEFAULT);

	m_giftFlag |= 0x40;
}

// valentineeventdlg.cpp:516
void FrValEventDlg::OnMaxBtnInit(intptr_t id) {
	BindButton(m_pMaxBtn, id);
}

// valentineeventdlg.cpp:528
void FrValEventDlg::OnMaxBtnLBDown(void) {
	if (m_pMaxBtn) {

		m_pMaxBtn->SetVisible(false);
	}

	WSendPacket packet(0xDD);
	packet.Encode4(4);
	packet.Send(SEND_TO_DEFAULT);

	m_giftFlag |= 0x80;
}

// valentineeventdlg.cpp:548
void FrValEventDlg::OnKazBtnInit(intptr_t id) {
	BindButton(m_pKazBtn, id);
}

// valentineeventdlg.cpp:560
void FrValEventDlg::OnKazBtnLBDown(void) {
	if (m_pKazBtn) {

		m_pKazBtn->SetVisible(false);
	}

	WSendPacket packet(0xDD);
	packet.Encode4(7);
	packet.Send(SEND_TO_DEFAULT);

	m_giftFlag |= 0x100;
}

// valentineeventdlg.cpp:580
void FrValEventDlg::OnHanaBtnInit(intptr_t id) {
	BindButton(m_pHanaBtn, id);

	if (m_pHanaBtn && m_hanaCount >= 10) {
		m_pHanaBtn->SetVisible(true);
		m_pHanaBtn->Enable(true);

		m_pHanaBtn->SetVisible(false);
	}
}

// valentineeventdlg.cpp:594
void FrValEventDlg::OnHanaBtnLBDown(void) {
}

// valentineeventdlg.cpp:612
void FrValEventDlg::OnArinBtnInit(intptr_t id) {
	BindButton(m_pArinBtn, id);

	if (m_pArinBtn && m_arinCount >= 10) {
		m_pArinBtn->SetVisible(true);
		m_pArinBtn->Enable(true);

		m_pArinBtn->SetVisible(false);
	}
}

// valentineeventdlg.cpp:626
void FrValEventDlg::OnArinBtnLBDown(void) {
}

// valentineeventdlg.cpp:645
void FrValEventDlg::OnCecilBtnInit(intptr_t id) {
	BindButton(m_pCecilBtn, id);

	if (m_pCecilBtn && m_cecilCount >= 10) {
		m_pCecilBtn->SetVisible(true);
		m_pCecilBtn->Enable(true);

		m_pCecilBtn->SetVisible(false);
	}
}

// valentineeventdlg.cpp:659
void FrValEventDlg::OnCecilBtnLBDown(void) {
}

// valentineeventdlg.cpp:678
void FrValEventDlg::OnKoohBtnInit(intptr_t id) {
	BindButton(m_pKoohBtn, id);

	if (m_pKoohBtn && m_koohCount >= 10) {
		m_pKoohBtn->SetVisible(true);
		m_pKoohBtn->Enable(true);

		m_pKoohBtn->SetVisible(false);
	}
}

// valentineeventdlg.cpp:692
void FrValEventDlg::OnKoohBtnLBDown(void) {
}

// valentineeventdlg.cpp:711
void FrValEventDlg::OnLuciaBtnInit(intptr_t id) {
	BindButton(m_pLuciaBtn, id);

	if (m_pLuciaBtn && m_luciaCount >= 10) {
		m_pLuciaBtn->SetVisible(true);
		m_pLuciaBtn->Enable(true);

		m_pLuciaBtn->SetVisible(false);
	}
}

// valentineeventdlg.cpp:725
void FrValEventDlg::OnLuciaBtnLBDown(void) {
}

// valentineeventdlg.cpp:742
void FrValEventDlg::OnNellBtnInit(int id) {
	BindButton(m_pNellBtn, id);

	if (m_pNellBtn && m_nellCount >= 10) {
		m_pNellBtn->SetVisible(true);
		m_pNellBtn->Enable(true);

		m_pLuciaBtn->SetVisible(false);
	}
}

// valentineeventdlg.cpp:756
void FrValEventDlg::OnNellBtnLBDown(void) {
}

// Notice dialog showing one image; declared in this file (its GetRTTI's
// line is valentineeventdlg.cpp:777). Its size (0x118) is the `new` of
// NtNotifyImgDlgMakeInstance.
class NtNotifyImgDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // valentineeventdlg.cpp:777

	NtNotifyImgDlg(void);
	virtual ~NtNotifyImgDlg(void);

	void SetBgImg(const char* name);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnCloseBtnInit(intptr_t param);
	void OnCloseBtnUp(void);
	void OnCaptionInit(intptr_t param);
	void OnImgInit(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

	FrArea* m_pImg;    // 0x110 (OnImgInit)
	UNKNOWN(0x114, 0x118);
};

CHECK_SIZE(NtNotifyImgDlg, 0x118);
BEGIN_LAYOUT_CHECK(NtNotifyImgDlg)
	CHECK_FIELD(NtNotifyImgDlg, m_pImg, 0x110);
END_LAYOUT_CHECK

// valentineeventdlg.cpp:781
NtNotifyImgDlg::NtNotifyImgDlg(void) {
	m_pImg = 0;
}

// valentineeventdlg.cpp:786
NtNotifyImgDlg::~NtNotifyImgDlg(void) {
}

// valentineeventdlg.cpp:789
void NtNotifyImgDlg::SetBgImg(const char* name) {
	if (m_pImg)
		m_pImg->SetBgImg(name);
}

// valentineeventdlg.cpp:810 (run-time type and factory registration of NtNotifyImgDlg)
const WRTTI NtNotifyImgDlg::m_RTTI("NtNotifyImgDlg", &FrForm::m_RTTI);

IObject* NtNotifyImgDlgMakeInstance(void) {
	return new NtNotifyImgDlg;
}

struct __sNtNotifyImgDlg {
	__sNtNotifyImgDlg(void) {
		ObjectFactory().AddObjectFunctor(NtNotifyImgDlgMakeInstance, "NtNotifyImgDlg");
	}
};
static __sNtNotifyImgDlg s_registerNtNotifyImgDlg;

// valentineeventdlg.cpp:812
const sFRESH_MSGMAP* NtNotifyImgDlg::GetMessageMap(void) const {
	return &NtNotifyImgDlg::_MsgMap;
}

sFRESH_MSGMAP NtNotifyImgDlg::_MsgMap = { &FrForm::_MsgMap, &NtNotifyImgDlg::_MsgEntries[0] };

sFRESH_ENTRY NtNotifyImgDlg::_MsgEntries[] = {
	// valentineeventdlg.cpp:814
	{ "close", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtNotifyImgDlg::OnCloseBtnInit },
	// valentineeventdlg.cpp:815
	{ "close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&NtNotifyImgDlg::OnCloseBtnUp },
	// valentineeventdlg.cpp:816
	{ "caption", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtNotifyImgDlg::OnCaptionInit },
	// valentineeventdlg.cpp:817
	{ "img", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtNotifyImgDlg::OnImgInit },

	// valentineeventdlg.cpp:819
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// valentineeventdlg.cpp:823
void NtNotifyImgDlg::OnCloseBtnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// valentineeventdlg.cpp:829
void NtNotifyImgDlg::OnCloseBtnUp(void) {
	OnFreshOkay();
}

// valentineeventdlg.cpp:835
void NtNotifyImgDlg::OnCaptionInit(intptr_t param) {
	DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:844
void NtNotifyImgDlg::OnImgInit(intptr_t param) {
	m_pImg = DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:851 (run-time type and factory registration of NtValentineEventDlg)
const WRTTI NtValentineEventDlg::m_RTTI("NtValentineEventDlg", &FrForm::m_RTTI);

IObject* NtValentineEventDlgMakeInstance(void) {
	return new NtValentineEventDlg;
}

struct __sNtValentineEventDlg {
	__sNtValentineEventDlg(void) {
		ObjectFactory().AddObjectFunctor(NtValentineEventDlgMakeInstance, "NtValentineEventDlg");
	}
};
static __sNtValentineEventDlg s_registerNtValentineEventDlg;

// valentineeventdlg.cpp:853
const sFRESH_MSGMAP* NtValentineEventDlg::GetMessageMap(void) const {
	return &NtValentineEventDlg::_MsgMap;
}

sFRESH_MSGMAP NtValentineEventDlg::_MsgMap = { &FrForm::_MsgMap, &NtValentineEventDlg::_MsgEntries[0] };

sFRESH_ENTRY NtValentineEventDlg::_MsgEntries[] = {
	// valentineeventdlg.cpp:855
	{ "close", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnCloseBtnInit },

	// valentineeventdlg.cpp:856
	{ "close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnCloseBtnUp },

	// valentineeventdlg.cpp:857
	{ "detail", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnDetailBtnInit },

	// valentineeventdlg.cpp:858
	{ "detail", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnDetailBtnUp },

	// valentineeventdlg.cpp:859
	{ "caption", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnCaptionInit },

	// valentineeventdlg.cpp:860
	{ "bg1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnBg1Init },

	// valentineeventdlg.cpp:861
	{ "bg2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnBg2Init },

	// valentineeventdlg.cpp:862
	{ "bg3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnBg3Init },

	// valentineeventdlg.cpp:863
	{ "bg4", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnBg4Init },

	// valentineeventdlg.cpp:864
	{ "valen0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnValen0Init },

	// valentineeventdlg.cpp:865
	{ "valen1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnValen1Init },

	// valentineeventdlg.cpp:866
	{ "valen2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnValen2Init },

	// valentineeventdlg.cpp:867
	{ "valen3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnValen3Init },

	// valentineeventdlg.cpp:868
	{ "valen0", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnValen0OwnerDraw },

	// valentineeventdlg.cpp:869
	{ "valen1", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnValen1OwnerDraw },

	// valentineeventdlg.cpp:870
	{ "valen2", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnValen2OwnerDraw },

	// valentineeventdlg.cpp:871
	{ "valen3", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnValen3OwnerDraw },

	// valentineeventdlg.cpp:873
	{ "white0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnWhite0Init },

	// valentineeventdlg.cpp:874
	{ "white1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnWhite1Init },

	// valentineeventdlg.cpp:875
	{ "white2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnWhite2Init },

	// valentineeventdlg.cpp:876
	{ "white3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnWhite3Init },

	// valentineeventdlg.cpp:877
	{ "white0", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnWhite0OwnerDraw },

	// valentineeventdlg.cpp:878
	{ "white1", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnWhite1OwnerDraw },

	// valentineeventdlg.cpp:879
	{ "white2", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnWhite2OwnerDraw },

	// valentineeventdlg.cpp:880
	{ "white3", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnWhite3OwnerDraw },

	// valentineeventdlg.cpp:882
	{ "combine0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnCombine0BtnInit },

	// valentineeventdlg.cpp:883
	{ "combine0", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnCombine0BtnUp },

	// valentineeventdlg.cpp:884
	{ "combine1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnCombine1BtnInit },

	// valentineeventdlg.cpp:885
	{ "combine1", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnCombine1BtnUp },

	// valentineeventdlg.cpp:886
	{ "combine2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnCombine2BtnInit },

	// valentineeventdlg.cpp:887
	{ "combine2", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnCombine2BtnUp },

	// valentineeventdlg.cpp:888
	{ "combine3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&NtValentineEventDlg::OnCombine3BtnInit },

	// valentineeventdlg.cpp:889
	{ "combine3", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&NtValentineEventDlg::OnCombine3BtnUp },

	// valentineeventdlg.cpp:891
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// valentineeventdlg.cpp:895
NtValentineEventDlg::NtValentineEventDlg(void) {
	char* fontFile[] = { "[font_hole_skins_red.jpg" };
	m_pTitleFont = g_resrcmng->GetTitleFont();

	m_titleFontInfo.filename = fontFile;

	m_titleFontInfo.flag = 0;
	m_titleFontInfo.fonth = 16;
	m_titleFontInfo.fontw = 16;
	m_titleFontInfo.numPages = 1;
	m_titleFontInfo.texw = 128;
	m_titleFontInfo.texh = 32;
	m_titleFontInfo.pCharSet = "1234567890-,";
	m_pTitleFont->Create(&m_titleFontInfo);

	for (int i = 0; i < 4; i++) {
		m_pValen[i] = 0;
		m_valenCount[i] = 0;
	}
	for (int j = 0; j < 4; j++) {
		m_pWhite[j] = 0;
		m_whiteCount[j] = 0;
	}
}

// valentineeventdlg.cpp:920
NtValentineEventDlg::~NtValentineEventDlg(void) {
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// valentineeventdlg.cpp:926
void NtValentineEventDlg::SetUserData(int valen0, int valen1, int valen2, int valen3) {
	m_valenCount[0] = valen0;
	m_valenCount[1] = valen1;
	m_valenCount[2] = valen2;
	m_valenCount[3] = valen3;
}

// valentineeventdlg.cpp:935
void NtValentineEventDlg::SetUserDataWhite(int white0, int white1, int white2, int white3) {
	m_whiteCount[0] = white0;
	m_whiteCount[1] = white1;
	m_whiteCount[2] = white2;
	m_whiteCount[3] = white3;

	UpdateCombinerState();
}

// valentineeventdlg.cpp:946
void NtValentineEventDlg::UpdateCombinerState(void) {
	for (int i = 0; i < 4; i++) {
		if (m_valenCount[i] >= 5 && m_whiteCount[i] >= 5)
			m_pCombineBtn[i]->SetVisible(true);
		else
			m_pCombineBtn[i]->SetVisible(false);
	}
}

// valentineeventdlg.cpp:958
void NtValentineEventDlg::OnCloseBtnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// valentineeventdlg.cpp:964
void NtValentineEventDlg::OnCloseBtnUp(void) {
	OnFreshOkay();
}

// valentineeventdlg.cpp:970
void NtValentineEventDlg::OnDetailBtnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// valentineeventdlg.cpp:976
void NtValentineEventDlg::OnDetailBtnUp(void) {
	NtDetailDlg* dlg = CreateForm<NtDetailDlg>(g_pFresh->GetManager(), this, "detail_popup", NULL);
	dlg->Open((FRESH_RESULT_CALLBACK)&NtValentineEventDlg::OnDetailDlgResult, 1);
}

// valentineeventdlg.cpp:985
void NtValentineEventDlg::OnCaptionInit(intptr_t param) {
	DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:994
void NtValentineEventDlg::OnBg1Init(intptr_t param) {
	DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:1003
void NtValentineEventDlg::OnBg2Init(intptr_t param) {
	DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:1012
void NtValentineEventDlg::OnBg3Init(intptr_t param) {
	DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:1021
void NtValentineEventDlg::OnBg4Init(intptr_t param) {
	DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:1030
void NtValentineEventDlg::OnValen0Init(intptr_t param) {
	m_pValen[0] = DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:1036
void NtValentineEventDlg::OnValen1Init(intptr_t param) {
	m_pValen[1] = DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:1042
void NtValentineEventDlg::OnValen2Init(intptr_t param) {
	m_pValen[2] = DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:1048
void NtValentineEventDlg::OnValen3Init(intptr_t param) {
	m_pValen[3] = DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:1054
void NtValentineEventDlg::OnValen0OwnerDraw(void) {
	FrArea* area = m_pValen[0];

	if (area) {
		const WRect& rect = area->GetRect();

		char* text = MakeStr("%d", m_valenCount[0]);
		float width = m_pTitleFont->GetTextWidth(g_view, text);

		m_pTitleFont->Print(g_view, rect.x + rect.w - width, rect.y, text, 0, 0xFFFFFFFF, NULL);
	}
}

// valentineeventdlg.cpp:1069
void NtValentineEventDlg::OnValen1OwnerDraw(void) {
	FrArea* area = m_pValen[1];

	if (area) {
		const WRect& rect = area->GetRect();

		char* text = MakeStr("%d", m_valenCount[1]);
		float width = m_pTitleFont->GetTextWidth(g_view, text);

		m_pTitleFont->Print(g_view, rect.x + rect.w - width, rect.y, text, 0, 0xFFFFFFFF, NULL);
	}
}

// valentineeventdlg.cpp:1084
void NtValentineEventDlg::OnValen2OwnerDraw(void) {
	FrArea* area = m_pValen[2];

	if (area) {
		const WRect& rect = area->GetRect();

		char* text = MakeStr("%d", m_valenCount[2]);
		float width = m_pTitleFont->GetTextWidth(g_view, text);

		m_pTitleFont->Print(g_view, rect.x + rect.w - width, rect.y, text, 0, 0xFFFFFFFF, NULL);
	}
}

// valentineeventdlg.cpp:1099
void NtValentineEventDlg::OnValen3OwnerDraw(void) {
	FrArea* area = m_pValen[3];

	if (area) {
		const WRect& rect = area->GetRect();

		char* text = MakeStr("%d", m_valenCount[3]);
		float width = m_pTitleFont->GetTextWidth(g_view, text);

		m_pTitleFont->Print(g_view, rect.x + rect.w - width, rect.y, text, 0, 0xFFFFFFFF, NULL);
	}
}

// valentineeventdlg.cpp:1114
void NtValentineEventDlg::OnWhite0Init(intptr_t param) {
	m_pWhite[0] = DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:1120
void NtValentineEventDlg::OnWhite1Init(intptr_t param) {
	m_pWhite[1] = DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:1126
void NtValentineEventDlg::OnWhite2Init(intptr_t param) {
	m_pWhite[2] = DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:1132
void NtValentineEventDlg::OnWhite3Init(intptr_t param) {
	m_pWhite[3] = DYNAMIC_CAST(FrArea, param);
}

// valentineeventdlg.cpp:1138
void NtValentineEventDlg::OnWhite0OwnerDraw(void) {
	FrArea* area = m_pWhite[0];

	if (area) {
		const WRect& rect = area->GetRect();

		char* text = MakeStr("%d", m_whiteCount[0]);
		float width = m_pTitleFont->GetTextWidth(g_view, text);

		m_pTitleFont->Print(g_view, rect.x + rect.w - width, rect.y, text, 0, 0xFFFFFFFF, NULL);
	}
}

// valentineeventdlg.cpp:1153
void NtValentineEventDlg::OnWhite1OwnerDraw(void) {
	FrArea* area = m_pWhite[1];

	if (area) {
		const WRect& rect = area->GetRect();

		char* text = MakeStr("%d", m_whiteCount[1]);
		float width = m_pTitleFont->GetTextWidth(g_view, text);

		m_pTitleFont->Print(g_view, rect.x + rect.w - width, rect.y, text, 0, 0xFFFFFFFF, NULL);
	}
}

// valentineeventdlg.cpp:1168
void NtValentineEventDlg::OnWhite2OwnerDraw(void) {
	FrArea* area = m_pWhite[2];

	if (area) {
		const WRect& rect = area->GetRect();

		char* text = MakeStr("%d", m_whiteCount[2]);
		float width = m_pTitleFont->GetTextWidth(g_view, text);

		m_pTitleFont->Print(g_view, rect.x + rect.w - width, rect.y, text, 0, 0xFFFFFFFF, NULL);
	}
}

// valentineeventdlg.cpp:1183
void NtValentineEventDlg::OnWhite3OwnerDraw(void) {
	FrArea* area = m_pWhite[3];

	if (area) {
		const WRect& rect = area->GetRect();

		char* text = MakeStr("%d", m_whiteCount[3]);
		float width = m_pTitleFont->GetTextWidth(g_view, text);

		m_pTitleFont->Print(g_view, rect.x + rect.w - width, rect.y, text, 0, 0xFFFFFFFF, NULL);
	}
}

// valentineeventdlg.cpp:1198
bool NtValentineEventDlg::OnDetailDlgResult(int result, FrForm* form) {
	return true;
}

// valentineeventdlg.cpp:1204
bool NtValentineEventDlg::OnNotifyDlgResult(int result, FrForm* form) {
	UpdateCombinerState();
	return true;
}

// valentineeventdlg.cpp:1211
void NtValentineEventDlg::OnCombine0BtnInit(intptr_t param) {
	m_pCombineBtn[0] = DYNAMIC_CAST(FrButton, param);
}

// valentineeventdlg.cpp:1217
void NtValentineEventDlg::OnCombine0BtnUp(void) {
	WSendPacket packet(0x6F);
	packet.Encode4(0);
	packet.Send(SEND_TO_DEFAULT);

	NtNotifyImgDlg* dlg = CreateForm<NtNotifyImgDlg>(g_pFresh->GetManager(), this, "notify_img", NULL);

	if (dlg) {

		dlg->SetBgImg("event_result_03");
		dlg->Open((FRESH_RESULT_CALLBACK)&NtValentineEventDlg::OnNotifyDlgResult, 1);
	}

	m_valenCount[0] = m_valenCount[0] - 5 < 0 ? 0 : m_valenCount[0] - 5;
	m_whiteCount[0] = m_whiteCount[0] - 5 < 0 ? 0 : m_whiteCount[0] - 5;
}

// valentineeventdlg.cpp:1236
void NtValentineEventDlg::OnCombine1BtnInit(intptr_t param) {
	m_pCombineBtn[1] = DYNAMIC_CAST(FrButton, param);
}

// valentineeventdlg.cpp:1242
void NtValentineEventDlg::OnCombine1BtnUp(void) {
	WSendPacket packet(0x6F);
	packet.Encode4(1);
	packet.Send(SEND_TO_DEFAULT);

	NtNotifyImgDlg* dlg = CreateForm<NtNotifyImgDlg>(g_pFresh->GetManager(), this, "notify_img", NULL);

	if (dlg) {

		dlg->SetBgImg("event_result_04");
		dlg->Open((FRESH_RESULT_CALLBACK)&NtValentineEventDlg::OnNotifyDlgResult, 1);
	}

	m_valenCount[1] = m_valenCount[1] - 5 < 0 ? 0 : m_valenCount[1] - 5;
	m_whiteCount[1] = m_whiteCount[1] - 5 < 0 ? 0 : m_whiteCount[1] - 5;
}

// valentineeventdlg.cpp:1261
void NtValentineEventDlg::OnCombine2BtnInit(intptr_t param) {
	m_pCombineBtn[2] = DYNAMIC_CAST(FrButton, param);
}

// valentineeventdlg.cpp:1267
void NtValentineEventDlg::OnCombine2BtnUp(void) {
	WSendPacket packet(0x6E);
	packet.Encode4(0x1A000016);
	packet.Send(SEND_TO_DEFAULT);

	NtNotifyImgDlg* dlg = CreateForm<NtNotifyImgDlg>(g_pFresh->GetManager(), this, "notify_img", NULL);

	if (dlg) {

		dlg->SetBgImg("event_result_02");
		dlg->Open((FRESH_RESULT_CALLBACK)&NtValentineEventDlg::OnNotifyDlgResult, 1);
	}

	m_valenCount[2] = m_valenCount[2] - 5 < 0 ? 0 : m_valenCount[2] - 5;
	m_whiteCount[2] = m_whiteCount[2] - 5 < 0 ? 0 : m_whiteCount[2] - 5;
}

// valentineeventdlg.cpp:1286
void NtValentineEventDlg::OnCombine3BtnInit(intptr_t param) {
	m_pCombineBtn[3] = DYNAMIC_CAST(FrButton, param);
}

// valentineeventdlg.cpp:1292
void NtValentineEventDlg::OnCombine3BtnUp(void) {
}

// valentineeventdlg.cpp:1311 (run-time type and factory registration of ntHolesEventDlg)
const WRTTI ntHolesEventDlg::m_RTTI("ntHolesEventDlg", &FrForm::m_RTTI);

IObject* ntHolesEventDlgMakeInstance(void) {
	return new ntHolesEventDlg;
}

struct __sntHolesEventDlg {
	__sntHolesEventDlg(void) {
		ObjectFactory().AddObjectFunctor(ntHolesEventDlgMakeInstance, "ntHolesEventDlg");
	}
};
static __sntHolesEventDlg s_registerntHolesEventDlg;

// valentineeventdlg.cpp:1313
const sFRESH_MSGMAP* ntHolesEventDlg::GetMessageMap(void) const {
	return &ntHolesEventDlg::_MsgMap;
}

sFRESH_MSGMAP ntHolesEventDlg::_MsgMap = { &FrForm::_MsgMap, &ntHolesEventDlg::_MsgEntries[0] };

sFRESH_ENTRY ntHolesEventDlg::_MsgEntries[] = {
	// valentineeventdlg.cpp:1315
	{ "prize0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntHolesEventDlg::OnPrize0Init },
	// valentineeventdlg.cpp:1316
	{ "prize1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntHolesEventDlg::OnPrize1Init },
	// valentineeventdlg.cpp:1317
	{ "prize2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntHolesEventDlg::OnPrize2Init },
	// valentineeventdlg.cpp:1318
	{ "prize3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntHolesEventDlg::OnPrize3Init },
	// valentineeventdlg.cpp:1319
	{ "prize4", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntHolesEventDlg::OnPrize4Init },
	// valentineeventdlg.cpp:1320
	{ "gauge", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&ntHolesEventDlg::OnGaugeOwnerDraw },

	// valentineeventdlg.cpp:1322
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// The title font's texture list (data at 0xabafa8).
static char* s_holeFontFiles[] = { "[font_bong.jpg" };

// valentineeventdlg.cpp:1331
ntHolesEventDlg::ntHolesEventDlg(void) {
	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = s_holeFontFiles;
	info.fontw = 9;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 32;
	info.pCharSet = "1234567890,";

	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);
}

// valentineeventdlg.cpp:1349
ntHolesEventDlg::~ntHolesEventDlg(void) {
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// The local player's holes played (CSharedDoc 0x5C6, statistics +0xD7),
// read by the prize images and the gauge. Accessed by offset until
// SharedDoc.h declares the member.
#define HOLES_PLAYED(doc) (*(unsigned long*)((char*)(doc) + 0x5C6))

// valentineeventdlg.cpp:1354
void ntHolesEventDlg::OnPrize0Init(intptr_t param) {
	FrArea* area = DYNAMIC_CAST(FrArea, param);
	if (area) {
		if (HOLES_PLAYED(WSingleton<CSharedDoc>::Instance()) >= 100) {
			area->SetBgImg("holes_prize_00");
		}
	}
}

// valentineeventdlg.cpp:1364
void ntHolesEventDlg::OnPrize1Init(intptr_t param) {
	FrArea* area = DYNAMIC_CAST(FrArea, param);
	if (area) {
		if (HOLES_PLAYED(WSingleton<CSharedDoc>::Instance()) >= 300) {
			area->SetBgImg("holes_prize_01");
		}
	}
}

// valentineeventdlg.cpp:1374
void ntHolesEventDlg::OnPrize2Init(intptr_t param) {
	FrArea* area = DYNAMIC_CAST(FrArea, param);
	if (area) {
		if (HOLES_PLAYED(WSingleton<CSharedDoc>::Instance()) >= 500) {
			area->SetBgImg("holes_prize_02");
		}
	}
}

// valentineeventdlg.cpp:1384
void ntHolesEventDlg::OnPrize3Init(intptr_t param) {
	FrArea* area = DYNAMIC_CAST(FrArea, param);
	if (area) {
		if (HOLES_PLAYED(WSingleton<CSharedDoc>::Instance()) >= 700) {
			area->SetBgImg("holes_prize_03");
		}
	}
}

// valentineeventdlg.cpp:1394
void ntHolesEventDlg::OnPrize4Init(intptr_t param) {
	FrArea* area = DYNAMIC_CAST(FrArea, param);
	if (area) {
		if (HOLES_PLAYED(WSingleton<CSharedDoc>::Instance()) >= 1000) {
			area->SetBgImg("holes_prize_04");
		}
	}
}

// valentineeventdlg.cpp:1428
ntMightyMACEventDlg::ntMightyMACEventDlg(void) {
}

// valentineeventdlg.cpp:1432
ntMightyMACEventDlg::~ntMightyMACEventDlg(void) {
}
