// Original: d:\build\custom\temp\code\client\projectg\registry.h
#ifndef PG_CLIENT_PROJECTG_REGISTRY_H
#define PG_CLIENT_PROJECTG_REGISTRY_H

#include <windows.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>

#include "..\..\defines.h"

// Registry access helper: a root key plus the path of the current subkey.
class CRegistry {
public:
	CRegistry(void);
	~CRegistry(void);

	int SetRootKey(HKEY hRootKey);
	int CreateKey(WTL::CString strKey);
	int DeleteValue(WTL::CString strName);
	int GetDataSize(WTL::CString strValueName);
	unsigned long GetDataType(WTL::CString strValueName);
	int GetSubKeyCount(void);
	int GetValueCount(void);
	int KeyExists(WTL::CString strKey, HKEY hRootKey);
	int ValueExists(WTL::CString strName);
	void RenameValue(WTL::CString strOldName, WTL::CString strNewName);
	double ReadFloat(WTL::CString strName, double fDefault);
	unsigned long ReadDword(WTL::CString strName, unsigned long dwDefault);
	int ReadInt(WTL::CString strName, int nDefault);
	int ReadBool(WTL::CString strName, int bDefault);
	unsigned long ReadColor(WTL::CString strName, unsigned long rgbDefault);
	int ReadFont(WTL::CString strName, WTL::CFont* pFont);
	int ReadPoint(WTL::CString strName, WTL::CPoint* pPoint);
	int ReadSize(WTL::CString strName, WTL::CSize* pSize);
	int ReadRect(WTL::CString strName, WTL::CRect* pRect);
	int WriteBool(WTL::CString strName, int bValue);
	int WriteString(WTL::CString strName, WTL::CString strValue);
	int WriteFloat(WTL::CString strName, double fValue);
	int WriteInt(WTL::CString strName, int nValue);
	int WriteDword(WTL::CString strName, unsigned long dwValue);
	int WriteColor(WTL::CString strName, unsigned long rgbValue);
	int WriteFont(WTL::CString strName, WTL::CFont* pFont);
	int WritePoint(WTL::CString strName, WTL::CPoint* pPoint);
	int WriteSize(WTL::CString strName, WTL::CSize* pSize);
	int WriteRect(WTL::CString strName, WTL::CRect* pRect);
	int ClearKey(void);
	int DeleteKey(WTL::CString strKey);
	int SetKey(WTL::CString strKey, int bCanCreate);
	WTL::CString ReadString(WTL::CString strName, WTL::CString strDefault);

	int m_nLastError;                 // 0x0, cleared by the constructor
	HKEY m_hRootKey;                  // 0x4
	int m_bLazyWrite;                 // 0x8
	WTL::CString m_strCurrentPath;    // 0xC
};
CHECK_SIZE(CRegistry, 0x10);
CHECK_FIELD(CRegistry, m_nLastError, 0x0);
CHECK_FIELD(CRegistry, m_hRootKey, 0x4);
CHECK_FIELD(CRegistry, m_bLazyWrite, 0x8);
CHECK_FIELD(CRegistry, m_strCurrentPath, 0xC);

#endif // PG_CLIENT_PROJECTG_REGISTRY_H
