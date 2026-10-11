// Original: d:\build\custom\temp\code\shared\titlemanager.h
#ifndef PG_SHARED_TITLEMANAGER_H
#define PG_SHARED_TITLEMANAGER_H

#include <map>

#include "..\..\defines.h"

class Title;

// Title identifiers (TitleManager::Register); only the values the client
// registers are known, their original names are not.
enum eTitle {
	TITLE_21 = 21,
	TITLE_22 = 22,
	TITLE_23 = 23,
	TITLE_24 = 24,
	TITLE_25 = 25,
	TITLE_26 = 26,
	TITLE_27 = 27,
	TITLE_28 = 28,
	TITLE_357 = 357,
	TITLE_358 = 358,
	TITLE_359 = 359,
	TITLE_360 = 360,
	TITLE_361 = 361
};

class TitleManager {
	LAYOUT_CHECK_FRIEND;

public:
	TitleManager(void);
	~TitleManager(void);
	Title *GetTitle(eTitle title);
	void Register(eTitle title, Title *instance);

private:
	std::map<eTitle, Title *> m_titles; // 0x0
};
CHECK_SIZE(TitleManager, 0xC);
BEGIN_LAYOUT_CHECK(TitleManager)
	CHECK_FIELD(TitleManager, m_titles, 0x0);
END_LAYOUT_CHECK

#endif // PG_SHARED_TITLEMANAGER_H
