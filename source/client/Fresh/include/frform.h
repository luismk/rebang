#pragma once
#include "rtti.h"
#include "frcmdtarget.h"
#include "frelement.h"
#include "frwnd.h"
#include "frframe.h"
#include "../../Wangreal/include/wtypes.h"

class FrEdit;
class FrFrame;
class FrGuiItem;
class FrStatic;
class FrWndManager;

enum eFormRet
{
	FrNONE,
	FrOK,
	FrCANCEL,
	FrYES,
	FrNO
};

enum eFormFlag
{
	FFL_ESCAPE = 0x1,
	FFL_ENTER = 0x2,
	FFL_MODELESS = 0x4
};

class FrForm : public FrWnd
{
	friend class FrWndManager;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI() const { return &m_RTTI; }

	FrForm();
	virtual ~FrForm();
	void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);
	FrForm* _Init(FrWndManager* pManager, FrCmdTarget* pOwner,
		const char* layout, FrWnd* pParent);
	virtual bool Open(FRESH_PFN_RESULT pFnResult, const WPoint& pos,
		unsigned long style);
	virtual bool Open(FRESH_PFN_RESULT pFnResult, unsigned long style);
	virtual bool Close(eFormRet ret, bool immediate);
	virtual bool Close(bool immediate);
	void SetResultCallback(FRESH_PFN_RESULT pFnResult);
	virtual void SetCaption(const char* caption);
	virtual void SetDesc(const char* desc);
	virtual void SetMessage(const char* message, bool b);
	virtual void EnableDrag(bool enable);
	virtual void SetTimeLimit(float limit) { m_timeLimit = limit; }
	void DescHidePrivacy() { m_pBaseFrm->HidePrivacy(true); }
	bool GetFlag(unsigned long flag) { return (m_flag & flag) ? true : false; }
	void SetFlag(unsigned long flag) { m_flag = flag; }
	void EnableFlag(unsigned long flag) { m_flag |= flag; }
	void DisableFlag(unsigned long flag) { m_flag &= ~flag; }
	void SetCloseResult(eFormRet ret) { m_retCode = ret; }
	void Adjust(WRect& rect);
	void SetFrameCaptionFocus(bool focus);
	void SetCaptionOffset(bool offset) { m_pBaseFrm->SetCaptionOffset(offset); }
	void SetMessageControlByType(enumGuiType type, FrWnd* pWnd);
	virtual bool OnInit();

protected:
	virtual void OnDraw();
	virtual void OnProc(const float deltaTime);
	virtual void OnOK();
	virtual void OnCancel();

public:
	virtual void OnMouseMove(const WPoint& point);
	virtual bool OnLButtonDown(const WPoint& point);
	virtual bool OnLButtonUp(const WPoint& point);
	virtual void OnSetCursor(bool bInClient, const WPoint& point);
	virtual void SetRect(const WRect& rect);
	virtual void SetIconRect(const WRect& rect);
	virtual void SetClientRect(const WRect& rect);

protected:
	bool m_canDrag;
	bool m_bCaptionDown;
	bool m_bResizeBtnDown;
	unsigned long m_flag;
	eFormRet m_retCode;
	FRESH_PFN_RESULT m_pFnResult;
	FrCmdTarget* m_pCmdDest;
	FrFrame* m_pBaseFrm;
	FrStatic* m_pMsgStatic;
	FrEdit* m_pMsgEdit;
	float m_timeLimit;
	float m_dt;
	eFormRet m_defaultRetCode;

private:
	bool IsTitleBarArea(const WPoint& point);
	virtual bool IsResizeBtnArea(const WPoint& point);
	static WPoint ms_oldPos;

protected:
	void OnFreshOkay();
	void OnFreshCancel();
	void OnFreshYes();
	void OnFreshNo();

	DECLARE_FRESH_MSGMAP()

public:
	static bool ms_bHasTail;
	static bool ms_bExclusive;
	static void EnableTail(bool enable) { ms_bHasTail = enable; }
};
