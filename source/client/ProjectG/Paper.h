// Original: d:\build\custom\temp\code\client\projectg\paper.h
#ifndef PG_CLIENT_PROJECTG_PAPER_H
#define PG_CLIENT_PROJECTG_PAPER_H

#include <windows.h>
#include <atltime.h>
#include <string>
#include <map>
#include <vector>
#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"

class WReceivedPacket;

// Theme of a paper memo (values only known by count so far: GetEnumStr
// names the first three).
enum ePaperTheme {
	PAPER_THEME_0 = 0,
	PAPER_THEME_1 = 1,
	PAPER_THEME_2 = 2,
	PAPER_THEME_COUNT = 3
};

// One line written to a paper; holds its text.
class CPaperMemo {
public:
	CPaperMemo(const char* text);
	~CPaperMemo(void);
	void __cdecl Write(ePaperTheme theme, const char* format, ...);

	std::string m_text; // 0x0
};

CHECK_SIZE(CPaperMemo, 0x1C);

// The paper (crash/debug report) singleton.
class CPaper : public WSingleton<CPaper> {
public:
	// A memo entry, keyed by its theme (several per theme).
	class cMemo {
	public:
		// paper.h:57
		cMemo(const char* time, unsigned int funcSig, const char* text)
			: m_time(time), m_funcSig(funcSig), m_text(text) {
		}

		std::string m_time;    // 0x0 (server time string, printed as <TIME>)
		unsigned int m_funcSig; // 0x1C
		std::string m_text;    // 0x20
	};

	CPaper(void);
	virtual ~CPaper(void);

	void SetServerTime(const _SYSTEMTIME& time);
	void PrepareNewPaper(const char* name);
	const char* GetEnumStr(ePaperTheme theme) const;
	const char* GetFuncSigStr(unsigned int id) const;
	const char* GetServerTime(void) const;
	void GetServerTime(_SYSTEMTIME& time) const;
	const char* SystemTimeToString(const _SYSTEMTIME& time) const;
	void ReceivedPacket(WReceivedPacket* packet);
	void SendUpdatableQuery(void);
	void Send(void);
	void Destroy(void);
	unsigned int GetMemoSize(ePaperTheme theme) const;
	bool PrintOut(ePaperTheme theme);
	unsigned int GetFuncSigId(const char* name);
	bool AddMemo(ePaperTheme theme, const char* name, const char* text);

	_SYSTEMTIME m_time;                                // 0x28
	_SYSTEMTIME m_sendTime;                            // 0x38 (only cleared by Destroy here)
	std::string m_name;                                // 0x48
	std::multimap<ePaperTheme, cMemo> m_memos;         // 0x64
	std::vector<std::string> m_funcSigs;               // 0x70
	std::map<std::string, unsigned int> m_funcSigIds;  // 0x80
	int m_updatable;                                   // 0x8C
};

CHECK_SIZE(CPaper::cMemo, 0x3C);
CHECK_FIELD(CPaper::cMemo, m_funcSig, 0x1C);
CHECK_FIELD(CPaper::cMemo, m_text, 0x20);
CHECK_FIELD(CPaper, m_time, 0x28);
CHECK_FIELD(CPaper, m_sendTime, 0x38);
CHECK_FIELD(CPaper, m_name, 0x48);
CHECK_FIELD(CPaper, m_memos, 0x64);
CHECK_FIELD(CPaper, m_funcSigs, 0x70);
CHECK_FIELD(CPaper, m_funcSigIds, 0x80);
CHECK_FIELD(CPaper, m_updatable, 0x8C);
CHECK_SIZE(CPaper, 0x90);

#endif // PG_CLIENT_PROJECTG_PAPER_H
