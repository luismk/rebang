// Original: d:\build\custom\temp\code\client\projectg\uwin.h
#ifndef PG_CLIENT_PROJECTG_UWIN_H
#define PG_CLIENT_PROJECTG_UWIN_H

#include <windows.h>
#include <string>

// Small Win32 helpers (files, windows, processes, network).
namespace UWIN {
	unsigned int GetFileSize(const char* filename);
	void GenerateCurrentFullPath(const char* filename, char* fullPath);
	void DeleteReadOnlyFile(const char* filename);
	void DeleteReadOnlyFiles(const char* pattern);
	void SetWindowVisible(HWND hWnd, bool visible);
	bool GetExeFilename(HWND hWnd, char* filename);
	bool FindProcessByModuleName(const char* moduleName);
	void* BeginThreadEx(unsigned int (__stdcall* startAddress)(void*), void* arg);
	int axtoi(char* hex);
	int IP2Integer(const char* ip);
	void AlterChar(std::string& str, char from, char to);
	int GetLocalMacAddress(char* buffer, int index, char separator);
	int IsDots(const char* name);
	int DeleteDirectory(const char* path);
}

#endif // PG_CLIENT_PROJECTG_UWIN_H
