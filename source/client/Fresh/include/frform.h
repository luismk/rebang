// Original: d:\build\custom\temp\code\client\fresh\include\frform.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRFORM_H
#define PG_CLIENT_FRESH_INCLUDE_FRFORM_H

#include "..\..\defines.h"
#include "frelement.h"
#include "frwnd.h"

class FrEdit;
class FrFrame;
class FrGuiItem;
class FrStatic;
class FrWndManager;

// Result a form closes with (PDB enum).
enum eFormRet {
	FrNONE = 0,
	FrOK = 1,
	FrCANCEL = 2,
	FrYES = 3,
	FrNO = 4
};

class FrForm;

// Creates the form `name` of the window manager's layout as a T owned by
// pOwner (frform.h:23-32).
template <class T>
T* CreateForm(FrWndManager* pManager, FrCmdTarget* pOwner, const char* name, FrWnd* pParent)
{
	T* pForm = new T;
	FrForm* pRet = pForm->_Init(pManager, pOwner, name, pParent);
	if (pRet) {
		return DYNAMIC_CAST(T, pRet);
	}
	delete pForm;
	return 0;
}

// A dialog window. Layout, access and vtable from the PDB (size 0x110, base
// FrWnd). The new virtuals are declared in slot order (23-36); cl places
// overloaded virtuals in reverse declaration order, hence Open(cb, flag)
// before Open(cb, pt, flag). The bodies written here are the inline ones of
// frform.h whose COMDATs the line marks show.
class FrForm : public FrWnd {
	LAYOUT_CHECK_FRIEND;
	// FrWndManager::ProcessHotKey calls OnOK / OnCancel.
	friend class FrWndManager;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frform.h:65

	FrForm(void);
	virtual ~FrForm(void);

	void Init(FrGuiItem& item, FrWndManager* pWndManager, FrWnd* pParent);
	FrForm* _Init(FrWndManager* pWndManager, FrCmdTarget* pCmdDest, const char* name, FrWnd* pParent);

	// vtable slots 23-31
	virtual bool Open(FRESH_RESULT_CALLBACK pfnResult, unsigned long flag);
	virtual bool Open(FRESH_RESULT_CALLBACK pfnResult, const WPoint& pt, unsigned long flag);
	virtual bool Close(eFormRet ret, bool bDestroy);
	virtual bool Close(bool bDestroy);   // FrWnd's slot 2
	virtual void SetCaption(const char* caption);
	virtual void SetDesc(const char* desc);
	virtual void SetMessage(const char* message, bool b);
	virtual void EnableDrag(bool bEnable);
	virtual void SetTimeLimit(float timeLimit) {                     // frform.h:85
		m_timeLimit = timeLimit;
		m_dt = 0;
	}
	virtual bool OnInit(void);

	void SetResultCallback(FRESH_RESULT_CALLBACK pfnResult);
	void DescHidePrivacy(void);
	bool GetFlag(unsigned long flag) { return (m_flag & flag) ? true : false; }   // frform.h:87
	void SetFlag(unsigned long flag) { m_flag = flag; }                 // frform.h:88
	void EnableFlag(unsigned long flag) { m_flag |= flag; }             // frform.h:89
	void DisableFlag(unsigned long flag) { m_flag &= ~flag; }           // frform.h:90
	void SetCloseResult(eFormRet ret) { m_defaultRetCode = ret; }       // frform.h:91
	void Adjust(WRect& rect);
	void SetFrameCaptionFocus(bool bFocus);
	void SetCaptionOffset(bool b);
	void SetMessageControlByType(enumGuiType type, FrWnd* pChild);

	void OnMouseMove(const WPoint& pt);
	bool OnLButtonDown(const WPoint& pt);
	bool OnLButtonUp(const WPoint& pt);
	void OnSetCursor(bool b, const WPoint& pt);
	void SetIconRect(const WRect& rect);

	static void EnableTail(bool bEnable) { ms_bHasTail = bEnable; }    // frform.h:159

	static bool ms_bHasTail;
	static bool ms_bExclusive;

protected:
	// vtable slots 32-33
	virtual void OnOK(void);
	virtual void OnCancel(void);

	void OnDraw(void);
	void OnProc(const float dt);

	void OnFreshOkay(void);
	void OnFreshCancel(void);
	void OnFreshYes(void);
	void OnFreshNo(void);

public:
	// vtable slots 34-35
	virtual void SetRect(const WRect& rect);
	virtual void SetClientRect(const WRect& rect);

private:
	// vtable slot 36
	virtual bool IsResizeBtnArea(const WPoint& pt);

	bool IsTitleBarArea(const WPoint& pt);

	static WPoint ms_oldPos;
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	bool m_canDrag;                          // 0xd4
	bool m_bCaptionDown;                     // 0xd5
	bool m_bResizeBtnDown;                   // 0xd6
	unsigned long m_flag;                    // 0xd8
	eFormRet m_retCode;                      // 0xdc
	FRESH_RESULT_CALLBACK m_pFnResult;       // 0xe0 (16-byte pointer to member)
	FrCmdTarget* m_pCmdDest;                 // 0xf0
	FrFrame* m_pBaseFrm;                     // 0xf4
	FrStatic* m_pMsgStatic;                  // 0xf8
	FrEdit* m_pMsgEdit;                      // 0xfc
	float m_timeLimit;                       // 0x100
	float m_dt;                              // 0x104
	eFormRet m_defaultRetCode;               // 0x108
};

CHECK_SIZE(FrForm, 0x110);
BEGIN_LAYOUT_CHECK(FrForm)
	CHECK_FIELD(FrForm, m_canDrag, 0xd4);
	CHECK_FIELD(FrForm, m_bCaptionDown, 0xd5);
	CHECK_FIELD(FrForm, m_bResizeBtnDown, 0xd6);
	CHECK_FIELD(FrForm, m_flag, 0xd8);
	CHECK_FIELD(FrForm, m_retCode, 0xdc);
	CHECK_FIELD(FrForm, m_pFnResult, 0xe0);
	CHECK_FIELD(FrForm, m_pCmdDest, 0xf0);
	CHECK_FIELD(FrForm, m_pBaseFrm, 0xf4);
	CHECK_FIELD(FrForm, m_pMsgStatic, 0xf8);
	CHECK_FIELD(FrForm, m_pMsgEdit, 0xfc);
	CHECK_FIELD(FrForm, m_timeLimit, 0x100);
	CHECK_FIELD(FrForm, m_dt, 0x104);
	CHECK_FIELD(FrForm, m_defaultRetCode, 0x108);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRFORM_H
