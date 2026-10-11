#ifndef PG_CLIENT_PROJECTG_CHTTP_H
#define PG_CLIENT_PROJECTG_CHTTP_H

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "..\..\defines.h"

// wininet.h is not in the toolchain's Platform SDK: the imports cHttp uses,
// with their SDK signatures.
#ifndef INTERNET_FLAG_NO_CACHE_WRITE
typedef LPVOID HINTERNET;
#define INTERNET_FLAG_RELOAD 0x80000000
#define INTERNET_FLAG_NO_CACHE_WRITE 0x04000000
extern "C" {
__declspec(dllimport) HINTERNET WINAPI InternetOpenA(LPCSTR agent, DWORD accessType, LPCSTR proxy, LPCSTR proxyBypass, DWORD flags);
__declspec(dllimport) HINTERNET WINAPI InternetOpenUrlA(HINTERNET internet, LPCSTR url, LPCSTR headers, DWORD headersLength, DWORD flags, DWORD_PTR context);
__declspec(dllimport) BOOL WINAPI InternetQueryDataAvailable(HINTERNET file, LPDWORD bytesAvailable, DWORD flags, DWORD_PTR context);
__declspec(dllimport) BOOL WINAPI InternetReadFile(HINTERNET file, LPVOID buffer, DWORD bytesToRead, LPDWORD bytesRead);
__declspec(dllimport) BOOL WINAPI InternetFindNextFileA(HINTERNET find, LPVOID findData);
__declspec(dllimport) BOOL WINAPI InternetCloseHandle(HINTERNET internet);
}
#endif

class cHttp {
	LAYOUT_CHECK_FRIEND;
public:
	cHttp();
	~cHttp();

	bool Init(HWND hWnd);
	void Close();
	bool Open(const char* url);
	bool DownLoad(const char* server, const char* file, const char* localPath, const char* localFile);
	bool DownLoad2(const char* server, const char* file, const char* localPath, const char* localFile, unsigned long size);
	char* DownLoad(const char* server, const char* file, unsigned long& size);

	// chttp.h:66
	template <typename T>
	T* DownLoad(const char* server, const char* file, unsigned long& size) {
		char url[MAX_PATH];
		char buffer[10240];
		DWORD available;
		DWORD read;
		DWORD total;
		T* data = new T[512000];
		T* cursor = data;

		Close();

		sprintf(url, "%s%s", file, server);

		m_hInternet = InternetOpenA("cHttp DownLoad", 0, NULL, NULL, 0);
		if (m_hInternet == NULL) {
			delete[] data;
			return NULL;
		}

		m_hUrl = InternetOpenUrlA(m_hInternet, url, NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE, 0);
		if (m_hUrl == NULL) {
			InternetCloseHandle(m_hInternet);
			delete[] data;
			return NULL;
		}

		total = 0;
		do {
			if (!InternetQueryDataAvailable(m_hUrl, &available, 0, 0)) {
				break;
			}

			if (available <= 0) {
				break;
			}
			if (available > sizeof(buffer)) {
				available = sizeof(buffer);
			}

			if (!InternetReadFile(m_hUrl, buffer, available, &read)) {
				break;
			}

			memcpy(cursor, buffer, read);
			cursor += read;

			total += read;
			if (total > 512000) {
				OutputDebugStringA("memory buffer is overflowed, require more memory.\n");
				break;
			}
		} while (read != 0);

		Close();

		size = total;

		return data;
	}

protected:
	void SendLog(unsigned int msg, long lParam);
	unsigned long GetFileSize(const char* file);

	HINTERNET m_hInternet;    // 0x0
	HINTERNET m_hUrl;         // 0x4
	HWND m_hWnd;              // 0x8
	char m_status[MAX_PATH];  // 0xC
	DWORD m_downloaded;       // 0x110
};

CHECK_SIZE(cHttp, 0x114);
BEGIN_LAYOUT_CHECK(cHttp)
	CHECK_FIELD(cHttp, m_hInternet, 0x0);
	CHECK_FIELD(cHttp, m_hUrl, 0x4);
	CHECK_FIELD(cHttp, m_hWnd, 0x8);
	CHECK_FIELD(cHttp, m_status, 0xC);
	CHECK_FIELD(cHttp, m_downloaded, 0x110);
END_LAYOUT_CHECK

#endif
