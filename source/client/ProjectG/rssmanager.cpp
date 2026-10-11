// Original: d:\build\custom\temp\code\client\projectg\rssmanager.cpp
#include <windows.h>
#include <process.h>

#include <stdio.h>
#include <string.h>

#include "RSSManager.h"
#include "tinyxml.h"

// wininet.h is not in the toolchain's Platform SDK: the imports used here,
// with their SDK signatures (the same block as cHttp.h).
#ifndef INTERNET_FLAG_NO_CACHE_WRITE
typedef LPVOID HINTERNET;
#define INTERNET_FLAG_RELOAD 0x80000000
#define INTERNET_FLAG_NO_CACHE_WRITE 0x04000000
extern "C" {
__declspec(dllimport) HINTERNET WINAPI InternetOpenA(LPCSTR agent, DWORD accessType, LPCSTR proxy, LPCSTR proxyBypass, DWORD flags);
__declspec(dllimport) HINTERNET WINAPI InternetOpenUrlA(HINTERNET internet, LPCSTR url, LPCSTR headers, DWORD headersLength, DWORD flags, DWORD_PTR context);
__declspec(dllimport) BOOL WINAPI InternetQueryDataAvailable(HINTERNET file, LPDWORD bytesAvailable, DWORD flags, DWORD_PTR context);
__declspec(dllimport) BOOL WINAPI InternetReadFile(HINTERNET file, LPVOID buffer, DWORD bytesToRead, LPDWORD bytesRead);
__declspec(dllimport) BOOL WINAPI InternetCloseHandle(HINTERNET internet);
}
#endif

unsigned __stdcall RSSThreadFunc(void *param);

// rssmanager.cpp:17
bool LoadXMLVer(const char *fileName, char *ver, int size)
{
	TiXmlDocument doc;
	if (doc.LoadFile(fileName)) {
		TiXmlNode *pRss = doc.FirstChild("rss");
		if (pRss) {
			TiXmlNode *pNode = pRss->FirstChild("ver");
			if (pNode)
				pNode = pNode->FirstChild();

			if (pNode == 0)
				return false;

			const char *text = pNode->Value();
			strncpy(ver, text, strlen(text));

			return true;
		}
	}

	return false;
}

// rssmanager.cpp:54
bool GetUrlFile(const char *url, const char *fileName)
{
	HINTERNET hInternet = InternetOpenA("Daemon", 0, NULL, NULL, 0);
	if (hInternet == NULL)
		return false;

	HINTERNET hUrl = InternetOpenUrlA(hInternet, url, NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if (hUrl == NULL) {
		InternetCloseHandle(hInternet);
		return false;
	}

	HANDLE hFile = CreateFileA(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		InternetCloseHandle(hInternet);
		InternetCloseHandle(hUrl);
		return false;
	}

	DWORD available = 0;
	DWORD read = 0;
	DWORD written = 0;
	char buffer[0x2800];

	do {
		InternetQueryDataAvailable(hUrl, &available, 0, 0);
		InternetReadFile(hUrl, buffer, available, &read);
		WriteFile(hFile, buffer, read, &written, NULL);
	} while (read != 0);

	CloseHandle(hFile);

	InternetCloseHandle(hInternet);
	InternetCloseHandle(hUrl);

	return true;
}

// rssmanager.cpp:107
unsigned __stdcall RSSThreadFunc(void *param)
{
	RSSManager *pThis = (RSSManager *)param;
	char url[256] = { 0 };

	sprintf(url, "%s/NoticeRSS.aspx?gIdx=%d\n", "http://qa.club.pangya.gametree.co.kr/InGame", pThis->m_guildIdx);

	if (GetUrlFile(url, "guildlast.xml")) {
		pThis->CallBackFunction(0);
	} else {
		pThis->m_bNewRss = false;
		pThis->m_bComplete = false;
	}

	return 0;
}

// rssmanager.cpp:135
RSSManager::RSSManager(void)
{
	m_bNewRss = false;
	m_bComplete = false;
}

// rssmanager.cpp:141
RSSManager::~RSSManager(void)
{
}

// rssmanager.cpp:150
void RSSManager::GetRSSFile(unsigned long guildIdx, void (__fastcall *callBack)(int))
{
	if (guildIdx > 0) {
		unsigned threadId;
		m_guildIdx = guildIdx;
		m_pfnCallBack = callBack;
		uintptr_t thread = _beginthreadex(NULL, 0, RSSThreadFunc, this, 0, &threadId);
		if (thread != (uintptr_t)-1) {
			CloseHandle((HANDLE)thread);
		}
	}
}

// rssmanager.cpp:174
void RSSManager::GetRSSFile_JP(unsigned long guildIdx, void (__fastcall *callBack)(int), const char *param1, const char *param2)
{
	if (guildIdx > 0) {
		unsigned threadId;
		m_guildIdx = guildIdx;
		m_pfnCallBack = callBack;
		m_strJpParam1 = param1;
		m_strJpParam2 = param2;
		uintptr_t thread = _beginthreadex(NULL, 0, RSSThreadFunc, this, 0, &threadId);
		if (thread != (uintptr_t)-1) {
			CloseHandle((HANDLE)thread);
		}
	}
}

// rssmanager.cpp:203
void RSSManager::CallBackFunction(int result)
{
	m_bNewRss = false;
	m_bComplete = false;

	if (result == 0) {
		char lastVer[100] = { 0 };
		char newsVer[100] = { 0 };

		if (LoadXMLVer("guildlast.xml", lastVer, 100)) {
			if (!LoadXMLVer("guildnews.xml", newsVer, 100)) {
				DeleteFileA("guildnews.xml");
				rename("guildlast.xml", "guildnews.xml");
			} else if (stricmp(lastVer, newsVer) == 0) {
				DeleteFileA("guildnews.xml");
				rename("guildlast.xml", "guildnews.xml");
				m_bComplete = true;
				m_pfnCallBack(4);
				return;
			} else {
				DeleteFileA("guildnews.xml");
				rename("guildlast.xml", "guildnews.xml");
				m_pfnCallBack(3);
			}

			m_bComplete = true;
			m_bNewRss = true;
		}
	}
}

// rssmanager.cpp:274
bool RSSManager::IsNewRss(void)
{
	return m_bNewRss;
}

// rssmanager.cpp:281
const char *RSSManager::LoadFirstId(const char *fileName)
{
	TiXmlDocument doc;
	if (doc.LoadFile(fileName)) {
		TiXmlNode *pRss = doc.FirstChild("rss");
		if (pRss) {
			TiXmlNode *pNode = pRss->FirstChild("notice");
			if (pNode)
				pNode = pNode->FirstChild();

			if (pNode == 0)
				return 0;

			const char *id = pNode->Value();

			return id;
		}
	}

	return 0;
}

// rssmanager.cpp:318
void RSSManager::ResetVars(void)
{
	m_bNewRss = false;
	m_bComplete = false;
}
