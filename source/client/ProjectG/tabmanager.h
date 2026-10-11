// Original: d:\build\custom\temp\code\client\projectg\tabmanager.h
#ifndef PG_CLIENT_PROJECTG_TABMANAGER_H
#define PG_CLIENT_PROJECTG_TABMANAGER_H

#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"

class FrWnd;

// One tab: its id and the windows (held through FrWnd** slots) shown while
// it is open. Layout from CTabManager::Add/Open (size 0x14).
struct sTabElement {
	int m_id;                                // 0x0
	std::vector<FrWnd**> m_wnds;             // 0x4
};
CHECK_SIZE(sTabElement, 0x14);
CHECK_FIELD(sTabElement, m_id, 0x0);
CHECK_FIELD(sTabElement, m_wnds, 0x4);

// Shows the windows of one tab and hides those of the others. Layout from
// the constructor (tabmanager.cpp:9) and destructor (tabmanager.cpp:13).
class CTabManager {
	LAYOUT_CHECK_FRIEND;
public:
	CTabManager(void);
	~CTabManager(void);

	void __cdecl Add(int id, ...);
	void Open(int id);

private:
	sTabElement* Find(int id);

	std::vector<sTabElement> m_tabs;         // 0x0
	UNKNOWN(0x10, 0x14);                     // not set by the constructor
	std::vector<int> m_unknown14;            // 0x14, element type unknown (only constructed/destroyed here)
};
CHECK_SIZE(CTabManager, 0x24);
BEGIN_LAYOUT_CHECK(CTabManager)
	CHECK_FIELD(CTabManager, m_tabs, 0x0);
	CHECK_FIELD(CTabManager, m_unknown14, 0x14);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_TABMANAGER_H
