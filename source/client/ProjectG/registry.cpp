// Original: d:\build\custom\temp\code\client\projectg\registry.cpp
#include "Registry.h"

#define CLASS_NAME_LENGTH 255

// registry.cpp:28
CRegistry::CRegistry(void) {
	m_hRootKey = HKEY_CURRENT_USER;
	m_bLazyWrite = TRUE;
	m_nLastError = 0;
}

// registry.cpp:35
CRegistry::~CRegistry(void) {
	ClearKey();
}

// registry.cpp:41
int CRegistry::ClearKey(void) {
	m_strCurrentPath.Empty();
	m_hRootKey = HKEY_CURRENT_USER;
	m_bLazyWrite = TRUE;
	return TRUE;
}

// registry.cpp:55
int CRegistry::SetRootKey(HKEY hRootKey) {
	if (hRootKey != HKEY_CLASSES_ROOT && hRootKey != HKEY_CURRENT_USER && hRootKey != HKEY_LOCAL_MACHINE && hRootKey != HKEY_USERS)
		return FALSE;

	m_hRootKey = hRootKey;
	return TRUE;
}

// registry.cpp:112
int CRegistry::DeleteValue(WTL::CString strName) {
	HKEY hKey;
	LONG lResult;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS)
		return FALSE;

	lResult = ::RegDeleteValue(hKey, LPCTSTR(strName));
	::RegCloseKey(hKey);

	if (lResult == ERROR_SUCCESS)
		return TRUE;
	return FALSE;
}

// registry.cpp:159
unsigned long CRegistry::GetDataType(WTL::CString strValueName) {
	HKEY hKey;
	m_nLastError = ::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0, KEY_QUERY_VALUE, &hKey);

	if (m_nLastError != ERROR_SUCCESS)
		return 0;

	DWORD dwType = 1;
	m_nLastError = ::RegQueryValueEx(hKey, LPCTSTR(strValueName), NULL, &dwType, NULL, NULL);
	::RegCloseKey(hKey);

	if (m_nLastError == ERROR_SUCCESS)
		return dwType;
	return 0;
}

// registry.cpp:181
int CRegistry::GetSubKeyCount(void) {
	HKEY hKey;
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return -1;

	LONG lResult;
	DWORD dwSubKeyCount, dwValueCount, dwClassNameLength, dwMaxSubKeyName, dwMaxValueName, dwMaxValueLength;
	FILETIME ftLastWritten;

	TCHAR szClassBuffer[CLASS_NAME_LENGTH];

	dwClassNameLength = CLASS_NAME_LENGTH;
	lResult = ::RegQueryInfoKey(hKey, szClassBuffer, &dwClassNameLength, NULL, &dwSubKeyCount, &dwMaxSubKeyName, NULL, &dwValueCount, &dwMaxValueName, &dwMaxValueLength, NULL, &ftLastWritten);

	::RegCloseKey(hKey);
	if (lResult != ERROR_SUCCESS)
		return -1;

	return (int)dwSubKeyCount;
}

// registry.cpp:210
int CRegistry::GetValueCount(void) {
	HKEY hKey;
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return -1;

	LONG lResult;
	DWORD dwSubKeyCount, dwValueCount, dwClassNameLength, dwMaxSubKeyName, dwMaxValueName, dwMaxValueLength;
	FILETIME ftLastWritten;

	TCHAR szClassBuffer[CLASS_NAME_LENGTH];

	dwClassNameLength = CLASS_NAME_LENGTH;
	lResult = ::RegQueryInfoKey(hKey, szClassBuffer, &dwClassNameLength, NULL, &dwSubKeyCount, &dwMaxSubKeyName, NULL, &dwValueCount, &dwMaxValueName, &dwMaxValueLength, NULL, &ftLastWritten);

	::RegCloseKey(hKey);
	if (lResult != ERROR_SUCCESS)
		return -1;

	return (int)dwValueCount;
}

// registry.cpp:239
int CRegistry::KeyExists(WTL::CString strKey, HKEY hRootKey) {
	HKEY hKey;

	if (hRootKey == NULL)
		hRootKey = m_hRootKey;

	LONG lResult = ::RegOpenKeyEx(hRootKey, LPCTSTR(strKey), 0, KEY_ALL_ACCESS, &hKey);
	::RegCloseKey(hKey);
	if (lResult == ERROR_SUCCESS)
		return TRUE;
	return FALSE;
}

// registry.cpp:256
int CRegistry::SetKey(WTL::CString strKey, int bCanCreate) {
	HKEY hKey;

	if (strKey.GetLength() == 0) {
		m_strCurrentPath.Empty();
		return TRUE;
	}

	DWORD dwDisposition;
	if (bCanCreate) {
		if (::RegCreateKeyEx(m_hRootKey, LPCTSTR(strKey), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition) != ERROR_SUCCESS)
			return FALSE;
		m_strCurrentPath = strKey;
		if (!m_bLazyWrite)
			::RegFlushKey(hKey);
		::RegCloseKey(hKey);
		return TRUE;
	}

	m_nLastError = ::RegOpenKeyEx(m_hRootKey, LPCTSTR(strKey), 0, KEY_ALL_ACCESS, &hKey);
	if (m_nLastError != ERROR_SUCCESS)
		return FALSE;
	m_strCurrentPath = strKey;
	if (!m_bLazyWrite)
		::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return TRUE;
}

// registry.cpp:306
int CRegistry::ValueExists(WTL::CString strName) {
	HKEY hKey;
	LONG lResult;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return FALSE;

	lResult = ::RegQueryValueEx(hKey, LPCTSTR(strName), NULL, NULL, NULL, NULL);
	::RegCloseKey(hKey);

	if (lResult == ERROR_SUCCESS)
		return TRUE;
	return FALSE;
}

// registry.cpp:332
void CRegistry::RenameValue(WTL::CString strOldName, WTL::CString strNewName) {
}

// registry.cpp:375
double CRegistry::ReadFloat(WTL::CString strName, double fDefault) {
	DWORD dwType = REG_BINARY;
	double d;
	DWORD dwSize = sizeof(d);
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return fDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL, &dwType, (LPBYTE)&d, &dwSize) != ERROR_SUCCESS)
		d = fDefault;
	::RegCloseKey(hKey);
	return d;
}

// registry.cpp:400
WTL::CString CRegistry::ReadString(WTL::CString strName, WTL::CString strDefault) {
	DWORD dwType = REG_SZ;
	DWORD dwSize = 255;
	BOOL bSuccess = TRUE;
	TCHAR sz[256];
	HKEY hKey;

	dwType = GetDataType(strName);

	if (dwType != REG_SZ && dwType != REG_EXPAND_SZ)
		return strDefault;

	m_nLastError = ::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0, KEY_READ, &hKey);
	if (m_nLastError != ERROR_SUCCESS)
		return strDefault;

	m_nLastError = ::RegQueryValueEx(hKey, LPCTSTR(strName), NULL, &dwType, (LPBYTE)sz, &dwSize);
	if (m_nLastError != ERROR_SUCCESS)
		bSuccess = FALSE;
	::RegCloseKey(hKey);

	if (!bSuccess)
		return strDefault;
	return WTL::CString(sz);
}

// registry.cpp:432
unsigned long CRegistry::ReadDword(WTL::CString strName, unsigned long dwDefault) {
	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwValue;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return dwDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL, &dwType, (LPBYTE)&dwValue, &dwSize) != ERROR_SUCCESS)
		dwValue = dwDefault;
	::RegCloseKey(hKey);
	return dwValue;
}

// registry.cpp:451
int CRegistry::ReadInt(WTL::CString strName, int nDefault) {
	DWORD dwType = REG_BINARY;
	DWORD dwSize = sizeof(int);
	int n;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return nDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL, &dwType, (LPBYTE)&n, &dwSize) != ERROR_SUCCESS)
		n = nDefault;
	::RegCloseKey(hKey);
	return n;
}

// registry.cpp:468
int CRegistry::ReadBool(WTL::CString strName, int bDefault) {
	DWORD dwType = REG_BINARY;
	DWORD dwSize = sizeof(int);
	int b;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return bDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL, &dwType, (LPBYTE)&b, &dwSize) != ERROR_SUCCESS)
		b = bDefault;
	::RegCloseKey(hKey);
	return b;
}

// registry.cpp:486
unsigned long CRegistry::ReadColor(WTL::CString strName, unsigned long rgbDefault) {
	DWORD dwType = REG_BINARY;
	DWORD dwSize = sizeof(unsigned long);
	unsigned long rgb;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return rgbDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL, &dwType, (LPBYTE)&rgb, &dwSize) != ERROR_SUCCESS)
		rgb = rgbDefault;
	::RegCloseKey(hKey);
	return rgb;
}
