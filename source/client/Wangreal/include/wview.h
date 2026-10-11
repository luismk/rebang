// Original: d:\build\custom\temp\code\client\wangreal\include\wview.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WVIEW_H
#define PG_CLIENT_WANGREAL_INCLUDE_WVIEW_H

#include "..\..\defines.h"
#include "wmath.h"
#include "wresource.h"
#include "wresrcmng.h"
#include "wscene.h"
#include "wtypes.h"

// wview.cpp emits CheckReflectiveAndConvertCullFlag out of line, wbone
// ProcessEffect.
#ifndef WVIEW_WVIEW_INLINE
#define WVIEW_WVIEW_INLINE inline
#endif
#ifndef WBONE_WVIEW_INLINE
#define WBONE_WVIEW_INLINE inline
#endif

class Waabb;
class Wobb;
class WVideoDev;
struct WTVertex;
struct WxBatchState;

// The transforms and light the view hands to the video device (PDB layout,
// size 0x9c).
struct WxViewState {
	LightSet xLight;        // 0x0
	WMatrix4 xmView;        // 0x1c
	WMatrix4 xmProj;        // 0x5c
};
CHECK_SIZE(WxViewState, 0x9c);
CHECK_FIELD(WxViewState, xLight, 0x0);
CHECK_FIELD(WxViewState, xmView, 0x1c);
CHECK_FIELD(WxViewState, xmProj, 0x5c);

// A camera with its projection, frustum and render state (PDB layout,
// size 0x1b1c). The inline accessors carry their wview.h line.
class WView : public WResource {
	LAYOUT_CHECK_FRIEND;
public:
	enum UPDATE_MODE {
		BEGINESCENE = 1,
		CLEARZBUFF = 2,
		CLEARFRAME = 4
	};
	enum UPDATE_TRANSFORM {
		UPDATE_NONE = 0,
		UPDATE_VIEW = 1,
		UPDATE_PROJ = 2
	};
	enum PROJECTION_MODE {
		PERSPECTIVE = 0,
		PARALLEL = 1,
		NUM_PROJECTION_MODE = 2
	};

	WView(void);
	virtual ~WView(void) {}

	virtual void BeginScene(void);
	virtual void Clear(unsigned long flags, int color);
	virtual void EndScene(void);
	virtual void Flush(unsigned long flags);
	virtual void Render(void);
	virtual void DrawPolygonFan(WTVertex** vl, int num, int flags, bool clip);
	virtual void DrawIndexedTriangles(WTVertex* vtx, int nvtx, unsigned short* idx, int nidx, int flags, int texhandle);
	virtual bool IsShadowView(void) { return false; }   // wview.h:119
	virtual void DrawIndexedTrianglesDirect(WTVertex* vtx, int nvtx, unsigned short* idx, int nidx, int flags, int texhandle);

	void SetFOV(float fov);
	void SetClip(float clipNear, float clipFar, bool fastclip);
	bool SetFogEnable(bool enable);
	void SetFogState(float fogStart, float fogEnd, unsigned long color);
	void SetCamera(const WMatrix& cam);
	void SetPrevCamera(const WMatrix& cam);
	WVector Projection(const WVector& v);
	void Projection2(WTVertex* vtx, const WVector& v);
	void DrawProjPolygonFan(WTVertex** vl, int num, int flags);
	void DrawLine2D(const WVector& v1, const WVector& v2, unsigned long color, float lineWidth);
	void DrawLine2D(const _WPOINT& a, const _WPOINT& b, unsigned long colorA, unsigned long colorB, int flags);
	void DrawLine2D(const _WPOINT& a, const _WPOINT& b, unsigned long color, int flags);
	void DrawLine(const WVector& a, unsigned long colorA, const WVector& b, unsigned long colorB, int flags);
	void DrawAABB(const Waabb& box, unsigned long color, bool solid, int flags);
	void DrawAABB(WMatrix& m, const Waabb& box, unsigned long color, bool solid, int flags);
	void DrawOBB(const Wobb& box, unsigned long color);
	void DrawOBB(const WMatrix& m, unsigned long color);
	void DrawSphere(const WSphere& sphere, unsigned long color, int slices, int stacks);
	void DrawSphere(const WVector& center, float radius, unsigned long color, int slices, int stacks);
	void Draw2DTexture(const WRect& dst, const WRect& src, int texhandle, unsigned long color, unsigned long flags);
	bool InFrustum(const WSphere& sphere) {                                     // wview.h:384
		for (int i = 0; i < 6; i++) {
			if (frustum[i].x * sphere.pos.x + frustum[i].z * sphere.pos.z + frustum[i].y * sphere.pos.y + frustum[i].dis > sphere.radius)
				return false;
		}
		return true;
	}
	bool InFrustum(const Waabb& box) {                                          // wview.h:406
		for (int i = 0; i < 6; i++) {
			float z = frustum[i].z < 0.0f ? box.max.z : box.min.z;
			float y = frustum[i].y < 0.0f ? box.max.y : box.min.y;
			float x = frustum[i].x < 0.0f ? box.max.x : box.min.x;
			if (frustum[i].x * x + frustum[i].z * z + frustum[i].y * y + frustum[i].dis > 0.0f)
				return false;
		}
		return true;
	}
	bool InFrustum(const WVector& v);
	bool InFrustumSafe(const Waabb& box) {                                      // wview.h:419
		for (int i = 0; i < 6; i++) {
			float z = frustumSafe[i].z < 0.0f ? box.max.z : box.min.z;
			float y = frustumSafe[i].y < 0.0f ? box.max.y : box.min.y;
			float x = frustumSafe[i].x < 0.0f ? box.max.x : box.min.x;
			if (frustumSafe[i].x * x + frustumSafe[i].z * z + frustumSafe[i].y * y + frustumSafe[i].dis > 0.0f)
				return false;
		}
		return true;
	}
	bool InFrustumSafe(const WSphere& sphere) {                                 // wview.h:395
		for (int i = 0; i < 6; i++) {
			if (!(frustumSafe[i].x * sphere.pos.x + frustumSafe[i].z * sphere.pos.z + frustumSafe[i].y * sphere.pos.y + frustumSafe[i].dis + sphere.radius < 0.0f))
				return false;
		}
		return true;
	}

	const WMatrix& GetCamera(void) const { return camera; }                     // wview.h:153
	const WMatrix& GetInvCamera(void) const { return invcamera; }               // wview.h:154
	float GetClipNearValue(void) const { return clip_near; }                    // wview.h:156
	float GetClipFarValue(void) const { return clip_far; }                      // wview.h:157
	float GetFOV(void) const { return SCREEN_YS / SCREEN_XS * FOV * (4.0f / 3.0f); }   // wview.h:158
	float GetWidth(void) const { return SCREEN_XS; }                            // wview.h:159
	float GetHeight(void) const { return SCREEN_YS; }                           // wview.h:160
	float GetRatio(void) const { return SCREEN_YS / SCREEN_XS; }                // wview.h:161
	void SetViewport(float width, float height);
	WVector2D GetScreenCenter(void) const { return WVector2D(m_center_x, m_center_y); }  // wview.h:164
	void SetScreenCenter(const WVector2D& c) { SetScreenCenter(c.x, c.y); }    // wview.h:166
	void ResetScreenCenter(void) { SetScreenCenter(SCREEN_XS * 0.5f, SCREEN_YS * 0.5f); }  // wview.h:167
	void SetScreenCenter(float x, float y);
	void GetScreen2World(float sx, float sy, WVector& from, WVector& to);
	void SetClippingArea(const WRect& rect);
	void ResetClippingArea(void);
	void UpdateCamera(void);
	float GetScale(void) const { return m_scale; }                              // wview.h:181
	void SetScale(float scale);
	WVideoDev* GetVideoDevice(void) const { return GetResrcManager()->VideoReference(); }  // wview.h:196
	float GetFOV_Unmodified(void) const { return FOV; }                         // wview.h:200

protected:
	void SetFOV_Unmodified(float fov);
	const WMatrix& GetLastCamera(void) { return lastcam; }                      // wview.h:206

public:
	bool ProcessEffect(void);
	bool GetReflective(void) const { return m_isReflective; }                   // wview.h:253
	void SetReflective(bool reflective) { m_isReflective = reflective; }       // wview.h:254

protected:
	void CheckReflectiveAndConvertCullFlag(int& flags) const;

public:
	const WxViewState& xGetViewState(void) const { return m_xViewState; }  // wview.h:279
	void xSetLight(unsigned long index, const LightSet& light);
	LightSet* xGetLight(void);
	void xDrawIndexedTriangles(const WxBatchState& state);
	void xScaleProjMat(int width, int height);
	float xGetProjScale(void) const { return proj_scale; }                      // wview.h:284
	// wview.h:286
	void xConvScreenRectByProjScale(WRect& rect) const {
		if (proj_scale > 1.0f) {
			rect.x = (rect.x - (left + 0.5f) * GetWidth()) * proj_scale;
			rect.y = (rect.y - (top + 0.5f) * GetHeight()) * proj_scale;
			rect.w = rect.w * proj_scale;
			rect.h = rect.h * proj_scale;
		}
	}

protected:
	void CheckViewAndProjTransformUpdateToVideo(void);
	WVector Projection_Perspective(const WVector& v);
	WVector Projection_Parallel(const WVector& v);
	void Projection2_Perspective(WTVertex* vtx, const WVector& v);
	void Projection2_Parallel(WTVertex* vtx, const WVector& v);
	void UpdateProjectionTransform_Perspective(void);
	void UpdateProjectionTransform_Parallel(void);
	void UpdateCamera_Perspective(void);
	void UpdateCamera_Parallel(void);
	void GetScreen2World_Perspective(float sx, float sy, WVector& from, WVector& to);
	void GetScreen2World_Parallel(float sx, float sy, WVector& from, WVector& to);
	void UpdateViewTransform(void);
	void ClipPlane(WTVertex* out, const WTVertex* a, const WTVertex* b, int flags, const WPlane& plane);

public:
	PROJECTION_MODE GetProjectionMode(void) const { return m_projMode; }        // wview.h:335
	void SetProjectionMode(PROJECTION_MODE mode);

	WMatrix camera;                         // 0x2c
	WMatrix invcamera;                      // 0x5c

protected:
	WMatrix matrix;                         // 0x8c
	WPlane frustum[6];                      // 0xbc
	WPlane frustumSafe[6];                  // 0x11c
	float scalex;                           // 0x17c
	float scaley;                           // 0x180
	float m_center_x;                       // 0x184
	float m_center_y;                       // 0x188
	float clip_near;                        // 0x18c
	float clip_far;                         // 0x190
	float clip_scaled_near;                 // 0x194
	float clip_scaled_far;                  // 0x198
	float SCREEN_XS;                        // 0x19c
	float SCREEN_YS;                        // 0x1a0
	unsigned long m_cliptype;               // 0x1a4
	bool m_fastclip;                        // 0x1a8
	WRect m_clipArea;                       // 0x1ac
	float clip_scale_z;                     // 0x1bc
	float clip_near_scale;                  // 0x1c0
	float FOV;                              // 0x1c4
	WVector m_temp[512];                    // 0x1c8
	WMatrix lastcam;                        // 0x19c8
	int update;                             // 0x19f8
	float proj_scale;                       // 0x19fc
	float left;                             // 0x1a00
	float right;                            // 0x1a04
	float top;                              // 0x1a08
	float bottom;                           // 0x1a0c
	bool m_bProcessEffect;                  // 0x1a10
	bool m_bDisableFog;                     // 0x1a11
	bool m_isReflective;                    // 0x1a12
	WxViewState m_xViewState;               // 0x1a14
	bool m_xNeedToUpdateViewTransfToVideo;  // 0x1ab0
	bool m_xNeedToUpdateProjTransfToVideo;  // 0x1ab1

public:
	static const WMatrix4 ms_uvConvMat;

protected:
	static WVector (WView::*ms_fnProjection[NUM_PROJECTION_MODE])(const WVector& v);
	static void (WView::*ms_fnProjection2[NUM_PROJECTION_MODE])(WTVertex* vtx, const WVector& v);
	static void (WView::*ms_fnUpdateProjectionTransform[NUM_PROJECTION_MODE])(void);
	static void (WView::*ms_fnUpdateCamera[NUM_PROJECTION_MODE])(void);
	static void (WView::*ms_fnGetScreen2World[NUM_PROJECTION_MODE])(float sx, float sy, WVector& from, WVector& to);
	PROJECTION_MODE m_projMode;             // 0x1ab4
	float m_scale;                          // 0x1ab8
	WPlane frustumByCam[6];                 // 0x1abc
};
CHECK_SIZE(WView, 0x1b1c);

// wview.cpp: the camera matrix of an eye at pos looking at at.
WMatrix __cdecl MakeCameraMatrix(const WVector& pos, const WVector& at);
BEGIN_LAYOUT_CHECK(WView)
	CHECK_FIELD(WView, camera, 0x2c);
	CHECK_FIELD(WView, invcamera, 0x5c);
	CHECK_FIELD(WView, matrix, 0x8c);
	CHECK_FIELD(WView, frustum, 0xbc);
	CHECK_FIELD(WView, frustumSafe, 0x11c);
	CHECK_FIELD(WView, scalex, 0x17c);
	CHECK_FIELD(WView, scaley, 0x180);
	CHECK_FIELD(WView, m_center_x, 0x184);
	CHECK_FIELD(WView, m_center_y, 0x188);
	CHECK_FIELD(WView, clip_near, 0x18c);
	CHECK_FIELD(WView, clip_far, 0x190);
	CHECK_FIELD(WView, clip_scaled_near, 0x194);
	CHECK_FIELD(WView, clip_scaled_far, 0x198);
	CHECK_FIELD(WView, SCREEN_XS, 0x19c);
	CHECK_FIELD(WView, SCREEN_YS, 0x1a0);
	CHECK_FIELD(WView, m_cliptype, 0x1a4);
	CHECK_FIELD(WView, m_fastclip, 0x1a8);
	CHECK_FIELD(WView, m_clipArea, 0x1ac);
	CHECK_FIELD(WView, clip_scale_z, 0x1bc);
	CHECK_FIELD(WView, clip_near_scale, 0x1c0);
	CHECK_FIELD(WView, FOV, 0x1c4);
	CHECK_FIELD(WView, m_temp, 0x1c8);
	CHECK_FIELD(WView, lastcam, 0x19c8);
	CHECK_FIELD(WView, update, 0x19f8);
	CHECK_FIELD(WView, proj_scale, 0x19fc);
	CHECK_FIELD(WView, left, 0x1a00);
	CHECK_FIELD(WView, right, 0x1a04);
	CHECK_FIELD(WView, top, 0x1a08);
	CHECK_FIELD(WView, bottom, 0x1a0c);
	CHECK_FIELD(WView, m_bProcessEffect, 0x1a10);
	CHECK_FIELD(WView, m_bDisableFog, 0x1a11);
	CHECK_FIELD(WView, m_isReflective, 0x1a12);
	CHECK_FIELD(WView, m_xViewState, 0x1a14);
	CHECK_FIELD(WView, m_xNeedToUpdateViewTransfToVideo, 0x1ab0);
	CHECK_FIELD(WView, m_xNeedToUpdateProjTransfToVideo, 0x1ab1);
	CHECK_FIELD(WView, m_projMode, 0x1ab4);
	CHECK_FIELD(WView, m_scale, 0x1ab8);
	CHECK_FIELD(WView, frustumByCam, 0x1abc);
END_LAYOUT_CHECK

// wview.h:244
WBONE_WVIEW_INLINE bool WView::ProcessEffect(void) {
	return m_bProcessEffect;
}

// wview.h:258
WVIEW_WVIEW_INLINE void WView::CheckReflectiveAndConvertCullFlag(int& flags) const {
	if (m_isReflective) {
		switch (flags & 0xc00) {
		case 0:
			flags |= 0x800;
			break;
		case 0x800:
			flags &= ~0x800;
			break;
		}
	}
}

extern WView* g_view;

// wview.h:344
inline unsigned long ULblend(unsigned long from, unsigned long to, unsigned char ratio) {
	return ((((from & 0x00FF00FF) * (255 - ratio) + (to & 0x00FF00FF) * ratio) >> 8) & 0x00FF00FF)
	     | ((((from >> 8) & 0x00FF00FF) * (255 - ratio) + ((to >> 8) & 0x00FF00FF) * ratio) & 0xFF00FF00);
}

#endif // PG_CLIENT_WANGREAL_INCLUDE_WVIEW_H
