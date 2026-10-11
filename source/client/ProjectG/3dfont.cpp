// Original: d:\build\custom\temp\code\client\projectg\3dfont.cpp
#include "3dfont.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "wresrcmng.h"
#include "wview.h"

// 3dfont.cpp:19
C3dFont::C3dFont(void)
	: m_texture(0), m_projectionStyle(ProjectionStyle::Default)
{
	m_pos.x = m_pos.y = m_pos.z = 0.0f;

	for (int i = 0; i < 4; i++) {
		m_vertexList[i] = &m_vertex[i];
	}
	m_unknownE0 = 0;
}

// 3dfont.cpp:31
C3dFont::~C3dFont(void)
{
	if (m_texture) {
		g_resrcmng->Release(m_texture);
		m_texture = 0;
	}
}

// 3dfont.cpp:40
void C3dFont::LoadTexture(const char* filename, int charWidth, int charHeight, const char* charSet, float offsetX, float offsetY)
{
	m_texture = g_resrcmng->LoadTexture(filename, 0, 0, 0);
	int width = g_resrcmng->GetTextureWidth(m_texture);
	int height = g_resrcmng->GetTextureHeight(m_texture);

	m_cellU = (float)charWidth / width;
	m_columns = width / charWidth;
	m_charSet = charSet;

	m_offsetX = offsetX;
	m_offsetY = offsetY;

	m_charWidth = charWidth;
	m_charHeight = charHeight;
	m_cellV = (float)charHeight / height;
}

// 3dfont.cpp:58
void C3dFont::SetPos(const WVector& pos)
{
	m_pos = pos;
}

// 3dfont.cpp:63
void C3dFont::SetSize(int width, int height)
{
	m_charWidth = width;
	m_charHeight = height;
}

// 3dfont.cpp:69
void C3dFont::SetColor(int color)
{
	for (int i = 0; i < 4; i++) m_vertexList[i]->diffuse = color;
}

// 3dfont.cpp:76
void C3dFont::Printf(WView* view, int spacing, const char* format, ...)
{
	if (m_columns == 0) {
		return;
	}

	WVector pos = view->Projection(m_pos);
	if (pos.z <= 0.0f || pos.z >= 1.0f) {
		return;
	}

	for (int i = 0; i < 4; i++) {
		m_vertex[i].z = pos.z;
		m_vertex[i].rhw = 1.0f;
	}

	char buff[1024];
	va_list args;
	va_start(args, format);
	if (_vsnprintf(buff, 1023, format, args) == -1) {
		buff[1023] = 0;
	}

	int len = strlen(buff);

	const char* charSet = m_charSet;
	if (charSet == NULL) {
		return;
	}

	for (int n = 0; n < len; n++) {
		int ch = buff[n];
		int cell = strchr(charSet, ch) - charSet;

		float u = (cell % m_columns) * m_cellU;
		float v = (cell / m_columns) * m_cellV;

		m_vertex[0].tu = u;
		m_vertex[0].tv = v + m_cellV;
		m_vertex[1].tu = u;
		m_vertex[1].tv = v;
		m_vertex[2].tu = u + m_cellU;
		m_vertex[2].tv = v;
		m_vertex[3].tu = u + m_cellU;
		m_vertex[3].tv = v + m_cellV;

		float left = m_offsetX - (m_charWidth * len + (len - 1) * spacing) * 0.5f + (m_charWidth + spacing) * n;
		float right = left + m_charWidth;
		float top = m_offsetY - m_charHeight * 0.5f;
		float bottom = top + m_charHeight;

		float x0 = pos.x + left;
		float x1;
		float y0;
		float y1;
		if (x0 > g_view->GetWidth() - 0.5f || (x1 = pos.x + right) < 0.5f
			|| (y0 = pos.y + top) > g_view->GetHeight() - 0.5f || (y1 = pos.y + bottom) < 0.5f) {
			continue;
		}

		m_vertex[0].x = x0;
		m_vertex[0].y = y1;
		m_vertex[1].x = x0;
		m_vertex[1].y = y0;
		m_vertex[2].x = x1;
		m_vertex[2].y = y0;
		m_vertex[3].x = x1;
		m_vertex[3].y = y1;

		for (int j = 0; j < 4; j++) {
			ClipChar(j);
		}

		int flags = (m_texture & 0x7ff) | 0x2080000;
		if (m_projectionStyle == ProjectionStyle::Overlay) {
			flags |= 0x300000;
		}

		view->DrawPolygonFan(m_vertexList, flags, 0, true);
	}
}

// 3dfont.cpp:205
void C3dFont::ClipChar(int index)
{
	if (m_vertex[index].x < 0.5f) {
		m_vertex[index].tu += (0.5f - m_vertex[index].x) * m_cellU / m_charWidth;
		m_vertex[index].x = 0.5f;
	} else if (g_view->GetWidth() - 0.5f < m_vertex[index].x) {
		m_vertex[index].tu -= (m_vertex[index].x - g_view->GetWidth() + 0.5f) * m_cellU / m_charWidth;
		m_vertex[index].x = g_view->GetWidth() - 0.5f;
	}

	if (m_vertex[index].y < 0.5f) {
		m_vertex[index].tv += (0.5f - m_vertex[index].y) * m_cellV / m_charHeight;
		m_vertex[index].y = 0.5f;
	} else if (g_view->GetHeight() - 0.5f < m_vertex[index].y) {
		m_vertex[index].tv -= (m_vertex[index].y - g_view->GetHeight() + 0.5f) * m_cellV / m_charHeight;
		m_vertex[index].y = g_view->GetHeight() - 0.5f;
	}
}
