// Original: d:\build\custom\temp\code\client\projectg\ambientsound.cpp
#define AMBIENTSOUND_WMATH_INLINE
#include "ambientsound.h"
#include "wmath.h"

#include <new>
#include <map>
#include <string>

#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "polysoup.h"
#include "SoundManager.h"
#include "wutil.h"

extern WMatrix g_camera;

// ambientsound.cpp:17 (run-time type and factory registration of CAmbientSound)
const WRTTI CAmbientSound::m_RTTI("CAmbientSound", &IActor::m_RTTI);

IObject* CAmbientSoundMakeInstance(void) {
	return new CAmbientSound;
}

struct __impCAmbientSound {
	__impCAmbientSound(void) {
		ObjectFactory().AddObjectFunctor(CAmbientSoundMakeInstance, "CAmbientSound");
		ActorFactory().AddActorClass("CAmbientSound");
	}
};
static __impCAmbientSound s_impCAmbientSound;

// ambientsound.cpp:20
CAmbientSound::CAmbientSound(void) {
	AddSkipList(0x85040);
}

// ambientsound.cpp:25
void CAmbientSound::OnInit(void) {
	std::list<sSoundBox>& boxes = GOLFDOC()->m_pPolySoup->m_soundBoxes;
	for (std::list<sSoundBox>::iterator it = boxes.begin(); it != boxes.end(); ++it) {

		if (it->m_interval < 0.0f && !g_audio->IsPlaying(it->m_name))
			g_audio->PlaySfx(it->m_name, NULL, 1, it->m_name, NULL, 0.5f, 200.0f);
	}
}

// ambientsound.cpp:35
float CAmbientSound::GetVolume(const Wobb& box, float range) {
	if (box.IsInclude(g_camera.pivot) || range < 0.0f)
		return 1.0f;

	float dist = 0.0f;
	WVector delta = (g_camera.pivot - box.center) * 0.5f;

	for (int i = 0; i < 3; i++) {


		float half = box.extend[i].Magnitude() * 0.5f;
		float proj = WDotProduct(delta, box.extend[i]) / half;

		if (proj < half * -1.0f) {

			proj += half;
			dist += proj * proj;

		} else if (proj > half) {

			proj -= half;
			dist += proj * proj;
		}
	}



	float volume = 1.0f - (float)sqrt(dist) / (range * 3.2f);
	if (0.0f < volume)
		return volume;

	return 0.0f;
}

// ambientsound.cpp:62
void CAmbientSound::OnProcess(float delta) {
	std::list<sSoundBox>& boxes = GOLFDOC()->m_pPolySoup->m_soundBoxes;

	std::map<std::string, float> volumes;


	volumes.clear();


	for (std::list<sSoundBox>::iterator it = boxes.begin(); it != boxes.end(); ++it) {
		float volume;
		if (it->m_interval > 0.0f) {

			it->m_elapsed += delta;

			if (it->m_elapsed > it->m_interval) {


				it->m_elapsed = 0.0f;
				g_audio->PlaySfx(it->m_name, NULL, 0, NULL, NULL, 0.5f, 200.0f);
			}

			if (g_audio->IsPlaying(it->m_name))
				volume = GetVolume(it->m_box, it->m_range);
			else
				volume = 0.0f;

		} else {

			volume = GetVolume(it->m_box, it->m_range);
		}


		std::map<std::string, float>::iterator found = volumes.find(it->m_name);
		if (found == volumes.end())
			volumes[it->m_name] = volume;
		else
			volumes[it->m_name] = Max(volumes[it->m_name], volume);
	}



	for (std::map<std::string, float>::iterator v = volumes.begin(); v != volumes.end(); ++v) {

		if (g_audio->IsPlaying(v->first.c_str()))
			g_audio->SetSFXVolumeWhilePlaying(v->first.c_str(), v->second);
	}
}

// ambientsound.cpp:109
void CAmbientSound::HandleMsg(const MsgObject& msg) {
	if (msg.m_msg == 500) {









		std::list<sSoundBox>& boxes = GOLFDOC()->m_pPolySoup->m_soundBoxes;
		for (std::list<sSoundBox>::iterator it = boxes.begin(); it != boxes.end(); ++it)
			g_audio->StopSfx(it->m_name);
	}
}

// actor.h:118
void IActor::OnPreLoadInit(void) {
}

// actor.h:119
void IActor::OnLoad(void) {
}

// actor.h:124
void IActor::OnDestroy(void) {
}

// actor.h:125
void IActor::OnPreserveBack(char const *) {
}

// actor.h:126
void IActor::OnRestorePreserved(void) {
}
