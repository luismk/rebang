// Original: d:\build\custom\temp\code\client\projectg\ghostplayer.h
#ifndef PG_CLIENT_PROJECTG_GHOSTPLAYER_H
#define PG_CLIENT_PROJECTG_GHOSTPLAYER_H

#include <list>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "frcmdtarget.h"

class CGhostDocument;
class CGhostHoleData;
class CSequenceStateMachine;
class CStateUnit;
class FrForm;
class FrGhostAward;
class IObject;
class MsgObject;
class WOverlay;
class WRTTI;
struct GHOST_ACTION;
struct GHOST_SHOT_DATA;

// The actor replaying a ghost (a recorded round): an IActor with a Fresh
// command target at 0x18 (size 0x54, the `new` of CGhostPlayerMakeInstance).
class CGhostPlayer : public IActor, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;
public:
	// The black bars of the replay's letterbox.
	class cMovieStyleDisplay {
	public:
		cMovieStyleDisplay(void);
		~cMovieStyleDisplay(void);

		void ResetHeight(void);
		void Process(float fDelta);
		void Render(void);

		float m_height;       // 0x0
		WOverlay* m_pOverlay; // 0x4
	};

	static const WRTTI m_RTTI;
	// ghostplayer.h:13
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	CGhostPlayer(void);
	virtual ~CGhostPlayer(void);

	virtual void OnLoad(void);
	virtual void OnInit(void);
	virtual void OnReload(void);
	virtual void OnProcess(float fDelta);
	virtual void OnDisplay(void);
	virtual void OnDestroy(void);

protected:
	virtual void HandleMsg(const MsgObject &msg);   // ghostplayer.cpp:1002

private:
	CStateUnit* CreateStateUnit(int type, GHOST_ACTION const* pAction);
	bool DoShot(GHOST_SHOT_DATA const* pShot);
	void SetWindAndWeather(unsigned char holeNum);
	void UseItem(unsigned long itemTypeId, unsigned long itemId);
	void SendAwardResult(void);
	bool ExceptionAlarmWaitCallBack(int result, FrForm* pForm);

	CGhostDocument* m_pGhostDoc;                 // 0x1c
	CGhostHoleData const* m_pHoleData;           // 0x20
	std::list<GHOST_SHOT_DATA*> m_shots;         // 0x24
	CSequenceStateMachine* m_pStateMachine;      // 0x30
	FrGhostAward* m_pAward;                      // 0x34
	cMovieStyleDisplay* m_pMovieStyle;           // 0x38
	int m_count3c;                               // 0x3c (accumulated in OnProcess)
	unsigned int m_count40;                      // 0x40 (shots played, accumulated in OnProcess)
	int m_phase;                                 // 0x44 (0..4)
	int m_alarmState;                            // 0x48
	int m_awardSent;                             // 0x4c (SendAwardResult called)
	unsigned long m_serverProperty;              // 0x50 (restored by OnDestroy)
};
IObject* CGhostPlayerMakeInstance(void);

CHECK_SIZE(CGhostPlayer::cMovieStyleDisplay, 0x8);
CHECK_FIELD(CGhostPlayer::cMovieStyleDisplay, m_height, 0x0);
CHECK_FIELD(CGhostPlayer::cMovieStyleDisplay, m_pOverlay, 0x4);
CHECK_SIZE(CGhostPlayer, 0x54);
BEGIN_LAYOUT_CHECK(CGhostPlayer)
	CHECK_FIELD(CGhostPlayer, m_pGhostDoc, 0x1c);
	CHECK_FIELD(CGhostPlayer, m_pHoleData, 0x20);
	CHECK_FIELD(CGhostPlayer, m_shots, 0x24);
	CHECK_FIELD(CGhostPlayer, m_pStateMachine, 0x30);
	CHECK_FIELD(CGhostPlayer, m_pAward, 0x34);
	CHECK_FIELD(CGhostPlayer, m_pMovieStyle, 0x38);
	CHECK_FIELD(CGhostPlayer, m_count3c, 0x3c);
	CHECK_FIELD(CGhostPlayer, m_count40, 0x40);
	CHECK_FIELD(CGhostPlayer, m_phase, 0x44);
	CHECK_FIELD(CGhostPlayer, m_alarmState, 0x48);
	CHECK_FIELD(CGhostPlayer, m_awardSent, 0x4c);
	CHECK_FIELD(CGhostPlayer, m_serverProperty, 0x50);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GHOSTPLAYER_H
