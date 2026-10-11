// Original: d:\build\custom\temp\code\client\projectg\fxspray.h
#ifndef PG_CLIENT_PROJECTG_FXSPRAY_H
#define PG_CLIENT_PROJECTG_FXSPRAY_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wmath.h"
#include "FxBox.h"
#include "ringq.h"

class CFxBox;
struct Attacher;
struct LightSet;
class CFxSequence;
class WView;
class CPetBody;

// One particle of a CFxSpray (size 0xF4), new'd by CFxSpray::Generate.
struct CFxParticle {
	LAYOUT_CHECK_FRIEND;
	// One point of the particle's tail (size 0x10).
	struct sTail {
		sTail(void) {}
		WVector pos;                // 0x0, MakeTail: the spray's output position
		unsigned long value;        // 0xC, MakeTail copies the particle's 0x9C
	};
	// One segment of the particle's bandana strip (size 0x1C).
	struct sBandana {
		sBandana(void) {}
		WVector a;                  // 0x0, one edge of the strip
		WVector b;                  // 0xC, the other edge
		float fLen;                 // 0x18, distance to the previous segment's middle
	};

	CFxParticle(void);
	virtual ~CFxParticle(void);

	bool m_bDead;                   // 0x4, Process skips the motion of a dead particle
	bool m_bGrounded;               // 0x5, Process: landed on the ground (ground friction, no gravity)
	PADDING(0x6, 0x8);
	WVector m_basePos;              // 0x8, Spawn places the spawned spray at m_basePos + m_pos
	WVector m_offset;               // 0x14, zeroed by InitParticle (use unknown)
	WVector m_pos;                  // 0x20, CFxSpray::GetBgPos(const CFxParticle*)
	WMatrix m_rot;                  // 0x2C, CFxSpray::GetBgRot
	WVector m_bubbleOffset;         // 0x5C, Process: the bubble wobble, added to the displayed position
	std::list<WVector> m_points;    // 0x68
	WVector m_dir;                  // 0x74, the velocity; Spawn copies it into the spawned spray's m_spawnDir
	float m_fWindScale;             // 0x80, Process scales the spray's wind by it; InitParticle picks it in a random range
	WVector m_rotSpeed;             // 0x84, Process: m_rotation's speed, scaled by the rotation keys
	WVector m_rotation;             // 0x90, Process: the pet body's rotation
	int m_frame;                    // 0x9C, the sprite frame (frame keys); MakeTail copies it into each tail point
	float m_fFrameTime;             // 0xA0, Process: time of the last frame change
	float m_fLifeTime;              // 0xA4, the particle's whole life
	float m_fLifeLeft;              // 0xA8, Process counts it down; the particle dies at 0
	float m_fBaseSize;              // 0xAC, Process: m_fSize = size key * m_fBaseSize / 100
	float m_fSize;                  // 0xB0, the current size; MakeTail's strip width, scaled by the spray's
	float m_fSizeParam;             // 0xB4, InitParticle: init_size's third number
	unsigned char m_color[4];       // 0xB8, Process: the fade keys' bytes in reverse order (m_color[3] the alpha)
	W3dAniSpr* m_pSprite;           // 0xBC, InitParticle copies the spray's sprite (Param::m_pSprite)
	CPetBody* m_pPetBody;           // 0xC0, InitParticle's puppet particle, deleted with it
	TRingQ<sTail> m_tails;          // 0xC4
	TRingQ<sBandana> m_bandanas;    // 0xD4
	float m_fTailTime;              // 0xE4, MakeTail: time of the last tail point
	float m_fScale;                 // 0xE8, 1 until Process scales it
	int m_spawnCount;               // 0xEC, Spawn counts the sprays it spawned
	float m_fSpawnTime;             // 0xF0, Process: time of the last interval spawn
};
CHECK_SIZE(CFxParticle::sTail, 0x10);
CHECK_SIZE(CFxParticle::sBandana, 0x1C);
CHECK_FIELD(CFxParticle::sTail, value, 0xC);
CHECK_FIELD(CFxParticle::sBandana, b, 0xC);
CHECK_FIELD(CFxParticle::sBandana, fLen, 0x18);
CHECK_SIZE(CFxParticle, 0xF4);
BEGIN_LAYOUT_CHECK(CFxParticle)
	CHECK_FIELD(CFxParticle, m_basePos, 0x8);
	CHECK_FIELD(CFxParticle, m_offset, 0x14);
	CHECK_FIELD(CFxParticle, m_pos, 0x20);
	CHECK_FIELD(CFxParticle, m_fSizeParam, 0xB4);
	CHECK_FIELD(CFxParticle, m_rot, 0x2C);
	CHECK_FIELD(CFxParticle, m_points, 0x68);
	CHECK_FIELD(CFxParticle, m_dir, 0x74);
	CHECK_FIELD(CFxParticle, m_fWindScale, 0x80);
	CHECK_FIELD(CFxParticle, m_pSprite, 0xBC);
	CHECK_FIELD(CFxParticle, m_pPetBody, 0xC0);
	CHECK_FIELD(CFxParticle, m_tails, 0xC4);
	CHECK_FIELD(CFxParticle, m_bandanas, 0xD4);
	CHECK_FIELD(CFxParticle, m_fScale, 0xE8);
	CHECK_FIELD(CFxParticle, m_spawnCount, 0xEC);
	CHECK_FIELD(CFxParticle, m_bDead, 0x4);
	CHECK_FIELD(CFxParticle, m_bGrounded, 0x5);
	CHECK_FIELD(CFxParticle, m_bubbleOffset, 0x5C);
	CHECK_FIELD(CFxParticle, m_rotSpeed, 0x84);
	CHECK_FIELD(CFxParticle, m_rotation, 0x90);
	CHECK_FIELD(CFxParticle, m_frame, 0x9C);
	CHECK_FIELD(CFxParticle, m_fFrameTime, 0xA0);
	CHECK_FIELD(CFxParticle, m_fLifeTime, 0xA4);
	CHECK_FIELD(CFxParticle, m_fLifeLeft, 0xA8);
	CHECK_FIELD(CFxParticle, m_fBaseSize, 0xAC);
	CHECK_FIELD(CFxParticle, m_fSize, 0xB0);
	CHECK_FIELD(CFxParticle, m_color, 0xB8);
	CHECK_FIELD(CFxParticle, m_fSpawnTime, 0xF0);
	CHECK_FIELD(CFxParticle, m_fTailTime, 0xE4);
END_LAYOUT_CHECK

// A particle spray of an effect (a ".spr" file), owned by a CFxBox and
// optionally by the CFxSequence that opened it. Layout from the CFxBox code
// that uses it (fxbox.cpp); the rest stays UNKNOWN.
class CFxSpray {
	LAYOUT_CHECK_FRIEND;
	friend class CFxBox;
	friend class CPvsFxBox;
	friend class CFx;
	friend class CRealMyRoom_Avatar;
	friend class CExhibition;
	friend class CSky;
	friend class CFxSequence;
public:
	CFxSpray(CFxBox& box);
	virtual ~CFxSpray(void);

	bool ParseFile(const char* name, bool bOriginal);
	bool ParseLine(const char* line);
	void SetActive(bool bActive);                 // fxspray.inl:63
	WVector& InitialMinVel(void);                 // fxspray.inl:84
	WVector& InitialMaxVel(void);                 // fxspray.inl:91
	void SetLight(const LightSet& light);
	void SetWind(const WVector& wind);
	void SetDistLimit(float limitA, float limitB);

protected:
	void Display(WView* pView);
	bool Process(float fDelta);
	unsigned long GetParticleColor(const WView* view, const WVector& pos, const CFxParticle* particle, unsigned long color);

protected:
	WVector GetBgPos(const CFxParticle* particle);   // fxspray.inl:51
	WMatrix GetBgRot(const CFxParticle* particle);   // fxspray.inl:57
	void GetBoneMatrix(WMatrix* pMatrix);            // fxspray.inl:107
	void DrawSprite(WView* pView, CFxParticle* pParticle, const WVector& pos);
	void DrawTail(WView* pView, CFxParticle* pParticle, const WVector& pos);
	void DrawBandana(WView* pView, CFxParticle* pParticle, const WVector& pos);
	WVector GetBgPos(void);
	WMatrix Rotate(float fTime, float fScale);
	WMatrix RotateNozzle(float fTime, float fScale);
	void Revolute(float fTime, float fScale);
	void RevoluteGrand(float fTime, float fScale);

public:
	void Reset(void);

public:
	// Speed key of a ".spr" file, the particles' initial speed interpolated
	// from v0 at t0 to v1 at t1 (size 0x10). Generate uses the last active one.
	struct sVel {
		int t0;                         // 0x0
		float v0;                       // 0x4
		int t1;                         // 0x8
		float v1;                       // 0xC
	};
	// Rotation key (size 0x10).
	struct sRot {
		sRot(void) {}
		float m_v[4];                   // 0x0, "add_rotation": atof, number, atof, number
	};
	// Rotation about an axis, its angle interpolated from a0 at t0 to a1
	// at t1 (size 0x1C). Rotate and RotateNozzle sum the active ones.
	struct sNozzleRot {
		sNozzleRot(void) {}
		WVector axis;                   // 0x0
		int t0;                         // 0xC
		float a0;                       // 0x10
		int t1;                         // 0x14
		float a1;                       // 0x18
	};
	// Revolution key of Revolute / RevoluteGrand ("add_revolution" /
	// "add_prevolution"): a turn about axis through center, its speed
	// interpolated from speed0 at t0 to speed1 at t1 (size 0x28).
	struct sRevol {
		sRevol(void) {}
		WVector center;                 // 0x0
		WVector axis;                   // 0xC
		int t0;                         // 0x18
		float speed0;                   // 0x1C
		int t1;                         // 0x20
		float speed1;                   // 0x24
	};
	// Generation key: Generate emits count particles spread evenly over
	// [t0, t1] (size 0x18).
	struct sGen {
		int t0;                         // 0x0
		int t1;                         // 0x4
		int count;                      // 0x8, particles to emit over the key
		float spread;                   // 0xC, InitParticle's random spread
		int generated;                  // 0x10, particles emitted so far
		float lastTime;                 // 0x14, the last time Generate emitted
	};
	// Gravity key (size 0x18).
	struct sGravity {
		sGravity(void) {}
		float m_v[6];                   // 0x0, "add_gravitypoint": its six numbers
	};
	// Fade key (size 0x10).
	struct sFade {
		sFade(void) {}
		float t0;                       // 0x0, "add_fade": time of the first key
		unsigned char start[4];         // 0x4, the first key's four values (start[0] is the one "lifetime" forces to 255)
		float t1;                       // 0x8
		unsigned char end[4];           // 0xC, the second key's four values
	};
	// Size key (size 0x10).
	struct sSize {
		sSize(void) {}
		float m_v[4];                   // 0x0, "add_size": its four numbers
	};
	// Texture frame key (size 0x14).
	struct sFrame {
		sFrame(void) {}
		float m_f0;                     // 0x0, "add_frame"
		float m_f1;                     // 0x4
		int m_i2;                       // 0x8
		int m_i3;                       // 0xC
		int m_i4;                       // 0x10, a ParseNumber result
	};
	// Camera quake key (size 0x34).
	struct sQuake {
		sQuake(void) {}
		struct sKey {
			float p0;                   // 0x0
			float p1;                   // 0x4
			float p2;                   // 0x8
			int t;                      // 0xC, a ParseNumber result
		};
		sKey m_keys[3];                 // 0x0, "add_quake": three groups of three numbers and a time
		float m_fLastTime;              // 0x30, Process: time of the last shake (0 from ParseLine)
	};
	// Flash key (size 0xC).
	struct sFlash {
		sFlash(void) {}
		int m_v[3];                     // 0x0, "add_flash": three ParseNumber results
	};
	// Bubble key (size 0x18).
	struct sBubble {
		sBubble(void) {}
		WVector m_min;                  // 0x0, "add_bubble": arguments 0, 2, 4
		WVector m_max;                  // 0xC, arguments 1, 3, 5
	};
	// A spray a particle spawns (size 0x114).
	struct sSpawn {
		int m_type;                     // 0x0, 0 interval, 1 age, 2 on death, 3 on ground
		char m_name[0x104];             // 0x4, the ".spr" file of the spawned spray
		bool m_bFollow;                 // 0x108, the spawned spray follows this one (m_pSpawner, Param::m_bFollowSpawner)
		PADDING(0x109, 0x10C);
		float m_fParam;                 // 0x10C, the interval or age of m_type
		int m_intervalCount;            // 0x110, interval spawns: the integer after the interval

	};
	// A sound the spray plays (size 0x108).
	struct sSound {
		sSound(void) {}
		char m_name[0x104];             // 0x0, the file name without ".wav"
		float m_fVolume;                // 0x104, -1 when the line gives none
	};

protected:
	void Spawn(const sSpawn& spawn, CFxParticle* pParticle);
	bool InitParticle(bool bSpawn, CFxParticle* pParticle, float fSpeed, float fSpread, Waabb& area, bool bSecond);
	bool Generate(float fTime);
	void MakeTail(float fTime, CFxParticle* pParticle, const WVector& pos);

public:
	// The spray's parameters, read from its ".spr" file (size 0x3DC).
	struct Param {
		bool m_bSurvivesSequence;       // 0x0, stays in the box when its sequence goes; particles follow the spray (GetBgPos/GetBgRot)
		bool m_bFollowSpawner;          // 0x1, Spawn: it follows the spray that spawned it
		bool m_bBillboard;              // 0x2, "source": the sprite is a billboard
		bool m_bTransformPos;           // 0x3, GetBgPos turns m_pos by the spray's matrix; "AttachedToParent on"
		unsigned long m_flags;          // 0x4, "AttachedToParent": bits 1, 2, 4 from its three integers
		W3dAniSpr* m_pSprite;           // 0x8, "source": the sprite CFxBox::LoadSprite made
		char m_sourceName[0x104];       // 0xC, "source": the line's first argument
		bool m_bPetLight;               // 0x110, "source ... petlight"
		bool m_bRoundArea;              // 0x111, m_area's min is a center, max.x/max.y radii
		PADDING(0x112, 0x114);
		Waabb m_area;                   // 0x114, where Generate places the particles
		bool m_bRegenAtSamePos;         // 0x12C, "regenatsamepos"
		PADDING(0x12D, 0x130);
		float m_fCamOffset;             // 0x130, "camoffset"
		int m_bScaleArea;               // 0x134, Generate scales m_area by m_fAreaScaleXZ/Y
		int m_iLifetimeMax;             // 0x138, "lifetime": the second number (m_bScaleArea holds the first)
		float m_fAlphaScale;            // 0x13C, GetParticleColor scales the alpha by it
		float m_fAreaScaleXZ;           // 0x140
		float m_fAreaScaleY;            // 0x144
		float m_fFriction;              // 0x148, "friction"
		float m_fGroundFriction;        // 0x14C, "groundfriction"
		WVector m_gravity;              // 0x150, "gravity"
		WVector m_ground;               // 0x15C, "ground"
		unsigned long m_srcBlend;       // 0x168, "blend": 0 normal, 0x800000 add, 0x1000000 multiply, 0x1800000 invmultiply
		unsigned long m_dstBlend;       // 0x16C, "blend"'s second value
		int m_lightSource;              // 0x170, SetLight: 0 white, 1 diffuse, 2 ambient, 3 ambient2
		unsigned long m_lightColor;     // 0x174, SetLight's pick from the LightSet
		int m_initFrame[2];             // 0x178, "init_frame"
		int m_bTail;                    // 0x180, MakeTail: particles leave a tail
		float m_fTailInterval;          // 0x184, MakeTail: time between two tail points
		unsigned char m_tailBytes[4];   // 0x188, "tail": arguments 2 to 5
		char m_tailTexture[0x104];      // 0x18C, "tail": the texture file
		int m_hTexture;                 // 0x290, texture handle of g_resrcmng; MakeTail builds a bandana strip when set
		float m_fBandanaWidth;          // 0x294, MakeTail scales the particles' strip width by it
		bool m_bMonolith;               // 0x298, "tail ... monolith"
		bool m_bBandanaFaceCamera;      // 0x299, MakeTail turns the strip to the box's camera
		UNKNOWN(0x29A, 0x29C);
		WVector m_initialMinVel;        // 0x29C, InitialMinVel
		WVector m_initialMaxVel;        // 0x2A8, InitialMaxVel
		float m_fWindScaleA;            // 0x2B4, SetWind keeps the wind only when one is non-zero
		float m_fWindScaleB;            // 0x2B8
		WVector m_wind;                 // 0x2BC, SetWind
		int m_extOrientation;           // 0x2C8, "ext_orientation": bits 1, 2, 4 from its three integers
		std::list<sVel> m_vels;         // 0x2CC
		float m_initAngle[6];           // 0x2D8, "init_angle"
		float m_initRotation[6];        // 0x2F0, "init_rotation"
		std::list<sRot> m_rots;         // 0x308
		std::list<sNozzleRot> m_nozzleRots;   // 0x314, RotateNozzle
		std::list<sNozzleRot> m_axisRots;     // 0x320, Rotate
		std::list<sRevol> m_revols;           // 0x32C, Revolute
		std::list<sRevol> m_grandRevols;      // 0x338, RevoluteGrand
		std::list<sGen> m_gens;         // 0x344
		std::list<sGravity> m_gravities;      // 0x350
		std::list<sFade> m_fades;       // 0x35C
		float m_initSize[3];            // 0x368, "init_size"
		std::list<sSize> m_sizes;       // 0x374
		std::list<sFrame> m_frames;     // 0x380
		std::list<sQuake> m_quakes;     // 0x38C
		std::list<sFlash> m_flashes;    // 0x398
		float m_fDistLimitA;            // 0x3A4, SetDistLimit
		float m_fDistLimitB;            // 0x3A8
		std::list<sBubble> m_bubbles;   // 0x3AC
		std::list<sSpawn> m_spawns;     // 0x3B8
		std::list<sSound> m_sounds;     // 0x3C4
		bool m_bSphereKind2;            // 0x3D0, "issphere 2"
		UNKNOWN(0x3D1, 0x3D8);
		float m_fSphereAngle;           // 0x3D8, "issphere 2": InitParticle's next angle (degrees) on the ring; zeroed by Reset
	};

protected:
	bool m_bAlive;                  // 0x4, ClearSpray(true) keeps the spray while false
	UNKNOWN(0x5, 0x8);
	const Attacher* m_pAttacher;    // 0x8, CFx::Attach's, or its sequence's when the sequence goes
	float m_fAttachLimitA;          // 0xC, Attacher::m_fLimitA (CFx::Attach), FLT_MAX by default
	float m_fAttachLimitB;          // 0x10, Attacher::m_fLimitB
	const WMatrix* m_pMatrix[4];    // 0x14, attachment matrices
	const WVector* m_pOffset[4];    // 0x24, attachment offsets
	defines_t m_defines;            // 0x34, cleared by CFxBox::LoadSpray
	char m_name[0x40];              // 0x10B8, ParseFile: the ".spr" file name
	CFxBox& m_box;                  // 0x10F8, the box that owns it
	std::list<CFxParticle*> m_particles;   // 0x10FC, deleted by the destructor
	CFxSequence* m_pParent;         // 0x1108, the sequence that opened it
	bool m_bActive;                 // 0x110C, SetActive
	PADDING(0x110D, 0x1110);
	float m_fTime;                  // 0x1110, the spray's age, zeroed by Reset, advanced by Process
	WVector m_outputDir;            // 0x1114, CFxBox::GetOutput adds it, scaled by distance
	WVector m_outputPos;            // 0x1120, CFxBox::GetOutput's reference point
	unsigned char m_outputAlpha;    // 0x112C, CFxBox::GetOutput adds it (clamped to 255)
	PADDING(0x112D, 0x1130);
	int m_displayedCount;           // 0x1130, summed by CFxBox::Display
	int m_displayedCount2;          // 0x1134, summed by CFxBox::Display
public:
	WVector m_pos;                  // 0x1138, moved by its sequence's position when detached
protected:
	WVector m_drawBgPos;            // 0x1144, Display: the drawn particle's GetBgPos
	WVector m_spawnDir;             // 0x1150, Spawn: the spawning particle's m_dir; the "spray" line's direction
	CFxSpray* m_pSpawner;           // 0x115C, the spray whose particle spawned it; GetBgPos keeps m_pos then
	WMatrix m_matrix;               // 0x1160, GetBgRot, GetBgPos's m_pos rotation
	WMatrix m_baseMatrix;           // 0x1190, identity after Reset
	Param m_param;                  // 0x11C0
};
CHECK_SIZE(CFxSpray::sVel, 0x10);
CHECK_FIELD(CFxSpray::sVel, t1, 0x8);
CHECK_FIELD(CFxSpray::sVel, v1, 0xC);
CHECK_SIZE(CFxSpray::sRot, 0x10);
CHECK_SIZE(CFxSpray::sNozzleRot, 0x1C);
CHECK_FIELD(CFxSpray::sNozzleRot, t0, 0xC);
CHECK_FIELD(CFxSpray::sNozzleRot, a0, 0x10);
CHECK_FIELD(CFxSpray::sNozzleRot, t1, 0x14);
CHECK_FIELD(CFxSpray::sNozzleRot, a1, 0x18);
CHECK_SIZE(CFxSpray::sRevol, 0x28);
CHECK_FIELD(CFxSpray::sRevol, axis, 0xC);
CHECK_FIELD(CFxSpray::sRevol, t0, 0x18);
CHECK_FIELD(CFxSpray::sRevol, speed0, 0x1C);
CHECK_FIELD(CFxSpray::sRevol, t1, 0x20);
CHECK_FIELD(CFxSpray::sRevol, speed1, 0x24);
CHECK_SIZE(CFxSpray::sGen, 0x18);
CHECK_FIELD(CFxSpray::sGen, spread, 0xC);
CHECK_FIELD(CFxSpray::sGen, lastTime, 0x14);
CHECK_SIZE(CFxSpray::sGravity, 0x18);
CHECK_SIZE(CFxSpray::sFade, 0x10);
CHECK_SIZE(CFxSpray::sSize, 0x10);
CHECK_SIZE(CFxSpray::sFrame, 0x14);
CHECK_SIZE(CFxSpray::sQuake, 0x34);
CHECK_SIZE(CFxSpray::sFlash, 0xC);
CHECK_SIZE(CFxSpray::sBubble, 0x18);
CHECK_SIZE(CFxSpray::sSpawn, 0x114);
CHECK_FIELD(CFxSpray::sSpawn, m_name, 0x4);
CHECK_FIELD(CFxSpray::sSpawn, m_bFollow, 0x108);
CHECK_FIELD(CFxSpray::sSpawn, m_fParam, 0x10C);
CHECK_FIELD(CFxSpray::sSpawn, m_intervalCount, 0x110);
CHECK_SIZE(CFxSpray::sSound, 0x108);
CHECK_SIZE(CFxSpray::Param, 0x3DC);
CHECK_FIELD(CFxSpray::Param, m_bSurvivesSequence, 0x0);
CHECK_FIELD(CFxSpray::Param, m_bFollowSpawner, 0x1);
CHECK_FIELD(CFxSpray::Param, m_bTransformPos, 0x3);
CHECK_FIELD(CFxSpray::Param, m_flags, 0x4);
CHECK_FIELD(CFxSpray::Param, m_bRoundArea, 0x111);
CHECK_FIELD(CFxSpray::Param, m_area, 0x114);
CHECK_FIELD(CFxSpray::Param, m_bScaleArea, 0x134);
CHECK_FIELD(CFxSpray::Param, m_fAlphaScale, 0x13C);
CHECK_FIELD(CFxSpray::Param, m_fAreaScaleXZ, 0x140);
CHECK_FIELD(CFxSpray::Param, m_fAreaScaleY, 0x144);
CHECK_FIELD(CFxSpray::Param, m_bTail, 0x180);
CHECK_FIELD(CFxSpray::Param, m_fTailInterval, 0x184);
CHECK_FIELD(CFxSpray::Param, m_fBandanaWidth, 0x294);
CHECK_FIELD(CFxSpray::Param, m_bBandanaFaceCamera, 0x299);
CHECK_FIELD(CFxSpray::Param, m_lightSource, 0x170);
CHECK_FIELD(CFxSpray::Param, m_lightColor, 0x174);
CHECK_FIELD(CFxSpray::Param, m_hTexture, 0x290);
CHECK_FIELD(CFxSpray::Param, m_initialMinVel, 0x29C);
CHECK_FIELD(CFxSpray::Param, m_initialMaxVel, 0x2A8);
CHECK_FIELD(CFxSpray::Param, m_fWindScaleA, 0x2B4);
CHECK_FIELD(CFxSpray::Param, m_fWindScaleB, 0x2B8);
CHECK_FIELD(CFxSpray::Param, m_wind, 0x2BC);
CHECK_FIELD(CFxSpray::Param, m_vels, 0x2CC);
CHECK_FIELD(CFxSpray::Param, m_rots, 0x308);
CHECK_FIELD(CFxSpray::Param, m_nozzleRots, 0x314);
CHECK_FIELD(CFxSpray::Param, m_axisRots, 0x320);
CHECK_FIELD(CFxSpray::Param, m_revols, 0x32C);
CHECK_FIELD(CFxSpray::Param, m_grandRevols, 0x338);
CHECK_FIELD(CFxSpray::Param, m_gens, 0x344);
CHECK_FIELD(CFxSpray::Param, m_gravities, 0x350);
CHECK_FIELD(CFxSpray::Param, m_fades, 0x35C);
CHECK_FIELD(CFxSpray::Param, m_sizes, 0x374);
CHECK_FIELD(CFxSpray::Param, m_frames, 0x380);
CHECK_FIELD(CFxSpray::Param, m_quakes, 0x38C);
CHECK_FIELD(CFxSpray::Param, m_flashes, 0x398);
CHECK_FIELD(CFxSpray::Param, m_fDistLimitA, 0x3A4);
CHECK_FIELD(CFxSpray::Param, m_fDistLimitB, 0x3A8);
CHECK_FIELD(CFxSpray::Param, m_bubbles, 0x3AC);
CHECK_FIELD(CFxSpray::Param, m_spawns, 0x3B8);
CHECK_FIELD(CFxSpray::Param, m_sounds, 0x3C4);
CHECK_FIELD(CFxSpray::Param, m_bSphereKind2, 0x3D0);
CHECK_FIELD(CFxSpray::Param, m_fSphereAngle, 0x3D8);
CHECK_SIZE(CFxSpray, 0x159C);
BEGIN_LAYOUT_CHECK(CFxSpray)
	CHECK_FIELD(CFxSpray, m_bAlive, 0x4);
	CHECK_FIELD(CFxSpray, m_defines, 0x34);
	// m_box (0x10F8) is a reference: offsetof cannot check it.
	CHECK_FIELD(CFxSpray, m_particles, 0x10FC);
	CHECK_FIELD(CFxSpray, m_name, 0x10B8);
	CHECK_FIELD(CFxSpray, m_pParent, 0x1108);
	CHECK_FIELD(CFxSpray, m_bActive, 0x110C);
	CHECK_FIELD(CFxSpray, m_displayedCount, 0x1130);
	CHECK_FIELD(CFxSpray, m_displayedCount2, 0x1134);
	CHECK_FIELD(CFxSpray, m_pAttacher, 0x8);
	CHECK_FIELD(CFxSpray, m_fAttachLimitA, 0xC);
	CHECK_FIELD(CFxSpray, m_pMatrix, 0x14);
	CHECK_FIELD(CFxSpray, m_pOffset, 0x24);
	CHECK_FIELD(CFxSpray, m_outputDir, 0x1114);
	CHECK_FIELD(CFxSpray, m_outputPos, 0x1120);
	CHECK_FIELD(CFxSpray, m_outputAlpha, 0x112C);
	CHECK_FIELD(CFxSpray, m_pos, 0x1138);
	CHECK_FIELD(CFxSpray, m_drawBgPos, 0x1144);
	CHECK_FIELD(CFxSpray, m_spawnDir, 0x1150);
	CHECK_FIELD(CFxSpray, m_pSpawner, 0x115C);
	CHECK_FIELD(CFxSpray, m_matrix, 0x1160);
	CHECK_FIELD(CFxSpray, m_fTime, 0x1110);
	CHECK_FIELD(CFxSpray, m_baseMatrix, 0x1190);
	CHECK_FIELD(CFxSpray, m_param, 0x11C0);
END_LAYOUT_CHECK

#include "fxspray.inl"

#endif // PG_CLIENT_PROJECTG_FXSPRAY_H
