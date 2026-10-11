// Original: d:\build\custom\temp\code\client\projectg\3dfont.h
#ifndef PG_CLIENT_PROJECTG_3DFONT_H
#define PG_CLIENT_PROJECTG_3DFONT_H

#include "..\..\defines.h"
#include "wmath.h"
#include "wview.h"
#include "wvideo.h"

// A bitmap font drawn at a projected 3D position: one textured quad per
// character, cut from a texture grid of m_charWidth x m_charHeight cells.
class C3dFont {
	LAYOUT_CHECK_FRIEND;
public:
	struct ProjectionStyle {
		enum Enum {
			Default = 0,
			Overlay = 1 // drawn with the 0x300000 polygon flags
		};
	};

	C3dFont(void);
	~C3dFont(void);

	void LoadTexture(const char* filename, int charWidth, int charHeight, const char* charSet, float offsetX, float offsetY);
	void SetPos(const WVector& pos);
	void SetSize(int width, int height);
	void SetColor(int color);
	void SetProjectionStyle(ProjectionStyle::Enum style) { m_projectionStyle = style; } // 3dfont.h:25
	void Printf(WView* view, int spacing, const char* format, ...);

protected:
	void ClipChar(int index);

	int m_texture;                         // 0x0: texture handle of g_resrcmng
	const char* m_charSet;                 // 0x4: characters of the texture grid, in cell order
	int m_columns;                         // 0x8: cells per texture row
	float m_cellU;                         // 0xc: texture width of a cell
	float m_cellV;                         // 0x10: texture height of a cell
	int m_charWidth;                       // 0x14
	int m_charHeight;                      // 0x18
	float m_offsetX;                       // 0x1c
	float m_offsetY;                       // 0x20
	WVector m_pos;                         // 0x24
	WTVertex m_vertex[4];                  // 0x30
	WTVertex* m_vertexList[4];             // 0xd0: &m_vertex[i], for DrawPolygonFan
	int m_unknownE0;                       // 0xe0: zeroed by the constructor only
	ProjectionStyle::Enum m_projectionStyle; // 0xe4
};
CHECK_SIZE(C3dFont, 0xe8);
BEGIN_LAYOUT_CHECK(C3dFont)
	CHECK_FIELD(C3dFont, m_texture, 0x0);
	CHECK_FIELD(C3dFont, m_charSet, 0x4);
	CHECK_FIELD(C3dFont, m_columns, 0x8);
	CHECK_FIELD(C3dFont, m_cellU, 0xc);
	CHECK_FIELD(C3dFont, m_cellV, 0x10);
	CHECK_FIELD(C3dFont, m_charWidth, 0x14);
	CHECK_FIELD(C3dFont, m_charHeight, 0x18);
	CHECK_FIELD(C3dFont, m_offsetX, 0x1c);
	CHECK_FIELD(C3dFont, m_offsetY, 0x20);
	CHECK_FIELD(C3dFont, m_pos, 0x24);
	CHECK_FIELD(C3dFont, m_vertex, 0x30);
	CHECK_FIELD(C3dFont, m_vertexList, 0xd0);
	CHECK_FIELD(C3dFont, m_unknownE0, 0xe0);
	CHECK_FIELD(C3dFont, m_projectionStyle, 0xe4);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_3DFONT_H
