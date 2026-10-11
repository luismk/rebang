// Original: d:\build\custom\temp\code\client\projectg\pool.h
#ifndef PG_CLIENT_PROJECTG_POOL_H
#define PG_CLIENT_PROJECTG_POOL_H

#include "..\..\defines.h"
#include "wmath.h"
#include "wresrcmng.h"
#include "../client/Wangreal/include/wvideo.h"
#include "../shared/globalgamedefine.h"

class WView;
class WTextureView;

// A water pool of a course map: the base of the caustic and reflective
// pools. Vtable 0x9ed4b0: the destructor, then five pure virtuals.
class CPool {
	LAYOUT_CHECK_FRIEND;
public:
	// Bits of m_flags, set from the pool's type by SetArea.
	enum {
		FLAG_NO_DROPLET = 0x1,     // no random droplets (Process)
		FLAG_NO_WAVE = 0x2,        // no wave simulation (Process)
		FLAG_NO_COLLISION = 0x4,   // not registered as water in the PVS quadtree
		FLAG_8 = 0x8
	};

	CPool(void);
	virtual ~CPool(void);

	static CPool* CreatePoolByMapType(eMapType type);
	void SetArea(const Waabb& area, const char* name);

	virtual void SetReflection(bool reflection) = 0;
	virtual void Process(float elapsed) = 0;
	virtual void Prerender(WView* view) = 0;
	virtual void Render(WView* view) = 0;

private:
	virtual void BuildMesh(void) = 0;

protected:
	int m_type;              // 0x4: "*type" of the area name (1 = "*noripple")
	Waabb m_area;            // 0x8
	unsigned long m_flags;   // 0x20: FLAG_*
};
CHECK_SIZE(CPool, 0x24);

BEGIN_LAYOUT_CHECK(CPool)
	CHECK_FIELD(CPool, m_type, 0x4);
	CHECK_FIELD(CPool, m_area, 0x8);
	CHECK_FIELD(CPool, m_flags, 0x20);
END_LAYOUT_CHECK

// The caustic pool (vtable 0x9ed4c8): three texture layers.
class CPoolCaustic : public CPool {
	LAYOUT_CHECK_FRIEND;
public:
	CPoolCaustic(void);
	virtual ~CPoolCaustic(void);







	// A caustic pool reflects nothing.
	virtual void SetReflection(bool reflection) {}


	virtual void Prerender(WView* view) {}
	virtual void Process(float elapsed);
	virtual void Render(WView* view);

private:
	virtual void BuildMesh(void);
	void AddLayer(float a, float b, float c, unsigned char d);

public:
	// One texture layer of the caustic: a quad scrolling its texture.
	struct sLayer {
		LAYOUT_CHECK_FRIEND;
		float m_speedU;                       // 0x0: texture scroll per second
		float m_speedV;                       // 0x4
		WTVertex* m_polygon[5];               // 0x8: the quad, NULL-terminated
		WTVertex m_vertices[4];               // 0x1c
		int m_texture;                        // 0xbc: texture handle of g_resrcmng

	public:
		sLayer(void)
		{
			m_texture = 0;
		}


		~sLayer(void) {
			if (g_resrcmng != NULL && m_texture != 0) { g_resrcmng->Release(m_texture); m_texture = 0; }
		}
	};

private:
	Waabb m_box;          // 0x24: m_area lowered by one unit (BuildMesh)
	WVector m_corners[4]; // 0x3c: the quad at the top of m_box
	sLayer m_layers[3];   // 0x6c
	int m_numLayers;      // 0x2ac
};
CHECK_SIZE(CPoolCaustic::sLayer, 0xc0);
CHECK_SIZE(CPoolCaustic, 0x2b0);

BEGIN_LAYOUT_CHECK(CPoolCaustic::sLayer)
	CHECK_FIELD(CPoolCaustic::sLayer, m_speedU, 0x0);
	CHECK_FIELD(CPoolCaustic::sLayer, m_polygon, 0x8);
	CHECK_FIELD(CPoolCaustic::sLayer, m_vertices, 0x1c);
	CHECK_FIELD(CPoolCaustic::sLayer, m_texture, 0xbc);
END_LAYOUT_CHECK

BEGIN_LAYOUT_CHECK(CPoolCaustic)
	CHECK_FIELD(CPoolCaustic, m_box, 0x24);
	CHECK_FIELD(CPoolCaustic, m_layers, 0x6c);
	CHECK_FIELD(CPoolCaustic, m_corners, 0x3c);
	CHECK_FIELD(CPoolCaustic, m_numLayers, 0x2ac);
END_LAYOUT_CHECK

// The reflective pool (vtable 0x9ed4e0): a height-field grid with
// droplets and waves, rendered with a reflection texture.
class CPoolReflective : public CPool {
	LAYOUT_CHECK_FRIEND;
public:
	CPoolReflective(void);
	virtual ~CPoolReflective(void);

	virtual void SetReflection(bool reflection);
	virtual void Process(float elapsed);
	virtual void Prerender(WView* view);
	virtual void Render(WView* view);

private:
	virtual void BuildMesh(void);
	void Droplet(void);
	void ExcludePoint(void);
	void MakeNorm(void);
	void Wave(float elapsed);

public:
	// One point of the height-field grid.
	struct sWaveVertex {
		sWaveVertex(void) {}

		WVector m_pos;        // 0x0
		WVector m_normal;     // 0xc: WVector::UNIT_POS_Y at build (MakeNorm)
		float m_unknown18;    // 0x18: 0 at build
		float m_height;       // 0x1c: wave height (0 at build)
		float m_unknown20;    // 0x20: 0.5 at build
		float m_unknown24;    // 0x24: 0.5 at build
		bool m_excluded;      // 0x28: under the ground (ExcludePoint)
	};

private:
	int m_width;                    // 0x24: grid points per row
	int m_height;                   // 0x28: grid rows
	sWaveVertex** m_grid;            // 0x2c: m_height rows of m_width points
	unsigned long m_lastDropTime;   // 0x30: g_CurrentTime of the last droplet
	WTVertex* m_buffers[4];         // 0x34: the quad of the cell (NULL-terminated at 0x44) handed to DrawPolygonFan
	int m_unknown44;                // 0x44: zeroed by the constructor
	WTVertex m_bufferData[4];      // 0x48: the vertices m_buffers points to
	WTextureView* m_textureView;    // 0xe8: reflection render target
};
CHECK_SIZE(CPoolReflective::sWaveVertex, 0x2c);
CHECK_SIZE(CPoolReflective, 0xec);

BEGIN_LAYOUT_CHECK(CPoolReflective)
	CHECK_FIELD(CPoolReflective, m_width, 0x24);
	CHECK_FIELD(CPoolReflective, m_height, 0x28);
	CHECK_FIELD(CPoolReflective, m_grid, 0x2c);
	CHECK_FIELD(CPoolReflective, m_lastDropTime, 0x30);
	CHECK_FIELD(CPoolReflective, m_buffers, 0x34);
	CHECK_FIELD(CPoolReflective, m_unknown44, 0x44);
	CHECK_FIELD(CPoolReflective, m_bufferData, 0x48);
	CHECK_FIELD(CPoolReflective, m_textureView, 0xe8);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_POOL_H
