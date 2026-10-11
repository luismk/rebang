// Original: d:\build\custom\temp\code\client\projectg\pool.cpp
// This object keeps the out-of-line copy of operator*(WMatrix4, WMatrix4) and
// of WMatrix(const WPlane&).
#define POOL_WMATH_INLINE
#include "pool.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "slicestr.h"
#include "commonutil.h"
#include "../shared/classdefine.h"
#include "quadtree.h"
#include "SceneManager.h"
#include "SharedDoc.h"
#include "ClientSetting.h"
#include "../client/Wangreal/include/wview.h"
#include "../client/Wangreal/include/wscene.h"
#include "../client/Wangreal/include/wutil.h"
#include "../client/Wangreal/include/wtextureview.h"

extern unsigned long g_CurrentTime;
extern int causticAlpha;
extern LightSet g_lightset;
extern WMatrix g_camera;

// Nameless globals of the default light (0xAA5274 is probably a LightSet:
// nearone 0xAA5278, diffuse 0xAA5284, ambient 0xAA5288).
extern WVector data_00aa5278;
extern unsigned long data_00aa5284;
extern unsigned long data_00aa5288;

// wmath.inl:177 (this object keeps the out-of-line copy; callers reference it)
WMatrix WMatrix::operator~(void) const
{
	WMatrix r;
	WVector c;
	c.x = yy * zz - yz * zy;
	c.y = xz * zy - zz * xy;
	c.z = yz * xy - yy * xz;
	float inv = 1.0f / (xx * c.x + yx * c.y + zx * c.z);
	r.xx = c.x * inv;
	r.yx = (yz * zx - yx * zz) * inv;
	r.zx = (yx * zy - zx * yy) * inv;
	r.xy = c.y * inv;
	r.yy = (xx * zz - zx * xz) * inv;
	r.zy = (zx * xy - xx * zy) * inv;
	r.xz = c.z * inv;
	r.yz = (yx * xz - yz * xx) * inv;
	r.zz = (yy * xx - yx * xy) * inv;
	r.xm = -(r.xx * xm + r.xy * ym + r.xz * zm);
	r.ym = -(r.yx * xm + r.yy * ym + r.yz * zm);
	r.zm = -(r.zx * xm + r.zy * ym + r.zz * zm);
	return r;
}

// wutil.h:288 (per-channel add of `right` scaled by value/256 to `left`,
// saturated at 0xff, alpha dropped; this object holds the only copy)
unsigned long __fastcall AddDiffuse(unsigned long left, unsigned long right, unsigned char value)
{
	unsigned long blue = Min<unsigned long>(0xff, ((right & 0xff) * value >> 8) + (left & 0xff));
	unsigned long green = Min<unsigned long>(0xff, ((right >> 8 & 0xff) * value >> 8) + (left >> 8 & 0xff));
	unsigned long red = Min<unsigned long>(0xff, ((right >> 16 & 0xff) * value >> 8) + (left >> 16 & 0xff));

	return (red << 16) | (green << 8) | blue;
}

// pool.cpp:22
CPool* CPool::CreatePoolByMapType(eMapType type)
{
	switch (type) {
	case MAP_ICE_CANNON:
	case MAP_PINK_WIND:
	case MAP_DEEP_INFERNO:
	case MAP_ICE_SPA:
	case MAP_LOST_SEAWAY:
	case MAP_EASTERN_VALLEY:
	case (eMapType)17:
	case MAP_WIZ_CITY:
		return new CPoolCaustic;
	}
	return new CPoolReflective;
}

// pool.cpp:46
CPool::CPool(void) : m_type(0), m_flags(0)
{
	m_area.min.Reset(); m_area.max.Reset();
}

// pool.cpp:51
CPool::~CPool(void)
{
}

// pool.cpp:55
void CPool::SetArea(const Waabb& area, const char* name)
{
	m_area = area;

	WSliceStr slice(name);

	m_type = 0;
	slice.Get("*type", "%d", &m_type);

	if (strstr(name, "*noripple")) {
		m_type = 1;
	}

	switch (m_type) {
	case 1:
		m_flags |= FLAG_NO_DROPLET | FLAG_NO_WAVE;
		break;

	case 2:
		m_flags |= FLAG_NO_DROPLET | FLAG_NO_COLLISION | FLAG_8;
		break;
	}

	if (!(m_flags & FLAG_NO_COLLISION)) {
		GetPVS().AddWater(m_area, 3);
	}

	BuildMesh();
}

// pool.cpp:92
CPoolCaustic::CPoolCaustic(void)
{
	m_numLayers = 0;
}

// pool.cpp:96
CPoolCaustic::~CPoolCaustic(void)
{
}

// pool.cpp:105
void CPoolCaustic::Process(float elapsed)
{
	WView* view = g_view;
	if (!view->InFrustum(m_area) || !view->InFrustum(m_box)) {
		return;
	}

	float speed = 1.0f;

	// Slower caustics on some courses.
	if (GetCurMap() == MAP_DEEP_INFERNO) {
		speed = 0.7f;
	} else if (GetCurMap() == MAP_ICE_SPA && m_type == 2) {
		speed = 0.5f;
	}

	for (int i = 0; i < m_numLayers; i++) {
		sLayer& layer = m_layers[i];
		for (int j = 0; j < 4; j++) {
			layer.m_vertices[j].tu += speed * layer.m_speedU * elapsed;
			layer.m_vertices[j].tv += speed * layer.m_speedV * elapsed;
		}
	}
}

// pool.cpp:128
void CPoolCaustic::Render(WView* view)
{
	if (!view->InFrustum(m_area) || !view->InFrustum(m_box)) {
		return;
	}

	for (int i = 0; i < m_numLayers; i++) {
		sLayer& layer = m_layers[i];

		for (int j = 0; j < 4; j++) {
			layer.m_vertices[j].SetPosition(m_corners[j]);
			layer.m_vertices[j].diffuse = (layer.m_vertices[j].diffuse & 0xffffff) | (causticAlpha << 24);
		}

		int blend = 0x800000;

		if (GetCurMap() == MAP_ICE_SPA) {
			// The two lower layers of Ice Spa.
			if (i < 2) {
				blend = 0;
			}
		}

		view->DrawPolygonFan(layer.m_polygon, (layer.m_texture & 0x7ff) | blend | 0x20400000, 0x400, false);
	}
}

// pool.cpp:174
void CPoolCaustic::BuildMesh(void)
{
	m_box = m_area;

	// A one-unit slab under the water surface.
	m_box.min.y = (m_box.max.y -= 1.0f) - 1.0f;

	m_corners[0] = WVector(m_box.max.x, m_box.max.y, m_box.max.z);
	m_corners[1] = WVector(m_box.max.x, m_box.max.y, m_box.min.z);
	m_corners[2] = WVector(m_box.min.x, m_box.max.y, m_box.min.z);
	m_corners[3] = WVector(m_box.min.x, m_box.max.y, m_box.max.z);

	float scale = 1.3f;

	if (GetCurMap() == MAP_ICE_SPA && m_type == 2) {
		scale = 0.325f;
	}

	AddLayer(-(cos(30 * (3.141592654f / 180.0f)) * 0.02f), sin(30 * (3.141592654f / 180.0f)) * -0.02f, scale * 200.0f, 255);
	AddLayer(cos(30 * (3.141592654f / 180.0f)) * 0.02f, sin(30 * (3.141592654f / 180.0f)) * -0.02f, scale * 100.0f, 255);
	AddLayer(0.0f, 0.02f, scale * 150.0f, 255);
}

// pool.cpp:202
void CPoolCaustic::AddLayer(float a, float b, float c, unsigned char d)
{
	if (m_numLayers >= 3) {
		return;
	}

	sLayer& layer = m_layers[m_numLayers];
	layer.m_speedU = a;
	layer.m_speedV = b;

	int index = m_numLayers + 1 > 2 ? 2 : m_numLayers + 1;
	const char* mapName = WSingleton<CSharedDoc>::Instance()->m_pCurCourse ? WSingleton<CSharedDoc>::Instance()->m_pCurCourse->m_mapName : 0;

	if (stricmp(mapName, "pola") == 0 && m_type == 0) {
		mapName = "icee";
	}

	layer.m_texture = g_resrcmng->LoadTexture(MakeStr("[caustic_%s%02d.jpg", mapName, index), 0, 0, 0);

	if (layer.m_texture == 0) {
		// No course-specific caustic: the generic one.
		layer.m_texture = g_resrcmng->LoadTexture(MakeStr("[caustic_%02d.jpg", index), 0, 0, 0);
	}

	float inv = 1.0f / c;

	unsigned long alpha = d << 24;

	for (int i = 0; i < 4; i++) {
		layer.m_vertices[i].tu = (m_corners[i].x - m_box.min.x) * inv;
		layer.m_vertices[i].tv = (m_corners[i].z - m_box.min.z) * inv;
		layer.m_vertices[i].diffuse = alpha | g_lightset.ambient2;
		layer.m_polygon[i] = &layer.m_vertices[i];
	}

	layer.m_polygon[4] = NULL;

	m_numLayers++;
}

// pool.cpp:244
CPoolReflective::CPoolReflective(void) : m_grid(NULL), m_textureView(NULL)
{
	m_buffers[0] = &m_bufferData[0];
	m_buffers[1] = &m_bufferData[1];
	m_buffers[2] = &m_bufferData[3];
	m_buffers[3] = &m_bufferData[2];
	m_unknown44 = 0;
}

// pool.cpp:253
CPoolReflective::~CPoolReflective(void)
{
	if (m_grid != NULL) {
		for (int i = 0; i < m_height; i++) {
			if (m_grid[i] != NULL) { delete[] m_grid[i]; m_grid[i] = NULL; }
		}
		if (m_grid != NULL) { delete[] m_grid; m_grid = NULL; }
	}

	if (m_textureView != NULL) { delete m_textureView; m_textureView = NULL; }
}

// pool.cpp:266
void CPoolReflective::SetReflection(bool reflection)
{
	if (reflection) {
		// Already reflecting.
		if (m_textureView != NULL) {
			return;
		}

		m_textureView = new WTextureView;
		m_textureView->SetResourceManager(g_resrcmng);
		m_textureView->SetViewport(g_view->GetWidth() * 0.5f, g_view->GetHeight() * 0.5f);
		m_textureView->SetReflective(true);

		WTextureView::TextureParam param;
		param.m_numRts = 1;

		param.m_rtTexInfo[0].Set(0x14000, WRenderToTextureSizeInfo::SIZE_HALF, false, 0);
		param.m_depthSurfInfo.Set(WRenderToTextureParam::DepthSurfInfo::USE_SHARED_SURFACE, WRenderToTextureSizeInfo::SIZE_HALF, true, 1.0f);

		m_textureView->SetTextureParam(param);
	} else {
		if (m_textureView != NULL) { delete m_textureView; m_textureView = NULL; }
	}
}

// pool.cpp:294
void CPoolReflective::Process(float elapsed)
{
	if (g_view->InFrustum(m_area)) {
		// Drop a droplet every second.
		if (!(m_flags & FLAG_NO_DROPLET)) {
			if (g_CurrentTime - m_lastDropTime > 1000) {
				m_lastDropTime = g_CurrentTime;
				Droplet();
			}
		}

		if (!(m_flags & FLAG_NO_WAVE)) {
			MakeNorm();
			Wave(elapsed);
		}
	}
}

namespace {

// The lit colour of a water point: ambient, the sun's diffuse by the angle of
// the surface to it, and a white highlight by the angle of the half vector
// between the view and the sun (the same expressions the flat and the grid
// paths of CPoolReflective::Render evaluate).
unsigned long WaterColor(WView *view, const WVector &pos, const WVector &normal)
{
	WVector sun = data_00aa5278;

	float lit = normal.x * -sun.x + normal.y * -sun.y + normal.z * -sun.z;
	if (!(lit > 0.0f)) {
		lit = 0.0f;
	}

	WVector toView = view->camera.pivot - pos;
	toView.Normalize();
	WVector half = toView - sun;
	half.Normalize();

	float spec = normal.x * half.x + normal.y * half.y + normal.z * half.z;
	if (!(spec > 0.0f)) {
		spec = 0.0f;
	}

	unsigned long color = AddDiffuse(0, data_00aa5288, 0x66);
	color = AddDiffuse(color, data_00aa5284, (unsigned char)(int)(lit * 102.0f));
	color = AddDiffuse(color, 0x808080, (unsigned char)(int)(spec * spec * 255.0f));
	return color;
}

} // namespace

// pool.cpp:315
void CPoolReflective::Prerender(WView *view)
{
	if (view->camera.pivot.y >= m_area.max.y && view->InFrustum(m_area)) {
		if (m_textureView != NULL && WSingleton<CSceneManager>::IsInstantiated()) {
			WVideoDev *video = g_resrcmng->VideoReference();

			m_textureView->SetFOV(view->GetFOV());

			// The camera mirrored about the water surface.
			WPlane plane(WVector::UNIT_POS_Y, -m_area.max.y);
			WMatrix reflected = view->camera * ~WMatrix(plane);

			m_textureView->SetClip(0.1f, view->GetClipFarValue(), false);
			m_textureView->SetCamera(reflected);
			m_textureView->UpdateCamera();

			WView *oldView = g_view;
			WMatrix oldCamera = g_camera;
			g_view = m_textureView;
			g_camera = m_textureView->camera;

			m_textureView->Begin();

			WSingleton<CSceneManager>::Instance()->Render(m_textureView, 0x80);

			if (!(m_flags & FLAG_8)) {
				// Clip what is below the surface.
				WPlane clip(WVector::UNIT_POS_Y, 1.0f - m_area.max.y);
				video->BeginUsingCustomRenderState();
				video->SetCustomRenderState(WVDRS_CLIPPLANEENABLE, 1);
				video->SetCustomClipPlane(0, clip);

				WSingleton<CSceneManager>::Instance()->Render(m_textureView, 0xD);

				video->EndUsingCustomRenderState();
			}

			m_textureView->Flush(0);
			m_textureView->End();

			g_view = oldView;
			g_camera = oldCamera;
		}
	}
}

// pool.cpp:373
void CPoolReflective::Render(WView *view)
{
	if (view->camera.pivot.y >= m_area.max.y && view->InFrustum(m_area)) {
		int texture = m_textureView ? m_textureView->GetTexture(0) : 0;

		WVideoDev *video = g_resrcmng->VideoReference();
		if (texture != 0) {
			video->BeginUsingCustomRenderState();

			// Projects the reflection texture on the water.
			WMatrix4 texMatrix = view->xGetViewState().xmView * view->xGetViewState().xmProj * WView::ms_uvConvMat;

			video->SetCustomFxMacro(0x10000);
			video->SetCustomFxParamMatrix((WVDFXPARAMETERTYPE)0x12, texMatrix);
		}

		unsigned char alpha;
		if (IsCurMapSnow() || GetCurMap() == 10) {
			alpha = 0x28;
		} else {
			alpha = (m_type == 2) ? 0x50 : 0xA0;
		}

		// The surface is more transparent seen from above.
		float facing = -view->camera.za.x * WVector::UNIT_POS_Y.x + -view->camera.za.y * WVector::UNIT_POS_Y.y + -view->camera.za.z * WVector::UNIT_POS_Y.z;
		if (!(facing > 0.0f)) {
			facing = 0.0f;
		}
		float steep = 1.0f - facing;
		unsigned long alphaBits = (unsigned long)(unsigned char)(int)(((1.0f - steep) * 0.5f + steep) * alpha) << 24;

		if (m_flags & FLAG_NO_WAVE) {
			// A single flat quad over the area.
			float top = m_area.max.y;
			WVector up = WVector::UNIT_POS_Y;
			WVector center = (m_area.min + m_area.max) * 0.5f;
			unsigned long color = WaterColor(view, center, up) | alphaBits;

			m_bufferData[0].x = m_area.min.x;
			m_bufferData[0].y = top;
			m_bufferData[0].z = m_area.min.z;
			m_bufferData[0].diffuse = color;
			m_bufferData[1].x = m_area.min.x;
			m_bufferData[1].y = top;
			m_bufferData[1].z = m_area.max.z;
			m_bufferData[1].diffuse = color;
			m_bufferData[2].x = m_area.max.x;
			m_bufferData[2].y = top;
			m_bufferData[2].z = m_area.min.z;
			m_bufferData[2].diffuse = color;
			m_bufferData[3].x = m_area.max.x;
			m_bufferData[3].y = top;
			m_bufferData[3].z = m_area.max.z;
			m_bufferData[3].diffuse = color;

			view->DrawPolygonFan(m_buffers, (texture & 0x7FF) | 0xA400000, 0, false);
		} else {
			for (int y = 0; y < m_height - 1; y++) {
				for (int x = 0; x < m_width - 1; x++) {
					int included = 0;
					for (int i = 0; i < 4; i++) {
						sWaveVertex &v = m_grid[y + (i & 1)][x + ((i >> 1) & 1)];
						if (!v.m_excluded) {
							included++;
						}

						m_bufferData[i].x = v.m_pos.x;
						m_bufferData[i].y = v.m_pos.y;
						m_bufferData[i].z = v.m_pos.z;
						m_bufferData[i].diffuse = WaterColor(view, v.m_pos, v.m_normal) | alphaBits;
					}

					if (included != 4) {
						view->DrawPolygonFan(m_buffers, (texture & 0x7FF) | 0xA400000, 0, false);
					}
				}
			}
		}

		if (texture != 0) {
			video->EndUsingCustomRenderState();
		}
	}
}

// pool.cpp:513
void CPoolReflective::BuildMesh(void)
{
	if (!(m_flags & FLAG_NO_WAVE)) {
		// One grid point every 20 units.
		m_width = (int)ceil((m_area.max.x - m_area.min.x) * 0.05f) + 1;
		m_height = (int)ceil((m_area.max.z - m_area.min.z) * 0.05f) + 1;

		float stepX = (m_area.max.x - m_area.min.x) / (m_width - 1);
		float stepZ = (m_area.max.z - m_area.min.z) / (m_height - 1);

		m_grid = new sWaveVertex*[m_height];
		for (int i = 0; i < m_height; i++) {
			m_grid[i] = new sWaveVertex[m_width];
		}

		for (int y = 0; y < m_height; y++) {
			for (int x = 0; x < m_width; x++) {
				m_grid[y][x].m_pos.x = m_area.min.x + x * stepX;
				m_grid[y][x].m_pos.y = m_area.max.y;
				m_grid[y][x].m_pos.z = m_area.min.z + y * stepZ;

				m_grid[y][x].m_unknown18 = 0.0f;
				m_grid[y][x].m_height = 0.0f;

				m_grid[y][x].m_unknown20 = 0.5f;
				m_grid[y][x].m_unknown24 = 0.5f;

				m_grid[y][x].m_normal = WVector::UNIT_POS_Y;
				m_grid[y][x].m_excluded = false;
			}
		}

		m_lastDropTime = g_CurrentTime;

		ExcludePoint();
	}

	SetReflection(WSingleton<COption>::Instance()->vIsReflectionEnabled());
}

// pool.cpp:561
void CPoolReflective::Droplet(void)
{
	int x = rand() % (m_width - 1);
	int y = rand() % (m_height - 1);

	m_grid[y][x].m_height += 2.0f;
	m_grid[y][x + 1].m_height += 2.0f;
	m_grid[y + 1][x].m_height += 2.0f;
	m_grid[y + 1][x + 1].m_height += 2.0f;
}

// pool.cpp:574
void CPoolReflective::MakeNorm(void)
{
	WVector dx;
	WVector dz;
	WVector normal;

	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			if (m_grid[y][x].m_excluded) {
				// Two edges from the point to its neighbours, inward on the borders.
				int lastY = m_height - 1;
				int lastX = m_width - 1;

				if (y == lastY) {
					dx = m_grid[y][x == lastX ? x - 1 : x + 1].m_pos - m_grid[y][x].m_pos;
					dz = m_grid[y - 1][x].m_pos - m_grid[y][x].m_pos;
				} else if (x == lastX) {
					dx = m_grid[y][x - 1].m_pos - m_grid[y][x].m_pos;
					dz = m_grid[y + 1 == lastY ? y - 1 : y + 1][x].m_pos - m_grid[y][x].m_pos;
				} else {
					dx = m_grid[y][x + 1].m_pos - m_grid[y][x].m_pos;
					dz = m_grid[y + 1][x].m_pos - m_grid[y][x].m_pos;
				}

				normal = WCrossProduct(dz, dx);
				normal.Normalize();

				m_grid[y][x].m_normal = normal;
				m_grid[y][x].m_unknown20 = (normal.x + 1.0f) * 0.5f;
				m_grid[y][x].m_unknown24 = (normal.z + 1.0f) * 0.5f;
			}
		}
	}
}

// pool.cpp:612
void CPoolReflective::Wave(float elapsed)
{
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			m_grid[y][x].m_unknown18 = 0.0f;
		}
	}

	for (int y = 1; y < m_height - 1; y++) {
		for (int x = 1; x < m_width - 1; x++) {
			if (m_grid[y][x].m_excluded) {
				float d;

				d = m_grid[y][x].m_pos.y - m_grid[y][x - 1].m_pos.y;
				m_grid[y][x].m_unknown18 -= d;
				m_grid[y][x - 1].m_unknown18 += d;

				// Self-assignments in the original.
				m_grid[y][x].m_unknown18 = m_grid[y][x].m_unknown18;
				m_grid[y - 1][x].m_unknown18 = m_grid[y - 1][x].m_unknown18;

				d = m_grid[y][x].m_pos.y - m_grid[y][x + 1].m_pos.y;
				m_grid[y][x].m_unknown18 -= d;
				m_grid[y][x + 1].m_unknown18 += d;

				d = m_grid[y][x].m_pos.y - m_grid[y + 1][x].m_pos.y;
				m_grid[y][x].m_unknown18 -= d;
				m_grid[y + 1][x].m_unknown18 += d;

				// Diagonals, weighted by 1/sqrt(2).
				d = (m_grid[y][x].m_pos.y - m_grid[y + 1][x + 1].m_pos.y) * 0.70710677f;
				m_grid[y][x].m_unknown18 -= d;
				m_grid[y + 1][x + 1].m_unknown18 += d;

				d = (m_grid[y][x].m_pos.y - m_grid[y - 1][x - 1].m_pos.y) * 0.70710677f;
				m_grid[y][x].m_unknown18 -= d;
				m_grid[y - 1][x - 1].m_unknown18 += d;

				d = (m_grid[y][x].m_pos.y - m_grid[y + 1][x - 1].m_pos.y) * 0.70710677f;
				m_grid[y][x].m_unknown18 -= d;
				m_grid[y + 1][x - 1].m_unknown18 += d;

				// The original repeats the (y + 1, x - 1) diagonal.
				d = (m_grid[y][x].m_pos.y - m_grid[y + 1][x - 1].m_pos.y) * 0.70710677f;
				m_grid[y][x].m_unknown18 -= d;
				m_grid[y + 1][x - 1].m_unknown18 += d;
			}
		}
	}

	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			if (m_grid[y][x].m_excluded) {
				m_grid[y][x].m_height += elapsed * m_grid[y][x].m_unknown18;
				m_grid[y][x].m_pos.y = Max(m_grid[y][x].m_pos.y + elapsed * m_grid[y][x].m_height, m_area.max.y - 2.0f);
			}
		}
	}
}

// pool.cpp:679
void CPoolReflective::ExcludePoint(void)
{
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			int index;
			WVector ground = GetPVS().GetGroundPoint(m_grid[y][x].m_pos, true, &index, false);

			if (index >= 0 && ground.y < m_area.max.y && GetPVS().GetMeshNormal(index).y > 0.0f) {
				m_grid[y][x].m_excluded = true;
			}
		}
	}
}
