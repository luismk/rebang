// Original: d:\build\custom\temp\code\client\wangreal\source\wreg.cpp
#include "../include/wreg.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// wreg.cpp:6
WRegistry::WRegistry() {
	SetLocation("HKEY_CURRENT_USER\\Software\\Ntreev\\PangYa");
}

// wreg.cpp:11
WRegistry::~WRegistry() {
}

// wreg.cpp:15
void WRegistry::SetLocation(const char* loc) {
	static struct {
		HKEY__* root;
		const char* name;
	} roots[] = {
		{ HKEY_CURRENT_USER, "HKEY_CURRENT_USER" },
		{ HKEY_CLASSES_ROOT, "HKEY_CLASSES_ROOT" },
		{ HKEY_LOCAL_MACHINE, "HKEY_LOCAL_MACHINE" },
		{ HKEY_USERS, "HKEY_USERS" },
		{ HKEY_CURRENT_CONFIG, "HKEY_CURRENT_CONFIG" },
	};

	for (int i = 0; i < sizeof(roots) / sizeof(roots[0]); i++) {
		const char* p = strstr(loc, roots[i].name);

		if (p) {
			m_root = roots[i].root;
			strcpy(m_location, strchr(p, '\\') + 1);
			if (m_location[strlen(m_location) - 1] == '\\')
				m_location[strlen(m_location) - 1] = 0;
			return;
		}
	}

	strcpy(m_location, loc);
	if (m_location[strlen(m_location) - 1] == '\\')
		m_location[strlen(m_location) - 1] = 0;
}

// wreg.cpp:54
bool __cdecl WRegistry::Read(char* field, char* out, int len) {
	char subkey[128];
	bool ret = false;
	unsigned long datalen;
	unsigned long type;
	HKEY__* hkey;

	if (strchr(field, '\\')) {
		sprintf(subkey, "%s\\%s", m_location, field);
		*strrchr(subkey, '\\') = 0;
		field = strrchr(field, '\\') + 1;
	} else {
		strcpy(subkey, m_location);
	}

	if (RegOpenKeyEx(m_root, subkey, 0, KEY_READ, &hkey) == ERROR_SUCCESS) {
		datalen = len;
		if (RegQueryValueEx(hkey, field, 0, &type, (LPBYTE)out, &datalen) == ERROR_SUCCESS)
			ret = true;
		RegCloseKey(hkey);
	}

	return ret;
}

// wreg.cpp:82
int __cdecl WRegistry::Scan(char* field, char* fmt, ...) {
	char text[512] = { 0 };
	char temp[128] = { 0 };
	int c = 0;
	int i;
	int j;
	float code2;
	va_list ap;

	va_start(ap, fmt);
	if (Read(field, text, sizeof(text)) == true) {
		for (i = 0, j = 0; fmt[i]; i++) {
			if (fmt[i] == '%') {
				while (text[j] && (unsigned char)text[j] < ' ')
					j++;
				int n = 0;
				while (text[j + n] && (unsigned char)text[j + n] >= ' ' && n < sizeof(temp))
					n++;

				if (n == 0)
					break;

				memcpy(temp, &text[j], n);
				temp[n] = 0;

				c++;
				j += n;

				switch (fmt[i + 1]) {
				case 'x':
				case 'X': {
					int v = 0;
					for (int k = temp[0] == '-'; temp[k]; k++) {
						int d = temp[k];

						if (d >= 'A')
							d -= 'A' - 10;
						else
							d -= d <= '9' ? '0' : 'a' - 10;

						// Never true; the original tests it as written.
						if (d < 0 && d >= 16)
							break;

						v = v * 16 + (temp[0] == '-' ? -d : d);
					}
					*va_arg(ap, int*) = v;
					break;
				}
				case 'f':
				case 'F':
				case 'g':
				case 'G':
					code2 = (float)atof(temp);
					memcpy(va_arg(ap, float*), &code2, sizeof(code2));
					break;
				case 'd':
				case 'D':
				case 'u':
				case 'U':
					*va_arg(ap, int*) = atoi(temp);
					break;
				case 's':
				case 'S':
					strcpy(va_arg(ap, char*), temp);
					break;
				}
			}
		}
	}

	return c;
}

// wreg.cpp:154
void __cdecl WRegistry::Write(char* field, const char* str, ...) {
	char subkey[128];
	char buffer[1024];
	HKEY__* hkey;
	va_list ap;

	va_start(ap, str);
	_vsnprintf(buffer, sizeof(buffer), str, ap);

	if (strchr(field, '\\')) {
		sprintf(subkey, "%s\\%s", m_location, field);
		*strrchr(subkey, '\\') = 0;
		field = strrchr(field, '\\') + 1;
	} else {
		strcpy(subkey, m_location);
	}

	if (RegOpenKeyEx(m_root, subkey, 0, KEY_WRITE, &hkey) != ERROR_SUCCESS)
		RegCreateKey(m_root, subkey, &hkey);

	RegSetValueEx(hkey, field, 0, REG_SZ, (const BYTE*)buffer, strlen(buffer));

	RegCloseKey(hkey);
}

// wreg.cpp:182
void WRegistry::Write(char* field, unsigned long v) {
	char subkey[128];
	HKEY__* hkey;

	char* p = strchr(field, '\\');
	if (p) {
		sprintf(subkey, "%s\\%s", m_location, field);
		*strrchr(subkey, '\\') = 0;
		field = strrchr(field, '\\') + 1;
	} else {
		strcpy(subkey, m_location);
	}

	if (RegOpenKeyEx(m_root, subkey, 0, KEY_WRITE, &hkey) != ERROR_SUCCESS)
		RegCreateKey(m_root, subkey, &hkey);

	RegSetValueEx(hkey, field, 0, REG_DWORD, (const BYTE*)&v, sizeof(v));

	RegCloseKey(hkey);
}
