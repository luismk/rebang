// Original: d:\build\custom\temp\code\client\projectg\uccdrawdlg.h
#ifndef PG_CLIENT_PROJECTG_UCCDRAWDLG_H
#define PG_CLIENT_PROJECTG_UCCDRAWDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "ToolTipManager.h"
#include "bitmap.h"
#include "FadingText.h"
#include "TabManager.h"
#include "OverlayCursor.h"

class CUccBase;
class FrArea;
class FrButton;
class FrListBox;
class FrUccPickColorDlg;
class FrUccSetInfoDlg;
class FrWnd;
struct sItemInfo;
enum eDrawMode;

// Tab of the drawing dialog (ChangeTab's argument; OnTabDrawBtnUp opens 0,
// OnTabPatternBtnUp 1).
enum eUccTab {
	UCC_TAB_DRAW = 0,
	UCC_TAB_PATTERN = 1
};

// The UCC (user-created content) drawing dialog. Size 0x898 (the `new` of
// FrUccDrawDlgMakeInstance). Only the members the transcribed code uses are
// declared.
class FrUccDrawDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
	friend class FrUccSetInfoDlg;  // reads m_pItemInfo and m_pUccBase
	friend class FrUccPickColorDlg;  // lists and fills m_palette
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const;

	FrUccDrawDlg(void);
	virtual ~FrUccDrawDlg(void);

	virtual bool Open(FRESH_RESULT_CALLBACK pfnResult, const WPoint& pt, unsigned long flag);
	virtual bool Close(eFormRet ret, bool bDestroy);

	bool IsInitialized(void);           // uccdrawdlg.h:72
	bool IsSaved(void);                 // uccdrawdlg.h:86

	void SetName(const char* name);
	void Notify(const char* text);
	void SetCurrentColor(unsigned char r, unsigned char g, unsigned char b);
	void SetSaved(bool bSaved);
	void MoveCanvasToCenter(void);
	void AfterZoom(void);
	void SetUccDrawInfo(eDrawMode mode, sItemInfo* pItemInfo, bool b);
	void SetToolbarStatus(FrButton* pButton);
	void CloseWaitingForm(void);
	void Init(void);
	void Finish(void);
	void Clear(void);
	void ChangeTab(eUccTab tab);
	void BuildCurrentPattern(void);
	void BuildColorList(void);
	void BuildBrushList(void);
	void BuildPenList(void);
	void BuildEraserList(void);
	void BuildPatternList(int first, bool b);

private:
	static sFRESH_ENTRY _MsgEntries[];
protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnCancel(void);

	void OnUccCanvasInit(int param);
	void OnUndoBtnInit(int param);
	void OnRedoBtnInit(int param);
	void OnFlipBtnInit(int param);
	void OnFlipBtnOwnerDraw(int param);
	void OnFlipBtnUp(void);
	void OnResetBtnInit(int param);
	void OnGridBtnInit(int param);
	void OnDragBtnInit(int param);
	void OnZoomInBtnInit(int param);
	void OnZoomOutBtnInit(int param);
	void OnSide1AreaInit(int param);
	void OnSide2AreaInit(int param);
	void OnTempSaveBtnInit(int param);
	void OnTabDrawBtnInit(int param);
	void OnTabPatternBtnInit(int param);
	void OnTabPatternBtnUp(void);
	void OnTabEmoticonBtnInit(int param);
	void OnTabTextBtnInit(int param);
	void OnFinishBtnInit(int param);
	void OnCloseBtnInit(intptr_t param);
	void OnPaintBtnInit(int param);
	void OnPickColorBtnInit(int param);
	void OnCurrentColorBtnInit(int param);
	void OnCurrentColorBtnUp(void);
	void OnCurrentColorAreaInit(int param);
	void OnColorListInit(intptr_t param);
	void OnColorListOwnerDraw(intptr_t param);
	void OnColorListBtnDown(void);
	void OnEraserListInit(int param);
	void OnEraserListOwnerDraw(int param);
	void OnEraserListBtnDown(void);
	void OnToolInfoEraserAreaInit(int param);
	void OnBaseColorListInit(int param);
	void OnExtendedBasePatternListInit(int param);
	void OnBrushListInit(int param);
	void OnBrushListOwnerDraw(int param);
	void OnPenListInit(int param);
	void OnPenListOwnerDraw(int param);
	void OnToolInfoPenAreaInit(int param);
	void OnToolInfoBrushAreaInit(int param);
	void OnColorListsAreaInit(int param);
	void OnArrowDownBtnInit(int param);
	void OnArrowRightBtnInit(int param);
	void OnArrowLeftBtnInit(int param);
	void OnArrowLeftBtnUp(void);
	void OnDeleteBtnInit(int param);
	void OnModifyBtnInit(int param);
	void OnMakeBtnInit(int param);
	void OnRateListInit(int param);
	void OnRateListOwnerDraw(int param);
	void OnPatternListInit(int param);
	void OnPatternListOwnerDraw(int param);
	void OnEtcInit(int param);
	void OnToolTipInit(intptr_t param);
	void OnUndoBtnUp(void);
	void OnRedoBtnUp(void);
	void OnGridBtnUp(void);
	void OnDragBtnUp(void);
	void OnZoomInBtnUp(void);
	void OnZoomOutBtnUp(void);
	void OnTabDrawBtnUp(void);
	void OnTabEmoticonBtnUp(void);
	void OnTabTextBtnUp(void);
	void OnFinishBtnUp(void);
	bool OnAskCompleteDlgResult(int ret, FrForm* pForm);
	bool OnAskResetDlgResult(int ret, FrForm* pForm);
	bool OnAskCancelDlgResult(int ret, FrForm* pForm);
	void OnResetBtnUp(void);
	void OnBrushListBtnDown(void);
	void OnPenListBtnDown(void);
	void OnPatternListBtnDown(void);
	void OnArrowDownBtnUp(void);
	void OnArrowRightBtnUp(void);
	void OnCloseBtnUp(void);
	void OnPaintBtnUp(void);
	void OnPickColorBtnUp(void);
	bool OnUccPickColorDlgResult(int ret, FrForm* pForm);
	void OnDeleteBtnUp(void);
	void OnModifyBtnUp(void);
	void OnMakeBtnUp(void);
	void OnToolTipOnwerDraw(int param);
	bool IsSavingTemporarily(void);
	void Reset(void);
	void OnTempSaveBtnUp(void);
	bool OnUccSetInfoDlgResult(int ret, FrForm* pForm);
	void OnRateListBtnDown(void);

	sItemInfo* m_pItemInfo;            // 0x110
	CUccBase* m_pUccBase[2];           // 0x114, one canvas per side
	bool m_bSavingTemporarily;         // 0x11c
	FrForm* m_pWaitingForm;            // 0x120
	unsigned long m_waitingFormParam;  // 0x124, cleared with m_pWaitingForm
	Bitmap m_palette[18];              // 0x128, one pixel per colour; the colour list items point at them
	CToolTipManager m_toolTipManager;  // 0x290
	CTabManager m_tabManager;          // 0x2a0
	bool m_bSaved;                     // 0x2c4
	bool m_bCloseAfterSave;            // 0x2c5, set when the cancel dialog asks to save first
	PADDING(0x2c6, 0x2c8);             // FrUccDrawDlg members
	FrWnd* m_pCanvasWnd;               // 0x2c8, the window the canvas is drawn in
	FrButton* m_pUndoBtn;              // 0x2cc
	FrButton* m_pRedoBtn;              // 0x2d0
	FrButton* m_pFlipBtn;              // 0x2d4
	FrButton* m_pResetBtn;             // 0x2d8
	FrArea* m_pSide1Area;              // 0x2dc
	FrArea* m_pSide2Area;              // 0x2e0
	FrButton* m_pTempSaveBtn;          // 0x2e4
	FrButton* m_pGridBtn;              // 0x2e8
	FrButton* m_pDragBtn;              // 0x2ec
	FrButton* m_pZoomInBtn;            // 0x2f0
	FrButton* m_pZoomOutBtn;           // 0x2f4
	FrButton* m_pDrawBtn;              // 0x2f8, the draw tab button; shown for the draw, paint and pick colour tools
	FrButton* m_pTabPatternBtn;        // 0x2fc
	FrButton* m_pTabEmoticonBtn;       // 0x300, disabled
	FrButton* m_pTabTextBtn;           // 0x304, disabled
	FrButton* m_pFinishBtn;            // 0x308
	FrButton* m_pCloseBtn;             // 0x30c
	FrButton* m_pPickColorBtn;         // 0x310, disabled on the pattern tab
	FrButton* m_pCurrentColorBtn;      // 0x314, disabled on the pattern tab
	FrButton* m_pPaintBtn;             // 0x318
	FrArea* m_pCurrentColorArea;       // 0x31c, shows m_currentColor
	FrListBox* m_pColorList;           // 0x320, lists m_palette
	FrListBox* m_pEraserList;          // 0x324, brushes of type 2
	FrArea* m_pToolInfoEraserArea;     // 0x328
	Bitmap m_currentColor;             // 0x32c, one pixel of the current colour
	FrListBox* m_pBrushList;           // 0x340, brushes of type 0
	FrListBox* m_pPenList;             // 0x344, brushes of type 1
	FrArea* m_pToolInfoPenArea;        // 0x348
	FrArea* m_pToolInfoBrushArea;      // 0x34c
	FrArea* m_pColorListsArea;         // 0x350
	FrButton* m_pArrowDownBtn;         // 0x354
	FrButton* m_pPatternRightBtn;      // 0x358, enabled while patterns remain right of the list
	FrButton* m_pPatternLeftBtn;       // 0x35c, enabled while the list does not start at pattern 0
	FrButton* m_pDeleteBtn;            // 0x360, disabled
	FrButton* m_pModifyBtn;            // 0x364, disabled
	FrButton* m_pMakeBtn;              // 0x368, disabled
	FrListBox* m_pRateList;            // 0x36c, its item idx picks the pattern rate
	FrListBox* m_pPatternList;         // 0x370, items hold the pattern index
	FrArea* m_pBaseColorListArea;      // 0x374
	FrArea* m_pExtendedBasePatternListArea; // 0x378
	FrArea* m_pEtcArea;                // 0x37c
	FrArea* m_pToolTipArea;            // 0x380
	PADDING(0x384, 0x388);             // FrUccDrawDlg members
	FrUccSetInfoDlg* m_pUccSetInfoDlg; // 0x388
	FrUccPickColorDlg* m_pUccPickColorDlg; // 0x38c
	FrForm* m_pAskCancelDlg;           // 0x390
	FrForm* m_pAskResetDlg;            // 0x394
	FrForm* m_pAskCompleteDlg;         // 0x398
	const Bitmap* m_pToolBox;          // 0x39c, "ucc_tool_box", the frame of each pattern list item
	const Bitmap* m_pSelectBox;        // 0x3a0, "ucc_select_box"
	const Bitmap* m_pSelectSBox2;      // 0x3a4, "ucc_select_sbox2"
	const Bitmap* m_pSideTextBg[2][2]; // 0x3a8, [side area][front, back]: "ucc_bg_text_front1" ...
	const Bitmap* m_pToolChangeIcon;   // 0x3b8, drawn over the flip button
	const Bitmap* m_pSelectBoxYellow;  // 0x3bc, "ucc_select_box_yellow"
	const Bitmap* m_pSelectBoxPink;    // 0x3c0, "ucc_select_box_pink"
	const Bitmap* m_pSelectColor;      // 0x3c4, "ucc_select_color", frames the selected colour
	PADDING(0x3c8, 0x3cc);             // FrUccDrawDlg members
	int m_eraserIndex;                 // 0x3cc, eraser the list selects while it has no selection
	int m_brushType;                   // 0x3d0, eBrushType of the current tool (3: pattern)
	int m_curSide;                     // 0x3d4, index into m_pUccBase
	eDrawMode m_drawMode;              // 0x3d8
	char m_name[64];                   // 0x3dc, length not confirmed
	WRect m_canvasRect;                // 0x41c, m_pCanvasWnd's rect relative to the dialog
	Bitmap m_patternThumbnails[24];    // 0x42c, 18x18 thumbnails of the pattern brushes (Init)
	eUccTab m_curTab;                  // 0x60c, the tab ChangeTab opened last
	Bitmap m_blankPattern;             // 0x610, 88x88 white, the brush added for a missing pattern (Init)
	Bitmap m_patternHighlight;         // 0x624, 18x18 light grey, behind the selected pattern
	int m_patternIndex;                // 0x638, brush of type 3 shown by BuildCurrentPattern
	int m_patternRate;                 // 0x63c, 2 * (rate list item idx + 1)
	bool m_bPatternListExpanded;       // 0x640, toggled by OnArrowDownBtnUp
	bool m_bShowGrid;                  // 0x641
	PADDING(0x642, 0x644);             // FrUccDrawDlg members
	Bitmap m_spareBitmap;              // 0x644, constructed and destroyed only; no use recovered
	bool m_initialized;                // 0x658
	PADDING(0x659, 0x65c);             // FrUccDrawDlg members
	COverlayCursor m_overlayCursor;    // 0x65c, its cursor type follows the toolbar's tool
	CFadingText m_fadingText;          // 0x670
	PADDING(0x894, 0x898);             // FrUccDrawDlg members
};

CHECK_SIZE(FrUccDrawDlg, 0x898);
BEGIN_LAYOUT_CHECK(FrUccDrawDlg)
	CHECK_FIELD(FrUccDrawDlg, m_pItemInfo, 0x110);
	CHECK_FIELD(FrUccDrawDlg, m_pUccBase, 0x114);
	CHECK_FIELD(FrUccDrawDlg, m_bSavingTemporarily, 0x11c);
	CHECK_FIELD(FrUccDrawDlg, m_pWaitingForm, 0x120);
	CHECK_FIELD(FrUccDrawDlg, m_waitingFormParam, 0x124);
	CHECK_FIELD(FrUccDrawDlg, m_palette, 0x128);
	CHECK_FIELD(FrUccDrawDlg, m_toolTipManager, 0x290);
	CHECK_FIELD(FrUccDrawDlg, m_tabManager, 0x2a0);
	CHECK_FIELD(FrUccDrawDlg, m_pUndoBtn, 0x2cc);
	CHECK_FIELD(FrUccDrawDlg, m_pRedoBtn, 0x2d0);
	CHECK_FIELD(FrUccDrawDlg, m_pFlipBtn, 0x2d4);
	CHECK_FIELD(FrUccDrawDlg, m_pResetBtn, 0x2d8);
	CHECK_FIELD(FrUccDrawDlg, m_pSide1Area, 0x2dc);
	CHECK_FIELD(FrUccDrawDlg, m_pSide2Area, 0x2e0);
	CHECK_FIELD(FrUccDrawDlg, m_pTempSaveBtn, 0x2e4);
	CHECK_FIELD(FrUccDrawDlg, m_pGridBtn, 0x2e8);
	CHECK_FIELD(FrUccDrawDlg, m_pTabPatternBtn, 0x2fc);
	CHECK_FIELD(FrUccDrawDlg, m_pTabEmoticonBtn, 0x300);
	CHECK_FIELD(FrUccDrawDlg, m_pTabTextBtn, 0x304);
	CHECK_FIELD(FrUccDrawDlg, m_pFinishBtn, 0x308);
	CHECK_FIELD(FrUccDrawDlg, m_pCloseBtn, 0x30c);
	CHECK_FIELD(FrUccDrawDlg, m_pPickColorBtn, 0x310);
	CHECK_FIELD(FrUccDrawDlg, m_pCurrentColorBtn, 0x314);
	CHECK_FIELD(FrUccDrawDlg, m_pPaintBtn, 0x318);
	CHECK_FIELD(FrUccDrawDlg, m_pCurrentColorArea, 0x31c);
	CHECK_FIELD(FrUccDrawDlg, m_pToolInfoEraserArea, 0x328);
	CHECK_FIELD(FrUccDrawDlg, m_pToolInfoPenArea, 0x348);
	CHECK_FIELD(FrUccDrawDlg, m_pToolInfoBrushArea, 0x34c);
	CHECK_FIELD(FrUccDrawDlg, m_pColorListsArea, 0x350);
	CHECK_FIELD(FrUccDrawDlg, m_pArrowDownBtn, 0x354);
	CHECK_FIELD(FrUccDrawDlg, m_pDeleteBtn, 0x360);
	CHECK_FIELD(FrUccDrawDlg, m_pModifyBtn, 0x364);
	CHECK_FIELD(FrUccDrawDlg, m_pMakeBtn, 0x368);
	CHECK_FIELD(FrUccDrawDlg, m_pBaseColorListArea, 0x374);
	CHECK_FIELD(FrUccDrawDlg, m_pExtendedBasePatternListArea, 0x378);
	CHECK_FIELD(FrUccDrawDlg, m_pEtcArea, 0x37c);
	CHECK_FIELD(FrUccDrawDlg, m_pToolTipArea, 0x380);
	CHECK_FIELD(FrUccDrawDlg, m_curTab, 0x60c);
	CHECK_FIELD(FrUccDrawDlg, m_pToolBox, 0x39c);
	CHECK_FIELD(FrUccDrawDlg, m_patternThumbnails, 0x42c);
	CHECK_FIELD(FrUccDrawDlg, m_blankPattern, 0x610);
	CHECK_FIELD(FrUccDrawDlg, m_patternHighlight, 0x624);
	CHECK_FIELD(FrUccDrawDlg, m_pSelectBox, 0x3a0);
	CHECK_FIELD(FrUccDrawDlg, m_pSelectSBox2, 0x3a4);
	CHECK_FIELD(FrUccDrawDlg, m_pSideTextBg, 0x3a8);
	CHECK_FIELD(FrUccDrawDlg, m_pToolChangeIcon, 0x3b8);
	CHECK_FIELD(FrUccDrawDlg, m_pSelectBoxYellow, 0x3bc);
	CHECK_FIELD(FrUccDrawDlg, m_pSelectBoxPink, 0x3c0);
	CHECK_FIELD(FrUccDrawDlg, m_pSelectColor, 0x3c4);
	CHECK_FIELD(FrUccDrawDlg, m_bSaved, 0x2c4);
	CHECK_FIELD(FrUccDrawDlg, m_bCloseAfterSave, 0x2c5);
	CHECK_FIELD(FrUccDrawDlg, m_pCanvasWnd, 0x2c8);
	CHECK_FIELD(FrUccDrawDlg, m_pDragBtn, 0x2ec);
	CHECK_FIELD(FrUccDrawDlg, m_pZoomInBtn, 0x2f0);
	CHECK_FIELD(FrUccDrawDlg, m_pZoomOutBtn, 0x2f4);
	CHECK_FIELD(FrUccDrawDlg, m_pDrawBtn, 0x2f8);
	CHECK_FIELD(FrUccDrawDlg, m_pColorList, 0x320);
	CHECK_FIELD(FrUccDrawDlg, m_pEraserList, 0x324);
	CHECK_FIELD(FrUccDrawDlg, m_currentColor, 0x32c);
	CHECK_FIELD(FrUccDrawDlg, m_pBrushList, 0x340);
	CHECK_FIELD(FrUccDrawDlg, m_pPenList, 0x344);
	CHECK_FIELD(FrUccDrawDlg, m_pPatternRightBtn, 0x358);
	CHECK_FIELD(FrUccDrawDlg, m_pPatternLeftBtn, 0x35c);
	CHECK_FIELD(FrUccDrawDlg, m_pRateList, 0x36c);
	CHECK_FIELD(FrUccDrawDlg, m_pPatternList, 0x370);
	CHECK_FIELD(FrUccDrawDlg, m_pAskCancelDlg, 0x390);
	CHECK_FIELD(FrUccDrawDlg, m_pUccSetInfoDlg, 0x388);
	CHECK_FIELD(FrUccDrawDlg, m_pUccPickColorDlg, 0x38c);
	CHECK_FIELD(FrUccDrawDlg, m_pAskResetDlg, 0x394);
	CHECK_FIELD(FrUccDrawDlg, m_pAskCompleteDlg, 0x398);
	CHECK_FIELD(FrUccDrawDlg, m_eraserIndex, 0x3cc);
	CHECK_FIELD(FrUccDrawDlg, m_brushType, 0x3d0);
	CHECK_FIELD(FrUccDrawDlg, m_curSide, 0x3d4);
	CHECK_FIELD(FrUccDrawDlg, m_drawMode, 0x3d8);
	CHECK_FIELD(FrUccDrawDlg, m_name, 0x3dc);
	CHECK_FIELD(FrUccDrawDlg, m_canvasRect, 0x41c);
	CHECK_FIELD(FrUccDrawDlg, m_patternIndex, 0x638);
	CHECK_FIELD(FrUccDrawDlg, m_patternRate, 0x63c);
	CHECK_FIELD(FrUccDrawDlg, m_bPatternListExpanded, 0x640);
	CHECK_FIELD(FrUccDrawDlg, m_bShowGrid, 0x641);
	CHECK_FIELD(FrUccDrawDlg, m_initialized, 0x658);
	CHECK_FIELD(FrUccDrawDlg, m_spareBitmap, 0x644);
	CHECK_FIELD(FrUccDrawDlg, m_overlayCursor, 0x65c);
	CHECK_FIELD(FrUccDrawDlg, m_fadingText, 0x670);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_UCCDRAWDLG_H
