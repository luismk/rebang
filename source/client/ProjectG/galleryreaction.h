// Original: d:\build\custom\temp\code\client\projectg\galleryreaction.h
#ifndef PG_CLIENT_PROJECTG_GALLERYREACTION_H
#define PG_CLIENT_PROJECTG_GALLERYREACTION_H

#include "..\..\defines.h"
#include "actor.h"
#include "RTTI.h"

class IObject;

IObject *CGalleryMakeInstance(void);

// The gallery's reactions to the shots (applause, sighs): plays a gallery
// sound on the round's messages, at most once per delay for the same sound.
// Size 0x3C (the `new` of CGalleryMakeInstance).
class CGallery : public IActor {
	LAYOUT_CHECK_FRIEND;
	friend IObject *CGalleryMakeInstance(void);
public:
	// galleryreaction.h:9
	virtual ~CGallery(void) {}

	static const WRTTI m_RTTI;
	// galleryreaction.h:11
	virtual const WRTTI *GetRTTI(void) const { return &m_RTTI; }

	virtual void OnProcess(float dt);
	virtual void HandleMsg(const MsgObject &msg);

protected:
	CGallery(void);

	void SetSound(const char *name, float delay);

	float m_soundDelay;     // 0x18, time left before the same sound may play again
	char m_soundName[0x20]; // 0x1C, the sound playing (alias of the sfx)
};

BEGIN_LAYOUT_CHECK(CGallery)
	CHECK_FIELD(CGallery, m_soundDelay, 0x18);
	CHECK_FIELD(CGallery, m_soundName, 0x1C);
	CHECK_SIZE(CGallery, 0x3C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GALLERYREACTION_H
