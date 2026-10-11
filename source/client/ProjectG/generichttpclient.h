#ifndef PG_CLIENT_PROJECTG_GENERICHTTPCLIENT_H
#define PG_CLIENT_PROJECTG_GENERICHTTPCLIENT_H

#include <stddef.h>
#include <windows.h>
#include <vector>
#include "..\..\defines.h"
#include "..\..\padding.h"

// wininet.h is not in the toolchain's Platform SDK: the imports
// GenericHTTPClient uses, with their SDK signatures (as in cHttp.h).
typedef LPVOID HINTERNET;
typedef WORD INTERNET_PORT;
#ifndef INTERNET_FLAG_NO_CACHE_WRITE
#define INTERNET_FLAG_NO_CACHE_WRITE 0x04000000
#endif
#ifndef INTERNET_FLAG_RELOAD
#define INTERNET_FLAG_RELOAD 0x80000000
#endif
#define INTERNET_FLAG_KEEP_CONNECTION 0x00400000
#define INTERNET_FLAG_SECURE 0x00800000
#define INTERNET_OPEN_TYPE_PRECONFIG 0
#define INTERNET_SERVICE_HTTP 3
#define INTERNET_DEFAULT_HTTP_PORT 80
#define INTERNET_DEFAULT_HTTPS_PORT 443
#define HTTP_ADDREQ_FLAG_REPLACE 0x80000000
#define HTTP_QUERY_RAW_HEADERS_CRLF 22
#define HTTP_ADDREQ_FLAG_ADD_IF_NEW 0x10000000
#define HSR_INITIATE 0x00000008
typedef struct _INTERNET_BUFFERSA {
	DWORD dwStructSize; // 0x0
	struct _INTERNET_BUFFERSA *Next; // 0x4
	LPCSTR lpcszHeader; // 0x8
	DWORD dwHeadersLength; // 0xC
	DWORD dwHeadersTotal; // 0x10
	LPVOID lpvBuffer; // 0x14
	DWORD dwBufferLength; // 0x18
	DWORD dwBufferTotal; // 0x1C
	DWORD dwOffsetLow; // 0x20
	DWORD dwOffsetHigh; // 0x24
} INTERNET_BUFFERSA, *LPINTERNET_BUFFERSA;
CHECK_SIZE(INTERNET_BUFFERSA, 0x28);
extern "C" {
__declspec(dllimport) BOOL WINAPI HttpSendRequestExA(HINTERNET request, LPINTERNET_BUFFERSA buffersIn, LPINTERNET_BUFFERSA buffersOut, DWORD flags, DWORD_PTR context);
__declspec(dllimport) BOOL WINAPI HttpEndRequestA(HINTERNET request, LPINTERNET_BUFFERSA buffersOut, DWORD flags, DWORD_PTR context);
__declspec(dllimport) BOOL WINAPI InternetWriteFile(HINTERNET file, LPCVOID buffer, DWORD bytesToWrite, LPDWORD bytesWritten);
__declspec(dllimport) HINTERNET WINAPI InternetOpenA(LPCSTR agent, DWORD accessType, LPCSTR proxy, LPCSTR proxyBypass, DWORD flags);
__declspec(dllimport) HINTERNET WINAPI InternetConnectA(HINTERNET internet, LPCSTR serverName, INTERNET_PORT serverPort, LPCSTR userName, LPCSTR password, DWORD service, DWORD flags, DWORD_PTR context);
__declspec(dllimport) DWORD WINAPI InternetAttemptConnect(DWORD reserved);
__declspec(dllimport) BOOL WINAPI InternetReadFile(HINTERNET file, LPVOID buffer, DWORD bytesToRead, LPDWORD bytesRead);
__declspec(dllimport) BOOL WINAPI InternetCloseHandle(HINTERNET internet);
__declspec(dllimport) HINTERNET WINAPI HttpOpenRequestA(HINTERNET connect, LPCSTR verb, LPCSTR objectName, LPCSTR version, LPCSTR referrer, LPCSTR *acceptTypes, DWORD flags, DWORD_PTR context);
__declspec(dllimport) BOOL WINAPI HttpAddRequestHeadersA(HINTERNET request, LPCSTR headers, DWORD headersLength, DWORD modifiers);
__declspec(dllimport) BOOL WINAPI HttpSendRequestA(HINTERNET request, LPCSTR headers, DWORD headersLength, LPVOID optional, DWORD optionalLength);
__declspec(dllimport) BOOL WINAPI HttpQueryInfoA(HINTERNET request, DWORD infoLevel, LPVOID buffer, LPDWORD bufferLength, LPDWORD index);
}

#define __SIZE_HTTP_ARGUMENT_NAME 256
#define __SIZE_HTTP_ARGUMENT_VALUE 65536
#define __SIZE_HTTP_BUFFER 100000

class GenericHTTPClient {
	LAYOUT_CHECK_FRIEND;
public:
	struct __GENERIC_HTTP_ARGUMENT {
		char szName[__SIZE_HTTP_ARGUMENT_NAME]; // 0x0
		char szValue[__SIZE_HTTP_ARGUMENT_VALUE]; // 0x100
		DWORD dwType; // 0x10100
	};

	enum enumRequestMethod {
		RequestUnknown = 0,
		RequestGetMethod = 1,
		RequestPostMethod = 2,
		RequestPostMethodMultiPartsFormData = 3
	};

	enum enumTypePostArgument {
		TypeUnknown = 0,
		TypeNormal = 1,
		TypeBinary = 2
	};

	GenericHTTPClient();
	virtual ~GenericHTTPClient();

	static enumRequestMethod GetMethod(int nMethod);
	static enumTypePostArgument GetPostArgumentType(int nType);

	BOOL Connect(LPCSTR szAddress, LPCSTR szAgent, unsigned short nPort, LPCSTR szUserAccount, LPCSTR szPassword);
	BOOL Close();
	VOID InitilizePostArguments();
	VOID AddPostArguments(LPCSTR szName, DWORD nValue);
	VOID AddPostArguments(LPCSTR szName, LPCSTR szValue, BOOL bBinary);
	BOOL Request(LPCSTR szURL, int nMethod, LPCSTR szAgent);
	BOOL RequestOfURI(LPCSTR szURI, int nMethod);
	BOOL Response(PBYTE pHeaderBuffer, DWORD dwHeaderBufferLength, PBYTE pBuffer, DWORD dwBufferLength, DWORD &dwResultSize);
	LPCSTR QueryHTTPResponse();
	LPCSTR QueryHTTPResponseHeader();
	DWORD GetLastError();
	LPCSTR GetContentType(LPCSTR szName);
	VOID ParseURL(LPCSTR szURL, LPSTR szProtocol, LPSTR szAddress, DWORD &dwPort, LPSTR szURI);

protected:
	BOOL RequestGet(LPCSTR szURI);
	BOOL RequestPost(LPCSTR szURI);
	BOOL RequestPostMultiPartsFormData(LPCSTR szURI);
	DWORD ResponseOfBytes(PBYTE pBuffer, DWORD dwSize);
	DWORD GetPostArguments(LPSTR szArguments, DWORD dwLength);
	DWORD AllocMultiPartsFormData(PBYTE &pInBuffer, LPCSTR szBoundary);
	VOID FreeMultiPartsFormData(PBYTE &pBuffer);
	DWORD GetMultiPartsFormDataLength();

	std::vector<__GENERIC_HTTP_ARGUMENT> _vArguments; // 0x4
	char _szHTTPResponseHTML[__SIZE_HTTP_BUFFER]; // 0x14
	char _szHTTPResponseHeader[__SIZE_HTTP_BUFFER]; // 0x186B4
	HINTERNET _hHTTPOpen; // 0x30D54
	HINTERNET _hHTTPConnection; // 0x30D58
	HINTERNET _hHTTPRequest; // 0x30D5C
	DWORD _dwError; // 0x30D60
	UNKNOWN(0x30D64, 0x30D6C);
	DWORD _dwRequestFlags; // 0x30D6C, INTERNET_FLAG_SECURE for port 443, ORed into HttpOpenRequest's flags
};

CHECK_SIZE(GenericHTTPClient::__GENERIC_HTTP_ARGUMENT, 0x10104);
BEGIN_LAYOUT_CHECK(GenericHTTPClient)
	CHECK_FIELD(GenericHTTPClient, _vArguments, 0x4);
	CHECK_FIELD(GenericHTTPClient, _szHTTPResponseHTML, 0x14);
	CHECK_FIELD(GenericHTTPClient, _szHTTPResponseHeader, 0x186B4);
	CHECK_FIELD(GenericHTTPClient, _hHTTPOpen, 0x30D54);
	CHECK_FIELD(GenericHTTPClient, _hHTTPConnection, 0x30D58);
	CHECK_FIELD(GenericHTTPClient, _hHTTPRequest, 0x30D5C);
	CHECK_FIELD(GenericHTTPClient, _dwError, 0x30D60);
	CHECK_FIELD(GenericHTTPClient, _dwRequestFlags, 0x30D6C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GENERICHTTPCLIENT_H
