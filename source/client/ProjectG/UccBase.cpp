// Original: d:\build\custom\temp\code\client\projectg\uccbase.cpp
// The out-of-line copies of UccBase.h's inline functions live in this object.
#define UCCBASE_UCCBASE_INLINE
#include "UccBase.h"
#include "UccLibrary.h"
#include "frarea.h"
#include "wresrcmng.h"
#include "InputManager.h"
#include "UccDrawDlg.h"
#include "../client/Wangreal/include/wtypes.h"

#include <stdlib.h>
#include <string.h>

class FrGraphicInterface;

// client/ProjectG/UccLibrary (not yet declared in ucclibrary.h)
FrGraphicInterface* GetGDI(void);

// uccbase.cpp:90
void CDrawHistory::Clear(void) {
	for (unsigned int i = 0; i < m_histories.size(); i++) {
		delete m_histories[i];
	}
	m_histories.clear();
	m_index = -1;
}

// uccbase.cpp:100
void CDrawHistory::Undo(Bitmap* pBitmap) {
	if (pBitmap == NULL) {
		return;
	}

	if (IsUndoPossible()) {
		Draw(pBitmap, m_histories[m_index], false);
		m_index--;
	}
}

// uccbase.cpp:114
void CDrawHistory::Redo(Bitmap* pBitmap) {
	if (pBitmap == NULL) {
		return;
	}

	if (m_index >= -1 && m_index < (int)m_histories.size() - 1 && m_histories.size() != 0) {
		Draw(pBitmap, m_histories[++m_index], true);
	}
}

// uccbase.cpp:128
bool CDrawHistory::IsUndoPossible(void) {
	return m_index >= 0 && m_index < m_histories.size() && m_histories.size() != 0;
}

// uccbase.cpp:133
bool CDrawHistory::IsRedoPossible(void) {
	return m_index >= -1 && m_index < (int)m_histories.size() - 1 && m_histories.size() != 0;
}

// uccbase.cpp:138
void CDrawHistory::Draw(Bitmap* pBitmap, sDrawHistory* pHistory, bool bRedo) {
	if (pBitmap == NULL || pHistory == NULL) {
		return;
	}

	Bitmap* pSrc;
	if (bRedo) {
		pSrc = &pHistory->after;
	}
	else {
		pSrc = &pHistory->before;
	}

	CopyBitmap(pSrc, pBitmap, 0, 0, pHistory->width, pHistory->height, pHistory->x, pHistory->y);
}

// uccbase.cpp:159
void CDrawHistory::CopyBitmap(Bitmap* pSrc, Bitmap* pDest, int srcX, int srcY, int width, int height, int destX, int destY) {
	if (pSrc == NULL || pDest == NULL) {
		return;
	}

	if (pSrc->BitsPerPixel() != pDest->BitsPerPixel()) {
		return;
	}

	if (srcX + width > pSrc->Width() || srcY + height > pSrc->Height() ||
		destX + width > pDest->Width() || destY + height > pDest->Height()) {
		return;
	}

	int bytesPerPixel = pSrc->BitsPerPixel() / 8;

	for (int y = srcY; y < srcY + height; y++) {
		unsigned char* pSrcLine = pSrc->GetVram(y) + srcX * bytesPerPixel;
		unsigned char* pDestLine = pDest->GetVram(destY + y - srcY) + destX * bytesPerPixel;
		memcpy(pDestLine, pSrcLine, width * bytesPerPixel);
	}
}

// uccbase.cpp:193
CUccBase::CUccBase(void) {
	InitVariables();
}

// uccbase.cpp:198
CUccBase::~CUccBase(void) {
	DestroyVariables();
}

// uccbase.cpp:203
void CUccBase::InitVariables(void) {
	m_pTexture = NULL;
	m_pMask = NULL;
	m_pArea = NULL;
	m_pTexture32 = NULL;
	m_pTextureCache = NULL;
	m_pTexture32Cache = NULL;
	m_pDefaultBrush = NULL;
	m_bInitialized = false;
	m_refreshFlags = 0;
	m_lastPoint.x = -1;
	m_lastPoint.y = -1;

	m_zoomRate = 1;
	m_pZoomed = NULL;
	m_mode = UCC_MODE_DRAW;

	m_pParent = NULL;
}

// uccbase.cpp:226
void CUccBase::DestroyVariables(void) {
	ClearTexCache(m_pTexture); delete m_pTexture; m_pTexture = NULL;
	ClearTexCache(m_pMask); delete m_pMask; m_pMask = NULL;
	ClearTexCache(m_pTexture32); delete m_pTexture32; m_pTexture32 = NULL;
	ClearTexCache(m_pTextureCache); delete m_pTextureCache; m_pTextureCache = NULL;
	ClearTexCache(m_pTexture32Cache); delete m_pTexture32Cache; m_pTexture32Cache = NULL;
	ClearTexCache(m_pZoomed); delete m_pZoomed; m_pZoomed = NULL;
}

// uccbase.cpp:236
void CUccBase::Initialize(FrArea* pArea, Bitmap* pTexture, Bitmap* pMask) {
	if (m_pArea == NULL && pArea != NULL) {
		m_pArea = pArea;
	}

	if (m_pTexture == NULL && pTexture != NULL) {
		if (m_pArea != NULL) {
			SetTexture(pTexture);
		}
	}

	if (m_pMask == NULL && pMask != NULL) {
		SetMask(pMask);
	}

	m_refreshFlags = 7;
}

// uccbase.cpp:259
void CUccBase::Process(void) {
	if (m_refreshFlags & 1) {
		Refresh2D();
	}
	if (m_refreshFlags & 2) {
		Refresh3D();
	}
	if (m_refreshFlags & 4) {
		RefreshCache();
	}
}

// uccbase.cpp:276
void CUccBase::MouseEventHandler(eButton button, int state, const WPoint& point) {
	switch (m_mode) {
	case UCC_MODE_DRAW:
		DrawEventHandler(button, state, point);
		break;
	case UCC_MODE_MOVE:
		MoveEventHandler(button, state, point);
		break;
	case UCC_MODE_SPOID:
		SpoidEventHandler(button, state, point);
		break;
	case UCC_MODE_PAINT:
		PaintEventHandler(button, state, point);
		break;
	case UCC_MODE_ZOOM_IN:
	case UCC_MODE_ZOOM_OUT:
		ZoomEventHandler(button, state, point);
		break;
	}
}

// uccbase.cpp:321
void CUccBase::Refresh2D(void) {
	Convert24to32(m_pTexture, m_pTexture32, m_pMask);

	if (m_pArea != NULL && m_pTexture32 != NULL) {
		m_pArea->SetBgImg(m_pTexture32);
	}

	m_refreshFlags &= ~1;
}

// uccbase.cpp:333
void CUccBase::Refresh3D(void) {
	m_refreshFlags &= ~2;
}

// uccbase.cpp:338
void CUccBase::RefreshCache(void) {
	GetGDI();

	if (m_pTexture32 != NULL) {
		RefreshTexCache(m_pTexture32);
	}

	if (m_zoomRate > 1 && m_pZoomed != NULL) {
		RefreshTexCache(m_pZoomed);
	}

	m_refreshFlags &= ~4;
}

// uccbase.cpp:536
void CUccBase::DrawLine(IPoint from, IPoint to, Bitmap* pBrush) {
	if (pBrush == NULL) {
		return;
	}

	int length = abs(to.x - from.x) >= abs(to.y - from.y) ? abs(to.x - from.x) : abs(to.y - from.y);

	float stepX = (float)(to.x - from.x) / length;
	float stepY = (float)(to.y - from.y) / length;

	WPoint pos;
	pos.x = (float)from.x;
	pos.y = (float)from.y;

	while (true) {
		Draw((int)pos.x, (int)pos.y, pBrush, false);

		if ((length == abs(to.x - from.x) && pos.x == to.x) || (length == abs(to.y - from.y) && pos.y == to.y)) {
			break;
		}

		pos.x += stepX;
		pos.y += stepY;
	}

	m_refreshFlags |= 4;
}

// uccbase.cpp:572
void CUccBase::Undo(void) {
	m_textureHistory.Undo(m_pTexture);
	m_texture32History.Undo(m_pTexture32);

	if (m_zoomRate > 1) {
		_DrawZoomedArea();
	}

	m_refreshFlags |= 7;
}

// uccbase.cpp:584
void CUccBase::Redo(void) {
	m_textureHistory.Redo(m_pTexture);
	m_texture32History.Redo(m_pTexture32);

	if (m_zoomRate > 1) {
		_DrawZoomedArea();
	}

	m_refreshFlags |= 7;
}

// uccbase.cpp:596
bool CUccBase::IsUndoPossible(void) {
	return m_textureHistory.IsUndoPossible();
}

// uccbase.cpp:601
bool CUccBase::IsRedoPossible(void) {
	return m_textureHistory.IsRedoPossible();
}

// uccbase.cpp:606
void CUccBase::SetTexture(Bitmap* pTexture) {
	ClearTexCache(m_pTexture); delete m_pTexture; m_pTexture = NULL;
	m_pTexture = new Bitmap;
	*m_pTexture = *pTexture;

	ClearTexCache(m_pTexture32); delete m_pTexture32; m_pTexture32 = NULL;
	m_pTexture32 = new Bitmap(m_pTexture->Width(), m_pTexture->Height(), 32);

	m_refreshFlags |= 1;
}

// uccbase.cpp:618
void CUccBase::SetMask(Bitmap* pMask) {
	ClearTexCache(m_pMask); delete m_pMask; m_pMask = NULL;
	m_pMask = new Bitmap;
	*m_pMask = *pMask;
}

// uccbase.cpp:625
void CUccBase::SetDefaultBrush(Bitmap* pBrush) {
	ClearTexCache(m_pDefaultBrush); delete m_pDefaultBrush; m_pDefaultBrush = NULL;
	m_pDefaultBrush = new Bitmap;
	*m_pDefaultBrush = *pBrush;
}

// uccbase.cpp:632
bool CUccBase::IsMouseOver(const WPoint& point) {
	if (m_pArea == NULL || (m_pTexture == NULL && m_pTexture32 == NULL)) {
		return false;
	}

	if (!m_pArea->IsVisible()) {
		return false;
	}

	if (!m_pArea->IsViewFocused()) {
		return false;
	}

	WRect rc = m_pArea->GetRect();
	int width = 0;
	int height = 0;

	if (m_zoomRate > 1) {
		width = m_pZoomed->Width();
		height = m_pZoomed->Height();
	}
	else {
		if (m_pTexture != NULL) {
			width = m_pTexture->Width();
			height = m_pTexture->Height();
		}
		else if (m_pTexture32 != NULL) {
			width = m_pTexture32->Width();
			height = m_pTexture32->Height();
		}
	}

	if (rc.x <= point.x && point.x < rc.x + width && rc.y <= point.y && rc.y + height > point.y) {
		return true;
	}

	return false;
}

// uccbase.cpp:686
Bitmap* CUccBase::GetTexture32BPP(bool bOriginal) {
	if (m_zoomRate > 1 && !bOriginal) {
		return m_pZoomed;
	}
	else {
		return m_pTexture32;
	}
}

// uccbase.cpp:698
bool CUccBase::ZoomIn(int x, int y) {
	if (m_zoomRate >= 8) {
		return false;
	}

	m_zoomRate++;
	MakeZoomedArea(x, y);

	return true;
}

// uccbase.cpp:710
bool CUccBase::ZoomOut(int x, int y) {
	if (m_zoomRate <= 1) {
		return false;
	}

	m_zoomRate--;
	MakeZoomedArea(x, y);

	return true;
}

// uccbase.cpp:722
void CUccBase::MakeZoomedArea(int x, int y) {
	if (m_zoomRate == 1) {
		if (m_pTexture32 != NULL) {
			WRect rc = m_pArea->GetRect();
			rc.w = (float)m_pTexture32->Width();
			rc.h = (float)m_pTexture32->Height();
			m_pArea->SetRect(rc);

			RefreshTexCache(m_pTexture32);
			m_pArea->SetBgImg(m_pTexture32);
		}
		return;
	}

	IPoint offset((int)m_pTexture32->Width() / 2, (int)m_pTexture32->Height() / 2);
	if (x == -1 && y == -1) {
		x = (int)(m_zoomArea.x + m_zoomArea.w * 0.5f);
		y = (int)(m_zoomArea.y + m_zoomArea.h * 0.5f);
	}
	else {
		if (m_zoomRate * x < 128) {
			x = 256 / (m_zoomRate * 2);
		}
		if ((m_pTexture->Width() - x) * m_zoomRate < 128) {
			x = m_pTexture->Width() - 256 / (m_zoomRate * 2);
		}
		if (m_zoomRate * y < 128) {
			y = 256 / (m_zoomRate * 2);
		}
		if ((m_pTexture->Height() - y) * m_zoomRate < 128) {
			y = m_pTexture->Height() - 256 / (m_zoomRate * 2);
		}
	}

	offset.x = x - offset.x;
	offset.y = y - offset.y;

	int width = m_pTexture32->Width();
	int height = m_pTexture32->Height();

	if (m_zoomRate * width <= 256) {
		m_zoomArea.x = 0.0f;
		m_zoomArea.w = (float)width;
	}
	else {
		int margin = (m_zoomRate * width - 256) / 2;
		m_zoomArea.x = (float)(margin / m_zoomRate - 1);
		m_zoomArea.w = (float)((m_zoomRate * width - margin * 2) / m_zoomRate);
	}

	if (m_zoomRate * height <= 256) {
		m_zoomArea.y = 0.0f;
		m_zoomArea.h = (float)height;
	}
	else {
		int margin = (m_zoomRate * height - 256) / 2;
		m_zoomArea.y = (float)(margin / m_zoomRate - 1);
		m_zoomArea.h = (float)((m_zoomRate * height - margin * 2) / m_zoomRate);
	}

	m_zoomArea.x += offset.x;
	m_zoomArea.y += offset.y;

	if (m_zoomArea.x < 0.0f) {
		m_zoomArea.x = 0.0f;
	}
	if (m_zoomArea.y < 0.0f) {
		m_zoomArea.y = 0.0f;
	}
	if (m_zoomArea.w + m_zoomArea.x > m_pTexture32->Width()) {
		m_zoomArea.x = m_pTexture32->Width() - m_zoomArea.w;
	}
	if (m_zoomArea.h + m_zoomArea.y > m_pTexture32->Height()) {
		m_zoomArea.y = m_pTexture32->Height() - m_zoomArea.h;
	}

	_DrawZoomedArea();
}

// uccbase.cpp:840
void CUccBase::_DrawZoomedArea(void) {
	int zoomedWidth = (int)(m_zoomRate * m_zoomArea.w);
	int width = zoomedWidth < 256 ? zoomedWidth : 256;
	int zoomedHeight = (int)(m_zoomRate * m_zoomArea.h);
	int height = zoomedHeight < 256 ? zoomedHeight : 256;

	m_pArea->SetBgImg((const Bitmap*)NULL);
	ClearTexCache(m_pZoomed); delete m_pZoomed; m_pZoomed = NULL;
	m_pZoomed = new Bitmap(width, height, 32);
	if (m_pZoomed == NULL) {
		return;
	}

	for (int y = (int)m_zoomArea.y; y < m_zoomArea.y + m_zoomArea.h; y++) {
		for (int x = (int)m_zoomArea.x; x < m_zoomArea.x + m_zoomArea.w; x++) {
			unsigned char r, g, b, a;
			m_pTexture32->GetPixel(x, y, r, g, b, a);

			IPoint start, end;
			start.x = (int)((x - m_zoomArea.x) * m_zoomRate);
			end.x = (int)((x - m_zoomArea.x + 1) * m_zoomRate);
			start.y = (int)((y - m_zoomArea.y) * m_zoomRate);
			end.y = (int)((y - m_zoomArea.y + 1) * m_zoomRate);

			for (int zy = start.y; zy < end.y; zy++) {
				for (int zx = start.x; zx < end.x; zx++) {
					m_pZoomed->SetPixel(zx, zy, r, g, b, a);
				}
			}
		}
	}

	WRect rc = m_pArea->GetRect();
	rc.w = (float)m_pZoomed->Width();
	rc.h = (float)m_pZoomed->Height();
	m_pArea->SetRect(rc);

	m_pArea->SetBgImg(m_pZoomed);
}

// uccbase.cpp:1023
void CUccBase::MoveEventHandler(eButton button, int state, const WPoint& point) {
	if (m_zoomRate <= 1) {
		return;
	}

	if (m_zoomArea.x == 0.0f && m_zoomArea.w == m_pTexture32->Width() &&
		m_zoomArea.y == 0.0f && m_zoomArea.h == m_pTexture32->Height()) {
		return;
	}

	if (button != LEFT_BUTTON) {
		return;
	}

	switch (state) {
	case 1: {
		m_bInitialized = true;
		WRect rc = m_pArea->GetRect();
		IPoint pos(point.x - rc.x, point.y - rc.y);
		if (pos == m_lastPoint) {
			return;
		}

		m_lastPoint = pos;
		break;
	}
	case 2:
		m_bInitialized = false;
		m_lastPoint.x = -1;
		m_lastPoint.y = -1;
		break;
	case 3: {
		WRect rc = m_pArea->GetRect();
		IPoint pos(point.x - rc.x, point.y - rc.y);
		if (pos == m_lastPoint) {
			return;
		}

		if (m_lastPoint.x == -1 || m_lastPoint.y == -1) {
			return;
		}

		IPoint delta(m_lastPoint.x - pos.x, m_lastPoint.y - pos.y);
		delta.x = delta.x / m_zoomRate;
		delta.y = delta.y / m_zoomRate;

		if (delta.x == 0 && delta.y == 0) {
			return;
		}

		if (m_zoomArea.w + m_zoomArea.x + delta.x > m_pTexture32->Width()) {
			delta.x = (int)(m_pTexture32->Width() - (m_zoomArea.w + m_zoomArea.x));
		}
		if (m_zoomArea.h + m_zoomArea.y + delta.y > m_pTexture32->Height()) {
			delta.y = (int)(m_pTexture32->Height() - (m_zoomArea.h + m_zoomArea.y));
		}

		if (delta.x + m_zoomArea.x < 0.0f) {
			delta.x = (int)-m_zoomArea.x;
		}
		if (delta.y + m_zoomArea.y < 0.0f) {
			delta.y = (int)-m_zoomArea.y;
		}

		m_lastPoint = pos;
		m_zoomArea.x += delta.x;
		m_zoomArea.y += delta.y;
		_DrawZoomedArea();
		break;
	}
	}
}

// uccbase.cpp:1109
void CUccBase::SpoidEventHandler(eButton button, int state, const WPoint& point) {
	if (button != LEFT_BUTTON) {
		return;
	}

	switch (state) {
	case 1:
	case 3: {
		WRect rc = m_pArea->GetRect();
		IPoint pos(point.x - rc.x, point.y - rc.y);

		unsigned char r = 0, g = 0, b = 0;
		(m_zoomRate > 1 ? m_pZoomed : m_pTexture32)->GetPixel(pos.x, pos.y, r, g, b);

		if (m_pParent != NULL) {
			m_pParent->SetCurrentColor(r, g, b);
		}
		break;
	}
	case 2:
		m_mode = UCC_MODE_DRAW;
		break;
	}
}

// uccbase.cpp:1146
void CUccBase::PaintEventHandler(eButton button, int state, const WPoint& point) {
	if (button != LEFT_BUTTON) {
		return;
	}

	switch (state) {
	case 2: {
		m_pTextureCache = new Bitmap;
		*m_pTextureCache = *m_pTexture;
		ClearTexCache(m_pTexture32Cache); delete m_pTexture32Cache; m_pTexture32Cache = NULL;
		m_pTexture32Cache = new Bitmap;
		*m_pTexture32Cache = *m_pTexture32;

		WRect rc = m_pArea->GetRect();
		IPoint pos(point.x - rc.x, point.y - rc.y);

		Paint(pos.x, pos.y, true, 0, 0, 0);

		if (m_zoomRate > 1) {
			_DrawZoomedArea();
		}

		IPoint from(0, 0), to((int)m_pTexture->Width() - 1, (int)m_pTexture->Height() - 1);

		m_textureHistory.Add(m_pTextureCache, m_pTexture, from, to);
		m_texture32History.Add(m_pTexture32Cache, m_pTexture32, from, to);

		m_refreshFlags |= 7;
		m_mode = UCC_MODE_DRAW;
		m_pParent->SetSaved(false);
		break;
	}
	}
}

// uccbase.cpp:1281
void CUccBase::ZoomEventHandler(eButton button, int state, const WPoint& point) {
	if (button != LEFT_BUTTON) {
		return;
	}

	switch (state) {
	case 2: {
		WRect rc = m_pArea->GetRect();
		int x = (int)(point.x - rc.x);
		int y = (int)(point.y - rc.y);

		if (m_zoomRate > 1) {
			x = (int)(x / m_zoomRate + m_zoomArea.x);
			y = (int)(y / m_zoomRate + m_zoomArea.y);
		}

		if (m_mode == UCC_MODE_ZOOM_IN) {
			ZoomIn(x, y);
			if (m_zoomRate == 8) {
				m_mode = UCC_MODE_DRAW;
			}
		}
		else if (m_mode == UCC_MODE_ZOOM_OUT) {
			ZoomOut(x, y);
			if (m_zoomRate == 1) {
				m_mode = UCC_MODE_DRAW;
			}
		}

		if (m_pParent != NULL) {
			m_pParent->AfterZoom();
		}
		break;
	}
	}
}

// uccbase.cpp:1330
CUccBaseItemDraw::CUccBaseItemDraw(void) {
	m_textureName[0] = '\0';
	m_uccKey[0] = '\0';
	m_typeId = 0;
}

// uccbase.cpp:1338
CUccBaseItemDraw::~CUccBaseItemDraw(void) {
}

// uccbase.cpp:1342
void CUccBaseItemDraw::InitializeItemDraw(int index, FrArea* pArea, Bitmap* pTexture, Bitmap* pMask, unsigned long typeId, const char* uccKey) {
	CUccBase::Initialize(pArea, pTexture, pMask);

	strncpy(m_uccKey, uccKey, 9);
	m_typeId = typeId;

	GenerateTextureName(index, typeId, m_uccKey);

	m_textureHistory.Clear();
	m_texture32History.Clear();

	Convert24to32(m_pTexture, m_pTexture32, m_pMask);
}

// uccbase.cpp:1357
bool CUccBaseItemDraw::IsInitialized(void) {
	return strlen(m_uccKey) != 0 && m_typeId != 0 && m_pArea != NULL && m_pTexture != NULL && m_pMask != NULL;
}

// uccbase.cpp:1362
void CUccBaseItemDraw::Refresh2D(void) {
	if (m_pArea != NULL) {
		Bitmap* pBitmap;
		if (m_zoomRate > 1) {
			pBitmap = m_pZoomed;
		}
		else {
			pBitmap = m_pTexture32;
		}
		if (pBitmap != NULL) {
			m_pArea->SetBgImg(pBitmap);
		}
	}

	m_refreshFlags &= ~1;
}

// uccbase.cpp:1381
void CUccBaseItemDraw::Refresh3D(void) {
	char* textureName = m_textureName;
	w_texlist* pTexList = g_resrcmng->FindTexture(textureName);
	if (pTexList != NULL) {
		if (m_pTexture != NULL) {
			g_resrcmng->FixTexture(pTexList->texhandle, m_pTexture, 0, NULL);
		}
	}
	else {
		if (textureName != NULL && m_pTexture != NULL) {
			g_resrcmng->UploadTexture(textureName, m_pTexture, 0, NULL);
		}
	}

	m_refreshFlags &= ~2;
}

// uccbase.cpp:1402
void CUccBaseItemDraw::GenerateTextureName(int index, unsigned long typeId, const char* uccKey) {
	const char* key = uccKey;
	if (key == NULL) {
		key = m_uccKey;
	}
	if (strlen(key) == 0) {
		return;
	}

	if (index == 0) {
		strcpy(m_textureName, GenerateTextureName_F(typeId, uccKey).c_str());
	}
	else {
		strcpy(m_textureName, GenerateTextureName_B(typeId, uccKey).c_str());
	}
}

// uccbase.cpp:1420
void CUccBaseItemDraw::Process(void) {
	if (m_refreshFlags & 1) {
		Refresh2D();
	}
	if (m_refreshFlags & 2) {
		Refresh3D();
	}
	if (m_refreshFlags & 4) {
		RefreshCache();
	}
}
