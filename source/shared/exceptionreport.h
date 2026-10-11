// Original: d:\build\custom\temp\code\shared\exceptionreport.h
#ifndef PG_SHARED_EXCEPTIONREPORT_H
#define PG_SHARED_EXCEPTIONREPORT_H

#include <windows.h>

#include "..\..\defines.h"
#include "..\..\padding.h"

// Writes a report of an unhandled exception (registers, stack, memory) and
// keeps the per-thread handle and id the report names.
class CExceptionReport {
public:
	CExceptionReport(void) {   // exceptionreport.h:86
		Init();
	}
	~CExceptionReport(void) {   // exceptionreport.h:91
		Shutdown();
	}

	int Enable(void);            // exceptionreport.cpp:372
	int Disable(void);           // exceptionreport.cpp:377
	int IsEnabled(void) const;   // exceptionreport.cpp:382

	static void SetThreadInfo(void *hThread, unsigned long dwThreadId);   // exceptionreport.cpp:1099
	static void *GetCurrentThread(void);                                  // exceptionreport.cpp:1105
	static unsigned long GetCurrentThreadId(void);                        // exceptionreport.cpp:1113

private:
	LAYOUT_CHECK_FRIEND;
	friend void __cdecl SecurityErrorHandler(int code, void *data);   // exceptionreport.cpp:257

	void Init(void);                                    // exceptionreport.cpp:339
	void Shutdown(void);                                // exceptionreport.cpp:360
	void StartLog(const char *pszFileName);             // exceptionreport.cpp:387
	void __cdecl LogPrintf(const char *pszFormat, ...) const;   // exceptionreport.cpp:396
	void EndLog(void);                                  // exceptionreport.cpp:409
	void CollectSystemInfo(void);
	void EndStackLog(void);                             // exceptionreport.cpp:524
	void DumpErrorMessage(const CONTEXT *pContext, const EXCEPTION_RECORD *pRecord) const;
	void DumpRegisters(const CONTEXT *pContext) const;
	void PrintStack(unsigned long dwFrom, unsigned long dwTo) const;
	void IntelStackWalk(CONTEXT *pContext) const;
	void DumpMemory(const CONTEXT *pContext) const;
	void DumpSystemInfo(void) const;
	void ImageHelpStackWalk(CONTEXT *pContext) const;
	void DumpStackTrace(const CONTEXT *pContext) const;   // exceptionreport.cpp:792
	void DumpHeaderInfo(EXCEPTION_POINTERS *pExceptionInfo);
	void DumpHeader(EXCEPTION_POINTERS *pExceptionInfo) const;
	void DumpExceptionReport(EXCEPTION_POINTERS *pExceptionInfo);

	// The thread the report names, per thread: GetCurrentThread and
	// GetCurrentThreadId fill them on first use unless SetThreadInfo did.
	static __declspec(thread) void *m_curThread;            // TLS 0x9174, -1 = not yet known
	static __declspec(thread) unsigned long m_curThreadId;  // TLS 0x9178, 0 = not yet known

	bool m_bInitialized;   // 0x0, set by Init, cleared by Shutdown
	UNKNOWN(0x1, 0x4);
	HANDLE m_hFile;        // 0x4, the open log, INVALID_HANDLE_VALUE when closed
	LONG m_bEnabled;       // 0x8, switched by Enable/Disable
	char m_szTime[0xC8];   // 0xC, the local time of the report (DumpExceptionReport)
	char m_szOS[0xC8];        // 0xD4, "<Windows variant> <major>.<minor>.<build> <CSD>" (CollectSystemInfo)
	char m_szUser[0xC8];      // 0x19C, GetUserName
	char m_szComputer[0xC8];  // 0x264, GetComputerName
	SYSTEM_INFO m_systemInfo;       // 0x32C
	MEMORYSTATUS m_memoryStatus;    // 0x350
};

CHECK_SIZE(CExceptionReport, 0x370);

BEGIN_LAYOUT_CHECK(CExceptionReport)
	CHECK_FIELD(CExceptionReport, m_bInitialized, 0x0);
	CHECK_FIELD(CExceptionReport, m_hFile, 0x4);
	CHECK_FIELD(CExceptionReport, m_bEnabled, 0x8);
	CHECK_FIELD(CExceptionReport, m_szTime, 0xC);
	CHECK_FIELD(CExceptionReport, m_szOS, 0xD4);
	CHECK_FIELD(CExceptionReport, m_szUser, 0x19C);
	CHECK_FIELD(CExceptionReport, m_szComputer, 0x264);
	CHECK_FIELD(CExceptionReport, m_systemInfo, 0x32C);
	CHECK_FIELD(CExceptionReport, m_memoryStatus, 0x350);
END_LAYOUT_CHECK

#endif // PG_SHARED_EXCEPTIONREPORT_H
