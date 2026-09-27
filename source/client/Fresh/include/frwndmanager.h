#pragma once
#include <list>
#include <string>
#include "../../Wangreal/include/wtypes.h"
#include "frelement.h"

class Bitmap;
class CChatMsg;
class FrCmdTarget;
class FrCursor;
class FrDesktop;
class FrEdit;
class FrEmoticon;
class FrGraphicInterface;
class FrScrollBar;
class FrWnd;
class WSize;

struct FrInputState
{
	unsigned long mouse;
	bool hoverChecked;
	WPoint mousePos;
	WPoint oldMousePos;
	float wheelDelta;
	FrWnd* keyFocused;
	FrScrollBar* wheelFocused;
	CChatMsg* im;
};

class FrWndManager
{
public:
	enum eFadeState
	{
		OPENING = 0x0,
		OPENED = 0x1,
	};

	FrWndManager(FrElementDoc* pDoc);
	virtual ~FrWndManager();

	bool Init(const char* wallPaper, bool exclusiveKey);
	void Display(bool drawDesktop);
	void Process(const float deltaTime);
	FrElementDoc* GetDocument() const;
	const Bitmap* GetBitmap(const char* resource, const char* id) const;
	void CreateToolTip();
	void DestroyToolTip();
	bool CreateLayout(const char* layout, FrCmdTarget* owner, bool firstTime);
	void CloseLayout();
	bool IsValidWindow(FrWnd* pWnd);
	bool AddTopmostWindow(FrWnd* pWnd);
	bool DeleteTopmostWindow(FrWnd* pWnd);
	FrWnd* GetModalForm();
	FrWnd* DoCreate(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent,
		FrCmdTarget* pOwner);
	__forceinline FrDesktop* GetDesktop() { return this->m_pDesktop; }
	unsigned int& RefIndex();
	FrGraphicInterface* GetGDI();
	const WPoint& GetMousePos() const;
	FrEmoticon* GetEmoticon() { return m_pEmoticon; }
	FrWnd* HandOverViewFocus();
	bool SetKeyFocus(FrWnd* pWnd, bool resetPrevImeData);
	FrWnd* GetKeyFocused();
	bool CanGetKeyFocus(FrWnd* pWnd);
	bool ResetKeyFocus(FrWnd* pWnd);
	bool MoveKeyFocusToNext(bool resetPrevImeData);
	FrScrollBar* GetWheelFocus();
	void SetWheelFocus(FrScrollBar* pScrBar);
	void SetCursor(int cursor);
	int GetCursor();
	void MoveCursor(FrWnd* pParent, const char* name);
	__forceinline const char* GetLayoutID() { return this->m_layoutID.c_str(); }
	bool SetCapture(FrWnd* pWnd);
	FrWnd* GetCapture();
	bool ReleaseCapture(FrWnd* pWnd);
	WPoint GetCreatePosition(const WSize& rectSize);
	void ConfineRect(WRect& dr);
	void RemoveFade();
	void CloseWindow(FrWnd* pWnd, bool bFade);
	void CloseForm(bool bFade);
	bool HasEscKeyWindow();
	bool HasValidWindow();
	void SetExclusiveKey(bool set);
	bool IsKeyExclusive();
	void ResetKey();
	void ResetTopFocus();
	float GetTextWidth(const char* text);
	float PrintText(const WPoint& pos, unsigned long align, const char* text,
		float limit, unsigned long emoDiffuse);
	float PrintText11(const WPoint& pos, unsigned long align, const char* text,
		float limit, unsigned long emoDiffuse);
	FrInputState* GetInputState();
	void IME_ShowCandWindow(FrEdit* pFocusedEdit, WPoint& caretPos);
	std::list<FrWnd*>& GetTopWndList();
	__forceinline void HidePrivacy(bool hide) { this->m_hidePrivacy = hide; }
	__forceinline bool HidePrivacy() { return this->m_hidePrivacy; }

protected:
	void CheckSystemStatus();
	void ProcessHotKey();
	void ProcessKey(FrWnd* pOldKeyFocused);

	FrElementDoc* m_pDoc;
	FrDesktop* m_pDesktop;
	FrCursor* m_pCursor;
	FrInputState m_istate;
	bool m_exclusiveKey;
	std::list<FrWnd*> m_topmostList;
	std::string m_layoutID;
	FrWnd* m_pCaptured;
	FrGraphicInterface* m_pDevice;
	unsigned int m_nRefIndex;
	int m_cursorIndex;
	FrEmoticon* m_pEmoticon;
	float m_fadeoutAlpha;
	eFadeState m_state;
	FrEdit* m_pFocusedEdit;
	WPoint m_caretPos;
	bool m_hidePrivacy;
};
