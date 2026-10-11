// Original: d:\build\custom\temp\code\client\projectg\ucclibrary.h
#ifndef PG_CLIENT_PROJECTG_UCCLIBRARY_H
#define PG_CLIENT_PROJECTG_UCCLIBRARY_H

#include <string.h>
#include <string>

#include "..\..\defines.h"

class Bitmap;
class FrGraphicInterface;
class WReceivedPacket;
struct sItemInfo;
struct sCharacterInfo;
struct sMailIncludeItem;
struct sUserInfoTime;
struct sUserInfo;
struct sRoomSlot;
namespace IFF_STRUCT {
	struct sPart;
}
namespace ranktype {
	struct sRankUserInfo;
}

// The "header" entry of a UCC texture archive, filled by UnzipUccClothes.
struct sUccHeader {
	sUccHeader(void)
	{
		Initialize();
	}

	// ucclibrary.h:28
	void Initialize(void)
	{
		m_flags = 0;
		m_value4 = 0;
		m_value8 = 0;
		memset(m_data, 0, sizeof(m_data));
	}

	unsigned long m_flags;          // 0x00, bit 0: show the warning clothes instead (_RealDownLoad)
	unsigned long m_value4;         // 0x04, use not seen yet
	unsigned long m_value8;         // 0x08, use not seen yet
	char m_data[0x100];             // 0x0C, use not seen yet
};
CHECK_SIZE(sUccHeader, 0x10C);
CHECK_FIELD(sUccHeader, m_flags, 0x00);
CHECK_FIELD(sUccHeader, m_value4, 0x04);
CHECK_FIELD(sUccHeader, m_value8, 0x08);
CHECK_FIELD(sUccHeader, m_data, 0x0C);

// ucclibrary.h:60
template <class T> T Max(T a, T b, T c) {
	T bc = b >= c ? b : c;
	return a >= bc ? a : bc;
}

// ucclibrary.h:66
template <class T> T Min(T a, T b, T c) {
	T bc = b > c ? c : b;
	return a > bc ? bc : a;
}

struct sRGB;

// A colour in the hue/saturation/value space, one byte per channel.
struct sHSV {
	sHSV(void) {                      // ucclibrary.h:185
		h = s = v = 0;
	}

	sRGB toRGB(void);

	unsigned char h;   // 0x0
	unsigned char s;   // 0x1
	unsigned char v;   // 0x2
};

CHECK_SIZE(sHSV, 0x3);
CHECK_FIELD(sHSV, h, 0x0);
CHECK_FIELD(sHSV, s, 0x1);
CHECK_FIELD(sHSV, v, 0x2);

// A 24-bit colour.
struct sRGB {
	sHSV toHSV(void);

	unsigned char r;   // 0x0
	unsigned char g;   // 0x1
	unsigned char b;   // 0x2
};

CHECK_SIZE(sRGB, 0x3);
CHECK_FIELD(sRGB, r, 0x0);
CHECK_FIELD(sRGB, g, 0x1);
CHECK_FIELD(sRGB, b, 0x2);

// A 32-bit colour with alpha.
struct sRGBA : public sRGB {
	sRGBA(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) {   // ucclibrary.h:217
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	void RemoveAlpha(void);

	unsigned char a;   // 0x3
};

CHECK_SIZE(sRGBA, 0x4);
CHECK_FIELD(sRGBA, a, 0x3);

const char* GetUccTempFoler(void);
bool IsUccClothes(unsigned long typeId);
bool IsSameClothes(unsigned long typeId, unsigned long otherTypeId);
bool IsSameClothes(IFF_STRUCT::sPart* pPart, IFF_STRUCT::sPart* pOtherPart);
std::string FilterFilename(std::string filename);
void MixColor(sRGBA& back, sRGBA& front, sRGBA& result);
bool NoDrawArea(const Bitmap* pBitmap, int x, int y, int width, int height);
unsigned char HueToRGB(unsigned char n1, unsigned char n2, unsigned char hue);
void GetUccInfoFromPacket(WReceivedPacket* pPacket);
bool ZipUccClothes(Bitmap* pTexture, Bitmap* pMask, Bitmap* pIcon, const char* filename);
bool UnzipUccClothes(unsigned char* pData, unsigned long size, Bitmap* pTexture, Bitmap* pMask, Bitmap* pIcon, sUccHeader* pHeader);
bool LoadWarningClothes(unsigned int typeId, Bitmap* pTexture, Bitmap* pMask, Bitmap* pIcon);
FrGraphicInterface *GetGDI(void);
void RefreshTexCache(Bitmap& bitmap);
void RefreshTexCache(Bitmap *bitmap);
void ClearTexCache(Bitmap *bitmap);
float Power(float x, float y);
void Convert24to32(Bitmap *src, Bitmap *dest, const Bitmap *mask);
void Convert32to24(Bitmap *src, Bitmap *dest, const Bitmap *mask);
void SetBrightness(sRGBA& color, sRGBA& brightness, sRGBA& result);
const Bitmap* GetUccItemIcon(sItemInfo* pItem);
const char* GetUccItemName(sItemInfo* pItem);
void RequestUccInfo(sCharacterInfo& info);
void RequestUccInfo(sMailIncludeItem& item);
void RequestUccInfo(sUserInfoTime& info);
void RequestUccInfo(sUserInfo& info);
void RequestUccInfo(sRoomSlot& slot);
void RequestUccInfo(ranktype::sRankUserInfo& info);
sItemInfo *GetMyItemInfo(unsigned long serial);
sItemInfo *GetMyItemInfo(unsigned long typeId, const char* uccKey, unsigned short uccDrawCount);
const Bitmap* GetUccItemIcon(unsigned long serial);
const char* GetUccItemName(unsigned long serial);
const Bitmap* GetUccItemIcon(unsigned long typeId, const char* uccKey);
const char* GetUccItemName(unsigned long typeId, const char* uccKey);
std::string Lower(std::string text);
std::string GetMaskFilename(const char* filename);
std::string GenerateIconFilename(unsigned long typeId, const char *uccKey);
std::string GenerateTextureName_F(unsigned long typeId, const char *uccKey);
std::string GenerateTextureName_B(unsigned long typeId, const char *uccKey);
std::string GenerateTextureFullPath(int side, unsigned long typeId, const char *uccKey);
sRGBA GetAverageColor(const Bitmap* pBitmap, int x, int y, int width, int height, bool ignoreAlpha);
sRGBA GetSimilarColor(sRGBA color, sRGBA first, sRGBA second);
void RGBToHLS(unsigned char r, unsigned char g, unsigned char b, unsigned char& h, unsigned char& l, unsigned char& s);
void HLSToRGB(unsigned char h, unsigned char l, unsigned char s, unsigned char& r, unsigned char& g, unsigned char& b);
void GetResizedImage(const Bitmap* pSrc, const Bitmap* pMask, Bitmap* pDest, int width, int height);
void CreateItemThumbnail(const Bitmap* pTexture, const Bitmap* pMask, Bitmap* pDest);

// A pixel position on a UCC canvas (CUccBase::DrawLine takes two by value).
struct IPoint {
	IPoint(void) {                    // ucclibrary.h:252
		x = 0;
		y = 0;
	}

	IPoint(int _x, int _y) {          // ucclibrary.h:258
		x = _x; y = _y;
	}

	IPoint(float _x, float _y) {      // ucclibrary.h:264
		x = (int)_x;
		y = (int)_y;
	}

	bool operator==(const IPoint& point) const {   // ucclibrary.h:271
		return x == point.x && y == point.y;
	}

	int x;   // 0x0
	int y;   // 0x4
};

CHECK_SIZE(IPoint, 0x8);
CHECK_FIELD(IPoint, x, 0x0);
CHECK_FIELD(IPoint, y, 0x4);

#endif // PG_CLIENT_PROJECTG_UCCLIBRARY_H
