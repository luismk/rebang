// Original: d:\build\custom\temp\code\client\projectg\ccrc32.h
#ifndef PG_CLIENT_PROJECTG_CCRC32_H
#define PG_CLIENT_PROJECTG_CCRC32_H

#include <windows.h>
#include <vector>

#include "..\..\defines.h"

// Table-driven CRC-32 (reflected polynomial 0xEDB88320), the widespread
// public CRC_32 class; the table is the only member seen (ccrc32.cpp:132-220).
class CRC_32 {
	LAYOUT_CHECK_FRIEND;
public:
	CRC_32(void);

	unsigned long CalcCRC(void *buffer, unsigned int size, HWND hWnd);
	unsigned long CalcCRC(const char *fileName, HWND hWnd);
	void Calculate(unsigned char *const buffer, unsigned int size, unsigned long &crc, unsigned long sleepTime);

private:
	unsigned long Reflect(unsigned long ref, char ch);
	static unsigned long __stdcall CRC32ThreadProc(void *param);

	unsigned long m_table[256];              // 0x0
};

CHECK_SIZE(CRC_32, 0x400);
BEGIN_LAYOUT_CHECK(CRC_32)
	CHECK_FIELD(CRC_32, m_table, 0x0);
END_LAYOUT_CHECK

// Background CRC check of a list of files (ThreadCRCCheckProc); layout from
// the constructor, AddWork and Terminate (ccrc32.cpp:355-523).
class CRC_MT {
	LAYOUT_CHECK_FRIEND;
public:
	struct sCRC_CHKTASK {
		char m_fileName[0x104];              // 0x0
		unsigned long m_crc;                 // 0x104
	};

	CRC_MT(void);
	~CRC_MT(void);

	void GetWork(std::vector<sCRC_CHKTASK *> &work);
	int AddWork(const char *fileName, unsigned long crc);
	void Reset(void);
	int Begin(HWND hParentWnd);
	void Pause(void);
	void Resume(void);

	// ccrc32.h:80
	int IsTerminate(void) { return m_terminate; }
	// ccrc32.h:82
	int IsWorking(void) { return m_tasks.size() != 0; }
	// ccrc32.h:84
	HWND GetParentWnd(void) { return m_hParentWnd; }
	// ccrc32.h:85
	void SetSleepTime(unsigned long sleepTime) { m_sleepTime = sleepTime; }
	// ccrc32.h:86
	unsigned long GetSleepTime(void) { return m_sleepTime; }

private:
	void Terminate(void);

	HWND m_hParentWnd;                       // 0x0
	HANDLE m_hThread;                        // 0x4
	int m_paused;                            // 0x8
	int m_terminate;                         // 0xC
	unsigned long m_sleepTime;               // 0x10
	std::vector<sCRC_CHKTASK *> m_tasks;     // 0x14
};

CHECK_SIZE(CRC_MT::sCRC_CHKTASK, 0x108);
CHECK_SIZE(CRC_MT, 0x24);
BEGIN_LAYOUT_CHECK(CRC_MT)
	CHECK_FIELD(CRC_MT::sCRC_CHKTASK, m_fileName, 0x0);
	CHECK_FIELD(CRC_MT::sCRC_CHKTASK, m_crc, 0x104);
	CHECK_FIELD(CRC_MT, m_hParentWnd, 0x0);
	CHECK_FIELD(CRC_MT, m_hThread, 0x4);
	CHECK_FIELD(CRC_MT, m_paused, 0x8);
	CHECK_FIELD(CRC_MT, m_terminate, 0xC);
	CHECK_FIELD(CRC_MT, m_sleepTime, 0x10);
	CHECK_FIELD(CRC_MT, m_tasks, 0x14);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CCRC32_H
