// Original: d:\build\custom\temp\code\client\projectg\ucclibrary.cpp
#include <windows.h>
#include <shlwapi.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include <algorithm>

#include "UccLibrary.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "SharedDoc.h"
#include "../shared/ItemManager.h"
#include "../shared/classdefine.h"
#include "Packet.h"
#include "UccManager.h"
#include "NetResourceManager.h"
#include "commonutil.h"
#include "bitmap.h"
#include "westpak.h"
#include "../Wangreal/source/XUnzip.h"

// xzip.h (Wangreal)
HZIP CreateZipZ(void* z, unsigned int len, unsigned long flags, unsigned long attributes);
ZRESULT ZipAdd(HZIP hz, const char* dstzn, void* src, unsigned int len, unsigned long flags);
ZRESULT CloseZipZ(HZIP hz);

// westpak.cpp
cFile* __cdecl GetCFile(const char* filename, int pakIndex, int flags);

namespace ranktype {

// A user's entry of the rank server's replies (its constructor is emitted in
// rankingdlg.obj); the original header is not known yet, layout partial.
#pragma pack(push, 1)
struct sRankUserInfo {
	UNKNOWN(0x0, 0x32);
	sCharacterInfo m_characterInfo;   // 0x32 (RequestUccInfo)
};
#pragma pack(pop)

CHECK_FIELD(sRankUserInfo, m_characterInfo, 0x32);

} // namespace ranktype

// ucclibrary.cpp:17
const char* GetUccTempFoler(void) {
	return "capture";
}

// ucclibrary.cpp:22
bool IsUccClothes(unsigned long typeId) {
	if (!WSingleton<CSharedDoc>::IsInstantiated() || ItemManager() == NULL) {
		return false;
	}

	if ((typeId & 0xFC000000) == 0x08000000) {
		IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(typeId);

		if (pPart != NULL) {
			if (pPart->slot == 7 || pPart->slot == 8 || pPart->slot == 9) {
				return true;
			}
		}
	}

	return false;
}

// ucclibrary.cpp:44
bool IsSameClothes(IFF_STRUCT::sPart* pPart, IFF_STRUCT::sPart* pOtherPart) {
	if (pPart == NULL || pOtherPart == NULL) {
		return false;
	}

	bool same = !_strcmpi(pPart->model, pOtherPart->model);
	for (int i = 0; i < 3; i++) {
		same = same
			&& !_strcmpi(pPart->texture[i], pOtherPart->texture[i])
			&& !_strcmpi(pPart->texture2[i], pOtherPart->texture2[i]);
	}

	return same;
}

// ucclibrary.cpp:65
bool IsSameClothes(unsigned long typeId, unsigned long otherTypeId) {
	if (typeId == otherTypeId) {
		return true;
	}

	if (!WSingleton<CSharedDoc>::IsInstantiated() || ItemManager() == NULL) {
		return false;
	}

	IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(typeId);
	IFF_STRUCT::sPart* pOtherPart = ItemManager()->FindPart(otherTypeId);

	return IsSameClothes(pPart, pOtherPart);
}

// ucclibrary.cpp:79
void CreateItemThumbnail(const Bitmap* pTexture, const Bitmap* pMask, Bitmap* pDest) {
	if (pTexture == NULL || pDest == NULL) {
		return;
	}

	Bitmap resized;

	float scale = 64.0f / (std::max)(pTexture->Width(), pTexture->Height());
	float width = pTexture->Width() * scale;
	float height = pTexture->Height() * scale;
	float offsetX = (64.0f - width) * 0.5f;
	float offsetY = (84.0f - height) * 0.5f;

	GetResizedImage(pTexture, pMask, &resized, (int)width, (int)height);

	pDest->Create(64, 84, 32);
	for (int y = 0; y < 84; y++) {
		for (int x = 0; x < 64; x++) {
			if (x < offsetX || x >= offsetX + width || y < offsetY || y >= offsetY + height) {
				pDest->SetPixel(x, y, 0xFF, 0xFF, 0xFF, 0);
				continue;
			}

			unsigned char r, g, b, a;
			resized.GetPixel(x - (int)offsetX, y - (int)offsetY, r, g, b, a);
			pDest->SetPixel(x, y, r, g, b, a);
		}
	}
}

// ucclibrary.cpp:117
const Bitmap* GetUccItemIcon(sItemInfo* pItem) {
	if (pItem == NULL) {
		return NULL;
	}

	if (pItem->m_uccStatus & 1) {
		sUccClothes* pClothes = UccManager()->FindClothes(pItem->m_typeId, pItem->m_uccKey);
		if (pClothes != NULL && UccManager()->IsDownloadComplete(pClothes)) {
			return pClothes->m_pIcon;
		}
	}

	if (!WSingleton<CSharedDoc>::IsInstantiated() || ItemManager() == NULL) {
		return NULL;
	}

	IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(pItem->m_typeId);
	if (pPart == NULL) {
		return NULL;
	}

	return g_resrcmng->LoadBitmap(pPart->m_icon, 0, false);
}

// ucclibrary.cpp:141
const char* GetUccItemName(sItemInfo* pItem) {
	if (!WSingleton<CSharedDoc>::IsInstantiated() || ItemManager() == NULL) {
		return "";
	}

	if (!(pItem->m_uccStatus & 1)) {
		IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(pItem->m_typeId);
		if (pPart == NULL) {
			return "";
		}

		return pPart->m_name;
	}

	sUccClothes* pClothes = UccManager()->FindClothes(pItem->m_typeId, pItem->m_uccKey);
	if (pClothes == NULL) {
		return "";
	}

	if (UccManager()->IsDownloadComplete(pClothes)) {
		return pClothes->m_uccName;
	}

	bool requested = NetResourceManager::Instance()->IsDownloadRequested(RESOURCE_TYPE_UCC_CLOTHES, pItem->m_typeId, pItem->m_uccKey);

	if (!requested) {
		// "%s (download failed)"
		return MakeStr("%s (\xb4\xd9\xbf\xee\xb7\xce\xb5\xe5 \xbd\xc7\xc6\xd0)", pClothes->m_uccName);
	}

	// "%s (downloading)"
	return MakeStr("%s (\xb4\xd9\xbf\xee\xb7\xce\xb5\xe5 \xc1\xdf)", pClothes->m_uccName);
}

// ucclibrary.cpp:190
sItemInfo* GetMyItemInfo(unsigned long serial) {
#define FIND_MY_ITEM_BY_SERIAL(items) \
	for (std::list<sItemInfo>::iterator it##items = WSingleton<CSharedDoc>::Instance()->items.begin(); it##items != WSingleton<CSharedDoc>::Instance()->items.end(); ++it##items) { \
		if ((*it##items).m_serial == serial) return &(*it##items); \
	}

	FIND_MY_ITEM_BY_SERIAL(m_myItemList)
	FIND_MY_ITEM_BY_SERIAL(m_myAuxItemList)
	FIND_MY_ITEM_BY_SERIAL(m_myPartsList)
	FIND_MY_ITEM_BY_SERIAL(m_itemInfoList)
	FIND_MY_ITEM_BY_SERIAL(m_myGearList)
	FIND_MY_ITEM_BY_SERIAL(m_myExtraItemList)
	FIND_MY_ITEM_BY_SERIAL(m_myExtraItemList2)
	FIND_MY_ITEM_BY_SERIAL(m_mySkinList)
	FIND_MY_ITEM_BY_SERIAL(m_myExtraItemList3)
	FIND_MY_ITEM_BY_SERIAL(m_myExtraItemList4)

	return NULL;
#undef FIND_MY_ITEM_BY_SERIAL
}

// ucclibrary.cpp:221
sItemInfo* GetMyItemInfo(unsigned long typeId, const char* uccKey, unsigned short uccDrawCount) {
#define FIND_MY_UCC_ITEM(items) \
	for (std::list<sItemInfo>::iterator it##items = WSingleton<CSharedDoc>::Instance()->items.begin(); it##items != WSingleton<CSharedDoc>::Instance()->items.end(); ++it##items) { \
		if (strlen((*it##items).m_uccKey) != 0 && (*it##items).m_uccSeq != 0 && (*it##items).m_typeId == typeId && strcmp((*it##items).m_uccKey, uccKey) == 0 && (uccDrawCount == 0 || (*it##items).m_uccSeq == uccDrawCount)) return &(*it##items); \
	}

	FIND_MY_UCC_ITEM(m_myItemList)
	FIND_MY_UCC_ITEM(m_myAuxItemList)
	FIND_MY_UCC_ITEM(m_myPartsList)
	FIND_MY_UCC_ITEM(m_itemInfoList)
	FIND_MY_UCC_ITEM(m_myGearList)
	FIND_MY_UCC_ITEM(m_myExtraItemList)
	FIND_MY_UCC_ITEM(m_myExtraItemList2)
	FIND_MY_UCC_ITEM(m_mySkinList)
	FIND_MY_UCC_ITEM(m_myExtraItemList3)
	FIND_MY_UCC_ITEM(m_myExtraItemList4)

	return NULL;
#undef FIND_MY_UCC_ITEM
}

// ucclibrary.cpp:242
const Bitmap* GetUccItemIcon(unsigned long serial) {
	if (serial == 0xFFFFFFFF) {
		return NULL;
	}

	sItemInfo* pItem = GetMyItemInfo(serial);
	if (pItem != NULL && !(pItem->m_uccStatus & 1)) {
		IFF_STRUCT::sPart* pPart = ItemManager() != NULL ? ItemManager()->FindPart(pItem->m_typeId) : NULL;
		if (pPart != NULL) {
			return g_pFresh->GetBitmap(pPart->m_icon);
		}
	}

	sUccClothes* pClothes = UccManager()->FindClothes(serial);
	if (pClothes != NULL && UccManager()->IsDownloadComplete(pClothes)) {
		return pClothes->m_pIcon;
	}

	return NULL;
}

// ucclibrary.cpp:268
const char* GetUccItemName(unsigned long serial) {
	if (serial == 0xFFFFFFFF) {
		return "";
	}

	sItemInfo* pItem = GetMyItemInfo(serial);
	if (pItem != NULL && !(pItem->m_uccStatus & 1)) {
		IFF_STRUCT::sPart* pPart = ItemManager() != NULL ? ItemManager()->FindPart(pItem->m_typeId) : NULL;
		if (pPart != NULL) {
			return pPart->m_name;
		}
	}

	sUccClothes* pClothes = UccManager()->FindClothes(serial);
	if (pClothes != NULL) {
		if (UccManager()->IsDownloadComplete(pClothes)) {
			return pClothes->m_uccName;
		}

		bool requested = NetResourceManager::Instance()->IsDownloadRequested(RESOURCE_TYPE_UCC_CLOTHES, pClothes->m_typeId, pClothes->m_uccKey);

		if (!requested) {
			// "%s (download failed)"
			return MakeStr("%s (\xb4\xd9\xbf\xee\xb7\xce\xb5\xe5 \xbd\xc7\xc6\xd0)", pClothes->m_uccName);
		}

		// "%s (downloading)"
		return MakeStr("%s (\xb4\xd9\xbf\xee\xb7\xce\xb5\xe5 \xc1\xdf)", pClothes->m_uccName);
	}

	return "";
}

// ucclibrary.cpp:309
const Bitmap* GetUccItemIcon(unsigned long typeId, const char* uccKey) {
	if (typeId == 0 || uccKey == NULL || strlen(uccKey) == 0) {
		return NULL;
	}

	sItemInfo* pItem = GetMyItemInfo(typeId, uccKey, 0);
	if (pItem != NULL && !(pItem->m_uccStatus & 1)) {
		IFF_STRUCT::sPart* pPart = ItemManager() != NULL ? ItemManager()->FindPart(pItem->m_typeId) : NULL;
		if (pPart != NULL) {
			return g_pFresh->GetBitmap(pPart->m_icon);
		}
	}

	sUccClothes* pClothes = UccManager()->FindClothes(typeId, uccKey);
	if (pClothes != NULL && UccManager()->IsDownloadComplete(pClothes)) {
		return pClothes->m_pIcon;
	}

	IFF_STRUCT::sPart* pPart = ItemManager() != NULL ? ItemManager()->FindPart(typeId) : NULL;
	if (pPart != NULL) {
		return g_pFresh->GetBitmap(pPart->m_icon);
	}

	return NULL;
}

// ucclibrary.cpp:342
const char* GetUccItemName(unsigned long typeId, const char* uccKey) {
	if (typeId == 0 || uccKey == NULL || strlen(uccKey) == 0) {
		return "";
	}

	sItemInfo* pItem = GetMyItemInfo(typeId, uccKey, 0);
	if (pItem != NULL && !(pItem->m_uccStatus & 1)) {
		IFF_STRUCT::sPart* pPart = ItemManager() != NULL ? ItemManager()->FindPart(pItem->m_typeId) : NULL;
		if (pPart != NULL) {
			return pPart->m_name;
		}
	}

	sUccClothes* pClothes = UccManager()->FindClothes(typeId, uccKey);
	if (pClothes != NULL) {
		if (UccManager()->IsDownloadComplete(pClothes)) {
			return pClothes->m_uccName;
		}

		bool requested = NetResourceManager::Instance()->IsDownloadRequested(RESOURCE_TYPE_UCC_CLOTHES, pClothes->m_typeId, pClothes->m_uccKey);

		if (!requested) {
			// "%s (download failed)"
			return MakeStr("%s (\xb4\xd9\xbf\xee\xb7\xce\xb5\xe5 \xbd\xc7\xc6\xd0)", pClothes->m_uccName);
		}

		// "%s (downloading)"
		return MakeStr("%s (\xb4\xd9\xbf\xee\xb7\xce\xb5\xe5 \xc1\xdf)", pClothes->m_uccName);
	}

	IFF_STRUCT::sPart* pPart = ItemManager() != NULL ? ItemManager()->FindPart(typeId) : NULL;
	if (pPart != NULL) {
		return pPart->m_name;
	}

	return "";
}

// ucclibrary.cpp:392
bool ZipUccClothes(Bitmap* pTexture, Bitmap* pMask, Bitmap* pIcon, const char* filename) {
	unsigned int textureSize = pTexture != NULL ? pTexture->BitsPerPixel() * pTexture->Height() * pTexture->Width() / 8 : 0;
	unsigned int maskSize = pMask != NULL ? pMask->BitsPerPixel() * pMask->Height() * pMask->Width() / 8 : 0;
	unsigned int iconSize = pIcon != NULL ? pIcon->BitsPerPixel() * pIcon->Height() * pIcon->Width() / 8 : 0;

	bool result = true;

	HZIP hZip = CreateZipZ((void*)filename, 0, 2, 2);
	if (pTexture != NULL) {
		result = ZipAdd(hZip, "front", pTexture->vram, textureSize, 3) == 0;
	}

	if (pMask != NULL) {
		result = result && ZipAdd(hZip, "back", pMask->vram, maskSize, 3) == 0;
	}

	if (pIcon != NULL) {
		result = result && ZipAdd(hZip, "icon", pIcon->vram, iconSize, 3) == 0;
	}

	IsZipHandleU(hZip) ? CloseZipU(hZip) : CloseZipZ(hZip);

	return result;
}

// ucclibrary.cpp:424
bool UnzipUccClothes(unsigned char* pData, unsigned long size, Bitmap* pTexture, Bitmap* pMask, Bitmap* pIcon, sUccHeader* pHeader) {
	ZIPENTRY ze;
	int index;
	bool result = true;

	HZIP hZip = OpenZipU(pData, size, 3);
	if (pHeader != NULL && FindZipItemA(hZip, "header", true, &index, &ze) == 0) UnzipItem(hZip, index, pHeader, ze.unc_size, 3);

	if (pTexture != NULL) {
		if (FindZipItemA(hZip, "front", true, &index, &ze) == 0) {
			if (pTexture->BitsPerPixel() * pTexture->Height() * pTexture->Width() / 8 != (unsigned int)ze.unc_size) {
				IsZipHandleU(hZip) ? CloseZipU(hZip) : CloseZipZ(hZip);
				return false;
			}

			UnzipItem(hZip, index, pTexture->vram, ze.unc_size, 3);
			result = result && true;
		} else {
			result = false;
		}
	}

	if (pMask != NULL) {
		if (FindZipItemA(hZip, "back", true, &index, &ze) == 0) {
			if (pMask->BitsPerPixel() * pMask->Height() * pMask->Width() / 8 != (unsigned int)ze.unc_size) {
				IsZipHandleU(hZip) ? CloseZipU(hZip) : CloseZipZ(hZip);
				return false;
			}

			UnzipItem(hZip, index, pMask->vram, ze.unc_size, 3);
			result = result && true;
		} else {
			result = false;
		}
	}

	if (pIcon != NULL) {
		if (FindZipItemA(hZip, "icon", true, &index, &ze) == 0) {
			if (pIcon->BitsPerPixel() * pIcon->Height() * pIcon->Width() / 8 != (unsigned int)ze.unc_size) {
				result = false;
			}

			UnzipItem(hZip, index, pIcon->vram, ze.unc_size, 3);
			result = result && true;
		} else {
			result = false;
		}
	}

	IsZipHandleU(hZip) ? CloseZipU(hZip) : CloseZipZ(hZip);

	return result;
}

// ucclibrary.cpp:516
bool LoadWarningClothes(unsigned int typeId, Bitmap* pTexture, Bitmap* pMask, Bitmap* pIcon) {
	if (typeId == 0 || pTexture == NULL || pMask == NULL || pIcon == NULL) {
		return false;
	}

	char filename[512] = { 0 };
	sprintf(filename, "data/ucc/warning/%d.jpg", typeId);

	cFile* file = GetCFile(filename, -1, 0);
	if (file == NULL) {
		return false;
	}

	int size = file->Length();
	unsigned char* pData = new unsigned char[size];
	file->Read(pData, size);

	if (pData == NULL) {
		delete[] pData;

		CloseCFile(file);
		return false;
	}

	CloseCFile(file);

	if (!UnzipUccClothes(pData, size, pTexture, pMask, pIcon, NULL)) {
		delete[] pData;
		return false;
	}

	delete[] pData;

	return true;
}

// ucclibrary.cpp:568
FrGraphicInterface* GetGDI(void) {
	if (g_pFresh == NULL) {
		return NULL;
	}

	FrWndManager* pManager = g_pFresh->GetManager();
	if (pManager == NULL) {
		return NULL;
	}

	return pManager->GetGDI();
}

// ucclibrary.cpp:579
void RefreshTexCache(Bitmap& bitmap) {
	FrGraphicInterface* pGDI = GetGDI();

	if (pGDI != NULL) {
		pGDI->RefreshTexCache(bitmap);
	}
}

// ucclibrary.cpp:589
void RefreshTexCache(Bitmap* bitmap) {
	if (bitmap != NULL) {
		RefreshTexCache(*bitmap);
	}
}

// ucclibrary.cpp:597
void ClearTexCache(Bitmap* bitmap) {
	FrGraphicInterface* pGDI = GetGDI();

	if (pGDI != NULL && bitmap != NULL) {
		pGDI->InvalidateCache(*bitmap);
	}
}

// ucclibrary.cpp:607
float Power(float x, float y) {
	return exp(log(x) * y);
}

// ucclibrary.cpp:612
std::string Lower(std::string text) {
	std::transform(text.begin(), text.end(), text.begin(), tolower);
	return text;
}

// ucclibrary.cpp:618
std::string FilterFilename(std::string filename) {
	std::string result;

	for (unsigned int i = 0; i < filename.size(); i++) {
		char c = filename[i];

		if (strchr("[]{}`~!@#$%^&()-+=\\|<>/?", c) != NULL) {
			continue;
		}

		result += c;
	}

	return result;
}

// ucclibrary.cpp:636
std::string GetMaskFilename(const char* filename) {
	std::string name(filename);

	const char* ext = strrchr(filename, '.');

	if (ext == NULL) {
		return filename;
	}

	name = name.substr(0, ext - filename);
	name += "_mask";
	name += ext;

	return Lower(name);
}

// ucclibrary.cpp:657
std::string GenerateTextureFullPath(int side, unsigned long typeId, const char* uccKey) {
	if (side == 0) {
		return MakeStr("%s\\%s", GetUccTempFoler(), GenerateTextureName_F(typeId, uccKey).c_str());
	}

	return MakeStr("%s\\%s", GetUccTempFoler(), GenerateTextureName_B(typeId, uccKey).c_str());
}

// ucclibrary.cpp:669
std::string GenerateIconFilename(unsigned long typeId, const char* uccKey) {
	if (!WSingleton<CSharedDoc>::IsInstantiated() || ItemManager() == NULL) {
		return "";
	}

	std::string name;

	IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(typeId);
	if (pPart == NULL) {
		return "";
	}

	name = FilterFilename(pPart->texture[0]);
	name = name.substr(0, name.rfind('.'));
	name += "_";
	name += uccKey;
	name += "_i.jpg";

	return Lower(name);
}

// ucclibrary.cpp:689
std::string GenerateTextureName_F(unsigned long typeId, const char* uccKey) {
	if (!WSingleton<CSharedDoc>::IsInstantiated() || ItemManager() == NULL) {
		return "";
	}

	std::string name;

	IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(typeId);
	if (pPart == NULL) {
		return "";
	}

	name = FilterFilename(pPart->texture[0]);
	name = name.substr(0, name.rfind('.'));
	name += "_";
	name += uccKey;
	name += ".jpg";

	return Lower(name);
}

// ucclibrary.cpp:708
std::string GenerateTextureName_B(unsigned long typeId, const char* uccKey) {
	if (!WSingleton<CSharedDoc>::IsInstantiated() || ItemManager() == NULL) {
		return "";
	}

	std::string name;

	IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(typeId);
	if (pPart == NULL) {
		return "";
	}

	name = FilterFilename(pPart->texture[0]);
	name = name.substr(0, name.rfind('.'));
	name += "_";
	name += uccKey;
	name += ".jpg";

	return Lower(name);
}

// ucclibrary.cpp:727
void Convert24to32(Bitmap* src, Bitmap* dest, const Bitmap* mask) {
	if (src == NULL || dest == NULL) {
		return;
	}

	for (int y = 0; y < src->Height(); y++) {
		for (int x = 0; x < src->Width(); x++) {
			dest->SetPixel(x, y, 255, 255, 255, 255);

			if (mask != NULL) {
				unsigned char r, g, b;

				mask->GetPixel(x, y, r, g, b);
				if (r < 5 && g < 5 && b < 5) {
					dest->SetPixel(x, y, 255, 255, 255, 0);
				}
			}
		}
	}
}

// ucclibrary.cpp:757
void Convert32to24(Bitmap* src, Bitmap* dest, const Bitmap* mask) {
	if (src == NULL || dest == NULL || mask == NULL) {
		return;
	}

	dest->Create(src->Width(), dest->Height(), 24);

	sRGBA color(0, 0, 0, 0);
	sRGBA brightness(0, 0, 0, 0);
	sRGBA result(0, 0, 0, 0);

	for (int y = 0; y < src->Height(); y++) {
		for (int x = 0; x < src->Width(); x++) {
			src->GetPixel(x, y, color.r, color.g, color.b);
			mask->GetPixel(x, y, brightness.r, brightness.g, brightness.b);

			SetBrightness(color, brightness, result);
			dest->SetPixel(x, y, result.r, result.g, result.b, 255);
		}
	}
}

// ucclibrary.cpp:783
void MixColor(sRGBA& back, sRGBA& front, sRGBA& result) {
	float backAlpha = back.a * (1.0f / 255.0f);
	float frontAlpha = front.a * (1.0f / 255.0f);

	result.r = (unsigned char)(back.r * (1.0f - frontAlpha) * backAlpha + front.r * frontAlpha);
	result.g = (unsigned char)(back.g * (1.0f - frontAlpha) * backAlpha + front.g * frontAlpha);
	result.b = (unsigned char)(back.b * (1.0f - frontAlpha) * backAlpha + front.b * frontAlpha);
	result.a = 255;
}

// ucclibrary.cpp:806
void SetBrightness(sRGBA& color, sRGBA& brightness, sRGBA& result) {
	float colorValue = (color.r + color.g + color.b) / 3.0f;

	float brightnessValue = (brightness.r + brightness.g + brightness.b) / 3.0f;
	float targetValue = brightnessValue * colorValue / 255.0f;
	float ratio = targetValue / colorValue;

	int value;
	value = (int)(color.r * ratio); result.r = (unsigned char)(value > 255 ? 255 : value);
	value = (int)(color.g * ratio); result.g = (unsigned char)(value > 255 ? 255 : value);
	value = (int)(color.b * ratio); result.b = (unsigned char)(value > 255 ? 255 : value);
	result.a = color.a;
}

// ucclibrary.cpp:821
void GetResizedImage(const Bitmap* pSrc, const Bitmap* pMask, Bitmap* pDest, int width, int height) {
	if (pSrc == NULL || pDest == NULL) {
		return;
	}

	float scaleX = (float)pSrc->Width() / width;
	float scaleY = (float)pSrc->Height() / height;
	bool ignoreAlpha = pMask == NULL && pSrc->BitsPerPixel() == 32;

	pDest->Create(width, height, 32);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int left = (int)floor(x * scaleX);
			int top = (int)floor(y * scaleY);
			int right = (int)ceil((x + 1) * scaleX - 1.0f);
			int bottom = (int)ceil((y + 1) * scaleY - 1.0f);

			if (pMask != NULL && NoDrawArea(pMask, left, top, right, bottom)) {
				pDest->SetPixel(x, y, 0, 0, 0, 0);
				continue;
			}

			sRGBA color = GetAverageColor(pSrc, left, top, right, bottom, ignoreAlpha);
			pDest->SetPixel(x, y, color.r, color.g, color.b, color.a);
		}
	}
}

// ucclibrary.cpp:860
sRGBA GetAverageColor(const Bitmap* pBitmap, int left, int top, int right, int bottom, bool ignoreAlpha) {
	unsigned int r = 0, g = 0, b = 0, a = 0;
	unsigned int weightSum = 0;
	unsigned int alphaCount = 0;

	for (int y = top; y <= bottom; y++) {
		if ((unsigned int)y >= pBitmap->Height()) {
			break;
		}

		for (int x = left; x <= right; x++) {
			if ((unsigned int)x >= pBitmap->Width()) {
				break;
			}

			unsigned char pr, pg, pb, pa;

			if (pBitmap->BitsPerPixel() == 32) {
				pBitmap->GetPixel(x, y, pr, pg, pb, pa);
			} else {
				pa = 0xFF;
				pBitmap->GetPixel(x, y, pr, pg, pb);
			}

			sRGBA color(pr, pg, pb, pa);
			unsigned char value;
			if (ignoreAlpha) {
				value = color.a;
			} else {
				value = color.toHSV().v;
			}

			unsigned char weight = (255 - value) / 20 + 1;
			r += pr * weight;
			g += pg * weight;
			b += pb * weight;
			a += pa;

			weightSum += weight;
			if (pa > 0) {
				alphaCount++;
			}
		}
	}

	if (weightSum != 0) {
		r /= weightSum;
		g /= weightSum;
		b /= weightSum;
	}

	if (alphaCount != 0) {
		a /= alphaCount;
	}

	return sRGBA(r, g, b, a);
}

// ucclibrary.cpp:929
bool NoDrawArea(const Bitmap* pBitmap, int left, int top, int right, int bottom) {
	for (int y = top; y <= bottom; y++) {
		for (int x = left; x <= right; x++) {
			unsigned char r, g, b;

			pBitmap->GetPixel(x, y, r, g, b);

			if (r <= 0x10 && g <= 0x10 && b <= 0x10) {
				return true;
			}
		}
	}

	return false;
}

// ucclibrary.cpp:949
sRGBA GetSimilarColor(sRGBA color, sRGBA first, sRGBA second) {
	sRGBA c = color, f = first, s = second;

	c.RemoveAlpha();
	f.RemoveAlpha();
	s.RemoveAlpha();

	sHSV hsv, firstHSV, secondHSV;
	hsv = c.toHSV();
	firstHSV = f.toHSV();
	secondHSV = s.toHSV();

	int firstDiff = abs(hsv.h - firstHSV.h) > abs(firstHSV.h - hsv.h + 360) ? abs(firstHSV.h - hsv.h + 360) : abs(hsv.h - firstHSV.h);
	int secondDiff = abs(hsv.h - secondHSV.h) > abs(secondHSV.h - hsv.h + 360) ? abs(secondHSV.h - hsv.h + 360) : abs(hsv.h - secondHSV.h);

	if (firstDiff < secondDiff) {
		return first;
	}
	if (firstDiff > secondDiff) {
		return second;
	}

	if (abs(hsv.v - firstHSV.v) < abs(hsv.v - secondHSV.v)) {
		return first;
	}

	return second;
}

// ucclibrary.cpp:981
void RGBToHLS(unsigned char r, unsigned char g, unsigned char b, unsigned char& h, unsigned char& l, unsigned char& s) {
	WORD hue;
	WORD luminance;
	WORD saturation;

	ColorRGBToHLS(RGB(r, g, b), &hue, &luminance, &saturation);
	h = (unsigned char)hue;
	l = (unsigned char)luminance;
	s = (unsigned char)saturation;
}

// ucclibrary.cpp:1035
void HLSToRGB(unsigned char h, unsigned char l, unsigned char s, unsigned char& r, unsigned char& g, unsigned char& b) {
	COLORREF color = ColorHLSToRGB(h, l, s);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);
}

// ucclibrary.cpp:1069
unsigned char HueToRGB(unsigned char n1, unsigned char n2, unsigned char hue) {
	if (hue > 240) {
		hue -= 240;
	}

	if (hue < 40) {
		return n1 + ((n2 - n1) * hue + 20) / 40;
	}
	if (hue < 120) {
		return n2;
	}
	if (hue < 160) {
		return n1 + ((n2 - n1) * (160 - hue) + 20) / 40;
	}

	return n1;
}

// ucclibrary.cpp:1089
void GetUccInfoFromPacket(WReceivedPacket* pPacket) {
	if (pPacket == NULL) {
		return;
	}

	int count = pPacket->Decode1();

	for (int i = 0; i < count; i++) {
		unsigned long serial = pPacket->Decode4();
		unsigned long typeId = pPacket->Decode4();

		char uccKey[9];
		memset(uccKey, 0, sizeof(uccKey));
		strncpy(uccKey, pPacket->DecodeStr().c_str(), sizeof(uccKey));

		UccManager()->AddClothes(0, serial, typeId, uccKey, false, false);
	}
}

// ucclibrary.cpp:1109
void RequestUccInfo(sUserInfoTime& info) {
	RequestUccInfo(info.m_userInfo.m_characterInfo);
}

// ucclibrary.cpp:1114
void RequestUccInfo(sUserInfo& info) {
	RequestUccInfo(info.m_characterInfo);
}

// ucclibrary.cpp:1119
void RequestUccInfo(sRoomSlot& slot) {
	RequestUccInfo(slot.m_characterInfo);
}

// ucclibrary.cpp:1124
void RequestUccInfo(ranktype::sRankUserInfo& info) {
	RequestUccInfo(info.m_characterInfo);
}

// ucclibrary.cpp:1129
void RequestUccInfo(sCharacterInfo& info) {
	for (int i = 0; i < 24; i++) {
		unsigned long typeId = info.partTypeIds[i];
		unsigned long serial = info.partIds[i];

		if (typeId != 0 && IsUccClothes(typeId) && UccManager()->FindClothes(serial) == NULL) {
			WSendPacket packet((enumClientPacket)0xB1);
			packet.Encode1(1);
			packet.Encode4(serial);
			packet.Encode1(0);
			packet.Send(SEND_TO_DEFAULT);
		}
	}
}

// ucclibrary.cpp:1148
void RequestUccInfo(sMailIncludeItem& item) {
	if (IsUccClothes(item.typeId)) {
		UccManager()->AddClothes(0, 0, item.typeId, item.uccKey, false, false);
	}
}

// ucclibrary.cpp:1161
sHSV sRGB::toHSV(void) {
	sHSV hsv;

	double minValue = Min(r, g, b);
	double maxValue = Max(r, g, b);
	double delta = maxValue - minValue;

	hsv.v = (unsigned char)maxValue;
	if (delta == 0.0) {
		hsv.s = 0;
		hsv.h = 0;
	} else {
		double saturation = delta / maxValue;
		hsv.s = (unsigned char)(saturation * 255.0);

		double hue;
		if (r == (int)maxValue) {
			hue = (g - b) / delta;
		} else if (g == (int)maxValue) {
			hue = (b - r) / delta + 2.0;
		} else {
			hue = (r - g) / delta + 4.0;
		}

		hue *= 60.0;
		if (hue < 0.0) {
			hue += 360.0;
		}

		if (hue == 360.0) {
			hue = 0.0;
		}

		hsv.h = (unsigned char)hue;
	}

	return hsv;
}

// ucclibrary.cpp:1207
sRGB sHSV::toRGB(void) {
	sRGB rgb;

	double value = v;
	double chroma = s * value / 255.0;
	double minValue = value - chroma;

	double hue = h;
	if (h > 300 || h <= 60) {
		rgb.r = (unsigned char)value;

		if (h > 300) {
			rgb.g = (unsigned char)minValue;
			rgb.b = (unsigned char)(-((hue - 360.0) / 60.0 * chroma - minValue));
		} else {
			rgb.b = (unsigned char)minValue;
			rgb.g = (unsigned char)(hue / 60.0 * chroma + minValue);
		}
	} else if (h < 180) {
		rgb.g = (unsigned char)value;

		double delta = (hue / 60.0 - 2.0) * chroma;
		if (h < 120) {
			rgb.b = (unsigned char)minValue;
			rgb.r = (unsigned char)(minValue - delta);
		} else {
			rgb.r = (unsigned char)minValue;
			rgb.b = (unsigned char)(delta + minValue);
		}
	} else {
		rgb.b = (unsigned char)value;

		double delta = (hue / 60.0 - 4.0) * chroma;
		if (h < 240) {
			rgb.r = (unsigned char)minValue;
			rgb.g = (unsigned char)(minValue - delta);
		} else {
			rgb.g = (unsigned char)minValue;
			rgb.r = (unsigned char)(delta + minValue);
		}
	}

	return rgb;
}

// ucclibrary.cpp:1273
void sRGBA::RemoveAlpha(void) {
	unsigned char alpha = a;
	if (alpha != 255) {
		sHSV hsv = toHSV();
		hsv.s = (unsigned char)((float)alpha * hsv.s * (1.0f / 255.0f));
		sRGB rgb = hsv.toRGB();

		r = rgb.r; g = rgb.g;
		b = rgb.b;
		a = 255;
	}
}
