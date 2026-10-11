// Original: d:\build\custom\temp\code\client\wangreal\include\wvideo.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WVIDEO_H
#define PG_CLIENT_WANGREAL_INCLUDE_WVIDEO_H

#include "..\..\defines.h"
#include "wmath.h"
#include "winput.h"

struct HWND__;
struct tagBITMAPINFO;
struct tagRECT;
class WVector4;
struct WxViewState;
struct WRenderToTextureSizeInfo;
struct WRenderToTextureParam;

// Video device enums (PDB values).
enum wVDevState {
	W_VDEVSTATE_NORMAL,
	W_VDEVSTATE_LOST,
	NUM_W_VDEVSTATES
};
enum wVDevMessage {
	W_VDEV_SETCLIPPER,
	W_VDEV_ACTIVATEAPP,
	W_VDEV_MOVE,
	W_VDEV_SIZE,
	W_VDEV_SETSIZE,
	W_VDEV_CAPTURE_SCREEN,
	W_VDEV_GET_FRONTSURFACE,
	W_VDEV_GET_BACKSURFACE,
	W_VDEV_DRAWBOX,
	W_VDEV_SETHWND,
	W_VDEV_SETRECT,
	W_VDEV_FLUSH,
	W_VDEV_FLIPFRONTSURFACE,
	W_VDEV_GET_CAPTURE_MODE,
	W_VDEV_SET_CAPTURE_MODE,
	W_VDEV_RELEASE_CAPTURERESOURCE,
	W_VDEV_CAPTURED_BG,
	W_VDEV_OVERDRAW_ANALYZE,
	W_VDEV_GET_ZBUFF_HISTOGRAM,
	W_VDEV_SCREEN_SHOT,
	W_VDEV_GET_SPLASH,
	W_VDEV_GET_COPIEDSCREENSPLASH,
	W_VDEV_UPDATE_LOD_TEXTURE,
	W_VDEV_SET_DDS_RES,
	W_VDEV_USE_MIPMAP,
	W_VDEV_MAX_MIPLVL,
	W_VDEV_MIP_CREATE_FILTER,
	W_VDEV_MIP_TEXSTAGE_FILTER,
	W_VDEV_GETAVAIL_VRAM,
	W_VDEV_NUM_TEXTURES,
	W_VDEV_NUM_TNL_BUFFERS,
	WX_VDEV_GET_STATISTICS,
	WX_VDEV_GET_CAPS,
	WX_VDEV_FILLMODE,
	WX_VDEV_DRIVER_INFO,
	WX_VDEV_GET_SORTBUFFER_SIZE,
	WX_VDEV_GET_MERGEBUFFER_SIZE
};
enum wWxVertexElem {
	WX_VELEM_POSITION,
	WX_VELEM_NORMAL,
	WX_VELEM_DIFFUSE,
	WX_VELEM_TEX1,
	WX_VELEM_TEX2
};
enum WVDTRANSFORMSTATETYPE {
	WVDTS_VIEW = 2,
	WVDTS_PROJECTION,
	WVDTS_TEXTURE0 = 16,
	WVDTS_TEXTURE1,
	WVDTS_TEXTURE2,
	WVDTS_TEXTURE3,
	WVDTS_TEXTURE4,
	WVDTS_TEXTURE5,
	WVDTS_TEXTURE6,
	WVDTS_TEXTURE7,
	WVDTS_FORCE_DWORD = 2147483647
};
enum WVDRENDERSTATETYPE {
	WVDRS_ZENABLE = 7,
	WVDRS_FILLMODE,
	WVDRS_SHADEMODE,
	WVDRS_ZWRITEENABLE = 14,
	WVDRS_ALPHATESTENABLE,
	WVDRS_LASTPIXEL,
	WVDRS_SRCBLEND = 19,
	WVDRS_DESTBLEND,
	WVDRS_CULLMODE = 22,
	WVDRS_ZFUNC,
	WVDRS_ALPHAREF,
	WVDRS_ALPHAFUNC,
	WVDRS_DITHERENABLE,
	WVDRS_ALPHABLENDENABLE,
	WVDRS_FOGENABLE,
	WVDRS_SPECULARENABLE,
	WVDRS_FOGCOLOR = 34,
	WVDRS_FOGTABLEMODE,
	WVDRS_FOGSTART,
	WVDRS_FOGEND,
	WVDRS_FOGDENSITY,
	WVDRS_RANGEFOGENABLE = 48,
	WVDRS_STENCILENABLE = 52,
	WVDRS_STENCILFAIL,
	WVDRS_STENCILZFAIL,
	WVDRS_STENCILPASS,
	WVDRS_STENCILFUNC,
	WVDRS_STENCILREF,
	WVDRS_STENCILMASK,
	WVDRS_STENCILWRITEMASK,
	WVDRS_TEXTUREFACTOR,
	WVDRS_WRAP0 = 128,
	WVDRS_WRAP1,
	WVDRS_WRAP2,
	WVDRS_WRAP3,
	WVDRS_WRAP4,
	WVDRS_WRAP5,
	WVDRS_WRAP6,
	WVDRS_WRAP7,
	WVDRS_CLIPPING,
	WVDRS_LIGHTING,
	WVDRS_AMBIENT = 139,
	WVDRS_FOGVERTEXMODE,
	WVDRS_COLORVERTEX,
	WVDRS_LOCALVIEWER,
	WVDRS_NORMALIZENORMALS,
	WVDRS_DIFFUSEMATERIALSOURCE = 145,
	WVDRS_SPECULARMATERIALSOURCE,
	WVDRS_AMBIENTMATERIALSOURCE,
	WVDRS_EMISSIVEMATERIALSOURCE,
	WVDRS_VERTEXBLEND = 151,
	WVDRS_CLIPPLANEENABLE,
	WVDRS_POINTSIZE = 154,
	WVDRS_POINTSIZE_MIN,
	WVDRS_POINTSPRITEENABLE,
	WVDRS_POINTSCALEENABLE,
	WVDRS_POINTSCALE_A,
	WVDRS_POINTSCALE_B,
	WVDRS_POINTSCALE_C,
	WVDRS_MULTISAMPLEANTIALIAS,
	WVDRS_MULTISAMPLEMASK,
	WVDRS_PATCHEDGESTYLE,
	WVDRS_DEBUGMONITORTOKEN = 165,
	WVDRS_POINTSIZE_MAX,
	WVDRS_INDEXEDVERTEXBLENDENABLE,
	WVDRS_COLORWRITEENABLE,
	WVDRS_TWEENFACTOR = 170,
	WVDRS_BLENDOP,
	WVDRS_POSITIONDEGREE,
	WVDRS_NORMALDEGREE,
	WVDRS_SCISSORTESTENABLE,
	WVDRS_SLOPESCALEDEPTHBIAS,
	WVDRS_ANTIALIASEDLINEENABLE,
	WVDRS_MINTESSELLATIONLEVEL = 178,
	WVDRS_MAXTESSELLATIONLEVEL,
	WVDRS_ADAPTIVETESS_X,
	WVDRS_ADAPTIVETESS_Y,
	WVDRS_ADAPTIVETESS_Z,
	WVDRS_ADAPTIVETESS_W,
	WVDRS_ENABLEADAPTIVETESSELLATION,
	WVDRS_TWOSIDEDSTENCILMODE,
	WVDRS_CCW_STENCILFAIL,
	WVDRS_CCW_STENCILZFAIL,
	WVDRS_CCW_STENCILPASS,
	WVDRS_CCW_STENCILFUNC,
	WVDRS_COLORWRITEENABLE1,
	WVDRS_COLORWRITEENABLE2,
	WVDRS_COLORWRITEENABLE3,
	WVDRS_BLENDFACTOR,
	WVDRS_SRGBWRITEENABLE,
	WVDRS_DEPTHBIAS,
	WVDRS_WRAP8 = 198,
	WVDRS_WRAP9,
	WVDRS_WRAP10,
	WVDRS_WRAP11,
	WVDRS_WRAP12,
	WVDRS_WRAP13,
	WVDRS_WRAP14,
	WVDRS_WRAP15,
	WVDRS_SEPARATEALPHABLENDENABLE,
	WVDRS_SRCBLENDALPHA,
	WVDRS_DESTBLENDALPHA,
	WVDRS_BLENDOPALPHA,
	WVDRS_FORCE_DWORD = 2147483647
};
enum WVDTEXTURESTAGESTATETYPE {
	WVDTSS_COLOROP = 1,
	WVDTSS_COLORARG1,
	WVDTSS_COLORARG2,
	WVDTSS_ALPHAOP,
	WVDTSS_ALPHAARG1,
	WVDTSS_ALPHAARG2,
	WVDTSS_BUMPENVMAT00,
	WVDTSS_BUMPENVMAT01,
	WVDTSS_BUMPENVMAT10,
	WVDTSS_BUMPENVMAT11,
	WVDTSS_TEXCOORDINDEX,
	WVDTSS_BUMPENVLSCALE = 22,
	WVDTSS_BUMPENVLOFFSET,
	WVDTSS_TEXTURETRANSFORMFLAGS,
	WVDTSS_COLORARG0 = 26,
	WVDTSS_ALPHAARG0,
	WVDTSS_RESULTARG,
	WVDTSS_CONSTANT = 32,
	WVDTSS_FORCE_DWORD = 2147483647
};
enum WVDSAMPLERSTATETYPE {
	WVDSAMP_ADDRESSU = 1,
	WVDSAMP_ADDRESSV,
	WVDSAMP_ADDRESSW,
	WVDSAMP_BORDERCOLOR,
	WVDSAMP_MAGFILTER,
	WVDSAMP_MINFILTER,
	WVDSAMP_MIPFILTER,
	WVDSAMP_MIPMAPLODBIAS,
	WVDSAMP_MAXMIPLEVEL,
	WVDSAMP_MAXANISOTROPY,
	WVDSAMP_SRGBTEXTURE,
	WVDSAMP_ELEMENTINDEX,
	WVDSAMP_DMAPOFFSET,
	WVDSAMP_FORCE_DWORD = 2147483647
};
enum WVDFXPARAMETERTYPE {
	WVDFXPARAM_WORLDVIEWPROJECTION,
	WVDFXPARAM_WORLD,
	WVDFXPARAM_WORLDVIEW,
	WVDFXPARAM_PREV_VIEW,
	WVDFXPARAM_VIEW,
	WVDFXPARAM_PROJECTION,
	WVDFXPARAM_VIEWPROJECTION,
	WVDFXPARAM_CONSTCOLOR,
	WVDFXPARAM_LIGHT_DIFFUSE,
	WVDFXPARAM_LIGHT_AMBIENT,
	WVDFXPARAM_LIGHT_DIRECTION,
	WVDFXPARAM_FOG_RANGE,
	WVDFXPARAM_FOG_COLOR,
	WVDFXPARAM_CAMERAPOSITION,
	WVDFXPARAM_SHCOEFF,
	WVDFXPARAM_EXTRATEXTURE,
	WVDFXPARAM_REFTEXTURE,
	WVDFXPARAM_MATERIALCOLOR,
	WVDFXPARAM_TEXTURE_TRANSFORM,
	WVDFXPARAM_BLUR_UVOFFSET,
	WVDFXPARAM_SHADOW_COLOR,
	WVDFXPARAM_NUM
};

// A transformed, lit vertex (PDB layout, size 0x28).
struct WTVertex {
	void SetPosition(const WVector& v) { x = v.x; y = v.y; z = v.z; }   // wvideo.h:43

	union {
		struct { float sx, sy, sz; };
		struct { float x, y, z; };
	};
	float rhw;                  // 0xc
	unsigned long diffuse;      // 0x10
	float tu;                   // 0x14
	float tv;                   // 0x18
	float lu;                   // 0x1c
	float lv;                   // 0x20
	float vz;                   // 0x24
};
CHECK_SIZE(WTVertex, 0x28);
CHECK_FIELD(WTVertex, sx, 0x0);
CHECK_FIELD(WTVertex, x, 0x0);
CHECK_FIELD(WTVertex, y, 0x4);
CHECK_FIELD(WTVertex, z, 0x8);
CHECK_FIELD(WTVertex, rhw, 0xc);
CHECK_FIELD(WTVertex, diffuse, 0x10);
CHECK_FIELD(WTVertex, tu, 0x14);
CHECK_FIELD(WTVertex, tv, 0x18);
CHECK_FIELD(WTVertex, lu, 0x1c);
CHECK_FIELD(WTVertex, lv, 0x20);
CHECK_FIELD(WTVertex, vz, 0x24);

// The render state of one batch of indexed triangles (PDB layout, size 0x5c).
// xiFlag0 carries the texture handle and the render flags.
struct WxBatchState {
	WxBatchState() : xiFlag0(0), xiFlag1(0), xiBaseVtxIdx(0), xnVtxs(0), xiBaseIdxIdx(0), xnIdxs(0), xdwDiffuse(0xffffffff), xfDepth(3.402823466e+38F), xnmTransfs(0), xpapmW(0), xpapmO(0) {
		xmW.Reset();   // xfDepth starts at FLT_MAX (float.h is not in the toolchain copy)
	}

	int xiFlag0;                        // 0x0
	int xiFlag1;                        // 0x4
	int xiBaseVtxIdx;                   // 0x8
	int xnVtxs;                         // 0xc
	int xiBaseIdxIdx;                   // 0x10
	int xnIdxs;                         // 0x14
	unsigned long xdwDiffuse;           // 0x18
	WMatrix xmW;                        // 0x1c
	float xfDepth;                      // 0x4c
	int xnmTransfs;                     // 0x50
	const WMatrix* const* xpapmW;       // 0x54
	const WMatrix* const* xpapmO;       // 0x58
};
CHECK_SIZE(WxBatchState, 0x5c);
CHECK_FIELD(WxBatchState, xiFlag0, 0x0);
CHECK_FIELD(WxBatchState, xiFlag1, 0x4);
CHECK_FIELD(WxBatchState, xiBaseVtxIdx, 0x8);
CHECK_FIELD(WxBatchState, xnVtxs, 0xc);
CHECK_FIELD(WxBatchState, xiBaseIdxIdx, 0x10);
CHECK_FIELD(WxBatchState, xnIdxs, 0x14);
CHECK_FIELD(WxBatchState, xdwDiffuse, 0x18);
CHECK_FIELD(WxBatchState, xmW, 0x1c);
CHECK_FIELD(WxBatchState, xfDepth, 0x4c);
CHECK_FIELD(WxBatchState, xnmTransfs, 0x50);
CHECK_FIELD(WxBatchState, xpapmW, 0x54);
CHECK_FIELD(WxBatchState, xpapmO, 0x58);

// The engine's abstract video device (PDB layout, size 0x14).
class WVideoDev : public WDevice {
public:
	virtual ~WVideoDev();                                                                           // vtable 0x0
	virtual wVDevState GetDeviceState() = 0;                                                        // vtable 0x10
	virtual void SetMainThreadId(unsigned long id);                                                 // vtable 0x14
	virtual void DrawPolygonFan(WTVertex** ppVtx, int nVtx, int a, int b, unsigned long c) = 0;     // vtable 0x18
	virtual void DrawIndexedTriangles(WTVertex* pVtx, int nVtx, unsigned short* pIdx, int nIdx, int a, int b);  // vtable 0x1c
	virtual void DrawLine(WTVertex** ppVtx, int nVtx) = 0;                                          // vtable 0x20
	virtual int Command(wVDevMessage msg, int wParam, int lParam) = 0;                              // vtable 0x24
	virtual int CreateTexture(tagBITMAPINFO* pInfo, int flags) = 0;                                 // vtable 0x28
	virtual void UpdateTexture(int tex, tagBITMAPINFO* pInfo, void* pBits, unsigned long flags) = 0;  // vtable 0x2c
	virtual void DestroyTexture(int tex) = 0;                                                       // vtable 0x30
	virtual int UploadCompressedTexture(void* pData, unsigned int size, int flags) = 0;             // vtable 0x34
	virtual void UpdateCompressedTexture(int tex, void* pData, unsigned int size, int flags) = 0;   // vtable 0x38
	virtual void FixTexturePart(int tex, const tagRECT& rect, tagBITMAPINFO* pInfo, void* pBits, int flags) = 0;  // vtable 0x3c
	virtual bool IsTextureFilled(int tex) = 0;                                                      // vtable 0x40
	virtual int GetTextureWidth(int tex) = 0;                                                       // vtable 0x44
	virtual int GetTextureHeight(int tex) = 0;                                                      // vtable 0x48
	virtual void SetRenderTargetSizeInfo(int index, const WRenderToTextureSizeInfo& info) = 0;      // vtable 0x4c
	virtual bool BeginScene() = 0;                                                                  // vtable 0x50
	virtual void EndScene() = 0;                                                                    // vtable 0x54
	virtual void Paint() = 0;                                                                       // vtable 0x58
	virtual void SetGlobalRenderState(int state, int value) = 0;                                    // vtable 0x5c
	virtual bool IsSupportVS() = 0;                                                                 // vtable 0x60
	virtual bool IsSupportPS() = 0;                                                                 // vtable 0x64
	virtual bool IsSupportMRT() = 0;                                                                // vtable 0x68
	virtual bool IsSupportClipPlane() = 0;                                                          // vtable 0x6c
	virtual void Clear(unsigned long color, int flags, float z) = 0;                                // vtable 0x70
	virtual int GetWidth() = 0;                                                                     // vtable 0x74
	virtual int GetHeight() = 0;                                                                    // vtable 0x78
	virtual int GetBackBufferBpp() = 0;                                                             // vtable 0x7c
	virtual bool IsWindowed() = 0;                                                                  // vtable 0x80
	virtual bool IsFillScreenMode() = 0;                                                            // vtable 0x84
	virtual float GetMonitorSupportFPS() = 0;                                                       // vtable 0x88
	virtual bool SetFogEnable(bool enable) = 0;                                                     // vtable 0x8c
	virtual void SetFogState(float start, float end, unsigned long color) = 0;                      // vtable 0x90
	virtual WVideoDev* MakeClone(char* name, HWND__* hWnd, int flags) = 0;                          // vtable 0x94
	virtual bool Reset(bool windowed, int width, int height, int bpp, long refresh, int flags) = 0;  // vtable 0x98
	virtual unsigned long VertexSize(unsigned long fvf) = 0;                                        // vtable 0x9c
	virtual unsigned char xGetStride(int type) = 0;                                                 // vtable 0xa0
	virtual bool xHasVertexElem(unsigned long fvf, wWxVertexElem elem) = 0;                         // vtable 0xa4
	virtual int xGetVertexElemOffset(unsigned long fvf, wWxVertexElem elem) = 0;                    // vtable 0xa8
	virtual int xGetBlendWeightSize(unsigned long fvf) = 0;                                         // vtable 0xac
	virtual unsigned long xDetermineFVF(int a, int b, int c) = 0;                                   // vtable 0xb0
	virtual unsigned long xDetermineBufferUsage(unsigned long fvf) = 0;                             // vtable 0xb4
	virtual int xCreateVertexBuffer(int size, unsigned long fvf, unsigned long usage) = 0;          // vtable 0xb8
	virtual int xCreateIndexBuffer(int size, unsigned long usage) = 0;                              // vtable 0xbc
	virtual void xReleaseVertexBuffer(int vb) = 0;                                                  // vtable 0xc0
	virtual void xReleaseIndexBuffer(int ib) = 0;                                                   // vtable 0xc4
	virtual unsigned char* xLockVertexBuffer(int vb, unsigned int offset, unsigned int size) = 0;   // vtable 0xc8
	virtual unsigned char* xLockIndexBuffer(int ib, unsigned int offset, unsigned int size) = 0;    // vtable 0xcc
	virtual void xUnlockVertexBuffer(int vb) = 0;                                                   // vtable 0xd0
	virtual void xUnlockIndexBuffer(int ib) = 0;                                                    // vtable 0xd4
	virtual void xDrawIndexedTriangles(const WxViewState& view, const WxBatchState& batch) = 0;     // vtable 0xd8
	virtual void xSetTransform(WVDTRANSFORMSTATETYPE type, const WMatrix4& m) = 0;                  // vtable 0xdc
	virtual void xSetPrevViewTransform(const WMatrix4& m) = 0;                                      // vtable 0xe0
	virtual void SetShaderSource(const char* src) = 0;                                              // vtable 0xe4
	virtual void BeginUsingCustomRenderState() = 0;                                                 // vtable 0xe8
	virtual void SetCustomRenderState(WVDRENDERSTATETYPE state, unsigned long value) = 0;           // vtable 0xec
	virtual void SetCustomTextureStageState(unsigned long stage, WVDTEXTURESTAGESTATETYPE type, unsigned long value) = 0;  // vtable 0xf0
	virtual void SetCustomTransform(WVDTRANSFORMSTATETYPE type, const WMatrix4& m) = 0;             // vtable 0xf4
	virtual void SetCustomTexture(unsigned long stage, int tex) = 0;                                // vtable 0xf8
	virtual void SetCustomClipPlane(unsigned long index, const WPlane& plane) = 0;                  // vtable 0xfc
	virtual void SetCustomSamplerState(unsigned long sampler, WVDSAMPLERSTATETYPE type, unsigned long value) = 0;  // vtable 0x100
	virtual void SetCustomFxMacro(unsigned long macro) = 0;                                         // vtable 0x104
	virtual void SetCustomFxParamInt(WVDFXPARAMETERTYPE param, int value) = 0;                      // vtable 0x108
	virtual void SetCustomFxParamVector2(WVDFXPARAMETERTYPE param, const WVector2D& v) = 0;         // vtable 0x10c
	virtual void SetCustomFxParamVector3(WVDFXPARAMETERTYPE param, const WVector& v) = 0;           // vtable 0x110
	virtual void SetCustomFxParamVector4(WVDFXPARAMETERTYPE param, const WVector4& v) = 0;          // vtable 0x114
	virtual void SetCustomFxParamMatrix(WVDFXPARAMETERTYPE param, const WMatrix4& m) = 0;           // vtable 0x118
	virtual void SetCustomFxParamTexture(WVDFXPARAMETERTYPE param, int tex) = 0;                    // vtable 0x11c
	virtual void EndUsingCustomRenderState() = 0;                                                   // vtable 0x120
	virtual bool BeginRenderToTexture(const WRenderToTextureParam& param) = 0;                      // vtable 0x124
	virtual void EndRenderToTexture(const WRenderToTextureParam& param) = 0;                        // vtable 0x128
	virtual bool SupportRenderTargetFormat() = 0;                                                   // vtable 0x12c
	virtual bool IsSupportedDisplayMode(bool windowed, int width, int height, int bpp) = 0;         // vtable 0x130
	virtual bool GetWindowDisplayMode(int& width, int& height, int& bpp) = 0;                       // vtable 0x134
	virtual int GetBufferingMeshNum() = 0;                                                          // vtable 0x138
	virtual void SetViewPort(unsigned long x, unsigned long y, unsigned long width, unsigned long height) = 0;  // vtable 0x13c

	unsigned long GetRenderCount();
	void SetRenderCount(unsigned long count);

protected:
	unsigned long m_mainThreadId;   // 0x4
	unsigned long m_renderCount;    // 0x8

public:
	float m_clip_scale_z;           // 0xc
	float m_clip_near_scale;        // 0x10

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(WVideoDev, 0x14);
BEGIN_LAYOUT_CHECK(WVideoDev)
	CHECK_FIELD(WVideoDev, m_mainThreadId, 0x4);
	CHECK_FIELD(WVideoDev, m_renderCount, 0x8);
	CHECK_FIELD(WVideoDev, m_clip_scale_z, 0xc);
	CHECK_FIELD(WVideoDev, m_clip_near_scale, 0x10);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WVIDEO_H
