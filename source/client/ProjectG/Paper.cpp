// Original: d:\build\custom\temp\code\client\projectg\paper.cpp
#include <stdio.h>
#include "Paper.h"
#include "Packet.h"

// Difference between the server's clock and the local one (SetServerTime).
static ATL::CTimeSpan s_serverTimeGap;

// Names of the paper themes, indexed by ePaperTheme.
extern const char* g_paperThemeNames[PAPER_THEME_COUNT];

// paper.cpp:19
CPaperMemo::CPaperMemo(const char* text)
	: m_text(text) {
}

// paper.cpp:24
CPaperMemo::~CPaperMemo(void) {
}

// paper.cpp:29
void __cdecl CPaperMemo::Write(ePaperTheme theme, const char* format, ...) {
	char text[512] = "";

	va_list args;
	va_start(args, format);
	_vsnprintf(text, sizeof(text) - 1, format, args);
	va_end(args);

	CPaper::Instance()->AddMemo(theme, m_text.c_str(), text);
}

// paper.cpp:59
CPaper::CPaper(void) {
	m_updatable = 0;
	s_serverTimeGap = 0;
}

// paper.cpp:64
CPaper::~CPaper(void) {
}

// paper.cpp:69
void CPaper::SetServerTime(const _SYSTEMTIME& time) {
	_SYSTEMTIME localTime;
	GetLocalTime(&localTime);

	ATL::CTime local(localTime);
	ATL::CTime server(time);

	s_serverTimeGap = server - local;
}

// paper.cpp:80
void CPaper::PrepareNewPaper(const char* name) {
	Destroy();

	m_name = name;
	GetServerTime(m_time);
}

// paper.cpp:88
void CPaper::Destroy(void) {
	memset(&m_time, 0, sizeof(m_time));
	memset(&m_sendTime, 0, sizeof(m_sendTime));
	m_name = "";
	m_memos.clear();
	m_funcSigs.clear();
	m_funcSigIds.clear();
}

// paper.cpp:98
bool CPaper::AddMemo(ePaperTheme theme, const char* name, const char* text) {
	if (m_updatable) {
		return false;
	}

	std::multimap<ePaperTheme, cMemo>::const_iterator first = m_memos.lower_bound(theme);
	std::multimap<ePaperTheme, cMemo>::const_iterator last = m_memos.upper_bound(theme);

	if (std::distance(first, last) < 999) {
		cMemo memo(GetServerTime(), GetFuncSigId(name), text);
		m_memos.insert(std::multimap<ePaperTheme, cMemo>::value_type(theme, memo));
		return true;
	}

	return false;
}

// paper.cpp:115
const char* CPaper::GetEnumStr(ePaperTheme theme) const {
	if (theme < PAPER_THEME_COUNT) {
		return g_paperThemeNames[theme];
	}
	return "";
}

// paper.cpp:123
const char* CPaper::GetFuncSigStr(unsigned int id) const {
	if (id > 0 && id < m_funcSigs.size()) {
		return m_funcSigs[id].c_str();
	}
	return "";
}

// paper.cpp:132
const char* CPaper::GetServerTime(void) const {
	static char s_timeString[512];
	_SYSTEMTIME localTime;
	GetLocalTime(&localTime);

	ATL::CTime server(localTime);
	server += s_serverTimeGap;

	struct tm localTm;
	server.GetLocalTm(&localTm);

	memset(s_timeString, 0, sizeof(s_timeString));
	strftime(s_timeString, sizeof(s_timeString) - 1, "%y-%m-%d %H:%M:%S", &localTm);

	return s_timeString;
}

// paper.cpp:149
unsigned int CPaper::GetMemoSize(ePaperTheme theme) const {
	std::multimap<ePaperTheme, cMemo>::const_iterator first = m_memos.lower_bound(theme);
	std::multimap<ePaperTheme, cMemo>::const_iterator last = m_memos.upper_bound(theme);

	return std::distance(first, last);
}

// paper.cpp:157
void CPaper::GetServerTime(_SYSTEMTIME& time) const {
	_SYSTEMTIME localTime;
	GetLocalTime(&localTime);

	ATL::CTime server(localTime);
	server += s_serverTimeGap;

	server.GetAsSystemTime(time);
}

// paper.cpp:168
bool CPaper::PrintOut(ePaperTheme theme) {
	std::multimap<ePaperTheme, cMemo>::const_iterator it = m_memos.lower_bound(theme);
	if (it == m_memos.end()) {
		return false;
	}

	FILE* fp = fopen("./saved/paper.xml", "wt");
	if (fp == NULL) {
		return false;
	}

	fprintf(fp, "<?xml version=\"1.0\" encoding=\"euc-kr\" standalone=\"yes\" ?>\n");

	fprintf(fp, "<?xml:stylesheet type=\"text/xsl\" href=\"paperview.xsl\" ?>\n");

	fprintf(fp, "<!DOCTYPE PAPERS[\n");
	fprintf(fp, "\t<!ELEMENT PAPERS (INDEX,PAPER)>\n");
	fprintf(fp, "\t<!ELEMENT INDEX  (USERNICK,CREATETIME,SENDTIME)>\n");
	fprintf(fp, "\t<!ELEMENT PAPER  (NUMBER,TIME,FUNCSIG,MESSAGE)>\n");
	fprintf(fp, "\t<!ELEMENT USERNICK \t(#PCDATA)>\n");
	fprintf(fp, "\t<!ELEMENT CREATETIME (#PCDATA)>\n");
	fprintf(fp, "\t<!ELEMENT SENDTIME \t(#PCDATA)>\n");
	fprintf(fp, "\t<!ELEMENT NUMBER \t(#PCDATA)>\n");
	fprintf(fp, "\t<!ELEMENT TIME\t\t(#PCDATA)>\n");
	fprintf(fp, "\t<!ELEMENT FUNCSIG\t(#PCDATA)>\n");
	fprintf(fp, "\t<!ELEMENT MESSAGE\t(#PCDATA)>\n");
	fprintf(fp, "]>\n");

	fprintf(fp, "<PAPERS>\n");

	fprintf(fp, "<INDEX>\n");
	fprintf(fp, "\t<THEME>%s</THEME>\n", GetEnumStr(theme));
	fprintf(fp, "\t<USERNICK>%s</USERNICK>\n", m_name.c_str());
	fprintf(fp, "\t<CREATETIME>%s</CREATETIME>\n", SystemTimeToString(m_time));
	fprintf(fp, "\t<SENDTIME>%s</SENDTIME>\n", GetServerTime());
	fprintf(fp, "</INDEX>\n");

	int number = 0;
	std::multimap<ePaperTheme, cMemo>::const_iterator last = m_memos.upper_bound(theme);
	for (; it != last; ++it) {
		const cMemo& memo = it->second;

		fprintf(fp, "<PAPER>\n");
		fprintf(fp, "\t<NUMBER>%d</NUMBER>\n", ++number);
		fprintf(fp, "\t<TIME>%s</TIME>\n", memo.m_time.c_str());
		fprintf(fp, "\t<FUNCSIG>%s</FUNCSIG>\n", GetFuncSigStr(memo.m_funcSig));
		fprintf(fp, "\t<MESSAGE>%s</MESSAGE>\n", memo.m_text.c_str());
		fprintf(fp, "</PAPER>\n");
	}

	fprintf(fp, "</PAPERS>\n");
	fclose(fp);

	return true;
}

// paper.cpp:230
void CPaper::ReceivedPacket(WReceivedPacket* packet) {
	packet->Decode1();

	m_updatable = 0;
}

// paper.cpp:241
void CPaper::SendUpdatableQuery(void) {
	m_updatable = 1;
}

// paper.cpp:251
void CPaper::Send(void) {
}

// paper.cpp:256
const char* CPaper::SystemTimeToString(const _SYSTEMTIME& time) const {
	static char s_timeString[512];
	ATL::CTime t(time);

	struct tm localTm;
	t.GetLocalTm(&localTm);

	memset(s_timeString, 0, sizeof(s_timeString));
	strftime(s_timeString, sizeof(s_timeString) - 1, "%y-%m-%d %H:%M:%S", &localTm);

	return s_timeString;
}

// paper.cpp:269
unsigned int CPaper::GetFuncSigId(const char* name) {
	std::map<std::string, unsigned int>::iterator it = m_funcSigIds.find(name);
	if (it != m_funcSigIds.end()) {
		return it->second;
	}

	unsigned int id = m_funcSigIds.size();
	m_funcSigIds.insert(std::map<std::string, unsigned int>::value_type(name, id));
	m_funcSigs.push_back(name);

	return id;
}
