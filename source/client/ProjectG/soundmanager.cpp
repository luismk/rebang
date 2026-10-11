// Original: d:\build\custom\temp\code\client\projectg\soundmanager.cpp
#include "SoundManager.h"
#include "Miles.h"
#include "wmath.h"
#include "wutil.h"
#include "wview.h"

#include <math.h>

#include <stdio.h>
#include <string.h>
#include <windows.h>

// Definition of the global sound manager used throughout the codebase.
// Many objects reference `g_audio` (declared `extern` in SoundManager.h),
// so provide a single definition here.
CSoundManager* g_audio = NULL;

// soundmanager.cpp:26
CSoundManager::CSoundManager(void) : m_sfxList(16, 16), m_idleList(16, 16) {
	Init();
}

// soundmanager.cpp:31
CSoundManager::~CSoundManager(void) {
	Destroy();
}

// soundmanager.cpp:36
void CSoundManager::Init(void) {
	m_volume = 1.0f;
	m_bgmVolume = 1.0f;
	m_fadeVolume = 1.0f;
	m_fadeOutTime = 1.0f;
	m_pMSS = NULL;
	m_bActive = true;
	m_idleCheckTime = 0;
	m_hBGM = 0;
	m_hFadeBGM = 0;
	m_fadeElapsed = 0;
	m_farDistance = 160.0f;
	m_nearDistance = 128.0f;
	m_maxDistance = 960.0f;
	memset(m_bgmName, 0, sizeof(m_bgmName));
}

// soundmanager.cpp:59
void CSoundManager::Destroy(void) {
	AllDelSfx();

	m_pMSS = NULL;
}

// soundmanager.cpp:66
void CSoundManager::SetMSS(WMilesSoundSystem* pMSS) {
	m_pMSS = pMSS;
}

// soundmanager.cpp:73
WMilesSoundSystem* CSoundManager::GetMSS(void) const {
	return m_pMSS;
}

// soundmanager.cpp:78
void CSoundManager::Activate(bool active) {
	m_bActive = active;
	if (m_pMSS) {
		m_pMSS->Activate(active);
	}
}

// soundmanager.cpp:85
void CSoundManager::Process(float fDelta, WView* pView) {
	float dist = 0;


	if (!m_pMSS) {
		return;
	}




	float maxDistSq = m_maxDistance * m_maxDistance;
	float range = m_farDistance - m_nearDistance;



	if (m_hFadeBGM) {

		m_fadeElapsed += fDelta;

		if (m_fadeElapsed > m_fadeOutTime) {

			m_pMSS->Stop(m_hFadeBGM);
			m_hFadeBGM = 0;
			m_fadeElapsed = 0;

		} else {

			m_pMSS->SetVolume(m_hFadeBGM, (1.0f - m_fadeElapsed / m_fadeOutTime) * m_fadeVolume);
		}
	}



	for (_SFX_INFO* pSfx = m_sfxList.Start(); pSfx; pSfx = m_sfxList.Next()) {


		if (pSfx->m_bPlaying == true && pSfx->m_b3D == true) {


			if (pSfx->m_flags & 4) {

				pSfx->m_pos = pSfx->m_pOwner->GetPos();
				WVector pos = pSfx->m_pos * pView->invcamera;
				m_pMSS->SetPosition(pSfx->m_hSound, pos.x, pos.y, pos.z);
			}






			if (!pSfx->IsSet(2) || !pSfx->m_state) {


				dist = Abs(pView->camera.pivot.y - pSfx->m_pos.y);

				if (dist >= m_nearDistance) {
					float volume = (m_farDistance - dist) / range * m_volume;
					m_pMSS->SetVolume(pSfx->m_hSound, 0.0f < volume ? volume : 0.0f);
				}
				WVector pos = pSfx->m_pos * pView->invcamera;
				m_pMSS->SetPosition(pSfx->m_hSound, pos.x, pos.y, pos.z);




				if (!pSfx->m_state) {

					WVector diff;
					diff = pView->camera.pivot - pSfx->m_pos;
					float distSq = diff.x * diff.x + diff.z * diff.z;


					if ((distSq < maxDistSq && dist < m_farDistance) || pSfx->IsSet(1)) {






						m_pMSS->Play(pSfx->m_hSound, pSfx->m_flags & 1, true);
						pSfx->m_state = 1;
					}
				}
			}
		}
	}



	CheckIdleSfx(fDelta);
}

// soundmanager.cpp:178
void CSoundManager::CheckIdleSfx(float fDelta) {
	if (!m_bActive) {
		return;
	}

	m_idleCheckTime += fDelta;

	if (m_idleCheckTime > 5.0f) {



		m_idleCheckTime = 0;
		unsigned long tick = GetTickCount();

		_SFX_INFO* pSfx;
		for (pSfx = m_sfxList.Start(); pSfx; pSfx = m_sfxList.Next()) {

			if (tick - pSfx->m_startTick > 50000 && m_pMSS && !m_pMSS->IsPlaying(pSfx->m_hSound)) {
				m_idleList += pSfx;
			}
		}

		for (pSfx = m_idleList.Start(); pSfx; pSfx = m_idleList.Next()) {

			DelSfx(pSfx);
		}


		m_idleList.Reset();
	}
}

// soundmanager.cpp:213
_SFX_INFO* CSoundManager::FindIdleSfx(const char* name) {
	_SFX_INFO* pSfx;


	for (pSfx = m_sfxList.Start(); pSfx; pSfx = m_sfxList.Next()) {








		if ((!pSfx->m_bPlaying || pSfx->m_state == 1) && !strcmpi(pSfx->m_name, name) && m_pMSS && !m_pMSS->IsPlaying(pSfx->m_hSound)) {

			return pSfx;
		}
	}



	return NULL;
}

// soundmanager.cpp:235
int CSoundManager::GetSoundFx(char* name, bool bStream) {
	if (!m_pMSS || !name) {
		return 0;
	}




	return m_pMSS->Load(name, bStream ? 1 : ((strstr(name, ".a") || strstr(name, ".mp3")) ? 2 : 0));
}

// The larger of two values, each argument evaluated as written.
#define SFX_MAX(a, b) ((a) > (b) ? (a) : (b))

// soundmanager.cpp:249
_SFX_INFO* CSoundManager::LoadSfx(const char* name, const WVector* pPos, unsigned long flags, const char* alias, const CSoundObject* pOwner, float volume, float range) {
	if (!m_pMSS) {
		return NULL;
	}

	bool b3D = pPos && flags != 5;

	_SFX_INFO* pSfx = FindIdleSfx(name);
	if (!pSfx) {


		char path[0x100];
		sprintf(path, "sound/%s.wav", name);
		int hSound = GetSoundFx(path, b3D);
		if (!hSound) {


			return NULL;
		}

		pSfx = new _SFX_INFO;
		if (!pSfx) {

			m_pMSS->DestroySoundBuffer(hSound);
			return NULL;
		}

		memset(pSfx, 0, sizeof(_SFX_INFO));
		strcpy(pSfx->m_name, name);
		pSfx->m_hSound = hSound;
		AddSfx(pSfx);
	}



	float vol = m_volume;

	memset(pSfx->m_alias, 0, sizeof(pSfx->m_alias));
	if (alias) {

		strcpy(pSfx->m_alias, alias);
	}


	pSfx->m_flags = flags;
	pSfx->m_bPlaying = true;
	pSfx->m_startTick = GetTickCount();


	if (pPos && flags != 5) {

		memcpy(&pSfx->m_pos, pPos, sizeof(WVector));
		pSfx->m_b3D = b3D;
		pSfx->m_state = !b3D;
	} else {




		pSfx->m_b3D = false;
		pSfx->m_state = 1;
		if (flags == 5) {
			vol *= SFX_MAX(volume, (range - (float)sqrt(pPos->x * pPos->x + pPos->y * pPos->y + pPos->z * pPos->z)) * (1.0f / range));
			pSfx->m_flags = flags = 0;
		}
	}
	pSfx->m_pOwner = (flags & 4) ? pOwner : NULL;
	m_pMSS->SetVolume(pSfx->m_hSound, vol);
	return pSfx;
}

// soundmanager.cpp:327
void CSoundManager::PlaySfx(const char* name, const WVector* pPos, unsigned long flags, const char* alias, const CSoundObject* pOwner, float volume, float range) {
	_SFX_INFO* pSfx = LoadSfx(name, pPos, flags, alias, pOwner, volume, range);
	if (pSfx) {


		if (pSfx->m_state == 1) {

			m_pMSS->Play(pSfx->m_hSound, pSfx->IsSet(1), true);
		}
	}
}

// soundmanager.cpp:341
void CSoundManager::StopSfx(const char* alias) {
	if (!m_pMSS) {
		return;
	}




	if (alias == NULL) {

		for (_SFX_INFO* pSfx = m_sfxList.Start(); pSfx; pSfx = m_sfxList.Next()) {

			if (pSfx->m_bPlaying) {

				m_pMSS->Stop(pSfx->m_hSound);
				pSfx->m_bPlaying = false;
			}
		}
	} else {





		_SFX_INFO* pSfx = FindAliasSfx(alias);

		if (pSfx && pSfx->m_hSound) {

			if (pSfx->m_bPlaying) {

				m_pMSS->Stop(pSfx->m_hSound);
				pSfx->m_bPlaying = false;
			}
		}
	}
}

// soundmanager.cpp:374
bool CSoundManager::IsPlayingBGM(void) {
	return m_hBGM != 0 ? true : false;
}

// soundmanager.cpp:379
bool CSoundManager::IsPlaying(const char* alias) {
	int len = strlen(alias);
	if (len == 0) {
		return false;
	}
	_SFX_INFO* pSfx = FindAliasSfx(alias);

	if (pSfx) {

		if (pSfx->m_hSound && m_pMSS) {
			return m_pMSS->IsPlaying(pSfx->m_hSound);
		}
	}

	if (!stricmp(alias, m_bgmName)) {
		return m_pMSS->IsPlaying(m_hBGM);
	}
	return false;
}

// soundmanager.cpp:398
_SFX_INFO* CSoundManager::FindSfx(const char* name) {
	_SFX_INFO* pSfx;


	for (pSfx = m_sfxList.Start(); pSfx; pSfx = m_sfxList.Next()) {

		if (!strcmpi(pSfx->m_name, name)) {
			return pSfx;
		}
	}

	return NULL;
}

// soundmanager.cpp:411
_SFX_INFO* CSoundManager::FindAliasSfx(const char* alias) {
	_SFX_INFO* pSfx;


	for (pSfx = m_sfxList.Start(); pSfx; pSfx = m_sfxList.Next()) {

		if (!strcmpi(pSfx->m_alias, alias)) {
			return pSfx;
		}
	}

	return NULL;
}

// soundmanager.cpp:424
void CSoundManager::AddSfx(_SFX_INFO* pSfx) {
	m_sfxList += pSfx;
}

// soundmanager.cpp:429
void CSoundManager::DelSfx(const char* alias) {
	_SFX_INFO* pSfx = FindAliasSfx(alias);
	if (pSfx) {
		DelSfx(pSfx);
	}
}

// soundmanager.cpp:436
void CSoundManager::DelSfx(_SFX_INFO* pSfx) {




	m_sfxList -= pSfx;
	if (m_pMSS) {

		m_pMSS->DestroySoundBuffer(pSfx->m_hSound);
	}

	delete pSfx;
}

// soundmanager.cpp:452
void CSoundManager::AllDelSfx(void) {
	_SFX_INFO* pSfx;

	while ((pSfx = m_sfxList.Start()) != NULL) {
		DelSfx(pSfx);
	}

	DestroyBGM();
}

// soundmanager.cpp:462
void CSoundManager::SetVolume(float volume) {
	m_volume = volume;


	if (m_pMSS) {



		for (_SFX_INFO* pSfx = m_sfxList.Start(); pSfx; pSfx = m_sfxList.Next()) {
			m_pMSS->SetVolume(pSfx->m_hSound, volume);
		}
	}
}

// soundmanager.cpp:476
float CSoundManager::GetVolume(void) const {
	return m_volume;
}

// soundmanager.cpp:481
void CSoundManager::SetSFXVolumeWhilePlaying(const char* alias, float volume) {
	_SFX_INFO* pSfx = FindAliasSfx(alias);
	if (pSfx && m_pMSS) {
		m_pMSS->SetVolume(pSfx->m_hSound, volume * m_volume);
	}
}

// soundmanager.cpp:489
void CSoundManager::PlayBGM(const char* name, bool bOnce, bool bFade) {
	if (!m_pMSS) {
		return;
	}

	if (name == NULL) {

		StopBGM(bFade);
		m_bgmName[0] = '\0';
	} else {









		if (m_hBGM && !stricmp(name, m_bgmName)) {


			if (!m_pMSS->IsPlaying(m_hBGM)) {
				m_pMSS->Resume(m_hBGM, !bOnce);
			}
			return;
		}




		int hBGM = GetSoundFx((char*)name, false);
		if (hBGM) {


			StopBGM(bFade);


			m_hBGM = hBGM;
			strcpy(m_bgmName, name);

			m_pMSS->Play(m_hBGM, !bOnce, true);

			SetBGMVolume(m_bgmVolume);
		}
	}
}

// soundmanager.cpp:532
void CSoundManager::StopBGM(bool bFade) {
	if (m_hBGM && m_pMSS) {

		if (bFade) {

			if (m_hFadeBGM) {

				m_pMSS->Stop(m_hFadeBGM);
			}



			m_hFadeBGM = m_hBGM;


			m_fadeVolume = m_bgmVolume;
			m_fadeElapsed = 0;
		} else {



			m_pMSS->Stop(m_hBGM);

			m_hFadeBGM = 0;
		}
		m_hBGM = 0;
	}
}

// soundmanager.cpp:558
void CSoundManager::PauseBGM(bool bFade) {
	if (m_hBGM && m_pMSS) {

		if (bFade) {

			if (m_hFadeBGM) {

				m_pMSS->Stop(m_hFadeBGM);
			}



			m_hFadeBGM = m_hBGM;


			m_fadeVolume = m_bgmVolume;
			m_fadeElapsed = 0;
		} else {



			m_pMSS->Stop(m_hBGM);
		}
	}
}

// soundmanager.cpp:581
void CSoundManager::SetBGMVolume(float volume) {
	m_bgmVolume = volume;
	if (m_hBGM && m_pMSS) {
		m_pMSS->SetVolume(m_hBGM, volume);
	}
}

// soundmanager.cpp:589
void CSoundManager::SetBGMVolumeInBGMArea(float volume) {
	if (m_hBGM && m_pMSS) {
		m_pMSS->SetVolume(m_hBGM, volume * m_bgmVolume);
	}
}

// soundmanager.cpp:595
float CSoundManager::GetBGMVolume(void) const {
	return m_bgmVolume;
}

// soundmanager.cpp:600
void CSoundManager::DestroyBGM(void) {
	if (m_hBGM) {

		if (m_pMSS) {
			m_pMSS->DestroySoundBuffer(m_hBGM);
		}
		m_hBGM = 0;
	}
}

// soundmanager.cpp:613
void CSoundManager::StopAllSound(void) {











	if (m_pMSS) {


		for (_SFX_INFO* pSfx = m_sfxList.Start(); pSfx; pSfx = m_sfxList.Next()) {

			if (pSfx->m_bPlaying) {

				m_pMSS->Stop(pSfx->m_hSound);
				pSfx->m_bPlaying = false;
			}


			delete pSfx;
		}


		m_sfxList.Reset();
	}
}

// soundmanager.cpp:644
void CSoundManager::SetSpeakerType(int type) {
	if (m_pMSS) {
		m_pMSS->SpeakerType((WMilesSoundSystem::w_speaker_type)type);
	}
}
