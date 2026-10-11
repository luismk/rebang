// Original: d:\build\custom\temp\code\client\projectg\logininfo.h
#ifndef PG_CLIENT_PROJECTG_LOGININFO_H
#define PG_CLIENT_PROJECTG_LOGININFO_H

#include <windows.h>

#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"

class ChannelingChecker;

// logininfo.h:73
class ILoginInfo {
public:
	ILoginInfo() {}
	virtual ~ILoginInfo() {}

	// Declared in the vtable's order: cl 13.10 groups the overloads of a name
	// at its first declaration, in reverse declaration order.
	virtual int Init() = 0;
	virtual void SetProvType() = 0;
	virtual unsigned long ProvType() = 0;
	virtual void ProvType(unsigned long provType) = 0;
	virtual const char* FullId() = 0;
	virtual void FullId(std::string fullId) = 0;
	virtual const char* Id() = 0;
	virtual void Id(std::string id) = 0;
	virtual unsigned long Uid() = 0;
	virtual void Uid(unsigned long uid) = 0;
	virtual const char* Passwd() = 0;
	virtual void Passwd(std::string passwd) = 0;
	virtual int IsPcBang() = 0;
	virtual unsigned long AuthUid() = 0;
	virtual void AuthUid(unsigned long authUid) = 0;
	virtual void EnablePcBang(int enable) = 0;
	virtual unsigned char Gender() = 0;
	virtual void Gender(unsigned char gender) = 0;
	virtual const char* ZipCode() = 0;
	virtual void ZipCode(const std::string& zipCode) = 0;
	virtual const char* BirthDay() = 0;
	virtual void BirthDay(const std::string& birthDay) = 0;
	virtual void PcBang(int pcBang) = 0;
};
CHECK_SIZE(ILoginInfo, 0x4);

// Singleton holding the account the client logged in with. Only the members
// this project's code uses so far are declared.
class CLoginInfo {
	LAYOUT_CHECK_FRIEND;

public:
	static CLoginInfo *Instance(void);   // logininfo.h:121

	int Destroy(void);
	unsigned long ProvType(void) const;
	const char *FullId(void) const;
	const char *Id(void) const;
	unsigned long Uid(void) const;
	const char *Passwd(void) const;
	int IsWebLogin(void) const;          // logininfo.h:132
	int IsPcBang(void) const;
	unsigned long AuthUid(void) const;
	unsigned char Gender(void) const;
	const char *ZipCode(void) const;
	const char *BirthDay(void) const;
	void EnablePcBang(int enable);
	void SetId(const std::string &id);
	void SetUid(unsigned long uid);
	void SetAuthUid(unsigned long authUid);
	void SetPw(const std::string &passwd);
	void SetGender(unsigned char gender);
	void SetZipCode(const std::string &zipCode);
	void SetBirthDay(const std::string &birthDay);
	ILoginInfo *QueryInterface(void);    // logininfo.cpp:537
	void SetProvType(unsigned long provType);
	int InitWebLogin(const char *cmdLine);

private:
	CLoginInfo(void);
	virtual ~CLoginInfo(void);

	ILoginInfo *m_pLoginInfo;    // 0x4, deleted by Destroy
	int m_webLogin;              // 0x8, set by InitWebLogin

	static CLoginInfo *m_pInst;
};
CHECK_SIZE(CLoginInfo, 0xC);
BEGIN_LAYOUT_CHECK(CLoginInfo)
	CHECK_FIELD(CLoginInfo, m_pLoginInfo, 0x4);
	CHECK_FIELD(CLoginInfo, m_webLogin, 0x8);
END_LAYOUT_CHECK

// Defined empty, before its first include, by GolfRule.cpp: the original kept
// the out-of-line copy of this accessor in GolfRule.obj; every other unit
// inlines it.
#ifndef GOLFRULE_LOGININFO_INLINE
#define GOLFRULE_LOGININFO_INLINE inline
#endif

// logininfo.h:132
GOLFRULE_LOGININFO_INLINE int CLoginInfo::IsWebLogin(void) const {
	return m_webLogin;
}

// logininfo.h:200
class BaseLoginInfo : public ILoginInfo {
public:
	BaseLoginInfo()
		: m_authUid(0), m_uid(0), m_provType(2), m_pcBang(0), m_gender(0), m_channelingChecker(NULL) {}
	virtual ~BaseLoginInfo() {}

	virtual const char* FullId() { return m_fullId.c_str(); }
	virtual const char* Id() { return m_id.c_str(); }
	virtual unsigned long Uid() { return m_uid; }
	virtual unsigned long AuthUid() { return m_authUid; }
	virtual const char* Passwd() { return m_passwd.c_str(); }
	virtual unsigned long ProvType() { return m_provType; }
	virtual int IsPcBang() { return m_pcBang; }
	virtual unsigned char Gender() { return m_gender; }
	virtual const char* ZipCode() { return m_zipCode.c_str(); }
	virtual const char* BirthDay() { return m_birthDay.c_str(); }

	virtual void Id(std::string id) { m_id = id; }
	virtual void FullId(std::string fullId) { m_fullId = fullId; }
	virtual void Uid(unsigned long uid) { m_uid = uid; }
	virtual void AuthUid(unsigned long authUid) { m_authUid = authUid; }
	virtual void Passwd(std::string passwd) { m_passwd = passwd; }
	virtual void ProvType(unsigned long provType) { m_provType = provType; }
	virtual void PcBang(int pcBang) { m_pcBang = pcBang; }
	virtual void Gender(unsigned char gender) { m_gender = gender; }
	virtual void ZipCode(const std::string& zipCode) { m_zipCode = zipCode; }
	virtual void BirthDay(const std::string& birthDay) { m_birthDay = birthDay; }
	virtual void EnablePcBang(int enable) { m_pcBang = enable; }

protected:
	virtual void Convert(void* data) = 0;

public:
	void DependChannelingChecker(ChannelingChecker* checker) { m_channelingChecker = checker; }
	ChannelingChecker* GetChannelingChecker() { return m_channelingChecker; }

	std::string m_id; // 0x4
	std::string m_fullId; // 0x20
	std::string m_passwd; // 0x3C
	unsigned long m_authUid; // 0x58
	unsigned long m_uid; // 0x5C
	unsigned long m_provType; // 0x60
	int m_pcBang; // 0x64
	unsigned char m_gender; // 0x68
	std::string m_zipCode; // 0x6C
	std::string m_birthDay; // 0x88
	ChannelingChecker* m_channelingChecker; // 0xA4
};
CHECK_FIELD(BaseLoginInfo, m_id, 0x4);
CHECK_FIELD(BaseLoginInfo, m_fullId, 0x20);
CHECK_FIELD(BaseLoginInfo, m_passwd, 0x3C);
CHECK_FIELD(BaseLoginInfo, m_authUid, 0x58);
CHECK_FIELD(BaseLoginInfo, m_uid, 0x5C);
CHECK_FIELD(BaseLoginInfo, m_provType, 0x60);
CHECK_FIELD(BaseLoginInfo, m_pcBang, 0x64);
CHECK_FIELD(BaseLoginInfo, m_gender, 0x68);
CHECK_FIELD(BaseLoginInfo, m_zipCode, 0x6C);
CHECK_FIELD(BaseLoginInfo, m_birthDay, 0x88);
CHECK_FIELD(BaseLoginInfo, m_channelingChecker, 0xA4);
CHECK_SIZE(BaseLoginInfo, 0xA8);

// logininfo.h:258
class ChannelingChecker {
public:
	ChannelingChecker();
	~ChannelingChecker();

	int ChannelingCheck();
	void* GetMapData();
	unsigned long GetProvType();

private:
	void SetChannelingFlag();

	HANDLE m_mapping; // 0x0
	void* m_mapData; // 0x4
	unsigned long m_provType; // 0x8

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(ChannelingChecker, 0xC);
BEGIN_LAYOUT_CHECK(ChannelingChecker)
	CHECK_FIELD(ChannelingChecker, m_mapping, 0x0);
	CHECK_FIELD(ChannelingChecker, m_mapData, 0x4);
	CHECK_FIELD(ChannelingChecker, m_provType, 0x8);
END_LAYOUT_CHECK

// The original emits these out of line in logininfo.obj (the only includer):
// they are defined in the header without inline.

// logininfo.h:258
ChannelingChecker::ChannelingChecker() : m_mapData(NULL), m_provType(0xFFFFFFFF) {}

// logininfo.h:260
ChannelingChecker::~ChannelingChecker() {
}

// logininfo.h:269
int ChannelingChecker::ChannelingCheck() {
	m_mapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "927628CA6D76A6E9162C56D4E3E6D6E3");
	if (m_mapping == NULL) {
		return FALSE;
	}
	m_mapData = MapViewOfFile(m_mapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (m_mapData == NULL) {
		return FALSE;
	}
	SetChannelingFlag();
	return TRUE;
}

// logininfo.h:288
void* ChannelingChecker::GetMapData() { return m_mapData; }

// logininfo.h:289
unsigned long ChannelingChecker::GetProvType() { return m_provType; }

// logininfo.h:293
void ChannelingChecker::SetChannelingFlag() {
	std::string mapText((const char*)m_mapData);
	if (mapText.find("SiteCode") != std::string::npos) {
		m_provType = 4;
	} else {
		m_provType = 2;
	}
}

// Account record of a web login, parsed from the "key=value|..." text of the
// channeling mapping by NtreevLogin::SetNtreevUserIdentity.
struct _sNtreevUserIdentity {
	unsigned long memberNo; // 0x0
	char sex; // 0x4
	char age; // 0x5
	PADDING(0x6, 0x8);
	int wardNo; // 0x8
	short cityCode; // 0xC
	char pcBangNo; // 0xE
	UNKNOWN(0xF, 0x50);
	char loginId[0x33]; // 0x50
	char authKey[0x41]; // 0x83
};
CHECK_FIELD(_sNtreevUserIdentity, sex, 0x4);
CHECK_FIELD(_sNtreevUserIdentity, age, 0x5);
CHECK_FIELD(_sNtreevUserIdentity, wardNo, 0x8);
CHECK_FIELD(_sNtreevUserIdentity, cityCode, 0xC);
CHECK_FIELD(_sNtreevUserIdentity, pcBangNo, 0xE);
CHECK_FIELD(_sNtreevUserIdentity, loginId, 0x50);
CHECK_FIELD(_sNtreevUserIdentity, authKey, 0x83);
CHECK_SIZE(_sNtreevUserIdentity, 0xC4);

// Same record for an NHN (Hangame) channeling login.
struct _sNhnUserIdentity {
	unsigned long memberNo; // 0x0
	char sex; // 0x4
	char age; // 0x5
	PADDING(0x6, 0x8);
	int wardNo; // 0x8
	short cityCode; // 0xC
	char pcBangNo; // 0xE
	UNKNOWN(0xF, 0x50);
	__int64 siteMemberNo; // 0x50
	char loginId[0x51]; // 0x58
	char authKey[0x41]; // 0xA9
	char siteCookie[0x400]; // 0xEA
	char provType; // 0x4EA
	PADDING(0x4EB, 0x4F0);
};
CHECK_FIELD(_sNhnUserIdentity, sex, 0x4);
CHECK_FIELD(_sNhnUserIdentity, pcBangNo, 0xE);
CHECK_FIELD(_sNhnUserIdentity, siteMemberNo, 0x50);
CHECK_FIELD(_sNhnUserIdentity, loginId, 0x58);
CHECK_FIELD(_sNhnUserIdentity, authKey, 0xA9);
CHECK_FIELD(_sNhnUserIdentity, siteCookie, 0xEA);
CHECK_FIELD(_sNhnUserIdentity, provType, 0x4EA);
CHECK_SIZE(_sNhnUserIdentity, 0x4F0);

// logininfo.h:325
class NtreevLogin : public BaseLoginInfo {
public:
	NtreevLogin() {}
	virtual ~NtreevLogin() {}

	virtual int Init();
	virtual void SetProvType();

protected:
	virtual void Convert(void* data);

private:
	void SetNtreevUserIdentity(const char* text, _sNtreevUserIdentity& identity);
};
CHECK_SIZE(NtreevLogin, 0xA8);

// logininfo.h:353
class NhnLogin : public BaseLoginInfo {
public:
	NhnLogin() {}
	virtual ~NhnLogin() {}

	virtual int Init();
	virtual void SetProvType();

	void SetNHNMemberNumber(__int64 memberNumber);
	__int64 GetNHNMemberNumber() const;
	void SetSiteCookie(const char* siteCookie);
	const char* GetSiteCookie() const;

protected:
	virtual void Convert(void* data);

private:
	void SetNhnUserIdentity(const char* text, _sNhnUserIdentity& identity);

	__int64 m_nhnMemberNumber; // 0xA8
	std::string m_siteCookie; // 0xB0

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(NhnLogin, 0xD0);
BEGIN_LAYOUT_CHECK(NhnLogin)
	CHECK_FIELD(NhnLogin, m_nhnMemberNumber, 0xA8);
	CHECK_FIELD(NhnLogin, m_siteCookie, 0xB0);
END_LAYOUT_CHECK

// Login used when the client was not started by a web launcher.
class PangYaLogin : public BaseLoginInfo {
public:
	// logininfo.h:423
	PangYaLogin() {}
	// logininfo.h:424
	virtual ~PangYaLogin() {}

	// logininfo.h:427
	virtual int Init()
	{
		return 0;
	}

	// logininfo.h:432
	virtual void Convert(void* data)
	{
	}
	// logininfo.h:435
	virtual void SetProvType()
	{
		m_provType = 2;
	}
};
CHECK_SIZE(PangYaLogin, 0xA8);

// logininfo.h:459
class cHttpNtreevAuthResult {
public:
	cHttpNtreevAuthResult(const char* response) : m_response(response) {}
	~cHttpNtreevAuthResult() {}

	bool GetResult();
	std::string GetMessage();
	std::string GetExternalLink();
	std::string GetFieldValue(const char* field);

private:
	std::string GetElement(const char* element);

	std::string m_response; // 0x0
};
CHECK_SIZE(cHttpNtreevAuthResult, 0x1C);

CLoginInfo *LOGININFO(void);         // logininfo.h:475
const char *LOGINPW(void);           // logininfo.h:480
bool IsWebLogin(void);               // logininfo.h:481
int IsPcBang(void);                  // logininfo.h:482

#endif
