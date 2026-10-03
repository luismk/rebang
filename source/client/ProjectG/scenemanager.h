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

class CWavelet
{
public:
	struct sWavelet
	{
		WVector pos[2];
		WVector dir;
		float u[2];
	};

	struct sLayer
	{
		float pos;
		float delay;
		float alpha;
		float time;
		float speed;
		int state;
	};

	CWavelet();
	virtual ~CWavelet();

	virtual void Process(float dt);
	virtual void Render();

	void SetWavelet(const std::vector<WVector>& points, const char* name,
		int layerNum);

protected:
	virtual void ReadAttribute(const char* name);
	virtual void SetLayer(int num);

	std::vector<sWavelet> m_wavelet;
	int m_texHandle;
	float m_width;
	float m_frequency;
	float m_deceleration;
	float m_fadeInTime;
	float m_fadeOutTime;
	float m_delay;
	std::vector<sLayer> m_layer;

	static WTVertex vtr[4];
	static WTVertex* vtx[5];
};
