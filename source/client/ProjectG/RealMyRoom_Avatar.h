// Original: d:\build\custom\temp\code\client\projectg\realmyroom_avatar.h
#ifndef PG_CLIENT_PROJECTG_REALMYROOM_AVATAR_H
#define PG_CLIENT_PROJECTG_REALMYROOM_AVATAR_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wmath.h"
#include "singleton.h"
#include <list>

#include "FxBox.h"
#include "fx.h"
#include "wboneset.h"
#include "wscene.h"
#include "parttidlist.h"
#include "../shared/globalgamedefine.h"

class CPetFrame;
class CRealMyRoomMain;
struct sSlotInfo;
struct sTexturePart;
struct w_motion_data;

class CRealMyRoom_AvatarBurden {
public:
	CRealMyRoom_AvatarBurden();
	CRealMyRoom_AvatarBurden(unsigned long, const struct sCharacterInfo &, CPetFrame *);
	~CRealMyRoom_AvatarBurden();
	bool LoadAvatarBurden();

	unsigned long m_guid;         // 0x0
	sCharacterInfo m_charInfo;    // 0x4
	CPartTidList m_partTidList;   // 0x1C0
	CPetFrame *m_pPetFrame;       // 0x288
};
CHECK_SIZE(CRealMyRoom_AvatarBurden, 0x28C);
CHECK_FIELD(CRealMyRoom_AvatarBurden, m_guid, 0x0);
CHECK_FIELD(CRealMyRoom_AvatarBurden, m_charInfo, 0x4);
CHECK_FIELD(CRealMyRoom_AvatarBurden, m_partTidList, 0x1C0);
CHECK_FIELD(CRealMyRoom_AvatarBurden, m_pPetFrame, 0x288);

class CRealMyRoom_Avatar {
public:
	CRealMyRoom_Avatar(const sSlotInfo &slotInfo, const sCharacterInfo &charInfo, CRealMyRoomMain *pMain);
	~CRealMyRoom_Avatar();

	CPetFrame *GetPetFrame(); // realmyroom_avatar.h:80
	const char *GetIDName() const; // realmyroom_avatar.h:84
	const char *GetNickName() const; // realmyroom_avatar.h:85
	const WVector &GetPosition() const; // realmyroom_avatar.h:93
	const WVector &GetTargetPosition() const; // realmyroom_avatar.h:94
	void SetState(unsigned long state);
	void SubState(unsigned long state); // realmyroom_avatar.h:120
	void SetHeadIcon(int headIcon); // realmyroom_avatar.h:125

	bool IsTargetPositionChanged(bool changed);
	CRealMyRoom_Avatar *GetFollowingAvatar() const;
	void ChangeCharacterInfo();
	void SetPosition(const WVector &position);
	void SetStartPosition(const WVector &position);
	void SetTargetPosition(const WVector &position);
	void SetAngle(float angle);
	void SetTargetAngle(float angle);
	void SetPendingTargetAngle(float angle);
	void Follow(const char *nickName);
	void SetHide(bool hide);
	bool GetBoneScreenCoord(const char *boneName, int *pX, int *pY) const;
	bool IsClickable(float x, float y) const;
	void RenderOverlay(bool detail);
	void RefreshUccClothes(unsigned long tid, unsigned long id);
	void SetChatFacial(int facial);
	void SetBalloonText(const char *text);
	void SetTradeTitle(const char *title);
	void SetCharactorInfo(const sCharacterInfo &charInfo);
	void Stop();
	void SetAction(int action);
	void ChangeMotion(int motion, const char *name, float blendTime);
	void Render();
	void Process(float elapsed);

private:
	void ScriptProcess(const struct w_motion_data *data, float from, float to, bool all);
	void ParsePuppetScript(const char *script);
	void ProcessChangeState();
	void ProcessMotion(float elapsed);
	void ParseBoundingBoxScript();
	void ParseScript_ShowVis(bool show, const char *script);
	sTexturePart *FindTexturePart(const char *part, const char *texture);
	void SetPendingTexPart(const char *part, const char *texture);
	void SendMove(const WVector &position, bool force);

public:
	char m_idName[0x16];              // 0x0
	char m_nickName[0x16];            // 0x16 (length assumed)
	bool m_moveFlag;                  // 0x2C, cleared by Stop
	PADDING(0x2D, 0x42);
	char m_followNickName[0x16];      // 0x42, the avatar this one follows (length assumed)
	PADDING(0x58, 0xDC);
	int m_guid;                       // 0xDC
	bool m_isAdministrator;           // 0xE0, identity bit 2 (administrator) copied by the constructor
	PADDING(0xE1, 0xE4);
	unsigned long m_charTypeId;       // 0xE4, the character's type id
	unsigned long m_state;            // 0xE8
	unsigned long m_prevState;        // 0xEC
	unsigned long m_action;           // 0xF0
	WVector m_position;               // 0xF4
	WVector m_startPosition;          // 0x100
	float m_angle;                    // 0x10C
	float m_targetAngle;              // 0x110
	bool m_hasPendingTargetAngle;     // 0x114
	PADDING(0x115, 0x118);
	float m_pendingTargetAngle;       // 0x118
	PADDING(0x11C, 0x128);
	WVector m_targetPosition;         // 0x128
	bool m_isTargetPositionChanged;   // 0x134
	PADDING(0x135, 0x138);
	int m_headIcon;                   // 0x138
	WVector m_moveDelta;              // 0x13C, the move not sent yet
	PADDING(0x148, 0x3D9);
	sCharacterInfo m_charInfo;        // 0x3D9
	PADDING(0x595, 0x598);
	CPartTidList m_partTidList;       // 0x598

	CPetFrame *m_pPetFrame;           // 0x660
	std::list<sTexturePart *> m_pendingTexParts; // 0x664, freed by the destructor
	float m_time;                     // 0x670, the avatar's clock (IsClickable waits for 0.1)
	LightSet m_light;                 // 0x674, the light Render passes to CPetFrame::UpdateLightSource
	bool m_hide;                      // 0x690, set by SetHide
	PADDING(0x691, 0x694);
	w_motion_data *m_pMotionData;     // 0x694, the motion playing (ChangeMotion)
	int m_motion;                     // 0x698, its ChangeMotion kind
	WBoneSet m_boneSet0;              // 0x69C (role unknown)
	w_motion_data *m_pPrevMotionData; // 0x6C4, the motion blended out of
	int m_prevMotion;                 // 0x6C8
	WBoneSet m_boneSet1;              // 0x6CC (role unknown)
	float m_blendDuration;            // 0x6F4, the blend length ChangeMotion was given
	float m_blendTime;                // 0x6F8, the time blended so far
	PADDING(0x6FC, 0x700);
	float m_facialTime;               // 0x700
	unsigned long m_lastMotionSendTime; // 0x704, timeGetTime of the last motion packet
	PADDING(0x708, 0x718);
	float m_balloonTime;              // 0x718, m_time when the text was set
	unsigned long m_balloonElapsed;   // 0x71C
	char m_tradeTitle[0x40];          // 0x720 (length assumed)
	PADDING(0x760, 0x764);
	CRealMyRoomMain *m_pMain;         // 0x764
	CPvsFxBox m_fxBox;                // 0x768
};
CHECK_SIZE(CRealMyRoom_Avatar, 0xACC);
CHECK_FIELD(CRealMyRoom_Avatar, m_idName, 0x0);
CHECK_FIELD(CRealMyRoom_Avatar, m_nickName, 0x16);
CHECK_FIELD(CRealMyRoom_Avatar, m_moveFlag, 0x2C);
CHECK_FIELD(CRealMyRoom_Avatar, m_followNickName, 0x42);
CHECK_FIELD(CRealMyRoom_Avatar, m_guid, 0xDC);
CHECK_FIELD(CRealMyRoom_Avatar, m_charTypeId, 0xE4);
CHECK_FIELD(CRealMyRoom_Avatar, m_charInfo, 0x3D9);
CHECK_FIELD(CRealMyRoom_Avatar, m_tradeTitle, 0x720);
CHECK_FIELD(CRealMyRoom_Avatar, m_state, 0xE8);
CHECK_FIELD(CRealMyRoom_Avatar, m_prevState, 0xEC);
CHECK_FIELD(CRealMyRoom_Avatar, m_action, 0xF0);
CHECK_FIELD(CRealMyRoom_Avatar, m_moveDelta, 0x13C);
CHECK_FIELD(CRealMyRoom_Avatar, m_position, 0xF4);
CHECK_FIELD(CRealMyRoom_Avatar, m_startPosition, 0x100);
CHECK_FIELD(CRealMyRoom_Avatar, m_angle, 0x10C);
CHECK_FIELD(CRealMyRoom_Avatar, m_targetAngle, 0x110);
CHECK_FIELD(CRealMyRoom_Avatar, m_hasPendingTargetAngle, 0x114);
CHECK_FIELD(CRealMyRoom_Avatar, m_pendingTargetAngle, 0x118);
CHECK_FIELD(CRealMyRoom_Avatar, m_targetPosition, 0x128);
CHECK_FIELD(CRealMyRoom_Avatar, m_isTargetPositionChanged, 0x134);
CHECK_FIELD(CRealMyRoom_Avatar, m_headIcon, 0x138);
CHECK_FIELD(CRealMyRoom_Avatar, m_partTidList, 0x598);
CHECK_FIELD(CRealMyRoom_Avatar, m_pPetFrame, 0x660);
CHECK_FIELD(CRealMyRoom_Avatar, m_pendingTexParts, 0x664);
CHECK_FIELD(CRealMyRoom_Avatar, m_time, 0x670);
CHECK_FIELD(CRealMyRoom_Avatar, m_boneSet0, 0x69C);
CHECK_FIELD(CRealMyRoom_Avatar, m_boneSet1, 0x6CC);
CHECK_FIELD(CRealMyRoom_Avatar, m_facialTime, 0x700);
CHECK_FIELD(CRealMyRoom_Avatar, m_balloonTime, 0x718);
CHECK_FIELD(CRealMyRoom_Avatar, m_balloonElapsed, 0x71C);
CHECK_FIELD(CRealMyRoom_Avatar, m_hide, 0x690);
CHECK_FIELD(CRealMyRoom_Avatar, m_isAdministrator, 0xE0);
CHECK_FIELD(CRealMyRoom_Avatar, m_light, 0x674);
CHECK_FIELD(CRealMyRoom_Avatar, m_pMotionData, 0x694);
CHECK_FIELD(CRealMyRoom_Avatar, m_pPrevMotionData, 0x6C4);
CHECK_FIELD(CRealMyRoom_Avatar, m_blendDuration, 0x6F4);
CHECK_FIELD(CRealMyRoom_Avatar, m_lastMotionSendTime, 0x704);
CHECK_FIELD(CRealMyRoom_Avatar, m_pMain, 0x764);
CHECK_FIELD(CRealMyRoom_Avatar, m_fxBox, 0x768);

class CRealMyRoom_AvatarManager : public WSingleton<CRealMyRoom_AvatarManager> {
public:
	CRealMyRoom_AvatarManager();
	virtual ~CRealMyRoom_AvatarManager();

	void FreeAllAvatars();
	CRealMyRoom_Avatar *CreateAvatar(const sSlotInfo &slotInfo, const sCharacterInfo &charInfo, CRealMyRoomMain *pMain);
	CRealMyRoom_Avatar *FindAvatarByIDName(const char *idName) const;
	CRealMyRoom_Avatar *FindAvatarByNickName(const char *nickName) const;
	CRealMyRoom_Avatar *FindAvatarByGuid(int guid) const;
	void Process(float elapsed);
	bool DeleteAvatar(const char *idName);
	CRealMyRoom_Avatar *AvatarByRayIntersect(const WVector &pos, const WVector &dir) const;
	CRealMyRoom_Avatar *CheckClick(float x, float y) const;
	void ResetAvatarPositionAngle();
	void Render();
	void RenderOverlay();

	int m_avatarCount;                    // 0x28
	CRealMyRoom_Avatar *m_pAvatars[30];   // 0x2C
	CRealMyRoom_Avatar *m_pMyAvatar;      // 0xA4, the local player's avatar
	CRealMyRoom_Avatar *m_pLitAvatar;     // 0xA8, the avatar Render lights in full white
	unsigned long m_unknownAC;            // 0xAC, only zeroed by the constructor so far
	bool m_isRenderEnabled;               // 0xB0, Render draws nothing while false
	bool m_overlayDetail;                 // 0xB1, passed to CRealMyRoom_Avatar::RenderOverlay
	// The size is not recovered yet.
};
CHECK_FIELD(CRealMyRoom_AvatarManager, m_avatarCount, 0x28);
CHECK_FIELD(CRealMyRoom_AvatarManager, m_pAvatars, 0x2C);
CHECK_FIELD(CRealMyRoom_AvatarManager, m_pMyAvatar, 0xA4);
CHECK_FIELD(CRealMyRoom_AvatarManager, m_pLitAvatar, 0xA8);
CHECK_FIELD(CRealMyRoom_AvatarManager, m_isRenderEnabled, 0xB0);
CHECK_FIELD(CRealMyRoom_AvatarManager, m_overlayDetail, 0xB1);

#endif // PG_CLIENT_PROJECTG_REALMYROOM_AVATAR_H
