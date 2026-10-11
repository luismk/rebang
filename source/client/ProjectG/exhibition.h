// Original: d:\build\custom\temp\code\client\projectg\exhibition.h
#ifndef PG_CLIENT_PROJECTG_EXHIBITION_H
#define PG_CLIENT_PROJECTG_EXHIBITION_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wmath.h"
#include "wscene.h"
#include <list>
#include <vector>
#include "fx.h"
#include "parttidlist.h"

class WPuppet;
struct w_motion_data;

class CPetFrame;
class CFxSpray;
class CFxSequence;
struct sTexturePart;
class WRect;
class WPoint;

// What a CExhibition shows (SetModel's second argument; 14 kinds, the
// others are not named yet).
enum eExhibitType {
	EXHIBIT_CHARACTER = 0,   // a character, drawn through m_pPetFrame
	EXHIBIT_CADDIE = 1
};

// A model viewer (character, caddie, item...) with its own camera, light and
// effect box; only the members the unit's code uses are named.
class CExhibition {
public:
	CExhibition(float x, float y, float screenScale, bool autoRecover);
	void ResetCameraRecoverTime(void);      // exhibition.h:78
	void SetAutoRecoverCamera(bool autoRecover); // exhibition.h:100
	WPuppet* GetModel(void);                // exhibition.h:111
	~CExhibition(void);
	void SetDistance(float distance, bool b1, bool b2);
	void SetCenter(float x, float y);
	void Process(float dt, bool b);
	CPetFrame* GetPetFrame(void);           // exhibition.h:112
	float GetInitialMotionLength(void);     // exhibition.h:113
	unsigned long GetTypeId(void) const;   // exhibition.h:115
	bool IsNextMotion(void);                // exhibition.h:82
	float GetDirection(void);               // exhibition.h:116
	float GetDistance(void);
	void SetDirection(float direction);
	const WVector2D& GetCenter(void);
	void SetArea(float radius);
	void SetArea(float, float);
	bool SetMotion(const char *, bool, float);
	bool SetModel(CPartTidList &, unsigned long *, const char *, float, int);
	w_motion_data* GetMotionData(void);
	void ParseBoundingBoxScript(void);
	void Display(float, float);
	const char* GetMotionName(void);
	bool ChangeBoneTexture(const char* boneName, const char* texName, const char* fileName);
	bool ChangeBoneTexture(const char* boneName, const char* fileName);
	void SetDefaultDirection(float direction);
	void Zoom(float delta);
	bool IsDirectionChanged(void);
	const char* GetNextCharMotion(int motionSet);
	const char* GetNextCaddieMotion(int motionSet);
	void SetChatFacial(int expression, unsigned long faceType);
	void SetAlpha(unsigned char alpha);
	bool NextMotion(void);
	bool SetModel(const char *, eExhibitType);
	bool SetModel(unsigned long typeId);
	int ProcessBongdariShop(float dt);
	void DisplayClip(const WRect &, float, float, float);
	void AttachAngelWingFx(void);
	void MoveModel(const WPoint& point, const WRect* pRect, float fov);
	WMatrix& GetMatrix(void) { return m_matrix; }                 // exhibition.h:117
	void SetDistanceRange(float minDistance, float maxDistance) { // exhibition.h:98
		m_minDistance = minDistance;
		m_maxDistance = maxDistance;
	}
	void SetControlFlag(unsigned long flag) { m_controlFlag = flag; } // exhibition.h:99
	void SetEnableRotate(bool enable) { m_enableRotate = enable; }  // exhibition.h:101
	void SetEnableZoom(bool enable) { m_enableZoom = enable; }      // exhibition.h:102

private:
	float m_motionDelay;             // 0x0 counts down in Process; the motion runs once it is spent
	float m_initialMotionLength;     // 0x4
	int m_motionIndex;               // 0x8 index in the current motion set
	int m_charMotionSet;             // 0xc row of the character motion table
	int m_caddieMotionSet;           // 0x10 row of the caddie motion table
	int m_cameraUpdate;              // 0x14 camera needs an update (1, 2 after Init)
	bool m_autoRecoverCamera;        // 0x18
	bool m_lockDirection;            // 0x19 left button held down in the area
	bool m_zoomDragging;             // 0x1a right button held down in the area
	UNKNOWN(0x1B, 0x1C);
	float m_alpha;                   // 0x1c raised to 255 by Process
	float m_checkedDirection;        // 0x20 direction when m_directionChanged was last raised
	WVector2D m_angle;               // 0x24 camera angles: x pitch (about x), y direction (about y)
	float m_pitchSpeed;              // 0x2c recovery speed toward m_defaultAngle.x
	float m_directionSpeed;          // 0x30 recovery speed toward m_defaultAngle.y
	WVector2D m_defaultAngle;        // 0x34 x pitch, y direction (copied whole into m_angle)
	float m_curDistance;             // 0x3c moves toward m_distance (UpdateCamera)
	float m_distance;                // 0x40
	float m_defaultDistance;         // 0x44 restored by the camera recovery
	float m_screenScale;             // 0x48 constructor's third argument (SetCenter, SetScreenPos)
	float m_cameraRecoverTime;       // 0x4c
	float m_pitchLimit;              // 0x50 the drag keeps m_angle.x - m_defaultAngle.x below it + 15 degrees
	int m_resetByInit;               // 0x54 cleared by Init, read nowhere in the unit
	WVector2D m_areaSize;            // 0x58
	WVector2D m_center;              // 0x60
	float m_areaRadius;              // 0x68
	WMatrix m_camera;                // 0x6c
	float m_frame;                   // 0x9c time in the current motion
	w_motion_data* m_pMotionData;    // 0xa0
	CPetFrame* m_pPetFrame;          // 0xa4
	WPuppet* m_model;                // 0xa8
	WMatrix m_matrix;                // 0xac the model's rotation (GetMatrix)
	int m_texHandle;                 // 0xdc released through g_resrcmng
	LightSet m_light;                // 0xe0
	float m_minDistance;             // 0xfc SetDistance clamps to [min, max]
	float m_maxDistance;             // 0x100
	CPvsFxBox m_fxBox;               // 0x104
	std::vector<CFxSpray*> m_sprays;         // 0x468 sprays of the bounding box script
	std::vector<CFxSequence*> m_sequences;   // 0x478 sequences of the bounding box script
	std::list<sTexturePart*> m_texParts;     // 0x488 pending texture parts
	int m_exhibitType;               // 0x494 0: pet frame (character), else m_model
	unsigned long m_typeId;          // 0x498
	unsigned long* m_pItemSlots;     // 0x49c SetModel's item slots
	unsigned long m_controlFlag;     // 0x4a0 1 zoom, 2 rotate, 4 pitch, 8 click: next motion, 0x10 follow Bip01
	bool m_nextMotion;               // 0x4a4
	UNKNOWN(0x4A5, 0x4A8);
	float m_directionCheckTime;      // 0x4a8 1 s period of the direction change test
	float m_facialTime;              // 0x4ac 2 s period of the chat facial
	bool m_directionChanged;         // 0x4b0
	bool m_facialChanged;            // 0x4b1
	bool m_setByCtor;                // 0x4b2 set by the constructor, read nowhere in the unit
	UNKNOWN(0x4B3, 0x4B4);
	CPartTidList m_partTidList;      // 0x4b4
	CFxSequence* m_pFxSequence1;     // 0x57c deleted by SetCenter
	CFxSequence* m_pFxSequence2;     // 0x580 deleted by SetCenter
	bool m_enableZoom;               // 0x584
	bool m_enableRotate;             // 0x585
	UNKNOWN(0x586, 0x588);

protected:
	void Init(void);
	void UpdateCamera(float dt);
	float GetBaseAngle(float angle, float baseAngle);
	void ReleasePet(void);
	void SetPendingTexPart(char* partName, const char* texName);
	sTexturePart* FindTexturePart(char* partName, char* texName);
	void ParseScript_ShowVis(bool show, const char* script);
	bool InValidArea(void);
	void ScriptProcess(const w_motion_data* data, float from, float to, bool all);
	void Animate(void);
	void SetScreenPos(WPuppet* pPuppet);
	void ParsePuppetScript(const char* script);

	LAYOUT_CHECK_FRIEND;
};
BEGIN_LAYOUT_CHECK(CExhibition)
	CHECK_FIELD(CExhibition, m_initialMotionLength, 0x4);
	CHECK_FIELD(CExhibition, m_autoRecoverCamera, 0x18);
	CHECK_FIELD(CExhibition, m_angle, 0x24);
	CHECK_FIELD(CExhibition, m_nextMotion, 0x4A4);
	CHECK_FIELD(CExhibition, m_cameraRecoverTime, 0x4C);
	CHECK_FIELD(CExhibition, m_distance, 0x40);
	CHECK_FIELD(CExhibition, m_areaSize, 0x58);
	CHECK_FIELD(CExhibition, m_center, 0x60);
	CHECK_FIELD(CExhibition, m_areaRadius, 0x68);
	CHECK_FIELD(CExhibition, m_pMotionData, 0xA0);
	CHECK_FIELD(CExhibition, m_pPetFrame, 0xA4);
	CHECK_FIELD(CExhibition, m_model, 0xA8);
	CHECK_FIELD(CExhibition, m_typeId, 0x498);
	CHECK_FIELD(CExhibition, m_motionIndex, 0x8);
	CHECK_FIELD(CExhibition, m_charMotionSet, 0xC);
	CHECK_FIELD(CExhibition, m_caddieMotionSet, 0x10);
	CHECK_FIELD(CExhibition, m_cameraUpdate, 0x14);
	CHECK_FIELD(CExhibition, m_lockDirection, 0x19);
	CHECK_FIELD(CExhibition, m_texHandle, 0xDC);
	CHECK_FIELD(CExhibition, m_exhibitType, 0x494);
	CHECK_FIELD(CExhibition, m_directionChanged, 0x4B0);
	CHECK_FIELD(CExhibition, m_facialChanged, 0x4B1);
	CHECK_FIELD(CExhibition, m_checkedDirection, 0x20);
	CHECK_FIELD(CExhibition, m_pitchSpeed, 0x2C);
	CHECK_FIELD(CExhibition, m_directionSpeed, 0x30);
	CHECK_FIELD(CExhibition, m_defaultAngle, 0x34);
	CHECK_FIELD(CExhibition, m_curDistance, 0x3C);
	CHECK_FIELD(CExhibition, m_defaultDistance, 0x44);
	CHECK_FIELD(CExhibition, m_resetByInit, 0x54);
	CHECK_FIELD(CExhibition, m_camera, 0x6C);
	CHECK_FIELD(CExhibition, m_matrix, 0xAC);
	CHECK_FIELD(CExhibition, m_light, 0xE0);
	CHECK_FIELD(CExhibition, m_minDistance, 0xFC);
	CHECK_FIELD(CExhibition, m_maxDistance, 0x100);
	CHECK_FIELD(CExhibition, m_pItemSlots, 0x49C);
	CHECK_FIELD(CExhibition, m_controlFlag, 0x4A0);
	CHECK_FIELD(CExhibition, m_directionCheckTime, 0x4A8);
	CHECK_FIELD(CExhibition, m_facialTime, 0x4AC);
	CHECK_FIELD(CExhibition, m_enableZoom, 0x584);
	CHECK_FIELD(CExhibition, m_enableRotate, 0x585);
	CHECK_FIELD(CExhibition, m_motionDelay, 0x0);
	CHECK_FIELD(CExhibition, m_zoomDragging, 0x1A);
	CHECK_FIELD(CExhibition, m_alpha, 0x1C);
	CHECK_FIELD(CExhibition, m_screenScale, 0x48);
	CHECK_FIELD(CExhibition, m_pitchLimit, 0x50);
	CHECK_FIELD(CExhibition, m_frame, 0x9C);
	CHECK_FIELD(CExhibition, m_fxBox, 0x104);
	CHECK_FIELD(CExhibition, m_sprays, 0x468);
	CHECK_FIELD(CExhibition, m_sequences, 0x478);
	CHECK_FIELD(CExhibition, m_texParts, 0x488);
	CHECK_FIELD(CExhibition, m_setByCtor, 0x4B2);
	CHECK_FIELD(CExhibition, m_partTidList, 0x4B4);
	CHECK_FIELD(CExhibition, m_pFxSequence1, 0x57C);
	CHECK_FIELD(CExhibition, m_pFxSequence2, 0x580);
END_LAYOUT_CHECK
CHECK_SIZE(CExhibition, 0x588);

// RealMyRoomMainUI.cpp defines REALMYROOMMAINUI_EXHIBITION_INLINE empty before
// its includes: the original holds the out-of-line copies of these inlines in
// RealMyRoomMainUI's object; every other unit inlines it.
#ifndef REALMYROOMMAINUI_EXHIBITION_INLINE
#define REALMYROOMMAINUI_EXHIBITION_INLINE inline
#endif

// scoreboard.cpp defines SCOREBOARD_EXHIBITION_INLINE empty before its
// includes: the original holds the out-of-line copies of these inlines in
// scoreboard's object; every other unit inlines them.
#ifndef SCOREBOARD_EXHIBITION_INLINE
#define SCOREBOARD_EXHIBITION_INLINE inline
#endif

// exhibition.h:78
REALMYROOMMAINUI_EXHIBITION_INLINE void CExhibition::ResetCameraRecoverTime(void) {
	m_cameraRecoverTime = 0.0f;
}

// exhibition.h:100
REALMYROOMMAINUI_EXHIBITION_INLINE void CExhibition::SetAutoRecoverCamera(bool autoRecover) {
	m_autoRecoverCamera = autoRecover;
}

// exhibition.h:111
REALMYROOMMAINUI_EXHIBITION_INLINE WPuppet* CExhibition::GetModel(void) {
	return m_model;
}

// exhibition.h:112
SCOREBOARD_EXHIBITION_INLINE CPetFrame* CExhibition::GetPetFrame(void) {
	return m_pPetFrame;
}

// exhibition.h:113
SCOREBOARD_EXHIBITION_INLINE float CExhibition::GetInitialMotionLength(void) {
	return m_initialMotionLength;
}

// exhibition.h:115
REALMYROOMMAINUI_EXHIBITION_INLINE unsigned long CExhibition::GetTypeId(void) const {
	return m_typeId;
}

#endif // PG_CLIENT_PROJECTG_EXHIBITION_H
