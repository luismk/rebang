// Original: d:\build\custom\temp\code\client\projectg\particle.h
#ifndef PG_CLIENT_PROJECTG_PARTICLE_H
#define PG_CLIENT_PROJECTG_PARTICLE_H

#include <list>
#include <vector>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wmath.h"
#include "wlist.h"
class IActor;    // actor.h names it in MsgObject before declaring it
#include "actor.h"
#include "RTTI.h"
#include "scenemanager.h"
#include "../client/Wangreal/include/wtypes.h"

class W3dAniSpr;
class CGolfDoc;
class CBgEffect;
class CParticle;
class IObject;
class Bitmap;

// Particle emitter loaded from a particle definition file.
class CParticleRecv {
	LAYOUT_CHECK_FRIEND;
	friend class CParticleEffect;   // Clear resets m_pDoc
public:
	// One particle; 0x3c bytes (new sFrag[] in the constructor).
	struct sFrag {
		WVector pos;                // 0x0 (a WVector-like member: the implicit ctor is not trivial)
		WVector vel;                // 0xc, units per second
		float alpha;                // 0x18, 0..255, scaled by m_alpha
		bool alive;                 // 0x1c
		float angle;                // 0x20, sprite rotation (W3dAniSpr::Render)
		float spin;                 // 0x24, angle change per second
		float size;                 // 0x28, sprite width and height
		int sprNum;                 // 0x2c, sprite frame (W3dAniSpr::Render)
		float frameTime;            // 0x30, time left before the next sprite frame
		float life;                 // 0x34, lifetime given by Generate
		float lifeLeft;             // 0x38
	};

	CParticleRecv(const char* fileName);
	~CParticleRecv(void);

	void SetTarget(WVector* pTarget);
	void Process(float elapsed);
	void Display(void);
	void SetBox(const Waabb& box);
	static void Disconnect(void);
	// particle.h:124
	bool IsActive(void) { return m_active; }

protected:
	void Destroy(void);
	void ApplyScale(void);
	void Generate(void);
	void BlowAway(float elapsed, int index);

	int m_emitShape;                // 0x0, Generate's switch (0: from the emission box)
	float m_alphaStart;             // 0x4
	float m_alphaEnd;               // 0x8
	float m_sizeStart;              // 0xc, 0xc..0x24 are lengths, multiplied by m_scale
	float m_sizeEnd;                // 0x10
	float m_speedMin;               // 0x14
	float m_speedMax;               // 0x18
	float m_distMin;                // 0x1c, random in [min, max] in Generate
	float m_distMax;                // 0x20
	float m_spinMin;                // 0x24
	float m_spinMax;                // 0x28
	float m_gravity;                // 0x2c
	float m_lifeMin;                // 0x30
	float m_lifeMax;                // 0x34
	float m_frameInterval;          // 0x38, seconds per sprite frame (none when <= 0)
	float m_interval;               // 0x3c, seconds between two emissions
	float m_bounce;                 // 0x40, speed kept when bouncing on m_ground (none when <= 0)
	float m_emitTime;               // 0x44, emission time left (counts down while > 0)
	float m_ground;                 // 0x48, height the fragments bounce on
	int m_renderType;               // 0x4c, W3dAniSpr::Render type (0x400000 added)
	bool m_active;                  // 0x50
	PADDING(0x51, 0x54);
	sFrag* m_frags;                 // 0x54
	int m_liveCount;                // 0x58, alive fragments
	W3dAniSpr* m_aniSpr;            // 0x5c
	WMatrix m_dirMatrix;            // 0x60, orientation of the emitted velocities (Generate)
	WVector m_dirAngles;            // 0x90, reset with m_dirMatrix
	WVector m_accel;                // 0x9c, units per second added to the position
	Waabb m_box;                    // 0xa8, emission box (SetBox scales the parameters to it)
	float m_spread;                 // 0xc0, Generate's second shape
	bool m_randomFrame;             // 0xc4, start on a random sprite frame
	PADDING(0xc5, 0xc8);
	float m_spriteSize;             // 0xc8, Get3DAniSpr size
	int m_startFrame;               // 0xcc, first sprite frame unless m_randomFrame
	char m_texName[0x20];           // 0xd0, Get3DAniSpr name
	int m_fragCount;                // 0xf0, entries of m_frags
	int m_nextFrag;                 // 0xf4, next entry of m_frags to emit
	WVector* m_pTarget;             // 0xf8, the followed position (m_center when none)
	WVector m_center;               // 0xfc
	float m_timer;                  // 0x108, time accumulated towards the next emission
	int m_emitCount;                // 0x10c, fragments per emission
	bool m_visible;                 // 0x110
	PADDING(0x111, 0x114);
	int m_objIndex;                 // 0x114, course object whose alpha the effect takes
	float m_alpha;                  // 0x118, multiplies each fragment's alpha
	float m_scale;                  // 0x11c

private:
	static CGolfDoc* m_pDoc;
};
CHECK_SIZE(CParticleRecv::sFrag, 0x3c);
CHECK_FIELD(CParticleRecv::sFrag, pos, 0x0);
CHECK_FIELD(CParticleRecv::sFrag, vel, 0xc);
CHECK_FIELD(CParticleRecv::sFrag, alpha, 0x18);
CHECK_FIELD(CParticleRecv::sFrag, spin, 0x24);
CHECK_FIELD(CParticleRecv::sFrag, frameTime, 0x30);
CHECK_FIELD(CParticleRecv::sFrag, life, 0x34);
CHECK_FIELD(CParticleRecv::sFrag, lifeLeft, 0x38);
CHECK_FIELD(CParticleRecv::sFrag, alive, 0x1c);
CHECK_FIELD(CParticleRecv::sFrag, angle, 0x20);
CHECK_FIELD(CParticleRecv::sFrag, size, 0x28);
CHECK_FIELD(CParticleRecv::sFrag, sprNum, 0x2c);
BEGIN_LAYOUT_CHECK(CParticleRecv)
	CHECK_FIELD(CParticleRecv, m_emitShape, 0x0);
	CHECK_FIELD(CParticleRecv, m_alphaStart, 0x4);
	CHECK_FIELD(CParticleRecv, m_sizeStart, 0xc);
	CHECK_FIELD(CParticleRecv, m_distMax, 0x20);
	CHECK_FIELD(CParticleRecv, m_gravity, 0x2c);
	CHECK_FIELD(CParticleRecv, m_lifeMax, 0x34);
	CHECK_FIELD(CParticleRecv, m_frameInterval, 0x38);
	CHECK_FIELD(CParticleRecv, m_bounce, 0x40);
	CHECK_FIELD(CParticleRecv, m_ground, 0x48);
	CHECK_FIELD(CParticleRecv, m_dirMatrix, 0x60);
	CHECK_FIELD(CParticleRecv, m_dirAngles, 0x90);
	CHECK_FIELD(CParticleRecv, m_accel, 0x9c);
	CHECK_FIELD(CParticleRecv, m_spread, 0xc0);
	CHECK_FIELD(CParticleRecv, m_randomFrame, 0xc4);
	CHECK_FIELD(CParticleRecv, m_spriteSize, 0xc8);
	CHECK_FIELD(CParticleRecv, m_startFrame, 0xcc);
	CHECK_FIELD(CParticleRecv, m_texName, 0xd0);
	CHECK_FIELD(CParticleRecv, m_renderType, 0x4c);
	CHECK_FIELD(CParticleRecv, m_active, 0x50);
	CHECK_FIELD(CParticleRecv, m_box, 0xa8);
	CHECK_FIELD(CParticleRecv, m_fragCount, 0xf0);
	CHECK_FIELD(CParticleRecv, m_visible, 0x110);
	CHECK_FIELD(CParticleRecv, m_alpha, 0x118);
	CHECK_FIELD(CParticleRecv, m_frags, 0x54);
	CHECK_FIELD(CParticleRecv, m_aniSpr, 0x5c);
	CHECK_FIELD(CParticleRecv, m_pTarget, 0xf8);
	CHECK_FIELD(CParticleRecv, m_center, 0xfc);
	CHECK_FIELD(CParticleRecv, m_scale, 0x11c);
	CHECK_FIELD(CParticleRecv, m_interval, 0x3c);
	CHECK_FIELD(CParticleRecv, m_emitTime, 0x44);
	CHECK_FIELD(CParticleRecv, m_liveCount, 0x58);
	CHECK_FIELD(CParticleRecv, m_nextFrag, 0xf4);
	CHECK_FIELD(CParticleRecv, m_timer, 0x108);
	CHECK_FIELD(CParticleRecv, m_emitCount, 0x10c);
	CHECK_FIELD(CParticleRecv, m_objIndex, 0x114);
END_LAYOUT_CHECK

// Owner of the particle textures and of the live particles of an effect.
class CParticleManager {
	LAYOUT_CHECK_FRIEND;
public:
	// A texture loaded by RegisterTexture; 0x24 bytes.
	struct sTexture {
		// particle.h:98
		sTexture(void) : m_handle(0) {}

		char m_fileName[0x20];      // 0x0
		int m_handle;               // 0x20, WResourceManager texture handle
	};

	CParticleManager(void);
	~CParticleManager(void);

	void Process(float elapsed);
	void Display(void);
	void DisplayShadow(void);
	void MakeParticle(CParticle* pParticle, const char* texName, bool bPermanent);
	void RemoveAll(void);
	void Remove(const char* texName);

protected:
	void RegisterTexture(const char* name, const char* fileName);

	CParticle* m_particles[20];         // 0x0
	int m_lastIndex;                    // 0x50, highest used slot (-1: none)
	int m_count;                        // 0x54, live particles
	int m_permanentCount;               // 0x58, the first slots, kept by RemoveAll
	WList<sTexture*> m_textures;        // 0x5c
};
CHECK_SIZE(CParticleManager::sTexture, 0x24);
CHECK_FIELD(CParticleManager::sTexture, m_handle, 0x20);
CHECK_SIZE(CParticleManager, 0x7c);
BEGIN_LAYOUT_CHECK(CParticleManager)
	CHECK_FIELD(CParticleManager, m_lastIndex, 0x50);
	CHECK_FIELD(CParticleManager, m_count, 0x54);
	CHECK_FIELD(CParticleManager, m_permanentCount, 0x58);
	CHECK_FIELD(CParticleManager, m_textures, 0x5c);
END_LAYOUT_CHECK

IObject* CParticleEffectMakeInstance(void);

// Actor that plays the particle effects of a scene.
class CParticleEffect : public IActor, public CRenderFuncPtr {
	LAYOUT_CHECK_FRIEND;
	friend IObject* CParticleEffectMakeInstance(void);   // the factory creates it
public:
	static const WRTTI m_RTTI;

	virtual const WRTTI* GetRTTI(void) const;
	// particle.h:24
	virtual ~CParticleEffect(void) {}
	virtual void OnPreLoadInit(void);
	virtual void OnInit(void);
	virtual void OnProcess(float elapsed);
	virtual void OnDestroy(void);
	virtual void HandleMsg(const MsgObject& msg);
	virtual void DisplayShadow(void);
	virtual void Display(void);

	bool CheckExtShotEffect(const char* sprayName, const char* sfxName);
	void CallSprayParticle(const char* sprayName, const char* sfxName, const WVector* pPos);

protected:
	CParticleEffect(void);

	void Clear(void);

	WList<CParticleRecv*> m_recvList;   // 0x1c
	CParticleManager* m_pManager;       // 0x3c
	PADDING(0x40, 0x54);
	WList<CBgEffect*> m_bgEffectList;   // 0x54
	CParticle* m_pParticle;             // 0x74, a CParticle subclass (HandleMsg writes its 0xe4)
};
CHECK_SIZE(CParticleEffect, 0x78);
BEGIN_LAYOUT_CHECK(CParticleEffect)
	CHECK_FIELD(CParticleEffect, m_recvList, 0x1c);
	CHECK_FIELD(CParticleEffect, m_pManager, 0x3c);
	CHECK_FIELD(CParticleEffect, m_bgEffectList, 0x54);
	CHECK_FIELD(CParticleEffect, m_pParticle, 0x74);
END_LAYOUT_CHECK

// Screen-space particle effect of the dialogs (list of sParticle, bitmaps
// from Fresh).
class CParticle2D {
	LAYOUT_CHECK_FRIEND;
public:
	// One particle; 0x44 bytes (new sParticle in Process). Its implicit
	// ctor is not trivial (a member with a user ctor, not transcribed yet).
	enum eSTYLE {
		STYLE_NORMAL = 0,           // full alpha
		STYLE_BLINK = 1             // alpha swings between 125 and 255 (names invented)
	};

	// One particle; 0x44 bytes (new sParticle in Process). Its implicit
	// ctor is not trivial (a member with a user ctor).
	struct sParticle {
		WVector2D m_pos;            // 0x0, screen position of the centre
		WVector2D m_vel;            // 0x8, direction, scaled by m_speed
		const Bitmap* m_pBitmap;    // 0x10
		float m_alpha;              // 0x14, 0..255
		int m_fadeIn;               // 0x18, alpha rising (STYLE_BLINK)
		float m_speed;              // 0x1c
		int m_alive;                // 0x20, 0: deleted by Process
		WRect m_src;                // 0x24, source rectangle in the bitmap
		WRect m_dest;               // 0x34, screen rectangle
	};

	~CParticle2D(void);

	int Initialize(const std::list<std::string>& bitmapNames, const WRect& rect, unsigned int maxCount, eSTYLE style, float speedMin, float speedMax, const WVector2D& velMin, const WVector2D& velMax);
	void Process(float elapsed);
	void Display(void);
	void Release(void);

private:
	void MakeDisplayRect(sParticle* pParticle);

protected:
	WRect m_rect;                           // 0x0, screen area the particles are clipped to
	eSTYLE m_style;                         // 0x10
	unsigned int m_maxCount;                // 0x14, particles kept alive
	float m_speedMin;                       // 0x18
	float m_speedMax;                       // 0x1c
	WVector2D m_velMin;                     // 0x20
	WVector2D m_velMax;                     // 0x28
	std::list<sParticle*> m_particles;      // 0x30
	std::vector<const Bitmap*> m_bitmaps;   // 0x3c
};
CHECK_SIZE(CParticle2D::sParticle, 0x44);
CHECK_FIELD(CParticle2D::sParticle, m_pos, 0x0);
CHECK_FIELD(CParticle2D::sParticle, m_vel, 0x8);
CHECK_FIELD(CParticle2D::sParticle, m_pBitmap, 0x10);
CHECK_FIELD(CParticle2D::sParticle, m_fadeIn, 0x18);
CHECK_FIELD(CParticle2D::sParticle, m_speed, 0x1c);
CHECK_FIELD(CParticle2D::sParticle, m_alive, 0x20);
CHECK_FIELD(CParticle2D::sParticle, m_alpha, 0x14);
CHECK_FIELD(CParticle2D::sParticle, m_src, 0x24);
CHECK_FIELD(CParticle2D::sParticle, m_dest, 0x34);
BEGIN_LAYOUT_CHECK(CParticle2D)
	CHECK_FIELD(CParticle2D, m_rect, 0x0);
	CHECK_FIELD(CParticle2D, m_style, 0x10);
	CHECK_FIELD(CParticle2D, m_maxCount, 0x14);
	CHECK_FIELD(CParticle2D, m_speedMin, 0x18);
	CHECK_FIELD(CParticle2D, m_velMin, 0x20);
	CHECK_FIELD(CParticle2D, m_velMax, 0x28);
	CHECK_FIELD(CParticle2D, m_particles, 0x30);
	CHECK_FIELD(CParticle2D, m_bitmaps, 0x3c);
END_LAYOUT_CHECK

#endif
