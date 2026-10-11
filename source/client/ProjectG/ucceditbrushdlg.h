// Original: d:\build\custom\temp\code\client\projectg\ucceditbrushdlg.h
#ifndef PG_CLIENT_PROJECTG_UCCEDITBRUSHDLG_H
#define PG_CLIENT_PROJECTG_UCCEDITBRUSHDLG_H

#include <stddef.h>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "bitmap.h"
#include "UccBase.h"
#include "UccBrush.h"

// The canvas of the brush editor: a CUccBase whose texture is the brush being
// drawn, magnified (ProduceOriginalBrush samples it back to 24x24). Size 0x9c
// (FrUccEditBrushDlg places the next member there). No virtual function of its
// own past CUccBase's 18 slots.
class CUccEditBrush : public CUccBase {
	LAYOUT_CHECK_FRIEND;
public:
	CUccEditBrush(void);
	virtual ~CUccEditBrush(void);

	virtual bool IsInitialized(void) { return m_pTexture != NULL; }   // ucceditbrushdlg.h:20
	virtual void Process(void);
	virtual void DrawLine(IPoint from, IPoint to, Bitmap* pBrush);

	Bitmap* GetTexture32BPP(void);
	Bitmap* ProduceOriginalBrush(void);

protected:
	virtual void Refresh2D(void);   // ucceditbrushdlg.h:28
	virtual void Refresh3D(void);   // ucceditbrushdlg.h:29

private:
	Bitmap m_originalBrush;   // 0x88, the 24x24 brush ProduceOriginalBrush returns
};

CHECK_SIZE(CUccEditBrush, 0x9c);
BEGIN_LAYOUT_CHECK(CUccEditBrush)
	CHECK_FIELD(CUccEditBrush, m_originalBrush, 0x88);
END_LAYOUT_CHECK

// Brush editor of the UCC editor: draws a new brush (SetIndex with index -1)
// or modifies one of the brush list of a type. Its size (0x1e8) is the `new`
// of FrUccEditBrushDlgMakeInstance.
class FrUccEditBrushDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ucceditbrushdlg.h:37

	FrUccEditBrushDlg(void);
	virtual ~FrUccEditBrushDlg(void);   // ucceditbrushdlg.cpp:128

	void InitNew(void);
	void InitModify(void);
	void SetIndex(eBrushType type, int index);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // ucceditbrushdlg.cpp:97

	virtual void OnProc(const float dt);
	void OnCanvasInit(intptr_t);
	void OnOKBtnUp(void);
	void OnCancelBtnUp(void);

private:
	int m_brushIndex;              // 0x110, -2 before SetIndex, -1 for a new brush
	eBrushType m_brushType;        // 0x114
	FrArea* m_pCanvas;             // 0x118
	Bitmap* m_pCanvasImage;        // 0x11c, the background of m_pCanvas
	Bitmap m_defaultBrush[2];      // 0x120, the pens of the left and right buttons
	CUccEditBrush m_editBrush;     // 0x148
	PADDING(0x1e4, 0x1e8);         // FrUccEditBrushDlg members
};

CHECK_SIZE(FrUccEditBrushDlg, 0x1e8);
BEGIN_LAYOUT_CHECK(FrUccEditBrushDlg)
	CHECK_FIELD(FrUccEditBrushDlg, m_brushIndex, 0x110);
	CHECK_FIELD(FrUccEditBrushDlg, m_brushType, 0x114);
	CHECK_FIELD(FrUccEditBrushDlg, m_pCanvas, 0x118);
	CHECK_FIELD(FrUccEditBrushDlg, m_pCanvasImage, 0x11c);
	CHECK_FIELD(FrUccEditBrushDlg, m_defaultBrush, 0x120);
	CHECK_FIELD(FrUccEditBrushDlg, m_editBrush, 0x148);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_UCCEDITBRUSHDLG_H
