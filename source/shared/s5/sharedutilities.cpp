// Original: d:\build\custom\temp\code\shared\s5\sharedutilities.cpp
#include "../shared/s5/SharedUtilities.h"

#include <ctype.h>
#include <mbstring.h>
#include <string.h>

#include "../client/ProjectG/SharedDoc.h"

// The clear bonus of each course (20) and difficulty (3); data with no symbol.
// Provide default definitions to satisfy link when original data is absent.
const unsigned long data_00a2b138[20][3] = { {0} };
// The hole-in-one bonus of each course (20), and the one of an unknown
// course; provide zero-initialized defaults (can be replaced with real
// table values later).
const sHoleInBonus data_00a2b228[20] = { {0} };
const sHoleInBonus data_00a2b5e8 = { 0 };

namespace S5 {

namespace CLASSICSRV {

// sharedutilities.cpp:403
unsigned long GetClearBonus(unsigned char level, unsigned char course) {
	int levelIndex = level - 2 < 0 ? 0 : level - 2;
	int courseIndex = (course > 0x7F && course != 0xFD) ? course - 0x80 : course;

	if (levelIndex < 0 || levelIndex > 2 || courseIndex < 0 || courseIndex >= 20) {
		return 0;
	}
	return data_00a2b138[courseIndex][levelIndex];
}

// sharedutilities.cpp:416
const sHoleInBonus *GetCourseHoleInBonus(unsigned char course) {
	int courseIndex = (course > 0x7F && course != 0xFD) ? course - 0x80 : course;
	if (courseIndex < 0 || courseIndex >= 20) {
		return &data_00a2b5e8;
	}
	return &data_00a2b228[courseIndex];
}

} // namespace CLASSICSRV

namespace RENTALPARTS {

// sharedutilities.cpp:438
int IsRentableParts(unsigned long typeId) {
	IFF_STRUCT::sPart *part = ItemManager()->FindPart(typeId);
	if (part == 0) {
		return 0;
	}
	return part->m_rentalPrice > 0;
}

// sharedutilities.cpp:444
int GetRentalPrice(unsigned long typeId) {
	if (IsRentableParts(typeId)) {
		IFF_STRUCT::sPart *part = ItemManager()->FindPart(typeId);
		return part->m_rentalPrice;
	}
	return 0x7FFFFFFF;
}

// sharedutilities.cpp:454
void CollectRentalItem(std::list<sItemInfo>& rentalList, const std::list<sItemInfo>& itemList) {
	for (std::list<sItemInfo>::const_iterator it = itemList.begin(); it != itemList.end(); ++it) {
		if (it->m_stateBits.kind == 6) {
			rentalList.push_back(*it);
		}
	}
}

// sharedutilities.cpp:464
int IsExistExpiredRentalItem(const std::list<sItemInfo>& itemList) {
	for (std::list<sItemInfo>::const_iterator it = itemList.begin(); it != itemList.end(); ++it) {
		if (it->m_stateBits.kind == 6 && it->m_stateBits.expired) {
			return 1;
		}
	}
	return 0;
}

} // namespace RENTALPARTS

// sharedutilities.cpp:544
__int64 ExchangeOwnCashToCookie(__int64 cash, __int64 cookie, int rate) {
	return cash / rate + cookie;
}

// sharedutilities.cpp:557
__int64 ExchangeOwnCookieToCash(__int64 cookie, int rate) {
	return cookie * rate;
}

} // namespace S5

namespace _util {

// sharedutilities.cpp:569
ePwdErrorCode VerifyInputPassword(const std::string &password) {
	ePwdErrorCode result = VerifyLength(password, 5, 8);
	if (result != PWD_OK) {
		return result;
	}
	return VerifyStringContents_IsNumber(password);
}

// sharedutilities.cpp:588
ePwdErrorCode VerifyLength(const std::string &text, int minLength, int maxLength) {
	int length = text.length();
	if (length == 0) {
		return PWD_EMPTY;
	}
	if (minLength > length) {
		return PWD_TOO_SHORT;
	}
	if (maxLength < length) {
		return PWD_TOO_LONG;
	}
	return PWD_OK;
}

// sharedutilities.cpp:609
ePwdErrorCode VerifyStringContents_IsNumber(const std::string &text) {
	int length = text.length();
	for (int i = 0; i < length; ++i) {
		if (!isdigit(text.at(i))) {
			return PWD_NOT_NUMBER;
		}
	}
	return PWD_OK;
}

// sharedutilities.cpp:623
ePwdErrorCode VerifyStringContents_Limit(const std::string &text) {
	int length = text.length();

	char input[9] = { 0, };

	_mbsnbcpy((unsigned char *)input, (const unsigned char *)text.c_str(), 8);

	char digits[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	int start;
	for (start = 0; start < 10; ++start) {
		if (input[0] == digits[start]) {
			break;
		}
	}

	char pattern[9];
	int k;
	memset(pattern, 0, sizeof(pattern)); for (k = 0; k < 8; ++k) pattern[k] = digits[start];

	bool same = true;
	int i;
	for (i = 0; i < length; ++i) {
		if (input[i] != pattern[i]) {
			same = false;
			break;
		}
	}
	if (same) {
		return PWD_SAME_DIGITS;
	}

	if (start < 6) {
		memset(pattern, 0, sizeof(pattern));
		int j;
		for (k = 0, j = start; k < 8 && j < 10; ++k, ++j) {
			pattern[k] = digits[j];
		}

		bool ascending = true;
		for (i = 0; i < length; ++i) {
			if (input[i] != pattern[i]) {
				ascending = false;
				break;
			}
		}
		if (ascending) {
			return PWD_ASCENDING;
		}
	}

	if (start > 3) {
		memset(pattern, 0, sizeof(pattern));
		int j;
		for (k = 0, j = start; k < 8 && j >= 0; ++k, --j) {
			pattern[k] = digits[j];
		}

		bool descending = true;
		for (i = 0; i < length; ++i) {
			if (input[i] != pattern[i]) {
				descending = false;
				break;
			}
		}
		if (descending) {
			return PWD_DESCENDING;
		}
	}

	return PWD_OK;
}

} // namespace _util

namespace S5 {

// sharedutilities.cpp:744
bool IsSpecialGameStyle(eGameStyle style) {
	if (style > 0 && style <= 2) {
		return true;
	}
	return false;
}

// sharedutilities.cpp:755
bool IsSpecialGame(unsigned char gameType, eGameStyle style) {
	if (gameType == 14) {
		return true;
	}
	if (style > 0 && style <= 2) {
		return true;
	}
	return false;
}

// sharedutilities.cpp:780
_SYSTEMTIME MakeSysTime(unsigned short year, unsigned short month, unsigned short day, unsigned short hour) {
	SYSTEMTIME time;
	memset(&time, 0, sizeof(time)); time.wYear = year;
	time.wMonth = month; time.wDay = day; time.wHour = hour;
	return time;
}

} // namespace S5
