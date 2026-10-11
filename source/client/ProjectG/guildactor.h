// Original: d:\build\custom\temp\code\client\projectg\guildactor.h
#ifndef PG_CLIENT_PROJECTG_GUILDACTOR_H
#define PG_CLIENT_PROJECTG_GUILDACTOR_H

#include <utility>

#include "..\..\defines.h"
#include "actor.h"
#include "frcmdtarget.h"

class FrForm;
class MsgObject;
struct GUILD_INFO;
struct GUILD_USER_INFO;

namespace _guild {

class CGuildInfo;
class CGuildMainForm;
class FrCreateGuildKit;
class FrRegisterGuildMark;
class FrChangeGuildName;
class FrGuildJoin;

// The guild actor: opens the guild forms (creation kit, mark registration,
// name change, join request, guild main form) and executes the guild
// messages (0x263). Its size (0x34) is the `new` of CGuildActorMakeInstance:
// an IActor with a Fresh command target at 0x18.
class CGuildActor : public IActor, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // guildactor.h:28

protected:
	CGuildActor(void);
	friend IObject* CGuildActorMakeInstance(void);   // the factory creates it

public:
	virtual ~CGuildActor(void);

	virtual void OnInit(void);
	virtual void HandleMsg(const MsgObject& msg);

	void RequestGuildUserList(int page);

	bool OnCreateGuildFormResult(int ret, FrForm* pForm);
	bool OnRegisterGuildMarkResult(int ret, FrForm* pForm);
	bool OnChangeGuildNameResult(int ret, FrForm* pForm);
	bool OnGuildJoinFormResult(int ret, FrForm* pForm);
	bool OnMainFormCloseResult(int ret, FrForm* pForm);
	bool OnGuildMasterDelegate(int ret, FrForm* pForm);
	bool OnGuildShutDownResult(int ret, FrForm* pForm);
	bool OnGuildOutResult(int ret, FrForm* pForm);

private:
	void InitControl(void);
	const char* SystemMsg(int index);
	void OpenGuileMainForm(void);
	void OpenGuildItemForm(unsigned long itemTypeId);
	void OpenGuildJoinForm(int guildNumber);
	void OpenGuildQuestionForm(const char* text, int type);
	void ReInitMyGuildInfo_SharedDoc(GUILD_USER_INFO* pInfo);
	void ChangeGuildName(GUILD_INFO* pInfo);
	void ChangeGuildEmblem(GUILD_USER_INFO* pInfo);
	std::pair<bool, bool> CheckMyGuildState(GUILD_USER_INFO* pInfo);
	void ExecGuildMessage(const MsgObject& msg);

	FrCreateGuildKit* m_pCreateGuildKit;         // 0x1c
	FrRegisterGuildMark* m_pRegisterGuildMark;   // 0x20
	FrChangeGuildName* m_pChangeGuildName;       // 0x24
	FrGuildJoin* m_pGuildJoin;                   // 0x28
	CGuildMainForm* m_pMainForm;                 // 0x2c
	CGuildInfo* m_pGuildInfo;                    // 0x30
};

CHECK_SIZE(CGuildActor, 0x34);
BEGIN_LAYOUT_CHECK(CGuildActor)
	CHECK_FIELD(CGuildActor, m_pCreateGuildKit, 0x1c);
	CHECK_FIELD(CGuildActor, m_pRegisterGuildMark, 0x20);
	CHECK_FIELD(CGuildActor, m_pChangeGuildName, 0x24);
	CHECK_FIELD(CGuildActor, m_pGuildJoin, 0x28);
	CHECK_FIELD(CGuildActor, m_pMainForm, 0x2c);
	CHECK_FIELD(CGuildActor, m_pGuildInfo, 0x30);
END_LAYOUT_CHECK

} // namespace _guild

#endif // PG_CLIENT_PROJECTG_GUILDACTOR_H
