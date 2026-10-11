// Original: d:\build\custom\temp\code\client\projectg\s5\skinnedtheme.h
#ifndef PG_CLIENT_PROJECTG_S5_SKINNEDTHEME_H
#define PG_CLIENT_PROJECTG_S5_SKINNEDTHEME_H

#include "..\..\defines.h"
#include <string>

class FrGroupBox;

// Image names of the three over-bar pieces (strncpy'd with size 0x3f).
struct THEME_OVERBAR {
	char szBar1[64];   // 0x00
	char szBar2[64];   // 0x40
	char szBar3[64];   // 0x80
};
CHECK_SIZE(THEME_OVERBAR, 0xc0);
CHECK_FIELD(THEME_OVERBAR, szBar1, 0x00);
CHECK_FIELD(THEME_OVERBAR, szBar2, 0x40);
CHECK_FIELD(THEME_OVERBAR, szBar3, 0x80);

// Image names of the three over-bar frame pieces.
struct THEME_OVERBAR_FRAME {
	char szFrame1[64];   // 0x00
	char szFrame2[64];   // 0x40
	char szFrame3[64];   // 0x80
};
CHECK_SIZE(THEME_OVERBAR_FRAME, 0xc0);
CHECK_FIELD(THEME_OVERBAR_FRAME, szFrame1, 0x00);
CHECK_FIELD(THEME_OVERBAR_FRAME, szFrame2, 0x40);
CHECK_FIELD(THEME_OVERBAR_FRAME, szFrame3, 0x80);

// Image names of a button's states ("_n", "_p", "_d", "_o" suffixes).
struct THEME_BUTTONS {
	char szNormal[64];    // 0x00
	char szPush[64];      // 0x40
	char szDisable[64];   // 0x80
	char szOver[64];      // 0xc0
};
CHECK_SIZE(THEME_BUTTONS, 0x100);
CHECK_FIELD(THEME_BUTTONS, szNormal, 0x00);
CHECK_FIELD(THEME_BUTTONS, szPush, 0x40);
CHECK_FIELD(THEME_BUTTONS, szDisable, 0x80);
CHECK_FIELD(THEME_BUTTONS, szOver, 0xc0);

// Skinned theme interface (vtable of 8 pure virtuals, no destructor).
struct _ISkinnedTheme {
	virtual const char* GetSkinedThemeClassName(void) = 0;                     // slot 0x00
	virtual const char* GetWindOutterCircleImgName(void) = 0;                  // slot 0x04
	virtual const char* GetWindInnerCircleImgName(void) = 0;                   // slot 0x08
	virtual const char* GetRoomListBackGroundImgName(void) = 0;                // slot 0x0c
	virtual void GetOverBarImgName(THEME_OVERBAR& overBar) = 0;                // slot 0x10
	virtual void GetOverBarFrameImgName(THEME_OVERBAR_FRAME& overBarFrame) = 0; // slot 0x14
	virtual void GetMakeRoomButtonImgName(THEME_BUTTONS& buttons) = 0;         // slot 0x18
	virtual void GetQuickStartButtonImgName(THEME_BUTTONS& buttons) = 0;       // slot 0x1c
};
CHECK_SIZE(_ISkinnedTheme, 0x4);

// The default theme.
class CSkinnedThemeBase : public _ISkinnedTheme {
public:
	virtual const char* GetSkinedThemeClassName(void);
	virtual const char* GetWindOutterCircleImgName(void);
	virtual const char* GetWindInnerCircleImgName(void);
	virtual const char* GetRoomListBackGroundImgName(void);
	virtual void GetOverBarImgName(THEME_OVERBAR& overBar);
	virtual void GetOverBarFrameImgName(THEME_OVERBAR_FRAME& overBarFrame);
	virtual void GetMakeRoomButtonImgName(THEME_BUTTONS& buttons);
	virtual void GetQuickStartButtonImgName(THEME_BUTTONS& buttons);
};
CHECK_SIZE(CSkinnedThemeBase, 0x4);

// The "classic" theme.
class CClassicTheme : public _ISkinnedTheme {
public:
	virtual const char* GetSkinedThemeClassName(void);
	virtual const char* GetWindOutterCircleImgName(void);
	virtual const char* GetWindInnerCircleImgName(void);
	virtual const char* GetRoomListBackGroundImgName(void);
	virtual void GetOverBarImgName(THEME_OVERBAR& overBar);
	virtual void GetOverBarFrameImgName(THEME_OVERBAR_FRAME& overBarFrame);
	virtual void GetMakeRoomButtonImgName(THEME_BUTTONS& buttons);
	virtual void GetQuickStartButtonImgName(THEME_BUTTONS& buttons);
};
CHECK_SIZE(CClassicTheme, 0x4);

// Singleton holding the class name of the current theme.
class CSkinnedThemeName {
	LAYOUT_CHECK_FRIEND;
public:
	static CSkinnedThemeName* Instance(void);
	void SetSkinedThemeName(const char* name);
	const std::string& GetSkinnedThemeName(void) const;

private:
	CSkinnedThemeName(void);

	std::string m_name;   // 0x00
};
CHECK_SIZE(CSkinnedThemeName, 0x1c);
BEGIN_LAYOUT_CHECK(CSkinnedThemeName)
	CHECK_FIELD(CSkinnedThemeName, m_name, 0x00);
END_LAYOUT_CHECK

namespace S5 {
namespace THEME {

void GetServerSelectButton(unsigned long flags, THEME_BUTTONS& buttons);
void GetChannelSelectButton(unsigned long flags, THEME_BUTTONS& buttons);
void ChangeGroupBoxChildAreaBgImg(FrGroupBox* groupBox, const char* childName, const char* imgName);
void ChangeOverbarTheme(_ISkinnedTheme* theme, FrGroupBox* groupBox);
void SetSkinnedTheme(const char* className);
void SetSkinnedTheme(unsigned long flags);
_ISkinnedTheme* GetCurrentSkinnedTheme(void);

}  // namespace THEME
}  // namespace S5

#endif  // PG_CLIENT_PROJECTG_S5_SKINNEDTHEME_H
