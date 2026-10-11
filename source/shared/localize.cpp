// Original: d:\build\custom\temp\code\shared\localize.cpp
#include "Localize.h"

#include <bitset>

// The service country (a bit mask) and the contents switched on for it,
// both set by InitLocalizeSystem.
static __int64 s_localCountry;
// localize.cpp:7
static std::bitset<164> s_localContents;

// localize.cpp:25
bool IsLocalCountry(__int64 country) {
	return (s_localCountry & country) ? true : false;
}

// localize.cpp:29
bool IsLocalContent(localContentType_t type) {
	if (type >= 164) {
		return false;
	}
	return s_localContents.test(type);
}

// localize.cpp:38
int GetLocalCountry() {
	return (int)s_localCountry;
}

#include "localize_kor.h"
