// Original: d:\build\custom\temp\code\client\projectg\uccbase.h
#ifndef PG_CLIENT_PROJECTG_UCCBASE_H
#define PG_CLIENT_PROJECTG_UCCBASE_H

#include "..\..\defines.h"
#include "..\..\padding.h"

#include <vector>

#include "bitmap.h"
#include "UccLibrary.h"
#include "../client/Wangreal/include/wtypes.h"

class FrArea;
class FrUccDrawDlg;
enum eButton;   // declared with CInputManager (LEFT_BUTTON, RIGHT_BUTTON, MIDDLE_BUTTON)

// One undo step: the area a stroke changed, before and after it.
struct sDrawHistory {
	sDrawHistory(void);      // uccbase.h:21

	int x;              // 0x0
	int y;              // 0x4
	int width;          // 0x8
	int height;         // 0xc
	Bitmap before;      // 0x10
	Bitmap after;       // 0x24
};

CHECK_SIZE(sDrawHistory, 0x38);
CHECK_FIELD(sDrawHistory, before, 0x10);
CHECK_FIELD(sDrawHistory, after, 0x24);

// Undo/redo list of a UCC bitmap (CUccBase keeps one per edited bitmap).
class CDrawHistory {
	LAYOUT_CHECK_FRIEND;
public:
	CDrawHistory(void) : m_index(-1) {}   // uccbase.h:38
	~CDrawHistory(void);

	void Add(Bitmap* pBefore, Bitmap* pAfter, IPoint from, IPoint to);
	bool IsUndoPossible(void);
	bool IsRedoPossible(void);
	void Undo(Bitmap* pBitmap);
	void Redo(Bitmap* pBitmap);
	void Clear(void);

private:
	void Draw(Bitmap* pBitmap, sDrawHistory* pHistory, bool bRedo);
	void CopyBitmap(Bitmap* pSrc, Bitmap* pDest, int srcX, int srcY, int width, int height, int destX, int destY);

	int m_index;                                // 0x0, current step, -1 when empty
	std::vector<sDrawHistory*> m_histories;     // 0x4
};

CHECK_SIZE(CDrawHistory, 0x14);
BEGIN_LAYOUT_CHECK(CDrawHistory)
	CHECK_FIELD(CDrawHistory, m_index, 0x0);
	CHECK_FIELD(CDrawHistory, m_histories, 0x4);
END_LAYOUT_CHECK

// Tool of the UCC canvas; CUccBase::MouseEventHandler dispatches on it to
// DrawEventHandler, MoveEventHandler, SpoidEventHandler, PaintEventHandler
// and ZoomEventHandler (zoom in and zoom out).
enum eMode {
	UCC_MODE_DRAW = 0,
	UCC_MODE_MOVE = 1,
	UCC_MODE_SPOID = 2,
	UCC_MODE_PAINT = 3,
	UCC_MODE_ZOOM_IN = 4,
	UCC_MODE_ZOOM_OUT = 5
};

// Drawing canvas of the UCC (user-created content) editor. Size 0x88 (the
// first member of CUccBaseItemDraw is at 0x88). Only the members the
// transcribed code uses are declared; the virtual functions are in slot
// order (vtable ??_7CUccBase@@6B@, 18 slots).
class CUccBase {
	LAYOUT_CHECK_FRIEND;
	friend class FrUccDrawDlg;   // AfterZoom reads m_zoomRate, Reset sets m_refreshFlags
public:
	CUccBase(void);
	virtual ~CUccBase(void);                                                         // slot 0
	virtual void Initialize(FrArea* pArea, Bitmap* pTexture, Bitmap* pMask);         // slot 1
	virtual bool IsInitialized(void);                                                // slot 2, uccbase.h:102
	virtual void InitVariables(void);                                                // slot 3
	virtual void DestroyVariables(void);                                             // slot 4
	virtual void Process(void);                                                      // slot 5
	virtual void MouseEventHandler(eButton button, int state, const WPoint& point);  // slot 6
	virtual bool IsMouseOver(const WPoint& point);                                   // slot 7
	virtual void Draw(int x, int y, Bitmap* pBrush, bool b);                         // slot 8
	virtual void DrawLine(IPoint from, IPoint to, Bitmap* pBrush);                   // slot 9

protected:
	virtual void Refresh2D(void);                                                    // slot 10
	virtual void Refresh3D(void);                                                    // slot 11
	virtual void RefreshCache(void);                                                 // slot 12
	virtual void DrawEventHandler(eButton button, int state, const WPoint& point);   // slot 13
	virtual void MoveEventHandler(eButton button, int state, const WPoint& point);   // slot 14
	virtual void SpoidEventHandler(eButton button, int state, const WPoint& point);  // slot 15
	virtual void PaintEventHandler(eButton button, int state, const WPoint& point);  // slot 16
	virtual void ZoomEventHandler(eButton button, int state, const WPoint& point);   // slot 17

public:

	void SetParent(FrUccDrawDlg* pParent);                           // uccbase.h:106
	void MustRefresh(int flags);                                     // uccbase.h:120
	bool IsZoomed(void);                                             // uccbase.h:125
	int GetZoomRate(void);                                           // uccbase.h:126
	void SetMode(eMode mode);                                        // uccbase.h:128
	eMode GetMode(void) { return m_mode; }                           // uccbase.h:129
	Bitmap* GetTexture(void);                                        // uccbase.h:141
	Bitmap* GetMask(void);                                           // uccbase.h:144
	void SetArea(FrArea* pArea);                                     // uccbase.h:148

	void SetTexture(Bitmap* pTexture);
	void SetMask(Bitmap* pMask);
	void SetDefaultBrush(Bitmap* pBrush);
	bool IsUndoPossible(void);
	bool IsRedoPossible(void);
	Bitmap* GetTexture32BPP(bool bOriginal);
	void MakeZoomedArea(int x, int y);
	bool ZoomIn(int x, int y);
	bool ZoomOut(int x, int y);
	void Paint(int x, int y, bool bFill, unsigned char r, unsigned char g, unsigned char b);
	void Undo(void);
	void Redo(void);

protected:
	void _DrawZoomedArea(void);

protected:
	bool m_bInitialized;         // 0x4

protected:
	Bitmap* m_pTexture;          // 0x8
	Bitmap* m_pMask;             // 0xc
	Bitmap* m_pTexture32;        // 0x10, the 32 bpp copy shown in m_pArea (Refresh2D)
	FrArea* m_pArea;             // 0x14, the window the canvas is drawn in (SetArea)

protected:
	Bitmap* m_pDefaultBrush;     // 0x18, a copy of the brush given to SetDefaultBrush
	IPoint m_lastPoint;          // 0x1c, (-1, -1) when no stroke
	CDrawHistory m_textureHistory;     // 0x24, undo list of m_pTexture
	Bitmap* m_pTextureCache;           // 0x38
	CDrawHistory m_texture32History;   // 0x3c, undo list of m_pTexture32
	Bitmap* m_pTexture32Cache;         // 0x50
	IPoint m_strokeMin;          // 0x54, bounds of the current stroke, passed to CDrawHistory::Add
	IPoint m_strokeMax;          // 0x5c

protected:
	int m_refreshFlags;          // 0x64, parts to refresh (1: Refresh2D, 4: RefreshCache); Initialize sets 7

protected:
	WRect m_zoomArea;            // 0x68, the texture area the zoomed view shows (MakeZoomedArea)
	int m_zoomRate;              // 0x78, 1 (not zoomed) to 8 (ZoomIn, ZoomOut)
	Bitmap* m_pZoomed;           // 0x7c, the zoomed view (GetTexture32BPP)
	eMode m_mode;                // 0x80
	FrUccDrawDlg* m_pParent;     // 0x84
};

CHECK_SIZE(CUccBase, 0x88);
BEGIN_LAYOUT_CHECK(CUccBase)
	CHECK_FIELD(CUccBase, m_pTexture, 0x8);
	CHECK_FIELD(CUccBase, m_pMask, 0xc);
	CHECK_FIELD(CUccBase, m_pTexture32, 0x10);
	CHECK_FIELD(CUccBase, m_pArea, 0x14);
	CHECK_FIELD(CUccBase, m_bInitialized, 0x4);
	CHECK_FIELD(CUccBase, m_pDefaultBrush, 0x18);
	CHECK_FIELD(CUccBase, m_lastPoint, 0x1c);
	CHECK_FIELD(CUccBase, m_textureHistory, 0x24);
	CHECK_FIELD(CUccBase, m_pTextureCache, 0x38);
	CHECK_FIELD(CUccBase, m_texture32History, 0x3c);
	CHECK_FIELD(CUccBase, m_pTexture32Cache, 0x50);
	CHECK_FIELD(CUccBase, m_strokeMin, 0x54);
	CHECK_FIELD(CUccBase, m_strokeMax, 0x5c);
	CHECK_FIELD(CUccBase, m_zoomArea, 0x68);
	CHECK_FIELD(CUccBase, m_refreshFlags, 0x64);
	CHECK_FIELD(CUccBase, m_zoomRate, 0x78);
	CHECK_FIELD(CUccBase, m_pZoomed, 0x7c);
	CHECK_FIELD(CUccBase, m_mode, 0x80);
	CHECK_FIELD(CUccBase, m_pParent, 0x84);
END_LAYOUT_CHECK

// A CUccBase drawing on an item's texture. Its size is not established yet.
class CUccBaseItemDraw : public CUccBase {
	LAYOUT_CHECK_FRIEND;
public:
	CUccBaseItemDraw(void);
	virtual ~CUccBaseItemDraw(void);
	virtual bool IsInitialized(void);
	virtual void Process(void);

	void InitializeItemDraw(int index, FrArea* pArea, Bitmap* pTexture, Bitmap* pMask, unsigned long typeId, const char* uccKey);

	char* GetTextureName(void);   // uccbase.h:212

private:
	virtual void Refresh2D(void);
	virtual void Refresh3D(void);
	void GenerateTextureName(int index, unsigned long typeId, const char* uccKey);

	unsigned long m_typeId;       // 0x88, set by InitializeItemDraw
	char m_textureName[64];       // 0x8c
	char m_uccKey[9];             // 0xcc, copied by InitializeItemDraw (strncpy 9)
};

BEGIN_LAYOUT_CHECK(CUccBaseItemDraw)
	CHECK_FIELD(CUccBaseItemDraw, m_typeId, 0x88);
	CHECK_FIELD(CUccBaseItemDraw, m_uccKey, 0xcc);
	CHECK_FIELD(CUccBaseItemDraw, m_textureName, 0x8c);
END_LAYOUT_CHECK

// UccBase.cpp defines UCCBASE_UCCBASE_INLINE empty before its includes: the
// original holds the out-of-line copy of these inline functions in UccBase's
// object; every other unit inlines them.
#ifndef UCCBASE_UCCBASE_INLINE
#define UCCBASE_UCCBASE_INLINE inline
#endif

// uccbase.h:21
UCCBASE_UCCBASE_INLINE sDrawHistory::sDrawHistory(void) {
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

// uccbase.h:102
UCCBASE_UCCBASE_INLINE bool CUccBase::IsInitialized(void) {
	return m_pArea != NULL && m_pTexture != NULL && m_pMask != NULL;
}

// uccbase.h:120
UCCBASE_UCCBASE_INLINE void CUccBase::MustRefresh(int flags) {
	m_refreshFlags |= flags;
}

// uccbase.h:125
UCCBASE_UCCBASE_INLINE bool CUccBase::IsZoomed(void) {
	return m_zoomRate > 1;
}

// uccbase.h:126
UCCBASE_UCCBASE_INLINE int CUccBase::GetZoomRate(void) {
	return m_zoomRate;
}

// uccbase.h:128
UCCBASE_UCCBASE_INLINE void CUccBase::SetMode(eMode mode) {
	m_mode = mode;
}

// uccbase.h:148
UCCBASE_UCCBASE_INLINE void CUccBase::SetArea(FrArea* pArea) {
	m_pArea = pArea;
}

// UccDrawDlg.cpp defines UCCDRAWDLG_UCCBASE_INLINE empty before its includes:
// the original holds the out-of-line copy of these inline accessors in
// UccDrawDlg's object; every other unit inlines them.
#ifndef UCCDRAWDLG_UCCBASE_INLINE
#define UCCDRAWDLG_UCCBASE_INLINE inline
#endif

// uccbase.h:141
UCCDRAWDLG_UCCBASE_INLINE Bitmap* CUccBase::GetTexture(void) {
	return m_pTexture;
}

// uccbase.h:144
UCCDRAWDLG_UCCBASE_INLINE Bitmap* CUccBase::GetMask(void) {
	return m_pMask;
}

#endif // PG_CLIENT_PROJECTG_UCCBASE_H
