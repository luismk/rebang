// Original: d:\build\custom\temp\code\client\projectg\integritycheck.cpp
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <atlbase.h>
#include "IntegrityCheck.h"

// integritycheck.cpp:21
CIntegrityCheck::CIntegrityCheck()
	: m_fp(NULL) {
}

// integritycheck.cpp:25
CIntegrityCheck::~CIntegrityCheck() {
}

// integritycheck.cpp:30
bool CIntegrityCheck::Report(const char* path) {
	m_fp = fopen(path, "wt");
	ReportCharParts();
	fclose(m_fp);
	return true;
}

// integritycheck.cpp:49
void CIntegrityCheck::Log(const char* format, ...) {
	char buffer[2048];
	va_list args;
	va_start(args, format);
	if (_vsnprintf(buffer, sizeof(buffer) - 1, format, args) == -1)
		buffer[sizeof(buffer) - 1] = '\0';
	va_end(args);
	fprintf(m_fp, buffer);
}

// integritycheck.cpp:65
bool CompareByName(const sResName& a, const sResName& b) {
	return strcmp(a.name, b.name) < 0 ? true : false;
}

// integritycheck.cpp:70
void CIntegrityCheck::PrintOkMissing(bool ok) {
	if (ok)
		Log("ok!!\n");
	else
		Log("missing!!\n");
}

// integritycheck.cpp:78
void CIntegrityCheck::PrintOkNg(bool ok) {
	if (ok)
		Log("ok!!\n");
	else
		Log("ng!!\n");
}
