#pragma once

class WPuppet;

enum eObjType
{
	OBJ_BASE,
	OBJ_FUNC,
	OBJ_STATIC,
	OBJ_MODEL,
	OBJ_ACTOR,
	OBJ_CUSTOM,
	OBJ_EFFECT,
	OBJ_NONE
};

class CRenderFuncPtr
{
public:
	CRenderFuncPtr() { }
	~CRenderFuncPtr() { }

	void DisplayAll() { Display(); }

public:
	virtual void DisplayShadow() { }
	virtual void Display() = 0;

protected:
	virtual void DebugDisplay() { }
};

// TODO: incomplete
class CSceneManager : public WSingleton<CSceneManager>
{
public:
	void RegisterElement(eObjType type, WPuppet* puppet, CRenderFuncPtr* func,
		const char* name, unsigned long flag, Waabb* aabb);
	void DeleteElement(const char* name);
	void SetVisible(const char* name, bool bVisible);
	void Load();
	void UpdateFog(const char* mapName);
};
