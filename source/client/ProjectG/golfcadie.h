// Original: d:\build\custom\temp\code\client\projectg\golfcadie.h
#ifndef PG_CLIENT_PROJECTG_GOLFCADIE_H
#define PG_CLIENT_PROJECTG_GOLFCADIE_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "scenemanager.h"
#include "wmath.h"
#include "../shared/classdefine.h"
#include "../shared/globalgamedefine.h"

#include <list>
#include <map>
#include <string>
#include <vector>

class CCaddieTalk;
class CPetBody;
class CRing;
class IObject;

// One caddie talk situation: its name in the talk tables.
struct sTalk {
	const char *m_name;     // 0x0, e.g. "click1", "danger", "ob"
	UNKNOWN(0x4, 0x8);
};
CHECK_SIZE(sTalk, 0x8);

extern sTalk talkTable[];

// The caddie actor of a golf round. Only the members the functions of this
// unit decompiled so far use are declared.
class CGolfCaddie : public IActor, public CRenderFuncPtr {
	LAYOUT_CHECK_FRIEND;
public:
	// A talk or motion of a caddie and the caddie level it needs.
	struct sBehavior {
		unsigned char m_level;   // 0x0, the caddie level (sCaddieInfo::level) the behavior needs
		std::string m_motion;    // 0x4
	};

	// One caddie the player owns: its body and IFF records.
	struct sCaddie {
		CPetBody *m_pBody;                  // 0x0
		UNKNOWN(0x4, 0x10);
		IFF_STRUCT::sCaddie *m_pIffCaddie;  // 0x10
		sCaddieInfo *m_pInfo;               // 0x14, the player's record of the caddie (0 if none)
		std::map<std::string, sBehavior> m_talks;   // 0x18, by talk name (talkTable) plus variant number (LoadTalkText)
		std::map<std::string, sBehavior> m_motions; // 0x24, by motion name (LoadMotionLevel)
		float m_stopDist;                   // 0x30, "*stop_dist" of the motion frame data, scaled by 3.2
		float m_startDelay;                 // 0x34, "*start_delay"
		float m_petSpeed;                   // 0x38, "*pet_speed", scaled by 3.2
		bool m_showRing;                    // 0x3c, the caddie's ring is displayed ("*ring")
		bool m_caddieView;                  // 0x3d, false when "*no_caddie_view" is set
		UNKNOWN(0x3e, 0x40);
	};

	// Predicate for std::find_if over the caddie list: matches a caddie by
	// its item type id and the type id of its equipped caddie item.
	struct sFindCaddie {
		// golfcadie.h:124
		sFindCaddie(unsigned long typeId, unsigned long cadItemTypeId)
			: m_typeId(typeId), m_cadItemTypeId(cadItemTypeId) {}

		// golfcadie.h:127
		bool operator()(sCaddie *caddie) {
			if (!caddie->m_pInfo)
				return caddie->m_pIffCaddie->m_typeId == m_typeId;

			return caddie->m_pInfo->cadItemTypeId == m_cadItemTypeId && caddie->m_pIffCaddie->m_typeId == m_typeId;
		}

		unsigned long m_typeId;        // 0x0
		unsigned long m_cadItemTypeId; // 0x4
	};

	static const WRTTI m_RTTI;

	virtual const WRTTI *GetRTTI(void) const { return &m_RTTI; }
	virtual void OnPreLoadInit(void);
	virtual void OnInit(void);
	virtual void OnReload(void);
	virtual void Display(void);
	virtual void DisplayShadow(void);
	virtual void OnDestroy(void);

protected:
	friend IObject *CGolfCaddieMakeInstance(void);

	CGolfCaddie(void);

	sCaddie *FindCaddie(unsigned long typeId, unsigned long cadItemTypeId);
	sCaddie *AddCaddie(unsigned long typeId, unsigned long cadItemTypeId, bool find);
	void LoadTalkText(sCaddie *caddie);
	void LoadMotionLevel(sCaddie *caddie);
	bool ReadAttribute(sCaddie *caddie);
	bool ShouldDisplay(void);
	bool IsGoodMotion(sCaddie *caddie, const char *motion);
	sBehavior *SetTalkToBubble(unsigned char type, bool chatOnly, unsigned char param);
	void SetBubble(unsigned char type, bool chatOnly, unsigned char param);
	void SetBubble(unsigned char type, const char *msg);
	void SetCaddieTalk_ChatOnly(unsigned char type);
	void ResetTalk(void);
	void LookUp(WVector *target, bool immediate);
	void DontLookUp(void);
	char *GetBehaviorToken(char *token, char *src);
	char *GetBehaviorEndToken(char *token, char *src);
	void UseSpecialAbility(void);
	void SearchGround(void);
	void Dodge(void);

	sCaddie *m_pCaddie;                 // 0x1c, the caddie in use
	std::list<sCaddie *> m_caddies;     // 0x20
	UNKNOWN(0x2c, 0x30);
	unsigned long m_unknown30;          // 0x30, cleared by OnInit
	UNKNOWN(0x34, 0x38);
	WVector m_position;                 // 0x38, where the caddie stands
	unsigned long m_unknown44;          // 0x44, cleared by OnInit
	float m_angle;                      // 0x48, the caddie's heading (SearchGround: RotMat about y)
	UNKNOWN(0x4c, 0x58);
	bool m_dodging;                     // 0x58, a dodge motion is playing (Dodge)
	bool m_visible;                     // 0x59, the caddie is shown
	UNKNOWN(0x5a, 0x60);
	unsigned long m_unknown60;          // 0x60, cleared by OnInit
	unsigned char m_unknown64;          // 0x64, cleared by OnInit
	UNKNOWN(0x65, 0x68);
	unsigned long m_unknown68;          // 0x68, cleared by OnInit
	UNKNOWN(0x6c, 0x70);
	int m_fxScript;                     // 0x70, FxInterpreter bounding-box script of the caddie (-1: none)
	CRing *m_pRing;                     // 0x74, the ring shown around the caddie
	unsigned char m_bubbleTalkType;     // 0x78, talk type shown in the bubble
	bool m_talked[36];                  // 0x79, per talk type: already said
	bool m_talkFlag;                    // 0x9d, cleared with the talk state
	unsigned long m_talkResetTime;      // 0xa0, g_CurrentTime of the last ResetTalk
	CCaddieTalk *m_pCaddieTalk;         // 0xa4
};

BEGIN_LAYOUT_CHECK(CGolfCaddie)
	CHECK_FIELD(CGolfCaddie, m_pCaddie, 0x1c);
	CHECK_FIELD(CGolfCaddie, m_caddies, 0x20);
	CHECK_FIELD(CGolfCaddie, m_position, 0x38);
	CHECK_FIELD(CGolfCaddie, m_visible, 0x59);
	CHECK_FIELD(CGolfCaddie, m_bubbleTalkType, 0x78);
	CHECK_FIELD(CGolfCaddie, m_talked, 0x79);
	CHECK_FIELD(CGolfCaddie, m_talkFlag, 0x9d);
	CHECK_FIELD(CGolfCaddie, m_fxScript, 0x70);
	CHECK_FIELD(CGolfCaddie, m_pRing, 0x74);
	CHECK_FIELD(CGolfCaddie, m_talkResetTime, 0xa0);
	CHECK_FIELD(CGolfCaddie, m_pCaddieTalk, 0xa4);
END_LAYOUT_CHECK
CHECK_SIZE(CGolfCaddie::sCaddie, 0x40);
CHECK_FIELD(CGolfCaddie::sCaddie, m_pBody, 0x0);
CHECK_FIELD(CGolfCaddie::sCaddie, m_pIffCaddie, 0x10);
CHECK_FIELD(CGolfCaddie::sCaddie, m_pInfo, 0x14);
CHECK_FIELD(CGolfCaddie::sCaddie, m_talks, 0x18);
CHECK_FIELD(CGolfCaddie::sCaddie, m_motions, 0x24);
CHECK_FIELD(CGolfCaddie::sCaddie, m_stopDist, 0x30);
CHECK_FIELD(CGolfCaddie::sCaddie, m_startDelay, 0x34);
CHECK_FIELD(CGolfCaddie::sCaddie, m_petSpeed, 0x38);
CHECK_FIELD(CGolfCaddie::sCaddie, m_showRing, 0x3c);
CHECK_FIELD(CGolfCaddie::sCaddie, m_caddieView, 0x3d);
CHECK_SIZE(CGolfCaddie::sFindCaddie, 0x8);
CHECK_FIELD(CGolfCaddie::sFindCaddie, m_typeId, 0x0);
CHECK_FIELD(CGolfCaddie::sFindCaddie, m_cadItemTypeId, 0x4);
CHECK_SIZE(CGolfCaddie::sBehavior, 0x20);
CHECK_FIELD(CGolfCaddie::sBehavior, m_level, 0x0);
CHECK_FIELD(CGolfCaddie::sBehavior, m_motion, 0x4);

// A source of caddie talk: loads its messages, then is asked for a message
// fitting the current situation.
struct ICaddieTalk {
public:
	virtual void Load(void) = 0;
	virtual bool FindMsg(void) = 0;
	// golfcadie.h:222
	virtual const char *GetTalkMsg(void) const { return 0; }
	// golfcadie.h:223
	virtual int GetPriority(void) { return 0; }
	// golfcadie.h:225
	virtual ~ICaddieTalk(void) {}
};

CHECK_SIZE(ICaddieTalk, 0x4);

// Reader of the caddie talk XML: a situation is looked up by name and
// parameters, and yields one attribute set per matching entry.
struct XmlCaddieTalk {
public:
	struct parameters {
		// golfcadie.h:241
		void add(const std::string &value) {
			m_values.push_back(value);
		}

		std::vector<std::string> m_values; // 0x0
	};

	struct attributes {
		// golfcadie.h:251
		const char *get(const std::string &name) {
			std::map<std::string, std::string>::iterator it = m_values.find(name);
			if (it != m_values.end())
				return it->second.c_str();
			return 0;
		}

		std::map<std::string, std::string> m_values; // 0x0
	};

	virtual bool LoadSituation(const char *situation, const parameters &params,
		std::vector<attributes> &result);
};

CHECK_SIZE(XmlCaddieTalk, 0x4);
CHECK_SIZE(XmlCaddieTalk::parameters, 0x10);
CHECK_SIZE(XmlCaddieTalk::attributes, 0xC);

// Talk for the tee shot.
class cCaddieTalkTeeShot : public XmlCaddieTalk, public ICaddieTalk {
public:
	struct sTalkData {
		// golfcadie.h:277
		bool operator==(const sTalkData &other) const {
			if (m_hole == -1)
				return m_par == other.m_par && m_condition == other.m_condition;

			return m_hole == other.m_hole && m_par == other.m_par && m_condition == other.m_condition;
		}

		int m_hole;          // 0x0
		int m_par;           // 0x4
		int m_condition;     // 0x8
		std::string m_msg;   // 0xC
	};

	// golfcadie.h:289
	cCaddieTalkTeeShot(void) {}
	// golfcadie.h:290
	virtual ~cCaddieTalkTeeShot(void) {}

	virtual void Load(void);
	virtual bool FindMsg(void);
	// golfcadie.h:293
	virtual const char *GetTalkMsg(void) const { return m_talkMsg.c_str(); }

	std::vector<sTalkData> m_talkData; // 0x8
	std::string m_talkMsg;             // 0x18
};

CHECK_SIZE(cCaddieTalkTeeShot::sTalkData, 0x28);
CHECK_FIELD(cCaddieTalkTeeShot::sTalkData, m_msg, 0xC);
CHECK_SIZE(cCaddieTalkTeeShot, 0x34);
CHECK_FIELD(cCaddieTalkTeeShot, m_talkData, 0x8);
CHECK_FIELD(cCaddieTalkTeeShot, m_talkMsg, 0x18);

// The caddie's talk sources; FindTalk picks the first one that has a message.
class CCaddieTalk {
public:
	~CCaddieTalk(void);

	void Init(void);
	void Release(void);
	const ICaddieTalk *FindTalk(void);
	// golfcadie.h:316
	void OnStop(void) { m_isTalking = 0; }

	std::list<ICaddieTalk *> m_talks; // 0x0
	int m_isTalking;                  // 0xC
};

CHECK_SIZE(CCaddieTalk, 0x10);
CHECK_FIELD(CCaddieTalk, m_talks, 0x0);
CHECK_FIELD(CCaddieTalk, m_isTalking, 0xC);

#endif // PG_CLIENT_PROJECTG_GOLFCADIE_H
