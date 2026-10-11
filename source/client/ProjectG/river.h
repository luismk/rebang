// Original: d:\build\custom\temp\code\client\projectg\river.h
#ifndef PG_CLIENT_PROJECTG_RIVER_H
#define PG_CLIENT_PROJECTG_RIVER_H
#include "wmath.h"
#include <vector>
struct WTVertex;
class ntRiver {
public:
	virtual ~ntRiver(void) {}

	virtual void Setup(const std::vector<WVector>& points, const char* name) = 0;
	virtual void Process(float fDelta) = 0;
	virtual void Render(void) = 0;
};
CHECK_SIZE(ntRiver, 0x4);

// River surface: a strip of sRiver segments along the points given to
// Setup, drawn with two scrolling texture layers.
class CRiver : public ntRiver {
	LAYOUT_CHECK_FRIEND;
public:
	struct sRiver {
		WVector bank[2];        // 0x0, the strip's two edge points
		float texV[2];          // 0x18, v coordinate of each layer
	};

	// One scrolling texture layer.
	struct sLayer {
		float offset;           // 0x0
		float speed;            // 0x4
		int texture;            // 0x8, WResourceManager handle
		float tileHead;         // 0xc
		float tileSide;         // 0x10
		float depth;            // 0x14
		bool enabled;           // 0x18
	};

	CRiver(void);
	virtual ~CRiver(void);

	virtual void Setup(const std::vector<WVector>& points, const char* name);
	virtual void Process(float fDelta);
	virtual void Render(void);

protected:
	void ReadAttribute(const char* name);

private:
	static WTVertex vtr[4];         // the quad drawn per segment
	static WTVertex* vtx[5];        // vtr in fan order, NULL-terminated

	float m_width;                  // 0x4, half width of the strip (Setup)
	std::vector<sRiver> m_rivers;   // 0x8
	sLayer m_layers[2];             // 0x18
	std::vector<WVector> m_points;  // 0x50
};
CHECK_SIZE(CRiver::sRiver, 0x20);
CHECK_FIELD(CRiver::sRiver, bank, 0x0);
CHECK_FIELD(CRiver::sRiver, texV, 0x18);
CHECK_SIZE(CRiver::sLayer, 0x1c);
CHECK_FIELD(CRiver::sLayer, offset, 0x0);
CHECK_FIELD(CRiver::sLayer, speed, 0x4);
CHECK_FIELD(CRiver::sLayer, texture, 0x8);
CHECK_FIELD(CRiver::sLayer, tileHead, 0xc);
CHECK_FIELD(CRiver::sLayer, tileSide, 0x10);
CHECK_FIELD(CRiver::sLayer, depth, 0x14);
CHECK_FIELD(CRiver::sLayer, enabled, 0x18);
CHECK_SIZE(CRiver, 0x60);
BEGIN_LAYOUT_CHECK(CRiver)
	CHECK_FIELD(CRiver, m_width, 0x4);
	CHECK_FIELD(CRiver, m_rivers, 0x8);
	CHECK_FIELD(CRiver, m_layers, 0x18);
	CHECK_FIELD(CRiver, m_points, 0x50);
END_LAYOUT_CHECK

// Lava surface: the CRiver strip with its own segment and layer types.
class ntRiver_Lava : public ntRiver {
	LAYOUT_CHECK_FRIEND;
public:
	struct sRiver {
		WVector bank[2];        // 0x0, the strip's two edge points
		float texV[2];          // 0x18, v coordinate of each layer
	};

	// One scrolling texture layer.
	struct sLayer {
		float offset;           // 0x0
		float speed;            // 0x4
		int texture;            // 0x8, WResourceManager handle
		float tileHead;         // 0xc
		float tileSide;         // 0x10
		float depth;            // 0x14
		bool enabled;           // 0x18
	};

	ntRiver_Lava(void);
	virtual ~ntRiver_Lava(void);

	virtual void Setup(const std::vector<WVector>& points, const char* name);
	virtual void Process(float fDelta);
	virtual void Render(void);

protected:
	void ReadAttribute(const char* name);

	static WTVertex vtr[4];         // the quad drawn per segment
	static WTVertex* vtx[5];        // vtr in fan order, NULL-terminated

private:
	UNKNOWN(0x4, 0x8);
	sLayer m_layers[2];             // 0x8
	std::vector<sRiver> m_rivers;   // 0x40
	std::vector<WVector> m_points;  // 0x50
};
CHECK_SIZE(ntRiver_Lava::sRiver, 0x20);
CHECK_SIZE(ntRiver_Lava::sLayer, 0x1c);
CHECK_SIZE(ntRiver_Lava, 0x60);
BEGIN_LAYOUT_CHECK(ntRiver_Lava)
	CHECK_FIELD(ntRiver_Lava, m_layers, 0x8);
	CHECK_FIELD(ntRiver_Lava, m_rivers, 0x40);
	CHECK_FIELD(ntRiver_Lava, m_points, 0x50);
END_LAYOUT_CHECK

// Extended river: the CRiver strip with its own segment and layer types.
class CRiver_ex : public ntRiver {
	LAYOUT_CHECK_FRIEND;
public:
	struct sRiver {
		WVector bank[2];        // 0x0, the strip's two edge points
		float texV[2];          // 0x18, v coordinate of each layer
	};

	// One scrolling texture layer.
	struct sLayer {
		float offset;           // 0x0
		float speed;            // 0x4
		int texture;            // 0x8, WResourceManager handle
		float tileHead;         // 0xc
		float tileSide;         // 0x10
		float depth;            // 0x14
		bool enabled;           // 0x18
	};

	CRiver_ex(void);
	virtual ~CRiver_ex(void);

	virtual void Setup(const std::vector<WVector>& points, const char* name);
	virtual void Process(float fDelta);
	virtual void Render(void);

protected:
	void ReadAttribute(const char* name);

private:
	static WTVertex vtr[4];         // the quad drawn per segment
	static WTVertex* vtx[5];        // vtr in fan order, NULL-terminated

	std::vector<sRiver> m_rivers;   // 0x4
	sLayer m_layers[2];             // 0x14
	std::vector<WVector> m_points;  // 0x4c
};
CHECK_SIZE(CRiver_ex::sRiver, 0x20);
CHECK_SIZE(CRiver_ex::sLayer, 0x1c);
CHECK_SIZE(CRiver_ex, 0x5c);
BEGIN_LAYOUT_CHECK(CRiver_ex)
	CHECK_FIELD(CRiver_ex, m_rivers, 0x4);
	CHECK_FIELD(CRiver_ex, m_layers, 0x14);
	CHECK_FIELD(CRiver_ex, m_points, 0x4c);
END_LAYOUT_CHECK

#endif
