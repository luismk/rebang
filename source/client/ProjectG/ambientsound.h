// Original: d:\build\custom\temp\code\client\projectg\ambientsound.h
#ifndef PG_CLIENT_PROJECTG_AMBIENTSOUND_H
#define PG_CLIENT_PROJECTG_AMBIENTSOUND_H

#include "..\..\defines.h"
#include "actor.h"
#include "RTTI.h"

class IObject;
class Wobb;

IObject* CAmbientSoundMakeInstance(void);

// Plays the course's ambient sound areas (WPolySoup's sound boxes).
class CAmbientSound : public IActor {
public:
	static const WRTTI m_RTTI;

	virtual ~CAmbientSound(void) {}   // ambientsound.h:9

	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ambientsound.h:11

	virtual void OnInit(void);
	virtual void OnProcess(float delta);

protected:
	friend IObject* CAmbientSoundMakeInstance(void);   // the factory creates it
	CAmbientSound(void);

	float GetVolume(const Wobb& box, float range);

	virtual void HandleMsg(const MsgObject& msg);
};

CHECK_SIZE(CAmbientSound, 0x18);

#endif // PG_CLIENT_PROJECTG_AMBIENTSOUND_H
