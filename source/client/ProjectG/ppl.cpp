// Original: d:\build\custom\temp\code\client\projectg\ppl.cpp
#include "PPL.h"
#include "ClientSetting.h"

#include <windows.h>

// wininet.h is not in the toolchain's Platform SDK: the four imports used
// here, with their SDK signatures.
typedef LPVOID HINTERNET;
#define INTERNET_FLAG_NO_CACHE_WRITE 0x04000000
extern "C" {
__declspec(dllimport) HINTERNET WINAPI InternetOpenA(LPCSTR agent, DWORD accessType, LPCSTR proxy, LPCSTR proxyBypass, DWORD flags);
__declspec(dllimport) HINTERNET WINAPI InternetOpenUrlA(HINTERNET internet, LPCSTR url, LPCSTR headers, DWORD headersLength, DWORD flags, DWORD_PTR context);
__declspec(dllimport) BOOL WINAPI InternetQueryDataAvailable(HINTERNET file, LPDWORD bytesAvailable, DWORD flags, DWORD_PTR context);
__declspec(dllimport) BOOL WINAPI InternetCloseHandle(HINTERNET internet);
}

// clientsetting.h:400
int COption::gGetPPLSize(void) {
	return m_option.m_pplSize;
}

// ppl.cpp:21
CPPL::CPPL(void) {
}

// ppl.cpp:26
CPPL::~CPPL(void) {
}

// ppl.cpp:42
bool CPPL::IsPPLEnable(void) {
	char url[] = "http://www.hjwon.com:8000/test.swf";
	DWORD size = 0;

	HINTERNET internet = InternetOpenA("PPL Check", 0, 0, 0, 0);
	if (internet == NULL) {
		return false;
	}

	HINTERNET file = InternetOpenUrlA(internet, url, 0, 0, INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if (file == NULL) {
		InternetCloseHandle(internet);
		return false;
	}

	if (!InternetQueryDataAvailable(file, &size, 0, 0)) {
		return false;
	}

	if (COption::Instance()->gGetPPLSize() == (int)size) {
		if (!COption::Instance()->gIsPPLEnable()) {
			return false;
		}
	}

	COption::Instance()->gSetPPLEnable(true);
	COption::Instance()->gSetPPLSize(size);
	return true;
}
