// Original: d:\build\custom\temp\code\client\projectg\ggc_helper.h
#ifndef PG_CLIENT_PROJECTG_GGC_HELPER_H
#define PG_CLIENT_PROJECTG_GGC_HELPER_H

#include <map>
#include <string>
#include <vector>
#include "..\..\defines.h"
#include "frcmdtarget.h"

class FrForm;
class GGC_Dlg;

// GameGuard error tracker: the singleton (GGC_GetHelper) that collects the
// error codes nProtect GameGuard reports, keeps one message per known code
// and shows the critical ones (message box before Fresh exists, GGC_Dlg
// after).
class GGC_Helper : public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;
	friend GGC_Helper* GGC_GetHelper(void);

protected:
	GGC_Helper(void);

public:
	virtual ~GGC_Helper(void);

	bool StackError(unsigned long code, bool critical);
	bool GetFirstCriticalErrorMessage(unsigned long& code, std::string& message);
	void ForceShutdown(void);
	void Reset(void);

	// ggc_helper.h:45
	unsigned int GetCriticalErrorCount(void) const { return m_criticalErrors.size(); }

protected:
	bool StackApprovableError(unsigned long code);
	bool StackCriticalError(unsigned long code);
	void GetErrorMessage(unsigned long code, std::string& message);
	bool OnGameGuardDlgResult(int result, FrForm* pForm);

private:
	std::map<unsigned long, std::string> m_errorMessages;   // 0x4, by GameGuard code
	std::vector<unsigned long> m_approvableErrors;          // 0x10
	std::vector<unsigned long> m_criticalErrors;            // 0x20
	char m_codeText[0x400];                                 // 0x30, " (%d)." of a code
	GGC_Dlg* m_pDlg;                                        // 0x430, the critical-error dialog
};
CHECK_SIZE(GGC_Helper, 0x434);
BEGIN_LAYOUT_CHECK(GGC_Helper)
	CHECK_FIELD(GGC_Helper, m_errorMessages, 0x4);
	CHECK_FIELD(GGC_Helper, m_approvableErrors, 0x10);
	CHECK_FIELD(GGC_Helper, m_criticalErrors, 0x20);
	CHECK_FIELD(GGC_Helper, m_codeText, 0x30);
	CHECK_FIELD(GGC_Helper, m_pDlg, 0x430);
END_LAYOUT_CHECK

// ggc_helper.cpp:26
GGC_Helper* GGC_GetHelper(void);

#endif // PG_CLIENT_PROJECTG_GGC_HELPER_H
