// Original: d:\build\custom\temp\code\client\projectg\chttp.cpp
#include "cHttp.h"

// chttp.cpp:23
cHttp::cHttp() {
	m_hInternet = NULL;
	m_hUrl = NULL;

	m_hWnd = NULL;
	m_downloaded = 0;
}

// chttp.cpp:32
cHttp::~cHttp() {
}

// chttp.cpp:36
bool cHttp::Init(HWND hWnd) {
	m_hWnd = hWnd;

	return true;
}

// chttp.cpp:43
void cHttp::SendLog(unsigned int msg, long lParam) {
	if (m_hWnd) {
		SendMessageA(m_hWnd, msg, (WPARAM)m_status, lParam);
	}
}

// chttp.cpp:49
void cHttp::Close() {
	if (m_hUrl) {
		InternetCloseHandle(m_hUrl);
		m_hUrl = NULL;
	}

	if (m_hInternet) {
		InternetCloseHandle(m_hInternet);
		m_hInternet = NULL;
	}
}

// chttp.cpp:64
bool cHttp::Open(const char* url) {
	Close();

	m_hInternet = InternetOpenA("cHttp DownLoad", 0, NULL, NULL, 0);
	if (m_hInternet == NULL) {
		return false;
	}

	m_hUrl = InternetOpenUrlA(m_hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (m_hUrl == NULL) {
		InternetCloseHandle(m_hInternet);
		return false;
	}

	Close();

	return true;
}

// chttp.cpp:85
bool cHttp::DownLoad(const char* fileName, const char* remoteFile, const char* localPath, const char* serverUrl) {
	char path[MAX_PATH];
	char url[MAX_PATH];
	char buffer[10240];
	DWORD available;
	DWORD read;
	DWORD written;

	Close();

	sprintf(url, "%s%s", serverUrl, remoteFile);
	sprintf(path, "%s%s", localPath, fileName);

	m_hInternet = InternetOpenA("cHttp DownLoad", 0, NULL, NULL, 0);
	if (m_hInternet == NULL) {
		return false;
	}

	m_hUrl = InternetOpenUrlA(m_hInternet, url, NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if (m_hUrl == NULL) {
		InternetCloseHandle(m_hInternet);
		return false;
	}

	HANDLE hFile = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		Close();
		return false;
	}

	bool result = true;

	do {
		if (!InternetQueryDataAvailable(m_hUrl, &available, 0, 0)) {
			result = false;
			break;
		}

		if (available <= 0) {
			break;
		}
		if (available > sizeof(buffer)) {
			available = sizeof(buffer);
		}

		if (!InternetReadFile(m_hUrl, buffer, available, &read)) {
			result = false;
			break;
		}

		if (!WriteFile(hFile, buffer, read, &written, NULL)) {
			result = false;
			break;
		}
	} while (read != 0);

	CloseHandle(hFile);
	Close();

	return result;
}

// chttp.cpp:247
char* cHttp::DownLoad(const char* server, const char* file, unsigned long& size) {
	return DownLoad<char>(server, file, size);
}

// chttp.cpp:254
unsigned long cHttp::GetFileSize(const char* file) {
	WIN32_FIND_DATAA findData;
	BOOL found = InternetFindNextFileA(m_hUrl, &findData);


	return found ? findData.nFileSizeLow : 0;
}
