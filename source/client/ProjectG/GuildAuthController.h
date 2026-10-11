// Original: d:\build\custom\temp\code\client\projectg\guildauthcontroller.h
#ifndef PG_CLIENT_PROJECTG_GUILDAUTHCONTROLLER_H
#define PG_CLIENT_PROJECTG_GUILDAUTHCONTROLLER_H

#include "..\..\defines.h"

struct FrListItem;

namespace _guild {

class CGuildInfo;
class CGuildMainForm;

// What the guild main form lets the local player do, by guild rank: one
// controller per rank overrides the commands that rank may use. The form
// and the guild data are shared by every controller (SetLinkDepends).
class IGuildAuthController {
protected:
	IGuildAuthController(void);

public:
	virtual ~IGuildAuthController(void);

	virtual void DrawGuildTalk(FrListItem* pItem) {}                  // guildauthcontroller.h:23

	virtual void PopupCommandMenu(float x, float y);

	virtual void ProcManagerCommand(const char* command) {}           // guildauthcontroller.h:29

	virtual void OnGuildIntroConfirm_LButtonUp(void) {}               // guildauthcontroller.h:32

	virtual void OnGuildNoticeConfirm_LButtonUp(void) {}              // guildauthcontroller.h:35

	virtual void OnGuildJoin_LButtonUp(void);

	virtual void OnGuildJoinAbort_LButtonUp(void) {}                  // guildauthcontroller.h:41

	virtual void OnGuildShutDown_LButtonUp(void) {}                   // guildauthcontroller.h:44

	virtual void OnGuildOut_LButtonUp(void) {}                        // guildauthcontroller.h:47

	static void SetLinkDepends(CGuildMainForm* pForm, CGuildInfo* pInfo);

	static CGuildMainForm* m_linkForm;
	static CGuildInfo* m_linkInfo;
};

CHECK_SIZE(IGuildAuthController, 0x4);

// The guild master and sub-master: the manager commands (member class
// changes, the guild introduction and notice).
class CGuildManager : public IGuildAuthController {
public:
	virtual void DrawGuildTalk(FrListItem* pItem);
	virtual void ProcManagerCommand(const char* command);
	virtual void OnGuildIntroConfirm_LButtonUp(void);
	virtual void OnGuildNoticeConfirm_LButtonUp(void);
};

CHECK_SIZE(CGuildManager, 0x4);

class CGuildMaster : public CGuildManager {
public:
	virtual void PopupCommandMenu(float x, float y);
	virtual void OnGuildShutDown_LButtonUp(void);
	virtual void OnGuildOut_LButtonUp(void);
};

CHECK_SIZE(CGuildMaster, 0x4);

class CGuildSubMaster : public CGuildManager {
public:
	virtual void PopupCommandMenu(float x, float y);
	virtual void OnGuildOut_LButtonUp(void);
};

CHECK_SIZE(CGuildSubMaster, 0x4);

class CGuildMember : public IGuildAuthController {
public:
	virtual void DrawGuildTalk(FrListItem* pItem);
	virtual void PopupCommandMenu(float x, float y);
	virtual void ProcManagerCommand(const char* command);
	virtual void OnGuildOut_LButtonUp(void);
};

CHECK_SIZE(CGuildMember, 0x4);

// A player whose join request is still waiting for the guild's answer.
class CGuildWaitMember : public IGuildAuthController {
public:
	virtual void OnGuildJoinAbort_LButtonUp(void);
	virtual void OnGuildOut_LButtonUp(void);
};

CHECK_SIZE(CGuildWaitMember, 0x4);

// A player in no guild.
class CGuildNull : public IGuildAuthController {
public:
	virtual void OnGuildJoin_LButtonUp(void);
};

CHECK_SIZE(CGuildNull, 0x4);

} // namespace _guild

#endif // PG_CLIENT_PROJECTG_GUILDAUTHCONTROLLER_H
