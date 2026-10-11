// Original: d:\build\custom\temp\code\client\projectg\inifile.cpp
#include "INIFile.h"
#include "wresrcmng.h"

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <string>

// westpak.cpp
void __cdecl CloseCFile(cFile* file);

extern char g_executeDirectory[];

// Result of ReadString.
static char s_readBuffer[1024];
// Full path of the current .ini file.
// inifile.cpp:15
static std::string s_filePath;

// inifile.cpp:19
CIniFile::CIniFile(void) {
}

// inifile.cpp:22
CIniFile::CIniFile(const char* filename) {
	Create(filename, IO_READ);
}

// inifile.cpp:28
CIniFile::~CIniFile(void) {
}

// inifile.cpp:34
bool CIniFile::Create(const char* filename, IO_MODE mode) {
	char path[MAX_PATH];
	cFile* file = NULL;
	if (mode == IO_READ) {
		file = g_resrcmng->GetCFile(filename, -1);
		if (file == NULL) {
			return false;
		}
	}
	if (mode == IO_WRITE) {
		s_filePath = g_executeDirectory;
		s_filePath += "\\";
		s_filePath += filename;
		WritePrivateProfileStringA(NULL, NULL, NULL, s_filePath.c_str());
	} else {
		CloseCFile(file);
		s_filePath = g_executeDirectory;
		s_filePath += "\\";
		s_filePath += filename;
	}
	return true;
}

// inifile.cpp:72
const char* CIniFile::ReadString(const char* section, const char* key) {
	if (s_filePath.length() > 0) {
		memset(s_readBuffer, 0, sizeof(s_readBuffer));
		GetPrivateProfileStringA(section, key, "OCCUR_ERROR", s_readBuffer, sizeof(s_readBuffer), s_filePath.c_str());
		if (memcmp(s_readBuffer, "OCCUR_ERROR", sizeof("OCCUR_ERROR")) != 0) {
			return s_readBuffer;
		}
	}
	return "";
}

// inifile.cpp:93
int CIniFile::ReadInteger(const char* section, const char* key) {
	if (s_filePath.length() > 0) {
		int value = GetPrivateProfileIntA(section, key, -3432, s_filePath.c_str());
		if (value != -3432) {
			return value;
		}
	}
	return INI_ERROR;
}

// inifile.cpp:114
int CIniFile::WriteString(const char* section, const char* key, const char* value) {
	if (s_filePath.length() > 0) {
		return WritePrivateProfileStringA(section, key, value, s_filePath.c_str()) ? 1 : INI_ERROR;
	}
	return INI_ERROR;
}

// inifile.cpp:137
int CIniFile::WriteInteger(const char* section, const char* key, int value) {
	if (s_filePath.length() > 0) {
		char text[20] = { 0 };
		itoa(value, text, 10);
		return WritePrivateProfileStringA(section, key, text, s_filePath.c_str()) ? 1 : INI_ERROR;
	}
	return INI_ERROR;
}
