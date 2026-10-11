// Original: d:\build\custom\temp\code\client\projectg\logininfo.cpp
#include "LoginInfo.h"

#include <algorithm>
#include <string.h>
#include <stdlib.h>

// Declared in s5/utilities.h (S5::UTIL), which does not declare it yet.
namespace S5 {
namespace UTIL {
int DecodingUrl(const char* src, char* dst);
} // namespace UTIL
} // namespace S5

// logininfo.cpp:37
int NtreevLogin::Init() {
	_sNtreevUserIdentity identity;
	memset(&identity, 0, sizeof(identity));

	SetNtreevUserIdentity((const char*)GetChannelingChecker()->GetMapData(), identity);

	Convert(&identity);

	return TRUE;
}

// logininfo.cpp:65
void NtreevLogin::SetProvType() {
	m_provType = 2;
}

// logininfo.cpp:69
void NtreevLogin::Convert(void* data) {
	_sNtreevUserIdentity* identity = (_sNtreevUserIdentity*)data;

	m_id = identity->loginId;
	m_fullId = identity->loginId;
	m_passwd = identity->authKey;
	m_authUid = identity->memberNo;
	m_pcBang = identity->pcBangNo != 0;

	m_birthDay = "19801129";
	m_zipCode = "131-116";

	m_gender = identity->sex != 'M';
}

// logininfo.cpp:89
void NtreevLogin::SetNtreevUserIdentity(const char* text, _sNtreevUserIdentity& identity) {
	std::string source(text);
	std::string key;
	std::string value;
	size_t end = 0;

	do {
		size_t begin = source.find('|', end);
		end = source.find('|', ++begin);

		std::string token;
		if (begin != std::string::npos) {
			if (end != std::string::npos) {
				token.assign(source.begin() + begin, source.begin() + end);
			} else {
				token.assign(source.begin() + begin, source.end());
			}

			size_t equal = token.find('=');

			key.assign(token.begin(), token.begin() + equal);
			value.assign(token.begin() + equal + 1, token.end());

			if (_stricmp(key.c_str(), "LoginID") == 0) {
				strncpy(identity.loginId, value.c_str(), 0x32);
			} else if (_stricmp(key.c_str(), "AuthKey") == 0) {
				strncpy(identity.authKey, value.c_str(), 0x40);
			} else if (_stricmp(key.c_str(), "Sex") == 0) {
				identity.sex = value != "M";
			} else if (_stricmp(key.c_str(), "MemberNo") == 0) {
				identity.memberNo = atoi(value.c_str());
			} else if (_stricmp(key.c_str(), "Age") == 0) {
				identity.age = atoi(value.c_str());
			} else if (_stricmp(key.c_str(), "WardNo") == 0) {
				identity.wardNo = atoi(value.c_str());
			} else if (_stricmp(key.c_str(), "CityCode") == 0) {
				identity.cityCode = atoi(value.c_str());
			} else if (_stricmp(key.c_str(), "PCBangNo") == 0) {
				identity.pcBangNo = atoi(value.c_str());
			}
		}
	} while (end != std::string::npos);
}

// logininfo.cpp:150
int NhnLogin::Init() {
	_sNhnUserIdentity identity;
	memset(&identity, 0, sizeof(identity));
	SetNhnUserIdentity((const char*)GetChannelingChecker()->GetMapData(), identity);
	Convert(&identity);

	return TRUE;
}

// logininfo.cpp:161
void NhnLogin::SetProvType() {
	m_provType = 4;
}

// logininfo.cpp:166
void NhnLogin::SetNHNMemberNumber(__int64 memberNumber) {
	m_nhnMemberNumber = memberNumber;
}

// logininfo.cpp:173
__int64 NhnLogin::GetNHNMemberNumber() const {
	return m_nhnMemberNumber;
}

// logininfo.cpp:178
void NhnLogin::SetSiteCookie(const char* siteCookie) {
	m_siteCookie = siteCookie;
}

// logininfo.cpp:183
const char* NhnLogin::GetSiteCookie() const {
	return m_siteCookie.c_str();
}

// logininfo.cpp:188
void NhnLogin::SetNhnUserIdentity(const char* text, _sNhnUserIdentity& identity) {
	std::string source(text);
	std::string key;
	std::string value;
	size_t end = 0;

	do {
		size_t begin = source.find('|', end);
		end = source.find('|', ++begin);

		std::string token;
		if (begin != std::string::npos) {
			if (end != std::string::npos) {
				token.assign(source.begin() + begin, source.begin() + end);
			} else {
				token.assign(source.begin() + begin, source.end());
			}

			size_t equal = token.find('=');

			key.assign(token.begin(), token.begin() + equal);
			value.assign(token.begin() + equal + 1, token.end());

			if (_stricmp(key.c_str(), "LoginID") == 0) {
				strncpy(identity.loginId, value.c_str(), 0x50);
			} else if (_stricmp(key.c_str(), "AuthKey") == 0) {
				strncpy(identity.authKey, value.c_str(), 0x40);
			} else if (_stricmp(key.c_str(), "SiteCookie") == 0) {
				strncpy(identity.siteCookie, value.c_str(), 0x3FF);
			} else if (_stricmp(key.c_str(), "Sex") == 0) {
				identity.sex = value == "M";
			} else if (_stricmp(key.c_str(), "MemberNo") == 0) {
				identity.memberNo = atoi(value.c_str());
			} else if (_stricmp(key.c_str(), "Age") == 0) {
				identity.age = atoi(value.c_str());
			} else if (_stricmp(key.c_str(), "WardNo") == 0) {
				identity.wardNo = atoi(value.c_str());
			} else if (_stricmp(key.c_str(), "CityCode") == 0) {
				identity.cityCode = atoi(value.c_str());
			} else if (_stricmp(key.c_str(), "PCBangNo") == 0) {
				identity.pcBangNo = atoi(value.c_str());
			} else if (_stricmp(key.c_str(), "SiteMemberNo") == 0) {
				identity.siteMemberNo = _atoi64(value.c_str());
			} else {
				char siteCode[32] = { 0 };
				if (_stricmp(key.c_str(), "SiteCode") == 0) {
					strncpy(siteCode, value.c_str(), 0x1F);
				}

				if (memcmp(siteCode, "NHN", 4) == 0) {
					identity.provType = 4;
				}
			}
		}
	} while (end != std::string::npos);
}

// logininfo.cpp:248
void NhnLogin::Convert(void* data) {
	_sNhnUserIdentity* identity = (_sNhnUserIdentity*)data;

	char loginId[22] = { 0 };
	S5::UTIL::DecodingUrl(identity->loginId, loginId);

	m_id = loginId;
	m_fullId = loginId;

	m_passwd = identity->authKey;
	m_authUid = identity->memberNo;
	m_pcBang = identity->pcBangNo != 0;
	m_nhnMemberNumber = identity->siteMemberNo;
	m_birthDay = "19801129";
	m_zipCode = "131-116";
	m_siteCookie = "HG_CP_LOGIN=";
	m_siteCookie += identity->siteCookie;
	m_gender = identity->sex != 'M';
}

// logininfo.cpp:343
CLoginInfo::CLoginInfo(void) : m_pLoginInfo(NULL), m_webLogin(0) {
}

// logininfo.cpp:348
int CLoginInfo::Destroy(void) {
	if (m_pLoginInfo != NULL) {
		delete m_pLoginInfo;
		m_pLoginInfo = NULL;
	}
	if (m_pInst != NULL) {
		delete m_pInst;
		m_pInst = NULL;
	}

	return TRUE;
}

// logininfo.cpp:356
CLoginInfo::~CLoginInfo(void) {
}

// logininfo.cpp:362
int CLoginInfo::InitWebLogin(const char *cmdLine) {
	std::string commandLine(cmdLine);

	if (commandLine.find("-vwccnp") != std::string::npos) {
		ChannelingChecker checker;
		checker.ChannelingCheck();

		switch (checker.GetProvType()) {
		case 2:
			m_pLoginInfo = new NtreevLogin;
			break;
		case 4:
			m_pLoginInfo = new NhnLogin;
			break;
		default:
			m_pLoginInfo = NULL;
			break;
		}

		if (m_pLoginInfo != NULL) {
			static_cast<BaseLoginInfo*>(m_pLoginInfo)->DependChannelingChecker(&checker);
		}
	}

	if (m_pLoginInfo != NULL) {
		if (m_pLoginInfo->Init()) {
			m_pLoginInfo->SetProvType();
			m_webLogin = 1;
			return TRUE;
		}

		delete m_pLoginInfo;
	}

	m_pLoginInfo = new PangYaLogin;

	return FALSE;
}

// logininfo.cpp:427
unsigned long CLoginInfo::ProvType(void) const {
	return m_pLoginInfo->ProvType();
}

// logininfo.cpp:433
const char *CLoginInfo::FullId(void) const {
	return m_pLoginInfo->FullId();
}

// logininfo.cpp:439
const char *CLoginInfo::Id(void) const {
	return m_pLoginInfo->Id();
}

// logininfo.cpp:445
unsigned long CLoginInfo::Uid(void) const {
	return m_pLoginInfo->Uid();
}

// logininfo.cpp:451
const char *CLoginInfo::Passwd(void) const {
	return m_pLoginInfo->Passwd();
}

// logininfo.cpp:466
int CLoginInfo::IsPcBang(void) const {
	return m_pLoginInfo->IsPcBang();
}

// logininfo.cpp:475
unsigned long CLoginInfo::AuthUid(void) const {
	return m_pLoginInfo->AuthUid();
}

// logininfo.cpp:480
unsigned char CLoginInfo::Gender(void) const {
	return m_pLoginInfo->Gender();
}

// logininfo.cpp:485
const char *CLoginInfo::ZipCode(void) const {
	return m_pLoginInfo->ZipCode();
}

// logininfo.cpp:490
const char *CLoginInfo::BirthDay(void) const {
	return m_pLoginInfo->BirthDay();
}

// logininfo.cpp:495
void CLoginInfo::EnablePcBang(int enable) {
	m_pLoginInfo->EnablePcBang(enable);
}

// logininfo.cpp:500
void CLoginInfo::SetId(const std::string &id) {
	m_pLoginInfo->Id(id);
	m_pLoginInfo->FullId(id);
}

// logininfo.cpp:506
void CLoginInfo::SetUid(unsigned long uid) {
	m_pLoginInfo->Uid(uid);
}

// logininfo.cpp:511
void CLoginInfo::SetAuthUid(unsigned long authUid) {
	m_pLoginInfo->AuthUid(authUid);
}

// logininfo.cpp:516
void CLoginInfo::SetPw(const std::string &passwd) {
	m_pLoginInfo->Passwd(passwd);
}

// logininfo.cpp:522
void CLoginInfo::SetGender(unsigned char gender) {
	m_pLoginInfo->Gender(gender);
}

// logininfo.cpp:527
void CLoginInfo::SetZipCode(const std::string &zipCode) {
	m_pLoginInfo->ZipCode(zipCode);
}

// logininfo.cpp:532
void CLoginInfo::SetBirthDay(const std::string &birthDay) {
	m_pLoginInfo->BirthDay(birthDay);
}

// logininfo.cpp:537
ILoginInfo *CLoginInfo::QueryInterface(void) {
	return m_pLoginInfo;
}

// logininfo.cpp:543
void CLoginInfo::SetProvType(unsigned long provType) {
	m_pLoginInfo->ProvType(provType);
}

// logininfo.cpp:549
bool cHttpNtreevAuthResult::GetResult() {
	std::string result = GetElement("result");
	if (result == "true") {
		return true;
	}

	return false;
}

// logininfo.cpp:558
std::string cHttpNtreevAuthResult::GetMessage() {
	std::string messages = GetElement("messages");

	char text[MAX_PATH] = { 0 };
	wchar_t wideText[MAX_PATH] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, messages.c_str(), -1, wideText, MAX_PATH);
	WideCharToMultiByte(CP_OEMCP, 0, wideText, -1, text, MAX_PATH, NULL, NULL);

	return std::string(text);
}

// logininfo.cpp:574
std::string cHttpNtreevAuthResult::GetElement(const char* element) {
	std::string openTag("<");
	openTag += element;
	openTag += ">";

	std::string closeTag("</");
	closeTag += element;
	closeTag += ">";

	size_t begin = m_response.find(openTag);
	size_t end = m_response.find(closeTag);
	if (begin != std::string::npos && end != std::string::npos) {
		begin += openTag.length();
		return std::string(m_response.begin() + begin, m_response.begin() + end);
	}

	return "";
}

// Collects the characters of a string that are not in m_trimChars.
struct FnTrim {
	FnTrim(const char* trimChars);
	void operator()(char c);

	std::string m_trimChars; // 0x0
	std::string m_result; // 0x1C
};

// logininfo.cpp:599
FnTrim::FnTrim(const char* trimChars) : m_trimChars(trimChars), m_result("") {}

// logininfo.cpp:600
void FnTrim::operator()(char c) {
	if (m_trimChars.find(c) == std::string::npos) {
		m_result += c;
	}
}

// logininfo.cpp:607
std::string cHttpNtreevAuthResult::GetFieldValue(const char* field) {
	std::string args = GetElement("arg");

	FnTrim trim = std::for_each(args.begin(), args.end(), FnTrim(" \t\n\r\0"));
	args = trim.m_result;

	size_t pos = args.find(field);
	if (pos == std::string::npos) {
		return "";
	}

	size_t equal = args.find("=", pos + 1);
	size_t bar = args.find("|", equal + 1);
	if (equal != std::string::npos) {
		if (bar != std::string::npos) {
			return std::string(args.begin() + equal + 1, args.begin() + bar);
		}
		return std::string(args.begin() + equal + 1, args.end());
	}

	return "";
}

// logininfo.cpp:632
std::string cHttpNtreevAuthResult::GetExternalLink() {
	return "";
}
