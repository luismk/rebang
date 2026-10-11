// Original: d:\build\custom\temp\code\client\projectg\fxinterpreter.h
#ifndef PG_CLIENT_PROJECTG_FXINTERPRETER_H
#define PG_CLIENT_PROJECTG_FXINTERPRETER_H

#include <list>

#include "..\..\defines.h"

struct Attacher;
class WPuppet;

// The effect script interpreter: parses "*fx" entries of a puppet's
// bounding-box scripts and keeps the effects it attached (CFx sequences and
// sprays) by handle. One instance, built on first use by GetFxInterpreter.
class FxInterpreter {
public:
	// How a script names its effect files.
	enum eScripFormat {
		SCRIPT_FORMAT_TEXT = 0,                // name runs till the next space
		SCRIPT_FORMAT_STRING = 1               // name is a quoted string constant
	};

	// What an attached effect is, from the file's extension.
	enum eEffectType {
		EFFECT_NONE = 0,
		EFFECT_SEQUENCE = 1,                   // ".seq", a CFxSequence
		EFFECT_SPRAY = 2                       // ".spr", a CFxSpray
	};

	struct sFxInfo {
		sFxInfo(int handle, eEffectType type, void* pFx);
		void DetachSingleFx(void);

		int m_handle;                          // 0x0
		eEffectType m_type;                    // 0x4
		void* m_pFx;                           // 0x8, CFxSequence* or CFxSpray*
	};

protected:
	FxInterpreter(void);

public:
	virtual ~FxInterpreter(void);

	int ParseBoundingBoxScript_fx(WPuppet* pPuppet, eScripFormat format);
	int ParseCharBoundingBoxScript_fx(WPuppet* pPuppet, eScripFormat format);
	int ParseCharFrameDataScrip_fx(WPuppet* pPuppet, eScripFormat format);
	void DetachFx(int handle);
	void DetachFxAll(void);

protected:
	bool ParseToken(const char** ppScript, eScripFormat format) const;
	bool ParseFxName(const char** ppScript, char* name, eScripFormat format, eEffectType& type) const;
	bool AttachFx(const char* name, const Attacher* pAttacher, eEffectType type);

	static const char TOKEN[2][8];

	std::list<sFxInfo> m_fxList;               // 0x04
	int m_handle;                              // 0x10, handle of the next script's effects

	friend FxInterpreter* GetFxInterpreter(void);
	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(FxInterpreter::sFxInfo, 0xC);
CHECK_FIELD(FxInterpreter::sFxInfo, m_type, 0x4);
CHECK_FIELD(FxInterpreter::sFxInfo, m_pFx, 0x8);
CHECK_SIZE(FxInterpreter, 0x14);
BEGIN_LAYOUT_CHECK(FxInterpreter)
	CHECK_FIELD(FxInterpreter, m_fxList, 0x4);
	CHECK_FIELD(FxInterpreter, m_handle, 0x10);
END_LAYOUT_CHECK;

FxInterpreter* GetFxInterpreter(void);

#endif // PG_CLIENT_PROJECTG_FXINTERPRETER_H
