// Original: d:\build\custom\temp\code\client\projectg\approachdesc.cpp
#include <stdio.h>
#include <string.h>
#include "ApproachDesc.h"
#include "commonutil.h"

// Each approach mission is a DescBlock whose constructor builds the
// description text from the mission parameters.
#define BEGIN_MISSION(n) \
class Mission_##n : public DescBlock { \
public: \
	Mission_##n(unsigned long dwParam1, unsigned long dwParam2, char const* pszParam) { \
		BuildDesc(dwParam1, dwParam2, pszParam); \
	} \
	virtual void BuildDesc(unsigned long dwParam1, unsigned long dwParam2, char const* pszParam) {

#define END_MISSION } };




















BEGIN_MISSION(1)
	char szBuf[8] = { 0, };
	if (dwParam1 == 0) { const char* psz = "\xC2\xA6\xBC\xF6"; strcpy(szBuf, psz); }
	else { const char* psz = "\xC8\xA6\xBC\xF6"; strcpy(szBuf, psz); }
	strcpy(m_szDesc, MakeStr("1\xC0\xA7 \xC0\xAF\xC0\xFA\xC0\xC7 \xBA\xF1\xB0\xC5\xB8\xAE\xB0\xA1 %s\xC0\xCF \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", szBuf));
END_MISSION




BEGIN_MISSION(2)
	char szBuf[8] = { 0, };
	if (dwParam2 == 0) { const char* psz = "\xC2\xA6\xBC\xF6"; strcpy(szBuf, psz); }
	else { const char* psz = "\xC8\xA6\xBC\xF6"; strcpy(szBuf, psz); }
	strcpy(m_szDesc, MakeStr("1~3\xC0\xA7\xC0\xC7 \xBA\xF1\xB0\xC5\xB8\xAE\xC0\xC7 \xC7\xD5\xB0\xE8\xB0\xA1 %s\xC0\xCF \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", szBuf));
END_MISSION




BEGIN_MISSION(3)
	const char* pszDesc = "OUT(\xC4\xA8\xC0\xCE)\xC0\xBB \xC7\xDF\xC0\xBB \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8."; strcpy(m_szDesc, pszDesc);
END_MISSION




BEGIN_MISSION(4)
	const char* pszDesc = "\xC0\xFC\xBF\xF8\xC0\xCC 20\xC3\xCA\xB0\xA1 \xC1\xF6\xB3\xAA\xB1\xE2 \xC0\xFC\xBF\xA1 \xC5\xB8\xB1\xB8\xB8\xA6 \xBF\xCF\xB7\xE1\xC7\xDF\xC0\xBB \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8."; strcpy(m_szDesc, pszDesc);
END_MISSION




BEGIN_MISSION(5)



	char szBuf[8] = { 0, };

	if (dwParam1 == 0) { const char* psz = "\xBF\xA9\xC0\xDA"; strcpy(szBuf, psz); }
	else { const char* psz = "\xB3\xB2\xC0\xDA"; strcpy(szBuf, psz); }
	strcpy(m_szDesc, MakeStr("1~3\xC0\xA7\xB0\xA1 \xC0\xFC\xBA\xCE %s(\xC4\xB3\xB8\xAF\xC5\xCD)\xC0\xCF \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", szBuf));
END_MISSION




BEGIN_MISSION(6)
	char szBuf[16] = { 0, };
	switch (dwParam1) {

	case 1: { const char* psz = "\xB1\xEE\xB8\xC1\xC1\xA6\xB8\xAE"; strcpy(szBuf, psz); } break;
	case 2: { const char* psz = "\xC7\xCF\xBE\xE7\xC1\xA6\xB8\xAE"; strcpy(szBuf, psz); } break;
	case 3: { const char* psz = "\xBA\xF4\xB8\xAE"; strcpy(szBuf, psz); } break;
	case 4: { const char* psz = "\xB6\xF3\xB9\xCC"; strcpy(szBuf, psz); } break;
	}
	strcpy(m_szDesc, MakeStr("%s\xBF\xCD \xC7\xD4\xB2\xB2 \xC7\xC3\xB7\xB9\xC0\xCC\xC7\xD1 \xC0\xAF\xC0\xFA\xB0\xA1 1\xB5\xEE\xC0\xCF \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", szBuf));

END_MISSION




BEGIN_MISSION(7)
	sprintf(m_szDesc, "%d\xC0\xA7\xB8\xA6 \xC7\xD1 \xC0\xAF\xC0\xFA\xB4\xC2 \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", dwParam1);
END_MISSION




BEGIN_MISSION(8)
	char szBuf[8] = { 0, };
	if (dwParam1 == 0) { const char* psz = "\xC2\xA6\xBC\xF6"; strcpy(szBuf, psz); }
	else { const char* psz = "\xC8\xA6\xBC\xF6"; strcpy(szBuf, psz); }
	strcpy(m_szDesc, MakeStr("\xB8\xF0\xB5\xE7 \xC0\xAF\xC0\xFA\xC0\xC7 \xB3\xB2\xC0\xBA \xB0\xC5\xB8\xAE\xB0\xA1 %s\xC0\xCF \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", szBuf));
END_MISSION




BEGIN_MISSION(9)
	char szBuf[8] = { 0, };
	if (dwParam1 == 0) { const char* psz = "\xBF\xA9\xC0\xDA"; strcpy(szBuf, psz); }
	else { const char* psz = "\xB3\xB2\xC0\xDA"; strcpy(szBuf, psz); }
	strcpy(m_szDesc, MakeStr("1\xC0\xA7\xB0\xA1 %s(\xC4\xB3\xB8\xAF\xC5\xCD)\xC0\xCF \xB0\xE6\xBF\xEC \xB8\xF0\xB5\xE7 %s(\xC4\xB3\xB8\xAF\xC5\xCD)\xB4\xC2 \xB9\xCC\xBC\xC7 \xBC\xBA\xB0\xF8.", szBuf, szBuf));
END_MISSION




BEGIN_MISSION(10)
	char szBuf[16] = { 0, };
	switch (dwParam2) {

	case 0: { const char* psz = "\xB4\xA9\xB8\xAE"; strcpy(szBuf, psz); } break;
	case 1: { const char* psz = "\xC7\xCF\xB3\xAA"; strcpy(szBuf, psz); } break;
	case 2: { const char* psz = "\xBE\xC6\xC0\xFA"; strcpy(szBuf, psz); } break;
	case 3: { const char* psz = "\xBC\xBC\xBD\xC7"; strcpy(szBuf, psz); } break;
	case 4: { const char* psz = "\xB8\xC6\xBD\xBA"; strcpy(szBuf, psz); } break;
	case 5: { const char* psz = "\xC4\xED"; strcpy(szBuf, psz); } break;
	case 6: { const char* psz = "\xBE\xC6\xB8\xB0"; strcpy(szBuf, psz); } break;
	case 7: { const char* psz = "\xC4\xAB\xC1\xEE"; strcpy(szBuf, psz); } break;
	case 8: { const char* psz = "\xB7\xE7\xBD\xC3\xBE\xC6"; strcpy(szBuf, psz); } break;
	case 9: { const char* psz = "\xB3\xDA"; strcpy(szBuf, psz); } break;
	}
	strcpy(m_szDesc, MakeStr("\xB3\xB2\xC0\xBA\xB0\xC5\xB8\xAE\xB0\xA1 10y \xB9\xCC\xB8\xB8\xC0\xC7 \xB8\xF0\xB5\xE7 %s \xC0\xAF\xC0\xFA\xB4\xC2 \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", szBuf));
END_MISSION




BEGIN_MISSION(11)
	char szName[2][8] = { 0, };
	if (dwParam1 == 0) {

		{ const char* psz = "\xC2\xA6\xBC\xF6"; strcpy(szName[0], psz); }
		{ const char* psz = "\xC8\xA6\xBC\xF6"; strcpy(szName[1], psz); }

	} else {

		{ const char* psz = "\xC8\xA6\xBC\xF6"; strcpy(szName[0], psz); }
		{ const char* psz = "\xC2\xA6\xBC\xF6"; strcpy(szName[1], psz); }
	}
	strcpy(m_szDesc, MakeStr("\xB3\xB2\xC0\xBA\xB0\xC5\xB8\xAE\xB0\xA1 %s\xC0\xCE \xC0\xAF\xC0\xFA\xB0\xA1 %s\xC0\xCE \xC0\xAF\xC0\xFA\xBA\xB8\xB4\xD9 \xB8\xB9\xC0\xB8\xB8\xE9 \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", szName[0], szName[1]));
END_MISSION




BEGIN_MISSION(12)
	const char* pszDesc = "\xB0\xFA\xB9\xDD\xBC\xF6 \xC0\xCC\xBB\xF3\xC0\xCC OUT(\xC4\xA8\xC0\xCE)\xC0\xBB \xC7\xDF\xC0\xBB \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8."; strcpy(m_szDesc, pszDesc);
END_MISSION




BEGIN_MISSION(13)
	char szBuf[8] = { 0, };
	if (dwParam1 == 0) { const char* psz = "\xBF\xA9\xC0\xDA"; strcpy(szBuf, psz); }
	else { const char* psz = "\xB3\xB2\xC0\xDA"; strcpy(szBuf, psz); }
	strcpy(m_szDesc, MakeStr("\xB3\xB2\xC0\xBA\xB0\xC5\xB8\xAE 1y\xC0\xCC\xB3\xBB\xC0\xC7 \xC0\xAF\xC0\xFA\xB0\xA1 %s(\xC4\xB3\xB8\xAF\xC5\xCD)\xC0\xCE \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8", szBuf));
END_MISSION




BEGIN_MISSION(14)
	char szBuf[8] = { 0, };
	switch (dwParam1) {

	case 1: { const char* psz = "1y"; strcpy(szBuf, psz); } break;
	case 2: { const char* psz = "2y"; strcpy(szBuf, psz); } break;
	case 3: { const char* psz = "3y"; strcpy(szBuf, psz); } break;
	case 4: { const char* psz = "4y"; strcpy(szBuf, psz); } break;
	case 5: { const char* psz = "5y"; strcpy(szBuf, psz); } break;
	}
	strcpy(m_szDesc, MakeStr("\xB3\xB2\xC0\xBA\xB0\xC5\xB8\xAE\xB0\xA1 %s\xB9\xCC\xB8\xB8\xC0\xCE \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8", szBuf));
END_MISSION




BEGIN_MISSION(15)
	char szBuf[8] = { 0, };
	if (dwParam2 == 0) { const char* psz = "\xC2\xA6\xBC\xF6"; strcpy(szBuf, psz); }
	else { const char* psz = "\xC8\xA6\xBC\xF6"; strcpy(szBuf, psz); }
	strcpy(m_szDesc, MakeStr("%d\xC0\xA7 \xC0\xAF\xC0\xFA\xC0\xC7 \xB3\xB2\xC0\xBA\xB0\xC5\xB8\xAE\xB0\xA1 %s\xC0\xCF \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", dwParam1, szBuf));
END_MISSION




BEGIN_MISSION(16)
	const char* pszDesc = "4\xB8\xED \xC0\xCC\xBB\xF3\xC0\xC7 \xC0\xAF\xC0\xFA\xB0\xA1 \xC4\xA8\xC0\xCE(OUT)\xC0\xBB \xC7\xD1 \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8"; strcpy(m_szDesc, pszDesc);
END_MISSION




BEGIN_MISSION(17)
	strcpy(m_szDesc, MakeStr("\xB8\xF0\xB5\xE7\xC0\xAF\xC0\xFA\xC0\xC7 \xB3\xB2\xC0\xBA\xB0\xC5\xB8\xAE \xC7\xD5\xB0\xE8\xB0\xA1 %dy \xB9\xCC\xB8\xB8\xC0\xCF \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", dwParam2));
END_MISSION




BEGIN_MISSION(18)
	sprintf(m_szDesc, "\xB3\xB2\xC0\xBA\xB0\xC5\xB8\xAE\xB0\xA1 %d.%d \xC0\xCE \xC0\xAF\xC0\xFA\xB0\xA1 \xC0\xD6\xC0\xBB \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", dwParam1, dwParam2);
END_MISSION




BEGIN_MISSION(19)
	char szName[2][8] = { 0, };
	if (dwParam1 == 0) { const char* psz = "\xB3\xB2\xC0\xDA"; strcpy(szName[0], psz); }
	else { const char* psz = "\xBF\xA9\xC0\xDA"; strcpy(szName[0], psz); }

	if (dwParam2 == 0) { const char* psz = "\xC8\xA6\xBC\xF6"; strcpy(szName[1], psz); }
	else { const char* psz = "\xC2\xA6\xBC\xF6"; strcpy(szName[1], psz); }
	strcpy(m_szDesc, MakeStr("%s(\xC4\xB3\xB8\xAF\xC5\xCD)\xC0\xAF\xC0\xFA\xC0\xC7 \xB3\xB2\xC0\xBA\xB0\xC5\xB8\xAE \xC7\xD5\xB0\xE8\xB0\xA1 %s\xC0\xCE \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", szName[0], szName[1]));
END_MISSION




BEGIN_MISSION(20)
	const char* pszDesc = "\xB3\xB2\xC0\xBA\xB0\xC5\xB8\xAE\xB0\xA1 \xB0\xA1\xC0\xE5 \xB8\xD5 \xC0\xAF\xC0\xFA\xC0\xCF \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8."; strcpy(m_szDesc, pszDesc);
END_MISSION




BEGIN_MISSION(21)
	sprintf(m_szDesc, MakeStr("\xB8\xF0\xB5\xE7\xC0\xAF\xC0\xFA\xC0\xC7 \xB3\xB2\xC0\xBA\xBD\xC3\xB0\xA3\xC0\xCC %d\xC3\xCA \xB9\xCC\xB8\xB8\xC0\xCF \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", dwParam1));
END_MISSION




BEGIN_MISSION(22)
	const char* pszDesc = "1\xC0\xA7\xC7\xD1 \xC0\xAF\xC0\xFA\xC0\xC7 \xB3\xB2\xC0\xBA\xBD\xC3\xB0\xA3\xC0\xCC 10\xC3\xCA \xB9\xCC\xB8\xB8\xC0\xCF\xB6\xA7 \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8."; strcpy(m_szDesc, pszDesc);
END_MISSION




BEGIN_MISSION(23)
	char szBuf[16] = { 0, };
	switch (dwParam1) {

	case 0: { const char* psz = "\xBA\xC0\xB4\xD9\xB8\xAE"; strcpy(szBuf, psz); } break;
	case 1: { const char* psz = "\xC7\xC7\xC7\xC9"; strcpy(szBuf, psz); } break;
	case 2: { const char* psz = "\xB6\xEC\xB6\xA5\xBB\xD1"; strcpy(szBuf, psz); } break;
	case 3: { const char* psz = "\xB5\xB9\xC7\xC7\xB4\xCF"; strcpy(szBuf, psz); } break;
	case 4: { const char* psz = "\xB7\xCE\xB7\xCE"; strcpy(szBuf, psz); } break;
	case 5: { const char* psz = "\xC5\xA5\xB8\xB6"; strcpy(szBuf, psz); } break;
	case 6: { const char* psz = "\xC6\xBC\xC5\xB0"; strcpy(szBuf, psz); } break;
	case 7: { const char* psz = "\xC4\xAB\xB5\xF0\xBF\xA1"; strcpy(szBuf, psz); } break;
	}
	sprintf(m_szDesc, "\xB3\xB2\xC0\xBA\xB0\xC5\xB8\xAE 20y\xB9\xCC\xB8\xB8, %s(\xC4\xB3\xB5\xF0)\xBB\xE7\xBF\xEB \xC0\xAF\xC0\xFA\xB4\xC2 \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", szBuf);
END_MISSION




BEGIN_MISSION(24)
	const char* pszDesc = "\xB0\xA1\xC0\xE5 \xBA\xFC\xB8\xA5\xBD\xC3\xB0\xA3\xBE\xC8\xBF\xA1 \xC5\xB8\xB1\xB8\xC7\xD1 \xC0\xAF\xC0\xFA\xC0\xCF \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8."; strcpy(m_szDesc, pszDesc);
END_MISSION




BEGIN_MISSION(25)
	char szBuf[8] = { 0, };
	if (dwParam1 == 0) { const char* psz = "\xC2\xA6\xBC\xF6"; strcpy(szBuf, psz); }
	else { const char* psz = "\xC8\xA6\xBC\xF6"; strcpy(szBuf, psz); }
	strcpy(m_szDesc, MakeStr("\xB3\xB2\xC0\xBA\xB0\xC5\xB8\xAE\xB0\xA1 %s\xC0\xCE \xC0\xAF\xC0\xFA\xC0\xCF \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", szBuf));
END_MISSION




BEGIN_MISSION(26)
	const char* pszDesc = "\xB0\xA1\xC0\xE5 \xB4\xCA\xB0\xD4 \xC5\xB8\xB1\xB8\xC7\xD1 \xC0\xAF\xC0\xFA\xC0\xCF \xB0\xE6\xBF\xEC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8."; strcpy(m_szDesc, pszDesc);
END_MISSION




BEGIN_MISSION(27)
	const char* pszDesc = "\xC4\xB3\xB8\xAF\xC5\xCD \xBC\xBA\xBA\xB0\xB7\xCE \xB3\xB2\xC0\xBA\xB0\xC5\xB8\xAE \xC6\xF2\xB1\xD5\xC0\xBB \xB3\xBB\xBE\xEE \xC0\xFB\xC0\xBA\xC2\xCA\xC0\xCC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8."; strcpy(m_szDesc, pszDesc);
END_MISSION




BEGIN_MISSION(28)
	const char* pszDesc = "\xC4\xB3\xB8\xAF\xC5\xCD \xBC\xBA\xBA\xB0\xB7\xCE \xB3\xB2\xC0\xBA\xBD\xC3\xB0\xA3 \xC6\xF2\xB1\xD5\xC0\xBB \xB3\xBB\xBE\xEE \xB8\xB9\xC0\xBA\xC2\xCA\xC0\xCC \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8."; strcpy(m_szDesc, pszDesc);
END_MISSION




BEGIN_MISSION(29)
	if (pszParam)
		strcpy(m_szDesc, MakeStr("[%s]\xB4\xD4\xC0\xCC \xC4\xA8\xC0\xCE\xC0\xBB \xC7\xCF\xB8\xE9 \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8.", pszParam));
END_MISSION




BEGIN_MISSION(30)
	char szBuf[32] = { 0, };
	switch (dwParam1) {

	case 4: { const char* psz = "\xBD\xBA\xC6\xC4\xC0\xCC\xC5\xA9"; strcpy(szBuf, psz); } break;
	case 8: { const char* psz = "\xC4\xDA\xBA\xEA\xB6\xF3"; strcpy(szBuf, psz); } break;
	case 2: { const char* psz = "\xC5\xE4\xB8\xB6\xC8\xA3\xC5\xA9"; strcpy(szBuf, psz); } break;
	}
	sprintf(m_szDesc, "%s\xB8\xA6 \xBB\xE7\xBF\xEB\xC7\xCF\xBF\xA9 \xC4\xA8\xC0\xCE\xC0\xBB \xC7\xD1 \xC0\xAF\xC0\xFA\xB4\xC2 \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8", szBuf);
END_MISSION




BEGIN_MISSION(31)
	const char* pszDesc = "\xB0\xA1\xC0\xE5 \xBB\xA1\xB8\xAE \xC4\xA8\xC0\xCE\xC0\xBB \xC7\xD1 \xC0\xAF\xC0\xFA\xB4\xC2 \xB9\xCC\xBC\xC7\xBC\xBA\xB0\xF8."; strcpy(m_szDesc, pszDesc);
END_MISSION








CApproachDesc::CApproachDesc(int nMission, unsigned long dwParam1, unsigned long dwParam2, char const* pszParam) {

	m_pDesc = NULL;
	switch (nMission) {
	case 1: m_pDesc = new Mission_1(dwParam1, dwParam2, pszParam); break;
	case 2: m_pDesc = new Mission_2(dwParam1, dwParam2, pszParam); break;
	case 3: m_pDesc = new Mission_3(dwParam1, dwParam2, pszParam); break;
	case 4: m_pDesc = new Mission_4(dwParam1, dwParam2, pszParam); break;
	case 5: m_pDesc = new Mission_5(dwParam1, dwParam2, pszParam); break;
	case 6: m_pDesc = new Mission_6(dwParam1, dwParam2, pszParam); break;
	case 7: m_pDesc = new Mission_7(dwParam1, dwParam2, pszParam); break;
	case 8: m_pDesc = new Mission_8(dwParam1, dwParam2, pszParam); break;
	case 9: m_pDesc = new Mission_9(dwParam1, dwParam2, pszParam); break;
	case 10: m_pDesc = new Mission_10(dwParam1, dwParam2, pszParam); break;
	case 11: m_pDesc = new Mission_11(dwParam1, dwParam2, pszParam); break;
	case 12: m_pDesc = new Mission_12(dwParam1, dwParam2, pszParam); break;
	case 13: m_pDesc = new Mission_13(dwParam1, dwParam2, pszParam); break;
	case 14: m_pDesc = new Mission_14(dwParam1, dwParam2, pszParam); break;
	case 15: m_pDesc = new Mission_15(dwParam1, dwParam2, pszParam); break;
	case 16: m_pDesc = new Mission_16(dwParam1, dwParam2, pszParam); break;
	case 17: m_pDesc = new Mission_17(dwParam1, dwParam2, pszParam); break;
	case 18: m_pDesc = new Mission_18(dwParam1, dwParam2, pszParam); break;
	case 19: m_pDesc = new Mission_19(dwParam1, dwParam2, pszParam); break;
	case 20: m_pDesc = new Mission_20(dwParam1, dwParam2, pszParam); break;
	case 21: m_pDesc = new Mission_21(dwParam1, dwParam2, pszParam); break;
	case 22: m_pDesc = new Mission_22(dwParam1, dwParam2, pszParam); break;
	case 23: m_pDesc = new Mission_23(dwParam1, dwParam2, pszParam); break;
	case 24: m_pDesc = new Mission_24(dwParam1, dwParam2, pszParam); break;
	case 25: m_pDesc = new Mission_25(dwParam1, dwParam2, pszParam); break;
	case 26: m_pDesc = new Mission_26(dwParam1, dwParam2, pszParam); break;
	case 27: m_pDesc = new Mission_27(dwParam1, dwParam2, pszParam); break;
	case 28: m_pDesc = new Mission_28(dwParam1, dwParam2, pszParam); break;
	case 29: m_pDesc = new Mission_29(dwParam1, dwParam2, pszParam); break;
	case 30: m_pDesc = new Mission_30(dwParam1, dwParam2, pszParam); break;
	case 31: m_pDesc = new Mission_31(dwParam1, dwParam2, pszParam); break;
	}
}
