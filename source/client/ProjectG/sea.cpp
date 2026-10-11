// Original: d:\build\custom\temp\code\client\projectg\sea.cpp
#include <windows.h>
#include <math.h>

#include "sea.h"

#include "commonutil.h"
#include "quadtree.h"
#include "../client/Wangreal/include/wview.h"
#include "../client/Wangreal/include/wvideo.h"

extern LightSet g_lightset;

// sea.cpp:22
CSea::CSea(void)
	: m_points(0), m_pointCount(0), m_layerCount(0), m_scale(1.0f)
{
	m_layers = new sLayer[3];
}

// sea.cpp:32
CSea::~CSea(void)
{
	for (int i = 0; i < m_layerCount; i++) {
		if (g_resrcmng && m_layers[i].m_texture) { g_resrcmng->Release(m_layers[i].m_texture); m_layers[i].m_texture = 0; }
	}

	if (m_layers) { delete[] m_layers; m_layers = 0; }
	if (m_points) { delete[] m_points; m_points = 0; }
}

// sea.cpp:43
void CSea::LoadTexture(void)
{
	for (int i = 0; i < m_layerCount; i++) {
		if (g_resrcmng && m_layers[i].m_texture) { g_resrcmng->Release(m_layers[i].m_texture); m_layers[i].m_texture = 0; }
		m_layers[i].m_texture = g_resrcmng->LoadTexture(MakeStr("[sea_0%d.jpg", i + 1), 0, 0, 0);
	}
}

// sea.cpp:53
void CSea::SetArea(const std::vector<WVector>& points)
{
	m_pointCount = points.size();
	m_points = new WVector[m_pointCount];

	m_aabb = Waabb(WVector::ONE * 10000.0f,
		WVector::ONE * -10000.0f);

	for (int i = 0; i < m_pointCount; i++) {
		m_points[i] = points[i];
		m_points[i].y = points[0].y;

		// Grow the bounding box over the outline.
		m_aabb.min.x = min(m_points[i].x, m_aabb.min.x);
		m_aabb.max.x = max(m_points[i].x, m_aabb.max.x);
		m_aabb.min.y = min(m_points[i].y, m_aabb.min.y);
		m_aabb.max.y = max(m_points[i].y, m_aabb.max.y);
		m_aabb.min.z = min(m_points[i].z, m_aabb.min.z);
		m_aabb.max.z = max(m_points[i].z, m_aabb.max.z);
	}

	m_aabb.max.y -= 1.0f;
	m_aabb.min.y = m_aabb.max.y - 1.0f;

	int last = m_pointCount - 1;
	for (int j = 0; j < m_pointCount / 2; j++) {
		WVector v0 = points[j];
		WVector v1 = points[last - j];
		WVector v2 = points[j + 1];
		WVector v3 = points[last - j - 1];

		// The quad between two opposite edges of the outline is one water
		// area of the course.
		Waabb box;
		box.max.x = max(max(v0.x, v1.x), max(v2.x, v3.x));
		box.max.y = m_aabb.max.y;
		box.max.z = max(max(v0.z, v1.z), max(v2.z, v3.z));

		box.min.x = min(min(v0.x, v1.x), min(v2.x, v3.x));
		box.min.y = m_aabb.min.y;
		box.min.z = min(min(v0.z, v1.z), min(v2.z, v3.z));

		GetPVS().AddWater(box, 1);
	}

	Init();
}

// sea.cpp:108
void CSea::Init(void)
{
	float angle = 30.0f * (3.141592654f / 180.0f);
	float flowY = sinf(angle) * -0.02f;
	float flowX = cosf(angle) * 0.02f;

	AddLayer(-flowX, flowY, 200.0f, 255);
	AddLayer(flowX, flowY, 100.0f, 255);
	AddLayer(0.0f, 0.02f, 150.0f, 255);
}

// sea.cpp:119
void CSea::AddLayer(float speedX, float speedY, float uvSize, unsigned char alpha)
{
	sLayer* layer = &m_layers[m_layerCount++];

	layer->m_speed.x = speedX;
	layer->m_speed.y = speedY;
	layer->m_color = (alpha << 24) | g_lightset.ambient2;
	layer->m_uv = new WVector2D[m_pointCount];
	layer->m_fan = new WTVertex*[m_pointCount + 1];
	layer->m_vertices = new WTVertex[m_pointCount];
	layer->m_texture = g_resrcmng->LoadTexture(MakeStr("[sea_0%d.jpg", min(2, m_layerCount)), 0, 0, 0);

	int i;
	for (i = 0; i < m_pointCount; i++) {
		layer->m_uv[i].x = (m_points[i].x - m_aabb.min.x) * (1.0f / uvSize);
		layer->m_uv[i].y = (m_points[i].z - m_aabb.min.z) * (1.0f / uvSize);
		layer->m_fan[i] = &layer->m_vertices[i];
		layer->m_vertices[i].diffuse = layer->m_color;
	}

	layer->m_fan[i] = 0;
}

// sea.cpp:144
void CSea::Process(float dt)
{
	for (int i = 0; i < m_layerCount; i++) {
		for (int j = 0; j < m_pointCount; j++) {
			m_layers[i].m_uv[j] += WVector2D(dt * m_layers[i].m_speed.x, dt * m_layers[i].m_speed.y);
		}
	}
}

// sea.cpp:156
void CSea::Render(void)
{
	if (!g_view->InFrustum(m_aabb)) {
		return;
	}

	for (int i = 0; i < m_layerCount; i++) {
		sLayer& layer = m_layers[i];

		for (int j = 0; j < m_pointCount; j++) {
			WVector pos = m_points[j];
			pos.y = m_aabb.max.y;
			layer.m_vertices[j].SetPosition(pos);
			layer.m_vertices[j].tu = layer.m_uv[j].x;
			layer.m_vertices[j].tv = layer.m_uv[j].y;
		}

		g_view->DrawPolygonFan(layer.m_fan, (layer.m_texture & 0x7FF) | 0x20800000, 0x400, false);
	}
}
