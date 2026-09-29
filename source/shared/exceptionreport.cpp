#include "minatl.h"
#include "projectg.h"
#include "networksystem.h"
#include "packet.h"
#include "packetversion.h"
#include "dxdiaginfo.h"
#include "s5/utilities.h"
#include <stdio.h>
#include <dbghelp.h>

extern "C" void* _ReturnAddress(void);
#pragma intrinsic(_ReturnAddress)

typedef BOOL(__stdcall* SYMCLEANUPPROC)(HANDLE);
typedef PVOID(__stdcall* SYMFUNCTIONTABLEACCESSPROC)(HANDLE, DWORD);
typedef DWORD(__stdcall* SYMGETMODULEBASEPROC)(HANDLE, DWORD);
typedef BOOL(__stdcall* SYMGETMODULEINFOPROC)(HANDLE, DWORD, PIMAGEHLP_MODULE);
typedef DWORD(__stdcall* SYMGETOPTIONSPROC)();
typedef BOOL(
	__stdcall* SYMGETSYMFROMADDRPROC)(HANDLE, DWORD, PDWORD, PIMAGEHLP_SYMBOL);
typedef BOOL(__stdcall* SYMINITIALIZEPROC)(HANDLE, PSTR, BOOL);
typedef DWORD(__stdcall* SYMSETOPTIONSPROC)(DWORD);
typedef BOOL(__stdcall* STACKWALKPROC)(DWORD, HANDLE, HANDLE, LPSTACKFRAME,
	PVOID, PREAD_PROCESS_MEMORY_ROUTINE, PFUNCTION_TABLE_ACCESS_ROUTINE,
	PGET_MODULE_BASE_ROUTINE, PTRANSLATE_ADDRESS_ROUTINE);
typedef DWORD(__stdcall* UNDECORATESYMBOLNAMEPROC)(PCSTR, PSTR, DWORD, DWORD);
typedef BOOL(
	__stdcall* SYMLOADMODULEPROC)(HANDLE, HANDLE, PSTR, PSTR, DWORD, DWORD);
typedef BOOL(
	__stdcall* SYMGETLINEFROMADDRPROC)(HANDLE, DWORD, PDWORD, PIMAGEHLP_LINE);

CExceptionReport g_exceptionReport;

_client::CCriticalSection g_criticalSection;

static UNDECORATESYMBOLNAMEPROC s_pfnUnDecorateSymbolName;
static STACKWALKPROC s_pfnStackWalk;
static SYMSETOPTIONSPROC s_pfnSymSetOptions;
static SYMLOADMODULEPROC s_pfnSymLoadModule;
static SYMINITIALIZEPROC s_pfnSymInitialize;
static SYMGETSYMFROMADDRPROC s_pfnSymGetSymFromAddr;
static SYMGETOPTIONSPROC s_pfnSymGetOptions;
static SYMGETMODULEINFOPROC s_pfnSymGetModuleInfo;
static SYMGETMODULEBASEPROC s_pfnSymGetModuleBase;
static SYMGETLINEFROMADDRPROC s_pfnSymGetLineFromAddr;
static SYMFUNCTIONTABLEACCESSPROC s_pfnSymFunctionTableAccess;
static SYMCLEANUPPROC s_pfnSymCleanup;
LPTOP_LEVEL_EXCEPTION_FILTER g_pPrevUnhandledExceptionFilter;
DWORD g_dwStack;
DWORD g_dwStackBottom;
HINSTANCE g_hDLL;

__declspec(thread) HANDLE CExceptionReport::m_curThread = INVALID_HANDLE_VALUE;
__declspec(thread) unsigned long CExceptionReport::m_curThreadId = 0;

static bool LoadDbgHelp()
{
	HINSTANCE hDLL = LoadLibraryA("dbghelp.dll");
	if (!hDLL && !(hDLL = LoadLibraryA("imagehlp.dll")))
		return false;

	s_pfnSymCleanup = (SYMCLEANUPPROC)GetProcAddress(hDLL, "SymCleanup");
	s_pfnSymFunctionTableAccess = (SYMFUNCTIONTABLEACCESSPROC)GetProcAddress(
		hDLL, "SymFunctionTableAccess");
	s_pfnSymGetModuleBase =
		(SYMGETMODULEBASEPROC)GetProcAddress(hDLL, "SymGetModuleBase");
	s_pfnSymGetModuleInfo =
		(SYMGETMODULEINFOPROC)GetProcAddress(hDLL, "SymGetModuleInfo");
	s_pfnSymGetOptions =
		(SYMGETOPTIONSPROC)GetProcAddress(hDLL, "SymGetOptions");
	s_pfnSymGetSymFromAddr =
		(SYMGETSYMFROMADDRPROC)GetProcAddress(hDLL, "SymGetSymFromAddr");
	s_pfnSymInitialize =
		(SYMINITIALIZEPROC)GetProcAddress(hDLL, "SymInitialize");
	s_pfnSymSetOptions =
		(SYMSETOPTIONSPROC)GetProcAddress(hDLL, "SymSetOptions");
	s_pfnStackWalk = (STACKWALKPROC)GetProcAddress(hDLL, "StackWalk");
	s_pfnUnDecorateSymbolName =
		(UNDECORATESYMBOLNAMEPROC)GetProcAddress(hDLL, "UnDecorateSymbolName");
	s_pfnSymLoadModule =
		(SYMLOADMODULEPROC)GetProcAddress(hDLL, "SymLoadModule");
	s_pfnSymGetLineFromAddr =
		(SYMGETLINEFROMADDRPROC)GetProcAddress(hDLL, "SymGetLineFromAddr");

	if (s_pfnSymCleanup && s_pfnSymFunctionTableAccess &&
		s_pfnSymGetModuleBase && s_pfnSymGetModuleInfo && s_pfnSymGetOptions &&
		s_pfnSymGetSymFromAddr && s_pfnSymInitialize && s_pfnSymSetOptions &&
		s_pfnStackWalk && s_pfnUnDecorateSymbolName && s_pfnSymLoadModule)
	{
		g_hDLL = hDLL;
		return true;
	}

	FreeLibrary(hDLL);
	return false;
}

static void FreeDbgHelp()
{
	if (g_hDLL)
	{
		FreeLibrary(g_hDLL);
		g_hDLL = NULL;
	}
}

static BOOL GetModulePath(const void* pAddress, char* pszPath, DWORD dwSize)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (VirtualQuery(pAddress, &mbi, sizeof(mbi)) &&
		GetModuleFileNameA((HMODULE)mbi.AllocationBase, pszPath, dwSize) > 0)
		return TRUE;

	strncpy(pszPath, "Unknown", dwSize);
	return FALSE;
}

static const char* GetExceptionString(const EXCEPTION_RECORD* pRecord)
{
	switch (pRecord->ExceptionCode)
	{
	case DBG_CONTROL_C:
		return "Control-C";
	case DBG_CONTROL_BREAK:
		return "Control-Break";
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		return "Datatype Misalignment";
	case EXCEPTION_BREAKPOINT:
		return "Breakpoint";
	case EXCEPTION_ACCESS_VIOLATION:
		return "Access Violation";
	case EXCEPTION_IN_PAGE_ERROR:
		return "an In Page Error";
	case STATUS_NO_MEMORY:
		return "No Memory";
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		return "Illegal Instruction";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		return "Noncontinuable Exception";
	case EXCEPTION_INVALID_DISPOSITION:
		return "Invalid Disposition";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		return "Array Bounds Exceeded";
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		return "Float Denormal Operand";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		return "Float Divide by Zero";
	case EXCEPTION_FLT_INEXACT_RESULT:
		return "Float Inexact Result";
	case EXCEPTION_FLT_INVALID_OPERATION:
		return "Float Invalid Operation";
	case EXCEPTION_FLT_OVERFLOW:
		return "Float Overflow";
	case EXCEPTION_FLT_STACK_CHECK:
		return "Float Stack Check";
	case EXCEPTION_FLT_UNDERFLOW:
		return "Float Underflow";
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		return "Integer Divide by Zero";
	case EXCEPTION_INT_OVERFLOW:
		return "Integer Overflow";
	case EXCEPTION_PRIV_INSTRUCTION:
		return "Privileged Instruction";
	case EXCEPTION_STACK_OVERFLOW:
		return "Stack Overflow";
	case 0xc0000142:
		return "DLL Initialization Failed";
	case 0xe06d7363:
		return "Microsoft C++ Exception";
	case 0x60000000:
		return "PacketDecodeException";
	case EXCEPTION_WITH_USER_INFO:
	{
		static __declspec(thread) char szMessage[8192];

		if (pRecord->NumberParameters >= 3)
			sprintf(szMessage,
				"EXCEPTION_WITH_USER_INFO => msg: %s, arg1: %d, arg2: %d",
				pRecord->ExceptionInformation[0],
				pRecord->ExceptionInformation[1],
				pRecord->ExceptionInformation[2]);
		else
			sprintf(szMessage,
				"EXCEPTION_WITH_USER_INFO => invalid parameter count(%d)",
				pRecord->NumberParameters);
		return szMessage;
	}
	default:
		return "Unknown exception type";
	}
}

static void MakeMiniDump(const char* pszFileName,
	EXCEPTION_POINTERS* pExceptionInfo)
{
	MINIDUMP_EXCEPTION_INFORMATION info;
	PMINIDUMP_EXCEPTION_INFORMATION pInfo;

	HANDLE hFile = CreateFileA(pszFileName, GENERIC_WRITE, FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	SetThreadPriority(CExceptionReport::GetCurrentThread(),
		THREAD_PRIORITY_HIGHEST);

	if (pExceptionInfo)
	{
		info.ThreadId = CExceptionReport::GetCurrentThreadId();
		info.ClientPointers = TRUE;
		info.ExceptionPointers = pExceptionInfo;
		pInfo = &info;
	}
	else
	{
		pInfo = NULL;
	}

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile,
		MiniDumpNormal, pInfo, NULL, NULL);

	SetThreadPriority(CExceptionReport::GetCurrentThread(),
		THREAD_PRIORITY_NORMAL);
	CloseHandle(hFile);
}

void __cdecl SecurityErrorHandler(int code, void* data)
{
	if (g_exceptionReport.IsEnabled())
	{
		_client::_private::CLock<_client::CCriticalSection> lock(
			g_criticalSection);

		g_exceptionReport.DumpExceptionReport(NULL);
	}
	ExitProcess(1);
}

long __stdcall RecordExceptionInfo(EXCEPTION_POINTERS* pExceptionInfo)
{
	static BOOL s_bReentered = FALSE;

	char szMessage[1024] = { 0 };

	if (g_exceptionReport.IsEnabled())
	{
		_client::_private::CLock<_client::CCriticalSection> lock(
			g_criticalSection);

		if (s_bReentered)
			goto CallPreviousFilter;

		s_bReentered = TRUE;
		g_exceptionReport.DumpExceptionReport(pExceptionInfo);

		wsprintfA(szMessage,
			"%s Exception raised at 0x%08x. Program will be terminated.",
			GetExceptionString(pExceptionInfo->ExceptionRecord),
			pExceptionInfo->ExceptionRecord->ExceptionAddress);
	}
	else
	{
		MessageBoxA(NULL, szMessage, "g_exceptionReport.IsEnabled() == false",
			MB_OK);
	}

	if (WNetworkSystem::Instance())
	{
		std::string strType("Exception");
		WSendPacket packet((enumClientPacket)0x33);
		packet.Encode1(0);
		packet.EncodeStr(strType);
		packet.Send((eSendTo)0);
	}

CallPreviousFilter:
	if (g_pPrevUnhandledExceptionFilter)
		return g_pPrevUnhandledExceptionFilter(pExceptionInfo);

	return EXCEPTION_CONTINUE_SEARCH;
}

void CExceptionReport::Init()
{
	LoadDbgHelp();
	g_pPrevUnhandledExceptionFilter =
		SetUnhandledExceptionFilter(RecordExceptionInfo);
	_set_security_error_handler(SecurityErrorHandler);
	SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX |
		SEM_NOOPENFILEERRORBOX);
	m_bEnable = TRUE;
	m_bInit = true;
}

void CExceptionReport::Shutdown()
{
	if (g_pPrevUnhandledExceptionFilter)
		SetUnhandledExceptionFilter(g_pPrevUnhandledExceptionFilter);
	SetErrorMode(0);
	FreeDbgHelp();
	m_bInit = false;
}

int CExceptionReport::Enable()
{
	return InterlockedExchange(&m_bEnable, TRUE) == FALSE;
}

int CExceptionReport::Disable()
{
	return InterlockedExchange(&m_bEnable, FALSE);
}

int CExceptionReport::IsEnabled() const
{
	return m_bEnable;
}

void CExceptionReport::StartLog(const char* pszFileName)
{
	m_hReportFile =
		CreateFileA(pszFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
}

void CExceptionReport::LogPrintf(const char* pszFormat, ...) const
{
	char szBuffer[4096];
	va_list args;
	va_start(args, pszFormat);
	vsprintf(szBuffer, pszFormat, args);
	DWORD dwWritten;
	WriteFile(m_hReportFile, szBuffer, strlen(szBuffer), &dwWritten, NULL);
	va_end(args);
}

void CExceptionReport::EndLog()
{
	LogPrintf(
		"--------------------------------- END_OF_LOG ---------------------------------\r\n");
	if (m_hReportFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hReportFile);
	m_hReportFile = INVALID_HANDLE_VALUE;
}

void CExceptionReport::CollectSystemInfo()
{
	OSVERSIONINFOA osvi;
	const char* pszName;

	memset(&osvi, 0, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	if (GetVersionExA(&osvi))
	{
		switch (osvi.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_WINDOWS:
			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
				pszName = "Windows 95";
			else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
				pszName = "Windows 98/98SE";
			else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
				pszName = "Windows ME";
			else
				pszName = "Windows ME or later";
			break;
		case VER_PLATFORM_WIN32_NT:
			if (osvi.dwMajorVersion <= 4)
				pszName = "Windows NT";
			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
				pszName = "Windows 2000";
			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
				pszName = "Microsoft Windows XP";
			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
				pszName = "Microsoft Windows Server 2003 family";
			else
				pszName = "Microsoft Vista or later";
			break;
		default:
			pszName = "Unknown Windows variant";
			break;
		}

		wsprintfA(m_szOSVersion, "%s %i.%02d.%i %s", pszName,
			osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber,
			osvi.szCSDVersion);
	}
	else
	{
		strcpy(m_szOSVersion, "Unknown");
	}

	GetSystemInfo(&m_systemInfo);

	m_memoryStatus.dwLength = sizeof(m_memoryStatus);
	GlobalMemoryStatus(&m_memoryStatus);

	DWORD dwSize = 200;
	if (!GetUserNameA(m_szUserName, &dwSize))
		strcpy(m_szUserName, "Unknown");

	dwSize = 200;
	if (!GetComputerNameA(m_szComputerName, &dwSize))
		strcpy(m_szComputerName, "Unknown");
}

static void GetBaseName(const char* pszSrc, char* pszDst)
{
	const char* p = pszSrc + strlen(pszSrc) - 1;
	while (p != pszSrc && p[-1] != '\\' && p[-1] != '/')
		--p;

	while (*p)
		*pszDst++ = *p++;
	*pszDst = '\0';
}

void CExceptionReport::DumpHeaderInfo(EXCEPTION_POINTERS* pExceptionInfo)
{
	char* pszNick = "NotDetected";
	int nUID = -1;
	char* pszID = "NotDetected";

	if (Doc() && strlen(MyId()))
	{
		pszID = MyId();
		nUID = MyUID();
		pszNick = MyNick();
	}

	char szEncodedNick[64] = { 0 };
	S5::UTIL::Base64Encode(pszNick, strlen(pszNick), szEncodedNick);

	LogPrintf("%d, %s, %d, %s, %s\n", -1, pszID, nUID, szEncodedNick,
		PY_CLIENT_VERSION);
}

void CExceptionReport::EndStackLog()
{
	if (m_hReportFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hReportFile);
	m_hReportFile = INVALID_HANDLE_VALUE;
}

void CExceptionReport::DumpHeader(EXCEPTION_POINTERS* pExceptionInfo) const
{
	char szPath[MAX_PATH];
	GetModulePath((void*)pExceptionInfo->ContextRecord->Eip, szPath, MAX_PATH);
	GetBaseName(szPath, szPath);

	char* pszID = "NotDetected";
	if (Doc() && strlen(MyId()))
		pszID = MyId();

	LogPrintf("<%s %s %s %04x:%08x %s>\r\n", pszID, PY_PUBLIC_VERSION,
		PY_CLIENT_VERSION, pExceptionInfo->ContextRecord->SegCs,
		pExceptionInfo->ContextRecord->Eip, szPath);
	LogPrintf(
		"==============================================================================\r\n");
	LogPrintf("  Pangya (Version: %s, Packet Version: %d)\r\n",
		PY_PUBLIC_VERSION, PY_PACKET_VERSION);
	LogPrintf(
		"==============================================================================\r\n");
}

void CExceptionReport::DumpSystemInfo() const
{
	LogPrintf("Time:      %s\r\n", m_szTime);
	LogPrintf("User:      %s\r\n", m_szUserName);
	LogPrintf("Computer:  %s\r\n", m_szComputerName);
	if (CProjectG::Instance() && CProjectG::Instance()->m_pDxDiagInfo)
		LogPrintf("OS:        %s\r\n",
			CProjectG::Instance()->m_pDxDiagInfo->GetOSName());
	else
		LogPrintf("OS:        %s\r\n", m_szOSVersion);
	LogPrintf("Processor: %d processor(s), type %d.\r\n",
		m_systemInfo.dwNumberOfProcessors, m_systemInfo.dwProcessorType);
	LogPrintf("Memory:    %d MBytes physical memory.\r\n",
		(m_memoryStatus.dwTotalPhys + 0xfffff) >> 20);
	if (CProjectG::Instance() && CProjectG::Instance()->m_pDxDiagInfo)
	{
		LogPrintf("Graphic Card: %s\r\n",
			CProjectG::Instance()->m_pDxDiagInfo->GetGraphicCardInfo());
		LogPrintf("CPU:       %s\r\n",
			CProjectG::Instance()->m_pDxDiagInfo->GetCPUInfo());
	}
	LogPrintf(
		"------------------------------------------------------------------------------\r\n");
}

void CExceptionReport::DumpErrorMessage(const CONTEXT* pContext,
	const EXCEPTION_RECORD* pRecord) const
{
	char szPath[MAX_PATH];
	GetModulePath((void*)pContext->Eip, szPath, MAX_PATH);

	LogPrintf("\r\n");
	LogPrintf("Program:   %s\r\n", szPath);
	LogPrintf("Exception: %08x (%s) at %04x:%08x.\r\n", pRecord->ExceptionCode,
		GetExceptionString(pRecord), pContext->SegCs, pContext->Eip);

	if (pRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION &&
		pRecord->NumberParameters >= 2)
	{
		LogPrintf(
			"\r\nThe instruction at \"%08x\" referenced memory at \"%08x\"\r\nThe memory could not be %s\r\n",
			pContext->Eip, pRecord->ExceptionInformation[1],
			pRecord->ExceptionInformation[0] ? "\"write\"" : "\"read\"");
	}

	LogPrintf("\r\n");
}

void CExceptionReport::DumpRegisters(const CONTEXT* pContext) const
{
	LogPrintf("----------------------------------------\r\n");
	LogPrintf("  x86 Registers\r\n");
	LogPrintf("----------------------------------------\r\n");
	LogPrintf("EAX=%08x  CS=%04x  EIP=%08x  EFLGS=%08x\r\n", pContext->Eax,
		pContext->SegCs, pContext->Eip, pContext->EFlags);
	LogPrintf("EBX=%08x  SS=%04x  ESP=%08x  EBP=%08x\r\n", pContext->Ebx,
		pContext->SegSs, pContext->Esp, pContext->Ebp);
	LogPrintf("ECX=%08x  DS=%04x  ESI=%08x  FS=%04x\r\n", pContext->Ecx,
		pContext->SegDs, pContext->Esi, pContext->SegFs);
	LogPrintf("EDX=%08x  ES=%04x  EDI=%08x  GS=%04x\r\n", pContext->Edx,
		pContext->SegEs, pContext->Edi, pContext->SegGs);
	LogPrintf("\r\n");
}

void CExceptionReport::PrintStack(unsigned long dwBegin,
	unsigned long dwEnd) const
{
	char szLine[92] = { 0 };

	dwBegin &= ~31;
	if (dwBegin < g_dwStack)
		dwBegin = g_dwStack;

	if (dwEnd > g_dwStackBottom)
		dwEnd = g_dwStackBottom;
	else
		dwEnd = (dwEnd + 31) & ~31;

	__try
	{
		char* pszCursor = szLine;
		while (dwBegin < dwEnd)
		{
			if (!(dwBegin & 31))
				pszCursor += wsprintfA(pszCursor, "%08x:", dwBegin);
			pszCursor += wsprintfA(pszCursor, " %08x", *(DWORD*)dwBegin);
			dwBegin += 4;
			if (!(dwBegin & 31))
			{
				LogPrintf("%s\r\n", szLine);
				szLine[0] = '\0';
				pszCursor = szLine;
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LogPrintf("*** Exception encountered during stack dump\r\n", dwBegin);
		g_dwStackBottom = dwBegin;
	}
}

void CExceptionReport::IntelStackWalk(CONTEXT* pContext) const
{
	DWORD* pFrame = (DWORD*)pContext->Ebp;
	DWORD dwEip = pContext->Eip;
	DWORD* pNext;

	do
	{
		char szPath[MAX_PATH];
		GetModulePath((void*)dwEip, szPath, MAX_PATH);

		LogPrintf("Module=%s\r\n", szPath);
		LogPrintf("Frame=%08x\r\n", pFrame);
		PrintStack((DWORD)pFrame, (DWORD)(pFrame + 40));

		LogPrintf("Address=%08x: ", dwEip);
		BYTE* pCode = (BYTE*)dwEip;
		for (int i = 0; i < 16; i++)
		{
			__try
			{
				LogPrintf("%02x ", pCode[i]);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogPrintf("?? ");
			}
		}
		LogPrintf("\r\n\r\n");

		pNext = (DWORD*)pFrame[0];
		dwEip = pFrame[1];

		if ((DWORD)pNext & 3)
			break;

		if (pNext <= pFrame)
			break;

		if (IsBadWritePtr(pNext, sizeof(void*) * 2))
			break;

		pFrame = pNext;
	} while (1);
}

void CExceptionReport::ImageHelpStackWalk(CONTEXT* pContext) const
{
	STACKFRAME frame;
	memset(&frame, 0, sizeof(frame));
	frame.AddrPC.Offset = pContext->Eip;
	frame.AddrPC.Mode = AddrModeFlat;
	frame.AddrStack.Offset = pContext->Esp;
	frame.AddrStack.Mode = AddrModeFlat;
	frame.AddrFrame.Offset = pContext->Ebp;
	frame.AddrFrame.Mode = AddrModeFlat;

	while (1)
	{
		if (!s_pfnStackWalk(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(),
				GetCurrentThread(), &frame, pContext, NULL,
				s_pfnSymFunctionTableAccess, s_pfnSymGetModuleBase, NULL))
			break;

		if (frame.AddrFrame.Offset == 0)
			break;

		LogPrintf("Frame=%08x\r\n", frame.AddrFrame.Offset);
		PrintStack(frame.AddrFrame.Offset, frame.AddrFrame.Offset + 40);

		BYTE symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 1024];
		PIMAGEHLP_SYMBOL pSymbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
		pSymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
		pSymbol->MaxNameLength = 1024;

		char szUndecorated[512];
		szUndecorated[0] = '\0';

		DWORD dwDisplacement = 0;

		LogPrintf("Address=%08x\r\n", frame.AddrPC.Offset);
		if (s_pfnSymGetSymFromAddr(GetCurrentProcess(), frame.AddrPC.Offset,
				&dwDisplacement, pSymbol))
		{
			LogPrintf("%s +0x%x\r\n", pSymbol->Name, dwDisplacement);
			s_pfnUnDecorateSymbolName(pSymbol->Name, szUndecorated,
				sizeof(szUndecorated), UNDNAME_COMPLETE);

			IMAGEHLP_LINE64 line;
			DWORD dwLineDisplacement = 0;
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			if (SymGetLineFromAddr64(GetCurrentProcess(), frame.AddrPC.Offset,
					&dwLineDisplacement, &line))
			{
				LogPrintf("%s(%d) : %s\r\n", line.FileName, line.LineNumber,
					szUndecorated);
				goto PrintParams;
			}
		}

		{
			char szPath[MAX_PATH];
			GetModulePath((void*)frame.AddrPC.Offset, szPath, MAX_PATH);
			LogPrintf("%s %s\r\n", szPath, szUndecorated);
		}

PrintParams:
		LogPrintf("Params: %08x %08x %08x %08x\r\n\r\n", frame.Params[0],
			frame.Params[1], frame.Params[2], frame.Params[3]);
	}
}

void CExceptionReport::DumpStackTrace(const CONTEXT* pContext) const
{
	g_dwStack = pContext->Esp & ~31;

	__asm
		{
		mov eax, fs:[4]
		mov g_dwStackBottom, eax
		}
	g_dwStackBottom &= ~31;

	LogPrintf("----------------------------------------\r\n");
	LogPrintf("  Stack Trace (Using DBGHELP.DLL)\r\n");
	LogPrintf("----------------------------------------\r\n");

	if (!s_pfnSymInitialize(GetCurrentProcess(), NULL, TRUE))
		return;

	DWORD dwOptions = s_pfnSymGetOptions();
	if ((dwOptions &
			(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES)) !=
		(SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES))
		s_pfnSymSetOptions((dwOptions & ~SYMOPT_UNDNAME) |
			(SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES));

	CONTEXT context = *pContext;
	ImageHelpStackWalk(&context);

	s_pfnSymCleanup(GetCurrentProcess());
}

void CExceptionReport::DumpMemory(const CONTEXT* pContext) const
{
	LogPrintf("----------------------------------------\r\n");
	LogPrintf("  Memory dump\r\n");
	LogPrintf("----------------------------------------\r\n");
	LogPrintf("Code: %d Bytes starting at CS:EIP = %08x:%08x\r\n\r\n", 16,
		pContext->SegCs, pContext->Eip);

	BYTE* pBytes = (BYTE*)pContext->Eip;
	unsigned int i, j;
	for (i = 0; i < 16; i++)
	{
		if (((i + 1) & 15) == 1)
			LogPrintf("%08x: ", pBytes + i);

		__try
		{
			LogPrintf("%02x ", pBytes[i]);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogPrintf("?? ");
		}

		if (!((i + 1) & 3))
			LogPrintf(" ");

		if (!((i + 1) & 15))
		{
			for (j = i - 16; j < i; j++)
			{
				__try
				{
					LogPrintf("%c", isprint(pBytes[j]) ? pBytes[j] : '.');
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					LogPrintf("?");
				}
			}
			LogPrintf("\r\n");
		}
	}

	LogPrintf("\r\n");
	LogPrintf("Stack: %d Bytes starting at ESP = %08x\r\n", 1024,
		pContext->Esp);
	LogPrintf("\r\n* = addr\r\n");
	LogPrintf("          ");
	for (i = 0; i < (pContext->Esp & 15); i++)
	{
		LogPrintf("   ");
		if (!((i + 1) & 3))
			LogPrintf(" ");
	}
	LogPrintf("**\r\n");

	pBytes = (BYTE*)(pContext->Esp - (pContext->Esp & 15));
	for (i = 0; i < 1024; i++)
	{
		if (((i + 1) & 15) == 1)
			LogPrintf("%08x: ", pBytes + i);

		__try
		{
			LogPrintf("%02x ", pBytes[i]);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogPrintf("?? ");
		}

		if (!((i + 1) & 3))
			LogPrintf(" ");

		if (!((i + 1) & 15))
		{
			for (j = i - 16; j < i; j++)
			{
				__try
				{
					LogPrintf("%c", isprint(pBytes[j]) ? pBytes[j] : '.');
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					LogPrintf("?");
				}
			}
			LogPrintf("\r\n");
		}
	}
	LogPrintf("\r\n");
}

__declspec(naked) DWORD SnapCurrentContext(CONTEXT* pContext)
{
	CONTEXT ctx;
	BOOL bResult;

	__asm
		{
		push ebp
		mov ebp, esp
		sub esp, __LOCAL_SIZE

		mov ctx.Eax, eax
		mov ctx.Ebx, ebx
		mov ctx.Ecx, ecx
		mov ctx.Edx, edx
		mov ctx.Edi, edi
		mov ctx.Esi, esi

		xor eax, eax
		mov ax, gs
		mov ctx.SegGs, eax
		mov ax, fs
		mov ctx.SegFs, eax
		mov ax, es
		mov ctx.SegEs, eax
		mov ax, ds
		mov ctx.SegDs, eax
		mov ax, cs
		mov ctx.SegCs, eax
		mov ax, ss
		mov ctx.SegSs, eax

		mov eax, [ebp]
		mov ctx.Ebp, eax

		mov eax, ebp
		add eax, 8
		mov ctx.Esp, eax

		push esi
		push edi
		push ebx
		push ecx
		push edx
		}

	memset(pContext, 0, sizeof(CONTEXT));
	pContext->ContextFlags = CONTEXT_FULL | CONTEXT_FLOATING_POINT |
		CONTEXT_DEBUG_REGISTERS | CONTEXT_EXTENDED_REGISTERS;

	if (GetThreadContext(CExceptionReport::GetCurrentThread(), pContext))
	{
		pContext->Eax = ctx.Eax;
		pContext->Ebx = ctx.Ebx;
		pContext->Ecx = ctx.Ecx;
		pContext->Edx = ctx.Edx;
		pContext->Edi = ctx.Edi;
		pContext->Esi = ctx.Esi;
		pContext->SegGs = ctx.SegGs;
		pContext->SegFs = ctx.SegFs;
		pContext->SegEs = ctx.SegEs;
		pContext->SegDs = ctx.SegDs;
		pContext->SegCs = ctx.SegCs;
		pContext->SegSs = ctx.SegSs;
		pContext->Ebp = ctx.Ebp;
		pContext->Eip = (DWORD)_ReturnAddress();
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}

	__asm
	{
		pop edx
		pop ecx
		pop ebx
		pop edi
		pop esi
		mov eax, bResult
		mov esp, ebp
		pop ebp
		ret
	}
}

void CExceptionReport::DumpExceptionReport(EXCEPTION_POINTERS* pExceptionInfo)
{
	SYSTEMTIME st;
	char szName[64];
	char szFileName[64];
	CONTEXT context;
	EXCEPTION_RECORD record;
	EXCEPTION_POINTERS pointers;

	GetLocalTime(&st);
	wsprintfA(m_szTime, "%04d/%02d/%02d %02d:%02d:%02d.%03d %s", st.wYear,
		st.wMonth, st.wDay, (st.wHour % 12) == 0 ? 12 : st.wHour % 12,
		st.wMinute, st.wSecond, st.wMilliseconds, st.wHour >= 12 ? "PM" : "AM");

	strcpy(szName, "exception");

	if (!pExceptionInfo)
	{
		SnapCurrentContext(&context);

		memset(&record, 0, sizeof(record));
		record.ExceptionAddress = (PVOID)context.Eip;

		memset(&pointers, 0, sizeof(pointers));
		pointers.ContextRecord = &context;
		pointers.ExceptionRecord = &record;

		pExceptionInfo = &pointers;
	}

	wsprintfA(szFileName, "%s.dmp", szName);
	MakeMiniDump(szFileName, pExceptionInfo);

	CollectSystemInfo();
	wsprintfA(szFileName, "%s.log", szName);
	StartLog(szFileName);
	DumpHeader(pExceptionInfo);
	DumpSystemInfo();
	DumpErrorMessage(pExceptionInfo->ContextRecord,
		pExceptionInfo->ExceptionRecord);
	DumpRegisters(pExceptionInfo->ContextRecord);
	DumpStackTrace(pExceptionInfo->ContextRecord);
	DumpMemory(pExceptionInfo->ContextRecord);
	EndLog();

	wsprintfA(szFileName, "%s.log", "stack");
	StartLog(szFileName);
	DumpHeaderInfo(pExceptionInfo);
	EndStackLog();
}

void CExceptionReport::SetThreadInfo(HANDLE hThread, unsigned long dwThreadId)
{
	m_curThread = hThread;
	m_curThreadId = dwThreadId;
}

HANDLE CExceptionReport::GetCurrentThread()
{
	if (m_curThread == INVALID_HANDLE_VALUE)
		m_curThread = ::GetCurrentThread();
	return m_curThread;
}

unsigned long CExceptionReport::GetCurrentThreadId()
{
	if (!m_curThreadId)
		m_curThreadId = ::GetCurrentThreadId();
	return m_curThreadId;
}
