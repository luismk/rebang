// Original: d:\build\custom\temp\code\client\projectg\uwin.cpp
#include "UWIN.h"

#include <process.h>
#include <ctype.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mbstring.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <wchar.h>
#include <tlhelp32.h>
#include <iphlpapi.h>

namespace UWIN {

// uwin.cpp:32
unsigned int GetFileSize(const char* filename) {
	struct _stat fileStat;
	int result = _stat(filename, &fileStat);
	return result == 0 ? fileStat.st_size : 0;
}

// uwin.cpp:41
void GenerateCurrentFullPath(const char* filename, char* fullPath) {
	if (filename != NULL && fullPath != NULL) {
		GetCurrentDirectory(MAX_PATH, fullPath);
		strcat(fullPath, "\\");
		strcat(fullPath, filename);
	}
}

// uwin.cpp:55
void DeleteReadOnlyFile(const char* filename) {
	if (filename != NULL) {
		DWORD attributes = GetFileAttributes(filename);
		SetFileAttributes(filename, attributes & ~FILE_ATTRIBUTE_READONLY);
		DeleteFile(filename);
	}
}

// uwin.cpp:69
void DeleteReadOnlyFiles(const char* pattern) {
	if (pattern != NULL) {
		WIN32_FIND_DATA findData;
		HANDLE hFind = FindFirstFile(pattern, &findData);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				DeleteReadOnlyFile(findData.cFileName);
			} while (FindNextFile(hFind, &findData));
		}
	}
}

// uwin.cpp:91
int IsDots(const char* name) {
	if (_mbscmp((const unsigned char*)name, (const unsigned char*)".") != 0 && _mbscmp((const unsigned char*)name, (const unsigned char*)"..") != 0)
		return FALSE;

	return TRUE;
}

// uwin.cpp:109
int DeleteDirectory(const char* path) {
	HANDLE hFind;
	WIN32_FIND_DATA findData;
	char dirPath[MAX_PATH];
	char fileName[MAX_PATH];

	strcpy(dirPath, path);
	strcat(dirPath, "\\*");
	strcpy(fileName, path);
	strcat(fileName, "\\");
	hFind = FindFirstFile(dirPath, &findData);
	if (hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	strcpy(dirPath, fileName);

	bool searching = true;
	while (searching) {
		if (FindNextFile(hFind, &findData)) {
			if (IsDots(findData.cFileName))
				continue;

			strcat(fileName, findData.cFileName);
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (!DeleteDirectory(fileName)) {
					FindClose(hFind);
					return FALSE;
				}
				RemoveDirectory(fileName);
				strcpy(fileName, dirPath);
			} else {
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
					_chmod(fileName, _S_IWRITE);

				if (!DeleteFile(fileName)) {
					FindClose(hFind);
					return FALSE;
				}
				strcpy(fileName, dirPath);
			}
		} else {
			if (GetLastError() == ERROR_NO_MORE_FILES) {
				searching = false;
			} else {
				FindClose(hFind);
				return FALSE;
			}
		}
	}
	FindClose(hFind);

	return RemoveDirectory(path);
}

// uwin.cpp:212
void SetWindowVisible(HWND hWnd, bool visible) {
	LONG style = GetWindowLong(hWnd, GWL_STYLE);

	if (visible)
		style |= WS_VISIBLE;
	else
		style &= ~WS_VISIBLE;

	SetWindowLong(hWnd, GWL_STYLE, style);
}

// uwin.cpp:243
bool GetExeFilename(HWND hWnd, char* filename) {
	if (hWnd == NULL)
		return false;

	bool found = false;
	DWORD processId = 0;
	GetWindowThreadProcessId(hWnd, &processId);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32* pEntry = new PROCESSENTRY32;
	pEntry->dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, pEntry)) {
		do {
			if (pEntry->th32ProcessID == processId) {
				if (stricmp(pEntry->szExeFile, "[System Process]") != 0) {
					char* name = strrchr(pEntry->szExeFile, '\\');

					if (name != NULL)
						strcpy(filename, name + 1);
					else
						strcpy(filename, pEntry->szExeFile);

					found = true;
				}
				break;
			}
		} while (Process32Next(hSnapshot, pEntry));
	}

	delete pEntry;
	CloseHandle(hSnapshot);
	return found;
}

// uwin.cpp:294
bool FindProcessByModuleName(const char* moduleName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	bool found = false;

	PROCESSENTRY32* pEntry = new PROCESSENTRY32;
	pEntry->dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, pEntry)) {
		do {
			char* name = strrchr(pEntry->szExeFile, '\\');
			if (name != NULL)
				name++;
			else
				name = pEntry->szExeFile;

			if (stricmp(name, moduleName) == 0) {
				found = true;
				break;
			}
		} while (Process32Next(hSnapshot, pEntry));
	}

	delete pEntry;
	CloseHandle(hSnapshot);
	return found;
}

// uwin.cpp:397
void* BeginThreadEx(unsigned int (__stdcall* startAddress)(void*), void* arg) {
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, startAddress, arg, 0, NULL);
	CloseHandle(hThread);
	return hThread;
}

// uwin.cpp:437
int axtoi(char* hex) {
	int value = 0;

	while (*hex != '\0') {
		char digit = *hex++;
		if (iswdigit(digit))
			digit -= '0';
		else if (digit >= 'A' && digit <= 'F')
			digit -= 'A' - 10;
		else if (digit >= 'a' && digit <= 'f')
			digit -= 'a' - 10;
		else
			digit = 0;

		value = value * 16 + digit;
	}

	return value;
}

// uwin.cpp:459
int IP2Integer(const char* ip) {
	int parts[4];
	char number[4];
	char address[20];

	int length = strlen(ip);

	if (length < 16) {
		memcpy(address, ip, length);

		address[length++] = '.';
		address[length] = '\0';

		int digits = 0;
		int part = 0;
		for (int i = 0; i < length; i++) {
			if (address[i] != '.') {
				number[digits++] = address[i];
			} else {
				number[digits] = '\0';
				parts[part++] = atoi(number);
				digits = 0;
			}
		}

		return ((parts[0] << 8 | parts[1]) << 8 | parts[2]) << 8 | parts[3];
	}

	return -1;
}

// uwin.cpp:535
void AlterChar(std::string& str, char from, char to) {
	char* p = (char*)str.c_str();

	for (unsigned int i = 0; i < str.size(); i++) {
		if (p[i] == from)
			p[i] = to;
	}
}

// uwin.cpp:686
int GetLocalMacAddress(char* buffer, int size, char separator) {
	if (buffer == NULL && size < 20)
		return 0;

	IP_ADAPTER_INFO* pAdapterInfo = (IP_ADAPTER_INFO*)new char[sizeof(IP_ADAPTER_INFO)];
	ULONG bufferLength = sizeof(IP_ADAPTER_INFO);
	int count = 0;

	if (pAdapterInfo != NULL) {
		DWORD result = GetAdaptersInfo(pAdapterInfo, &bufferLength);

		if (result == ERROR_BUFFER_OVERFLOW) {
			delete[] (char*)pAdapterInfo;
			pAdapterInfo = (IP_ADAPTER_INFO*)new char[bufferLength];
			result = GetAdaptersInfo(pAdapterInfo, &bufferLength);
		}

		if (result == NO_ERROR) {
			int length = 0;
			buffer[0] = '\0';
			IP_ADAPTER_INFO* pAdapter = pAdapterInfo;
			while (pAdapter != NULL) {
				char address[32];
				sprintf(address, "%02x%c%02x%c%02x%c%02x%c%02x%c%02x",
					pAdapter->Address[0], separator,
					pAdapter->Address[1], separator,
					pAdapter->Address[2], separator,
					pAdapter->Address[3], separator,
					pAdapter->Address[4], separator,
					pAdapter->Address[5]);

				length += strlen(address);

				if (length >= size)
					break;

				strcat(buffer, address);
				count++;
				pAdapter = pAdapter->Next;
			}
		}

		if (pAdapterInfo != NULL)
			delete[] (char*)pAdapterInfo;
	}

	return count;
}

} // namespace UWIN
