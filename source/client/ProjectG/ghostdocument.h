// Original: d:\build\custom\temp\code\client\projectg\ghostdocument.h
#ifndef PG_CLIENT_PROJECTG_GHOSTDOCUMENT_H
#define PG_CLIENT_PROJECTG_GHOSTDOCUMENT_H

#include "..\..\defines.h"
#include "..\..\padding.h"

#include <list>
#include <string>
#include <vector>

#include "ghosttypes.h"

struct GHOST_USER_DATA;
struct _iobuf;
struct sShotProtocol;
class MsgObject;

// The recorded game settings of a ghost. Only the members read so far are
// transcribed.
struct GHOST_GAME_DATA {
	unsigned char m_holeOrder[18];  // 0x00 (CSharedDoc::m_holeOrder, RecordGameData)
	PADDING(0x12, 0x5C);            // 0x14: unsigned long[18] (CSharedDoc dwords 0x4608, RecordGameData)
	unsigned char m_curMap;         // 0x5C (GetCurMap)
	unsigned char m_gameKind;       // 0x5D (CSharedDoc::m_gameKind)
	unsigned char m_roomType;       // 0x5E (CSharedDoc::m_roomType)
	PADDING(0x5F, 0x60);
	unsigned long m_serverProperty; // 0x60 (eGameServerProperty bits; 0x80 restored by CGhostPlayer::OnLoad)
};
CHECK_SIZE(GHOST_GAME_DATA, 0x64);
CHECK_FIELD(GHOST_GAME_DATA, m_holeOrder, 0x00);
CHECK_FIELD(GHOST_GAME_DATA, m_curMap, 0x5C);
CHECK_FIELD(GHOST_GAME_DATA, m_gameKind, 0x5D);
CHECK_FIELD(GHOST_GAME_DATA, m_roomType, 0x5E);
CHECK_FIELD(GHOST_GAME_DATA, m_serverProperty, 0x60);

// One recorded action of a ghost replay. The meaning of each parameter
// depends on the action type (wind, weather, direction, club, item, ...),
// which is the action's index in GHOST_SHOT_DATA::m_actions.
struct GHOST_ACTION {
	union PARAM {
		unsigned long m_dw;
		float m_f;
	};

	bool m_changed;        // 0x0 (the action happened during the shot)
	unsigned char m_arg;   // 0x1 (RecordAct's second argument)
	PARAM m_param[5];      // 0x4 (-1 when not given)
};
CHECK_SIZE(GHOST_ACTION, 0x18);
CHECK_FIELD(GHOST_ACTION, m_changed, 0x0);
CHECK_FIELD(GHOST_ACTION, m_arg, 0x1);
CHECK_FIELD(GHOST_ACTION, m_param, 0x4);

// One recorded shot of a ghost hole. Only the members read so far are
// transcribed.
struct GHOST_SHOT_DATA {
	int m_type;                  // 0x0 (1 for a dummy shot, RecordDummyShot)
	int m_holeNum;               // 0x4 (compared with the current hole)
	int m_shotNum;               // 0x8 (1-based, checked by CGhostPlayer::OnInit)
	GHOST_ACTION m_actions[9];   // 0xc (wind, weather, direction, special wind, club, item, power shot, ball drop, time out)
	GHOST_SHOT_PROTOCOL m_protocol; // 0xe4 (converted into CGolfDoc::m_shotProtocol by CGhostPlayer::DoShot)
	float m_windPower;           // 0x118 (CWind::GetGlobalIntensity)
	float m_windDirection;       // 0x11c (CWind::GetGlobalDirection)
	unsigned char m_windPowerFraction; // 0x120 (the intensity's first decimal digit)
	float m_ballPos[3];          // 0x124 (where the shot starts)
	float m_globalWind[3];       // 0x130 (CWind::GetGlobalWind)
	float m_cameraAngle;         // 0x13c (CGolfDoc 0x10C, the camera yaw)
};
CHECK_SIZE(GHOST_SHOT_DATA, 0x140);
CHECK_FIELD(GHOST_SHOT_DATA, m_type, 0x0);
CHECK_FIELD(GHOST_SHOT_DATA, m_holeNum, 0x4);
CHECK_FIELD(GHOST_SHOT_DATA, m_shotNum, 0x8);
CHECK_FIELD(GHOST_SHOT_DATA, m_actions, 0xC);
CHECK_FIELD(GHOST_SHOT_DATA, m_protocol, 0xE4);
CHECK_FIELD(GHOST_SHOT_DATA, m_windPower, 0x118);
CHECK_FIELD(GHOST_SHOT_DATA, m_windDirection, 0x11C);
CHECK_FIELD(GHOST_SHOT_DATA, m_windPowerFraction, 0x120);
CHECK_FIELD(GHOST_SHOT_DATA, m_ballPos, 0x124);
CHECK_FIELD(GHOST_SHOT_DATA, m_globalWind, 0x130);
CHECK_FIELD(GHOST_SHOT_DATA, m_cameraAngle, 0x13C);

// The recorded shots of one hole (size 0x14, ghostdocument.cpp:137).
class CGhostHoleData {
	LAYOUT_CHECK_FRIEND;
public:
	CGhostHoleData(void);
	~CGhostHoleData(void);

	// ghostdocument.h:42
	void SetHoleNum(unsigned char holeNum) { m_holeNum = holeNum; }
	void AddShotData(GHOST_SHOT_DATA const *pShotData);
	void RecordAct(int type, int arg, int const *pParams, int count);
	void RecordDummyShot(unsigned int shotNum);
	GHOST_SHOT_DATA const *RecordShotData(sShotProtocol const *pProtocol, unsigned int shotNum);
	unsigned int RollBackShotData(unsigned int count);
	// ghostdocument.h:46
	std::list<GHOST_SHOT_DATA*> const& GetShotDataList(void) const { return m_shotDataList; }

private:
	unsigned char m_holeNum;                     // 0x0
	std::list<GHOST_SHOT_DATA*> m_shotDataList;  // 0x4
	GHOST_SHOT_DATA* m_pRecordingShot;           // 0x10 (zeroed by the ctor)
};
CHECK_SIZE(CGhostHoleData, 0x14);
BEGIN_LAYOUT_CHECK(CGhostHoleData)
	CHECK_FIELD(CGhostHoleData, m_holeNum, 0x0);
	CHECK_FIELD(CGhostHoleData, m_shotDataList, 0x4);
	CHECK_FIELD(CGhostHoleData, m_pRecordingShot, 0x10);
END_LAYOUT_CHECK

// CGhostDocument is a Fresh hierarchy; its layout is not transcribed yet, so
// only the members this unit defines are declared.
class CGhostDocument {
	friend CGhostDocument *CreateDocument(void);
public:
	~CGhostDocument(void);

	bool Import(unsigned char *data, unsigned int size);
	bool Import(const std::string &fileName);
	void RecordHoleData(const MsgObject &msg);
	CGhostHoleData const *GetHoleData(unsigned char holeNum) const;
	void RecordUserData(void);
	void RecordGameData(void);
	void Export(const std::string &fileName);
	void RecordException(const char *message);

	// ghostdocument.h:87
	void SetInvalidate(void) { m_bInvalidate = 1; }
	int IsInvalidate(void) const; // ghostdocument.h:88
	struct GHOST_USER_DATA const *GetUserData(void) const; // ghostdocument.h:93
	struct GHOST_GAME_DATA const *GetGameData(void) const; // ghostdocument.h:94

	struct GHOST_GAME_DATA *m_pGameData; // 0x00
	struct GHOST_USER_DATA *m_pUserData; // 0x04
	CGhostHoleData *m_pHoleData;         // 0x08 (array, indexed by hole number - 1)
	std::vector<GHOST_CARD_DATA> m_cardList; // 0x0C
	std::list<std::string> m_exceptionList; // 0x1C (RecordException)
	unsigned int m_shotCount; // 0x28 (the recorded shots: +1 per shot, -1 per roll back, RecordHoleData)
	int m_bInvalidate; // 0x2C

protected:
	CGhostDocument(void);

private:
	std::string MakeTextFileName(void) const;
	bool WriteTextFile(struct _iobuf *fp, GHOST_SHOT_DATA const *pShotData) const;
	bool WriteTextFile(struct _iobuf *fp, CGhostHoleData const *pHoleData) const;
	bool WriteExceptionLogFile(void) const;
};
CHECK_FIELD(CGhostDocument, m_pGameData, 0x00);
CHECK_FIELD(CGhostDocument, m_pUserData, 0x04);
CHECK_FIELD(CGhostDocument, m_pHoleData, 0x08);
CHECK_FIELD(CGhostDocument, m_cardList, 0x0C);
CHECK_FIELD(CGhostDocument, m_exceptionList, 0x1C);
CHECK_FIELD(CGhostDocument, m_shotCount, 0x28);
CHECK_FIELD(CGhostDocument, m_bInvalidate, 0x2C);

#endif // PG_CLIENT_PROJECTG_GHOSTDOCUMENT_H
