#pragma once
#include "baseobject.h"
#include "singleton.h"
#include "../../client/Wangreal/include/wtypes.h"

class WFont;
class WOverlay;

class CChatMsg : public WSingleton<CChatMsg>
{
public:
	void Reset();
	void SetActive(bool active, bool b2, bool b3);
	void SetChatText(const char* text, bool b);
	void Process(float deltaTime);
	void ProcessMacro(const float deltaTime);
	const char* MakeShortID(const char* id, float width);
	bool IsActive() const { return m_bActive; }
	void GetChatText(std::string& front, std::string& comp, std::string& end);
	int GetCurCaretPos() { return m_iCurByte; }
	void ResetCurCaretPos()
	{
		m_iCurByte = 0;
		m_iCurChar = 0;
	}
	void CurCaretMoveRight() { OnMoveRight(); }
	int GetNumBytes() { return m_iNumBytes; }
	bool IsOpened() { return m_iState == 3; }
	bool IsClosed() { return m_iState == 0; }
	bool IsOpenning() { return m_iState == 2 || m_iState == 1; }
	void EnableCaretMove(bool enable) { m_bCaretMove = enable; }
	void SetBufLen(int len) { m_nMaxBytes = len; }
	void SetBufWidth(float width) { m_fMaxWidth = width; }
	int GetConsolKeyCode() const { return m_iConsolCode; }
	WFont* GetMaskedFont() const { return m_pMaskedFont; }

protected:
	void OnMoveRight();
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
	char m_szChat[1024];
	char m_szComp[1024];
	char m_szExt[1024];
	bool m_abWide[1024];
	int m_iNumBytes;
	int m_iCurByte;
	int m_iNumChars;
	int m_iCurChar;
};
