#pragma once

#define EXCEPTION_WITH_USER_INFO 0xe0000001

long __stdcall RecordExceptionInfo(EXCEPTION_POINTERS* pExceptionInfo);
void __cdecl SecurityErrorHandler(int code, void* data);

class CExceptionReport
{
	friend long __stdcall RecordExceptionInfo(
		EXCEPTION_POINTERS* pExceptionInfo);
	friend void __cdecl SecurityErrorHandler(int code, void* data);

private:
	void Init();
	void Shutdown();

	void StartLog(const char* pszFileName);
	void LogPrintf(const char* pszFormat, ...) const;
	void EndLog();

	void CollectSystemInfo();
	void DumpHeaderInfo(EXCEPTION_POINTERS* pExceptionInfo);
	void EndStackLog();
	void DumpHeader(EXCEPTION_POINTERS* pExceptionInfo) const;
	void DumpSystemInfo() const;
	void DumpErrorMessage(const CONTEXT* pContext,
		const EXCEPTION_RECORD* pRecord) const;
	void DumpRegisters(const CONTEXT* pContext) const;
	void PrintStack(unsigned long dwBegin, unsigned long dwEnd) const;
	void IntelStackWalk(CONTEXT* pContext) const;
	void ImageHelpStackWalk(CONTEXT* pContext) const;
	void DumpStackTrace(const CONTEXT* pContext) const;
	void DumpMemory(const CONTEXT* pContext) const;
	void DumpExceptionReport(EXCEPTION_POINTERS* pExceptionInfo);

	bool m_bInit;
	HANDLE m_hReportFile;
	long m_bEnable;
	char m_szTime[200];
	char m_szOSVersion[200];
	char m_szUserName[200];
	char m_szComputerName[200];
	SYSTEM_INFO m_systemInfo;
	MEMORYSTATUS m_memoryStatus;

	static __declspec(thread) HANDLE m_curThread;
	static __declspec(thread) unsigned long m_curThreadId;

public:
	static void SetThreadInfo(HANDLE hThread, unsigned long dwThreadId);
	static HANDLE GetCurrentThread();
	static unsigned long GetCurrentThreadId();

	CExceptionReport() { Init(); }

	~CExceptionReport() { Shutdown(); }

	int Enable();
	int Disable();
	int IsEnabled() const;

	void Crash() const { *(unsigned long*)0xdeaddead = 0xdeaddead; }
};

extern CExceptionReport g_exceptionReport;
