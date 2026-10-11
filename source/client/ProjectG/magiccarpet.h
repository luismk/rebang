// Original: d:\build\custom\temp\code\client\projectg\magiccarpet.h
#ifndef PG_CLIENT_PROJECTG_MAGICCARPET_H
#define PG_CLIENT_PROJECTG_MAGICCARPET_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"

class IObject;

IObject* CMagicCarpetHelperMakeInstance(void);

// Lowers the BGM while the magic carpet effect plays and raises it back
// afterwards, and plays the carpet's bounce sounds. Size 0x1c (the `new` of
// CMagicCarpetHelperMakeInstance).
class CMagicCarpetHelper : public IActor {
	LAYOUT_CHECK_FRIEND;
	friend IObject* CMagicCarpetHelperMakeInstance(void);
public:
	static const WRTTI m_RTTI;

	// magiccarpet.h:11
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

protected:
	CMagicCarpetHelper(void);
public:
	virtual ~CMagicCarpetHelper(void);

	virtual void OnInit(void);
	virtual void OnLoad(void);
	virtual void OnProcess(float elapsed);
	virtual void OnDisplay(void);
	virtual void HandleMsg(const MsgObject& msg);

private:
	void PlaySfxMagicCarpetBoundSound(void);
	void AutoVolumeAdjustProcess(const float& elapsed);

	bool m_bVolumeDown;     // 0x18 BGM fading down (true) or back up (false)
	bool m_bAutoVolume;     // 0x19 volume adjustment running
	PADDING(0x1a, 0x1c);
};

CHECK_SIZE(CMagicCarpetHelper, 0x1c);
BEGIN_LAYOUT_CHECK(CMagicCarpetHelper)
	CHECK_FIELD(CMagicCarpetHelper, m_bVolumeDown, 0x18);
	CHECK_FIELD(CMagicCarpetHelper, m_bAutoVolume, 0x19);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_MAGICCARPET_H
