// Original: d:\build\custom\temp\code\client\wangreal\source\westpak.cpp
#include "../include/westpak.h"
#include "../include/wlist.h"
#include "../include/wutil.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

// <io.h> of the VC7 CRT (not in externs/vs-2003/Vc7/include): the declarations used here.
extern "C" {
__declspec(dllimport) int __cdecl _close(int fh);
__declspec(dllimport) long __cdecl _lseek(int fh, long offset, int origin);
__declspec(dllimport) int __cdecl _read(int fh, void* buffer, unsigned int count);
__declspec(dllimport) long __cdecl _tell(int fh);
__declspec(dllimport) int __cdecl _open(const char* filename, int oflag, ...);
__declspec(dllimport) int __cdecl open(const char* filename, int oflag, ...);
__declspec(dllimport) int __cdecl _access(const char* path, int mode);
// <direct.h>
__declspec(dllimport) int __cdecl mkdir(const char* dirname);
__declspec(dllimport) int __cdecl _chdir(const char* dirname);
}

// <fcntl.h>
#define _O_RDONLY 0x0000
#define _O_BINARY 0x8000

// Stand-ins for Min/Max with the comparisons of rebang's wminmax.h (a > b ? b : a,
// a < b ? b : a), which these functions' branch layout needs; wutil.h's Min/Max
// (a <= b ? a : b, a >= b ? a : b) invert the branches. Drop these once the shared
// header's Min/Max take that form.
template <class T> T WestpakMin(T a, T b) { return a > b ? b : a; }
template <class T> T WestpakMax(T a, T b) { return a < b ? b : a; }

// The XTEA key of the pak index (type 0x20 entries), per regional client.
// The original is the KR client; the CMake build reads another region's paks
// with PG_PAK_REGION (CMakeLists.txt, docs/PAK_FORMAT.md). The default must
// stay KR: the matching build defines none of these.
#if defined(PG_PAK_REGION_EU)
#define PAK_KEY0 0x1E986D8
#define PAK_KEY1 0x5818479
#define PAK_KEY2 0x3D2B0BB
#define PAK_KEY3 0x2C9B030
#elif defined(PG_PAK_REGION_JP)
#define PAK_KEY0 0x20A5FD4
#define PAK_KEY1 0x1EEBDFF
#define PAK_KEY2 0x2B3C6A0
#define PAK_KEY3 0x4F6A3E1
#elif defined(PG_PAK_REGION_TH)
#define PAK_KEY0 0x50AD33B
#define PAK_KEY1 0x0BAFF09
#define PAK_KEY2 0x452FFDA
#define PAK_KEY3 0x2CB4422
#elif defined(PG_PAK_REGION_US)
#define PAK_KEY0 0x3F607A9
#define PAK_KEY1 0x36F5A3E
#define PAK_KEY2 0x11002B4
#define PAK_KEY3 0x4AB00EA
#elif defined(PG_PAK_REGION_ID)
#define PAK_KEY0 0x1640DB7
#define PAK_KEY1 0x1455A9B
#define PAK_KEY2 0x27F1AB7
#define PAK_KEY3 0x5918B54
#elif defined(PG_PAK_REGION_BR)
#define PAK_KEY0 0x165C26F
#define PAK_KEY1 0x14936E3
#define PAK_KEY2 0x27F1FB7
#define PAK_KEY3 0x5A9601C
#else // KR, the original
#define PAK_KEY0 0x485B576
#define PAK_KEY1 0x5148E02
#define PAK_KEY2 0x5141D96
#define PAK_KEY3 0x28FA9D6
#endif

// shared/tea.cpp
void Decipher(const unsigned int* const v, unsigned int* const w, const unsigned int* const k);

cFile* __cdecl GetCFile(const char* filename, int len, int pakonly);
void __cdecl HookPAKFile(cFile* fp);

// An index entry of a pak: the type, the next entry of the hash chain, where
// the packed data sits and the name, which runs past the end of the record.
struct sPakinfo {
	char type;         // 0x0
	short next;        // 0x2
	long offset;       // 0x4
	long packedsize;   // 0x8
	long size;         // 0xc
	char filename[1];  // 0x10
};
CHECK_SIZE(sPakinfo, 0x14);
CHECK_FIELD(sPakinfo, type, 0x0);
CHECK_FIELD(sPakinfo, next, 0x2);
CHECK_FIELD(sPakinfo, offset, 0x4);
CHECK_FIELD(sPakinfo, packedsize, 0x8);
CHECK_FIELD(sPakinfo, size, 0xc);
CHECK_FIELD(sPakinfo, filename, 0x10);

// An opened pak: its file, the base offset, the name hash and the index.
struct sPak {
	long num;             // 0x0
	cFile* fp;            // 0x4
	long offset;          // 0x8
	short hash[256];      // 0xc
	void* lock;           // 0x20c
	sPakinfo** list;      // 0x210
	sPak* next;           // 0x214
};
CHECK_SIZE(sPak, 0x218);
CHECK_FIELD(sPak, num, 0x0);
CHECK_FIELD(sPak, fp, 0x4);
CHECK_FIELD(sPak, offset, 0x8);
CHECK_FIELD(sPak, hash, 0xc);
CHECK_FIELD(sPak, lock, 0x20c);
CHECK_FIELD(sPak, list, 0x210);
CHECK_FIELD(sPak, next, 0x214);

// A file mapped into memory.
class cFileMap : public cFile {
	LAYOUT_CHECK_FRIEND;
public:
	cFileMap();
	virtual ~cFileMap();
	virtual bool Open(const char* filename);
	virtual int Read(void* buffer, int size);
	virtual int GetByte();
	virtual void Seek(int offset, int origin);
	virtual int Tell();

protected:
	void* m_hFile;  // 0x8
	void* m_hMap;   // 0xc
	char* m_pBuff;  // 0x10
	int m_offset;   // 0x14
};
CHECK_SIZE(cFileMap, 0x18);
BEGIN_LAYOUT_CHECK(cFileMap)
	CHECK_FIELD(cFileMap, m_hFile, 0x8);
	CHECK_FIELD(cFileMap, m_hMap, 0xc);
	CHECK_FIELD(cFileMap, m_pBuff, 0x10);
	CHECK_FIELD(cFileMap, m_offset, 0x14);
END_LAYOUT_CHECK

// A file opened with _open, read through the CRT's low-level calls.
class cFileDefault : public cFile {
	LAYOUT_CHECK_FRIEND;
public:
	cFileDefault(int fhl);
	virtual ~cFileDefault();
	virtual int Read(void* buffer, int size);
	virtual int GetByte();
	virtual void Seek(int offset, int origin);
	virtual int Tell();

private:
	int fhl;  // 0x8
};
CHECK_SIZE(cFileDefault, 0xc);
BEGIN_LAYOUT_CHECK(cFileDefault)
	CHECK_FIELD(cFileDefault, fhl, 0x8);
END_LAYOUT_CHECK

// A whole file copied into memory.
class cFileMemory : public cFile {
	LAYOUT_CHECK_FRIEND;
public:
	cFileMemory(cFile* file);
	virtual ~cFileMemory();
	virtual int Read(void* buffer, int size);
	virtual int GetByte();
	virtual void Seek(int offset, int origin);
	virtual int Tell();

private:
	char* buffer;  // 0x8
	int offset;    // 0xc
};
CHECK_SIZE(cFileMemory, 0x10);
BEGIN_LAYOUT_CHECK(cFileMemory)
	CHECK_FIELD(cFileMemory, buffer, 0x8);
	CHECK_FIELD(cFileMemory, offset, 0xc);
END_LAYOUT_CHECK

// A stored (uncompressed) entry of a pak, read through a block buffer.
class cFilePacked : public cFile {
	LAYOUT_CHECK_FRIEND;
public:
	cFilePacked(sPak* pak, sPakinfo* pakinfo, int bufsize);
	virtual ~cFilePacked();
	virtual int Read(void* buffer, int size);
	virtual int GetByte();
	virtual void Seek(int offset, int origin);
	virtual int Tell();

private:
	void ReadBlock();

	char* buffer;        // 0x8
	int current;         // 0xc
	int pivot;           // 0x10
	int trace;           // 0x14
	int size;            // 0x18
	int offset;          // 0x1c
	int bufsize;         // 0x20
	sPak* pak;           // 0x24
	sPakinfo* pakinfo;   // 0x28
};
CHECK_SIZE(cFilePacked, 0x2c);
BEGIN_LAYOUT_CHECK(cFilePacked)
	CHECK_FIELD(cFilePacked, buffer, 0x8);
	CHECK_FIELD(cFilePacked, current, 0xc);
	CHECK_FIELD(cFilePacked, pivot, 0x10);
	CHECK_FIELD(cFilePacked, trace, 0x14);
	CHECK_FIELD(cFilePacked, size, 0x18);
	CHECK_FIELD(cFilePacked, offset, 0x1c);
	CHECK_FIELD(cFilePacked, bufsize, 0x20);
	CHECK_FIELD(cFilePacked, pak, 0x24);
	CHECK_FIELD(cFilePacked, pakinfo, 0x28);
END_LAYOUT_CHECK

// An LZ-compressed entry of a pak, decoded block by block.
class cFileLZ : public cFilePacked {
	LAYOUT_CHECK_FRIEND;
public:
	cFileLZ(sPak* pak, sPakinfo* pakinfo, int bufsize);
	virtual int Read(void* buffer, int size);
	virtual int GetByte();
	virtual void Seek(int offset, int origin);
	virtual int Tell();

protected:
	virtual void ReadBlock();

	int current;                    // 0x2c
	int offset;                     // 0x30
	int trace;                      // 0x34
	unsigned char buffer[8192];     // 0x38
};
CHECK_SIZE(cFileLZ, 0x2038);
BEGIN_LAYOUT_CHECK(cFileLZ)
	CHECK_FIELD(cFileLZ, current, 0x2c);
	CHECK_FIELD(cFileLZ, offset, 0x30);
	CHECK_FIELD(cFileLZ, trace, 0x34);
	CHECK_FIELD(cFileLZ, buffer, 0x38);
END_LAYOUT_CHECK

// An entry of a pak compressed with the second LZ variant.
class cFileLZ2 : public cFileLZ {
public:
	cFileLZ2(sPak* pak, sPakinfo* pakinfo, int bufsize);

private:
	virtual void ReadBlock();
};
CHECK_SIZE(cFileLZ2, 0x2038);

sPak* pak = NULL;
static char logname[128];
static char clonedirectory[128];
// The keys cFileLZ2 scrambles its back references with.
static unsigned int secutable[8] = { 0xff21, 0x834f, 0x675f, 0x34, 0xf237, 0x815f, 0x4765, 0x233 };

// westpak.cpp:77
void __cdecl EnumFileList(WList<char*>* list) {
	WList<char*> mul(4096, 128);

	for (sPak* s = pak; s; s = s->next) {
		for (int i = 0; i < s->num; i++) {
			if (s->list[i]->type == 2)
				continue;

			if (mul.Find(s->list[i]->filename))
				continue;

			char* fname = new char[strlen(s->list[i]->filename) + 1];
			strcpy(fname, s->list[i]->filename);
			*list += fname;
			mul.AddItem(fname, fname);
		}
	}
}

// westpak.cpp:155
cFileLZ2::cFileLZ2(sPak* pak, sPakinfo* pakinfo, int bufsize) : cFileLZ(pak, pakinfo, bufsize) {}

// westpak.cpp:211
cFileMap::cFileMap() : m_hFile(NULL), m_hMap(NULL), m_pBuff(NULL), m_offset(0) {}

// A plain file read through a block buffer.
class cFileRaw : public cFile {
	LAYOUT_CHECK_FRIEND;
public:
	cFileRaw(int fhl, int bufsize);
	virtual ~cFileRaw();
	virtual int Read(void* buffer, int size);
	virtual int GetByte();
	virtual void Seek(int offset, int origin);
	virtual int Tell();

private:
	void ReadBlock();

	int current;    // 0x8
	int offset;     // 0xc
	int trace;      // 0x10
	int bufsize;    // 0x14
	char* buffer;   // 0x18
	int fhl;        // 0x1c
};
CHECK_SIZE(cFileRaw, 0x20);
BEGIN_LAYOUT_CHECK(cFileRaw)
	CHECK_FIELD(cFileRaw, current, 0x8);
	CHECK_FIELD(cFileRaw, offset, 0xc);
	CHECK_FIELD(cFileRaw, trace, 0x10);
	CHECK_FIELD(cFileRaw, bufsize, 0x14);
	CHECK_FIELD(cFileRaw, buffer, 0x18);
	CHECK_FIELD(cFileRaw, fhl, 0x1c);
END_LAYOUT_CHECK

// westpak.cpp:215
cFileMap::~cFileMap() {
	if (m_pBuff) {
		UnmapViewOfFile(m_pBuff);
		m_pBuff = NULL;
	}
	if (m_hMap) { CloseHandle(m_hMap); m_hMap = NULL; }
	if (m_hFile) { CloseHandle(m_hFile); m_hFile = NULL; }
}

// westpak.cpp:227
bool cFileMap::Open(const char* pFilename) {
	m_hFile = CreateFileA(pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_hFile == INVALID_HANDLE_VALUE)
		return false;

	m_nLen = GetFileSize(m_hFile, NULL);
	m_hMap = CreateFileMappingA(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (m_hMap != NULL && GetLastError() == ERROR_INVALID_ORDINAL) {
		CloseHandle(m_hMap);
		m_hMap = NULL;
	}

	if (m_hMap) {
		m_pBuff = (char*)MapViewOfFile(m_hMap, FILE_MAP_READ, 0, 0, 0);
		m_offset = 0;
	}

	return true;
}

// westpak.cpp:252
int cFileMap::Read(void* pDest, int size) {
	if (m_pBuff) {
		if (m_nLen - m_offset <= size) size = m_nLen - m_offset;
		if (size > 0) {
			memcpy(pDest, m_pBuff + m_offset, size);
			m_offset += size;
			return size;
		}
	}
	return 0;
}

// westpak.cpp:267
int cFileMap::GetByte() {
	if (m_pBuff) {
		if (m_offset < m_nLen && m_offset >= 0)
			return (unsigned char)m_pBuff[m_offset++];
	}
	return -1;
}

// westpak.cpp:277
void cFileMap::Seek(int pos, int origin) {
	switch (origin) {
	case SEEK_SET: m_offset = pos; break;
	case SEEK_CUR: m_offset += pos; break;
	case SEEK_END: m_offset = m_nLen + pos; break;
	}
}

// westpak.cpp:287
int cFileMap::Tell() {
	return m_offset;
}

// westpak.cpp:362
cFileDefault::cFileDefault(int fhl) : fhl(fhl) {}
// westpak.cpp:363
cFileDefault::~cFileDefault() { _close(fhl); }
// westpak.cpp:365
int cFileDefault::Read(void* data, int size) { return _read(fhl, data, size); }
// westpak.cpp:366
int cFileDefault::GetByte() { unsigned char c; if (Read(&c, 1)) return c; return -1; }
// westpak.cpp:367
void cFileDefault::Seek(int offset, int origin) { _lseek(fhl, offset, origin); }
// westpak.cpp:368
int cFileDefault::Tell() { return _tell(fhl); }

// westpak.cpp:383
cFileMemory::cFileMemory(cFile* fp) {
	m_nLen = fp->Length();
	buffer = new char[fp->Length()];
	fp->Read(buffer, fp->Length());
	offset = 0;
	delete fp;
}

// westpak.cpp:393
cFileMemory::~cFileMemory() {
	delete[] buffer;
}

// westpak.cpp:398
int cFileMemory::Read(void* data, int size) {
	if (offset + size > m_nLen)
		size = m_nLen - offset;

	if (size) {
		memcpy(data, buffer + offset, size);
		offset += size;
	}
	return size;
}

// westpak.cpp:411
int cFileMemory::GetByte() {
	return offset < m_nLen ? (unsigned char)buffer[offset++] : -1;
}

// westpak.cpp:416
void cFileMemory::Seek(int pos, int origin) {
	switch (origin) {
	case SEEK_CUR:
		offset += pos;
		break;

	case SEEK_SET:
		offset = pos;
		break;

	case SEEK_END:
		offset = m_nLen + pos;
		break;
	}
}

// westpak.cpp:432
int cFileMemory::Tell() {
	return offset;
}

// <new>: the array form is declared from here on (the earlier new[] call operator new).
void* __cdecl operator new[](unsigned int);

// westpak.cpp:573
static sPak* OpenPAKFILE2(cFile* fp) {
	int len = 0;
	if (!fp)
		return NULL;

	sPak* s = new sPak;

	s->lock = CreateEventA(NULL, FALSE, TRUE, NULL);

	for (int slot = 0; slot < 256; slot++)
		s->hash[slot] = -1;

	fp->Seek(-9, SEEK_END);

	int offset;
	fp->Read(&offset, 4);
	fp->Read(&s->num, 4);

	if (s->num <= 0) {
		delete s;
		return NULL;
	}

	s->list = new sPakinfo*[s->num];
	s->list[0] = (sPakinfo*)new char[fp->Tell() - offset + s->num * 6 - 8];

	fp->Seek(offset, SEEK_SET);

	offset = 0;
	int i = 0;
	if (s->num > 0) {
		do {
			s->list[i] = (sPakinfo*)((char*)s->list[0] + offset);

			fp->Read(&len, 1);
			fp->Read(s->list[i], 1);

			if (!(s->list[i]->type & 0xf0))
				s->list[i]->type |= 0x10;

			switch (s->list[i]->type & 0xf0) {
			case 0x10: {
				fp->Read(&s->list[i]->offset, 4);
				fp->Read(&s->list[i]->packedsize, 4);
				fp->Read(&s->list[i]->size, 4);
				fp->Read(s->list[i]->filename, len + 1);

				s->list[i]->size ^= 0x71;

				for (int j = 0; j < len; j++)
					s->list[i]->filename[j] ^= 0x71;

				int h = ((strlen(s->list[i]->filename) & 0xf) << 4) + (s->list[i]->filename[0] & 0xf);

				s->list[i]->next = s->hash[h];
				s->hash[h] = i;

				offset += len + sizeof(sPakinfo) + 1;
			} break;

			case 0x20: {
				unsigned int k[4];
				unsigned int v[2];
				unsigned int w[2];
				fp->Read(&s->list[i]->offset, 4);
				fp->Read(&s->list[i]->packedsize, 4);
				fp->Read(&s->list[i]->size, 4);
				fp->Read(s->list[i]->filename, len);

				v[0] = s->list[i]->offset; v[1] = s->list[i]->size;

				k[0] = PAK_KEY0; k[1] = PAK_KEY1; k[2] = PAK_KEY2; k[3] = PAK_KEY3; Decipher(v, w, k);
				s->list[i]->offset = w[0];
				s->list[i]->size = w[1];

				for (int kk = 0; kk < len; kk += 8) {
					v[0] = *(unsigned int*)(s->list[i]->filename + kk);
					v[1] = *(unsigned int*)(s->list[i]->filename + kk + 4);
					Decipher(v, (unsigned int*)(s->list[i]->filename + kk), k);
				}

				int h = ((len & 0xf) << 4) + (s->list[i]->filename[0] & 0xf);

				s->list[i]->next = s->hash[h];
				s->hash[h] = i;

				offset += len + sizeof(sPakinfo);
			} break;

			case 0x80: {
				fp->Read(&s->list[i]->offset, 4);
				fp->Read(&s->list[i]->packedsize, 4);
				fp->Read(&s->list[i]->size, 4);
				fp->Read(s->list[i]->filename, len + 1);

				int h = ((strlen(s->list[i]->filename) & 0xf) << 4) + (s->list[i]->filename[0] & 0xf);

				s->list[i]->next = s->hash[h];
				s->hash[h] = i;

				offset += len + sizeof(sPakinfo) + 1;
			} break;
			}
			i++;
		} while (i < s->num);
	}

	s->offset = fp->Tell();
	s->next = NULL;
	s->fp = fp;
	return s;
}

// westpak.cpp:713
void __cdecl HookPAKFile(const char* filename) {
	cFile* fp;
	int fhl = _open(filename, _O_BINARY, 0);

	if (fhl != -1) {
		fp = new cFileDefault(fhl);

		sPak* s = OpenPAKFILE2(fp);

		s->next = pak;
		pak = s;
	} else {
		fp = GetCFile(filename, 0x8000, 0);
		if (fp) HookPAKFile(fp);
	}
}

// westpak.cpp:755
void __cdecl HookPAKFile(cFile* fp) {
	sPak* s = OpenPAKFILE2(fp);

	s->next = pak;
	pak = s;
}

// westpak.cpp:771
static void ReleasePAK(sPak* s) {
	if (s) {
		if (s->next)
			ReleasePAK(s->next);

		delete[] s->list[0];
		delete[] s->list;
		delete s->fp;

		CloseHandle(s->lock);

		delete s;
	}
}

// westpak.cpp:806
void __cdecl ReleasePAK() {
	ReleasePAK(pak);
	pak = NULL;
}

// westpak.cpp:821
void __cdecl MakeLog(char* filename) {
	strcpy(logname, filename);

	FILE* fp = fopen(logname, "wt");
	fprintf(fp, "LogFile (%s %s)\n-------\n", __DATE__, __TIME__);
	fclose(fp);
}

// westpak.cpp:834
static void makedirectory(char* directory) {
	char dir[128];
	char curdir[128];
	char* p;

	GetCurrentDirectoryA(128, curdir);

	for (p = directory; *p; ) {
		p = strchr(p + 1, '\\');
		if (!p)
			p = directory + strlen(directory);
		if (p[-1] == ':')
			continue;

		memcpy(dir, directory, p - directory);
		dir[p - directory] = 0;
		mkdir(dir);
		_chdir(dir);
	}

	_chdir(curdir);
}

// westpak.cpp:857
void ConvertDirectoryMarks(char* pDest, unsigned int nDestLen, const char* pSrc) {
	strcpy(pDest, pSrc);
	for (unsigned int i = 0; pDest[i] && i < nDestLen; i++) {
		if (pDest[i] == '/')
			pDest[i] = '\\';
	}
}

// westpak.cpp:869
void __cdecl ExportFiles(const char* directory) {
	strcpy(clonedirectory, directory);

	if (clonedirectory[strlen(clonedirectory) - 1] != '/' || clonedirectory[strlen(clonedirectory) - 1] != '\\')
		strcat(clonedirectory, "\\");
}

// westpak.cpp:877
static cFile* GetCFileSub(const char* filename, int len, int pakonly) {
	char fname[260];

	if (len < 0) {
		cFile* fp = GetCFile(filename, 0x1000, pakonly);
		return fp ? new cFileMemory(fp) : NULL;
	}

	if (logname[0]) {
		FILE* fp = fopen(logname, "at");
		fprintf(fp, "%s\n", filename);
		fclose(fp);
	}

	for (sPak* s = pak; s; s = s->next) {
		for (int i = 0; i < s->num; i++) {
			if (strcmpi(filename, s->list[i]->filename) == 0) {
				switch (s->list[i]->type & 0xf) {
				case 0:
					return new cFilePacked(s, s->list[i], len);
				case 1:
					return new cFileLZ(s, s->list[i], len);
				case 3:
					return new cFileLZ2(s, s->list[i], len);
				}
			}
		}
	}

	if (pakonly)
		return NULL;

	ConvertDirectoryMarks(fname, 260, filename);

	int fhl;
	if ((fhl = open(fname, _O_RDONLY | _O_BINARY, 0)) != -1)
		return new cFileRaw(fhl, len);

	return NULL;
}

// westpak.cpp:980
cFile* __cdecl GetCFile(const char* filename, int len, int pakonly) {
	char dir[260];
	char fname[260];

	cFile* fp = GetCFileSub(filename, len, pakonly);

	if (fp && clonedirectory[0]) {
		sprintf(fname, "%s%s", clonedirectory, filename);
		while (strchr(fname, '/'))
			*strchr(fname, '/') = '\\';

		if (_access(fname, 0) == -1) {
			strcpy(dir, fname);
			*strrchr(dir, '\\') = 0;
			makedirectory(dir);

			FILE* f = fopen(fname, "wb");
			for (int i = 0; i < fp->Length(); i++)
				fputc(fp->GetByte(), f);
			fclose(f);
		}

		fp->Seek(0, 0);
	}

	return fp;
}

// westpak.cpp:1025
void __cdecl CloseCFile(cFile* file) {
	if (file) {
		delete file;
	}
}

// westpak.cpp:1047
// The original reaches the variadic arguments through the address of the last
// named parameter (`((T**)&fmt)[++arg]`), which only works where the arguments
// are contiguous on the stack. cl 13.10 lays them out that way; arm64 passes
// them in registers, so the port reads them through va_list (docs/PORTABILITY.md).
#if PG_ABI32
#define PG_SCAN_PTR(T) ((T)((void**)&fmt)[++arg])
#else
#define PG_SCAN_PTR(T) ((T)va_arg(ap, void*))
#endif

bool __cdecl cFile::Scan(const char* fmt, ...) {
	int arg = 0;
	char temp[128];
	int c = -1;
	int len = (int)strlen(fmt);
#if !PG_ABI32
	va_list ap;
	va_start(ap, fmt);
#endif

	for (int i = 0; i < len; i++) {
		if (fmt[i] == '%') {
			if (c == -1) {
				c = GetByte();
				if (c == -1)
					return false;
			}

			while (c == 10 || c == 9 || c == 13 || c == 32) {
				c = GetByte();
				if (c == -1)
					return false;
			}

			switch (fmt[i + 1]) {
			case 'X':
			case 'x': {
				int code = 0;
				int flag = 0;
				while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (!flag && c == '-')) {
					if (c == '-')
						flag |= 1;
					else {
						if (c >= 'a')
							c -= 0x20;
						code = code * 16 + (c <= '9' ? c - '0' : c - 'A' + 10);
					}
					c = GetByte();
					if (c == -1)
						return false;
				}
				if (flag)
					code = -code;
				*(int*)PG_SCAN_PTR(void*) = code;
			}
			break;
			case 'F':
			case 'G':
			case 'f':
			case 'g': {
				int code = 0;
				int flag = 0;
				for (; (c >= '0' && c <= '9') || (!code && c == '-') || (!flag && c == '.'); code++) {
					if (c == '.')
						flag = 1;
					temp[code] = (char)c;
					if ((c = GetByte()) == -1) {
						code++;
						break;
					}
				}
				temp[code] = 0;
				float code2 = (float)atof(temp);
				*(int*)PG_SCAN_PTR(void*) = *(int*)&code2;
			}
			break;
			case 'D':
			case 'U':
			case 'd':
			case 'u': {
				int code;
				for (code = 0; (c >= '0' && c <= '9') || (!code && c == '-'); code++) {
					temp[code] = (char)c;
					if ((c = GetByte()) == -1) {
						code++;
						break;
					}
				}
				temp[code] = 0;
				*(int*)PG_SCAN_PTR(void*) = atoi(temp);
			}
			break;
			case 'S':
			case 's': {
				int code = 0;
				char* target = PG_SCAN_PTR(char*);
				for (code = 0; c != 10; code++) {
					if (c == 9 || c == 13 || c == 32 || c == -1)
						break;
					target[code] = (char)c;
					if ((c = GetByte()) == -1) {
						code++;
						break;
					}
				}
				target[code] = 0;
			}
			break;
			case 'N':
			case 'n': {
				int code = 0;
				char* target = PG_SCAN_PTR(char*);
				for (code = 0; c != 10; code++) {
					if (c == 9 || c == 13 || c == -1)
						break;
					target[code] = (char)c;
					if ((c = GetByte()) == -1) {
						code++;
						break;
					}
				}
				target[code] = 0;
			}
			break;
			}
		}
	}

	if (c != -1 && c != 13)
		Seek(-1, 1);

#if !PG_ABI32
	va_end(ap);
#endif
	return true;
}
#undef PG_SCAN_PTR

// westpak.cpp:1200
void cFilePacked::ReadBlock() {
	if (offset < pivot + size) {
		WaitForSingleObject(pak->lock, INFINITE);
		if (pak->offset != offset)
			pak->fp->Seek(offset, SEEK_SET);

		int len = WestpakMin(bufsize, pivot - offset + size);
		int start = bufsize - len;

		current = start;
		trace = offset;
		offset += pak->fp->Read(buffer + start, len);
		pak->offset = offset;
		SetEvent(pak->lock);
	} else {
		current = bufsize;
	}
}

// westpak.cpp:1223
cFilePacked::cFilePacked(sPak* pak_, sPakinfo* pakinfo_, int len) {
	pak = pak_;
	pakinfo = pakinfo_;
	m_nLen = pakinfo->size;
	current = 0;
	trace = pakinfo->offset;
	pivot = pakinfo->offset;
	offset = pakinfo->offset;
	size = pakinfo->packedsize;
	bufsize = pakinfo->packedsize < len ? pakinfo->packedsize : len;
	buffer = new char[bufsize];

	ReadBlock();
}

// westpak.cpp:1239
cFilePacked::~cFilePacked() {
	delete[] buffer;
}

// westpak.cpp:1244
int cFilePacked::GetByte() {
	if (current >= bufsize) {
		if (offset >= size + pivot)
			return -1;
		ReadBlock();
	}

	return (unsigned char)buffer[current++];
}

// westpak.cpp:1256
int cFilePacked::Read(void* data, int len) {
	len = WestpakMin(size - offset + pivot - current + bufsize, len);

	int i = 0;
	if (len > 0) {
		do {
			if (current >= bufsize)
				ReadBlock();

			int n = WestpakMin(len - i, bufsize - current);

			memcpy((char*)data + i, buffer + current, n);

			current += n; i += n;
		} while (i < len);
	}

	return i;
}

// westpak.cpp:1278
void cFilePacked::Seek(int intv, int origin) {
	int noff;

	switch (origin) {
	case SEEK_SET:
		noff = pivot + intv;
		break;

	case SEEK_END:
		noff = size + pivot + intv;
		break;

	case SEEK_CUR:
		noff = offset - bufsize + current + intv;
		break;
	}

	noff = WestpakMax(pivot, WestpakMin(noff, size + pivot));

	if (noff >= trace && noff < offset) {
		current = bufsize - offset + noff;
		return;
	}
	offset = noff;
	trace = noff;
	ReadBlock();
}

// westpak.cpp:1308
int cFilePacked::Tell() {
	return offset - bufsize - pivot + current;
}

// westpak.cpp:1317
cFileLZ::cFileLZ(sPak* pak, sPakinfo* pakinfo, int bufsize) : cFilePacked(pak, pakinfo, bufsize) {
	current = 0;
	offset = 0;
	trace = 0;
}

// westpak.cpp:1324
void cFileLZ::ReadBlock() {
	unsigned short d1;
	unsigned short rest;
	unsigned short d2;

	trace = offset;
	d1 = (unsigned short)cFilePacked::GetByte();
	int i = 8;

	do {
		if (d1 & 1) {
			cFilePacked::Read(&d2, 2);
			unsigned short len = (d2 >> 12) + 2;

			d2 = offset - (d2 & 0xfff);

			while (len > 0) {
				rest = WestpakMin(WestpakMin((int)len, 8192 - (offset & 8191)), 8192 - (d2 & 8191));

				memcpy(&buffer[offset & 8191], &buffer[d2 & 8191], rest);
				offset += rest;
				d2 += rest;
				len -= rest;
			}
		} else {
			buffer[offset & 8191] = (unsigned char)cFilePacked::GetByte(); offset++;
		}
		d1 >>= 1;
		i--;
	} while (i != 0);

	if (offset > m_nLen)
		offset = m_nLen;
}

// westpak.cpp:1364
void cFileLZ2::ReadBlock() {
	unsigned short d1;
	unsigned short len;
	unsigned short d2;
	unsigned short rest;
	int i;

	trace = offset; unsigned int key = cFilePacked::GetByte();

	for (d1 = key ^ 0xc8, i = 0; i < 8; i++, d1 >>= 1) {
		if (d1 & 1) {
			cFilePacked::Read(&d2, 2);

			d2 ^= secutable[(key >> 3) & 7];

			len = (d2 >> 12) + 2; d2 = (unsigned short)offset - (d2 & 0xfff);

			while (len > 0) {
				rest = Min<int>(Min<int>(len, 8192 - (offset & 8191)), 8192 - (d2 & 8191));

				memcpy(&buffer[offset & 8191], &buffer[d2 & 8191], rest);

				offset += rest; d2 += rest;
				len -= rest;
			}
		} else {
			buffer[offset & 8191] = cFilePacked::GetByte(); offset++;
		}
	}

	if (offset > m_nLen)
		offset = m_nLen;
}

// westpak.cpp:1403
int cFileLZ::Read(void* data, int len) {
	len = WestpakMin(m_nLen - current, len);

	int i = 0;
	if (len > 0) {
		do {
			if (current >= offset)
				ReadBlock();

			int n = WestpakMin(WestpakMin(len - i, offset - current), 8192 - (current & 8191));

			memcpy((char*)data + i, buffer + (current & 8191), n);

			current += n; i += n;
		} while (i < len);
	}

	return i;
}

// westpak.cpp:1424
int cFileLZ::GetByte() {
	if (current >= offset) {
		if (current >= m_nLen)
			return -1;
		ReadBlock();
	}

	return buffer[current++ & 8191];
}

// westpak.cpp:1435
void cFileLZ::Seek(int pos, int pivot) {
	int noff;

	switch (pivot) {
	case SEEK_SET:
		noff = pos;
		break;

	case SEEK_END:
		noff = m_nLen + pos;
		break;

	case SEEK_CUR:
		noff = current + pos;
		break;

	default:
		noff = pos;
		break;
	}

	noff = WestpakMax(0, WestpakMin(noff, m_nLen));

	if (noff < trace) {
		current = 0;
		offset = 0;
		cFilePacked::Seek(0, SEEK_SET);
	}

	while (offset < noff)
		ReadBlock();

	current = noff;
}

// westpak.cpp:1469
int cFileLZ::Tell() {
	return current;
}

// westpak.cpp:1478
void cFileRaw::ReadBlock() {
	if (offset < m_nLen) {
		int len = WestpakMin(m_nLen - offset, bufsize);
		current = bufsize - len; trace = offset;

		offset += _read(fhl, buffer + current, len);
	} else {
		current = bufsize;
	}
}

// westpak.cpp:1492
cFileRaw::cFileRaw(int fhl_, int len) {
	fhl = fhl_;

	m_nLen = _lseek(fhl, 0, SEEK_END); current = 0; offset = 0;

	bufsize = WestpakMin(m_nLen, len);
	buffer = new char[bufsize];

	_lseek(fhl, 0, SEEK_SET);

	ReadBlock();
}

// westpak.cpp:1507
cFileRaw::~cFileRaw() {
	if (buffer)
		delete[] buffer;

	_close(fhl);
}

// westpak.cpp:1515
int cFileRaw::GetByte() {
	if (current >= bufsize) {
		if (offset >= m_nLen)
			return -1;
		ReadBlock();
	}

	return (unsigned char)buffer[current++];
}

// westpak.cpp:1531
int cFileRaw::Read(void* data, int len) {
	len = WestpakMin(m_nLen - current - offset + bufsize, len);

	int i = 0;
	while (i < len) {
		if (current >= bufsize)
			ReadBlock();

		int n = WestpakMin(len - i, bufsize - current);

		memcpy((char*)data + i, buffer + current, n);

		current += n; i += n;
	}

	return i;
}

// westpak.cpp:1556
void cFileRaw::Seek(int pos, int origin) {
	int noff;

	switch (origin) {
	case SEEK_SET:
		noff = pos;
		break;

	case SEEK_END:
		noff = m_nLen + pos;
		break;

	case SEEK_CUR:
		noff = offset - bufsize + current + pos;
		break;

	default:
		noff = pos;
		break;
	}

	noff = WestpakMax(0, WestpakMin(noff, m_nLen));

	if (noff >= trace && noff < offset) {
		current = bufsize - offset + noff;
		return;
	}
	_lseek(fhl, noff, SEEK_SET);
	int place = _tell(fhl);
	offset = place;
	trace = place;
	ReadBlock();
}

// westpak.cpp:1587
int cFileRaw::Tell() {
	return offset - bufsize + current;
}
