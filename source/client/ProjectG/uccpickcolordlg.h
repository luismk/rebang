// Original: d:\build\custom\temp\code\client\projectg\uccpickcolordlg.h
#ifndef PG_CLIENT_PROJECTG_UCCPICKCOLORDLG_H
#define PG_CLIENT_PROJECTG_UCCPICKCOLORDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "bitmap.h"
#include "ucccolorpicker.h"

class FrArea;
class FrButton;
class FrListBox;
class FrUccDrawDlg;

// Colour picker of the UCC drawing dialog: a colour map, a brightness bar
// with two cursors, the drawing dialog's palette and the new/previous colour
// swatches. Size 0x288 (the `new` of FrUccPickColorDlgMakeInstance).
class FrUccPickColorDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // uccpickcolordlg.h:12

	FrUccPickColorDlg(void);

	void SetParent(FrUccDrawDlg* pParent);
	void Init(void);
	void BuildColorList(void);
	void MoveCanvasCursor(unsigned char red, unsigned char green, unsigned char blue);
	void MoveCanvasCursor(const WPoint& point);
	void MoveCanvasCursor(unsigned char x, unsigned char y);
	void MoveBrightnessCursor(unsigned char red, unsigned char green, unsigned char blue);
	void MoveBrightnessCursor(unsigned char brightness);
	void MoveBrightnessCursor(const WPoint& point);
	void SetCurrentColor(unsigned char red, unsigned char green, unsigned char blue);

private:
	static sFRESH_ENTRY _MsgEntries[];

	void FindColor(unsigned char red, unsigned char green, unsigned char blue);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnProc(const float dt);

	void OnCancelBtnInit(intptr_t param);
	void OnCancelBtnUp(void);
	void OnOkBtnInit(intptr_t param);
	void OnOkBtnUp(void);
	void OnUseColorBtnInit(intptr_t param);
	void OnUseColorBtnUp(void);
	void OnColorListInit(intptr_t param);
	void OnColorListOwnerDraw(intptr_t param);
	void OnColorListBtnDown(void);
	void OnCanvasInit(intptr_t param);
	void OnCanvasCursorInit(intptr_t param);
	void OnBrightnessCanvasInit(intptr_t param);
	void OnBrightnessCursor1Init(intptr_t param);
	void OnBrightnessCursor2Init(intptr_t param);
	void OnNewColorInit(intptr_t param);
	void OnPrevColorInit(intptr_t param);

	FrUccDrawDlg* m_pParent;                  // 0x110
	Bitmap m_prevColorBmp;                    // 0x114, 1x1, shown by m_pPrevColor
	Bitmap m_newColorBmp;                     // 0x128, 1x1, shown by m_pNewColor
	unsigned char m_red;                      // 0x13c, the colour being picked
	unsigned char m_green;                    // 0x13d
	unsigned char m_blue;                     // 0x13e
	CUccColorPicker m_colorPicker;            // 0x140
	CUccColorBrightness m_colorBrightness;    // 0x1cc
	const Bitmap* m_pSelectColor;             // 0x258, frame drawn around the selected palette colour
	FrButton* m_pCancelBtn;                   // 0x25c
	FrButton* m_pOkBtn;                       // 0x260
	FrButton* m_pUseColorBtn;                 // 0x264
	FrListBox* m_pColorList;                  // 0x268, lists the parent's palette
	FrArea* m_pCanvas;                        // 0x26c, the colour map
	FrArea* m_pCanvasCursor;                  // 0x270
	FrArea* m_pBrightnessCanvas;              // 0x274
	FrArea* m_pBrightnessCursor1;             // 0x278
	FrArea* m_pBrightnessCursor2;             // 0x27c
	FrArea* m_pNewColor;                      // 0x280
	FrArea* m_pPrevColor;                     // 0x284
};

CHECK_SIZE(FrUccPickColorDlg, 0x288);
BEGIN_LAYOUT_CHECK(FrUccPickColorDlg)
	CHECK_FIELD(FrUccPickColorDlg, m_pParent, 0x110);
	CHECK_FIELD(FrUccPickColorDlg, m_prevColorBmp, 0x114);
	CHECK_FIELD(FrUccPickColorDlg, m_newColorBmp, 0x128);
	CHECK_FIELD(FrUccPickColorDlg, m_red, 0x13c);
	CHECK_FIELD(FrUccPickColorDlg, m_green, 0x13d);
	CHECK_FIELD(FrUccPickColorDlg, m_blue, 0x13e);
	CHECK_FIELD(FrUccPickColorDlg, m_colorPicker, 0x140);
	CHECK_FIELD(FrUccPickColorDlg, m_colorBrightness, 0x1cc);
	CHECK_FIELD(FrUccPickColorDlg, m_pSelectColor, 0x258);
	CHECK_FIELD(FrUccPickColorDlg, m_pCancelBtn, 0x25c);
	CHECK_FIELD(FrUccPickColorDlg, m_pOkBtn, 0x260);
	CHECK_FIELD(FrUccPickColorDlg, m_pUseColorBtn, 0x264);
	CHECK_FIELD(FrUccPickColorDlg, m_pColorList, 0x268);
	CHECK_FIELD(FrUccPickColorDlg, m_pCanvas, 0x26c);
	CHECK_FIELD(FrUccPickColorDlg, m_pCanvasCursor, 0x270);
	CHECK_FIELD(FrUccPickColorDlg, m_pBrightnessCanvas, 0x274);
	CHECK_FIELD(FrUccPickColorDlg, m_pBrightnessCursor1, 0x278);
	CHECK_FIELD(FrUccPickColorDlg, m_pBrightnessCursor2, 0x27c);
	CHECK_FIELD(FrUccPickColorDlg, m_pNewColor, 0x280);
	CHECK_FIELD(FrUccPickColorDlg, m_pPrevColor, 0x284);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_UCCPICKCOLORDLG_H
