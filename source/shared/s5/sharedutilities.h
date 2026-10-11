// Original: d:\build\custom\temp\code\shared\s5\sharedutilities.h
#ifndef PG_SHARED_S5_SHAREDUTILITIES_H
#define PG_SHARED_S5_SHAREDUTILITIES_H

#include <windows.h>

#include <string>
#include <list>

#include "..\..\padding.h"
#include "..\..\defines.h"

// The game style of a room (stGameClass, globalgamedefine.h:663); its
// enumerators are not transcribed yet.
enum eGameStyle;

struct sItemInfo;

// A course's hole-in-one bonus (0x30 bytes; a table of 20, one per course,
// in the original's data). Its members are not transcribed yet.
struct sHoleInBonus {
	UNKNOWN(0x00, 0x30);
};
CHECK_SIZE(sHoleInBonus, 0x30);

namespace S5 {

namespace CLASSICSRV {

unsigned long GetClearBonus(unsigned char level, unsigned char course);
const sHoleInBonus *GetCourseHoleInBonus(unsigned char course);

} // namespace CLASSICSRV

namespace RENTALPARTS {

int IsRentableParts(unsigned long typeId);
int GetRentalPrice(unsigned long typeId);
int IsExistExpiredRentalItem(const std::list<sItemInfo>& itemList);

} // namespace RENTALPARTS

__int64 ExchangeOwnCashToCookie(__int64 cash, __int64 cookie, int rate);
__int64 ExchangeOwnCookieToCash(__int64 cookie, int rate);

bool IsSpecialGameStyle(eGameStyle style);
bool IsSpecialGame(unsigned char gameType, eGameStyle style);

_SYSTEMTIME MakeSysTime(unsigned short year, unsigned short month, unsigned short day, unsigned short hour);

} // namespace S5

namespace _util {

// The result of a password check (SecondaryPasswordDlg).
enum ePwdErrorCode {
	PWD_OK = 0,
	PWD_NOT_NUMBER = 1,
	PWD_EMPTY = 2,
	PWD_TOO_SHORT = 3,
	PWD_TOO_LONG = 4,
	PWD_SAME_DIGITS = 5,
	PWD_ASCENDING = 6,
	PWD_DESCENDING = 7
};

ePwdErrorCode VerifyInputPassword(const std::string &password);
ePwdErrorCode VerifyLength(const std::string &text, int minLength, int maxLength);
ePwdErrorCode VerifyStringContents_IsNumber(const std::string &text);
ePwdErrorCode VerifyStringContents_Limit(const std::string &text);

} // namespace _util

#endif // PG_SHARED_S5_SHAREDUTILITIES_H
