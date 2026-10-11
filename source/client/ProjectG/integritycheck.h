// Original: d:\build\custom\temp\code\client\projectg\integritycheck.h
#ifndef PG_CLIENT_PROJECTG_INTEGRITYCHECK_H
#define PG_CLIENT_PROJECTG_INTEGRITYCHECK_H

#include <stdio.h>

#include "..\..\defines.h"

// The record CompareByName orders by name (only the leading name is known).
struct sResName {
	char name[1];       // 0x0 (length unknown)
};

class CIntegrityCheck {
public:
	CIntegrityCheck();
	virtual ~CIntegrityCheck();     // vtable slot 0 (??_E / ??_G)

	bool Report(const char* path);

protected:
	void Log(const char* format, ...);
	void PrintOkMissing(bool ok);
	void PrintOkNg(bool ok);
	bool ReportCharParts(void);

	FILE* m_fp;         // 0x4

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(CIntegrityCheck, 0x8);
BEGIN_LAYOUT_CHECK(CIntegrityCheck)
	CHECK_FIELD(CIntegrityCheck, m_fp, 0x4);
END_LAYOUT_CHECK

#endif
