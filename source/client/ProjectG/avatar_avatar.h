// Original: d:\build\custom\temp\code\client\projectg\avatar_avatar.h
#ifndef PG_CLIENT_PROJECTG_AVATAR_AVATAR_H
#define PG_CLIENT_PROJECTG_AVATAR_AVATAR_H

#include <list>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wlist.h"
#include "wmath.h"
#include "singleton.h"
#include "s5/specialprize/specialprizeitem.h"
#include "../shared/globalgamedefine.h"
#include "parttidlist.h"
#include "wboneset.h"
#include "fx.h"
#include "wscene.h"

class Bitmap;
class CPetFrame;
class CQuadTree;
class ntAvatarChatMain;
class ntBalloonText;
struct sSlotInfo;
struct sTexturePart;
struct w_motion_data;

enum eSPAVILITYTYPE {
};

class ntAvatarBurden {
public:
	ntAvatarBurden() : m_charInfo(), m_pPetFrame(NULL) {}       // avatar_avatar.h:48
	ntAvatarBurden(unsigned long first, unsigned long key, const sCharacterInfo &charInfo, CPetFrame *pPetFrame);
	~ntAvatarBurden();
	bool Load();
	bool operator<(const ntAvatarBurden &);
	bool operator==(const ntAvatarBurden &);

	unsigned long m_first;      // 0x0, the value ctor's first argument
	unsigned long m_key;        // 0x4, the value ctor's second argument; the sort key
	sCharacterInfo m_charInfo;  // 0x8
	CPartTidList m_partTidList; // 0x1C4
	CPetFrame *m_pPetFrame;     // 0x28C
};
CHECK_SIZE(ntAvatarBurden, 0x290);
CHECK_FIELD(ntAvatarBurden, m_first, 0x0);
CHECK_FIELD(ntAvatarBurden, m_key, 0x4);
CHECK_FIELD(ntAvatarBurden, m_charInfo, 0x8);
CHECK_FIELD(ntAvatarBurden, m_partTidList, 0x1C4);
CHECK_FIELD(ntAvatarBurden, m_pPetFrame, 0x28C);

class ntAvatar {
	LAYOUT_CHECK_FRIEND;
	friend class ntAvatarManager;
	friend class ntAvatarChatMain;  // its OnPreserveBack sets m_state bits

public:
	~ntAvatar();
	const sCharacterInfo &GetCharacterInfo(void) const { return m_characterInfo; } // avatar_avatar.h:110
	unsigned long GetGuid() const;                              // avatar_avatar.h:118
	const WVector &GetPosition() const;                         // avatar_avatar.h:128
	void SetHeadIcon(int icon);                                 // avatar_avatar.h:151
	bool IsTargetPositionChanged(bool changed);
	void SetCharactorInfo(const sCharacterInfo &charInfo);
	bool GetBoneScreenCoord(const char *boneName, int *pX, int *pY) const;
	void RefreshUccClothes(unsigned long tid, unsigned long id);
	void UseSPItem(eSPAVILITYTYPE type);
	void Follow(const char *nickName);
	ntAvatar *GetFollowingAvatar() const;
	void SetPosition(const WVector &position);
	void SetTargetPosition(const WVector &position);
	void SetAngle(float angle);
	void SetTargetAngle(float angle);
	void SetPendingTargetAngle(float angle);
	void SetState(unsigned long state);
	void SetAction(int action);
	void Stop();
	void SetBalloonText(const char *text);
	void SetChatFacial(int facial);
	void SetTradeTitle(const char *title);
	SpecialPrizeItem &GetMySpItem(eSPAVILITYTYPE type);
	void ChangeMotion(int motion, const char *name, float blendTime);
	bool IsClickable(float x, float y) const;
	void Render();
	void RenderOverlay(bool detail);
	void Process(float elapsed);

protected:
	ntAvatar(CQuadTree &pvs, const sSlotInfo &slotInfo, const sCharacterInfo &charInfo, ntAvatarChatMain *pChatMain);
	CQuadTree &GetPVS();
	void SetLocalPVS(CQuadTree &pvs);

private:
	void ProcessChangeState();
	void SendMove(const WVector &position, bool force);
	void ScriptProcess(const w_motion_data *data, float from, float to, bool all);
	void ParsePuppetScript(const char *script);
	void ProcessMotion(float elapsed);
	void ParseScript_ShowVis(bool show, const char *script);
	void SetPendingTexPart(const char *part, const char *texture);
	sTexturePart *FindTexturePart(const char *part, const char *texture);

	CQuadTree *m_pPVS;                  // 0x0
	char m_idName[0x16];                // 0x4
	char m_nickName[0x16];              // 0x1A
	bool m_moveFlag;                    // 0x30, cleared by Stop
	UNKNOWN(0x31, 0x46);
	char m_followName[0x16];            // 0x46, nickname of the avatar followed
	bool m_hasSlotBlockD1;              // 0x5C, sSlotInfo::m_valueCD == 1 (local content 0x10)
	char m_slotBlockD1[0x80];           // 0x5D, sSlotInfo::m_blockD1
	UNKNOWN(0xDD, 0xE0);
	unsigned long m_uid;                // 0xE0, sent by ntAvatarChatMain::OnKickAvatar
	int m_guid;                         // 0xE4
	bool m_canKick;                     // 0xE8, tested on the own avatar by OnKickAvatar
	unsigned char m_userFlag;           // 0xE9, sSlotInfo::m_attributeBits.userFlag
	bool m_slotFlag55;                  // 0xEA, sSlotInfo::m_flags55.flag0
	bool m_slotFlag57;                  // 0xEB, sSlotInfo::m_flags57.flag0
	unsigned long m_charTypeId;         // 0xEC, the character's type id
	unsigned long m_state;              // 0xF0
	unsigned long m_prevState;          // 0xF4
	unsigned long m_action;             // 0xF8

public:
	WVector m_position;                 // 0xFC, read by the file-local depth sort of ntAvatarManager::RenderOverlay

private:
	float m_angle;                      // 0x108
	float m_targetAngle;                // 0x10C
	bool m_hasPendingTargetAngle;       // 0x110
	float m_pendingTargetAngle;         // 0x114
	WVector m_groundNormal;             // 0x118, WVector::UNIT_POS_Y at construction
	WVector m_targetPosition;           // 0x124
	float kMoveEpsilon;
	bool m_targetPositionChanged;       // 0x130
	int m_headIcon;                     // 0x134
	WVector m_pendingMove;              // 0x138, movement accumulated by SendMove until sent
	ntAvatarBurden m_burden;            // 0x144
	UNKNOWN(0x3D4, 0x3D8);
	bool m_flag3D8;                     // 0x3D8, cleared by the constructor
	sCharacterInfo m_characterInfo;     // 0x3D9, copied by SetCharactorInfo (byte-packed)
	UNKNOWN(0x595, 0x598);
	CPartTidList m_partTidList;         // 0x598, the parts worn, passed to CPetFrame::SetPart
	CPetFrame *m_pPetFrame;             // 0x660
	std::list<sTexturePart *> m_pendingTexParts; // 0x664, searched by FindTexturePart
	float m_time;                       // 0x670, compared with 0.1 by IsClickable, passed to DrawDecal by Render
	LightSet m_light;                   // 0x674, the light Render passes to CPetFrame::UpdateLightSource
	w_motion_data *m_pMotion;           // 0x690, the motion played, found by ChangeMotion
	int m_motionType;                   // 0x694, ChangeMotion's motion (4: the named motion)
	WBoneSet m_boneSet;                 // 0x698, the current pose
	w_motion_data *m_pPrevMotion;       // 0x6C0, the motion blended from
	int m_prevMotionType;               // 0x6C4
	WBoneSet m_prevBoneSet;             // 0x6C8, ChangeMotion copies m_boneSet into it to blend from
	float m_blendTime;                  // 0x6F0, ChangeMotion's blend duration
	float m_blendElapsed;               // 0x6F4, zeroed by ChangeMotion
	float m_value6F8;                   // 0x6F8, zeroed by the constructor
	float m_facialTime;                 // 0x6FC
	unsigned long m_motionSendTime;     // 0x700, timeGetTime of the last named motion sent
	ntBalloonText *m_pBalloonText;      // 0x704
	float m_balloonOffset[3];           // 0x708
	float m_balloonTime;                // 0x714, m_time when the text was set
	unsigned long m_balloonElapsed;     // 0x718

	char m_tradeTitle[0x40];            // 0x71C, size not established
	const Bitmap *m_pEmblem;            // 0x75C, the guild emblem (NetResourceManager::GetEmblemByName)
	ntAvatarChatMain *m_pChatMain;      // 0x760, the constructor's pChatMain
	CPvsFxBox m_fxBox;                  // 0x764, cleared by SetCharactorInfo and the destructor
	SpecialPrizeItem m_spItems[5];      // 0xAC8, indexed by eSPAVILITYTYPE
};

BEGIN_LAYOUT_CHECK(ntAvatar)
	CHECK_FIELD(ntAvatar, m_pPVS, 0x0);
	CHECK_FIELD(ntAvatar, m_idName, 0x4);
	CHECK_FIELD(ntAvatar, m_nickName, 0x1A);
	CHECK_FIELD(ntAvatar, m_moveFlag, 0x30);
	CHECK_FIELD(ntAvatar, m_followName, 0x46);
	CHECK_FIELD(ntAvatar, m_hasSlotBlockD1, 0x5C);
	CHECK_FIELD(ntAvatar, m_slotBlockD1, 0x5D);
	CHECK_FIELD(ntAvatar, m_userFlag, 0xE9);
	CHECK_FIELD(ntAvatar, m_slotFlag55, 0xEA);
	CHECK_FIELD(ntAvatar, m_slotFlag57, 0xEB);
	CHECK_FIELD(ntAvatar, m_groundNormal, 0x118);
	CHECK_FIELD(ntAvatar, m_burden, 0x144);
	CHECK_FIELD(ntAvatar, m_flag3D8, 0x3D8);
	CHECK_FIELD(ntAvatar, m_value6F8, 0x6F8);
	CHECK_FIELD(ntAvatar, m_pEmblem, 0x75C);
	CHECK_FIELD(ntAvatar, m_uid, 0xE0);
	CHECK_FIELD(ntAvatar, m_guid, 0xE4);
	CHECK_FIELD(ntAvatar, m_pendingTexParts, 0x664);
	CHECK_FIELD(ntAvatar, m_canKick, 0xE8);
	CHECK_FIELD(ntAvatar, m_charTypeId, 0xEC);
	CHECK_FIELD(ntAvatar, m_state, 0xF0);
	CHECK_FIELD(ntAvatar, m_prevState, 0xF4);
	CHECK_FIELD(ntAvatar, m_action, 0xF8);
	CHECK_FIELD(ntAvatar, m_position, 0xFC);
	CHECK_FIELD(ntAvatar, m_angle, 0x108);
	CHECK_FIELD(ntAvatar, m_targetAngle, 0x10C);
	CHECK_FIELD(ntAvatar, m_hasPendingTargetAngle, 0x110);
	CHECK_FIELD(ntAvatar, m_pendingTargetAngle, 0x114);
	CHECK_FIELD(ntAvatar, m_targetPosition, 0x124);
	CHECK_FIELD(ntAvatar, m_targetPositionChanged, 0x130);
	CHECK_FIELD(ntAvatar, m_headIcon, 0x134);
	CHECK_FIELD(ntAvatar, m_pendingMove, 0x138);
	CHECK_FIELD(ntAvatar, m_characterInfo, 0x3D9);
	CHECK_FIELD(ntAvatar, m_partTidList, 0x598);
	CHECK_FIELD(ntAvatar, m_pPetFrame, 0x660);
	CHECK_FIELD(ntAvatar, m_time, 0x670);
	CHECK_FIELD(ntAvatar, m_light, 0x674);
	CHECK_FIELD(ntAvatar, m_pMotion, 0x690);
	CHECK_FIELD(ntAvatar, m_motionType, 0x694);
	CHECK_FIELD(ntAvatar, m_boneSet, 0x698);
	CHECK_FIELD(ntAvatar, m_pPrevMotion, 0x6C0);
	CHECK_FIELD(ntAvatar, m_prevMotionType, 0x6C4);
	CHECK_FIELD(ntAvatar, m_prevBoneSet, 0x6C8);
	CHECK_FIELD(ntAvatar, m_blendTime, 0x6F0);
	CHECK_FIELD(ntAvatar, m_blendElapsed, 0x6F4);
	CHECK_FIELD(ntAvatar, m_facialTime, 0x6FC);
	CHECK_FIELD(ntAvatar, m_motionSendTime, 0x700);
	CHECK_FIELD(ntAvatar, m_pBalloonText, 0x704);
	CHECK_FIELD(ntAvatar, m_balloonOffset, 0x708);
	CHECK_FIELD(ntAvatar, m_balloonTime, 0x714);
	CHECK_FIELD(ntAvatar, m_balloonElapsed, 0x718);
	CHECK_FIELD(ntAvatar, m_tradeTitle, 0x71C);
	CHECK_FIELD(ntAvatar, m_pChatMain, 0x760);
	CHECK_FIELD(ntAvatar, m_fxBox, 0x764);
	CHECK_FIELD(ntAvatar, m_spItems, 0xAC8);
END_LAYOUT_CHECK

// Only the members this project's code uses so far are declared.
class ntAvatarManager : public WSingleton<ntAvatarManager> {
	LAYOUT_CHECK_FRIEND;
	friend class ntAvatar;
	friend class ntAvatarChatMain;  // its OnUnderBar_*Up stop m_pMyAvatar

public:
	ntAvatarManager();
	~ntAvatarManager();
	void SetActive(bool active);   // avatar_avatar.h:280
	ntAvatar *FindAvatarByIDName(const char *idName) const;
	ntAvatar *FindAvatarByNickName(const char *nickName) const;
	ntAvatar *FindAvatarByGuid(int guid) const;
	ntAvatar *CheckClick(float x, float y) const;
	void Render();
	void RenderOverlay();
	void Process(float elapsed);
	void ResetAvatarPositionAngle();
	ntAvatar *CreateAvatar(const sSlotInfo &slotInfo, const sCharacterInfo &charInfo, ntAvatarChatMain *pChatMain);
	bool DeleteAvatar(const char *idName);
	ntAvatar *AvatarByRayIntersect(const WVector &pos, const WVector &dir) const;
	void FreeAllAvatars();

protected:
	CQuadTree &GetPVS();
	void SetLocalPVS(CQuadTree &pvs);

private:
	int m_avatarCount;             // 0x28
	ntAvatar *m_avatars[30];       // 0x2C
	ntAvatar *m_pMyAvatar;         // 0xA4
	ntAvatar *m_pLitAvatar;        // 0xA8, the avatar Render lights in full white
	ntAvatar *m_pHighlightAvatar;  // 0xAC, the avatar whose name RenderOverlay draws highlighted
	bool m_active;                 // 0xB0
	bool m_overlayDetail;          // 0xB1, passed to ntAvatar::RenderOverlay
	CQuadTree *m_pPVS;             // 0xB4
};
BEGIN_LAYOUT_CHECK(ntAvatarManager)
	CHECK_FIELD(ntAvatarManager, m_avatarCount, 0x28);
	CHECK_FIELD(ntAvatarManager, m_avatars, 0x2C);
	CHECK_FIELD(ntAvatarManager, m_pMyAvatar, 0xA4);
	CHECK_FIELD(ntAvatarManager, m_pLitAvatar, 0xA8);
	CHECK_FIELD(ntAvatarManager, m_pHighlightAvatar, 0xAC);
	CHECK_FIELD(ntAvatarManager, m_active, 0xB0);
	CHECK_FIELD(ntAvatarManager, m_overlayDetail, 0xB1);
	CHECK_FIELD(ntAvatarManager, m_pPVS, 0xB4);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_AVATAR_AVATAR_H
