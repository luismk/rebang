#pragma once

#include "wd3d.h"
#include <bitmap.h>
#include <wproc.h>
#include <wutil.h>
#include <wsplash.h>

#include <d3d9.h>
#include <d3dx9math.h>

#include <string>
#include <list>
#include <stack>
#include <vector>
#include <map>

namespace Nv
{
	struct IScreenCape;
	namespace Factory
	{
		class ScreenCapeFactory;
	}
}

#include <wresrcmng.h>

class WDirect3D8;
class WSplashD3D;

enum eWindowsVersion
{
	WinVerNone,
	WinVerOld,
	WinVerWindowsNt,
	WinVerWindows2000,
	WinVerWindowsXp,
	WinVerWindowsVista,
	WinVerWindows7,
};

eWindowsVersion GetWindowsVersion();

namespace nsWindowUtility
{
	typedef HRESULT(WINAPI* IsCompositionEnabledPtr)(BOOL*);
	typedef HRESULT(WINAPI* EnableCompositionPtr)(int);

	class CDWMApiDll;
};

struct sCpatureOption
{
	enum eCaptureMode
	{
		CM_A = 0x0,
		CM_B = 0x1,
		CM_Q = 0x2,
	};

	sCpatureOption();
	void SetMode(eCaptureMode mode, bool windowed);

	eCaptureMode currentMode;
	bool useMemCopy;
	bool updateWholeScreen;
	bool dxCopyRectsOK;
	bool useTexture;
	D3DSWAPEFFECT SwapEffect;
	unsigned int FullScreen_PresentationInterval;
};

class WSplashD3D : public WSplash
{
public:
	WSplashD3D(WDirect3D8* pDriver);
	~WSplashD3D();
	int Init(tagBITMAPINFO& bi, void* data, bool fitToScreen);
	int InitFromScreen(bool bCopy);
	void Reset();
	void Draw(const WPoint* pSrc, const WRect* pDest, unsigned long color);
	void Draw(unsigned long color);
	void ResetScreenSize();
	int GetWidth();
	int GetHeight();
	void SetTexCoordOffset(float u, float v);

	struct _MYVERTEX
	{
		float sx, sy, sz, rhw;
		unsigned long color;
		float tu, tv;
		_MYVERTEX();
		_MYVERTEX(const D3DVECTOR& v, float _rhw, unsigned long _color,
			float _tu, float _tv);
	};

private:
	WDirect3D8* m_pDriver;
	int m_hTexs[64];
	tagRECT m_rects[64];
	WVector2D m_uvs[64];
	WVector2D m_offset;
	int m_iBufWidth;
	int m_iBufHeight;
	int m_iSrcWidth;
	int m_iSrcHeight;
	int m_nSurfCount;
};

class WDirect3D8 : public WDirect3D, public WProc
{
	friend class WSplashD3D;

public:
	struct pix_info;
	WDirect3D8(const WDirect3D8&);
	WDirect3D8(char*, int);
	virtual ~WDirect3D8();
	int Init(char*, HWND__*, int);
	bool LoadSHCoeff(const char*);
	virtual WProc* ExternProc() { return this; }
	virtual int WinProc(unsigned int, unsigned long, unsigned long);
	virtual WVideoDev* MakeClone(char*, HWND__*, int);
	virtual char* GetDeviceName();
	virtual char* EnumModeName();
	virtual bool Reset(bool, int, int, int, long, int);

protected:
	void _DrawPrimitiveUP(_D3DPRIMITIVETYPE, unsigned int, const void*,
		unsigned int);
	void _DrawIndexedPrimitive(_D3DPRIMITIVETYPE, unsigned int, unsigned int,
		unsigned int, unsigned int);
	void _DrawIndexedPrimitiveUP(_D3DPRIMITIVETYPE, unsigned int, unsigned int,
		const void*, const void*, unsigned int);
	void _SetTransform(_D3DTRANSFORMSTATETYPE, const D3DXMATRIX&);
	void _SetRenderState(_D3DRENDERSTATETYPE, unsigned long);
	void _SetTextureStageState(unsigned long, _D3DTEXTURESTAGESTATETYPE,
		unsigned long);
	void _SetTexture(unsigned long, IDirect3DTexture9*);
	void _SetLight(unsigned long, const _D3DLIGHT9*);
	void _SetSamplerState(unsigned long, _D3DSAMPLERSTATETYPE, unsigned long);
	void _SetShader(unsigned long);

public:
	IDirect3DTexture9* m_pTexture[2];
	virtual void DrawPrimitive(int, int, unsigned long, void*,
		_D3DPRIMITIVETYPE, int);
	virtual void DrawPrimitiveIndexed(int, unsigned long, void*, int,
		unsigned short*, int, int);
	void Restore(int);
	virtual bool SetRenderState4Flushing(int);
	virtual void DrawLine(WTVertex** p, int type) { }
	virtual int Command(wVDevMessage, int, int);
	virtual int UploadCompressedTextureSurface(void*, unsigned int, int);
	virtual void UpdateCompressedTexture(int, void*, unsigned int, int);
	virtual void CreateTextureSurface(unsigned long, unsigned long, int, int,
		int, int, int);

protected:
	void UpdateTextureSurface(void*, int, int, int, pix_info*, tagBITMAPINFO*,
		void*, int);

public:
	virtual void UpdateTextureSurface(int, tagBITMAPINFO*, void*, int);
	virtual void DestroyTexture(int);
	virtual void FixTexturePart(int, const tagRECT&, tagBITMAPINFO*, void*,
		int);
	virtual bool IsTextureFilled(int) const;
	virtual int GetTextureWidth(int) const;
	virtual int GetTextureHeight(int) const;
	virtual void SetRenderTargetSizeInfo(int, const WRenderToTextureSizeInfo&);
	virtual void Clear(unsigned long, int, float);
	virtual bool BeginScene();
	virtual void EndScene();
	virtual void Paint();
	void SetExtraTexture(int);
	void SetRefTexture(int);
	virtual void SetGlobalRenderState(int rs, int rsEx)
	{
		this->m_GlobalRS[0] = rs;
		this->m_GlobalRS[1] = rsEx;
	}
	virtual bool IsSupportVS() const
	{
		return this->m_d3dcaps.VertexShaderVersion >= 0xFFFE0101;
	}
	virtual bool IsSupportPS() const
	{
		return this->m_d3dcaps.PixelShaderVersion >= 0xFFFF0200;
	}
	virtual bool IsSupportMRT() const
	{
		if (m_d3dcaps.NumSimultaneousRTs > 1)
			return true;
		return false;
	}
	virtual bool IsSupportClipPlane() const
	{
		if (m_d3dcaps.MaxUserClipPlanes >= 1)
			return true;
		return false;
	}
	virtual bool SetFogEnable(bool);
	virtual void SetFogState(float, float, unsigned long);
	void ResetFogState();
	virtual int GetWidth() const { return this->m_d3dpp.BackBufferWidth; }
	virtual int GetHeight() const { return this->m_d3dpp.BackBufferHeight; }
	virtual bool IsWindowed() const
	{
		if (m_d3dpp.Windowed)
			return true;
		return false;
	}
	virtual bool IsFillScreenMode() const { return this->m_fillScrMode; }
	virtual float GetMonitorSupportFPS() const { return this->m_fps; }
	virtual bool BeginRenderToTexture(const WRenderToTextureParam&);
	virtual void EndRenderToTexture(const WRenderToTextureParam&);
	virtual bool SupportRenderTargetFormat() const
	{
		return this->m_RtFmtIdx >= 0;
	}
	virtual bool IsSupportedDisplayMode(bool, int, int, int);
	virtual bool GetWindowDisplayMode(int&, int&, int&);
	virtual void SetViewPort(unsigned long, unsigned long, unsigned long,
		unsigned long);

public:
	struct sRtBackup
	{
		struct Rt
		{
			IDirect3DSurface9* surf[2];
			Rt() { Init(); }
			void Init() { memset(surf, 0, sizeof(surf)); }
		};

		Rt rt;
		IDirect3DSurface9* depthSurf;
		D3DVIEWPORT9 viewport;
		sRtBackup();
		void Init();
		void Restore(IDirect3DDevice9* dev, const sRtBackup& curRt);
	};

	struct pix_info
	{
		D3DFORMAT pixFmt;
		unsigned int cpp;
		unsigned int r_r_shift;
		unsigned int r_l_shift;
		unsigned int g_r_shift;
		unsigned int g_l_shift;
		unsigned int b_r_shift;
		unsigned int b_l_shift;
		unsigned int a_r_shift;
		unsigned int a_l_shift;

		unsigned long Pack(BYTE r, BYTE g, BYTE b)
		{
			return static_cast<unsigned long>(r) >> LOBYTE(this->r_r_shift)
					<< this->r_l_shift |
				static_cast<unsigned long>(g) >> LOBYTE(this->g_r_shift)
					<< this->g_l_shift |
				static_cast<unsigned long>(b) >> LOBYTE(this->b_r_shift)
					<< this->b_l_shift;
		}

		unsigned long Pack(BYTE a, BYTE r, BYTE g, BYTE b)
		{
			return static_cast<unsigned long>(r) >> LOBYTE(this->r_r_shift)
					<< this->r_l_shift |
				static_cast<unsigned long>(g) >> LOBYTE(this->g_r_shift)
					<< this->g_l_shift |
				static_cast<unsigned long>(b) >> LOBYTE(this->b_r_shift)
					<< this->b_l_shift |
				static_cast<unsigned long>(a) >> LOBYTE(this->a_r_shift)
					<< this->a_l_shift;
		}
	};

	struct sEffect
	{
		struct Param
		{
			Param()
				: handle(0), isShared(false)
			{
			}
			const char* handle;
			bool isShared;
		};

		sEffect()
			: pEffect(0)
		{
		}
		ID3DXEffect* pEffect;
		Param param[21];
	};

	struct sMergeBuffer
	{
		D3DPRIMITIVETYPE type;
		unsigned int numVertices;
		unsigned int primitiveCount;
		unsigned int vertexStreamZeroStride;
		int xhIb;
		int xhVb;
		unsigned int minIndex;
		unsigned int startIndex;
		int lockVertex;
		int lockIndex;
		D3DXMATRIX xLastMatrix;
		unsigned char* vertexStreamBuffer;
		unsigned long vertexStreamBufferSize;
		unsigned short* indexBuffer;
		unsigned long indexBufferSize;

		unsigned long GetVertexStreamBufferSize() const
		{
			return vertexStreamBufferSize;
		}
		unsigned long GetIndexBufferSize() const { return indexBufferSize; }

		void CheckAndIncreaseVertexStreamBuffer(unsigned long size);

		void CheckAndIncreaseIndexBuffer(unsigned long size);
	};

public:
	struct d3d8_texture
	{
		Bitmap* bitmap;
		int dxtcDataSize;
		char* dxtcData;
		int updateType;
		bool needToBeFilled;
		IDirect3DTexture9* pTex;
		IDirect3DSurface9* pSurf;
		int width;
		int height;
		int mipmaplevel;
		pix_info* pixFmtInfo;
		int renderTargetType;
		WRenderToTextureSizeInfo renderTargetSizeInfo;
		bool isFilled;
	};

	struct sDepthSurf
	{
		WORD wWidth;
		WORD wHeight;
		IDirect3DSurface9* pDepth;
	};

	struct sVb8
	{
		unsigned long xdwFVF;
		unsigned long xdwUsage;
		unsigned char xbStride;
		IDirect3DVertexBuffer9* xpVb;
		int xnVtxs;
		bool xbNeedToBeFilled;
		unsigned char* xpVertexData;
	};

	struct sIb8
	{
		unsigned long xdwUsage;
		IDirect3DIndexBuffer9* xpIb;
		int xnIdxs;
		bool xbNeedToBeFilled;
		unsigned short* xpIndexData;
	};

	struct sVertexDecl
	{
		IDirect3DVertexDeclaration9* pDecl;
		int pitch;
	};

	struct sRtFormat
	{
		D3DFORMAT fmt;
		int bpp;
	};

	struct FMTLIST
	{
		D3DFORMAT format;
		const char* dispaly;
		int bitNum;
	};

protected:
	void BackupMainRenderTarget();
	void ReleaseMainRenderTarget();
	void BackupRenderTarget(const WRenderToTextureParam&, IDirect3DSurface9*,
		const _D3DVIEWPORT9&);
	void RestoreBackedupRenderTarget();
	void ReleaseAllRendertargetBackupResource();

	std::stack<sRtBackup, std::deque<sRtBackup> > m_rtBackupList;
	sRtBackup m_mainRt;
	sRtBackup m_curRt;
	void SetRenderTargetFormat();
	int m_RtFmtIdx;
	int m_RtNoAlphaFmtIdx;
	int m_RtLowMemFmtIdx;
	int m_RtNoAlphaLowMemFmtIdx;
	int m_RtDepthFmtIdx;

	static sRtFormat ms_RtFmt[4];
	static sRtFormat ms_RtNoAlphaFmt[3];
	static sRtFormat ms_RtDepthFmt[5];
	HRESULT Present();
	virtual WDirect3D8* CreateClone(char*, int);
	bool BeginCapturedBackground();
	IDirect3DSurface9* m_pCopiedScreenSurface;
	int m_hCopiedScreenTexture;
	WSplashD3D* m_pCopiedScreenSplash;
	bool m_useCopiedScreen;
	void ReleaseCopiedScreenResource();
	_D3DSURFACE_DESC m_capturedDdsd;
	HRESULT ScreenShot(const char*, int);
	bool CaptureScreen(Bitmap*);
	void SetDefaultState();
	void SetTexture(unsigned long);
	void SetRenderState(unsigned long, unsigned long);
	void SetBlendMode(unsigned long);
	void SetVtxMode(unsigned long);
	void SetVtxType(unsigned long, unsigned long, unsigned long, unsigned long);
	void SetTextureStageState(int, int, unsigned long);
	void SetBlendState(unsigned long, unsigned long);
	unsigned long* GetZBufferHistogram();
	int GetBackBufferBpp(_D3DFORMAT);
	HRESULT SetVertexShader(unsigned long);
	unsigned long m_lastTexState;
	unsigned long m_lastRenderState;
	unsigned long m_lastBlendMode;
	unsigned long m_texStageState[2][6];
	unsigned long m_lastBlendState[2];
	unsigned long m_blendEnable;
	unsigned long m_lastVtxType;
	unsigned long m_vtxSize;
	int SetTextureFormat(_D3DFORMAT);
	_D3DFORMAT FindDepthBufferFormat(_D3DFORMAT);
	void Release();
	char* DuplicateString(const char*);
	bool GetDevName(const _D3DDISPLAYMODE&, char*, int);
	unsigned char* GetPixelPtr(tagBITMAPINFO*, unsigned char*, int, int);

	static FMTLIST ms_fmtList[5];

	void UpdateTextureSurfaceDirect(void*, int, pix_info*, const tagRECT&,
		tagBITMAPINFO*, void*);
	void UpdateTextureSurfaceDirect(void*, int, pix_info*, tagBITMAPINFO*,
		void*);
	void UpdateTextureSurfaceSampling(void*, int, int, int, pix_info*,
		tagBITMAPINFO*, void*, int);
	void UpdateTextureSurfaceFiltering(void*, int, int, int, pix_info*,
		tagBITMAPINFO*, unsigned char*);
	void UpdateTextureSurfaceNormal(void*, int, pix_info*, tagBITMAPINFO*,
		void*);
	void UpdateTextureSurfaceAlpha(void*, int, pix_info*, const tagRECT&,
		tagBITMAPINFO*, void*);
	IDirect3DSurface9* FindDepthSurf(unsigned short, unsigned short);
	static pix_info ms_fmtTypeList[9];
	static pix_info* FindPixInfoByFormat(_D3DFORMAT);

	d3d8_texture m_texList[2048];

	sDepthSurf m_depthSurfList[32];
	IDirect3DSurface9* m_commonDepthSurf;
	static _D3DFORMAT ms_fmtRecomList[6][4];
	static unsigned long ms_ZbuffHistogram[256];
	float m_fps;
	bool m_bWindow;
	pix_info* m_fmt[6];
	IDirect3D9* m_d3d8;
	IDirect3DDevice9* m_pd3dDevice;
	_D3DCAPS9 m_d3dcaps;
	_D3DPRESENT_PARAMETERS_ m_d3dpp;
	_D3DFORMAT m_fmtWindowed;
	char* m_devName;
	char* m_modList;
	int m_devId;
	HWND__* m_hwnd;
	long m_lWndStyle;
	bool m_clientRcCheckedAfterReset;
	IDirect3DQuery9* m_pEventQuery;
	Nv::IScreenCape* m_pScreenCape;
	Nv::Factory::ScreenCapeFactory* m_pScreenCapeFactory;

public:
	void xGetCaps(unsigned long, unsigned long*);
	virtual unsigned long xDetermineFVF(int, int, int);
	virtual unsigned long xDetermineBufferUsage(unsigned long);
	unsigned long xGetFVF(int);
	virtual unsigned char xGetStride(int);
	virtual bool xHasVertexElem(unsigned long, wWxVertexElem) const;
	virtual int xGetVertexElemOffset(unsigned long, wWxVertexElem) const;
	virtual int xGetBlendWeightSize(unsigned long) const;
	virtual void xCreateVertexBuffer(int, int, unsigned long, unsigned long);
	virtual void xCreateIndexBuffer(int, int, unsigned long);
	virtual void xReleaseVertexBuffer(int);
	virtual void xReleaseIndexBuffer(int);
	virtual unsigned char* xLockVertexBuffer(int, unsigned int, unsigned int);
	virtual unsigned char* xLockIndexBuffer(int, unsigned int, unsigned int);
	virtual void xUnlockVertexBuffer(int);
	virtual void xUnlockIndexBuffer(int);
	virtual void xDrawIndexedPrimitive(const WxViewState&, const WxBatchState&);
	virtual void SetShaderSource(const char*);

private:
	void xSetTnLBuffer(int, int);
	void xSetLight(unsigned long, const LightSet&);
	virtual void xSetTransform(WVDTRANSFORMSTATETYPE, const WMatrix4&);
	virtual void xSetPrevViewTransform(const WMatrix4&);
	void xSetRenderState(unsigned long, unsigned long);
	bool xReset(bool);
	void xReset_ReleaseResource();
	void xReset_CreateResource();
	void CreateEventQuery();
	void ReleaseEventQuery();
	void xInstantiateAndFillTexture(int);
	void xInstantiateTexture(int);
	void xFillTexture(int);
	void xInstantiateVertexBuffer(int);
	void xInstantiateIndexBuffer(int);
	void xFillVertexBuffer(int);
	void xFillIndexBuffer(int);
	unsigned long m_xdwDevBehavior;
	int m_xiVbSize;

	sVb8 m_xaVbList[1024];

	sIb8 m_xaIbList[256];
	bool m_xbCurHwTnL;
	bool m_xbLastHwTnL;
	unsigned long m_xLastDrawType;
	unsigned long m_xLastRenderState;
	unsigned long m_xLastVertexDecl;
	D3DXMATRIX m_xLastWorldMatrix[256];
	D3DXMATRIX m_xLastViewMatrix;
	D3DXMATRIX m_xLastProjMatrix;
	D3DXMATRIX m_xPrevViewMatrix;
	unsigned long m_LastTFactor;
	float m_LastFogStart;
	float m_LastFogEnd;
	_D3DVECTOR m_LightDirect;
	int m_xhLastVb;
	int m_xhLastIb;
	unsigned long m_xdwLastUsage;
	unsigned long m_xdwLastFVF;
	unsigned long m_xdwTFactor;
	bool m_xbUseTFactor;
	bool m_xbLastVertexBlend;
	int m_xnTotalTris;
	int m_xnDPUPs;
	int m_xnDIPUPs;
	int m_xnDPs;
	int m_xnDIPs;
	LightSet m_xaWLights[4];
	_D3DLIGHT9 m_xaLights[4];
	_D3DMATERIAL9 m_xMaterial;
	bool m_LightEnable;
	unsigned long m_xdwDiffuse;
	unsigned int m_iDDSRes;
	bool m_bUseMipmap;
	unsigned int m_iMaxMipLvl;
	unsigned long m_dwMipCreateFilter;
	unsigned long m_dwMipTexStateFilter;

	std::map<int, sVertexDecl> m_VertexDecl;
	std::string m_shadersrc;
	unsigned long m_Effect;
	unsigned long m_lastEffect;

	std::map<unsigned long, sEffect> m_EffectTable;
	bool CreateEffect(unsigned long);
	void UpdateShaderValue(int);
	bool ApplyShader();
	void ReloadShader();
	void ReleaseShaderResource();
	float m_SHCoeff[7][4];
	unsigned long m_GlobalRS[2];

	sMergeBuffer m_MergeBuffer;
	virtual void FlushRenderPrimitive();
	bool DrawIndexedPrimitiveLockable(_D3DPRIMITIVETYPE, const void*, int, int,
		unsigned short*, int);
	bool DrawPrimitiveLockable(_D3DPRIMITIVETYPE, unsigned int, const void*,
		unsigned int);
	int LockVB(const void*, int, int, int);
	int LockIB(const unsigned short*, int);
	IDirect3DVertexBuffer9* m_pLockableVB;
	IDirect3DIndexBuffer9* m_pLockableIB;
	int m_iVBOffset;
	int m_iIBOffset;
	static void FillVertex(unsigned char*, const unsigned char*, int, int, int,
		const D3DXMATRIX*);

public:
	class WFxParamPool
	{
	public:
		typedef std::map<const char*, int> StringIntMap;
		typedef std::map<const char*, D3DXVECTOR2> StringVec2Map;
		typedef std::map<const char*, D3DXVECTOR3> StringVec3Map;
		typedef std::map<const char*, D3DXVECTOR4> StringVec4Map;
		typedef std::map<const char*, D3DXMATRIX> StringMatrixMap;
		typedef std::map<const char*, IDirect3DBaseTexture9*> StringTexMap;

		typedef std::map<WVDFXPARAMETERTYPE, int> ParamIntMap;
		typedef std::map<WVDFXPARAMETERTYPE, D3DXVECTOR2> ParamVec2Map;
		typedef std::map<WVDFXPARAMETERTYPE, D3DXVECTOR3> ParamVec3Map;
		typedef std::map<WVDFXPARAMETERTYPE, D3DXVECTOR4> ParamVec4Map;
		typedef std::map<WVDFXPARAMETERTYPE, D3DXMATRIX> ParamMatrixMap;
		typedef std::map<WVDFXPARAMETERTYPE, IDirect3DBaseTexture9*>
			ParamTexMap;

		typedef std::pair<ParamIntMap, StringIntMap> IntCachePair;
		typedef std::pair<ParamVec2Map, StringVec2Map> Vec2CachePair;
		typedef std::pair<ParamVec3Map, StringVec3Map> Vec3CachePair;
		typedef std::pair<ParamVec4Map, StringVec4Map> Vec4CachePair;
		typedef std::pair<ParamMatrixMap, StringMatrixMap> MatrixCachePair;
		typedef std::pair<ParamTexMap, StringTexMap> TexCachePair;

		ID3DXEffectPool* m_pool;
		IntCachePair m_paramCacheListPair_Int;
		Vec2CachePair m_paramCacheListPair_Vec2;
		Vec3CachePair m_paramCacheListPair_Vec3;
		Vec4CachePair m_paramCacheListPair_Vec4;
		MatrixCachePair m_paramCacheListPair_Mat;
		TexCachePair m_paramCacheListPair_Tex;

		WFxParamPool()
			: m_pool(0)
		{
		}
		~WFxParamPool() { }
		void Create(IDirect3DDevice9*);
		void OnLostDevice(IDirect3DDevice9*);
		void OnResetDevice(IDirect3DDevice9*);

		ID3DXEffectPool* GetPool() const { return this->m_pool; }

		void GetInt(ID3DXEffect* ef, WVDFXPARAMETERTYPE wvdFxParamType,
			const char* hParam, bool isShared, int& Value) const;

		void SetInt(ID3DXEffect* ef, WVDFXPARAMETERTYPE wvdFxParamType,
			const char* hParam, bool isShared, int Value);

		void GetVector2(ID3DXEffect* ef, WVDFXPARAMETERTYPE wvdFxParamType,
			const char* hParam, bool isShared, D3DXVECTOR2& Value) const;

		void SetVector2(ID3DXEffect* ef, WVDFXPARAMETERTYPE wvdFxParamType,
			const char* hParam, bool isShared, const D3DXVECTOR2& Value);

		void GetVector3(ID3DXEffect* ef, WVDFXPARAMETERTYPE wvdFxParamType,
			const char* hParam, bool isShared, D3DXVECTOR3& Value) const;

		void SetVector3(ID3DXEffect* ef, WVDFXPARAMETERTYPE wvdFxParamType,
			const char* hParam, bool isShared, const D3DXVECTOR3& Value);

		void GetVector4(ID3DXEffect* ef, WVDFXPARAMETERTYPE wvdFxParamType,
			const char* hParam, bool isShared, D3DXVECTOR4& Value) const;

		void SetVector4(ID3DXEffect* ef, WVDFXPARAMETERTYPE wvdFxParamType,
			const char* hParam, bool isShared, const D3DXVECTOR4& Value);

		void GetMatrix(ID3DXEffect* ef, WVDFXPARAMETERTYPE wvdFxParamType,
			const char* hParam, bool isShared, D3DXMATRIX& Value) const;

		void SetMatrix(ID3DXEffect* ef, WVDFXPARAMETERTYPE wvdFxParamType,
			const char* hParam, bool isShared, const D3DXMATRIX& Value);

		void GetTexture(ID3DXEffect* ef, WVDFXPARAMETERTYPE wvdFxParamType,
			const char* hParam, bool isShared,
			IDirect3DBaseTexture9*& Value) const;

		void SetTexture(ID3DXEffect* ef, WVDFXPARAMETERTYPE wvdFxParamType,
			const char* hParam, bool isShared, IDirect3DBaseTexture9* Value);

	private:
		void ClearParamCache();

	public:
		void Release();
	};

protected:
	WFxParamPool& GetFxParamPool() { return this->m_fxParamPool; }
	WFxParamPool m_fxParamPool;

public:
	virtual void BeginUsingCustomRenderState();
	virtual void SetCustomRenderState(WVDRENDERSTATETYPE, unsigned long);
	virtual void SetCustomTextureStageState(unsigned long,
		WVDTEXTURESTAGESTATETYPE, unsigned long);
	virtual void SetCustomTransform(WVDTRANSFORMSTATETYPE, const WMatrix4&);
	virtual void SetCustomTexture(unsigned long, int);
	virtual void SetCustomClipPlane(unsigned long, const WPlane&);
	virtual void SetCustomSamplerState(unsigned long, WVDSAMPLERSTATETYPE,
		unsigned long);
	virtual void SetCustomFxMacro(unsigned long);
	virtual void SetCustomFxParamInt(WVDFXPARAMETERTYPE, int);
	virtual void SetCustomFxParamVector2(WVDFXPARAMETERTYPE, const WVector2D&);
	virtual void SetCustomFxParamVector3(WVDFXPARAMETERTYPE, const WVector&);
	virtual void SetCustomFxParamVector4(WVDFXPARAMETERTYPE, const WVector4&);
	virtual void SetCustomFxParamMatrix(WVDFXPARAMETERTYPE, const WMatrix4&);
	virtual void SetCustomFxParamTexture(WVDFXPARAMETERTYPE, int);
	virtual void EndUsingCustomRenderState();

protected:
	virtual void* SnapShotCustomRenderState();
	virtual void ClearCustomRenderStateSnapShotList();
	virtual void ApplyCustomRenderState(const void*);

public:
	template <typename T, typename A>
	struct sTwoVarRs
	{
		sTwoVarRs()
		{
			memset(&this->m_Type, 0, sizeof(this->m_Type));
			memset(&this->m_Value, 0, sizeof(this->m_Value));
			memset(&this->m_OldValue, 0, sizeof(this->m_OldValue));
		}

		sTwoVarRs(const T& Type, const A& Value)
		{
			this->m_Type = Type;
			this->m_Value = Value;
			memset(&this->m_OldValue, 0, sizeof(this->m_OldValue));
		}

		sTwoVarRs(const sTwoVarRs& rs)
		{
			m_Type = rs.m_Type;
			m_Value = rs.m_Value;
			m_OldValue = rs.m_OldValue;
		}

		virtual ~sTwoVarRs() { }

		T m_Type;
		A m_Value, m_OldValue;
	};

	template <typename T1, typename T2, typename A>
	struct sThreeVarRs : public sTwoVarRs<T1, A>
	{
		sThreeVarRs() { memset(&this->m_Type2, 0, sizeof(this->m_Type2)); }

		sThreeVarRs(const T1& Type1, const T2& Type2, const A& Value)
			: sTwoVarRs<T1, A>(Type1, Value)
		{
			this->m_Type2 = Type2;
		}

		sThreeVarRs(const sThreeVarRs& rs)
			: sTwoVarRs<T1, A>(rs), m_Type2(rs.m_Type2)
		{
		}

		~sThreeVarRs() { }
		T2 m_Type2;
	};

	struct sClipPlane : public sTwoVarRs<unsigned long, D3DXPLANE>
	{
		D3DXPLANE m_SetValue;

		sClipPlane(unsigned long Index, const D3DXPLANE& Value)
			: sTwoVarRs<unsigned long, D3DXPLANE>(Index, Value)
		{
		}
	};
	class WRenderState
	{
	public:
		typedef std::vector<sTwoVarRs<D3DRENDERSTATETYPE, DWORD> > RsList;
		RsList m_rsList;
		typedef std::vector<
			sThreeVarRs<DWORD, D3DTEXTURESTAGESTATETYPE, DWORD> >
			TssList;
		TssList m_tssList;
		typedef std::vector<sTwoVarRs<D3DTRANSFORMSTATETYPE, D3DXMATRIX> >
			TransfList;
		TransfList m_transfList;
		typedef std::vector<sTwoVarRs<DWORD, IDirect3DBaseTexture9*> > TexList;
		TexList m_texList;
		typedef std::vector<sClipPlane> ClipPlaneList;
		ClipPlaneList m_clipPlaneList;
		typedef std::vector<sThreeVarRs<DWORD, D3DSAMPLERSTATETYPE, DWORD> >
			SsList;
		SsList m_ssList;
		unsigned int m_fxMacro;
		typedef std::vector<sTwoVarRs<WVDFXPARAMETERTYPE, int> > FxParamIntList;
		FxParamIntList m_fxParamIntList;
		typedef std::vector<sTwoVarRs<WVDFXPARAMETERTYPE, D3DXVECTOR2> >
			FxParamVec2List;
		FxParamVec2List m_fxParamVec2List;
		typedef std::vector<sTwoVarRs<WVDFXPARAMETERTYPE, D3DXVECTOR3> >
			FxParamVec3List;
		FxParamVec3List m_fxParamVec3List;
		typedef std::vector<sTwoVarRs<WVDFXPARAMETERTYPE, D3DXVECTOR4> >
			FxParamVec4List;
		FxParamVec4List m_fxParamVec4List;
		typedef std::vector<sTwoVarRs<WVDFXPARAMETERTYPE, D3DXMATRIX> >
			FxParamMatList;
		FxParamMatList m_fxParamMatList;
		typedef std::vector<
			sTwoVarRs<WVDFXPARAMETERTYPE, IDirect3DBaseTexture9*> >
			FxParamTexList;
		FxParamTexList m_fxParamTexList;

		WRenderState(const RsList& rsList, const TssList& tssList,
			const TransfList& transfList, const TexList& texList,
			const ClipPlaneList& clipPlaneList, const SsList& ssList,
			int fxMacro, const FxParamIntList& fxParamIntList,
			const FxParamVec2List& fxParamVec2List,
			const FxParamVec3List& fxParamVec3List,
			const FxParamVec4List& fxParamVec4List,
			const FxParamMatList& fxParamMatList,
			const FxParamTexList& fxParamTexList)
			: m_rsList(rsList),
			  m_tssList(tssList),
			  m_transfList(transfList),
			  m_texList(texList),
			  m_clipPlaneList(clipPlaneList),
			  m_ssList(ssList),
			  m_fxMacro(fxMacro),
			  m_fxParamIntList(fxParamIntList),
			  m_fxParamVec2List(fxParamVec2List),
			  m_fxParamVec3List(fxParamVec3List),
			  m_fxParamVec4List(fxParamVec4List),
			  m_fxParamMatList(fxParamMatList),
			  m_fxParamTexList(fxParamTexList)
		{
		}

		WRenderState()
			: m_fxMacro(0)
		{
		}
		void Clear()
		{
			this->m_rsList.clear();
			this->m_tssList.clear();
			this->m_transfList.clear();
			this->m_texList.clear();
			this->m_clipPlaneList.clear();
			this->m_ssList.clear();
			this->m_fxMacro = 0;
			this->m_fxParamIntList.clear();
			this->m_fxParamVec2List.clear();
			this->m_fxParamVec3List.clear();
			this->m_fxParamVec4List.clear();
			this->m_fxParamMatList.clear();
			this->m_fxParamTexList.clear();
		}
		bool HasAnyState() const
		{
			if (m_rsList.size() > 0 || m_tssList.size() > 0 ||
				m_transfList.size() > 0 || m_texList.size() > 0 ||
				m_clipPlaneList.size() > 0 || m_ssList.size() > 0 ||
				m_fxMacro != 0 || m_fxParamIntList.size() > 0 ||
				m_fxParamVec2List.size() > 0 || m_fxParamVec3List.size() > 0 ||
				m_fxParamVec4List.size() > 0 || m_fxParamMatList.size() > 0 ||
				m_fxParamTexList.size() > 0)
				return true;
			return false;
		}
		bool operator!=(const WRenderState& rhs) const
		{
			return !(*this == rhs);
		}
		unsigned long GetFxMacro() const { return this->m_fxMacro; }

		bool operator==(const WRenderState& rhs) const;

		void SetFxMacro(unsigned long);

		void SetRenderState(D3DRENDERSTATETYPE state, unsigned long value);

		void SetTransform(D3DTRANSFORMSTATETYPE state,
			const D3DXMATRIX& matrix);

		void SetTexture(unsigned long stage, IDirect3DBaseTexture9* pTex);

		void SetFxParamInt(WVDFXPARAMETERTYPE paramType, int value);

		void SetFxParamVector2(WVDFXPARAMETERTYPE ParamType,
			const D3DXVECTOR2& Value);

		void SetFxParamVector3(WVDFXPARAMETERTYPE ParamType,
			const D3DXVECTOR3& Value);

		void SetFxParamVector4(WVDFXPARAMETERTYPE ParamType,
			const D3DXVECTOR4& Value);

		void SetFxParamMatrix(WVDFXPARAMETERTYPE ParamType,
			const D3DXMATRIX& Value);

		void SetFxParamTexture(WVDFXPARAMETERTYPE ParamType,
			IDirect3DBaseTexture9* pTex);

		void Begin(WDirect3D8& wd3d);

		void End(WDirect3D8& wd3d);

		void SetTextureStageState(unsigned long stage,
			D3DTEXTURESTAGESTATETYPE type, unsigned long value);

		void SetClipPlane(unsigned long Index, const D3DXPLANE& Value);

		void SetSamplerState(unsigned long sampler, D3DSAMPLERSTATETYPE type,
			unsigned long value);
	};

protected:
	WRenderState m_CustomRenderState;
	std::stack<WRenderState, std::deque<WRenderState> >
		m_CustomRenderStateBackupList;
	std::list<WRenderState> m_CustomRenderStateSnapShotList;
	nsWindowUtility::CDWMApiDll* m_pkDWMApiDll;
	bool m_fillScrMode;
};
