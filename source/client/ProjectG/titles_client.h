// Original: d:\build\custom\temp\code\client\projectg\titles_client.h
#ifndef PG_CLIENT_PROJECTG_TITLES_CLIENT_H
#define PG_CLIENT_PROJECTG_TITLES_CLIENT_H

#include "..\..\defines.h"

// Base of the client's equippable titles; each TitleNN overrides CanEquip.
class Title {
public:
	Title() {}
	virtual ~Title() {}
	virtual bool CanEquip() { return false; }
};
CHECK_SIZE(Title, 0x4);

class TitleManager;

void RegisterTitles(TitleManager *manager);

#endif // PG_CLIENT_PROJECTG_TITLES_CLIENT_H
