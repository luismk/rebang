// Original: d:\build\custom\temp\code\client\projectg\s5\skinnedtheme.cpp
#include "SkinnedTheme.h"
#include "frarea.h"
#include "frgroupbox.h"
#include <string.h>

// skinnedtheme.cpp:32
const char* CSkinnedThemeBase::GetSkinedThemeClassName(void) { return "CSkinnedThemeBase"; }

// skinnedtheme.cpp:34
const char* CSkinnedThemeBase::GetWindOutterCircleImgName(void) { return "[right_wind_b.jpg"; }
// skinnedtheme.cpp:35
const char* CSkinnedThemeBase::GetWindInnerCircleImgName(void) { return "[right_wind_s.jpg"; }

// skinnedtheme.cpp:37
const char* CSkinnedThemeBase::GetRoomListBackGroundImgName(void)
{
	return "roomlist_background.jpg";
}

// skinnedtheme.cpp:51
void CSkinnedThemeBase::GetOverBarImgName(THEME_OVERBAR& overBar)
{
	strncpy(overBar.szBar1, "over_bar_01", sizeof(overBar.szBar1) - 1);
	strncpy(overBar.szBar2, "over_bar_02", sizeof(overBar.szBar2) - 1);
	strncpy(overBar.szBar3, "over_bar_03", sizeof(overBar.szBar3) - 1);
}

// skinnedtheme.cpp:64
void CSkinnedThemeBase::GetOverBarFrameImgName(THEME_OVERBAR_FRAME& overBarFrame)
{
	strncpy(overBarFrame.szFrame1, "over_frame_1", sizeof(overBarFrame.szFrame1) - 1);
	strncpy(overBarFrame.szFrame2, "over_frame_2", sizeof(overBarFrame.szFrame2) - 1);
	strncpy(overBarFrame.szFrame3, "over_frame_3", sizeof(overBarFrame.szFrame3) - 1);
}

// skinnedtheme.cpp:77
void CSkinnedThemeBase::GetMakeRoomButtonImgName(THEME_BUTTONS& buttons)
{
	strncpy(buttons.szDisable, "btn_makeroom_d", sizeof(buttons.szDisable) - 1);
	strncpy(buttons.szNormal, "btn_makeroom_n", sizeof(buttons.szNormal) - 1);
	strncpy(buttons.szOver, "btn_makeroom_o", sizeof(buttons.szOver) - 1);
	strncpy(buttons.szPush, "btn_makeroom_d", sizeof(buttons.szPush) - 1);
}

// skinnedtheme.cpp:92
void CSkinnedThemeBase::GetQuickStartButtonImgName(THEME_BUTTONS& buttons)
{
	strncpy(buttons.szDisable, "btn_ingameguild_d", sizeof(buttons.szDisable) - 1);
	strncpy(buttons.szNormal, "btn_ingameguild_n", sizeof(buttons.szNormal) - 1);
	strncpy(buttons.szOver, "btn_ingameguild_o", sizeof(buttons.szOver) - 1);
	strncpy(buttons.szPush, "btn_ingameguild_d", sizeof(buttons.szPush) - 1);
}

// skinnedtheme.cpp:135
const char* CClassicTheme::GetSkinedThemeClassName(void) { return "CClassicTheme"; }

// skinnedtheme.cpp:137
const char* CClassicTheme::GetWindOutterCircleImgName(void) { return "[classic_right_wind_b.jpg"; }
// skinnedtheme.cpp:138
const char* CClassicTheme::GetWindInnerCircleImgName(void) { return "[classic_right_wind_s.jpg"; }

// skinnedtheme.cpp:140
const char* CClassicTheme::GetRoomListBackGroundImgName(void)
{
	return "classic_roomlist_background.jpg";
}

// skinnedtheme.cpp:149
void CClassicTheme::GetOverBarImgName(THEME_OVERBAR& overBar)
{
	strncpy(overBar.szBar1, "classic_over_bar_01", sizeof(overBar.szBar1) - 1);
	strncpy(overBar.szBar2, "classic_over_bar_02", sizeof(overBar.szBar2) - 1);
	strncpy(overBar.szBar3, "classic_over_bar_03", sizeof(overBar.szBar3) - 1);
}

// skinnedtheme.cpp:156
void CClassicTheme::GetOverBarFrameImgName(THEME_OVERBAR_FRAME& overBarFrame)
{
	strncpy(overBarFrame.szFrame1, "classic_over_frame_1", sizeof(overBarFrame.szFrame1) - 1);
	strncpy(overBarFrame.szFrame2, "over_frame_2", sizeof(overBarFrame.szFrame2) - 1);
	strncpy(overBarFrame.szFrame3, "classic_over_frame_3", sizeof(overBarFrame.szFrame3) - 1);
}

// skinnedtheme.cpp:163
void CClassicTheme::GetMakeRoomButtonImgName(THEME_BUTTONS& buttons)
{
	strncpy(buttons.szDisable, "classic_btn_makeroom_d", sizeof(buttons.szDisable) - 1);
	strncpy(buttons.szNormal, "classic_btn_makeroom_n", sizeof(buttons.szNormal) - 1);
	strncpy(buttons.szOver, "classic_btn_makeroom_o", sizeof(buttons.szOver) - 1);
	strncpy(buttons.szPush, "classic_btn_makeroom_d", sizeof(buttons.szPush) - 1);
}

// skinnedtheme.cpp:170
void CClassicTheme::GetQuickStartButtonImgName(THEME_BUTTONS& buttons)
{
	strncpy(buttons.szDisable, "btn_ingameguild_ntr_d", sizeof(buttons.szDisable) - 1);
	strncpy(buttons.szNormal, "btn_ingameguild_ntr_n", sizeof(buttons.szNormal) - 1);
	strncpy(buttons.szOver, "btn_ingameguild_ntr_o", sizeof(buttons.szOver) - 1);
	strncpy(buttons.szPush, "btn_ingameguild_ntr_d", sizeof(buttons.szPush) - 1);
}

// skinnedtheme.cpp:200
inline CSkinnedThemeName* CSkinnedThemeName::Instance(void)
{
	static CSkinnedThemeName _inst;

	return &_inst;
}

// skinnedtheme.cpp:205
void CSkinnedThemeName::SetSkinedThemeName(const char* name) { m_name = name; }
// skinnedtheme.cpp:206
const std::string& CSkinnedThemeName::GetSkinnedThemeName(void) const { return m_name; }

// skinnedtheme.cpp:209
CSkinnedThemeName::CSkinnedThemeName(void) : m_name("CSkinnedThemeBase")
{
}

namespace S5 {
namespace THEME {

// skinnedtheme.cpp:248
void GetServerSelectButton(unsigned long flags, THEME_BUTTONS& buttons)
{
	if (flags & 0x80) {
		strncpy(buttons.szDisable, "classic_server_select_btn_d", sizeof(buttons.szDisable) - 1);
		strncpy(buttons.szNormal, "classic_server_select_btn_n", sizeof(buttons.szNormal) - 1);
		strncpy(buttons.szOver, "classic_server_select_btn_o", sizeof(buttons.szOver) - 1);
		strncpy(buttons.szPush, "classic_server_select_btn_d", sizeof(buttons.szPush) - 1);
	} else {
		strncpy(buttons.szDisable, "server_select_btn_d", sizeof(buttons.szDisable) - 1);
		strncpy(buttons.szNormal, "server_select_btn_n", sizeof(buttons.szNormal) - 1);
		strncpy(buttons.szOver, "server_select_btn_o", sizeof(buttons.szOver) - 1);
		strncpy(buttons.szPush, "server_select_btn_d", sizeof(buttons.szPush) - 1);
	}
}

// skinnedtheme.cpp:279
void GetChannelSelectButton(unsigned long flags, THEME_BUTTONS& buttons)
{
	if (flags & 0x80) {
		strncpy(buttons.szDisable, "classic_channel_select_btn_d", sizeof(buttons.szDisable) - 1);
		strncpy(buttons.szNormal, "classic_channel_select_btn_n", sizeof(buttons.szNormal) - 1);
		strncpy(buttons.szOver, "classic_channel_select_btn_o", sizeof(buttons.szOver) - 1);
		strncpy(buttons.szPush, "classic_channel_select_btn_p", sizeof(buttons.szPush) - 1);
	} else {
		strncpy(buttons.szDisable, "channel_select_btn_d", sizeof(buttons.szDisable) - 1);
		strncpy(buttons.szNormal, "channel_select_btn_n", sizeof(buttons.szNormal) - 1);
		strncpy(buttons.szOver, "channel_select_btn_o", sizeof(buttons.szOver) - 1);
		strncpy(buttons.szPush, "channel_select_btn_p", sizeof(buttons.szPush) - 1);
	}
}

// skinnedtheme.cpp:301
void ChangeGroupBoxChildAreaBgImg(FrGroupBox* groupBox, const char* childName, const char* imgName)
{
	if (groupBox && childName && imgName) {
		FrWnd* child = groupBox->FindChildByName(childName);
		FrArea* area = DYNAMIC_CAST(FrArea, child);
		if (area) {
			area->SetBgImg(imgName);
		}
	}
}

// The two theme instances GetCurrentSkinnedTheme hands out.
static CSkinnedThemeBase s_skinnedThemeBase;
static CClassicTheme s_classicTheme;

// skinnedtheme.cpp:316
void SetSkinnedTheme(const char* className)
{
	CSkinnedThemeName::Instance()->SetSkinedThemeName(className);
}

// skinnedtheme.cpp:321
void SetSkinnedTheme(unsigned long flags)
{
	if (flags & 0x80) {
		CSkinnedThemeName::Instance()->SetSkinedThemeName("CClassicTheme");
	} else {
		CSkinnedThemeName::Instance()->SetSkinedThemeName("CSkinnedThemeBase");
	}
}

// skinnedtheme.cpp:335
_ISkinnedTheme* GetCurrentSkinnedTheme(void)
{
	const std::string& name = CSkinnedThemeName::Instance()->GetSkinnedThemeName();
	if (name == "CSkinnedThemeBase") {
		return &s_skinnedThemeBase;
	} else if (name == "CClassicTheme") {
		return &s_classicTheme;
	}

	return &s_skinnedThemeBase;
}

// skinnedtheme.cpp:348
void ChangeOverbarTheme(_ISkinnedTheme* theme, FrGroupBox* groupBox)
{
	if (groupBox) {
		std::string name;
		groupBox->GetWindowName(name);
		if (name != "obar") {
			return;
		}

		THEME_OVERBAR overBar;
		theme->GetOverBarImgName(overBar);
		THEME_OVERBAR_FRAME overBarFrame;
		theme->GetOverBarFrameImgName(overBarFrame);

		ChangeGroupBoxChildAreaBgImg(groupBox, "bar1", overBar.szBar1);
		ChangeGroupBoxChildAreaBgImg(groupBox, "bar2", overBar.szBar2);
		ChangeGroupBoxChildAreaBgImg(groupBox, "bar3", overBar.szBar3);

		ChangeGroupBoxChildAreaBgImg(groupBox, "frame_1", overBarFrame.szFrame1);
		ChangeGroupBoxChildAreaBgImg(groupBox, "frame_2", overBarFrame.szFrame2);
		ChangeGroupBoxChildAreaBgImg(groupBox, "frame_3", overBarFrame.szFrame3);
	}
}

}  // namespace THEME
}  // namespace S5
