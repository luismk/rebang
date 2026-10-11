// Original: d:\build\custom\temp\code\client\projectg\commonutil.cpp
#include "commonutil.h"
#include "padding.h"
#include "Exception.h"
#include "bitmap.h"
#include "westpak.h"
#include "wresrcmng.h"
#include "wlist.h"
#include "cCRC32.h"
#include "Registry.h"

#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <list>
#include <functional>
#include <ctype.h>

// westpak.cpp
void __cdecl CloseCFile(cFile* file);
void __cdecl HookPAKFile(const char* filename);

extern HINSTANCE g_instance;

// Loading progress shown while the pak files are checked (loading screen).
extern int g_iLoadTotalNum;
extern int g_iLoadNextNum;
extern bool g_bExitDrawLoading;

// A pak file of the update list: its name, its expected CRC and the key
// IntegrityPakCRC sorts the files to load by (greatest first).
struct PAKINFO {
	char m_name[0x104];       // 0x0
	unsigned long m_crc;      // 0x104
	unsigned long m_sortKey;  // 0x108
};
CHECK_SIZE(PAKINFO, 0x10C);
CHECK_FIELD(PAKINFO, m_crc, 0x104);
CHECK_FIELD(PAKINFO, m_sortKey, 0x108);

// Fetches the update list (pak files and their CRCs) of the integrated pak.
bool DownloadUpdateList(std::list<PAKINFO>* pakList, char* integratedName);

// Result of LoadStr.
static char s_loadStrBuffer[0x200];

// Ring of result buffers of MakeStr, one ring per thread.
static __declspec(thread) char s_makeStrBuffers[64][0x240];
static __declspec(thread) unsigned char s_makeStrIndex;

WResourceManager* WFile::m_pResMng;

#pragma pack(push, 1)
// BITMAPFILEHEADER with the signature as two characters.
struct BmpFileHeader {
	char type[2]; // 0x0
	DWORD size; // 0x2
	WORD reserved1; // 0x6
	WORD reserved2; // 0x8
	DWORD offBits; // 0xa
};
#pragma pack(pop)
CHECK_SIZE(BmpFileHeader, 0xe);

// Intel JPEG Library 1.5 (ijl.h, not available: proprietary): the part of
// JPEG_CORE_PROPERTIES SaveJPG fills, and the entry points it calls.
enum IJLERR { IJL_OK = 0 };
enum IJLIOTYPE { IJL_JFILE_WRITEWHOLEIMAGE = 8 };
enum IJL_COLOR { IJL_RGB = 1, IJL_BGR = 2, IJL_YCBCR = 3 };
enum IJL_JPGSUBSAMPLING { IJL_411 = 1 };

struct JPEG_CORE_PROPERTIES {
	DWORD UseJPEGPROPERTIES;               // 0x0
	BYTE* DIBBytes;                        // 0x4
	DWORD DIBWidth;                        // 0x8
	DWORD DIBHeight;                       // 0xc
	DWORD DIBPadBytes;                     // 0x10
	DWORD DIBChannels;                     // 0x14
	IJL_COLOR DIBColor;                    // 0x18
	DWORD DIBSubsampling;                  // 0x1c
	LPCSTR JPGFile;                        // 0x20
	BYTE* JPGBytes;                        // 0x24
	DWORD JPGSizeBytes;                    // 0x28
	DWORD JPGWidth;                        // 0x2c
	DWORD JPGHeight;                       // 0x30
	DWORD JPGChannels;                     // 0x34
	IJL_COLOR JPGColor;                    // 0x38
	IJL_JPGSUBSAMPLING JPGSubsampling;     // 0x3c
	DWORD JPGThumbWidth;                   // 0x40
	DWORD JPGThumbHeight;                  // 0x44
	DWORD cconversion_reqd;                // 0x48
	DWORD upsampling_reqd;                 // 0x4c
	DWORD jquality;                        // 0x50
	PADDING(0x54, 0x4e68);                 // JPEG_PROPERTIES jprops
};
CHECK_SIZE(JPEG_CORE_PROPERTIES, 0x4e68);
CHECK_FIELD(JPEG_CORE_PROPERTIES, JPGFile, 0x20);
CHECK_FIELD(JPEG_CORE_PROPERTIES, jquality, 0x50);

extern "C" IJLERR __stdcall ijlInit(JPEG_CORE_PROPERTIES* jcprops);
extern "C" IJLERR __stdcall ijlFree(JPEG_CORE_PROPERTIES* jcprops);
extern "C" IJLERR __stdcall ijlWrite(JPEG_CORE_PROPERTIES* jcprops, IJLIOTYPE iotype);

// commonutil.cpp:216
void SaveJPG(const char* filename, const Bitmap* bitmap, int quality) {
	JPEG_CORE_PROPERTIES jcprops;

	memset(&jcprops, 0, sizeof(jcprops));

	if (ijlInit(&jcprops) != IJL_OK) {
		return;
	}

	jcprops.DIBBytes = bitmap->vram;
	jcprops.DIBWidth = bitmap->Width();
	jcprops.DIBHeight = bitmap->Height();
	jcprops.DIBColor = IJL_BGR;
	jcprops.DIBChannels = 3;
	jcprops.DIBPadBytes = bitmap->pitch - bitmap->Width() * 3;

	jcprops.JPGFile = filename;
	jcprops.JPGWidth = jcprops.DIBWidth;
	jcprops.JPGHeight = jcprops.DIBHeight;
	jcprops.JPGColor = IJL_YCBCR;
	jcprops.JPGChannels = 3;
	jcprops.JPGSubsampling = IJL_411;
	jcprops.jquality = quality;

	ijlWrite(&jcprops, IJL_JFILE_WRITEWHOLEIMAGE);

	ijlFree(&jcprops);
}

// commonutil.cpp:247
void SaveBMP(const char* filename, const Bitmap* bitmap) {
	BmpFileHeader fileHeader;
	BITMAPINFOHEADER infoHeader;

	infoHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.biWidth = 0;
	infoHeader.biHeight = 0;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 24;
	infoHeader.biCompression = 0;
	infoHeader.biSizeImage = 0;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biClrImportant = 0;

	fileHeader.type[0] = 'B';
	fileHeader.type[1] = 'M';
	fileHeader.size = sizeof(BmpFileHeader);
	fileHeader.reserved1 = 0;
	fileHeader.reserved2 = 0;
	fileHeader.offBits = sizeof(BmpFileHeader) + sizeof(BITMAPINFOHEADER);

	FILE* fp = fopen(filename, "wb");
	if (fp != 0) {
		infoHeader.biWidth = bitmap->Width();
		infoHeader.biHeight = bitmap->Height();
		fwrite(&fileHeader, 1, sizeof(BmpFileHeader), fp);
		fwrite(&infoHeader, 1, sizeof(BITMAPINFOHEADER), fp);
		int bytesPerPixel = bitmap->BitsPerPixel() / 8;
		for (int y = infoHeader.biHeight - 1; y >= 0; y--) {
			for (int x = 0; x < infoHeader.biWidth; x++) {
				unsigned char* pixel = bitmap->vram + bitmap->pitch * y + x * bytesPerPixel;
				fputc(pixel[0], fp);
				fputc(pixel[1], fp);
				fputc(pixel[2], fp);
			}
			int padding = ((infoHeader.biWidth * 3 + 3) & ~3) - infoHeader.biWidth * 3;
			for (int i = 0; i < padding; i++) {
				fputc(0, fp);
			}
		}
		fclose(fp);
	}
}

// commonutil.cpp:302
const char* LoadStr(unsigned int id) {
	LoadStringA(g_instance, id, s_loadStrBuffer, sizeof(s_loadStrBuffer));
	return s_loadStrBuffer;
}

// commonutil.cpp:327
// The executable sorts first, then the pak files by name.
namespace std {
template<> struct less<PAKINFO> : public binary_function<PAKINFO, PAKINFO, bool> {
	bool operator()(PAKINFO left, PAKINFO right) {
		if (strcmpi("ProjectG_ReleaseQA.exe", left.m_name) == 0) {
			return false;
		}
		if (strcmpi("ProjectG_ReleaseQA.exe", right.m_name) == 0) {
			return true;
		}
		return strcmpi(left.m_name, right.m_name) < 0;
	}
};
}

// commonutil.cpp:344
bool IsPakFile(const char* filename) {
	int len = strlen(filename);
	if (len < 5) {
		return false;
	}
	return _strcmpi(filename + len - 4, ".pak") == 0;
}

// commonutil.cpp:355
bool IsIntegratedPakFile(const char* filename) {
	int len = strlen(filename);
	if (len < 6) {
		return false;
	}
	return _strcmpi(filename + len - 5, "+.pak") == 0;
}

// commonutil.cpp:366
bool IsExeFile(const char* filename) {
	int len = strlen(filename);
	if (len < 5) {
		return false;
	}
	return _strcmpi(filename + len - 4, ".exe") == 0;
}

// commonutil.cpp:377
void DeleteReadOnlyFile(const char* filename) {
	DWORD attributes = GetFileAttributesA(filename);
	if (attributes & FILE_ATTRIBUTE_READONLY) {
		SetFileAttributesA(filename, attributes & ~FILE_ATTRIBUTE_READONLY);
	}
	DeleteFileA(filename);
}

// commonutil.cpp:387
void TrimExt(const char* filename, char* out) {
	strcpy(out, filename);
	char* ext = strrchr(out, '.');
	if (ext) {
		*ext = 0;
	}
}

// commonutil.cpp:399
void GetModuleName(char* out) {
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	char* name = strrchr(path, '\\');
	if (name) {
		strcpy(out, name + 1);
	} else {
		strcpy(out, path);
	}
}

// commonutil.cpp:557
WTL::CString __stdcall GetRegIntegratedPakName() {
	CRegistry registry;
	registry.SetRootKey(HKEY_LOCAL_MACHINE);
	registry.SetKey("Software\\Ntreev\\Pangya_QA", TRUE);

	WTL::CString name = "";
	name = registry.ReadString("IntegratedPak", "");
	return name;
}

// commonutil.cpp:597
namespace std {
template<> struct greater<PAKINFO> : public binary_function<PAKINFO, PAKINFO, bool> {
	bool operator()(const PAKINFO& left, const PAKINFO& right) const {
		return left.m_sortKey > right.m_sortKey;
	}
};
}

// commonutil.cpp:604
int __stdcall IntegrityPakCRC(std::list<PAKINFO>& pakList, const char* integratedName, std::string& errorFile) {
	SYSTEM_INFO systemInfo = { 0 };
	GetSystemInfo(&systemInfo);
	int multiProcessor = 0;
	if (systemInfo.dwNumberOfProcessors > 1) {
		multiProcessor = 1;
	}

	CRC_32 crc;
	std::list<PAKINFO> checkList;

	for (std::list<PAKINFO>::iterator it = pakList.begin(); it != pakList.end(); ++it) {
		if (strcmpi("ProjectG_ReleaseQA.exe", it->m_name) == 0) {
			char moduleName[0x104] = { 0 };
			GetModuleName(moduleName);
			if (strcmpi(moduleName, it->m_name) == 0) {
				unsigned long value = crc.CalcCRC(it->m_name, NULL);
				if (value == it->m_crc) {
					continue;
				}
			}

			errorFile = "ProjectG_ReleaseQA.exe";
			g_bExitDrawLoading = true;
			return 0;
		}

		char baseName[0x104] = { 0 };
		strcpy(baseName, it->m_name);
		char* ext = strrchr(baseName, '.');
		if (ext) {
			*ext = 0;
		}

		if (strcmpi(integratedName, baseName) > 0) {
			continue;
		}

		if (!multiProcessor || strstr(it->m_name, "_")) {
			checkList.push_back(*it);
		}

		HookPAKFile(it->m_name);
	}

	float total = (float)checkList.size();
	float checked = 0.0f;
	checkList.sort(std::greater<PAKINFO>());
	for (std::list<PAKINFO>::iterator it = checkList.begin(); it != checkList.end(); ++it) {
		if (it->m_crc != crc.CalcCRC(it->m_name, NULL)) {
			errorFile = it->m_name;
			return 0;
		}

		checked += 1.0f;
		g_iLoadNextNum = (int)(checked / total * g_iLoadTotalNum);
	}

	g_iLoadNextNum = g_iLoadTotalNum;
	return 1;
}

// commonutil.cpp:717
bool __stdcall GetCRCCheckList(std::list<std::pair<std::string, unsigned long> >& crcList) {
	std::list<PAKINFO> pakList;
	WTL::CString name = GetRegIntegratedPakName();
	char integratedName[0x40] = { 0 };
	if (name.GetLength() != 0) {
		strcpy(integratedName, name);
		char* ext = strrchr(integratedName, '.');
		if (ext) {
			*ext = 0;
		}
	} else {
		// "Install the full version, or run the launcher first."
		MessageBox(NULL, "\xc0\xce\xbd\xba\xc5\xe7 \xb9\xf6\xc1\xaf\xc0\xbb \xbc\xb3\xc4\xa1, \xc8\xa4\xc0\xba \xb7\xb1\xc3\xc4\xb8\xa6 \xb8\xd5\xc0\xfa \xbd\xc7\xc7\xe0\xc7\xcf\xbd\xc3\xb1\xe2 \xb9\xd9\xb6\xf8\xb4\xcf\xb4\xd9.",
			"\xc6\xc4\xc0\xcf \xbf\xa1\xb7\xaf", MB_ICONEXCLAMATION);
		return false;
	}

	if (!DownloadUpdateList(&pakList, integratedName)) {
		return false;
	}

	pakList.sort(std::less<PAKINFO>());

	for (std::list<PAKINFO>::iterator it = pakList.begin(); it != pakList.end(); ++it) {
		PAKINFO* info = &*it;
		if (strcmpi("ProjectG_ReleaseQA.exe", info->m_name) == 0) {
			continue;
		}

		char baseName[0x104] = { 0 };
		strcpy(baseName, info->m_name);
		char* ext = strrchr(baseName, '.');
		if (ext) {
			*ext = 0;
		}
		if (strcmpi(integratedName, baseName) > 0) {
			continue;
		}

		if (strstr(info->m_name, "_") == NULL) {
			crcList.push_back(std::pair<std::string, unsigned long>(std::string(info->m_name), info->m_crc));
		}
	}
	return true;
}

// commonutil.cpp:776
bool InitPakedFile2() {
	std::list<PAKINFO> pakList;
	char integratedName[0x40] = { 0 };

	WTL::CString name = GetRegIntegratedPakName();
	if (name.GetLength() != 0) {
		strcpy(integratedName, name);
		char* ext = strrchr(integratedName, '.');
		if (ext) {
			*ext = 0;
		}
	} else {
		// "Install the full version, or run the launcher first."
		MessageBox(NULL, "\xc0\xce\xbd\xba\xc5\xe7 \xb9\xf6\xc1\xaf\xc0\xbb \xbc\xb3\xc4\xa1, \xc8\xa4\xc0\xba \xb7\xb1\xc3\xc4\xb8\xa6 \xb8\xd5\xc0\xfa \xbd\xc7\xc7\xe0\xc7\xcf\xbd\xc3\xb1\xe2 \xb9\xd9\xb6\xf8\xb4\xcf\xb4\xd9.",
			"\xc6\xc4\xc0\xcf \xbf\xa1\xb7\xaf", MB_ICONEXCLAMATION);
		return false;
	}

	if (!DownloadUpdateList(&pakList, integratedName)) {
		return false;
	}

	pakList.sort(std::less<PAKINFO>());

	std::string errorFile;
	// The original tests IntegrityPakCRC's result this way round: its 1
	// (every file checked) reports a damaged file.
	if (IntegrityPakCRC(pakList, integratedName, errorFile)) {
		// "Damage was found in the %s file. Please run it again."
		MessageBox(NULL, MakeStr("%s \xc6\xc4\xc0\xcf\xbf\xa1 \xbc\xd5\xbb\xf3\xc0\xcc \xb9\xdf\xb0\xdf\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9. \xb4\xd9\xbd\xc3 \xbd\xc7\xc7\xe0\xc7\xd8\xc1\xd6 \xbc\xbc\xbf\xe4.", errorFile.c_str()),
			"\xc6\xc4\xc0\xcf \xbf\xa1\xb7\xaf", MB_ICONEXCLAMATION);
		return false;
	}

	g_resrcmng->SetAutoMatchDirectory("data");
	return true;
}

// commonutil.cpp:880
void InitPakedFile() {
	try {
		WList<char*> paks(16, 16);
		WIN32_FIND_DATA fd;
		HANDLE hFind = FindFirstFile("projectg*.pak", &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				char* name = new char[strlen(fd.cFileName) + 1];
				strcpy(name, fd.cFileName);
				paks += name;
			} while (FindNextFile(hFind, &fd));
		}
		FindClose(hFind);

		char* first;
		while ((first = paks.Start()) != NULL) {
			for (char* name = first; name != NULL; name = paks.Next()) {
				if (strcmp(name, first) < 0) {
					first = name;
				}
			}
			paks -= first;
			HookPAKFile(first);
			delete[] first;
		}

		g_resrcmng->SetAutoMatchDirectory("data");
	} catch (...) {
		// "There is a problem with the files. Please reinstall." / "File error"
		MessageBox(NULL, "\xc6\xc4\xc0\xcf\xbf\xa1 \xb9\xae\xc1\xa6\xb0\xa1 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9. \xb4\xd9\xbd\xc3 \xbc\xb3\xc4\xa1\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4.",
			"\xc6\xc4\xc0\xcf \xbf\xa1\xb7\xaf", MB_ICONINFORMATION);
		throw;
	}
}

// commonutil.cpp:924
void LineParse(const char* line, std::vector<std::string>& tokens) {
	while (*line != 0 && *line != '\r') {
		std::string token;
		if (*line != '"') {
			while (!isspace(*line) && *line != 0) {
				token += *line;
				line++;
			}
		} else {
			line++;
			while (*line != '"' && *line != 0) {
				token += *line;
				line++;
			}
			if (*line == '"') {
				line++;
			}
		}
		tokens.push_back(token);
		while (isspace(*line)) {
			line++;
		}
	}
}

// commonutil.cpp:944
char* GetStr(char* str, char* out) {
	while ((*str == ' ' || *str == '\t') && *str != 0) {
		str++;
	}
	if (*str != 0) {
		if (*str++ == '"') {
			while (*str != 0 && *str != '"') {
				*out++ = *str++;
			}
			if (*str != 0) {
				str++;
			}
		}
	}
	*out = 0;
	return str;
}

// commonutil.cpp:975
const char* GetStrConst(const char* str, char* out) {
	while ((*str == ' ' || *str == '\t') && *str != 0) {
		str++;
	}
	if (*str != 0) {
		if (*str++ == '"') {
			while (*str != 0 && *str != '"') {
				*out++ = *str++;
			}
			if (*str != 0) {
				str++;
			}
		}
	}
	*out = 0;
	return str;
}

// commonutil.cpp:1005
const char* GetCharsTillSpace(const char* str, char* out) {
	while ((*str == ' ' || *str == '\t') && *str != 0) {
		str++;
	}
	if (*str == 0) {
		str = 0;
	} else {
		while (*str != 0 && *str != ' ') {
			*out++ = *str++;
		}
	}
	*out = 0;
	return str;
}

// commonutil.cpp:1021
char* FindTag(char* str, char* end, char* tag) {
	if (str != 0 && tag != 0) {
		char* found = strstr(str, tag);
		if (found != 0 && found < end) {
			return found + strlen(tag);
		}
	}
	return 0;
}

// commonutil.cpp:1037
void LimitText(char* str, int limit) {
	if (str != 0 && strlen(str) > (unsigned int)limit) {
		int i;
		for (i = 0; i < limit; i++) {
			if (str[i] & 0x80) {
				if (i + 1 >= limit) {
					break;
				}
				i++;
			}
		}
		str[i] = 0;
	}
}

// commonutil.cpp:1062
char* __cdecl MakeStr(char const* pszFormat, ...) {
	va_list args;
	va_start(args, pszFormat);
	_vsnprintf(s_makeStrBuffers[s_makeStrIndex & 63], sizeof(s_makeStrBuffers[0]), pszFormat, args);
	return s_makeStrBuffers[s_makeStrIndex++ & 63];
}

// commonutil.cpp:1079
bool str_Replace(std::string& str, const std::string& from, const std::string& to) {
	bool replaced = false;
	std::string::size_type pos = str.find(from, 0);
	while (pos != std::string::npos) {
		str.replace(str.begin() + pos, str.begin() + pos + from.size(), to);
		pos = str.find(from, pos + to.size());
		replaced = true;
	}
	return replaced;
}

// commonutil.cpp:1096
std::string GetMyComputerName() {
	DWORD size = 0;
	char name[17];
	size = sizeof(name);
	GetComputerNameA(name, &size);
	return std::string(name);
}

// commonutil.cpp:1106
float Random(float fMin, float fMax, int nRand) {
	if (nRand < 0) {
		nRand = rand();
	}
	return (fMax - fMin) * (nRand % 1023) * (1.0f / 1024.0f) + fMin;
}

// commonutil.cpp:1116
unsigned long htoi(const char* str) {
	char buffer[256];
	unsigned long value;
	if (str == 0 || (value = strlen(str)) == 0) {
		return 0;
	}
	sprintf(buffer, "0x%s", str);
	sscanf(buffer, "%X", &value);
	return value;
}

// commonutil.cpp:1128
unsigned long ModifyColor(unsigned long color, float factor) {
	unsigned char red = (unsigned char)(((color >> 16) & 0xff) * factor > 255.0f ? 255.0f : ((color >> 16) & 0xff) * factor);
	unsigned char green = (unsigned char)(((color >> 8) & 0xff) * factor > 255.0f ? 255.0f : ((color >> 8) & 0xff) * factor);
	unsigned char blue = (unsigned char)((color & 0xff) * factor > 255.0f ? 255.0f : (color & 0xff) * factor);

	return (red << 16) | (green << 8) | blue;
}

// commonutil.cpp:1139
void RemakeNumber(int number, char* out) {
	char buffer[32];
	sprintf(buffer, "%d", number);
	int len = strlen(buffer);
	int i = 0, j = 0;
	while (i < len) {
		out[j++] = buffer[i++];
		if (i < len && (len - i) % 3 == 0) {
			out[j++] = ',';
		}
	}
	out[j] = 0;
}

// commonutil.cpp:1166
WFile::WFile() {
	m_filename = 0;
	m_pFile = 0;
}

// commonutil.cpp:1171
WFile::WFile(const char* filename) : m_filename(filename), m_pFile(0) {
	m_pFile = m_pResMng->GetCFile(filename, -1);
	if (!m_pFile) {
		// "Cannot open the file."
		throw WAppException("\xc6\xc4\xc0\xcf\xc0\xbb \xbf\xad \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.");
	}
}

// commonutil.cpp:1180
WFile::~WFile() {
	Close();
}

// commonutil.cpp:1185
cFile* WFile::Open(const char* filename, unsigned int flag) {
	Close();
	return m_pFile = m_pResMng->GetCFile(filename, flag);
}

// commonutil.cpp:1193
void WFile::Close() {
	if (m_pFile) {
		CloseCFile(m_pFile);
	}
	m_pFile = 0;
}

// commonutil.cpp:1201
int WFile::GetByte() {
	if (m_pFile) {
		return m_pFile->GetByte();
	}
	return -1;
}

// commonutil.cpp:1208
int WFile::Tell() {
	if (m_pFile) {
		return m_pFile->Tell();
	}
	return -1;
}

// commonutil.cpp:1215
int WFile::Read(void* buffer, int size) {
	if (m_pFile) {
		return m_pFile->Read(buffer, size);
	}
	return -1;
}

// commonutil.cpp:1222
void WFile::Seek(int offset, int origin) {
	if (m_pFile) {
		m_pFile->Seek(offset, origin);
	}
}

// commonutil.cpp:1229
int WFile::Length() {
	if (m_pFile) {
		return m_pFile->Length();
	}
	return -1;
}

// commonutil.cpp:1236
void WFile::SetResourceManager(WResourceManager* pResMng) {
	m_pResMng = pResMng;
}
