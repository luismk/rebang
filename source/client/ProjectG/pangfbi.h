// Original: d:\build\custom\temp\code\client\projectg\pangfbi.h
#ifndef PG_CLIENT_PROJECTG_PANGFBI_H
#define PG_CLIENT_PROJECTG_PANGFBI_H

#include "..\..\defines.h"
#include "singleton.h"

// Sends crash ("crime_log") and bug ("app_log") reports to the QA report
// server. No members beyond WSingleton have been seen.
class CPangFBI : public WSingleton<CPangFBI> {
public:
	CPangFBI(void);
	virtual ~CPangFBI(void);

	void ProcessReport(const char *type, const char *data);
};
CHECK_SIZE(CPangFBI, 0x28);

#endif
