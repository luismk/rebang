// Original: d:\build\custom\temp\code\client\projectg\binstr.cpp
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <new>

#include "binstr.h"
#include "../shared/Localize.h"

// binstr.cpp:19
cToken::cToken(void) {
	m_Len = m_Pos = m_ResLen = 0;
	m_pcResBuf = 0;
}

// binstr.cpp:28
cToken::~cToken(void) {
	if (m_pcResBuf) {
		delete[] m_pcResBuf;
	}
}

// binstr.cpp:36
bool cToken::GetResultBuf(void) {
	if (m_Len + 1 > m_ResLen) {
		if (m_pcResBuf) {
			delete[] m_pcResBuf;
		}
		m_pcResBuf = new char[m_Len + 1];
		if (!m_pcResBuf) {
			m_ResLen = 0;
			return false;
		}
		m_ResLen = m_Len + 1;
	}
	return true;
}

// binstr.cpp:69
bool cToken::GetToken(char *const pToken, const char *sep, int sepLen) {
	pToken[0] = '\0';
	if (!GetBuf()) {
		return false;
	}

	int found = 0;
	bool bRet = false;
	int i;
	int j;
	const char *pSep;
	for (i = m_Pos; i < m_Len; i++) {
		pSep = sep;
		for (j = 0; j < sepLen; j++, pSep++) {
			if (GetBuf()[i] == *pSep) {
				if (*pSep & 0x80) {
					bool bSame;
					pSep++;
					if (IsLocalCountry(30)) {
						bSame = i + 1 < m_Len && GetBuf()[i + 1] == *pSep;
					} else {
						bSame = GetBuf()[i + 1] == *pSep;
					}
					if (bSame) {
						found = 2;
						break;
					}
				} else {
					found = 1;
					break;
				}
			} else if (*pSep & 0x80) {
				pSep++;
			}
		}
		if (found) {
			break;
		}

		pToken[i - m_Pos] = GetBuf()[i];
		if (GetBuf()[i] & 0x80) {
			if (IsLocalCountry(30)) {
				if (i + 1 < m_Len) {
					i++;
					pToken[i - m_Pos] = GetBuf()[i];
				}
			} else {
				i++;
				if (i < m_Len) {
					pToken[i - m_Pos] = GetBuf()[i];
				}
			}
		}
	}

	if (found == 1) {
		bRet = true;
	}
	if (found == 2) {
		bRet = true;
		i++;
		pToken[i - m_Pos - 1] = '\0';
	} else {
		pToken[i - m_Pos] = '\0';
	}
	m_Pos = (i < m_Len) ? i + 1 : m_Len;
	return bRet;
}

// binstr.cpp:158
bool cToken::GetTokenFullSep(char *const pToken, const char *sep, int sepLen) {
	pToken[0] = '\0';
	if (!GetBuf()) {
		return false;
	}

	int len = PosB(GetBuf() + m_Pos, m_Len - m_Pos, sep, sepLen);
	if (len < 0) {


		len = m_Len - m_Pos; memcpy(pToken, GetBuf() + m_Pos, len);
		pToken[len] = '\0';
		m_Pos = m_Len;
		return false;
	}

	memcpy(pToken, GetBuf() + m_Pos, len);

	pToken[len] = '\0'; m_Pos += len + sepLen;
	return true;
}

// binstr.cpp:184
int cToken::GetTokenNum(int start, char sep) const {
	return GetTokenNum(start, &sep, 1);
}

// binstr.cpp:191
int cToken::GetTokenNum(int start, const char *sep, int sepLen) const {
	if (!GetBuf()) {
		return 0;
	}

	int count = start < m_Len;
	for (int i = start; i < m_Len; i++) {
		for (int j = 0; j < sepLen; j++) {
			if (GetBuf()[i] == sep[j]) {
				count++;
				break;
			}
		}
		if (GetBuf()[i] & 0x80) {
			i++;
		}
	}
	return count;
}

// binstr.cpp:218
char cToken::GetChar(void) {
	return (GetBuf() && m_Pos < m_Len) ? GetBuf()[m_Pos++] : 0;
}

// binstr.cpp:229
int cToken::GetChange(char *const pDest) {
	if (GetBuf() && m_Pos < m_Len) {
		memcpy(pDest, GetBuf() + m_Pos, m_Len - m_Pos);
		return m_Len - m_Pos;
	}

	pDest[0] = 0;
	return 0;
}

// binstr.cpp:251
cTokenV::cTokenV(void) {
	m_pcBuf = 0;
}

// binstr.cpp:255
cTokenV::~cTokenV(void) {
}

// binstr.cpp:261
bool cTokenV::Init(const char *buf, int len) {
	if (!buf || len < 1) {
		return false;
	}

	m_pcBuf = buf; m_Len = len;
	m_Pos = 0;
	return true;
}

// binstr.cpp:280
cTokenS::cTokenS(void) {
	m_pcBuf = 0;
}

// binstr.cpp:286
cTokenS::~cTokenS(void) {
	if (m_pcBuf) {
		delete[] m_pcBuf;
	}
}

// binstr.cpp:294
bool cTokenS::Init(const char *buf, int len) {
	if (!buf || len < 1) {
		return false;
	}

	if (len > m_Len) {
		if (m_pcBuf) {
			delete[] m_pcBuf;
		}
		m_pcBuf = new char[len];
		if (!m_pcBuf) {
			m_Len = 0;
			return false;
		}
	}

	memcpy(m_pcBuf, buf, len);
	m_Len = len;
	m_Pos = 0;
	return true;
}

// binstr.cpp:322
int StrToIntDefA(const char *str, int def) {
	if (!str || !str[0]) {
		return def;
	}

	int last = strlen(str) - 1;
	int result = 0;
	int mul;
	int i;
	for (i = last, mul = 1; i > 0; i--) {
		if (str[i] < '0' || str[i] > '9') {
			return def;
		}
		result += (str[i] - '0') * mul;
		mul *= 10;
	}

	if (str[0] == '-') {
		result = -result;
	} else {
		if (str[0] < '0' || str[0] > '9') {
			return def;
		}
		result += (str[0] - '0') * mul;
	}

	return result;
}

// binstr.cpp:358
int StrToIntDefFilterA(const char *str, int def) {
	if (!str) {
		return def;
	}

	int last = strlen(str) - 1;
	int start = 0;
	while (start <= last && (str[start] < '0' || str[start] > '9')) {
		start++;
	}

	if (start > last) {
		return def;
	}
	int end = start + 1;
	while (end <= last && str[end] >= '0' && str[end] <= '9') {
		end++;
	}
	end--;

	int result = 0;
	int mul;
	for (mul = 1; end > start; end--) {
		if (str[end] < '0' || str[end] > '9') {
			return def;
		}
		result += (str[end] - '0') * mul;
		mul *= 10;
	}

	if (str[start] == '-') {
		result = -result;
	} else {
		if (str[start] < '0' || str[start] > '9') {
			return def;
		}
		result += (str[start] - '0') * mul;
	}

	return result;
}

// binstr.cpp:409
char *IntToStrA(int value, char *buf) {
	int pos;
	int len;
	int div;

	if (value < 0) {
		buf[0] = '-'; pos = 1; len = 2; div = -1;
	} else {
		len = 1; pos = 0; div = 1;
	}

	while (value / div > 9) {
		div *= 10; len++;
	}

	buf[len] = '\0';
	while (pos < len) {
		buf[pos++] = (char)(value / div + '0'); value %= div;

		div /= 10;
	}

	return buf;
}

// binstr.cpp:442
char *TrimA(char *str, const char *chars) {
	int start = 0;
	int end = strlen(str) - 1;

	while (start <= end && strrchr(chars, str[start])) {

		if (str[start] & 0x80) {
			start++;
		}
		start++;
	}

	if (start <= end) {

		while (strrchr(chars, str[end])) {
			end--;
		}

		memmove(str, str + start, end - start + 1);
		str[end - start + 1] = '\0';
	} else {
		str[0] = '\0';
	}

	return str;
}

// binstr.cpp:472
char *ExchangeA(char *str, const char *from, const char *to) {
	cTokenS token;

	char *pBuf = new char[strlen(str) + 1];

	pBuf[0] = '\0'; token.Init(str, strlen(str));

	str[0] = '\0'; bool bRet = token.GetTokenFullSep(pBuf, from, strlen(from));
	lstrcatA(str, pBuf);
	while (bRet) {
		lstrcatA(str, to);
		bRet = token.GetTokenFullSep(pBuf, from, strlen(from));
		lstrcatA(str, pBuf);
	}

	delete[] pBuf;

	return str;
}

// binstr.cpp:500
char *RemoveA(char *str, const char *chars) {
	cTokenS token;

	char *pBuf = new char[strlen(str) + 1];

	pBuf[0] = '\0'; token.Init(str, strlen(str));
	str[0] = '\0';

	bool bRet;
	do {
		bRet = token.GetTokenFullSep(pBuf, chars, strlen(chars));
		lstrcatA(str, pBuf);
	} while (bRet);

	delete[] pBuf;

	return str;
}

// binstr.cpp:525
char *MakePriceStrA(char *buf, __int64 price) {
	sprintf(buf, "%I64d", price);

	int len = strlen(buf);
	int newLen = len + ((len - 1 > 0) ? (len - 1) / 3 : 0);

	buf[newLen] = '\0';
	buf[newLen - 1] = buf[len - 1];

	for (int i = len - 2, j = newLen - 2; i >= 0 && j >= 0; i--, j--) {

		if ((len - 1 - i) % 3 == 0) {
			buf[j--] = ',';
		}
		buf[j] = buf[i];
	}

	return buf;
}

// binstr.cpp:548
char *UpperCase(const char *src, char *dest) {
	int len = strlen(src);

	for (int i = 0; i < len; i++) {

		if (src[i] & 0x80) {
			i++;
			continue;
		}
		dest[i] = (src[i] >= 'a' && src[i] <= 'z') ? src[i] - 0x20 : src[i];
	}

	dest[len] = '\0';
	return dest;
}

// binstr.cpp:565
char *LowerCase(const char *src, char *dest) {
	int len = strlen(src);

	for (int i = 0; i < len; i++) {

		if (src[i] & 0x80) {
			i++;
			continue;
		}
		dest[i] = (src[i] >= 'A' && src[i] <= 'Z') ? src[i] + 0x20 : src[i];
	}

	dest[len] = '\0';
	return dest;
}

// binstr.cpp:583
void MakeShortFileName(const char *path, char *dest) {
	cTokenV token;
	token.Init(path, strlen(path));

	int num = token.GetTokenNum(0, "/\\", 2);
	for (int i = 0; i < num; i++) {

		token.GetToken(dest, "/\\", 2);
	}
}

// binstr.cpp:612
int __cdecl mbscut(char *dest, const char *src, unsigned int len) {
	if (len == 0) {
		dest[0] = '\0';
		return 0;
	}

	if (strlen(src) > len) {
		if (src[len - 1] & 0x80) {
			unsigned int i;
			for (i = 0; i < len - 1; i++) {
				if (src[i] & 0x80) {
					i++;
				}
			}
			len = i;
		}
	}

	strncpy(dest, src, len);
	return strlen(dest);
}

// binstr.cpp:596
void ExtractPath(const char *path, char *dest) {
	cTokenV token;
	token.Init(path, strlen(path));

	char *pToken;
	dest[0] = '\0'; int num = token.GetTokenNum(0, "/\\", 2) - 1;
	for (int i = 0; i < num; i++) {


		token.GetToken(&pToken, "/\\", 2);
		lstrcatA(dest, pToken);
		lstrcatA(dest, "\\");
	}
}
