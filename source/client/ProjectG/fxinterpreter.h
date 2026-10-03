#pragma once

#include <list>

struct Attacher;

class FxInterpreter
{
	friend FxInterpreter* GetFxInterpreter();

public:
	enum eScripFormat
	{
		eSF_FX,
		eSF_PLAYIF,
	};

	enum eEffectType
	{
		eET_NONE,
		eET_SEQUENCE,
		eET_SPRAY,
	};

	struct sFxInfo
	{
		sFxInfo(int _id, eEffectType _type, void* _pFx);
		void DetachSingleFx();

		int id;
		eEffectType type;
		void* pFx;
	};

protected:
	FxInterpreter();

public:
	virtual ~FxInterpreter();

	int ParseBoundingBoxScript_fx(WPuppet* pet, eScripFormat format);
	int ParseCharBoundingBoxScript_fx(WPuppet* pet, eScripFormat format);
	int ParseCharFrameDataScrip_fx(WPuppet* pet, eScripFormat format);
	void DetachFx(int id);
	void DetachFxAll();

protected:
	bool ParseToken(const char** str, eScripFormat format) const;
	bool ParseFxName(const char** str, char* name, eScripFormat format,
		eEffectType& type) const;
	bool AttachFx(const char* name, const Attacher* attacher, eEffectType type);

	static const char TOKEN[][8];

	std::list<sFxInfo> m_fxList;
	int m_nextID;
};

FxInterpreter* GetFxInterpreter();
