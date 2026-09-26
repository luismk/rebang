#pragma once
#include "baseobject.h"
#include "singleton.h"
#include "../../client/Wangreal/include/wtypes.h"

class WFont;
class WOverlay;

class CChatMsg : public BaseObject, public WSingleton<CChatMsg>
{
public:
	void SetChatText(const char* text, bool b);
	int GetConsolKeyCode() const { return m_iConsolCode; }

protected:
	bool m_bPrinted;
	bool m_bCaretMove;
	int m_nMaxBytes;
	float m_fMaxWidth;
	WFont* m_pMaskedFont;
	WOverlay* m_pChatWindow[3];
	WRect m_rcChatWinSrc[3];
	WRect m_rcChatWinDst[3];
	float m_fVel;
	bool m_bActive;
	bool m_bInsert;
	bool m_bCompInProgress;
	bool m_bShowCaret;
	float m_fCaretTime;
	int m_iState;
	int m_iCompCode;
	int m_iPrevCode;
	int m_iConsolCode;
};
