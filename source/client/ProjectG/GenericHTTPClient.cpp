// Original: d:\build\custom\temp\code\client\projectg\generichttpclient.cpp
#define _MBCS
#include "GenericHTTPClient.h"
#include <stdio.h>
#include <string.h>
#include <tchar.h>

#ifdef PG_OFFLINE
#include "OfflineServer.h"
#endif

#define __SIZE_BUFFER 1024
#define __HTTP_VERB_GET "GET"
#define __HTTP_ACCEPT_TYPE "*/*"
#define __HTTP_VERB_POST "POST"
#define __HTTP_ACCEPT "Accept: */*\r\n"
#define __HTTP_CONTENT_TYPE "Content-Type: application/x-www-form-urlencoded\r\n"
#define __HTTP_CONTENT_TYPE_MULTIPART "Content-Type: multipart/form-data; boundary=--MULTI-PARTS-FORM-DATA-BOUNDARY\r\n"
#define __HTTP_MULTIPART_BOUNDARY "--MULTI-PARTS-FORM-DATA-BOUNDARY"
#define __SIZE_HTTP_HEAD_LINE 2048
#define INTERNET_FLAG_FORMS_SUBMIT 0x00000040

void PrintLastError(DWORD dwErrorCode);

// generichttpclient.cpp:26
void PrintLastError(DWORD dwErrorCode) {
	LPVOID lpMsgBuf;
	::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, NULL);
	::LocalFree(lpMsgBuf);
}

// generichttpclient.cpp:42
GenericHTTPClient::GenericHTTPClient() {
	_hHTTPOpen = _hHTTPConnection = _hHTTPRequest = NULL;
	_dwRequestFlags = 0;
}

// generichttpclient.cpp:48
GenericHTTPClient::~GenericHTTPClient() {
	_hHTTPOpen = _hHTTPConnection = _hHTTPRequest = NULL;
}

// generichttpclient.cpp:53
GenericHTTPClient::enumRequestMethod GenericHTTPClient::GetMethod(int nMethod) {
	return nMethod <= 0 ? GenericHTTPClient::RequestGetMethod : static_cast<GenericHTTPClient::enumRequestMethod>(nMethod);
}

// generichttpclient.cpp:58
GenericHTTPClient::enumTypePostArgument GenericHTTPClient::GetPostArgumentType(int nType) {
	return nType <= 0 ? GenericHTTPClient::TypeNormal : static_cast<GenericHTTPClient::enumTypePostArgument>(nType);
}

// generichttpclient.cpp:62
BOOL GenericHTTPClient::Connect(LPCSTR szAddress, LPCSTR szAgent, unsigned short nPort, LPCSTR szUserAccount, LPCSTR szPassword) {
	_hHTTPOpen = ::InternetOpenA(szAgent, INTERNET_OPEN_TYPE_PRECONFIG, "", "", 0);

	if (!_hHTTPOpen) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	_hHTTPConnection = ::InternetConnectA(_hHTTPOpen, szAddress, nPort, szUserAccount, szPassword, INTERNET_SERVICE_HTTP, INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE, 0);

	if (!_hHTTPConnection) {
		_dwError = ::GetLastError();
		::CloseHandle(_hHTTPOpen);
		return FALSE;
	}

	if (::InternetAttemptConnect(NULL) != NO_ERROR) {
		_dwError = ::GetLastError();
		::CloseHandle(_hHTTPConnection);
		::CloseHandle(_hHTTPOpen);
		return FALSE;
	}

	return TRUE;
}

// generichttpclient.cpp:107
BOOL GenericHTTPClient::Close() {
	if (_hHTTPRequest)
		::InternetCloseHandle(_hHTTPRequest);

	if (_hHTTPConnection)
		::InternetCloseHandle(_hHTTPConnection);

	if (_hHTTPOpen)
		::InternetCloseHandle(_hHTTPOpen);

	return TRUE;
}

// generichttpclient.cpp:121
BOOL GenericHTTPClient::Request(LPCSTR szURL, int nMethod, LPCSTR szAgent) {
	BOOL bReturn = TRUE;
	DWORD dwPort = 0;
	char szProtocol[__SIZE_BUFFER] = "";
	char szAddress[__SIZE_BUFFER] = "";
	char szURI[__SIZE_BUFFER] = "";
	DWORD dwSize = 0;

#ifdef PG_OFFLINE
	// The offline server of the CMake build (CMake PG_OFFLINE) answers in
	// place of Ntreev's web services: nothing leaves the machine.
	return PgOfflineHttpRequest(szURL, _szHTTPResponseHTML, __SIZE_HTTP_BUFFER) ? TRUE : FALSE;
#endif

	ParseURL(szURL, szProtocol, szAddress, dwPort, szURI);

	_dwRequestFlags = (dwPort == INTERNET_DEFAULT_HTTPS_PORT) ? INTERNET_FLAG_SECURE : 0;

	if (!Connect(szAddress, szAgent, (unsigned short)dwPort, NULL, NULL))
		return FALSE;

	if (!RequestOfURI(szURI, nMethod)) {
		bReturn = FALSE;
	} else {
		if (!Response((PBYTE)_szHTTPResponseHeader, __SIZE_HTTP_BUFFER, (PBYTE)_szHTTPResponseHTML, __SIZE_HTTP_BUFFER, dwSize))
			bReturn = FALSE;
	}

	Close();

	return bReturn;
}

// generichttpclient.cpp:158
BOOL GenericHTTPClient::RequestOfURI(LPCSTR szURI, int nMethod) {
	BOOL bReturn = TRUE;

	switch (nMethod) {
	case RequestGetMethod:
	default:
		bReturn = RequestGet(szURI);
		break;
	case RequestPostMethod:
		bReturn = RequestPost(szURI);
		break;
	case RequestPostMethodMultiPartsFormData:
		bReturn = RequestPostMultiPartsFormData(szURI);
		break;
	}

	return bReturn;
}

// generichttpclient.cpp:194
BOOL GenericHTTPClient::RequestGet(LPCSTR szURI) {
	LPCSTR szAcceptType[2] = {__HTTP_ACCEPT_TYPE, NULL};

	_hHTTPRequest = ::HttpOpenRequestA(_hHTTPConnection, __HTTP_VERB_GET, szURI, "HTTP/1.0", "", szAcceptType, _dwRequestFlags | INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE, 0);

	if (!_hHTTPRequest) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	if (!::HttpAddRequestHeadersA(_hHTTPRequest, __HTTP_ACCEPT, strlen(__HTTP_ACCEPT), HTTP_ADDREQ_FLAG_REPLACE)) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	if (!::HttpSendRequestA(_hHTTPRequest, NULL, 0, NULL, 0)) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	return TRUE;
}

// generichttpclient.cpp:237
BOOL GenericHTTPClient::RequestPost(LPCSTR szURI) {
	LPCSTR szAcceptType[2] = {__HTTP_ACCEPT_TYPE, NULL};
	char szPostArguments[__SIZE_BUFFER] = "";
	LPCSTR szContentType = __HTTP_CONTENT_TYPE;

	GetPostArguments(szPostArguments, __SIZE_BUFFER);

	_hHTTPRequest = ::HttpOpenRequestA(_hHTTPConnection, __HTTP_VERB_POST, szURI, "HTTP/1.0", "", szAcceptType, _dwRequestFlags | INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_FORMS_SUBMIT, 0);

	if (!_hHTTPRequest) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	if (!::HttpAddRequestHeadersA(_hHTTPRequest, __HTTP_ACCEPT, strlen(__HTTP_ACCEPT), HTTP_ADDREQ_FLAG_REPLACE)) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	if (!::HttpSendRequestA(_hHTTPRequest, szContentType, strlen(szContentType), szPostArguments, strlen(szPostArguments))) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	return TRUE;
}

// generichttpclient.cpp:286
BOOL GenericHTTPClient::RequestPostMultiPartsFormData(LPCSTR szURI) {
	LPCSTR szAcceptType[2] = {__HTTP_ACCEPT_TYPE, NULL};
	LPCSTR szContentType = __HTTP_CONTENT_TYPE_MULTIPART;
	PBYTE pPostBuffer = NULL;
	DWORD dwPostBufferLength = AllocMultiPartsFormData(pPostBuffer, __HTTP_MULTIPART_BOUNDARY);

	_hHTTPRequest = ::HttpOpenRequestA(_hHTTPConnection, __HTTP_VERB_POST, szURI, "HTTP/1.0", "", szAcceptType, _dwRequestFlags | INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_FORMS_SUBMIT, 0);

	if (!_hHTTPRequest) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	if (!::HttpAddRequestHeadersA(_hHTTPRequest, __HTTP_ACCEPT, strlen(__HTTP_ACCEPT), HTTP_ADDREQ_FLAG_REPLACE)) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	if (!::HttpAddRequestHeadersA(_hHTTPRequest, szContentType, strlen(szContentType), HTTP_ADDREQ_FLAG_ADD_IF_NEW)) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	char szContentLength[__SIZE_BUFFER];
	::ZeroMemory(szContentLength, __SIZE_BUFFER);

	sprintf(szContentLength, "Content-Length: %d\r\n", dwPostBufferLength);

	if (!::HttpAddRequestHeadersA(_hHTTPRequest, szContentLength, strlen(szContentLength), HTTP_ADDREQ_FLAG_ADD_IF_NEW)) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	INTERNET_BUFFERSA InternetBufferIn = {0};
	InternetBufferIn.dwStructSize = sizeof(INTERNET_BUFFERSA);
	InternetBufferIn.Next = NULL;

	if (!::HttpSendRequestExA(_hHTTPRequest, &InternetBufferIn, NULL, HSR_INITIATE, 0)) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	DWORD dwOutPostBufferLength = 0;
	if (!::InternetWriteFile(_hHTTPRequest, pPostBuffer, dwPostBufferLength, &dwOutPostBufferLength)) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	if (!::HttpEndRequestA(_hHTTPRequest, NULL, HSR_INITIATE, 0)) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	FreeMultiPartsFormData(pPostBuffer);

	return TRUE;
}

// generichttpclient.cpp:387
DWORD GenericHTTPClient::ResponseOfBytes(PBYTE pBuffer, DWORD dwSize) {
	static DWORD dwBytes = 0;

	if (!_hHTTPRequest) {
		_dwError = ::GetLastError();
		return 0;
	}

	::ZeroMemory(pBuffer, dwSize);

	if (!::InternetReadFile(_hHTTPRequest, pBuffer, dwSize, &dwBytes)) {
		_dwError = ::GetLastError();
		return 0;
	}

	return dwBytes;
}

// generichttpclient.cpp:413
BOOL GenericHTTPClient::Response(PBYTE pHeaderBuffer, DWORD dwHeaderBufferLength, PBYTE pBuffer, DWORD dwBufferLength, DWORD &dwResultSize) {
	BYTE pResponseBuffer[__SIZE_BUFFER] = {0};
	DWORD dwNumOfBytesToRead = 0;

	if (!_hHTTPRequest) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	::ZeroMemory(pBuffer, dwBufferLength);
	dwResultSize = 0;
	while ((dwNumOfBytesToRead = ResponseOfBytes(pResponseBuffer, __SIZE_BUFFER)) != 0 && dwResultSize < dwBufferLength) {
		::CopyMemory(pBuffer + dwResultSize, pResponseBuffer, dwNumOfBytesToRead);
		dwResultSize += dwNumOfBytesToRead;
	}

	::ZeroMemory(pHeaderBuffer, dwHeaderBufferLength);
	if (!::HttpQueryInfoA(_hHTTPRequest, HTTP_QUERY_RAW_HEADERS_CRLF, pHeaderBuffer, &dwHeaderBufferLength, NULL)) {
		_dwError = ::GetLastError();
		return FALSE;
	}

	return dwResultSize ? TRUE : FALSE;
}

// generichttpclient.cpp:441
VOID GenericHTTPClient::AddPostArguments(LPCSTR szName, LPCSTR szValue, BOOL bBinary) {
	__GENERIC_HTTP_ARGUMENT arg;
	::ZeroMemory(&arg, sizeof(arg));

	strcpy(arg.szName, szName);
	strcpy(arg.szValue, szValue);

	if (!bBinary)
		arg.dwType = TypeNormal;
	else
		arg.dwType = TypeBinary;

	_vArguments.push_back(arg);
}

// generichttpclient.cpp:458
VOID GenericHTTPClient::AddPostArguments(LPCSTR szName, DWORD nValue) {
	__GENERIC_HTTP_ARGUMENT arg;
	::ZeroMemory(&arg, sizeof(arg));

	strcpy(arg.szName, szName);
	sprintf(arg.szValue, "%d", nValue);
	arg.dwType = TypeNormal;

	_vArguments.push_back(arg);
}

// generichttpclient.cpp:472
DWORD GenericHTTPClient::GetPostArguments(LPSTR szArguments, DWORD dwLength) {
	::ZeroMemory(szArguments, dwLength);
	std::vector<__GENERIC_HTTP_ARGUMENT>::iterator itArg;
	for (itArg = _vArguments.begin(); itArg < _vArguments.end();) {
		strcat(szArguments, itArg->szName);
		strcat(szArguments, "=");
		strcat(szArguments, itArg->szValue);

		if (++itArg < _vArguments.end())
			strcat(szArguments, "&");
	}

	szArguments[dwLength] = '\0';

	return strlen(szArguments);
}

// generichttpclient.cpp:494
VOID GenericHTTPClient::InitilizePostArguments() {
	_vArguments.clear();
}

// generichttpclient.cpp:499
VOID GenericHTTPClient::FreeMultiPartsFormData(PBYTE &pBuffer) {
	if (pBuffer) {
		::LocalFree(pBuffer);
		pBuffer = NULL;
	}
}

// generichttpclient.cpp:510
DWORD GenericHTTPClient::AllocMultiPartsFormData(PBYTE &pInBuffer, LPCSTR szBoundary) {
	if (pInBuffer) {
		::LocalFree(pInBuffer);
		pInBuffer = NULL;
	}

	pInBuffer = (PBYTE)::LocalAlloc(LPTR, GetMultiPartsFormDataLength());

	std::vector<__GENERIC_HTTP_ARGUMENT>::iterator itArgv;
	DWORD dwPosition = 0;
	DWORD dwBufferSize = 0;

	for (itArgv = _vArguments.begin(); itArgv < _vArguments.end(); ++itArgv) {
		PBYTE pBuffer = NULL;

		switch (itArgv->dwType) {
		case TypeNormal:
			pBuffer = (PBYTE)::LocalAlloc(LPTR, __SIZE_HTTP_HEAD_LINE * 4);

			sprintf((char *)pBuffer,
				"--%s\r\n"
				"Content-Disposition: form-data; name=\"%s\"\r\n"
				"\r\n"
				"%s\r\n",
				szBoundary,
				itArgv->szName,
				itArgv->szValue);

			dwBufferSize = _tcslen((char *)pBuffer);
			break;
		case TypeBinary: {
			DWORD dwNumOfBytesToRead = 0;
			DWORD dwTotalBytes = 0;

			HANDLE hFile = ::CreateFileA(itArgv->szValue, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

			if (hFile != INVALID_HANDLE_VALUE) {
				DWORD dwSize = ::GetFileSize(hFile, NULL);

				pBuffer = (PBYTE)::LocalAlloc(LPTR, __SIZE_HTTP_HEAD_LINE * 3 + dwSize + 1);
				BYTE pBytes[__SIZE_BUFFER + 1] = "";

				sprintf((char *)pBuffer,
					"--%s\r\n"
					"Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n"
					"Content-Type: %s\r\n"
					"\r\n",
					szBoundary,
					itArgv->szName,
					itArgv->szValue,
					GetContentType(itArgv->szValue));

				DWORD dwBufPosition = _tcslen((char *)pBuffer);

				while (::ReadFile(hFile, pBytes, __SIZE_BUFFER, &dwNumOfBytesToRead, NULL) && dwNumOfBytesToRead > 0 && dwTotalBytes <= dwSize) {
					::CopyMemory(pBuffer + dwBufPosition + dwTotalBytes, pBytes, dwNumOfBytesToRead);
					::ZeroMemory(pBytes, __SIZE_BUFFER + 1);
					dwTotalBytes += dwNumOfBytesToRead;
				}

				::CopyMemory(pBuffer + dwBufPosition + dwTotalBytes, "\r\n", _tcslen("\r\n"));
				::CloseHandle(hFile);

				dwBufferSize = dwBufPosition + dwTotalBytes + _tcslen("\r\n");
			}
			break;
		}
		}

		::CopyMemory(pInBuffer + dwPosition, pBuffer, dwBufferSize);
		dwPosition += dwBufferSize;

		if (pBuffer) {
			::LocalFree(pBuffer);
			pBuffer = NULL;
		}
	}

	::CopyMemory(pInBuffer + dwPosition, "--", 2);
	::CopyMemory(pInBuffer + dwPosition + 2, szBoundary, _tcslen(szBoundary));
	::CopyMemory(pInBuffer + dwPosition + 2 + _tcslen(szBoundary), "--\r\n", 4);

	return dwPosition + 6 + _tcslen(szBoundary);
}

// generichttpclient.cpp:607
DWORD GenericHTTPClient::GetMultiPartsFormDataLength() {
	std::vector<__GENERIC_HTTP_ARGUMENT>::iterator itArgv;
	DWORD dwLength = 0;

	for (itArgv = _vArguments.begin(); itArgv < _vArguments.end(); ++itArgv) {
		switch (itArgv->dwType) {
		case TypeNormal:
			dwLength += __SIZE_HTTP_HEAD_LINE * 4;
			break;
		case TypeBinary: {
			HANDLE hFile = ::CreateFileA(itArgv->szValue, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile != INVALID_HANDLE_VALUE) {
				dwLength += __SIZE_HTTP_HEAD_LINE * 3 + ::GetFileSize(hFile, NULL);
				::CloseHandle(hFile);
			}
			break;
		}
		}
	}

	return dwLength;
}

// generichttpclient.cpp:643
LPCSTR GenericHTTPClient::GetContentType(LPCSTR szName) {
	static char szReturn[1024] = {0};
	LONG dwLen = 1024;
	DWORD dwLength = 0;
	HKEY hKey;

	for (dwLength = _tcslen(szName); dwLength > 0; dwLength--) {
		if (!_tcsncmp(szName + dwLength, ".", 1))
			break;
	}

	if (::RegOpenKeyExA(HKEY_CLASSES_ROOT, szName + dwLength, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
		if (::RegQueryValueExA(hKey, "Content Type", NULL, NULL, (LPBYTE)szReturn, (LPDWORD)&dwLen) != ERROR_SUCCESS)
			_tcsncpy(szReturn, "application/octet-stream", _tcslen("application/octet-stream"));
		::RegCloseKey(hKey);
	} else {
		_tcsncpy(szReturn, "application/octet-stream", _tcslen("application/octet-stream"));
	}

	return szReturn;
}

// generichttpclient.cpp:670
DWORD GenericHTTPClient::GetLastError() {
	return _dwError;
}

// generichttpclient.cpp:675
VOID GenericHTTPClient::ParseURL(LPCSTR szURL, LPSTR szProtocol, LPSTR szAddress, DWORD &dwPort, LPSTR szURI) {
	DWORD dwPosition = 0;
	BOOL bFlag = FALSE;

	while (_tcslen(szURL) > 0 && dwPosition < _tcslen(szURL) && _tcsncmp(szURL + dwPosition, ":", 1))
		++dwPosition;

	if (!_tcsncmp(szURL + dwPosition + 1, "/", 1)) {
		if (szProtocol) {
			_tcsncpy(szProtocol, szURL, dwPosition);
			szProtocol[dwPosition] = '\0';
		}
		bFlag = TRUE;
	} else {
		if (szProtocol) {
			_tcsncpy(szProtocol, "http", 4);
			szProtocol[5] = '\0';
		}
	}

	DWORD dwStartPosition = 0;

	if (bFlag)
		dwStartPosition = dwPosition += 3;
	else
		dwStartPosition = dwPosition = 0;

	bFlag = FALSE;
	while (_tcslen(szURL) > 0 && dwPosition < _tcslen(szURL) && _tcsncmp(szURL + dwPosition, "/", 1))
		++dwPosition;

	DWORD dwFind = dwStartPosition;

	for (; dwFind <= dwPosition; dwFind++) {
		if (!_tcsncmp(szURL + dwFind, ":", 1)) {
			bFlag = TRUE;
			break;
		}
	}

	if (bFlag) {
		char sztmp[__SIZE_BUFFER] = "";
		_tcsncpy(sztmp, szURL + dwFind + 1, dwPosition - dwFind);
		dwPort = _ttol(sztmp);
		_tcsncpy(szAddress, szURL + dwStartPosition, dwFind - dwStartPosition);
	} else if (!_tcscmp(szProtocol, "https")) {
		dwPort = INTERNET_DEFAULT_HTTPS_PORT;
		_tcsncpy(szAddress, szURL + dwStartPosition, dwPosition - dwStartPosition);
	} else {
		dwPort = INTERNET_DEFAULT_HTTP_PORT;
		_tcsncpy(szAddress, szURL + dwStartPosition, dwPosition - dwStartPosition);
	}

	if (dwPosition < _tcslen(szURL))
		_tcsncpy(szURI, szURL + dwPosition, _tcslen(szURL) - dwPosition);
	else
		szURI[0] = '\0';
}

// generichttpclient.cpp:743
LPCSTR GenericHTTPClient::QueryHTTPResponseHeader() {
	return _szHTTPResponseHeader;
}

// generichttpclient.cpp:748
LPCSTR GenericHTTPClient::QueryHTTPResponse() {
	return _szHTTPResponseHTML;
}
