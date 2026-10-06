#pragma once

#include <stdio.h>

struct sResName
{
	char name[64];
};

class CIntegrityCheck
{
public:
	CIntegrityCheck();
	virtual ~CIntegrityCheck();

	bool Report(const char* filename);

protected:
	void Log(const char* format, ...);
	void PrintOkMissing(bool ok);
	void PrintOkNg(bool ok);
	bool ReportCharParts();

	FILE* m_fp;
};
