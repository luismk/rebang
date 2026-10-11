// Original: d:\build\custom\temp\code\client\projectg\soundmanager.h
#ifndef PG_CLIENT_PROJECTG_SOUNDMANAGER_H
#define PG_CLIENT_PROJECTG_SOUNDMANAGER_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wlist.h"
#include "wmath.h"

// A sound emitter whose position LoadSfx's flag-4 effects follow
// (CSoundManager::Process). Only the position is known; the full size is not.
class CSoundObject {
public:
	// soundmanager.h:29
	WVector GetPos(void) const { return m_pos; }

protected:
	WVector m_pos;  // 0x0

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CSoundObject)
	CHECK_FIELD(CSoundObject, m_pos, 0x0);
END_LAYOUT_CHECK

class WView;
class WMilesSoundSystem;

// One loaded sound effect (operator new 0x80 in CSoundManager::LoadSfx).
// Only the members the decompiled code reads are transcribed.
struct _SFX_INFO {
	char m_name[0x40];   // 0x0, the sound file
	char m_alias[0x20];  // 0x40
	int m_hSound;  // 0x60, the sample's handle in the WMilesSoundSystem
	unsigned long m_startTick;  // 0x64, GetTickCount() when LoadSfx (re)used it
	WVector m_pos;              // 0x68, the 3D position
	bool m_b3D;                 // 0x74, played as a 3D sample
	unsigned char m_state;  // 0x75, 1: reusable while playing (FindIdleSfx)
	bool m_bPlaying;        // 0x76
	UNKNOWN(0x77, 0x78);
	unsigned long m_flags;  // 0x78, the LoadSfx flags
	const CSoundObject* m_pOwner;  // 0x7C, the owner when flag 4 is set

	// soundmanager.h:65
	bool IsSet(unsigned long flag) {
		return (m_flags & flag) != 0;
	}
};

CHECK_SIZE(_SFX_INFO, 0x80);
CHECK_FIELD(_SFX_INFO, m_name, 0x0);
CHECK_FIELD(_SFX_INFO, m_alias, 0x40);
CHECK_FIELD(_SFX_INFO, m_hSound, 0x60);
CHECK_FIELD(_SFX_INFO, m_startTick, 0x64);
CHECK_FIELD(_SFX_INFO, m_pos, 0x68);
CHECK_FIELD(_SFX_INFO, m_b3D, 0x74);
CHECK_FIELD(_SFX_INFO, m_state, 0x75);
CHECK_FIELD(_SFX_INFO, m_bPlaying, 0x76);
CHECK_FIELD(_SFX_INFO, m_flags, 0x78);
CHECK_FIELD(_SFX_INFO, m_pOwner, 0x7C);

// The game's sound manager. Only the methods are declared; the virtual ones
// follow the original vtable order (??_7CSoundManager@@6B@).
class CSoundManager {
public:
	CSoundManager(void);
	virtual ~CSoundManager(void);                                       // slot 0
	virtual void Process(float fDelta, WView* pView);                   // slot 1
	virtual _SFX_INFO* LoadSfx(const char* name, const WVector* pPos, unsigned long flags, const char* alias, const CSoundObject* pOwner, float volume, float range);  // slot 2
	virtual void PlaySfx(const char* name, const WVector* pPos, unsigned long flags, const char* alias, const CSoundObject* pOwner, float volume, float range);         // slot 3
	virtual void StopSfx(const char* alias);                            // slot 4
	virtual void PlayBGM(const char* name, bool bOnce, bool bFade);     // slot 5
	virtual void StopBGM(bool bFade);                                   // slot 6
	virtual void PauseBGM(bool bFade);                                  // slot 7
	virtual void AllDelSfx(void);                                       // slot 8

	void SetMSS(WMilesSoundSystem* pMSS);
	WMilesSoundSystem* GetMSS(void) const;
	bool IsPlayingBGM(void);
	float GetVolume(void) const;
	float GetBGMVolume(void) const;
	void SetVolume(float volume);
	void SetBGMVolume(float volume);

	bool IsPlaying(const char* alias);
	void SetSFXVolumeWhilePlaying(const char* alias, float volume);
	void SetSpeakerType(int type);
	void StopAllSound(void);

	void Init(void);
	int GetSoundFx(char* name, bool bStream);
	void Destroy(void);
	void Activate(bool active);
	void SetBGMVolumeInBGMArea(float volume);

	// soundmanager.h:126
	void SetFadeOutTime(float time) { m_fadeOutTime = time; }

protected:
	virtual void AddSfx(_SFX_INFO* pSfx);                               // slot 9
	virtual void DelSfx(const char* alias);                             // slot 11
	virtual void DelSfx(_SFX_INFO* pSfx);                               // slot 10

	void CheckIdleSfx(float fDelta);
	_SFX_INFO* FindIdleSfx(const char* name);
	_SFX_INFO* FindSfx(const char* name);
	_SFX_INFO* FindAliasSfx(const char* alias);
	void DestroyBGM(void);

	WMilesSoundSystem* m_pMSS;  // 0x4
	float m_volume;             // 0x8
	int m_hBGM;                 // 0xC, the playing BGM's handle in m_pMSS (0: none)
	int m_hFadeBGM;             // 0x10, the BGM fading out after StopBGM/PauseBGM (0: none)
	float m_fadeVolume;         // 0x14, the fading BGM's starting volume
	float m_fadeElapsed;        // 0x18, time spent fading
public:
	float m_fadeOutTime;        // 0x1C, fade-out duration (CTaskMain::SetRandomBGM/SetConstantBGM store it directly)
protected:
	float m_bgmVolume;          // 0x20
	char m_bgmName[0x40];       // 0x24, the playing BGM's name
	float m_nearDistance;       // 0x64, 128 by default
	float m_farDistance;        // 0x68, 160 by default
	float m_maxDistance;        // 0x6C, 960 by default
	float m_idleCheckTime;      // 0x70, time since the last CheckIdleSfx sweep
	bool m_bActive;             // 0x74
	WList<_SFX_INFO*> m_sfxList;   // 0x78, the loaded effects
	WList<_SFX_INFO*> m_idleList;  // 0x98

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CSoundManager)
	CHECK_FIELD(CSoundManager, m_pMSS, 0x4);
	CHECK_FIELD(CSoundManager, m_volume, 0x8);
	CHECK_FIELD(CSoundManager, m_hBGM, 0xC);
	CHECK_FIELD(CSoundManager, m_hFadeBGM, 0x10);
	CHECK_FIELD(CSoundManager, m_fadeVolume, 0x14);
	CHECK_FIELD(CSoundManager, m_fadeElapsed, 0x18);
	CHECK_FIELD(CSoundManager, m_fadeOutTime, 0x1C);
	CHECK_FIELD(CSoundManager, m_bgmVolume, 0x20);
	CHECK_FIELD(CSoundManager, m_bgmName, 0x24);
	CHECK_FIELD(CSoundManager, m_nearDistance, 0x64);
	CHECK_FIELD(CSoundManager, m_farDistance, 0x68);
	CHECK_FIELD(CSoundManager, m_maxDistance, 0x6C);
	CHECK_FIELD(CSoundManager, m_idleCheckTime, 0x70);
	CHECK_FIELD(CSoundManager, m_bActive, 0x74);
	CHECK_FIELD(CSoundManager, m_sfxList, 0x78);
	CHECK_FIELD(CSoundManager, m_idleList, 0x98);
END_LAYOUT_CHECK
CHECK_SIZE(CSoundManager, 0xB8);

extern CSoundManager* g_audio;

// The BGMs CTaskMain::SetRandomBGM picks from (?randomBGMList@@3PAPBDA).
extern const char* randomBGMList[];

#endif // PG_CLIENT_PROJECTG_SOUNDMANAGER_H
