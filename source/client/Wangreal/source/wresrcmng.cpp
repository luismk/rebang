// Original: d:\build\custom\temp\code\client\wangreal\source\wresrcmng.cpp
#include "..\..\Fresh\third_party\libpng-1.2.8\png.h"

#include "../include/wresrcmng.h"
#include "../include/bitmap.h"
#include "../include/w3danispr.h"
#include "../include/w3dspr.h"
#include "../include/westpak.h"
#include "../include/wfont.h"
#include "../include/wmemblock.h"
#include "../include/woverlay.h"
#include "../include/wresource.h"
#include "../include/wvideo.h"
#include "../include/wview.h"
#include "../include/wxpuppet.h"
#include "XUnzip.h"
#include "freetype-2.2.1/textout.h"

// westpak.cpp; the original declares it in a Wangreal header.
cFile* __cdecl GetCFile(const char* filename, int len, int pakonly);
void __cdecl EnumFileList(WList<char*>* list);

// The original declares WInstanceLock in wlock.h (line marks wlock.h:21-29);
// kept here until that header can be rebuilt with its includers.
class WInstanceLock {
public:
	// wlock.h:21
	WInstanceLock(WLock* lock) {
		m_lock = lock;
		m_lock->Lock();
	}
	// wlock.h:28
	~WInstanceLock(void) { m_lock->Unlock(); }

private:
	WLock* m_lock;  // 0x0
};
CHECK_SIZE(WInstanceLock, 0x4);

#define INSTANCE_LOCK(type) WInstanceLock __instanceLock##type(&m_lock[type])

// Intel JPEG Library 1.5 (ijl.h, not available: proprietary): the part
// of JPEG_CORE_PROPERTIES the decoder fills (PDB layout), and its entry points.
enum IJLERR { IJL_OK = 0 };
enum IJLIOTYPE { IJL_JBUFF_READPARAMS = 1, IJL_JBUFF_READWHOLEIMAGE = 3 };
enum IJL_COLOR { IJL_RGB = 1, IJL_BGR = 2, IJL_YCBCR = 3, IJL_G = 4, IJL_OTHER = 255 };
enum IJL_JPGSUBSAMPLING { IJL_NONE = 0 };

struct JPEG_CORE_PROPERTIES {
	int UseJPEGPROPERTIES;                 // 0x0
	unsigned char* DIBBytes;               // 0x4
	int DIBWidth;                          // 0x8
	int DIBHeight;                         // 0xc
	int DIBPadBytes;                       // 0x10
	int DIBChannels;                       // 0x14
	IJL_COLOR DIBColor;                    // 0x18
	IJL_JPGSUBSAMPLING DIBSubsampling;     // 0x1c
	const char* JPGFile;                   // 0x20
	unsigned char* JPGBytes;               // 0x24
	int JPGSizeBytes;                      // 0x28
	int JPGWidth;                          // 0x2c
	int JPGHeight;                         // 0x30
	int JPGChannels;                       // 0x34
	IJL_COLOR JPGColor;                    // 0x38
	IJL_JPGSUBSAMPLING JPGSubsampling;     // 0x3c
	int JPGThumbWidth;                     // 0x40
	int JPGThumbHeight;                    // 0x44
	int cconversion_reqd;                  // 0x48
	int upsampling_reqd;                   // 0x4c
	int jquality;                          // 0x50
	PADDING(0x54, 0x4e68);                 // JPEG_PROPERTIES jprops (0x58)
};
CHECK_SIZE(JPEG_CORE_PROPERTIES, 0x4e68);
CHECK_FIELD(JPEG_CORE_PROPERTIES, JPGBytes, 0x24);
CHECK_FIELD(JPEG_CORE_PROPERTIES, JPGColor, 0x38);
CHECK_FIELD(JPEG_CORE_PROPERTIES, jquality, 0x50);

extern "C" IJLERR __stdcall ijlInit(JPEG_CORE_PROPERTIES* jcprops);
extern "C" IJLERR __stdcall ijlFree(JPEG_CORE_PROPERTIES* jcprops);
extern "C" IJLERR __stdcall ijlRead(JPEG_CORE_PROPERTIES* jcprops, IJLIOTYPE iotype);

// The original declares WViewOrth in wview.h; kept here until that header
// can be rebuilt with its includers.
class WViewOrth : public WView {
public:
	virtual ~WViewOrth(void) {}
};
CHECK_SIZE(WViewOrth, sizeof(WView));

// A JPEG kept compressed in memory, decoded on Update.
class JpegBitmap : public Bitmap {
public:
	JpegBitmap(char* data, int len);
	virtual ~JpegBitmap(void);
	bool DecodeHeader(void);
	virtual void Update(void);

protected:
	bool DecodeImage(void);

private:
	char* m_data;   // 0x14
	int m_size;     // 0x18
};
CHECK_SIZE(JpegBitmap, 0x1c);

// The 18-byte header of a TGA file (PDB layout).
#pragma pack(push, 1)
struct sTgaHeader {
	unsigned char id_size;              // 0x0
	unsigned char color_map_type;       // 0x1
	unsigned char iType;                // 0x2
	unsigned short color_map_origin;    // 0x3
	unsigned short color_map_length;    // 0x5
	unsigned char color_map_size;       // 0x7
	short origin_x;                     // 0x8
	short origin_y;                     // 0xa
	short width;                        // 0xc
	short height;                       // 0xe
	unsigned char bitcount;             // 0x10
	unsigned char descript;             // 0x11
};
#pragma pack(pop)
CHECK_SIZE(sTgaHeader, 0x12);
CHECK_FIELD(sTgaHeader, iType, 0x2);
CHECK_FIELD(sTgaHeader, width, 0xc);
CHECK_FIELD(sTgaHeader, height, 0xe);
CHECK_FIELD(sTgaHeader, bitcount, 0x10);

// The 14-byte header of a BMP file (PDB layout).
#pragma pack(push, 1)
struct bmp_head {
	char signature[2];  // 0x0
	long size;          // 0x2
	short reserved1;    // 0x6
	short reserved2;    // 0x8
	long offbits;       // 0xa
};
#pragma pack(pop)
CHECK_SIZE(bmp_head, 0xe);
CHECK_FIELD(bmp_head, offbits, 0xa);

// WResrcCache members the original defines in wresrccache.h (line marks);
// kept here until that header can be rebuilt with its ~280 includers.
// wresrccache.h:63
template <class T>
T WResrcCache<T>::FirstOrigin(void) {
	w_origin* origin = originList.Start();
	if (origin)
		return origin->resource;
	return NULL;
}

// wresrccache.h:73
template <class T>
T WResrcCache<T>::NextOrigin(void) {
	w_origin* origin = originList.Next();
	if (origin)
		return origin->resource;
	return NULL;
}

// wresrccache.h:118
template <class T>
T WResrcCache<T>::FirstResource(void) {
	w_cache* cache = resrcList.Start();
	if (cache)
		return cache->resource;
	return NULL;
}

// wresrccache.h:128
template <class T>
T WResrcCache<T>::NextResource(void) {
	w_cache* cache = resrcList.Next();
	if (cache)
		return cache->resource;
	return NULL;
}

// wresrccache.h:218
template <class T>
char* WResrcCache<T>::FindIdleOrigin(char* first, int count) {
	for (w_origin* origin = originList.Start(); origin; origin = originList.Next()) {
		if (origin->count == 0) {
			if (first) {
				if (strcmp(origin->name, first) == 0)
					first = NULL;
			} else {
				if (origin->checkflag >= count)
					return origin->name;
				origin->checkflag++;
			}
		}
	}
	return NULL;
}

// wresrccache.h:254
template <class T>
bool WResrcCache<T>::AddResource(const char* name, T resource) {
	w_origin* origin = originList.Find(name);
	if (!origin) {
		AddOrigin(name, resource);
		origin = originList.Find(name);
	}
	w_cache* cache = (w_cache*)g_mem.Alloc(sizeof(w_cache));
	cache->origin = origin;
	cache->resource = resource;
	origin->count++;
	origin->checkflag = 0;
	resrcList.AddItem(cache, origin->name, false);
	if (origin->count > 1)
		return false;
	return true;
}

// wresrccache.h:286
template <class T>
bool WResrcCache<T>::DeleteResource(T resource) {
	for (w_cache* cache = resrcList.Start(); cache; cache = resrcList.Next()) {
		if (cache->resource == resource) {
			w_origin* origin = cache->origin;
			resrcList -= cache;
			g_mem.Free(cache);
			origin->count--;
			if (origin->resource != resource)
				return true;
			return false;
		}
	}
	return false;
}

// wresrccache.h:316
template <class T>
void WResrcCache<T>::AddOrigin(const char* name, T resource) {
	w_origin* origin = (w_origin*)g_mem.Alloc((int)strlen(name) + sizeof(w_origin));
	strcpy(origin->name, name);
	origin->resource = resource;
	origin->count = 0;
	origin->checkflag = 0;
	originList.AddItem(origin, origin->name, false);
}

// wresrcmng.cpp:49
WResourceManager::WResourceManager(void)
	: m_missingTextureList(8, 8), m_matchList(1024, 1024), texList(64, 64), voiceList(8, 0), originList(8, 0) {



	video = NULL; audio = NULL; remove("files_of_the_same_name.txt");

	m_matchDirectory = false;
	m_blankTexture = 0;
	m_nByteUsedTexture = 0;


	for (int i = 0; i < 3; i++) {
		m_savemem[i] = false;
	}
}

// wresrcmng.cpp:64
WResourceManager::~WResourceManager(void) {
	if (m_blankTexture) {
		video->DestroyTexture(m_blankTexture);
	}

	for (WFont* font = fontList.FirstResource(); font; font = fontList.NextResource()) {
		if (fontList.DeleteResource(font) == true) {
			delete font;
		}
	}

	for (WPuppet* puppet = puppetList.FirstResource(); puppet; puppet = puppetList.NextResource()) {
		if (puppetList.DeleteResource(puppet) == true) {
			delete puppet;
		}
	}

	for (WResource* resrc = originList.Start(); resrc; resrc = originList.Next()) {
		delete resrc;
	}

	do {
		w_texlist* tex = texList.Start();
		if (!tex) {
			break;
		}

		Release(tex->texhandle);
	} while (1);

	ReleaseMatchList();

	for (char* text = m_missingTextureList.Start(); text; text = m_missingTextureList.Next()) {
		if (text) {
			delete[] text;
		}
	}
	m_missingTextureList.Reset();
}

// wresrcmng.cpp:112
void WResourceManager::Lock(rmlType_t type) {
	m_lock[type].Lock();
}

// wresrcmng.cpp:119
void WResourceManager::Unlock(rmlType_t type) {
	m_lock[type].Unlock();
}

// wresrcmng.cpp:126
void WResourceManager::UnlockAllByThread(unsigned long threadID) {
	for (int i = 0; i < RML_COUNT; i++) {
		m_lock[i].UnlockInThread(threadID);
	}
}

// wresrcmng.cpp:136
void WResourceManager::SetUseLessVideoRam(int level) {
	m_savemem[0] = false;
	m_savemem[1] = level > 0;
	m_savemem[2] = level > 1;
}

// wresrcmng.cpp:147
void WResourceManager::Release(WResource* resrc) {
	if (resrc) {
		delete resrc;
	}
}

// wresrcmng.cpp:153
void WResourceManager::AddOriginList(WResource* resrc) {
	originList += resrc;
}

// wresrcmng.cpp:158
bool WResourceManager::IsOrigin(WResource* resrc) {
	for (WResource* origin = originList.Start(); origin; origin = originList.Next())
		if (origin == resrc)
			return true;
	return false;
}

// wresrcmng.cpp:168
template <class T>
static void ReleaseIdleOrigin(WResrcCache<T>* list, WList<WResource*>& originList, int cnt) {
	char next[128];

	next[0] = 0;
	for (char* name = list->FindIdleOrigin(NULL, cnt); name; name = list->FindIdleOrigin(next[0] ? next : NULL, cnt)) {
		strcpy(next, name);

		T resource = list->FindOrigin(name);
		list->ClearOrigin(name);
		originList -= resource;
		if (resource) delete resource;
	}
}

// wresrcmng.cpp:185
void WResourceManager::ReleaseIdleResources(int cnt) {
	ReleaseIdleOrigin(&puppetList, originList, cnt);

	ReleaseIdleOrigin(&fontList, originList, cnt);
}


// wresrcmng.cpp:198
WView* WResourceManager::GetView(const char* type) {
	WView* view;

	if (type != NULL && strstr(type, "rth")) {
		view = new WViewOrth;
	} else {
		view = new WView;
	}

	view->SetResourceManager(this);
	view->SetViewport((float)video->GetWidth(), (float)video->GetHeight());

	return view;
}
// wresrcmng.cpp:219
static void ExtractFilename(char* out, const char* filename) {
	strcpy(out, filename);
	char* p;
	while ((p = strchr(out, '\\')) != NULL) {
		*p = '/';
	}
}

// wresrcmng.cpp:227
WPuppet* WResourceManager::GetPuppet(const char* f, bool alphatest, bool bNeedTnLBuf, bool bForceLegacy) {
	char filename[260];
	char name[260];
	WPuppet* origin = NULL;
	int type;

	INSTANCE_LOCK(RML_PUPPET_LOADING);

	ExtractFilename(filename, f);
	strcpy(name, strrchr(filename, '/') ? strrchr(filename, '/') + 1 : filename);

	origin = puppetList.FindOrigin(name);

	char* ext = strrchr(name, '.');
	if (!strcmpi(ext, ".pet")) {
		type = 0xff;
	} else if (!strcmpi(ext, ".mpet")) {
		type = 0x11b;
	} else if (!strcmpi(ext, ".apet")) {
		type = 0x66;
	} else if (!strcmpi(ext, ".bpet")) {
		type = 0x82;
	} else {
		return NULL;
	}

	if (origin) {
		if (type == 0x66 || type == 0x11b) {
			puppetList.AddResource(name, origin);
			return origin;
		}

		WPuppet* puppet = origin->MakeClone(bNeedTnLBuf);
		puppet->SetResourceManager(this);
		puppet->xBuildTransfMatPtrList();
		puppet->SetAlpha(0xff);
		puppetList.AddResource(name, puppet);
		return puppet;
	}

	unsigned long dwCaps = 0;
	// The video device writes the caps through the pointer passed as lParam.
	video->Command(WX_VDEV_GET_CAPS, 1, (intptr_t)&dwCaps);

	WPuppet* puppet;
	if (bForceLegacy || !dwCaps || type == 0x66 || type == 0x11b) {
		puppet = new WPuppet;
	} else {
		puppet = new WxPuppet(bNeedTnLBuf);
	}

	puppet->SetResourceManager(this);
	if (!puppet->LoadPET(filename, alphatest, type)) {
		puppet->SetAlpha(0xff);
		puppetList.AddResource(name, puppet);
		AddOriginList(puppet);
		return puppet;
	}

	delete puppet;
	return NULL;
}

// wresrcmng.cpp:342
void WResourceManager::Release(WPuppet* puppet, bool bForceDeleteOrigin) {
	if (!puppet)
		return;

	if (puppetList.DeleteResource(puppet) == true) {
		delete puppet;
		return;
	}

	if (bForceDeleteOrigin == false)
		return;

	originList -= puppet;
	puppetList.ClearOrigin(puppet->GetPuppetName());

	delete puppet;
}

// wresrcmng.cpp:360
void WResourceManager::ReleaseAllPets(void) {
	WPuppet* puppet;
	for (puppet = puppetList.FirstResource(); puppet; puppet = puppetList.NextResource()) {
		if (puppetList.DeleteResource(puppet) == true)
			delete puppet;
	}

	for (puppet = puppetList.FirstOrigin(); puppet; puppet = puppetList.NextOrigin()) {
		originList -= puppet;
		puppetList.ClearOrigin(puppet->GetPuppetName());
		delete puppet;
	}
}

// wresrcmng.cpp:691
WOverlay* WResourceManager::GetOverlay(const char* filename, unsigned long flag) {
	WOverlay* overlay = new WOverlay;
	overlay->SetResourceManager(this);

	if (filename) {
		if (overlay->Load(filename, flag)) {
			delete overlay;
			return NULL;
		}
	}

	return overlay;
}

// wresrcmng.cpp:709
WOverlay* WResourceManager::GetOverlay(const char* name, Bitmap* bitmap, unsigned long flag) {
	WOverlay* overlay = new WOverlay;
	overlay->SetResourceManager(this);

	if (name) {
		if (overlay->Load(name, bitmap, flag)) {
			delete overlay;
			return NULL;
		}
	}

	return overlay;
}

// wresrcmng.cpp:731
WTitleFont* WResourceManager::GetTitleFont(void) {
	WTitleFont* font = new WTitleFont;
	font->SetResourceManager(this);

	return font;
}

// wresrcmng.cpp:739
void WResourceManager::Release(WTitleFont* font) {
	delete font;
}

// wresrcmng.cpp:747
WFont* WResourceManager::GetTrueFont(const char* filename, int fontsize, int fontindex) {
	WFont* font = GetFntClone(filename);
	if (font)
		return font;

	CTextOut* textout = new CTextOut(NULL);
	textout->SetResourceManager(this);
	if (textout->Load(filename, fontsize, fontindex, true) == true) {
		if (fontList.AddResource(filename, textout) == true)
			AddOriginList(textout);
		return textout;
	}

	return NULL;
}

// wresrcmng.cpp:771
WFont* WResourceManager::GetFntClone(const char* filename) {
	WFont* origin = fontList.FindOrigin(filename);

	if (origin) {
		WFont* font = origin->MakeClone();

		if (font) {
			font->SetResourceManager(this); fontList.AddResource(filename, font);
			return font;
		}
	}

	return NULL;
}

// wresrcmng.cpp:792
WFont* WResourceManager::GetTexFont(const char* filename) {
	WFont* font = GetFntClone(filename);
	if (!font) {
		WTexFont* texfont = new WTexFont(NULL);
		texfont->SetResourceManager(this);
		texfont->Load(filename, 0);
		if (fontList.AddResource(filename, texfont) == true)
			AddOriginList(texfont);
		font = texfont;
	}
	return font;
}

// wresrcmng.cpp:814
WFont* WResourceManager::GetFntFont(const char* filename) {
	WFont* font = GetFntClone(filename);
	if (font)
		return font;

	WFntFont* fntfont = new WFntFont(NULL);
	fntfont->SetResourceManager(this);
	if (fntfont->Load(filename)) {
		delete fntfont;
		return NULL;
	}
	if (fontList.AddResource(filename, fntfont) == true)
		AddOriginList(fntfont);
	return fntfont;
}

// wresrcmng.cpp:842
WFont* WResourceManager::GetFixedFont(const char* filename) {
	WFont* font = GetFntClone(filename);
	if (font)
		return font;

	WFixedFont* fixedfont = new WFixedFont(8);
	fixedfont->SetResourceManager(this);
	if (fixedfont->Load(filename)) {
		delete fixedfont;
		return NULL;
	}
	if (fontList.AddResource(filename, fixedfont) == true)
		AddOriginList(fixedfont);
	return fixedfont;
}

// wresrcmng.cpp:870
void WResourceManager::Release(WFont* font) {
	if (fontList.DeleteResource(font) == true)
		delete font;
}

// wresrcmng.cpp:882
W3dSpr* WResourceManager::Get3DSpr(const char* filename, int type) {
	W3dSpr* spr = new W3dSpr;
	spr->SetResourceManager(this);
	if (spr->LoadSprite(filename, type) == 0) {
		return spr;
	}
	delete spr;

	return NULL;
}

// wresrcmng.cpp:898
W3dAniSpr* WResourceManager::Get3DAniSpr(const char* filename, int type, float fSprSizeX, float fSprSizeY) {
	W3dAniSpr* spr = new W3dAniSpr;
	spr->SetResourceManager(this);
	if (spr->LoadSpritesInOneTexture(filename, type, fSprSizeX, fSprSizeY) == 0) {
		return spr;
	}

	delete spr;

	return NULL;
}

// wresrcmng.cpp:968
w_texlist* WResourceManager::FindTexture(const char* texName) {
	char name[64];
	StrcpyLower(name, strchr(texName, '/') ? strrchr(texName, '/') + 1 : texName);

	return texList.Find(name);
}

// wresrcmng.cpp:976
w_texlist* WResourceManager::FindTexture(int texHandle) {
	for (w_texlist* tex = texList.Start(); tex; tex = texList.Next()) {
		if (tex->texhandle == texHandle) {
			return tex;
		}
	}

	return NULL;
}

// wresrcmng.cpp:987
int WResourceManager::GetTextureWidth(int texHandle) {
	for (w_texlist* tex = texList.Start(); tex; tex = texList.Next()) {
		if (tex->texhandle == texHandle) {
			return tex->width;
		}
	}

	return 0;
}

// wresrcmng.cpp:998
int WResourceManager::GetTextureHeight(int texHandle) {
	for (w_texlist* tex = texList.Start(); tex; tex = texList.Next()) {
		if (tex->texhandle == texHandle) {
			return tex->height;
		}
	}

	return 0;
}

// wresrcmng.cpp:1009
void WResourceManager::AddMissingTexture(const char* filename) {
	if (m_missingTextureList.Find(filename) == NULL) {
		char* text = new char[strlen(filename) + 1];
		strcpy(text, filename);
		m_missingTextureList.AddItem(text, text);
	}
}

// wresrcmng.cpp:1019
w_texlist* WResourceManager::AddTexture(const char* texturename, int texhandle, int width, int height) {
	w_texlist* tex;
	char name[64];

	StrcpyLower(name, strchr(texturename, '/') ? strrchr(texturename, '/') + 1 : texturename);
	int len = strlen(name);

	tex = (w_texlist*)new char[sizeof(w_texlist) + len];
	tex->count = 1;
	tex->texhandle = texhandle;
	tex->width = width;
	tex->height = height;
	tex->size = 0;

	strcpy(tex->texname, name);
	texList.AddItem(tex, tex->texname, false);

	return tex;
}

// wresrcmng.cpp:1048
int WResourceManager::UploadTexture(const char* texName, Bitmap* bitmap, unsigned long type, tagRECT* rect) {
	if (texName != NULL) {
		w_texlist* tex = FindTexture(texName);
		if (tex != NULL) {
			tex->count++;
			return tex->texhandle;
		}
	}

	int texhandle;
	int width;
	int height;

	if (rect == NULL) {
		width = bitmap->Width();
		height = bitmap->Height();
		texhandle = video->CreateTexture(bitmap->bi, type);
		if (!(type & 0x1001f000)) {
			video->UpdateTexture(texhandle, bitmap->bi, bitmap->vram, type);
		}
	} else {
		Bitmap temp;
		temp.Create(rect->right - rect->left + 1, rect->bottom - rect->top + 1, bitmap->bi->bmiHeader.biBitCount);

		int bpp = (bitmap->BitsPerPixel() + 7) / 8;
		if (bpp <= 1) {
			int num = bitmap->bi->bmiHeader.biClrUsed ? bitmap->bi->bmiHeader.biClrUsed : 256;
			for (int i = 0; i < num; i++) {
				temp.bi->bmiColors[i].rgbRed = bitmap->bi->bmiColors[i].rgbRed;
				temp.bi->bmiColors[i].rgbGreen = bitmap->bi->bmiColors[i].rgbGreen;
				temp.bi->bmiColors[i].rgbBlue = bitmap->bi->bmiColors[i].rgbBlue;
			}
		}

		int offset = 0;
		for (int y = rect->top; y <= rect->bottom; y++) {
			memcpy(temp.vram + offset, bitmap->vram + bitmap->pitch * y + rect->left * bpp, (rect->right - rect->left + 1) * bpp);
			offset += temp.pitch;
		}

		width = temp.bi->bmiHeader.biWidth;
		height = temp.bi->bmiHeader.biHeight;
		texhandle = video->CreateTexture(temp.bi, type);
		video->UpdateTexture(texhandle, temp.bi, temp.vram, type);
	}

	const char* name = texName;
	if (texName == NULL || rect != NULL) {
		name = "Unknown";
	}

	w_texlist* tex = AddTexture(name, texhandle, width, height);
	tex->size = ((type >> 31) | 2) * height * width;
	if (type & 0x40000000) {
		tex->size += tex->size / 2;
	}
	m_nByteUsedTexture += tex->size;
	if (tex->texhandle == 0) {
		AddMissingTexture(tex->texname);
	}
	return tex->texhandle;
}

// wresrcmng.cpp:1167
int WResourceManager::GetBlankTexture(void) {
	if (m_blankTexture == 0) {
		Bitmap blank;
		blank.Create(32, 32, 24);
		memset(blank.vram, 0xff, blank.Size());
		m_blankTexture = video->CreateTexture(blank.bi, 0);
		video->UpdateTexture(m_blankTexture, blank.bi, blank.vram, 0);
	}

	return m_blankTexture;
}

// wresrcmng.cpp:1183
void WResourceManager::FixTexture(int texHandle, Bitmap* bitmap, unsigned long type, tagRECT* rect) {
	if (rect == NULL) {
		video->UpdateTexture(texHandle, bitmap->bi, bitmap->vram, type);
	} else {
		video->FixTexturePart(texHandle, *rect, bitmap->bi, bitmap->vram, type);
	}
}

// wresrcmng.cpp:1195
void WResourceManager::Release(int texhandle) {
	w_texlist* tex = FindTexture(texhandle);

	if (tex != NULL && --tex->count <= 0) {
		m_nByteUsedTexture -= tex->size;
		video->DestroyTexture(texhandle);
		texList -= tex;
		delete[] tex;
	}
}

// wresrcmng.cpp:1260
int WResourceManager::LoadTexture(const char* filename, unsigned long type, int level, const char* texname) {
	int texhandle = 0;

	INSTANCE_LOCK(RML_TEXTURE_LOADING);

	if (!texname || !stricmp(filename, texname)) {
		w_texlist* tex = FindTexture(filename);
		if (tex) {
			tex->count++;
			return tex->texhandle;
		}
	}

	Bitmap* bitmap = LoadBitmap(filename, level, false);
	if (bitmap) {
		bitmap->Update();
		if (strchr(filename, '{')) {
			type &= ~0x40000000;
			type |= 0x800;
		}
		if (strchr(filename, '!')) {
			type |= 0x20000000;
		}
		CheckMaskTexture(&bitmap, filename);
		texhandle = UploadTexture(texname ? texname : filename, bitmap, type, 0);
		delete bitmap;
	} else {
		char afname[260];
		strcpy(afname, filename);
		strlwr(afname);
		if (strstr(afname, ".dds")) {
			cFile* fp = ::GetCFile(FindMatchFile(filename), 0xffff, 0);
			if (!fp) {
				AddMissingTexture(filename);
				return 0;
			}
			int len = fp->Length();
			unsigned char* buff = new unsigned char[len];
			fp->Read(buff, len);
			CloseCFile(fp);
			texhandle = video->UploadCompressedTexture(buff, len, type);
			AddTexture(texname ? texname : filename, texhandle, video->GetTextureWidth(texhandle), video->GetTextureHeight(texhandle));
			delete[] buff;
		}
	}

	return texhandle;
}

// wresrcmng.cpp:1348
int WResourceManager::LoadTexture_DDS(unsigned char* dds, unsigned int size, const char* filename, int type, int level) {
	INSTANCE_LOCK(RML_TEXTURE_LOADING);

	int texhandle = video->UploadCompressedTexture(dds, size, type);
	if (!texhandle) {
		AddMissingTexture(filename);
	}
	AddTexture(filename, texhandle, video->GetTextureWidth(texhandle), video->GetTextureHeight(texhandle));
	return texhandle;
}

// wresrcmng.cpp:1364
int WResourceManager::LoadTexture(Bitmap* bitmap, const char* filename, int type, int level) {
	INSTANCE_LOCK(RML_TEXTURE_LOADING);

	w_texlist* tex = FindTexture(filename);
	if (tex) {
		tex->count++;
		return tex->texhandle;
	}

	bitmap->Update();
	if (strchr(filename, '{')) {
		type &= ~0x40000000;
		type |= 0x800;
	}
	if (strchr(filename, '!')) {
		type |= 0x20000000;
	}
	CheckMaskTexture(&bitmap, filename);
	return UploadTexture(filename, bitmap, type, 0);
}

// wresrcmng.cpp:1394
void WResourceManager::ReloadTextures(void) {
	INSTANCE_LOCK(RML_TEXTURE_LOADING);

	for (w_texlist* tex = texList.Start(); tex; tex = texList.Next()) {
		Bitmap* bitmap = LoadBitmap(tex->texname, 0, false);
		if (bitmap) {
			bitmap->Update();
			CheckMaskTexture(&bitmap, tex->texname);
			video->UpdateTexture(tex->texhandle, bitmap->bi, bitmap->vram, 0);
			delete bitmap;
		} else {
			char filename[260];
			strcpy(filename, tex->texname);
			strlwr(filename);
			if (strstr(filename, ".dds")) {
				unsigned long type = 0;
				if (strchr(tex->texname, '{')) {
					type = 0x800;
				}
				if (strchr(tex->texname, '!')) {
					type |= 0x20000000;
				}
				cFile* fp = ::GetCFile(FindMatchFile(tex->texname), 0xffff, 0);
				if (fp) {
					int len = fp->Length();
					unsigned char* buff = new unsigned char[len];
					fp->Read(buff, len);
					CloseCFile(fp);
					video->UpdateCompressedTexture(tex->texhandle, buff, len, type);
					delete[] buff;
				}
			}
		}
	}
}

// wresrcmng.cpp:1453
bool WResourceManager::CheckMaskTexture(Bitmap** ppBitmap, const char* filename) {
	if (ppBitmap == NULL || *ppBitmap == NULL || filename == NULL || filename[0] == '\0') {
		return false;
	}

	if (strchr(filename, '[') || strchr(filename, '+')) {
		char afname[260];

		strcpy(afname, filename);
		sprintf(strrchr(afname, '.'), "_mask%s", strrchr(filename, '.'));

		Bitmap* mask = LoadBitmap(afname, 0, false);
		if (mask) {
			if (mask->BitsPerPixel() > 8) {
				// CP949: the mask texture must be an 8-bit black-and-white image.
				MessageBox(NULL, "\xb8\xb6\xbd\xba\xc5\xa9 \xc5\xd8\xbd\xba\xc3\xe7\xb4\xc2 8\xba\xf1\xc6\xae \xb1\xd7\xb7\xb9\xc0\xcc\xb8\xa6 \xc0\xcc\xbf\xeb\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4.\n0.0a", filename, MB_OK);
			}

			if ((*ppBitmap && (*ppBitmap)->Width() != mask->Width()) || (*ppBitmap)->Height() != mask->Height()) {
				return false;
			}

			Bitmap* combined = Combine4Alpha(*ppBitmap, mask);
			delete *ppBitmap;
			delete mask;
			*ppBitmap = combined;

			return true;
		}
	}

	return false;
}

// wresrcmng.cpp:1502
bool WResourceManager::ChangeTexturePart(char* dstName, char* srcName, Bitmap** ppBmp, tagRECT* pRc) {
	w_texlist* tex = FindTexture(dstName);

	if (tex == NULL) {
		return false;
	}
	return ChangeTexturePart(tex->texhandle, srcName, ppBmp, pRc);
}

// wresrcmng.cpp:1524
void WResourceManager::ChangeTexturePart(char* dstName, Bitmap* pBmp, const tagRECT& rc) {
	if (dstName == NULL || pBmp == NULL) {
		return;
	}

	w_texlist* tex = FindTexture(dstName);

	if (tex == NULL) {
		return;
	}

	ChangeTexturePart(tex->texhandle, pBmp, rc);
}

// wresrcmng.cpp:1551
bool WResourceManager::ChangeTexturePart(int hDstTex, char* srcName, Bitmap** ppBmp, tagRECT* pRc) {
	if (!hDstTex) {
		return false;
	}

	w_texlist* tex = FindTexture(hDstTex);
	if (!tex) {
		return false;
	}

	Bitmap* pSrc = LoadBitmapPart(tex->texname, srcName);
	if (pSrc && tex->width == pSrc->Width() && tex->height == pSrc->Height()) {
		tagRECT rc;
		int nFlag = 0;
		if (GetRectInSourceTexture(pSrc->bi, pSrc->vram, rc)) {
			if (pSrc->bi->bmiHeader.biBitCount == 32) {
				nFlag = 0x40000;
			}
			Bitmap* pDst = new Bitmap(rc.right - rc.left, rc.bottom - rc.top, pSrc->BitsPerPixel());
			unsigned char* pDstBit = pDst->vram;
			unsigned char* pSrcBit = pSrc->vram + pSrc->pitch * rc.top + pSrc->BitsPerPixel() * rc.left / 8;
			int iSize = (rc.right - rc.left) * pSrc->BitsPerPixel() / 8;
			for (int i = 0; i < rc.bottom - rc.top; i++) {
				memcpy(pDstBit, pSrcBit, iSize);
				pDstBit += pDst->pitch;
				pSrcBit += pSrc->pitch;
			}
			delete pSrc;
			pSrc = pDst;
			video->FixTexturePart(hDstTex, rc, pSrc->bi, pSrc->vram, nFlag);
		}
		if (ppBmp) {
			*ppBmp = pSrc;
		} else {
			delete pSrc;
		}
		if (pRc) {
			*pRc = rc;
		}
		return true;
	}
	return false;
}

// wresrcmng.cpp:1634
void WResourceManager::ChangeTexturePart(int hDstTex, Bitmap* pBmp, const tagRECT& rc) {
	if (pBmp == NULL) {
		return;
	}

	w_texlist* tex = FindTexture(hDstTex);

	if (tex == NULL) {
		return;
	}

	if (tex->width >= rc.right && tex->height >= rc.bottom && tex->width >= pBmp->Width() && tex->height >= pBmp->Height()) {
		video->FixTexturePart(hDstTex, rc, pBmp->bi, pBmp->vram, pBmp->bi->bmiHeader.biBitCount == 32 ? 0x40000 : 0);
	}
}

// wresrcmng.cpp:1671
bool WResourceManager::GetRectInSourceTexture(const tagBITMAPINFO* src, const void* data, tagRECT& rc) const {
	int y;
	int x;
	int cpp = src->bmiHeader.biBitCount / 8;
	int pitch = (src->bmiHeader.biWidth * cpp + 3) & ~3;
	int padbytes = pitch - src->bmiHeader.biWidth * cpp;
	const unsigned char* p = (const unsigned char*)data;

	if (src->bmiHeader.biBitCount == 8) {
		for (y = 0; y < src->bmiHeader.biHeight; y++, p += padbytes) {
			for (x = 0; x < src->bmiHeader.biWidth; x++, p++) {
				if (*p != *(const unsigned char*)data) {
					goto GET_RECT_SIZE;
				}
			}
		}
	} else if (src->bmiHeader.biBitCount == 24 || src->bmiHeader.biBitCount == 32) {
		for (y = 0; y < src->bmiHeader.biHeight; y++, p += padbytes) {
			for (x = 0; x < src->bmiHeader.biWidth; x++, p += cpp) {
				if ((*(const unsigned int*)p & 0xffffff) != 0xff00) {
					goto GET_RECT_SIZE;
				}
			}
		}
	}
	return false;

GET_RECT_SIZE:
	int left = x;
	int top = y;
	int right, bottom;
	if (src->bmiHeader.biBitCount == 8) {
		unsigned char head = *(const unsigned char*)data;
		right = x + 1;
		const unsigned char* q = (const unsigned char*)data + top * pitch + right;
		while (right < src->bmiHeader.biWidth && *q != head) {
			right++;
			q++;
		}
		bottom = top + 1;
		q = (const unsigned char*)data + bottom * pitch + left;
		while (bottom < src->bmiHeader.biHeight && *q != head) {
			q += pitch;
			bottom++;
		}
	} else {
		right = x + 1;
		const unsigned char* q = (const unsigned char*)data + top * pitch + right * cpp;
		while (right < src->bmiHeader.biWidth && (*(const unsigned int*)q & 0xffffff) != 0xff00) {
			right++;
			q += cpp;
		}
		bottom = top + 1;
		q = (const unsigned char*)data + bottom * pitch + left * cpp;
		while (bottom < src->bmiHeader.biHeight && (*(const unsigned int*)q & 0xffffff) != 0xff00) {
			q += pitch;
			bottom++;
		}
	}

	int width = right - left;
	int height = bottom - top;
	if (width < 1 || height < 1) {
		return false;
	}

	rc.left = left;
	rc.top = top;
	rc.right = left + width;
	rc.bottom = top + height;
	return true;
}

// wresrcmng.cpp:1783
Bitmap* WResourceManager::LoadBitmap(const char* filename, int level, bool bNet) {
	Bitmap* bitmap = NULL;

	INSTANCE_LOCK(RML_BITMAP_LOADING);

	const char* extension = strrchr(filename, '.');
	const char* question = strrchr(filename, '?');

	if (question != NULL) {
		char inname[260] = "";
		memcpy(inname, filename, question - filename);

		bitmap = LoadBRES(question + 1, inname);
	} else if (extension != NULL) {
		if (!strcmpi(extension, ".bmp")) {
			bitmap = LoadBMP(filename);
		} else if (!strcmpi(extension, ".jpg")) {
			bitmap = LoadJPG(filename);
		} else if (!strcmpi(extension, ".png")) {
			bitmap = LoadPNG(filename, false, bNet);
		} else if (!strcmpi(extension, ".tga")) {
			bitmap = LoadTGA(filename);
		}

		if (bitmap == NULL && (!strcmpi(extension, ".jpg") || !strcmpi(extension, ".bmp"))) {
			char temp[256];
			strcpy(temp, filename);
			strcpy(strrchr(temp, '.'), ".png");

			bitmap = LoadPNG(temp, false, false);
		}
	}

	if (bitmap != NULL) {
		if (bitmap->Width() > 8 && bitmap->Height() > 8 && m_savemem[level]) {
			Bitmap* quarter = MakeQuarterBitmap(bitmap);
			delete bitmap;
			bitmap = quarter;
		}
	}

	if (level == 0 && bitmap != NULL) {
		bitmap->Update();
	}

	return bitmap;
}

// wresrcmng.cpp:1856
Bitmap* WResourceManager::LoadBitmapPart(char* dstName, char* srcName) {
	Bitmap* pSrc;

	if (*dstName == '[' || *dstName == '+') {
		pSrc = LoadBitmap(srcName, 0, false);
		if (pSrc == NULL) {
			return NULL;
		}
		pSrc->Update();
		if (!CheckMaskTexture(&pSrc, srcName) && !CheckMaskTexture(&pSrc, dstName)) {
			delete pSrc;
			return NULL;
		}
	} else {
		pSrc = LoadBitmap(srcName, 0, false);
		if (pSrc == NULL) {
			return NULL;
		}
	}
	return pSrc;
}

// wresrcmng.cpp:1896
Bitmap* WResourceManager::MakeQuarterBitmap(Bitmap* bitmap) {
	bitmap->Update();

	Bitmap* quarter = new Bitmap(bitmap->Width() / 2, bitmap->Height() / 2, bitmap->BitsPerPixel());

	if (bitmap->BitsPerPixel() <= 8)
		memcpy(quarter->bi->bmiColors, bitmap->bi->bmiColors, 256 * sizeof(RGBQUAD));

	for (unsigned int y = 0; y < quarter->Height(); y++) {
		for (unsigned int x = 0; x < quarter->Width(); x++) {
			unsigned int u = bitmap->Width() * x / quarter->Width();
			unsigned int v = bitmap->Height() * y / quarter->Height();

			if (bitmap->BitsPerPixel() <= 8) {
				quarter->GetVram(y)[x] = bitmap->GetVram(v)[u];
			} else {
				memcpy(quarter->GetVram(y) + x * 3, bitmap->GetVram(v) + u * 3, 3);
			}
		}
	}

	return quarter;
}

// wresrcmng.cpp:1925
Bitmap* WResourceManager::Combine4Alpha(Bitmap* bitmap, Bitmap* alpha) {
	bitmap->Update();
	alpha->Update();

	Bitmap* result = new Bitmap;
	result->Create(bitmap->Width(), bitmap->Height(), 32);

	for (unsigned int y = 0; y < bitmap->Height(); y++) {
		for (unsigned int x = 0; x < bitmap->Width(); x++) {
			unsigned char r, g, b, a;
			if (bitmap->BitsPerPixel() <= 8) {
				r = bitmap->bi->bmiColors[bitmap->vram[bitmap->pitch * y + x]].rgbRed;
				g = bitmap->bi->bmiColors[bitmap->vram[bitmap->pitch * y + x]].rgbGreen;
				b = bitmap->bi->bmiColors[bitmap->vram[bitmap->pitch * y + x]].rgbBlue;
			} else {
				r = bitmap->vram[bitmap->pitch * y + x * 3 + 2];
				g = bitmap->vram[bitmap->pitch * y + x * 3 + 1];
				b = bitmap->vram[bitmap->pitch * y + x * 3];
			}
			if (alpha->BitsPerPixel() <= 8) {
				a = alpha->bi->bmiColors[alpha->vram[alpha->pitch * y + x]].rgbBlue;
			} else {
				a = alpha->vram[alpha->pitch * y + x * (alpha->BitsPerPixel() / 8)];
			}

			result->vram[result->pitch * y + x * 4] = b;
			result->vram[result->pitch * y + x * 4 + 1] = g;
			result->vram[result->pitch * y + x * 4 + 2] = r;
			result->vram[result->pitch * y + x * 4 + 3] = a;
		}
	}
	return result;
}

// wresrcmng.cpp:1973
Bitmap* WResourceManager::Combine(Bitmap* dest, Bitmap* src) {
	Bitmap* result = new Bitmap;
	result->Create(dest->Width(), dest->Height(), 32);

	unsigned char SrcBpp = (unsigned char)(src->BitsPerPixel() / 8);
	unsigned char DestBpp = (unsigned char)(dest->BitsPerPixel() / 8);

	for (unsigned int y = 0; y < dest->Height(); y++) {
		for (unsigned int x = 0; x < dest->Width(); x++) {
			unsigned char alpha = src->vram[src->pitch * y + x * SrcBpp + 3];
			for (unsigned char i = 0; i < 3; i++) {
				result->vram[result->pitch * y + x * 4 + i] = (unsigned char)((dest->vram[dest->pitch * y + x * DestBpp + i] * (255 - alpha) + src->vram[src->pitch * y + x * SrcBpp + i] * alpha) / 255);
			}
			result->vram[result->pitch * y + x * 4 + 3] = 0xff;
		}
	}
	return result;
}

// The libpng state of the PNG being decoded (VisualPng's globals).
static png_structp png_ptr = NULL;
static png_infop info_ptr = NULL;
static png_bytepp ppbRowPointers = NULL;

// wresrcmng.cpp:2010
static void __cdecl png_cexcept_error(png_struct_def* png_ptr, const char* msg) {
	throw (char*)msg;
}

// wresrcmng.cpp:2018
static void __cdecl png_read_data(png_struct_def* png_ptr, unsigned char* data, unsigned int length) {
	cFile* file = (cFile*)png_ptr->io_ptr;
	if ((unsigned int)file->Read(data, length) != length) {
		png_error(png_ptr, "Read Error");
	}
}

// wresrcmng.cpp:2032
static void __cdecl png_read_data2(png_struct_def* png_ptr, unsigned char* data, unsigned int length) {
	unsigned char* src = (unsigned char*)png_ptr->io_ptr;
	memcpy(data, src, length);
	src += length;
	png_ptr->io_ptr = src;
}

// wresrcmng.cpp:2051
Bitmap* WResourceManager::LoadPNG(const char* filename, bool bFromMem, bool bNet) {
	unsigned char pngSig[8];
	unsigned long iWidth;
	unsigned long iHeight;
	int iBitDepth;
	int iColorType;
	cFile* fp = NULL;
	const unsigned char* data = NULL;
	Bitmap* bitmap;

	if (!bFromMem) {
		fp = ::GetCFile(FindMatchFile(filename), 0xffff, 0);
		if (fp == NULL) {
			AddMissingTexture(filename);
			return NULL;
		}
	} else {
		data = (const unsigned char*)filename;
	}

	try {
		if (bFromMem) {
			memcpy(pngSig, data, 8);
		} else {
			fp->Read(pngSig, 8);
		}

		if (!png_check_sig(pngSig, 8)) {
			if (fp) {
				CloseCFile(fp);
			}
			return NULL;
		}

		png_ptr = png_create_read_struct("1.2.8", NULL, png_cexcept_error, NULL);
		if (!png_ptr) {
			if (fp) {
				CloseCFile(fp);
			}
			return NULL;
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr) {
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			if (fp) {
				CloseCFile(fp);
			}
			return NULL;
		}

		if (!bFromMem) {
			png_set_read_fn(png_ptr, fp, png_read_data);
			png_set_sig_bytes(png_ptr, 8);
		} else {
			data += 8;
			png_set_read_fn(png_ptr, (void*)data, png_read_data2);
			png_set_sig_bytes(png_ptr, 8);
		}

		png_read_info(png_ptr, info_ptr);

		png_get_IHDR(png_ptr, info_ptr, &iWidth, &iHeight, &iBitDepth, &iColorType, NULL, NULL, NULL);

		if (iBitDepth == 16)
			png_set_strip_16(png_ptr);
		if (bNet && info_ptr->pixel_depth == 24 && iBitDepth == 8)
			png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
		if (iColorType == PNG_COLOR_TYPE_PALETTE)
			png_set_expand(png_ptr);
		if (iBitDepth < 8)
			png_set_expand(png_ptr);
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
			png_set_expand(png_ptr);
		if (iColorType == PNG_COLOR_TYPE_GRAY || iColorType == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png_ptr);

		png_set_bgr(png_ptr);

		png_read_update_info(png_ptr, info_ptr);

		png_get_IHDR(png_ptr, info_ptr, &iWidth, &iHeight, &iBitDepth, &iColorType, NULL, NULL, NULL);

		unsigned long ulRowBytes = png_get_rowbytes(png_ptr, info_ptr);
		unsigned long ulChannels = png_get_channels(png_ptr, info_ptr);

		bitmap = new Bitmap;
		if (bitmap == NULL) {
			if (fp) {
				CloseCFile(fp);
			}
			return NULL;
		}

		bitmap->Create(iWidth, iHeight, ulChannels * 8);

		if ((ppbRowPointers = (png_bytepp)png_malloc(png_ptr, iHeight * sizeof(png_bytep))) == NULL) {
			png_error(png_ptr, "Visual PNG: out of memory");
		}

		for (unsigned long i = 0; i < iHeight; i++)
			ppbRowPointers[i] = (png_bytep)bitmap->vram + i * ulRowBytes;

		png_read_image(png_ptr, ppbRowPointers);

		png_read_end(png_ptr, NULL);

		free(ppbRowPointers);
		ppbRowPointers = NULL;

		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	} catch (png_const_charp) {
		if (ppbRowPointers) {
			free(ppbRowPointers);
		}
		ppbRowPointers = NULL;
		if (fp) {
			CloseCFile(fp);
		}
		png_ptr = NULL;
		info_ptr = NULL;

		return NULL;
	} catch (...) {
		png_ptr = NULL;
		info_ptr = NULL;

		return NULL;
	}

	if (fp) {
		CloseCFile(fp);
	}
	return bitmap;
}

// wresrcmng.cpp:2438
Bitmap* WResourceManager::LoadTGA(const char* filename) {
	sTgaHeader header;
	Bitmap* bitmap = NULL;

	cFile* fp = ::GetCFile(FindMatchFile(filename), 0xffff, 0);
	if (fp) {
		fp->Read(&header, sizeof(header));

		if (header.iType == 2) {
			fp->Seek(header.id_size, SEEK_CUR);

			bitmap = new Bitmap;
			bitmap->Create(header.width, header.height, header.bitcount);

			for (int y = 0; y < header.height; y++) {
				fp->Read(bitmap->GetVram(header.height - y - 1), header.width * header.bitcount / 8);
			}
		}

		CloseCFile(fp);
	} else {
		AddMissingTexture(filename);
	}

	return bitmap;
}

// wresrcmng.cpp:2497
Bitmap* WResourceManager::LoadBMP(const char* filename) {
	bmp_head header;

	cFile* fp = ::GetCFile(FindMatchFile(filename), 0xffff, 0);

	if (fp == NULL) {
		AddMissingTexture(filename);
		return NULL;
	}

	fp->Read(&header, sizeof(header));

	// The original compares the literal "header", not header.signature.
	if (memcmp("header", "bm", 2) == 0) {
		CloseCFile(fp);
		return NULL;
	}

	int offset = header.offbits - sizeof(header);

	int size = fp->Length() - fp->Tell();
	unsigned char* buff = new unsigned char[size];
	fp->Read(buff, size);

	Bitmap* bitmap = new Bitmap;

	tagBITMAPINFOHEADER* info = (tagBITMAPINFOHEADER*)buff;
	unsigned char* vram = buff + offset;
	int bpl = ((info->biBitCount >> 3) * info->biWidth + 3) & ~3;
	bitmap->Create(info->biWidth, info->biHeight, info->biBitCount);

	if (info->biBitCount <= 8) {
		memcpy(bitmap->bi->bmiColors, ((tagBITMAPINFO*)buff)->bmiColors, (info->biClrUsed ? info->biClrUsed : 1 << info->biBitCount) * sizeof(RGBQUAD));
	}

	for (int y = 0; y < info->biHeight; y++) {
		memcpy(bitmap->GetVram(y), vram + (info->biHeight - y - 1) * bpl, bitmap->pitch);
	}

	delete[] buff;

	CloseCFile(fp);

	return bitmap;
}

// wresrcmng.cpp:2549
Bitmap* WResourceManager::LoadBRES(const char* zipname, const char* filename) {
	ZIPENTRY ze;

	cFile* fp = ::GetCFile(FindMatchFile(zipname), -1, 0);
	if (!fp) {
		return NULL;
	}

	int len = fp->Length();
	unsigned char* buff = new unsigned char[len];
	fp->Read(buff, len);
	CloseCFile(fp);

	HZIP hz = OpenZipU(buff, len, 3);  // ZIP_MEMORY
	if (!hz) {
		delete[] buff;
		return NULL;
	}

	GetZipItemA(hz, -1, &ze);
	int numItems = ze.index;
	for (int i = 0; i < numItems; i++) {
		GetZipItemA(hz, i, &ze);
		if (!strcmpi(ze.name, filename)) {
			unsigned char* item = new unsigned char[ze.unc_size];
			UnzipItem(hz, i, item, ze.unc_size, 3);  // ZIP_MEMORY

			char* ext = strrchr(ze.name, '.');
			Bitmap* pBitmap;
			unsigned char* vram;
			unsigned int height;
			unsigned int bpl;
			if (!stricmp(ext, ".bmp")) {
				vram = item + 0x36;
				pBitmap = new Bitmap(*(int*)(item + 0x12), *(int*)(item + 0x16), *(short*)(item + 0x1c));
				height = *(int*)(item + 0x16);
				bpl = pBitmap->pitch;
			} else if (!stricmp(ext, ".tga")) {
				vram = item + 0x12;
				pBitmap = new Bitmap(*(short*)(item + 0xc), *(short*)(item + 0xe), item[0x10]);
				height = *(short*)(item + 0xe);
				bpl = item[0x10] * *(short*)(item + 0xc) / 8;
			} else {
				delete[] item;
				break;
			}

			for (unsigned int y = 0; y < height; y++) {
				memcpy(pBitmap->vram + pBitmap->pitch * y, vram + (height - 1 - y) * bpl, bpl);
			}

			delete[] item;
			CloseZipU(hz);
			delete[] buff;
			return pBitmap;
		}
	}

	CloseZipU(hz);
	delete[] buff;
	return NULL;
}

// wresrcmng.cpp:2647
JpegBitmap::JpegBitmap(char* data, int len) {
	m_data = data;
	m_size = len;
}

// wresrcmng.cpp:2650
JpegBitmap::~JpegBitmap(void) {
	if (m_data) {
		delete[] m_data;
		m_data = NULL;
	}
}

// wresrcmng.cpp:2656
void JpegBitmap::Update(void) {
	if (m_data && DecodeImage() == true) {
		if (m_data) { delete[] m_data; m_data = NULL; }
	}
}

// wresrcmng.cpp:2671
bool JpegBitmap::DecodeHeader(void) {
	JPEG_CORE_PROPERTIES image;

	memset(&image, 0, sizeof(JPEG_CORE_PROPERTIES));

	if (ijlInit(&image) != IJL_OK) {
		return false;
	}

	image.JPGBytes = (unsigned char*)m_data;
	image.JPGSizeBytes = m_size;

	if (ijlRead(&image, IJL_JBUFF_READPARAMS) == IJL_OK) {
		bi = (tagBITMAPINFO*)new unsigned char[sizeof(BITMAPINFOHEADER) + (image.JPGChannels == 1 ? 256 * sizeof(RGBQUAD) : 0)];
		bi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bi->bmiHeader.biPlanes = 1;
		bi->bmiHeader.biCompression = BI_RGB;
		bi->bmiHeader.biSizeImage = 0;
		bi->bmiHeader.biClrUsed = 0;
		bi->bmiHeader.biWidth = image.JPGWidth;
		bi->bmiHeader.biHeight = image.JPGHeight;
		bi->bmiHeader.biBitCount = image.JPGChannels == 1 ? 8 : 24;
		ijlFree(&image);
		return true;
	}

	ijlFree(&image);
	return false;
}

// wresrcmng.cpp:2702
bool JpegBitmap::DecodeImage(void) {
	JPEG_CORE_PROPERTIES image;

	memset(&image, 0, sizeof(JPEG_CORE_PROPERTIES));

	if (ijlInit(&image) != IJL_OK) {
		return false;
	}

	image.JPGBytes = (unsigned char*)m_data;
	image.JPGSizeBytes = m_size;

	if (ijlRead(&image, IJL_JBUFF_READPARAMS) == IJL_OK) {
		Create(image.JPGWidth, image.JPGHeight, image.JPGChannels == 1 ? 8 : 24);

		image.DIBBytes = vram;
		image.DIBColor = image.JPGChannels == 1 ? IJL_G : IJL_BGR;
		image.DIBHeight = image.JPGHeight;
		image.DIBWidth = image.JPGWidth;
		image.DIBChannels = image.JPGChannels == 1 ? 1 : 3;
		image.DIBPadBytes = pitch - image.DIBChannels * image.DIBWidth;

		switch (image.JPGChannels) {
		case 1:
			image.JPGColor = IJL_G;
			break;
		case 3:
			image.JPGColor = IJL_YCBCR;
			break;
		default:
			image.DIBColor = IJL_OTHER;
			image.JPGColor = IJL_OTHER;
			break;
		}

		if (ijlRead(&image, IJL_JBUFF_READWHOLEIMAGE) == IJL_OK) {
			if (image.JPGChannels == 1) {
				unsigned char pal[768];
				int i;

				for (i = 0; i < 8; i++) {
					pal[i * 3 + 2] = 0; pal[i * 3 + 1] = 0; pal[i * 3 + 0] = 0;
				}
				for (i = 8; i < 248; i++) {
					pal[i * 3 + 2] = (unsigned char)i; pal[i * 3 + 1] = (unsigned char)i; pal[i * 3 + 0] = (unsigned char)i;
				}
				for (i = 248; i < 256; i++) {
					pal[i * 3 + 2] = 0xff; pal[i * 3 + 1] = 0xff; pal[i * 3 + 0] = 0xff;
				}

				SetPalette(pal);
			}

			ijlFree(&image);
			return true;
		}
	}

	ijlFree(&image);
	return false;
}

// wresrcmng.cpp:2772
Bitmap* WResourceManager::LoadJPG(const char* filename) {
	cFile* fp = ::GetCFile(FindMatchFile(filename), 0xffff, 0);
	if (fp == NULL) {
		AddMissingTexture(filename);
		return NULL;
	}

	int len = fp->Length();
	char* data = new char[len];
	fp->Read(data, len);
	CloseCFile(fp);

	JpegBitmap* bitmap = new JpegBitmap(data, len);
	if (bitmap->DecodeHeader() == true) {
		return bitmap;
	}

	delete bitmap;
	return NULL;
}

// wresrcmng.cpp:2803
void WResourceManager::SetAutoMatchDirectory(const char* directory) {
	m_matchDirectory = directory != NULL;
	ReleaseMatchList();
	LoadMatchList(directory);
}

// wresrcmng.cpp:2814
void WResourceManager::LoadMatchList(const char* directory) {
	WList<char*> list(0x200, 0x200);

	if (directory != NULL) {
		char name[256];
		ExtractFilename(name, directory);
		if (name[0] != '\0' && name[strlen(name) - 1] != '/') {
			strcat(name, "/");
		}

		EnumFileList(&list);

		for (char* p = list.Start(); p; p = list.Next()) {
			AddMatchList(p);
			delete[] p;
		}
	}
}

// wresrcmng.cpp:2845
void WResourceManager::ReleaseMatchList(void) {
	for (w_match_filename* match = m_matchList.Start(); match; match = m_matchList.Next()) {
		g_mem.Free(match);
	}

	m_matchList.Reset();
}

// wresrcmng.cpp:2859
void WResourceManager::LoadMatchCacheList(const char* filename) {
	cFile* file = ::GetCFile(filename, 0x8000, 0);
	if (file) {
		do {
			char fullname[256];
			file->Scan("%s", fullname);
			AddMatchList(fullname);
		} while (file->Tell() < file->Length());
		CloseCFile(file);
	}
}

// wresrcmng.cpp:2874
void WResourceManager::SaveMatchCacheList(const char* filename) {
	FILE* fp = fopen(filename, "wb");
	for (w_match_filename* match = m_matchList.Start(); match; match = m_matchList.Next()) {
		fprintf(fp, "%s\n", match->fullname);
	}
	fclose(fp);
}

// wresrcmng.cpp:2885
void WResourceManager::UpdateMatchList(const char* directory) {
	_WIN32_FIND_DATAA fd;
	void* find;
	char name[256];
	char filter[256];

	strcpy(filter, directory);
	strcat(filter, "*.*");
	find = FindFirstFileA(filter, &fd);
	if (find != INVALID_HANDLE_VALUE) {
		do {
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") && strcmpi(fd.cFileName, "cvs")) {
					sprintf(name, "%s%s/", directory, fd.cFileName);
					UpdateMatchList(name);
				}
			} else {
				sprintf(name, "%s%s", directory, fd.cFileName);
				AddMatchList(name);
			}
		} while (FindNextFileA(find, &fd));
	}

	FindClose(find);
}

// wresrcmng.cpp:2919
const char* WResourceManager::FindMatchFile(const char* filename) {
	char fname[64];
	w_match_filename* match = NULL;

	fname[0] = '\0';
	if (m_matchDirectory) {
		const char* p = strrchr(filename, '/');
		if (p == NULL) {
			p = strrchr(filename, '\\');
		}
		StrcpyLower(fname, p == NULL ? filename : p + 1);
		match = m_matchList.Find(fname);
	}

	return match ? match->fullname : filename;
}

// wresrcmng.cpp:2940
cFile* WResourceManager::GetCFile(const char* filename, int len) {
	return ::GetCFile(FindMatchFile(filename), len, 0);
}

// wresrcmng.cpp:2945
bool WResourceManager::FindFileInZip(const char* zipname, const char* filename) {
	cFile* fp = ::GetCFile(FindMatchFile(zipname), -1, 0);
	if (fp == NULL) {
		return false;
	}

	int len = fp->Length();
	char* data = new char[len];
	fp->Read(data, len);
	CloseCFile(fp);

	// 3 = XUnzip's ZIP_MEMORY
	HZIP hz = OpenZipU(data, len, 3);
	if (hz != NULL) {
		ZIPENTRY ze;
		GetZipItemA(hz, -1, &ze);
		int numItems = ze.index;

		for (int i = 0; i < numItems; i++) {
			GetZipItemA(hz, i, &ze);

			if (strcmpi(ze.name, filename) == 0) {
				CloseZipU(hz);
				delete[] data;
				return true;
			}
		}

		CloseZipU(hz);
	}

	delete[] data;
	return false;
}

// wresrcmng.cpp:2989
void WResourceManager::StrcpyLower(char* out, const char* src) {
	int i;
	for (i = 0; src[i]; i++) {
		out[i] = src[i] + ((src[i] >= 'A' && src[i] <= 'Z') ? 'a' - 'A' : 0);
	}

	out[i] = '\0';
}

// wresrcmng.cpp:2999
void WResourceManager::AddMatchList(const char* filename) {
	char fullname[256];

	StrcpyLower(fullname, filename);
	char* fname = strrchr(fullname, '/') ? strrchr(fullname, '/') + 1 : fullname;

	if (m_matchList.Find(fname) != NULL) {
		return;
	}

	int len = strlen(fullname) + 1;

	w_match_filename* n = (w_match_filename*)g_mem.Alloc(len);

	StrcpyLower(n->fullname, fullname);

	m_matchList.AddItem(n, n->fullname + (fname - fullname), false);
}

// wresrcmng.cpp:3072
WxStaticPuppetGrp* WResourceManager::xGetStaticPuppetGrp(int index, const char* name, WPuppet** puppet) {
	WxStaticPuppetGrp* grp = new WxStaticPuppetGrp;

	grp->SetResourceManager(this);
	if (grp->xLoad(index, name, puppet) == 0) {
		return grp;
	}
	delete grp;

	return NULL;
}
