// Original: d:\build\custom\temp\code\shared\localize.h
#ifndef PG_SHARED_LOCALIZE_H
#define PG_SHARED_LOCALIZE_H

#include "..\..\defines.h"

// Contents switched on or off per service region (localContentType_t,
// values only known by number so far).
enum localContentType_t {
	LOCAL_CONTENT_UNKNOWN = 0
};

bool IsLocalCountry(__int64 country);
bool IsLocalContent(localContentType_t type);
int GetLocalCountry();
void InitLocalizeSystem();

#endif // PG_SHARED_LOCALIZE_H
