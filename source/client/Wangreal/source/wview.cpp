// Original: d:\build\custom\temp\code\client\wangreal\source\wview.cpp
#define WVIEW_WMATH_INLINE
#define WVIEW_WVIEW_INLINE
#include "../include/wview.h"
#include "../include/wvideo.h"
#include "../include/wutil.h"

// The vertex offset into the batch buffer, reset at each flush.
static int g_foffset;

// wmath (not declared in wmath.h yet)
WVector RotVec(const WVector& v, const WMatrix& m);

WVector (WView::*WView::ms_fnProjection[WView::NUM_PROJECTION_MODE])(const WVector& v) = {
	&WView::Projection_Perspective,
	&WView::Projection_Parallel
};
void (WView::*WView::ms_fnProjection2[WView::NUM_PROJECTION_MODE])(WTVertex* vtx, const WVector& v) = {
	&WView::Projection2_Perspective,
	&WView::Projection2_Parallel
};
void (WView::*WView::ms_fnUpdateProjectionTransform[WView::NUM_PROJECTION_MODE])(void) = {
	&WView::UpdateProjectionTransform_Perspective,
	&WView::UpdateProjectionTransform_Parallel
};
void (WView::*WView::ms_fnUpdateCamera[WView::NUM_PROJECTION_MODE])(void) = {
	&WView::UpdateCamera_Perspective,
	&WView::UpdateCamera_Parallel
};
void (WView::*WView::ms_fnGetScreen2World[WView::NUM_PROJECTION_MODE])(float sx, float sy, WVector& from, WVector& to) = {
	&WView::GetScreen2World_Perspective,
	&WView::GetScreen2World_Parallel
};

// wview.cpp:60
WView::WView(void) {
	update = 3;
	FOV = 1.1780972f;
	clip_near = 6.3999996f;
	clip_far = 1600.0f;
	clip_scale_z = 1.004016f;
	clip_near_scale = 6.425702f;
	clip_scaled_near = 0.99600005f;
	clip_scaled_far = 249.00003f;
	m_fastclip = true;
	m_cliptype = 0;
	camera.Reset();
	lastcam.Reset();
	SetViewport(1.0f, 1.0f);
	m_bProcessEffect = true;
	proj_scale = 1.0f;
	left = -0.5f;
	right = 0.5f;
	top = -0.5f;
	bottom = 0.5f;
	m_bDisableFog = false;
	m_isReflective = false;
	m_xViewState.xLight.type = LightSet::W_LIGHT_DIRECTIONAL;
	m_xViewState.xLight.nearone = WVector::UNIT_NEG_Z;
	m_xViewState.xmView = WMatrix4::IDENTITY;
	m_xViewState.xmProj = WMatrix4::ZERO;
	update |= UPDATE_PROJ;
	m_xNeedToUpdateViewTransfToVideo = true;
	m_xNeedToUpdateProjTransfToVideo = true;
	m_projMode = PERSPECTIVE;
	m_scale = 1.0f;
	UpdateCamera();
}

// Definition of the UV conversion matrix declared in wview.h. This
// matrix converts from projection space to texture UV space. Use the
// WMatrix4 constructor to initialize the matrix since WMatrix4 defines
// a user constructor.
const WMatrix4 WView::ms_uvConvMat(
	0.5f, 0.0f, 0.0f, 0.0f,
	0.0f, -0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.0f, 1.0f
);


// wview.cpp:102
void WView::SetClip(float nearplane, float farplane, bool setToRenderer) {
	clip_near = nearplane;
	clip_far = farplane;
	clip_scale_z = farplane / (farplane - nearplane);
	clip_near_scale = clip_scale_z * nearplane;
	clip_scaled_near = nearplane / clip_near_scale;
	clip_scaled_far = farplane / clip_near_scale;

	if (setToRenderer) {
		WVideoDev* video = GetVideoDevice();

		if (video != NULL) {
			video->m_clip_scale_z = clip_scale_z;
			video->m_clip_near_scale = clip_near_scale;
		}
	}

	update |= UPDATE_PROJ;
}

// wview.cpp:134
void WView::Render(void) {
	WVideoDev* video = GetVideoDevice();

	video->EndScene();

	video->Paint();
}

// wview.cpp:150
void WView::SetPrevCamera(const WMatrix& mat) {
	WMatrix invcamera = ~mat;

	WMatrix4 mView;
	SetWMatrix4FromWMatrix(mView, invcamera);

	GetVideoDevice()->xSetPrevViewTransform(mView);
}

// wview.cpp:161
void WView::SetCamera(const WMatrix& mat) {
	update |= UPDATE_VIEW;
	lastcam = mat;
}

// wview.cpp:169
bool WView::SetFogEnable(bool enable) {
	WVideoDev* video = GetVideoDevice();

	if (video == NULL) {
		return false;
	}

	return video->SetFogEnable(enable);
}

// wview.cpp:182
void WView::SetFogState(float fogStart, float fogEnd, unsigned long color) {
	WVideoDev* video = GetVideoDevice();

	if (video != NULL) {
		video->SetFogState(fogStart, fogEnd, color);
	}
}

// wview.cpp:195
void WView::SetViewport(float xs, float ys) {
	m_center_x = xs * 0.5f;
	m_center_y = ys * 0.5f;
	SCREEN_XS = xs;
	SCREEN_YS = ys;
	m_clipArea = WRect(0.0f, 0.0f, xs, ys);
	update |= UPDATE_PROJ;
}

// wview.cpp:249
void WView::SetFOV(float fov) {
	FOV = SCREEN_XS / SCREEN_YS * fov * 0.75f;
	update |= UPDATE_PROJ;
}

// wview.cpp:255
void WView::DrawLine2D(const _WPOINT& p1, const _WPOINT& p2, unsigned long diffuse, int type) {
	WTVertex vtx[2];
	WTVertex* vl[3];

	vl[0] = &vtx[0];
	vtx[0].sx = p1.x;
	vtx[0].sy = p1.y;
	vtx[0].sz = 0.001f;
	vtx[0].rhw = 1.0f;
	vtx[0].diffuse = diffuse;

	vl[1] = &vtx[1];
	vtx[1].sx = p2.x;
	vtx[1].sy = p2.y;
	vtx[1].sz = 0.001f;
	vtx[1].rhw = 1.0f;
	vtx[1].diffuse = diffuse;

	vl[2] = NULL;

	DrawPolygonFan(vl, type | 0x4000000, 0, true);
}

// wview.cpp:281
void WView::DrawLine2D(const _WPOINT& p1, const _WPOINT& p2, unsigned long diffuse1, unsigned long diffuse2, int type) {
	WTVertex vtx[2];
	WTVertex* vl[3];

	vl[0] = &vtx[0];
	vtx[0].sx = p1.x;
	vtx[0].sy = p1.y;
	vtx[0].sz = 0.001f;
	vtx[0].rhw = 1.0f;
	vtx[0].diffuse = diffuse1;

	vl[1] = &vtx[1];
	vtx[1].sx = p2.x;
	vtx[1].sy = p2.y;
	vtx[1].sz = 0.001f;
	vtx[1].rhw = 1.0f;
	vtx[1].diffuse = diffuse2;

	vl[2] = NULL;

	DrawPolygonFan(vl, type | 0x4000000, 0, true);
}

// wview.cpp:306
void WView::Draw2DTexture(const WRect& src, const WRect& dest, int texHandle, unsigned long diffuse, unsigned long type) {
	static WTVertex vtx[4] = { 0 };
	static WTVertex* vl[5] = { &vtx[0], &vtx[1], &vtx[3], &vtx[2], NULL };

	for (int i = 0; i < 4; ++i) {
		vtx[i].sx = ((i & 1) ? dest.x + dest.w : dest.x) - 0.5f;
		vtx[i].sy = ((i & 2) ? dest.y + dest.h : dest.y) - 0.5f;
		vtx[i].sz = 0.001f;
		vtx[i].rhw = 1.0f;
		vtx[i].diffuse = diffuse;
		vtx[i].tu = (i & 1) ? src.x + src.w : src.x;
		vtx[i].tv = (i & 2) ? src.y + src.h : src.y;
	}

	DrawPolygonFan(vl, (texHandle & 0x7ff) | type | 0x20380000, 0, true);
}

// wview.cpp:329
void WView::DrawLine2D(const WVector& v1, const WVector& v2, unsigned long color, float lineWidth) {
	WTVertex vStart, vEnd;
	Projection2(&vStart, v1);
	Projection2(&vEnd, v2);

	WVector p1(vStart.sx, vStart.sy, 1.0f);
	WVector p2(vEnd.sx, vEnd.sy, 1.0f);
	WVector vRight = p1;
	vRight.z = 0.0f;
	WVector vUp = WCrossProduct(p2 - p1, vRight - p1);
	vUp.Normalize();
	vUp *= lineWidth;

	WVector vLT = p1 - vUp;
	WVector vLB = p2 - vUp;
	WVector vRB = p2 + vUp;
	WVector vRT = p1 + vUp;

	WTVertex v[4];
	WTVertex* pV[5] = { 0 };
	for (int i = 0; i < 4; ++i) {
		pV[i] = &v[i];
		v[i].diffuse = color;
		v[i].sz = 0.001f;
		v[i].rhw = 1.0f;
	}
	v[0].sx = vLT.x;
	v[0].sy = vLT.y;
	v[1].sx = vLB.x;
	v[1].sy = vLB.y;
	v[2].sx = vRB.x;
	v[2].sy = vRB.y;
	v[3].sx = vRT.x;
	v[3].sy = vRT.y;

	DrawPolygonFan(pV, 0x700000, 0, true);
}

// wview.cpp:389
void WView::DrawLine(const WVector& v1, unsigned long c1, const WVector& v2, unsigned long c2, int type) {
	WTVertex vtx[2];
	WTVertex* p[3];

	p[0] = &vtx[0];
	p[1] = &vtx[1];
	p[2] = NULL;
	vtx[0].diffuse = c1;
	vtx[1].diffuse = c2;
	vtx[0].SetPosition(v1);
	vtx[1].SetPosition(v2);

	DrawPolygonFan(p, type | 0x4000000, 0, false);
}

// wview.cpp:415
void WView::DrawAABB(const Waabb& aabb, unsigned long diffuse, bool solid, int type) {
}

// wview.cpp:455
void WView::DrawAABB(WMatrix& m, const Waabb& aabb, unsigned long diffuse, bool solid, int type) {
	static WTVertex vtr[4] = { 0 };
	static WTVertex* vtx[5] = { &vtr[0], &vtr[1], &vtr[2], &vtr[3], 0 };
	// The four corners of each face of the box, in fan order.
	static int box_p[6][4] = {
		{ 0, 2, 3, 1 },
		{ 4, 5, 7, 6 },
		{ 0, 1, 5, 4 },
		{ 3, 2, 6, 7 },
		{ 1, 3, 7, 5 },
		{ 2, 0, 4, 6 }
	};
	// Which corner of the box each vertex is (0 = min, 1 = max per axis).
	static int box_v[8][3] = {
		{ 0, 0, 0 },
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 1, 1, 0 },
		{ 0, 0, 1 },
		{ 1, 0, 1 },
		{ 0, 1, 1 },
		{ 1, 1, 1 }
	};
	WVector vt[8];
	int i;
	int j;

	vtr[0].diffuse = diffuse;
	vtr[1].diffuse = diffuse;
	vtr[2].diffuse = diffuse;
	vtr[3].diffuse = diffuse;
	for (i = 0; i < 8; i++) {
		vt[i] = WVector(box_v[i][0] ? aabb.max.x : aabb.min.x,
		                box_v[i][1] ? aabb.max.y : aabb.min.y,
		                box_v[i][2] ? aabb.max.z : aabb.min.z) * m;
	}
	for (j = 0; j < 6; j++) {
		for (i = 0; i < 4; i++) {
			if (solid) {
				vtr[i].SetPosition(vt[box_p[j][i]]);
				DrawPolygonFan(vtx, type | 0x400, 0, false);
			} else {
				DrawLine(vt[box_p[j][(i - 1) & 3]], diffuse, vt[box_p[j][i]], diffuse, type);
			}
		}
	}
}

// wview.cpp:490
void WView::DrawOBB(const WMatrix& mat, unsigned long diffuse) {
	// The four corners of each face of the box, in fan order.
	static int box_p[6][4] = {
		{ 0, 2, 3, 1 },
		{ 4, 5, 7, 6 },
		{ 0, 1, 5, 4 },
		{ 3, 2, 6, 7 },
		{ 1, 3, 7, 5 },
		{ 2, 0, 4, 6 }
	};
	// Which corner of the box each vertex is (0 = min, 1 = max per axis).
	static int box_v[8][3] = {
		{ 0, 0, 0 },
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 1, 1, 0 },
		{ 0, 0, 1 },
		{ 1, 0, 1 },
		{ 0, 1, 1 },
		{ 1, 1, 1 }
	};
	WVector vt[8];
	int i;
	int j;

	for (i = 0; i < 8; i++) {
		vt[i] = WVector(box_v[i][0] ? 1.0f : -1.0f, box_v[i][1] ? 1.0f : -1.0f, box_v[i][2] ? 1.0f : -1.0f) * mat;
	}
	for (j = 0; j < 6; j++) {
		for (i = 0; i < 4; i++) {
			DrawLine(vt[box_p[j][(i - 1) & 3]], diffuse, vt[box_p[j][i]], diffuse, 0);
		}
	}
}

// wview.cpp:506
void WView::DrawOBB(const Wobb& obb, unsigned long diffuse) {
	WMatrix m;
	m.pivot = obb.center;
	m.xa = obb.extend[0];
	m.ya = obb.extend[1];
	m.za = obb.extend[2];

	DrawOBB(m, diffuse);
}

// wview.cpp:517
void WView::DrawSphere(const WVector& pivot, float length, unsigned long diffuse, int seg, int offset) {
	WVector vtx[25];
	int i;

	if (seg > 24) {
		seg = 24;
	}

	for (i = 0; i < seg; i++) {
		float ang = (float)i * 6.28318548f / (float)seg;
		vtx[i] = pivot + camera.xa * length * cosf(ang) + camera.ya * length * sinf(ang);
	}

	int prev = seg - 1;
	for (i = 0; i < seg; i++) {
		if (offset == 0) {
			DrawLine(vtx[i], diffuse, vtx[prev], diffuse, 0);
		} else if ((i + 1) % (offset + 1) == 0) {
			DrawLine(vtx[i], 0xff00ff00, vtx[prev], 0xff00ff00, 0);
		} else {
			DrawLine(vtx[i], diffuse, vtx[prev], diffuse, 0);
		}
		prev = i;
	}
}

// wview.cpp:538
void WView::DrawSphere(const WSphere& sphere, unsigned long diffuse, int seg, int offset) {
	DrawSphere(sphere.pos, sphere.radius, diffuse, seg, offset);
}

// wview.cpp:616
void WView::Clear(unsigned long clearColor, int mode) {
	if (GetResrcManager() != NULL) {
		WVideoDev* video = GetVideoDevice();

		if (video != NULL) {
			switch (mode & (CLEARZBUFF | CLEARFRAME)) {
			case CLEARZBUFF:
				video->Clear(0, 2, 1.0f);
				break;
			case CLEARFRAME:
				video->Clear(clearColor, 1, 1.0f);
				break;
			case CLEARZBUFF | CLEARFRAME:
				video->Clear(clearColor, 3, 1.0f);
				break;
			}
		}
	}
}

// wview.cpp:642
void WView::BeginScene(void) {
	if (GetResrcManager() != NULL) {
		WVideoDev* video = GetVideoDevice();

		video->BeginScene();

		float sx;
		float sy;
		if ((sx = SCREEN_XS) != (float)video->GetWidth() || (sy = SCREEN_YS) != (float)video->GetHeight()) {
			SetViewport((float)video->GetWidth(), (float)video->GetHeight());
		}
	}

	update |= UPDATE_VIEW | UPDATE_PROJ;
	(this->*ms_fnUpdateCamera[m_projMode])();
}

// wview.cpp:659
void WView::EndScene(void) {
}

// wview.cpp:666
void WView::SetClippingArea(const WRect& rect) {
	m_clipArea.x = Max(0.0f, rect.x);
	m_clipArea.y = Max(0.0f, rect.y);
	m_clipArea.w = Min(rect.w, SCREEN_XS - m_clipArea.x);
	m_clipArea.h = Min(rect.h, SCREEN_YS - m_clipArea.y);

	WVideoDev* video = GetVideoDevice();

	if (video != NULL) {
		video->SetViewPort((int)m_clipArea.x, (int)m_clipArea.y, (int)m_clipArea.w, (int)m_clipArea.h);
	}

	update |= UPDATE_PROJ;
}

// wview.cpp:684
void WView::SetScreenCenter(float x, float y) {
	if (x != m_center_x || y != m_center_y) {
		m_center_x = x;
		m_center_y = y;

		update |= UPDATE_PROJ;
	}
}

// wview.cpp:696
void WView::UpdateCamera(void) {
	(this->*ms_fnUpdateCamera[m_projMode])();
}

// wview.cpp:835
void WView::DrawIndexedTriangles(WTVertex* p, int pnum, unsigned short* f, int fnum, int drawoption, int drawoption2) {
	if (proj_scale > 1.0f) {
		return;
	}

	DrawIndexedTrianglesDirect(p, pnum, f, fnum, m_bDisableFog ? drawoption & ~0x8000000 : drawoption, drawoption2);

	if (drawoption & 0x400000) {
		g_foffset += fnum;
	}
}

// wview.cpp:870
void WView::DrawIndexedTrianglesDirect(WTVertex* plist, int pn, unsigned short* flist, int fn, int type, int type2) {
	WVideoDev* video = GetVideoDevice();

	if (video == 0) {
		return;
	}

	if (type & 0x400000) {
		for (int i = 0; i < pn; i++) {
			plist[i].vz = TransformZ(plist[i].sx, plist[i].sy, plist[i].sz, invcamera);
		}
	}

	CheckViewAndProjTransformUpdateToVideo();

	CheckReflectiveAndConvertCullFlag(type2);

	video->DrawIndexedTriangles(plist, pn, flist, fn, type, type2);
}

// wview.cpp:895
void WView::SetFOV_Unmodified(float fov) {
	FOV = fov;
	update |= UPDATE_PROJ;
}

// wview.cpp:1408
void WView::GetScreen2World(float sx, float sy, WVector& start, WVector& ray) {
	(this->*ms_fnGetScreen2World[m_projMode])(sx, sy, start, ray);
}

// wview.cpp:1417
void WView::DrawPolygonFan(WTVertex** vl, int drawoption, int drawoption2, bool projected) {
	if (proj_scale > 1.0f) {
		return;
	}

	WVideoDev* video = GetVideoDevice();

	if (drawoption & 0x4000000) {
		if (vl[2] != 0) {
			float dx = vl[0]->sx - vl[1]->sx;
			float dy = vl[0]->sy - vl[1]->sy;
			if (dx <= 0.001f && dx >= -0.001f && dy <= 0.001f && dy >= -0.001f) {
				vl[1] = vl[2];
			}
		}

		if (drawoption & 0x400000) {
			int i = 0;
			if (projected) {
				for (; vl[i]; i++) {
					vl[i]->vz = -g_HUGE;
				}
			} else {
				for (; vl[i]; i++) {
					vl[i]->vz = TransformZ(vl[i]->sx, vl[i]->sy, vl[i]->sz, invcamera);
				}
			}
		}

		CheckViewAndProjTransformUpdateToVideo();
		CheckReflectiveAndConvertCullFlag(drawoption2);
		video->DrawPolygonFan(vl, m_bDisableFog ? drawoption & 0xf7ffffff : drawoption, 2, drawoption2, projected ? 4 : 2);
		return;
	}

	int n = 3;
	while (vl[n] != 0) {
		n++;
	}

	if (drawoption & 0x400000) {
		int i = 0;
		if (projected) {
			for (; vl[i]; i++) {
				vl[i]->vz = -g_HUGE;
			}
		} else {
			for (; vl[i]; i++) {
				vl[i]->vz = TransformZ(vl[i]->sx, vl[i]->sy, vl[i]->sz, invcamera);
			}
		}
	}

	if (m_xNeedToUpdateViewTransfToVideo) {
		m_xNeedToUpdateViewTransfToVideo = false;
		GetVideoDevice()->xSetTransform(WVDTS_VIEW, m_xViewState.xmView);
	}

	if (m_xNeedToUpdateProjTransfToVideo) {
		m_xNeedToUpdateProjTransfToVideo = false;
		GetVideoDevice()->xSetTransform(WVDTS_PROJECTION, m_xViewState.xmProj);
	}

	CheckReflectiveAndConvertCullFlag(drawoption2);
	video->DrawPolygonFan(vl, m_bDisableFog ? drawoption & 0xf7ffffff : drawoption, n, drawoption2, projected ? 4 : 2);
}

// wview.cpp:1543
void WView::Flush(unsigned long flag) {
	GetVideoDevice()->Command(W_VDEV_FLUSH, flag, 0);
	g_foffset = 0;
}

// wview.cpp:1549
void WView::xSetLight(unsigned long Index, const LightSet& Light) {
	m_xViewState.xLight = Light;
}

// wview.cpp:1554
LightSet* WView::xGetLight(void) {
	return &m_xViewState.xLight;
}

// wview.cpp:1559
void WView::CheckViewAndProjTransformUpdateToVideo(void) {
	if (m_xNeedToUpdateViewTransfToVideo) {
		m_xNeedToUpdateViewTransfToVideo = false;
		GetVideoDevice()->xSetTransform(WVDTS_VIEW, m_xViewState.xmView);
	}

	if (m_xNeedToUpdateProjTransfToVideo) {
		m_xNeedToUpdateProjTransfToVideo = false;
		GetVideoDevice()->xSetTransform(WVDTS_PROJECTION, m_xViewState.xmProj);
	}
}

// wview.cpp:1576
void WView::xScaleProjMat(int index, int scale) {
	float div = (float)scale;
	proj_scale = div;

	float unit = 1.0f / div;

	float l = (float)(index % scale) * unit - 0.5f;
	left = l;
	right = l + unit;
	float t = (float)(index / scale) * unit - 0.5f;
	top = t;
	bottom = t + unit;
	update |= UPDATE_PROJ;
}

// wview.cpp:1592
void WView::xDrawIndexedTriangles(const WxBatchState& state) {
	WxBatchState batch;

	batch = state;

	if (m_bDisableFog) {
		batch.xiFlag0 &= ~0x8000000;
	}
	CheckReflectiveAndConvertCullFlag(batch.xiFlag1);

	GetVideoDevice()->xDrawIndexedTriangles(m_xViewState, batch);
}

// wview.cpp:1606
void WView::SetScale(float scale) {
	m_scale = scale;
	update |= UPDATE_PROJ;
}

// wview.cpp:1615
void WView::ClipPlane(WTVertex* out, const WTVertex* a, const WTVertex* b, int type, const WPlane& p) {
	WVector va;
	va = WVector(a->x, a->y, a->z);
	WVector vb;
	vb = WVector(b->x, b->y, b->z);

	float t1 = WDotProduct(va, p.normal) + p.d;
	float t2 = WDotProduct(vb, p.normal) + p.d;
	float t = t1 / (t1 - t2);
	Projection2_Parallel(out, (1.0f - t) * va + vb * t);

	out->tu = (b->tu - a->tu) * t + a->tu;
	out->tv = (b->tv - a->tv) * t + a->tv;

	out->lu = (b->lu - a->lu) * a->lu * t;
	out->lv = (b->lv - a->lv) * a->lv * t;

	out->diffuse = ULblend(a->diffuse, b->diffuse, (unsigned char)(t * 256.0f));
}

// wview.cpp:1656
void WView::SetProjectionMode(PROJECTION_MODE projMode) {
	if (projMode != m_projMode) {
		m_projMode = projMode;
		memcpy(&m_xViewState.xmProj, &WMatrix4::ZERO, sizeof(WMatrix4));

		update |= UPDATE_PROJ;
	}
}

// wview.cpp:1736
WVector WView::Projection(const WVector& vec) {
	return (this->*ms_fnProjection[m_projMode])(vec);
}

// wview.cpp:1740
void WView::Projection2(WTVertex* vtx, const WVector& vec) {
	(this->*ms_fnProjection2[m_projMode])(vtx, vec);
}

// wview.cpp:1748
WMatrix __cdecl MakeCameraMatrix(const WVector& pos, const WVector& tag) {
	WMatrix mat;
	mat.Reset();
	mat.za = tag - pos;
	mat.za.Normalize();

	if (WisEqual(Abs(mat.za.y), 1.0f, g_EPSILON)) {
		mat.ya = WCrossProduct(mat.za, WVector::UNIT_POS_X);
		mat.xa = WCrossProduct(mat.ya, mat.za);
	} else {
		mat.ya = WVector::UNIT_POS_Y;

		mat.xa = WCrossProduct(mat.ya, mat.za);
		mat.xa.Normalize();

		mat.ya = WCrossProduct(mat.za, mat.xa);
		mat.ya.Normalize();
	}

	mat.pivot = pos;

	return mat;
}

// wview.cpp:2190
WVector WView::Projection_Perspective(const WVector& vec) {
	WVector v = vec * matrix;

	float rhw = 1.0f / v.z;
	WVector p;
	p.x = v.x * rhw + m_center_x;
	p.y = v.y * rhw + m_center_y;
	p.z = clip_scale_z - rhw;
	return p;
}

// wview.cpp:2205
WVector WView::Projection_Parallel(const WVector& vec) {
	WVector v = vec * matrix;

	WVector p;
	p.x = v.x + m_center_x;
	p.y = v.y + m_center_y;
	p.z = (vec.z - clip_near) / (clip_far - clip_near);
	return p;
}

// wview.cpp:2222
void WView::Projection2_Perspective(WTVertex* p, const WVector& vec) {
	WVector v = vec * matrix;
	float rhw = 1.0f / v.z;

	p->sx = v.x * rhw + m_center_x;
	p->sy = v.y * rhw + m_center_y;
	p->sz = clip_scale_z - rhw;
	p->rhw = rhw;
}

// wview.cpp:2236
void WView::Projection2_Parallel(WTVertex* p, const WVector& vec) {
	WVector v = vec * matrix;

	p->sx = v.x + m_center_x;
	p->sy = v.y + m_center_y;

	p->sz = (vec.z - clip_near) / (clip_far - clip_near);
	p->rhw = 1.0f;
}

// wview.cpp:2252
void WView::UpdateCamera_Perspective(void) {
	if (update != 0) {
		camera = lastcam;
		scalex = SCREEN_XS * 0.5f;
		scaley = SCREEN_YS * 0.5f;
		float tan[2];
		tan[0] = tanf(FOV * 0.5f);
		tan[1] = GetRatio() * tan[0];
		float safetan[2];
		safetan[0] = (scalex - 4.0f) / (scalex / tan[0]);
		safetan[1] = (scaley - 4.0f) / (scaley / tan[1]);
		invcamera = ~camera;
		matrix = invcamera;
		WVector scale(scalex / tan[0], -(scaley / tan[1]), 1.0f);
		scale /= clip_near_scale;
		matrix.xx = scale.x * matrix.xx;
		matrix.xy = scale.x * matrix.xy;
		matrix.xz = scale.x * matrix.xz;
		matrix.xm = scale.x * matrix.xm;
		matrix.yx = scale.y * matrix.yx;
		matrix.yy = scale.y * matrix.yy;
		matrix.yz = scale.y * matrix.yz;
		matrix.ym = scale.y * matrix.ym;
		matrix.zx = scale.z * matrix.zx;
		matrix.zy = scale.z * matrix.zy;
		matrix.zz = scale.z * matrix.zz;
		matrix.zm = scale.z * matrix.zm;

		frustumSafe[0] = frustum[0] = WPlane(RotVec(WVector::UNIT_NEG_Z, camera), camera.pivot + camera.za * GetClipNearValue());
		frustumSafe[1] = frustum[1] = WPlane(RotVec(WVector::UNIT_POS_Z, camera), camera.pivot + camera.za * GetClipFarValue());

		frustum[2] = WPlane(RotVec(WVector(-1.0f, 0.0f, -tan[0]), camera).Normalize(), camera.pivot);
		frustumSafe[2] = WPlane(RotVec(WVector(-1.0f, 0.0f, -safetan[0]), camera).Normalize(), camera.pivot);
		frustum[3] = WPlane(RotVec(WVector(1.0f, 0.0f, -tan[0]), camera).Normalize(), camera.pivot);
		frustumSafe[3] = WPlane(RotVec(WVector(1.0f, 0.0f, -safetan[0]), camera).Normalize(), camera.pivot);
		frustum[4] = WPlane(RotVec(WVector(0.0f, 1.0f, -tan[1]), camera).Normalize(), camera.pivot);
		frustumSafe[4] = WPlane(RotVec(WVector(0.0f, 1.0f, -safetan[1]), camera).Normalize(), camera.pivot);
		frustum[5] = WPlane(RotVec(WVector(0.0f, -1.0f, -tan[1]), camera).Normalize(), camera.pivot);
		frustumSafe[5] = WPlane(RotVec(WVector(0.0f, -1.0f, -safetan[1]), camera).Normalize(), camera.pivot);

		UpdateViewTransform();
		UpdateProjectionTransform_Perspective();
	}
}

// wview.cpp:2349
// Residue (99.9%): in the inlined WPlane(n, p) of frustumByCam[2] and [3] cl sums the
// zero-component products as y + z where the original sums z + y; the shape follows rebang.
void WView::UpdateCamera_Parallel(void) {
	if (update != 0) {
		camera = lastcam;
		invcamera = ~camera;
		scalex = SCREEN_XS * 0.5f;
		scaley = SCREEN_YS * 0.5f;
		matrix = invcamera;
		float scale = scalex / GetScale() * 2.0f;
		matrix.xx = scale * matrix.xx;
		matrix.xy = scale * matrix.xy;
		matrix.xz = scale * matrix.xz;
		matrix.xm = scale * matrix.xm;
		matrix.yx *= -scale;
		matrix.yy *= -scale;
		matrix.yz *= -scale;
		matrix.ym *= -scale;
		WVector2D s(GetScale() * 0.5f, GetRatio() * GetScale() * 0.5f);
		frustumSafe[0] = frustum[0] = WPlane(RotVec(WVector::UNIT_NEG_Z, camera), camera.pivot + camera.za * clip_near);
		frustumSafe[1] = frustum[1] = WPlane(RotVec(WVector::UNIT_POS_Z, camera), camera.pivot + camera.za * clip_far);
		frustumSafe[2] = frustum[2] = WPlane(RotVec(WVector::UNIT_NEG_X, camera), camera.pivot - camera.xa * s.x);
		frustumSafe[3] = frustum[3] = WPlane(RotVec(WVector::UNIT_POS_X, camera), camera.pivot + camera.xa * s.x);
		frustumSafe[4] = frustum[4] = WPlane(RotVec(WVector::UNIT_POS_Y, camera), camera.pivot + camera.ya * s.y);
		frustumSafe[5] = frustum[5] = WPlane(RotVec(WVector::UNIT_NEG_Y, camera), camera.pivot - camera.ya * s.y);

		frustumByCam[0] = WPlane(WVector::UNIT_NEG_Z, 0.0f);
		frustumByCam[1] = WPlane(WVector::UNIT_POS_Z, WVector(0.0f, 0.0f, clip_far - clip_near));
		frustumByCam[2] = WPlane(WVector::UNIT_NEG_X, WVector(-scalex, 0.0f, 0.0f));
		frustumByCam[3] = WPlane(WVector::UNIT_POS_X, WVector(scalex, 0.0f, 0.0f));
		frustumByCam[4] = WPlane(WVector::UNIT_POS_Y, WVector(0.0f, scaley, 0.0f));
		frustumByCam[5] = WPlane(WVector::UNIT_NEG_Y, WVector(0.0f, -scaley, 0.0f));

		UpdateViewTransform();
		UpdateProjectionTransform_Parallel();
	}
}

// wview.cpp:2400
void WView::GetScreen2World_Perspective(float sx, float sy, WVector& start, WVector& ray) {
	ray.z = clip_far;
	float scale = (float)tan(FOV * 0.5f) * ray.z / (SCREEN_XS * 0.5f);

	ray.x = (sx - SCREEN_XS * 0.5f) * scale;
	ray.y = (sy - SCREEN_YS * 0.5f) * scale * -1.0f;
	ray = RotVec(ray, camera);

	start = camera.pivot;
}

// wview.cpp:2415
void WView::GetScreen2World_Parallel(float sx, float sy, WVector& start, WVector& ray) {
	WVector v;
	v.x = (sx - SCREEN_XS * 0.5f) / SCREEN_XS * GetScale();
	v.y = (SCREEN_YS * 0.5f - sy) / SCREEN_XS * GetScale();
	v.z = clip_far;

	v = v * camera;

	start = v - camera.za * clip_far;
	ray = v - start;
}

// wview.cpp:2429
void WView::UpdateProjectionTransform_Perspective(void) {
	if (!(update & UPDATE_PROJ)) {
		return;
	}
	update &= ~UPDATE_PROJ;

	float halfw = GetWidth() * 0.5f;
	float halfh = GetHeight() * 0.5f;
	float clipl = (m_clipArea.x - halfw) / GetWidth();
	float clipr = (m_clipArea.x + m_clipArea.w - halfw) / GetWidth();
	float clipt = (m_clipArea.y - halfh) / GetHeight();
	float clipb = (m_clipArea.y + m_clipArea.h - halfh) / GetHeight();
	float fw = (proj_scale / (float)tan(FOV * 0.5f)) * (GetWidth() / m_clipArea.w);
	float fh = fw / GetRatio() * (GetHeight() / m_clipArea.h) / (GetWidth() / m_clipArea.w);

	m_xViewState.xmProj.xx = fw;
	m_xViewState.xmProj.yy = fh;
	m_xViewState.xmProj.zz = clip_scale_z;
	m_xViewState.xmProj.zm = -clip_near_scale;
	m_xViewState.xmProj.wz = 1.0f;
	m_xViewState.xmProj.wm = 0.0f;
	m_xViewState.xmProj.xz = (left + right) / (left - right)
		+ ((m_center_x - halfw) / halfw - (clipr + clipl)) / (clipr - clipl);
	m_xViewState.xmProj.yz = (top + bottom) / (bottom - top)
		- ((m_center_y - halfh) / halfh - (clipb + clipt)) / (clipb - clipt);
	m_xNeedToUpdateProjTransfToVideo = true;
}

// wview.cpp:2461
void WView::UpdateProjectionTransform_Parallel(void) {
	if (!(update & UPDATE_PROJ)) {
		return;
	}
	update &= ~UPDATE_PROJ;

	float halfw = GetWidth() * 0.5f;
	float halfh = GetHeight() * 0.5f;
	float clipl = (m_clipArea.x - halfw) / GetWidth();
	float clipr = (m_clipArea.x + m_clipArea.w - halfw) / GetWidth();
	float clipt = (m_clipArea.y - halfh) / GetHeight();
	float clipb = (m_clipArea.y + m_clipArea.h - halfh) / GetHeight();
	float fw = 2.0f * proj_scale / GetScale() * (GetWidth() / m_clipArea.w);
	float fh = fw / GetRatio() * (GetHeight() / m_clipArea.h) / (GetWidth() / m_clipArea.w);

	m_xViewState.xmProj.xx = fw;
	m_xViewState.xmProj.yy = fh;
	m_xViewState.xmProj.zz = 1.0f / (clip_far - clip_near);
	m_xViewState.xmProj.zm = -clip_near * m_xViewState.xmProj.zz;
	m_xViewState.xmProj.wz = 0.0f;
	m_xViewState.xmProj.wm = 1.0f;
	m_xViewState.xmProj.xz = (left + right) / (left - right)
		+ ((m_center_x - halfw) / halfw - (clipr + clipl)) / (clipr - clipl);
	m_xViewState.xmProj.yz = (top + bottom) / (bottom - top)
		- ((m_center_y - halfh) / halfh - (clipb + clipt)) / (clipb - clipt);
	m_xNeedToUpdateProjTransfToVideo = true;
}

// wview.cpp:2493
void WView::UpdateViewTransform(void) {
	if (update & UPDATE_VIEW) {
		update &= ~UPDATE_VIEW;

		SetWMatrix4FromWMatrix(m_xViewState.xmView, invcamera);

		m_xNeedToUpdateViewTransfToVideo = true;
	}
}
