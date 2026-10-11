// Original: d:\build\custom\temp\code\client\projectg\grid.h
#ifndef PG_CLIENT_PROJECTG_GRID_H
#define PG_CLIENT_PROJECTG_GRID_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"
#include "scenemanager.h"
#include "wmath.h"

class IObject;

IObject* CGridMakeInstance(void);

// The green grid actor (IActor at 0x0, CRenderFuncPtr at 0x18, seen in the
// constructor). Members are transcribed as far as the decompiled functions
// need them.
class CGrid : public IActor, public CRenderFuncPtr {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;

	// One cell of the 21x21 grid, 0x1c bytes. Its implicit constructor
	// only runs the (empty) WVector constructor. GetSlope reads m_pos of
	// [x][y], [x + 1][y] and [x][y + 1]; Reset clears m_bUse.
	struct sGridBlock {
		WVector m_pos;          // 0x0
		WVector m_slopePos;     // 0xc, m_pos shifted along the slope (MakeSlopeIndex)
		bool m_bUse;            // 0x18
		UNKNOWN(0x19, 0x1c);
	};

	// One corner of the slope quad, 0x28 bytes; OnPreLoadInit sets its
	// texture coordinates.
	struct sGridVertex {
		UNKNOWN(0x0, 0x14);
		float m_tu;             // 0x14
		float m_tv;             // 0x18
		UNKNOWN(0x1c, 0x28);
	};

	// grid.h:15
	virtual ~CGrid(void) {}
	// grid.h:17
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }
	virtual void OnPreLoadInit(void);
	virtual void OnLoad(void);
	virtual void OnReload(void);
	virtual void OnProcess(float);
	virtual void OnDestroy(void);
	virtual void Display(void);

protected:
	CGrid(void);

	friend IObject* CGridMakeInstance(void);

	virtual void HandleMsg(const MsgObject &msg);

	void Reset(void);
	int GetColor(float);
	float GetSlope(int, int, float, bool);
	void CheckGridPass(void);
	void MakeDynamicGrid(void);
	void SetHoleGrid(void);
	void MakeSlopeIndex(void);
	void MakeStaticGrid(void);
	void SetGridType(void);

private:
	void MoveLine(float);

protected:
	WVector m_center;               // 0x1c, the grid's centre (MakeStaticGrid lays the cells around it)
	sGridBlock m_blocks[21][21];    // 0x28
	float m_angle;                  // 0x3064, the grid's rotation (OnProcess: RotMat(m_angle, 1)); messages 0x2718 set it, 0x1A1 add to it
	int m_gridCountZ;               // 0x3068, cells along the grid's z axis
	int m_gridCountX;               // 0x306c, cells along the grid's x axis
	float m_blockSize;              // 0x3070
	int m_puttExtraCountZ;          // 0x3074, added with a putter ("1PT")
	int m_puttExtraCountX;          // 0x3078, added with a putter ("1PT")
	float m_puttExtraSize;          // 0x307c, added to m_blockSize with a putter
	UNKNOWN(0x3080, 0x3088);
	bool m_unknown3088;             // 0x3088, cleared by Reset
	UNKNOWN(0x3089, 0x308c);
	WVector m_axisX;                // 0x308c, CheckGridPass projects the ball on it
	WVector m_axisZ;                // 0x3098
	int m_slopeTex;                 // 0x30a4, "slope.jpg" texture handle
	int m_middleBlueTex;            // 0x30a8, "middle_blue.jpg" texture handle
	bool m_unknown30ac;             // 0x30ac, cleared by Reset
	UNKNOWN(0x30ad, 0x30b0);
	float m_unknown30b0;            // 0x30b0, -1 after OnPreLoadInit
	unsigned long m_gridColor;      // 0x30b4, chosen by the map in Reset
	sGridVertex* m_pQuad[4];        // 0x30b8, the vertices in strip order (0, 1, 3, 2)
	int m_unknown30c8;              // 0x30c8, cleared by OnPreLoadInit
	sGridVertex m_vertices[4];      // 0x30cc
	bool m_unknown316c;            // 0x316c, cleared by OnPreLoadInit and OnReload
	UNKNOWN(0x316d, 0x3170);
	int m_unknown3170;              // 0x3170, cleared by Reset
	unsigned long m_lineColor;      // 0x3174, 0xffff0000 after Reset
	float m_linePos;                // 0x3178, moved by MoveLine
	UNKNOWN(0x317c, 0x3180);
};

CHECK_SIZE(CGrid::sGridBlock, 0x1c);
CHECK_FIELD(CGrid::sGridBlock, m_pos, 0x0);
CHECK_FIELD(CGrid::sGridBlock, m_slopePos, 0xc);
CHECK_FIELD(CGrid::sGridBlock, m_bUse, 0x18);
BEGIN_LAYOUT_CHECK(CGrid)
	CHECK_FIELD(CGrid, m_center, 0x1c);
	CHECK_FIELD(CGrid, m_blocks, 0x28);
	CHECK_SIZE(CGrid::sGridVertex, 0x28);
	CHECK_FIELD(CGrid::sGridVertex, m_tu, 0x14);
	CHECK_FIELD(CGrid::sGridVertex, m_tv, 0x18);
	CHECK_FIELD(CGrid, m_angle, 0x3064);
	CHECK_FIELD(CGrid, m_gridCountZ, 0x3068);
	CHECK_FIELD(CGrid, m_gridCountX, 0x306c);
	CHECK_FIELD(CGrid, m_blockSize, 0x3070);
	CHECK_FIELD(CGrid, m_puttExtraCountZ, 0x3074);
	CHECK_FIELD(CGrid, m_puttExtraCountX, 0x3078);
	CHECK_FIELD(CGrid, m_puttExtraSize, 0x307c);
	CHECK_FIELD(CGrid, m_unknown3088, 0x3088);
	CHECK_FIELD(CGrid, m_axisX, 0x308c);
	CHECK_FIELD(CGrid, m_axisZ, 0x3098);
	CHECK_FIELD(CGrid, m_slopeTex, 0x30a4);
	CHECK_FIELD(CGrid, m_middleBlueTex, 0x30a8);
	CHECK_FIELD(CGrid, m_unknown30ac, 0x30ac);
	CHECK_FIELD(CGrid, m_unknown30b0, 0x30b0);
	CHECK_FIELD(CGrid, m_gridColor, 0x30b4);
	CHECK_FIELD(CGrid, m_pQuad, 0x30b8);
	CHECK_FIELD(CGrid, m_unknown30c8, 0x30c8);
	CHECK_FIELD(CGrid, m_vertices, 0x30cc);
	CHECK_FIELD(CGrid, m_unknown316c, 0x316c);
	CHECK_FIELD(CGrid, m_unknown3170, 0x3170);
	CHECK_FIELD(CGrid, m_lineColor, 0x3174);
	CHECK_FIELD(CGrid, m_linePos, 0x3178);
	CHECK_SIZE(CGrid, 0x3180);
END_LAYOUT_CHECK

#endif
