// Original: d:\build\custom\temp\code\client\projectg\rssmanager.h
#ifndef PG_CLIENT_PROJECTG_RSSMANAGER_H
#define PG_CLIENT_PROJECTG_RSSMANAGER_H

#include <string>

#include "..\..\defines.h"

// Downloads the guild notice RSS (NoticeRSS.aspx?gIdx=<guild>) on a worker
// thread, compares it with the last one saved and reports the result through
// a callback. Layout from the constructor (rssmanager.cpp:135) and
// GetRSSFile / GetRSSFile_JP.
class RSSManager {
	LAYOUT_CHECK_FRIEND;

public:
	RSSManager(void);
	virtual ~RSSManager(void);

	void GetRSSFile(unsigned long guildIdx, void (__fastcall *callBack)(int));
	void GetRSSFile_JP(unsigned long guildIdx, void (__fastcall *callBack)(int), const char *param1, const char *param2);
	bool IsNewRss(void);
	// rssmanager.h:40
	bool IsComplete(void)
	{
		return m_bComplete;
	}
	void ResetVars(void);

private:
	void CallBackFunction(int result);
	const char *LoadFirstId(const char *fileName);

	friend unsigned __stdcall RSSThreadFunc(void *param);

	unsigned long m_guildIdx;                // 0x4
	bool m_bNewRss;                          // 0x8
	bool m_bComplete;                        // 0x9
	void (__fastcall *m_pfnCallBack)(int);   // 0xC
	std::string m_strJpParam1;               // 0x10 (GetRSSFile_JP's third argument)
	std::string m_strJpParam2;               // 0x2C (GetRSSFile_JP's fourth argument)
};

CHECK_SIZE(RSSManager, 0x48);
BEGIN_LAYOUT_CHECK(RSSManager)
	CHECK_FIELD(RSSManager, m_guildIdx, 0x4);
	CHECK_FIELD(RSSManager, m_bNewRss, 0x8);
	CHECK_FIELD(RSSManager, m_bComplete, 0x9);
	CHECK_FIELD(RSSManager, m_pfnCallBack, 0xC);
	CHECK_FIELD(RSSManager, m_strJpParam1, 0x10);
	CHECK_FIELD(RSSManager, m_strJpParam2, 0x2C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_RSSMANAGER_H
