// Original: d:\build\custom\temp\code\client\wangreal\include\wresrcmng.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WRESRCMNG_H
#define PG_CLIENT_WANGREAL_INCLUDE_WRESRCMNG_H

#include <stddef.h>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wlist.h"
#include "wlock.h"
#include "wresrccache.h"

class Bitmap;
class cFile;
class W3dAniSpr;
class W3dSpr;
class WAudioDev;
class WFont;
class WOverlay;
class WPuppet;
class WResource;
class WSoundFx;
class WTitleFont;
class WVideoDev;
class WView;
class WxStaticPuppetGrp;

// A texture of the cache; texname runs past the end of the record.
struct w_texlist {
	int width;              // 0x0
	int height;             // 0x4
	int count;              // 0x8
	int texhandle;          // 0xc
	unsigned long size;     // 0x10
	char texname[1];        // 0x14
};
CHECK_SIZE(w_texlist, 0x18);
CHECK_FIELD(w_texlist, width, 0x0);
CHECK_FIELD(w_texlist, height, 0x4);
CHECK_FIELD(w_texlist, count, 0x8);
CHECK_FIELD(w_texlist, texhandle, 0xc);
CHECK_FIELD(w_texlist, size, 0x10);
CHECK_FIELD(w_texlist, texname, 0x14);

enum rmlType_t {
	RML_TEXTURE_LOADING = 0,
	RML_BITMAP_LOADING = 1,
	RML_PUPPET_LOADING = 2,
	RML_COUNT = 3
};

// Wangreal's resource cache (PDB layout, size 0x1d4). Texture handles are
// plain ints (0 = none).
class WResourceManager {
	LAYOUT_CHECK_FRIEND;
public:
	struct w_match_filename {
		char fullname[1];   // 0x0
	};

	WResourceManager(void);
	~WResourceManager(void);

	void SetVideoReference(WVideoDev* v) { video = v; }
	WVideoDev* VideoReference(void) const { return video; }

	void Lock(rmlType_t type);
	void Unlock(rmlType_t type);
	void UnlockAllByThread(unsigned long threadID);
	void SetUseLessVideoRam(int use);

	WOverlay* GetOverlay(const char* name, Bitmap* bitmap, unsigned long type);
	WOverlay* GetOverlay(const char* name, unsigned long type);
	WFont* GetTexFont(const char* name);
	WFont* GetTrueFont(const char* name, int size, int weight);
	WFont* GetFntFont(const char* name);
	WFont* GetFixedFont(const char* name);
	WTitleFont* GetTitleFont(void);
	WView* GetView(const char* name);
	WPuppet* GetPuppet(const char* name, bool a, bool b, bool c);
	WxStaticPuppetGrp* xGetStaticPuppetGrp(int index, const char* name, WPuppet** puppet);
	W3dSpr* Get3DSpr(const char* name, int type);
	W3dAniSpr* Get3DAniSpr(const char* name, int type, float a, float b);

	void Release(WTitleFont* font);
	void Release(WFont* font);
	void Release(int texhandle);
	void Release(WPuppet* puppet, bool a);
	void Release(WResource* resource);
	void ReleaseAllPets(void);
	void ReleaseIdleResources(int a);

	const WList<w_texlist*>& GetTextureList(void) const { return texList; }
	w_texlist* FindTexture(const char* texname);
	w_texlist* FindTexture(int texhandle);
	int GetTextureWidth(int texhandle);
	int GetTextureHeight(int texhandle);
	int UploadTexture(const char* texname, Bitmap* bitmap, unsigned long type, tagRECT* rect);
	void FixTexture(int texhandle, Bitmap* bitmap, unsigned long type, tagRECT* rect);
	bool CheckTexture(const char* texname) { return FindTexture(texname) ? true : false; }
	int LoadTexture(Bitmap* bitmap, const char* texname, int a, int b);
	int LoadTexture(const char* filename, unsigned long type, int level, const char* texname);
	int LoadTexture_DDS(unsigned char* buff, unsigned int size, const char* texname, int a, int b);
	void ChangeTexturePart(int texhandle, Bitmap* bitmap, const tagRECT& rect);
	bool ChangeTexturePart(int texhandle, char* filename, Bitmap** bitmap, tagRECT* rect);
	void ChangeTexturePart(char* texname, Bitmap* bitmap, const tagRECT& rect);
	bool ChangeTexturePart(char* texname, char* filename, Bitmap** bitmap, tagRECT* rect);
	int GetBlankTexture(void);
	void ReloadTextures(void);
	bool CheckMaskTexture(Bitmap** bitmap, const char* filename);
	Bitmap* LoadBitmap(const char* filename, int a, bool b);   // LoadBitmapA once windows.h renames it
	Bitmap* LoadBitmapPart(char* filename, char* part);
	Bitmap* LoadBMP(const char* filename);
	Bitmap* LoadJPG(const char* filename);
	Bitmap* LoadTGA(const char* filename);
	Bitmap* LoadPNG(const char* filename, bool a, bool b);
	Bitmap* LoadBRES(const char* filename, const char* name);
	Bitmap* MakeQuarterBitmap(Bitmap* bitmap);
	Bitmap* Combine4Alpha(Bitmap* a, Bitmap* b);
	Bitmap* Combine(Bitmap* a, Bitmap* b);
	bool IsOrigin(WResource* resource);
	cFile* GetCFile(const char* filename, int a);
	bool FindFileInZip(const char* zipname, const char* filename);
	void SetAutoMatchDirectory(const char* dir);
	const char* FindMatchFile(const char* filename);

protected:
	WVideoDev* video;                               // 0x0
	WAudioDev* audio;                               // 0x4

private:
	w_texlist* AddTexture(const char* texname, int a, int b, int c);
	void AddMissingTexture(const char* texname);
	bool GetRectInSourceTexture(const tagBITMAPINFO* info, const void* bits, tagRECT& rect) const;
	WFont* GetFntClone(const char* name);
	void AddMatchList(const char* filename);
	void UpdateMatchList(const char* filename);
	void StrcpyLower(char* dst, const char* src);
	void LoadMatchList(const char* filename);
	void SaveMatchCacheList(const char* filename);
	void LoadMatchCacheList(const char* filename);
	void ReleaseMatchList(void);
	void AddOriginList(WResource* resource);

	unsigned long m_nByteUsedTexture;               // 0x8
	int m_blankTexture;                             // 0xc
	bool m_savemem[3];                              // 0x10
	bool m_matchDirectory;                          // 0x13
	WList<char*> m_missingTextureList;              // 0x14
	WList<w_match_filename*> m_matchList;           // 0x34
	WList<w_texlist*> texList;                      // 0x54
	WResrcCache<WPuppet*> puppetList;               // 0x74
	WResrcCache<WSoundFx*> soundList;               // 0xb4
	WResrcCache<WFont*> fontList;                   // 0xf4
	WList<WSoundFx*> voiceList;                     // 0x134
	WList<WResource*> originList;                   // 0x154
	WLock m_lock[3];                                // 0x174
};
CHECK_SIZE(WResourceManager, 0x1d4);
CHECK_SIZE(WResrcCache<WFont*>, 0x40);
BEGIN_LAYOUT_CHECK(WResourceManager)
	CHECK_FIELD(WResourceManager, video, 0x0);
	CHECK_FIELD(WResourceManager, audio, 0x4);
	CHECK_FIELD(WResourceManager, m_nByteUsedTexture, 0x8);
	CHECK_FIELD(WResourceManager, m_blankTexture, 0xc);
	CHECK_FIELD(WResourceManager, m_savemem, 0x10);
	CHECK_FIELD(WResourceManager, m_matchDirectory, 0x13);
	CHECK_FIELD(WResourceManager, m_missingTextureList, 0x14);
	CHECK_FIELD(WResourceManager, m_matchList, 0x34);
	CHECK_FIELD(WResourceManager, texList, 0x54);
	CHECK_FIELD(WResourceManager, puppetList, 0x74);
	CHECK_FIELD(WResourceManager, soundList, 0xb4);
	CHECK_FIELD(WResourceManager, fontList, 0xf4);
	CHECK_FIELD(WResourceManager, voiceList, 0x134);
	CHECK_FIELD(WResourceManager, originList, 0x154);
	CHECK_FIELD(WResourceManager, m_lock, 0x174);
END_LAYOUT_CHECK

extern WResourceManager* g_resrcmng;

#endif // PG_CLIENT_WANGREAL_INCLUDE_WRESRCMNG_H
