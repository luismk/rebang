// Original: d:\build\custom\temp\code\client\projectg\sea.h
#ifndef PG_CLIENT_PROJECTG_SEA_H
#define PG_CLIENT_PROJECTG_SEA_H

#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "../client/Wangreal/include/wmath.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/wvideo.h"

// The sea surface: a polygon fan over the course's sea outline, drawn in
// scrolling texture layers.
class CSea {
	LAYOUT_CHECK_FRIEND;
public:
	// One scrolling texture layer of the fan (size 0x1c).
	struct sLayer {
		// sea.h:29
		sLayer() {
			m_uv = 0;
			m_fan = 0;
			m_vertices = 0;
			m_texture = 0;
		}
		// sea.h:38
		~sLayer() {
			if (m_uv) { delete[] m_uv; m_uv = 0; }
			if (m_fan) { delete[] m_fan; m_fan = 0; }
			if (m_vertices) { delete[] m_vertices; m_vertices = 0; }
			if (g_resrcmng && m_texture) { g_resrcmng->Release(m_texture); m_texture = 0; }
		}

		WVector2D m_speed;          // 0x0, texture scroll per second
		unsigned long m_color;      // 0x8, vertex diffuse
		WVector2D* m_uv;            // 0xc, one per point
		WTVertex** m_fan;           // 0x10, m_pointCount vertices + a null terminator
		WTVertex* m_vertices;       // 0x14, one per point
		int m_texture;              // 0x18, texture handle
	};

	CSea(void);
	~CSea(void);

	void LoadTexture(void);
	void SetArea(const std::vector<WVector>& points);
	void Process(float dt);
	void Render(void);

protected:
	void AddLayer(float speedX, float speedY, float uvSize, unsigned char alpha);
	void Init(void);

	Waabb m_aabb;                   // 0x0
	WVector* m_points;              // 0x18
	int m_pointCount;               // 0x1c
	sLayer* m_layers;               // 0x20, three allocated
	int m_layerCount;               // 0x24
	PADDING(0x28, 0x5c);
	float m_scale;                  // 0x5c, 1.0 from the constructor; no reader in this unit
};
CHECK_SIZE(CSea::sLayer, 0x1c);
CHECK_FIELD(CSea::sLayer, m_speed, 0x0);
CHECK_FIELD(CSea::sLayer, m_color, 0x8);
CHECK_FIELD(CSea::sLayer, m_uv, 0xc);
CHECK_FIELD(CSea::sLayer, m_fan, 0x10);
CHECK_FIELD(CSea::sLayer, m_vertices, 0x14);
CHECK_FIELD(CSea::sLayer, m_texture, 0x18);
CHECK_SIZE(CSea, 0x60);
BEGIN_LAYOUT_CHECK(CSea)
	CHECK_FIELD(CSea, m_aabb, 0x0);
	CHECK_FIELD(CSea, m_points, 0x18);
	CHECK_FIELD(CSea, m_pointCount, 0x1c);
	CHECK_FIELD(CSea, m_layers, 0x20);
	CHECK_FIELD(CSea, m_layerCount, 0x24);
	CHECK_FIELD(CSea, m_scale, 0x5c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SEA_H
