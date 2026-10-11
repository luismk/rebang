// Original: d:\build\custom\temp\code\client\projectg\s5\utilities.cpp
#include "Utilities.h"
#include "SharedDoc.h"
#include "Packet.h"
#include "../shared/Localize.h"
#include "TaskManager.h"
#include "actor.h"
#include "../client/Wangreal/include/wtypes.h"
#include "parttidlist.h"
#include "../shared/ItemManager.h"
#include "../shared/globalgamedefine.h"
#include <string.h>
#include <stdlib.h>

namespace {

unsigned long l_dwSRVPROP;

} // namespace

// The name of the gateway actor (IsUsableCalipers asks it).
static const char* s_gatewayActorName = "Gateway";

namespace S5 {

namespace UTIL {

// utilities.cpp:32
void SendCutInPacket(unsigned long cutInId, short type, unsigned long param1, unsigned long param2) {
	WSendPacket packet(0xE7);
	packet.Encode4(cutInId);
	packet.Encode2(type);
	packet.Encode4(param1);
	packet.Encode4(param2);
	packet.Send(SEND_TO_DEFAULT);
}

// utilities.cpp:41
void SetServerProperty(unsigned long property) { l_dwSRVPROP = property; }
// utilities.cpp:42
unsigned long GetServerProperty(void) { return l_dwSRVPROP; }

// utilities.cpp:45
int EncodingUrl(const char* src, char* dst) {
	int len = lstrlenA(src);
	if (len == 0) {
		return 0;
	}

	char safeChars[] = "/?:=,<>\"-_.~'(){}\\[]";

	const char* end = src + len;
	const char* p = src;
	int count = 0;
	while (p < end) {
		if (*p == ' ' || (*p >= '0' && *p <= '9') || (*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') || strchr(safeChars, *p)) {
		} else {
			count++;
		}
		p++;
	}

	int size = len + count * 2 + 1;
	if (dst) {
		while (src < end) {
			if (*src == ' ') {
				*dst = '+';
			} else if ((*src >= '0' && *src <= '9') || (*src >= 'A' && *src <= 'Z') || (*src >= 'a' && *src <= 'z') || strchr(safeChars, *src)) {
				*dst = *src;
			} else {
				*dst = '%';
				if (((unsigned char)*src >> 4) <= 9) {
					*++dst = ((unsigned char)*src >> 4) + '0';
				} else {
					*++dst = ((unsigned char)*src >> 4) + 'A' - 10;
				}
				if ((*src & 0x0F) <= 9) {
					*++dst = (*src & 0x0F) + '0';
				} else {
					*++dst = (*src & 0x0F) + 'A' - 10;
				}
			}
			src++;
			dst++;
		}
		*dst = '\0';
	}
	return size;
}

// A hexadecimal digit, either case.
#define IS_HEX_DIGIT(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))

// utilities.cpp:182
int DecodingUrl(const char* src, char* dst) {
	int len = lstrlenA(src);
	if (len == 0) {
		return 0;
	}

	const char* end = src + len;
	const char* p = src;
	int count = 0;
	while (p < end - 2) {
		if (*p == '%' && IS_HEX_DIGIT(p[1]) && IS_HEX_DIGIT(p[2])) {
			count++;
			p += 3;
		} else {
			p++;
		}
	}

	int size = len - count * 2 + 1;
	if (dst) {
		while (src < end) {
			if (*src == '%' && IS_HEX_DIGIT(src[1]) && IS_HEX_DIGIT(src[2])) {
				if (src[1] >= '0' && src[1] <= '9') {
					*dst = (src[1] - '0') << 4;
				} else if (src[1] >= 'A' && src[1] <= 'F') {
					*dst = (src[1] - 'A' + 10) << 4;
				} else {
					*dst = (src[1] - 'a' + 10) << 4;
				}
				if (src[2] >= '0' && src[2] <= '9') {
					*dst += src[2] - '0';
				} else if (src[2] >= 'A' && src[2] <= 'F') {
					*dst += src[2] - 'A' + 10;
				} else {
					*dst += src[2] - 'a' + 10;
				}
				dst++;
				src += 3;
			} else {
				if (*src == '+') {
					*dst = ' ';
				} else {
					*dst = *src;
				}
				dst++;
				src++;
			}
		}
		*dst = '\0';
	}
	return size;
}

// utilities.cpp:326
int GetEquipSkinIndex(eEquipSkin& skin, unsigned long typeId) {
	if ((typeId & 0xFC000000) == 0x38000000) {
		switch ((typeId >> 22) & 0x0F) {
		case 0: skin = (eEquipSkin)0; break;
		case 1: skin = (eEquipSkin)1; break;
		case 2: skin = (eEquipSkin)2; break;
		case 3: skin = (eEquipSkin)3; break;
		case 4: skin = (eEquipSkin)4; break;
		case 6: skin = (eEquipSkin)5; break;
		default:
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

// utilities.cpp:346
int ErasePartsCardAll(unsigned long partsId, std::list<unsigned long>& typeIds) {
	std::list<sSCardAvilityPeriodInfo>& cards = WSingleton<CSharedDoc>::Instance()->m_cardPeriodList[0];
	std::list<sSCardAvilityPeriodInfo>::iterator it = cards.begin();

	while (it != cards.end()) {
		if (it->partsId == partsId) {
			typeIds.push_back(it->typeId);
			it = WSingleton<CSharedDoc>::Instance()->m_cardPeriodList[0].erase(it);
		} else {
			++it;
		}
	}

	std::list<sItemInfo>::iterator part;
	for (part = WSingleton<CSharedDoc>::Instance()->m_myPartsList.begin(); part != WSingleton<CSharedDoc>::Instance()->m_myPartsList.end() && part->m_serial != partsId; ++part) {
	}
	if (part != WSingleton<CSharedDoc>::Instance()->m_myPartsList.end()) {
		for (int i = 0; i < 12; i++) {
			if (part->m_cardSlots[i] != 0) {
				std::list<unsigned long>::iterator id = typeIds.begin();
				while (id != typeIds.end()) {
					if (part->m_cardSlots[i] == (int)*id) {
						part->m_cardSlots[i] = 0;
					}
					++id;
				}
			}
		}
	}

	return typeIds.size() != 0;
}

// The base64 alphabet and its inverse (-1: not a base64 character).
static unsigned char s_base64EncodeTable[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static int s_base64DecodeTable[256] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
	-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
	-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

// utilities.cpp:390
int Base64Encode(char* src, int len, char* dst) {
	unsigned char in[3] = { 0, 0, 0 };
	unsigned char out0;
	unsigned char out1;
	unsigned char out2;
	unsigned char out3;
	int written = 0;

	char* last = src + len - 1;
	int size = (len / 3) * 4 + ((len % 3) ? 4 : 0) + 1;
	int i = 0;
	for (char* p = src; p <= last; p++, i++) {
		int mod = i % 3;
		in[mod] = *p;
		if (mod == 2 || p == last) {
			out0 = in[0] >> 2;
			out1 = ((in[0] & 0x03) << 4) | (in[1] >> 4);
			out2 = ((in[1] & 0x0F) << 2) | (in[2] >> 6);
			out3 = in[2] & 0x3F;
			dst[written++] = s_base64EncodeTable[out0];
			dst[written++] = s_base64EncodeTable[out1];
			dst[written++] = (mod == 0) ? '=' : s_base64EncodeTable[out2];
			dst[written++] = (mod < 2) ? '=' : s_base64EncodeTable[out3];
			in[0] = in[1] = in[2] = 0;
		}
	}
	dst[written] = '\0';
	return size;
}

// utilities.cpp:420
int Base64Decode(char* src, unsigned char* dst, int dstSize) {
	int phase = 0;
	int prev = 0;
	int written = 0;
	for (; *src; src++) {
		int value = s_base64DecodeTable[*src];
		if (value == -1) {
			continue;
		}
		switch (phase) {
		case 0:
			phase = 1;
			break;
		case 1:
			if (written < dstSize) {
				dst[written++] = (unsigned char)((prev << 2) | ((value >> 4) & 0x03));
			}
			phase = 2;
			break;
		case 2:
			if (written < dstSize) {
				dst[written++] = (unsigned char)((prev << 4) | ((value >> 2) & 0x0F));
			}
			phase = 3;
			break;
		case 3:
			if (written < dstSize) {
				dst[written++] = (unsigned char)((prev << 6) | value);
			}
			phase = 0;
			break;
		}
		prev = value;
	}
	if (written > 0) {
		dst[written] = '\0';
	}
	return written;
}

// utilities.cpp:470
int IsLunarYear(unsigned short year) {
	if (year % 4 == 0) {
		if (year % 100 == 0) {
			return year % 400 == 0;
		}
		return TRUE;
	}
	return FALSE;
}

// utilities.cpp:486
unsigned short GetMonthlyDayCount(unsigned short year, unsigned short month) {
	if (month == 2) {
		if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
			return 28;
		}
		return 29;
	}
	if (month == 4 || month == 6 || month == 9 || month == 11) {
		return 30;
	}
	return 31;
}

// utilities.cpp:501
unsigned long GetNickColor(unsigned long identity, int isNewbie, unsigned short flags) {
	if (identity & 0x14) {
		if (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().identity & 0x14) {
			return 0x50FFFFFF;
		}
		if (!(flags & 1)) {
			return 0;
		}
	} else if (IsLocalContent((localContentType_t)0x10)) {
		if (isNewbie) {
			return 0xFF33DBFF;
		}
	}
	return 0xFFFFFFFF;
}

// utilities.cpp:519
int GetHoleinCoinCount(int coinCount, int holeinCount) {
	if (coinCount == 0) {
		return 0;
	}
	if (holeinCount >= 2) {
		return coinCount * holeinCount;
	}
	return coinCount;
}

// utilities.cpp:537
bool IsEventServerControl(eGameServerProperty property) {
	return (WSingleton<CSharedDoc>::Instance()->m_serverProperty & property) ? true : false;
}

// utilities.cpp:542
bool FindVisibleWindowFromExStyle(unsigned long exStyle, std::vector<HWND>& windows) {
	HWND hWnd = FindWindowA(NULL, NULL);
	while (hWnd) {
		if (IsWindowVisible(hWnd)) {
			unsigned long style = GetWindowLongA(hWnd, GWL_EXSTYLE);
			if ((style & exStyle) == exStyle) {
				windows.push_back(hWnd);
			}
		}
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
	}
	return !windows.empty();
}

// utilities.cpp:558
int GetWindowBorderSize(HWND hWnd) {
	RECT windowRect;
	RECT clientRect;
	GetWindowRect(hWnd, &windowRect);
	GetClientRect(hWnd, &clientRect);

	return ((windowRect.right - windowRect.left) - (clientRect.right - clientRect.left)) / 2;
}

// utilities.cpp:568
int GetWindowTitleBarSize(HWND hWnd) {
	RECT windowRect;
	RECT clientRect;
	GetWindowRect(hWnd, &windowRect);
	GetClientRect(hWnd, &clientRect);

	int borderSize = GetWindowBorderSize(hWnd);
	return (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top) - borderSize;
}

// utilities.cpp:579
bool IsInRect(const RECT& rc, int x, int y) {
	return x >= rc.left && x <= rc.right && y >= rc.top && y <= rc.bottom;
}

// utilities.cpp:585
bool IsOverlappedRect(const RECT& rc1, const RECT& rc2) {
	if (IsInRect(rc2, rc1.left, rc1.top) || IsInRect(rc2, rc1.left, rc1.bottom) || IsInRect(rc2, rc1.right, rc1.top) || IsInRect(rc2, rc1.right, rc1.bottom)) {
		return true;
	}

	if (IsInRect(rc1, rc2.left, rc2.top) || IsInRect(rc1, rc2.left, rc2.bottom) || IsInRect(rc1, rc2.right, rc2.top) || IsInRect(rc1, rc2.right, rc2.bottom)) {
		return true;
	}

	return false;
}

// utilities.cpp:601
bool CalculateUVPos(WRect& rc, int textureWidth, int textureHeight, float cellWidth, float cellHeight, int index) {
	if ((float)textureWidth < cellWidth || (float)textureHeight < cellHeight) {
		return false;
	}

	int rows = (int)((float)textureHeight / cellHeight);
	if ((int)((float)textureWidth / cellWidth) * rows < index) {
		return false;
	}

	float invWidth = 1.0f / (float)textureWidth;
	int column = index / rows;
	int row = index % rows;
	float invHeight = 1.0f / (float)textureHeight;
	rc.x = (float)column * cellWidth * invWidth;
	rc.y = (float)row * cellHeight * invHeight;
	rc.w = cellWidth * invWidth;
	rc.h = cellHeight * invHeight;
	return true;
}

} // namespace UTIL

// utilities.cpp:631
bool UnderCategory(unsigned long charTypeId, unsigned long partTypeId, unsigned long slotMask) {
	if ((partTypeId & 0x600) || ((partTypeId >> 18) & 0xFF) != (charTypeId & 0x3FFFFFF)) {
		return false;
	}

	IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(partTypeId);
	if (pPart) {
		if (slotMask & (1 << pPart->slot)) {
			return true;
		}
	}
	return false;
}

// utilities.cpp:646
bool UnderCatogory(unsigned long charTypeId, unsigned long slotMask, IFF_STRUCT::sPart* pPart) {
	if ((pPart->m_typeId & 0x600) || ((pPart->m_typeId >> 18) & 0xFF) != (charTypeId & 0x3FFFFFF)) {
		return false;
	}

	return (slotMask & (1 << pPart->slot)) ? true : false;
}

// utilities.cpp:660
void GetLine(char* src, char* dst) {
	while (*src) {
		if (*src == '\\' && src[1] == 'n') {
			*dst++ = '\n';
			src += 2;
			continue;
		}
		*dst++ = *src++;
	}
}

// utilities.cpp:681
bool IsUsableCalipers(void) {
	IActor* pActor = AfxGetTask()->GetActor(s_gatewayActorName);
	if (pActor == 0) {
		return false;
	}

	int usable = 0;
	pActor << MsgObject(0, 0x26F, 1, (intptr_t)&usable, 0, 0, 0);

	return usable == 1;
}

} // namespace S5

namespace _util {

// utilities.cpp:713
const char* GetAvatarServerServiceMessage(int property) {
	const char* msg = NULL;
	switch (property) {
	case 0x2:
	case 0x40:
	case 0x80:
	case 0x100:
	case 0x200:
	case 0x400:
	case 0x800:
	case 0x1000:
		msg = "\xb4\xeb\xc8\xad\xb9\xe6 \xc0\xfc\xbf\xeb \xbc\xad\xb9\xf6\xbf\xa1\xbc\xad\xb4\xc2 \xb0\xd4\xc0\xd3\xb9\xe6\xc0\xbb \xb8\xb8\xb5\xe9 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.";
		break;
	}
	return msg;
}

// utilities.cpp:735
const char* GetNormalServerServiceMessage(int property) {
	const char* msg = NULL;
	switch (property) {
	case 0x2000:
		msg = "\xc7\xf6\xc0\xe7 \xbc\xad\xb9\xf6\xbf\xa1\xbc\xad\xb4\xc2 \xb4\xeb\xc8\xad\xb9\xe6\xc0\xbb \xb8\xb8\xb5\xe9 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.";
		break;
	}
	return msg;
}

// utilities.cpp:746
bool IsAvatarChatServer(int flag) {
	return (WSingleton<CSharedDoc>::Instance()->m_serverChannel.flags & flag) ? true : false;
}

// utilities.cpp:755
void GenerateRandomTidParts(unsigned long tidChar, unsigned long* const partTids, unsigned long* const partIds) {
	CPartTidList tidList;

	partIds[0] = 0;
	partTids[0] = 0;

	ItemManager()->GetDefCombo(tidChar, partTids);

	do {
		switch (tidChar & 0x3FFFFFF) {
		case 0:
			partTids[0] = rand() % 11 | 0x8000800;
			partTids[3] = rand() % 20 | 0x8006000;
			partTids[4] = rand() % 3 | 0x8008800;
			partTids[5] = rand() % 17 | 0x800A000;
			partTids[8] = rand() % 11 | 0x8010000;
			break;
		case 1:
			partTids[0] = rand() % 20 | 0x8040800;
			partTids[2] = rand() % 20 | 0x8044000;
			partTids[4] = rand() % 11 | 0x8048000;
			partTids[7] = rand() % 10 | 0x804E000;
			break;
		case 2:
			partTids[0] = rand() % 9 | 0x8080800;
			partTids[2] = (rand() % 18 + 1) | 0x8084000;
			partTids[4] = rand() % 4 | 0x8088800;
			partTids[5] = (rand() % 14 + 1) | 0x808A000;
			partTids[7] = rand() % 11 | 0x808E000;
			partTids[8] = rand() % 3 | 0x8090800;
			break;
		case 3:
			partTids[0] = rand() % 7 | 0x80C0000;
			partTids[2] = rand() % 22 | 0x80C4000;
			partTids[4] = rand() % 5 | 0x80C8000;
			partTids[5] = rand() % 15 | 0x80CA000;
			partTids[7] = rand() % 12 | 0x80CE000;
			partTids[10] = rand() % 4 | 0x80D4800;
			break;
		case 4:
			partTids[0] = rand() % 6 | 0x8100000;
			partTids[2] = rand() % 17 | 0x8104000;
			partTids[3] = rand() % 8 | 0x8106000;
			partTids[4] = rand() % 12 | 0x8108000;
			partTids[5] = rand() % 10 | 0x810A000;
			partTids[9] = rand() % 4 | 0x8112000;
			partTids[10] = rand() % 2 | 0x8114000;
			partTids[11] = rand() % 2 | 0x8116000;
			break;
		case 5:
			partTids[0] = rand() % 6 | 0x8140000;
			partTids[2] = rand() % 12 | 0x8144000;
			partTids[5] = rand() % 13 | 0x814A000;
			partTids[7] = rand() % 15 | 0x814E000;
			break;
		case 6:
			partTids[0] = rand() % 10 | 0x8180000;
			partTids[2] = rand() % 15 | 0x8184000;
			partTids[4] = rand() % 8 | 0x8188000;
			partTids[5] = rand() % 9 | 0x818A000;
			break;
		case 8:
			partTids[0] = 0x8200000;
			partTids[2] = 0x8204018;
			partTids[4] = 0x8208000;
			partTids[5] = 0x820A000;
			partTids[7] = 0x820E000;
			break;
		case 9:
			partTids[0] = 0x824001E;
			partTids[2] = 0x8244000;
			partTids[4] = 0x8248003;
			partTids[5] = 0x824A000;
			partTids[7] = 0x824E007;
			break;
		}

		sCharacterInfo charInfo = sCharacterInfo();
		charInfo.typeId = tidChar;
		memcpy(charInfo.partTypeIds, partTids, sizeof(charInfo.partTypeIds));

		tidList.SetTids(&charInfo, 0xFF);
	} while (!tidList.IsComboValid());
}

} // namespace _util
