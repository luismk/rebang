#pragma once
#include <winput.h>
#include <wproc.h>
#include <imm.h>

class WIme : public WInputDev, public WProc
{
public:
	WIme();
	WIme(HWND hwnd);
	virtual ~WIme();
	virtual int GetState(int code, int n);
	virtual void Update(unsigned long timeStamp);
	virtual void Reset();
	virtual WInputDev* MakeClone(char* modeName, HWND hwnd);
	virtual char* GetDeviceName() { return "Ime"; }
	virtual WProc* ExternProc() { return this; }
	virtual int WinProc(unsigned int msg, unsigned long wparam,
		unsigned long lparam);
	char* GetTextComp();
	virtual void SetActive(bool stat)
	{
		m_bActive = stat;
		pos = 0;
		current = 0;
	}
	virtual void SetAlphaNumericMode(bool alnum);
	virtual bool IsAlphaNumericMode();

protected:
	void OnStartComposition(unsigned int dwCommand, long dwData);
	void OnSetContext(unsigned int dwCommand, long dwData);
	void OnEndComposition(unsigned int dwCommand, long dwData);
	void OnCompositionFull(unsigned int dwCommand, long dwData);
	int OnNotify(unsigned int dwCommand, long dwData);
	void OnControl(unsigned int dwCommand, long dwData);
	int GetResultString();
	int GetCompString(long flag);
	virtual void ProcessResultString(char* str);
	virtual void ProcessCompString(char* str, char* strattr);
	void OnInputLangChange(unsigned int dwCommand, long dwData);
	void OnComposition(unsigned int dwCommand, long dwData);
	void OnChar(unsigned int nChar);

private:
	int IME_Enter();
	void Leave();
	int Check();
	void ClearData();
	void PutString(char* str);
	void InitIme(HWND hwnd_);

	DWORD m_property;
	HKL m_hKeyLayout;
	int m_nState;
	int m_nCompLen;
	HWND m_hwndCand[32];
	CANDIDATELIST* m_candList[32];
	int m_charWidth;
	int m_charHeight;
	bool m_bActive;
	HIMC m_hIMC;
	HWND m_hWnd;
	char m_TextComp[3];
	DWORD m_dwConvMode;
	unsigned char* buff;
	int current;
	int old_current;
	unsigned char ringBuf[1024];
	int pos;
};
