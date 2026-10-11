// Original: d:\build\custom\temp\code\shared\exceptionreport.cpp
#include "../shared/ExceptionReport.h"

#include "../client/ProjectG/lock.hpp"
#include "../client/ProjectG/SharedDoc.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dbghelp.h>

extern char PY_PUBLIC_VERSION[];

namespace S5 {
namespace UTIL {
int Base64Encode(char *src, int length, char *dst);   // s5/utilities.cpp
} // namespace UTIL
} // namespace S5

LONG WINAPI RecordExceptionInfo(EXCEPTION_POINTERS *pExceptionInfo);
void __cdecl SecurityErrorHandler(int code, void *data);
unsigned long SnapCurrentContext(CONTEXT *pContext);

// The image help entry points LoadImageHlp resolves (dbghelp.dll, else imagehlp.dll).
typedef BOOL (__stdcall *SYMCLEANUPPROC)(HANDLE);
typedef PVOID (__stdcall *SYMFUNCTIONTABLEACCESSPROC)(HANDLE, DWORD);
typedef BOOL (__stdcall *SYMGETLINEFROMADDRPROC)(HANDLE, DWORD, PDWORD, PIMAGEHLP_LINE);
typedef DWORD (__stdcall *SYMGETMODULEBASEPROC)(HANDLE, DWORD);
typedef BOOL (__stdcall *SYMGETMODULEINFOPROC)(HANDLE, DWORD, PIMAGEHLP_MODULE);
typedef DWORD (__stdcall *SYMGETOPTIONSPROC)(void);
typedef BOOL (__stdcall *SYMGETSYMFROMADDRPROC)(HANDLE, DWORD, PDWORD, PIMAGEHLP_SYMBOL);
typedef BOOL (__stdcall *SYMINITIALIZEPROC)(HANDLE, PSTR, BOOL);
typedef DWORD (__stdcall *SYMLOADMODULEPROC)(HANDLE, HANDLE, PSTR, PSTR, DWORD, DWORD);
typedef DWORD (__stdcall *SYMSETOPTIONSPROC)(DWORD);
typedef BOOL (__stdcall *STACKWALKPROC)(DWORD, HANDLE, HANDLE, LPSTACKFRAME, PVOID,
	PREAD_PROCESS_MEMORY_ROUTINE, PFUNCTION_TABLE_ACCESS_ROUTINE,
	PGET_MODULE_BASE_ROUTINE, PTRANSLATE_ADDRESS_ROUTINE);
typedef DWORD (__stdcall *UNDECORATESYMBOLNAMEPROC)(PCSTR, PSTR, DWORD, DWORD);

HINSTANCE g_hDLL;                                                // 0xb78d40
unsigned long g_dwStackBottom;                                   // 0xb78d44
unsigned long g_dwStack;                                         // 0xb78d48
LPTOP_LEVEL_EXCEPTION_FILTER g_pPrevUnhandledExceptionFilter;    // 0xb78d4c

static SYMCLEANUPPROC s_pfnSymCleanup;                           // 0xb78d50
static SYMFUNCTIONTABLEACCESSPROC s_pfnSymFunctionTableAccess;   // 0xb78d54
static SYMGETLINEFROMADDRPROC s_pfnSymGetLineFromAddr;           // 0xb78d58
static SYMGETMODULEBASEPROC s_pfnSymGetModuleBase;               // 0xb78d5c
static SYMGETMODULEINFOPROC s_pfnSymGetModuleInfo;               // 0xb78d60
static SYMGETOPTIONSPROC s_pfnSymGetOptions;                     // 0xb78d64
static SYMGETSYMFROMADDRPROC s_pfnSymGetSymFromAddr;             // 0xb78d68
static SYMINITIALIZEPROC s_pfnSymInitialize;                     // 0xb78d6c
static SYMLOADMODULEPROC s_pfnSymLoadModule;                     // 0xb78d70
static SYMSETOPTIONSPROC s_pfnSymSetOptions;                     // 0xb78d74
static STACKWALKPROC s_pfnStackWalk;                             // 0xb78d78
static UNDECORATESYMBOLNAMEPROC s_pfnUnDecorateSymbolName;       // 0xb78d7c

// exceptionreport.cpp:75
CExceptionReport g_exceptionReport;

// exceptionreport.cpp:77
_client::CCriticalSection g_criticalSection;

__declspec(thread) void *CExceptionReport::m_curThread = INVALID_HANDLE_VALUE;
__declspec(thread) unsigned long CExceptionReport::m_curThreadId = 0;

// exceptionreport.cpp:93
static bool LoadImageHlp(void) {
	HMODULE hDLL = LoadLibraryA("dbghelp.dll");
	if (hDLL == NULL) {
		hDLL = LoadLibraryA("imagehlp.dll");
		if (hDLL == NULL)
			return false;
	}

	s_pfnSymCleanup = (SYMCLEANUPPROC)GetProcAddress(hDLL, "SymCleanup");
	s_pfnSymFunctionTableAccess = (SYMFUNCTIONTABLEACCESSPROC)GetProcAddress(hDLL, "SymFunctionTableAccess");
	s_pfnSymGetModuleBase = (SYMGETMODULEBASEPROC)GetProcAddress(hDLL, "SymGetModuleBase");
	s_pfnSymGetModuleInfo = (SYMGETMODULEINFOPROC)GetProcAddress(hDLL, "SymGetModuleInfo");
	s_pfnSymGetOptions = (SYMGETOPTIONSPROC)GetProcAddress(hDLL, "SymGetOptions");
	s_pfnSymGetSymFromAddr = (SYMGETSYMFROMADDRPROC)GetProcAddress(hDLL, "SymGetSymFromAddr");
	s_pfnSymInitialize = (SYMINITIALIZEPROC)GetProcAddress(hDLL, "SymInitialize");
	s_pfnSymSetOptions = (SYMSETOPTIONSPROC)GetProcAddress(hDLL, "SymSetOptions");
	s_pfnStackWalk = (STACKWALKPROC)GetProcAddress(hDLL, "StackWalk");
	s_pfnUnDecorateSymbolName = (UNDECORATESYMBOLNAMEPROC)GetProcAddress(hDLL, "UnDecorateSymbolName");
	s_pfnSymLoadModule = (SYMLOADMODULEPROC)GetProcAddress(hDLL, "SymLoadModule");
	s_pfnSymGetLineFromAddr = (SYMGETLINEFROMADDRPROC)GetProcAddress(hDLL, "SymGetLineFromAddr");

	if (s_pfnSymCleanup == NULL || s_pfnSymFunctionTableAccess == NULL || s_pfnSymGetModuleBase == NULL
		|| s_pfnSymGetModuleInfo == NULL || s_pfnSymGetOptions == NULL || s_pfnSymGetSymFromAddr == NULL
		|| s_pfnSymInitialize == NULL || s_pfnSymSetOptions == NULL || s_pfnStackWalk == NULL
		|| s_pfnUnDecorateSymbolName == NULL || s_pfnSymLoadModule == NULL) {
		FreeLibrary(hDLL);
		return false;
	}

	g_hDLL = hDLL;

	return true;
}

// exceptionreport.cpp:149
static void UnloadImageHlp(void) {
	if (g_hDLL) {
		FreeLibrary(g_hDLL);
		g_hDLL = NULL;
	}
}

// exceptionreport.cpp:158
// Names the module containing addr; "Unknown" when it has none.
static BOOL GetModuleName(LPCVOID addr, char *pszModule, DWORD len) {
	MEMORY_BASIC_INFORMATION mbi;

	if (VirtualQuery(addr, &mbi, sizeof(mbi))) {
		if (GetModuleFileNameA((HMODULE)mbi.AllocationBase, pszModule, len) > 0)
			return TRUE;
	}

	strncpy(pszModule, "Unknown", len);
	return FALSE;
}

// exceptionreport.cpp:175
// The name of the exception the record describes.
static const char *GetExceptionDescription(const EXCEPTION_RECORD *pRecord) {
	static __declspec(thread) char s_szUserInfo[256];

	switch (pRecord->ExceptionCode) {
	case DBG_CONTROL_C: return "Control-C";
	case DBG_CONTROL_BREAK: return "Control-Break";
	case EXCEPTION_DATATYPE_MISALIGNMENT: return "Datatype Misalignment";
	case EXCEPTION_BREAKPOINT: return "Breakpoint";
	case EXCEPTION_ACCESS_VIOLATION: return "Access Violation";
	case EXCEPTION_IN_PAGE_ERROR: return "an In Page Error";
	case STATUS_NO_MEMORY: return "No Memory";
	case EXCEPTION_ILLEGAL_INSTRUCTION: return "Illegal Instruction";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "Noncontinuable Exception";
	case EXCEPTION_INVALID_DISPOSITION: return "Invalid Disposition";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "Array Bounds Exceeded";
	case EXCEPTION_FLT_DENORMAL_OPERAND: return "Float Denormal Operand";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "Float Divide by Zero";
	case EXCEPTION_FLT_INEXACT_RESULT: return "Float Inexact Result";
	case EXCEPTION_FLT_INVALID_OPERATION: return "Float Invalid Operation";
	case EXCEPTION_FLT_OVERFLOW: return "Float Overflow";
	case EXCEPTION_FLT_STACK_CHECK: return "Float Stack Check";
	case EXCEPTION_FLT_UNDERFLOW: return "Float Underflow";
	case EXCEPTION_INT_DIVIDE_BY_ZERO: return "Integer Divide by Zero";
	case EXCEPTION_INT_OVERFLOW: return "Integer Overflow";
	case EXCEPTION_PRIV_INSTRUCTION: return "Privileged Instruction";
	case EXCEPTION_STACK_OVERFLOW: return "Stack Overflow";
	case 0xC0000142: return "DLL Initialization Failed";   // STATUS_DLL_INIT_FAILED
	case 0xE06D7363: return "Microsoft C++ Exception";     // the C++ throw code
	case 0x60000000: return "PacketDecodeException";
	case 0xE0000001:   // thrown with a message and two arguments
		if (pRecord->NumberParameters >= 3) {
			sprintf(s_szUserInfo, "EXCEPTION_WITH_USER_INFO => msg: %s, arg1: %d, arg2: %d",
				(const char *)pRecord->ExceptionInformation[0], pRecord->ExceptionInformation[1],
				pRecord->ExceptionInformation[2]);
		} else {
			sprintf(s_szUserInfo, "EXCEPTION_WITH_USER_INFO => invalid parameter count(%d)",
				pRecord->NumberParameters);
		}
		return s_szUserInfo;
	}

	return "Unknown exception type";
}

// exceptionreport.cpp:225
// Writes a minidump of the process to pszFileName.
static void WriteMiniDump(const char *pszFileName, EXCEPTION_POINTERS *pExceptionInfo) {
	HANDLE hFile = CreateFileA(pszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	SetThreadPriority(CExceptionReport::GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
	MINIDUMP_EXCEPTION_INFORMATION *pInfo;
	if (pExceptionInfo) {
		exceptionInfo.ThreadId = CExceptionReport::GetCurrentThreadId();
		exceptionInfo.ClientPointers = TRUE;
		exceptionInfo.ExceptionPointers = pExceptionInfo;
		pInfo = &exceptionInfo;
	} else {
		pInfo = NULL;
	}

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, pInfo, NULL, NULL);

	SetThreadPriority(CExceptionReport::GetCurrentThread(), THREAD_PRIORITY_NORMAL);

	CloseHandle(hFile);
}

// exceptionreport.cpp:257
// The /GS handler: writes the report (without exception pointers) and exits.
void __cdecl SecurityErrorHandler(int code, void *data) {
	if (g_exceptionReport.m_bEnabled) {
		_client::_private::CLock<_client::CCriticalSection> lock(g_criticalSection);

		g_exceptionReport.DumpExceptionReport(NULL);
	}
	ExitProcess(1);
}

// exceptionreport.cpp:339
void CExceptionReport::Init(void) {
	LoadImageHlp();

	g_pPrevUnhandledExceptionFilter = SetUnhandledExceptionFilter(RecordExceptionInfo);

	_set_security_error_handler(SecurityErrorHandler);

	SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

	m_bEnabled = TRUE;
	m_bInitialized = true;
}

// exceptionreport.cpp:360
void CExceptionReport::Shutdown(void) {
	if (g_pPrevUnhandledExceptionFilter)
		SetUnhandledExceptionFilter(g_pPrevUnhandledExceptionFilter);

	SetErrorMode(0);

	UnloadImageHlp();

	m_bInitialized = false;
}

// exceptionreport.cpp:372
int CExceptionReport::Enable(void) {
	return InterlockedExchange(&m_bEnabled, TRUE) == FALSE;
}

// exceptionreport.cpp:377
int CExceptionReport::Disable(void) {
	return InterlockedExchange(&m_bEnabled, FALSE);
}

// exceptionreport.cpp:382
int CExceptionReport::IsEnabled(void) const {
	return m_bEnabled;
}

// exceptionreport.cpp:387
void CExceptionReport::StartLog(const char *pszFileName) {
	m_hFile = CreateFileA(pszFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
}

// exceptionreport.cpp:396
void __cdecl CExceptionReport::LogPrintf(const char *pszFormat, ...) const {
	char szBuffer[4096];
	DWORD dwWritten;
	va_list args;

	va_start(args, pszFormat);
	vsprintf(szBuffer, pszFormat, args);
	va_end(args);

	WriteFile(m_hFile, szBuffer, strlen(szBuffer), &dwWritten, NULL);
}

// exceptionreport.cpp:409
void CExceptionReport::EndLog(void) {
	LogPrintf("--------------------------------- END_OF_LOG ---------------------------------\r\n");

	if (m_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFile);

	m_hFile = INVALID_HANDLE_VALUE;
}

// exceptionreport.cpp:419
void CExceptionReport::CollectSystemInfo(void) {
	OSVERSIONINFOA osvi;
	DWORD dwSize;

	memset(&osvi, 0, sizeof(osvi));

	osvi.dwOSVersionInfoSize = sizeof(osvi);
	if (GetVersionExA(&osvi)) {
		const char *pszOS;
		switch (osvi.dwPlatformId) {
		case VER_PLATFORM_WIN32_WINDOWS:
			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
				pszOS = "Windows 95";
			else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
				pszOS = "Windows 98/98SE";
			else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
				pszOS = "Windows ME";
			else
				pszOS = "Windows ME or later";
			break;
		case VER_PLATFORM_WIN32_NT:
			if (osvi.dwMajorVersion <= 4)
				pszOS = "Windows NT";
			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
				pszOS = "Windows 2000";
			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
				pszOS = "Microsoft Windows XP";
			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
				pszOS = "Microsoft Windows Server 2003 family";
			else
				pszOS = "Microsoft Vista or later";
			break;
		default:
			pszOS = "Unknown Windows variant";
			break;
		}

		wsprintfA(m_szOS, "%s %i.%02d.%i %s", pszOS, osvi.dwMajorVersion, osvi.dwMinorVersion,
			osvi.dwBuildNumber, osvi.szCSDVersion);
	} else {
		strcpy(m_szOS, "Unknown");
	}

	GetSystemInfo(&m_systemInfo);

	m_memoryStatus.dwLength = sizeof(m_memoryStatus);
	GlobalMemoryStatus(&m_memoryStatus);

	dwSize = sizeof(m_szUser);
	if (!GetUserNameA(m_szUser, &dwSize))
		strcpy(m_szUser, "Unknown");

	dwSize = sizeof(m_szComputer);
	if (!GetComputerNameA(m_szComputer, &dwSize))
		strcpy(m_szComputer, "Unknown");
}

// exceptionreport.cpp:479
// Copies the file name part of pszPath (after its last '\\' or '/') to pszName.
static void GetFileName(char *pszName, const char *pszPath) {
	const char *p = pszPath + strlen(pszPath) - 1;

	while (p != pszPath && p[-1] != '\\' && p[-1] != '/')
		p--;

	while (*p)
		*pszName++ = *p++;

	*pszName = '\0';
}

// exceptionreport.cpp:496
void CExceptionReport::DumpHeaderInfo(EXCEPTION_POINTERS *pExceptionInfo) {
	unsigned long uid = (unsigned long)-1;
	const char *pszId = "NotDetected";
	const char *pszNickName = "NotDetected";

	CSharedDoc *pDoc = WSingleton<CSharedDoc>::Instance();
	if (pDoc != NULL && strlen(pDoc->GetPangyaUserInfo().id) != 0) {
		uid = MyUID();
		pszId = pDoc->GetPangyaUserInfo().id;
		pszNickName = pDoc->GetPangyaUserInfo().nickname;
	}

	char szNickName[64] = { 0 };
	S5::UTIL::Base64Encode((char *)pszNickName, strlen(pszNickName), szNickName);

	LogPrintf("%d, %s, %d, %s, %s\n", -1, pszId, uid, szNickName, "645.00");
}

// exceptionreport.cpp:524
void CExceptionReport::EndStackLog(void) {
	if (m_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFile);

	m_hFile = INVALID_HANDLE_VALUE;
}

// exceptionreport.cpp:534
void CExceptionReport::DumpHeader(EXCEPTION_POINTERS *pExceptionInfo) const {
	char szModule[MAX_PATH];

	GetModuleName((LPCVOID)pExceptionInfo->ContextRecord->Eip, szModule, MAX_PATH);
	GetFileName(szModule, szModule);

	const char *pszId = "NotDetected";
	CSharedDoc *pDoc = WSingleton<CSharedDoc>::Instance();
	if (pDoc != NULL && strlen(pDoc->GetPangyaUserInfo().id) != 0)
		pszId = pDoc->GetPangyaUserInfo().id;

	LogPrintf("<%s %s %s %04x:%08x %s>\r\n", pszId, PY_PUBLIC_VERSION, "645.00",
		pExceptionInfo->ContextRecord->SegCs, pExceptionInfo->ContextRecord->Eip, szModule);

	LogPrintf("==============================================================================\r\n");
	LogPrintf("  Pangya (Version: %s, Packet Version: %d)\r\n", PY_PUBLIC_VERSION, 0x77DEF61B);
	LogPrintf("==============================================================================\r\n");
}

// exceptionreport.cpp:593
void CExceptionReport::DumpErrorMessage(const CONTEXT *pContext, const EXCEPTION_RECORD *pRecord) const {
	char szModule[MAX_PATH];

	GetModuleName((LPCVOID)pContext->Eip, szModule, MAX_PATH);

	LogPrintf("\r\n");
	LogPrintf("Program:   %s\r\n", szModule);

	LogPrintf("Exception: %08x (%s) at %04x:%08x.\r\n", pRecord->ExceptionCode,
		GetExceptionDescription(pRecord), pContext->SegCs, pContext->Eip);

	if (pRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && pRecord->NumberParameters >= 2) {
		LogPrintf("\r\nThe instruction at \"%08x\" referenced memory at \"%08x\"\r\nThe memory could not be %s\r\n",
			pContext->Eip, pRecord->ExceptionInformation[1],
			pRecord->ExceptionInformation[0] ? "\"write\"" : "\"read\"");
	}

	LogPrintf("\r\n");
}

// exceptionreport.cpp:619
void CExceptionReport::DumpRegisters(const CONTEXT *pContext) const {
	LogPrintf("----------------------------------------\r\n");
	LogPrintf("  x86 Registers\r\n");
	LogPrintf("----------------------------------------\r\n");
	LogPrintf("EAX=%08x  CS=%04x  EIP=%08x  EFLGS=%08x\r\n", pContext->Eax, pContext->SegCs, pContext->Eip, pContext->EFlags);
	LogPrintf("EBX=%08x  SS=%04x  ESP=%08x  EBP=%08x\r\n", pContext->Ebx, pContext->SegSs, pContext->Esp, pContext->Ebp);
	LogPrintf("ECX=%08x  DS=%04x  ESI=%08x  FS=%04x\r\n", pContext->Ecx, pContext->SegDs, pContext->Esi, pContext->SegFs);
	LogPrintf("EDX=%08x  ES=%04x  EDI=%08x  GS=%04x\r\n", pContext->Edx, pContext->SegEs, pContext->Edi, pContext->SegGs);
	LogPrintf("\r\n");
}

// exceptionreport.cpp:631
void CExceptionReport::PrintStack(unsigned long dwFrom, unsigned long dwTo) const {
	char szBuffer[92] = { 0 };

	unsigned long dwStack = dwFrom & ~0x1F;
	if (dwStack < g_dwStack)
		dwStack = g_dwStack;

	if (dwTo > g_dwStackBottom)
		dwTo = g_dwStackBottom;
	else
		dwTo = (dwTo + 0x1F) & ~0x1F;

	__try {
		char *p = szBuffer;

		while (dwStack < dwTo) {
			if ((dwStack & 0x1F) == 0)
				p += wsprintfA(p, "%08x:", dwStack);

			p += wsprintfA(p, " %08x", *(const unsigned long *)dwStack);

			dwStack += 4;
			if ((dwStack & 0x1F) == 0) {
				LogPrintf("%s\r\n", szBuffer);
				szBuffer[0] = '\0';
				p = szBuffer;
			}
		}
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		LogPrintf("*** Exception encountered during stack dump\r\n", dwStack);
		g_dwStackBottom = dwStack;
	}
}

// exceptionreport.cpp:672
void CExceptionReport::IntelStackWalk(CONTEXT *pContext) const {
	unsigned long dwFrame = pContext->Ebp;
	unsigned long dwPC = pContext->Eip;
	char szModule[MAX_PATH];

	for (;;) {
		GetModuleName((LPCVOID)dwPC, szModule, MAX_PATH);

		LogPrintf("Module=%s\r\n", szModule);

		LogPrintf("Frame=%08x\r\n", dwFrame);
		PrintStack(dwFrame, dwFrame + 0xA0);

		LogPrintf("Address=%08x: ", dwPC);
		const unsigned char *pCode = (const unsigned char *)dwPC;
		for (int i = 0; i < 16; i++) {
			__try {
				LogPrintf("%02x ", pCode[i]);
			} __except (EXCEPTION_EXECUTE_HANDLER) {
				LogPrintf("?? ");
			}
		}
		LogPrintf("\r\n\r\n");

		unsigned long dwNextFrame = ((const unsigned long *)dwFrame)[0];

		dwPC = ((const unsigned long *)dwFrame)[1];

		if (dwNextFrame & 3)
			break;

		if (dwNextFrame <= dwFrame)
			break;

		if (IsBadWritePtr((LPVOID)dwNextFrame, 8))
			break;

		dwFrame = dwNextFrame;
	}
}

// exceptionreport.cpp:727
void CExceptionReport::ImageHelpStackWalk(CONTEXT *pContext) const {
	STACKFRAME sf;

	memset(&sf, 0, sizeof(sf));

	sf.AddrPC.Offset = pContext->Eip;
	sf.AddrPC.Mode = AddrModeFlat;
	sf.AddrStack.Offset = pContext->Esp;
	sf.AddrStack.Mode = AddrModeFlat;
	sf.AddrFrame.Offset = pContext->Ebp;
	sf.AddrFrame.Mode = AddrModeFlat;

	while (s_pfnStackWalk(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(), GetCurrentThread(), &sf, pContext,
		NULL, s_pfnSymFunctionTableAccess, s_pfnSymGetModuleBase, NULL)) {
		if (sf.AddrFrame.Offset == 0)
			break;

		LogPrintf("Frame=%08x\r\n", sf.AddrFrame.Offset);

		PrintStack(sf.AddrFrame.Offset, sf.AddrFrame.Offset + 0x28);

		BYTE symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 1024];
		PIMAGEHLP_SYMBOL pSymbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
		pSymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
		pSymbol->MaxNameLength = 1024;

		char szUndName[512];
		szUndName[0] = '\0';

		DWORD dwDisplacement = 0;

		LogPrintf("Address=%08x\r\n", sf.AddrPC.Offset);

		bool bFound = false;
		if (s_pfnSymGetSymFromAddr(GetCurrentProcess(), sf.AddrPC.Offset, &dwDisplacement, pSymbol)) {
			LogPrintf("%s +0x%x\r\n", pSymbol->Name, dwDisplacement);
			s_pfnUnDecorateSymbolName(pSymbol->Name, szUndName, sizeof(szUndName), 0);

			IMAGEHLP_LINE64 line;
			DWORD dwLineDisplacement = 0;
			line.SizeOfStruct = sizeof(line);
			if (SymGetLineFromAddr64(GetCurrentProcess(), sf.AddrPC.Offset, &dwLineDisplacement, &line)) {
				LogPrintf("%s(%d) : %s\r\n", line.FileName, line.LineNumber, szUndName);
				bFound = true;
			}
		}

		if (!bFound) {
			char szModule[MAX_PATH];
			GetModuleName((LPCVOID)sf.AddrPC.Offset, szModule, MAX_PATH);
			LogPrintf("%s %s\r\n", szModule, szUndName);
		}

		LogPrintf("Params: %08x %08x %08x %08x\r\n\r\n", sf.Params[0], sf.Params[1], sf.Params[2], sf.Params[3]);
	}
}

// exceptionreport.cpp:792
void CExceptionReport::DumpStackTrace(const CONTEXT *pContext) const {
	g_dwStack = pContext->Esp & ~0x1F;

	g_dwStackBottom = __readfsdword(4);

	g_dwStackBottom &= ~0x1F;

	LogPrintf("----------------------------------------\r\n");
	LogPrintf("  Stack Trace (Using DBGHELP.DLL)\r\n");
	LogPrintf("----------------------------------------\r\n");

	if (s_pfnSymInitialize(GetCurrentProcess(), NULL, TRUE)) {
		DWORD dwOptions = s_pfnSymGetOptions();
		if ((dwOptions & (SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES)) != (SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES))
			s_pfnSymSetOptions((dwOptions & ~SYMOPT_UNDNAME) | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);

		CONTEXT context = *pContext;
		ImageHelpStackWalk(&context);
		s_pfnSymCleanup(GetCurrentProcess());
	}
}

// exceptionreport.cpp:833
void CExceptionReport::DumpMemory(const CONTEXT *pContext) const {
	LogPrintf("----------------------------------------\r\n");
	LogPrintf("  Memory dump\r\n");
	LogPrintf("----------------------------------------\r\n");

	LogPrintf("Code: %d Bytes starting at CS:EIP = %08x:%08x\r\n\r\n", 16, pContext->SegCs, pContext->Eip);

	const unsigned char *pMemory;
	unsigned long i;
	unsigned long j;

	pMemory = (const unsigned char *)pContext->Eip;
	for (i = 0; i < 16; i++) {
		if (((i + 1) & 0xF) == 1)
			LogPrintf("%08x: ", pMemory + i);

		__try {
			LogPrintf("%02x ", pMemory[i]);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			LogPrintf("?? ");
		}

		if (((i + 1) & 3) == 0)
			LogPrintf(" ");

		if (((i + 1) & 0xF) == 0) {
			for (j = i - 16; j < i; j++) {
				__try {
					LogPrintf("%c", isprint(pMemory[j]) ? pMemory[j] : '.');
				} __except (EXCEPTION_EXECUTE_HANDLER) {
					LogPrintf("?");
				}
			}

			LogPrintf("\r\n");
		}
	}

	LogPrintf("\r\n");

	LogPrintf("Stack: %d Bytes starting at ESP = %08x\r\n", 0x400, pContext->Esp);
	LogPrintf("\r\n* = addr\r\n");

	LogPrintf("          ");
	for (unsigned long k = 0; k < (pContext->Esp & 0xF); k++) {
		LogPrintf("   ");
		if (((k + 1) & 3) == 0)
			LogPrintf(" ");
	}
	LogPrintf("**\r\n");

	pMemory = (const unsigned char *)(pContext->Esp - (pContext->Esp & 0xF));
	for (i = 0; i < 0x400; i++) {
		if (((i + 1) & 0xF) == 1)
			LogPrintf("%08x: ", pMemory + i);

		__try {
			LogPrintf("%02x ", pMemory[i]);
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			LogPrintf("?? ");
		}

		if (((i + 1) & 3) == 0)
			LogPrintf(" ");

		if (((i + 1) & 0xF) == 0) {
			for (j = i - 16; j < i; j++) {
				__try {
					LogPrintf("%c", isprint(pMemory[j]) ? pMemory[j] : '.');
				} __except (EXCEPTION_EXECUTE_HANDLER) {
					LogPrintf("?");
				}
			}

			LogPrintf("\r\n");
		}
	}

	LogPrintf("\r\n");
}

// exceptionreport.cpp:1024
void CExceptionReport::DumpExceptionReport(EXCEPTION_POINTERS *pExceptionInfo) {
	SYSTEMTIME st;
	CONTEXT context;
	EXCEPTION_RECORD record;
	EXCEPTION_POINTERS exceptionPointers;
	char szName[64];
	char szFileName[64];

	GetLocalTime(&st);

	wsprintfA(m_szTime, "%04d/%02d/%02d %02d:%02d:%02d.%03d %s", st.wYear, st.wMonth, st.wDay,
		(st.wHour % 12) == 0 ? 12 : (st.wHour % 12), st.wMinute, st.wSecond, st.wMilliseconds,
		st.wHour >= 12 ? "PM" : "AM");

	strcpy(szName, "exception");

	if (pExceptionInfo == NULL) {
		SnapCurrentContext(&context);

		memset(&record, 0, sizeof(record));
		record.ExceptionAddress = (PVOID)context.Eip;

		memset(&exceptionPointers, 0, sizeof(exceptionPointers));
		exceptionPointers.ContextRecord = &context;
		exceptionPointers.ExceptionRecord = &record;

		pExceptionInfo = &exceptionPointers;
	}

	wsprintfA(szFileName, "%s.dmp", szName);
	WriteMiniDump(szFileName, pExceptionInfo);

	CollectSystemInfo();

	wsprintfA(szFileName, "%s.log", szName);
	StartLog(szFileName);

	DumpHeader(pExceptionInfo);

	DumpSystemInfo();

	DumpErrorMessage(pExceptionInfo->ContextRecord, pExceptionInfo->ExceptionRecord);

	DumpRegisters(pExceptionInfo->ContextRecord);

	DumpStackTrace(pExceptionInfo->ContextRecord);

	DumpMemory(pExceptionInfo->ContextRecord);

	EndLog();

	wsprintfA(szFileName, "%s.log", "stack");
	StartLog(szFileName);
	DumpHeaderInfo(pExceptionInfo);
	EndStackLog();
}

// exceptionreport.cpp:1099
void CExceptionReport::SetThreadInfo(void *hThread, unsigned long dwThreadId) {
	m_curThread = hThread;
	m_curThreadId = dwThreadId;
}

// exceptionreport.cpp:1105
void *CExceptionReport::GetCurrentThread(void) {
	if (m_curThread == INVALID_HANDLE_VALUE)
		m_curThread = ::GetCurrentThread();

	return m_curThread;
}

// exceptionreport.cpp:1113
unsigned long CExceptionReport::GetCurrentThreadId(void) {
	if (m_curThreadId == 0)
		m_curThreadId = ::GetCurrentThreadId();

	return m_curThreadId;
}
