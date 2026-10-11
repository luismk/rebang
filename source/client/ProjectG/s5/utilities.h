// Original: d:\build\custom\temp\code\client\projectg\s5\utilities.h
#ifndef PG_CLIENT_PROJECTG_S5_UTILITIES_H
#define PG_CLIENT_PROJECTG_S5_UTILITIES_H

#include <windows.h>
#include <list>
#include <vector>
#include "frwnd.h"
#include "../shared/s5/SharedUtilities.h"

// Server property bits (CSharedDoc::m_serverProperty); values not recovered.
enum eGameServerProperty {
};

// The equip skin slot of a skin part (typeId bits 22-25 mapped to 0-5 by
// S5::UTIL::GetEquipSkinIndex); enumerator names not recovered.
enum eEquipSkin {
};

class WRect;

namespace IFF_STRUCT {
struct sPart;
}

namespace S5 {

namespace UTIL {

void SendCutInPacket(unsigned long cutInId, short type, unsigned long param1, unsigned long param2);
void SetServerProperty(unsigned long property);
unsigned long GetServerProperty(void);
int EncodingUrl(const char* src, char* dst);
int DecodingUrl(const char* src, char* dst);
int GetEquipSkinIndex(eEquipSkin& skin, unsigned long typeId);
int ErasePartsCardAll(unsigned long partsId, std::list<unsigned long>& typeIds);
int Base64Encode(char* src, int len, char* dst);
int Base64Decode(char* src, unsigned char* dst, int dstSize);
int GetHoleinCoinCount(int coinCount, int holeinCount);
bool IsEventServerControl(eGameServerProperty property);
bool FindVisibleWindowFromExStyle(unsigned long exStyle, std::vector<HWND>& windows);
int GetWindowBorderSize(HWND hWnd);
int GetWindowTitleBarSize(HWND hWnd);
bool IsInRect(const RECT& rc, int x, int y);
bool IsOverlappedRect(const RECT& rc1, const RECT& rc2);
bool CalculateUVPos(WRect& rc, int textureWidth, int textureHeight, float cellWidth, float cellHeight, int index);
int IsLunarYear(unsigned short year);
unsigned short GetMonthlyDayCount(unsigned short year, unsigned short month);
unsigned long GetNickColor(unsigned long identity, int isNewbie, unsigned short flags);

} // namespace UTIL

bool UnderCategory(unsigned long charTypeId, unsigned long partTypeId, unsigned long slotMask);
bool UnderCatogory(unsigned long charTypeId, unsigned long slotMask, IFF_STRUCT::sPart* pPart);
void GetLine(char* src, char* dst);
bool IsUsableCalipers(void);

// utilities.h:123
inline void EnableFreshWindow(FrWnd* pWnd, bool bEnable) {
	pWnd->SetVisible(bEnable);
	pWnd->Enable(bEnable);
}

// utilities.h:129
inline void EnableFreshWindow(FrWnd* pWnd) {
	pWnd->SetVisible(true);
	pWnd->Enable(true);
}

// utilities.h:135
inline void DisableFreshWindow(FrWnd* pWnd) {
	pWnd->SetVisible(false);
	pWnd->Enable(false);
}

} // namespace S5

namespace _util {

const char* GetAvatarServerServiceMessage(int property);
const char* GetNormalServerServiceMessage(int property);
bool IsAvatarChatServer(int flag);
void GenerateRandomTidParts(unsigned long tidChar, unsigned long* const partTids, unsigned long* const partIds);

} // namespace _util

#endif // PG_CLIENT_PROJECTG_S5_UTILITIES_H
