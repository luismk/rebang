// Original: d:\build\custom\temp\code\client\projectg\console.h
#ifndef PG_CLIENT_PROJECTG_CONSOLE_H
#define PG_CLIENT_PROJECTG_CONSOLE_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "wlist.h"

class WFont;

// One key's state in the console's key table (Init allocates 0x59 of them
// and clears the second byte of each).
struct ConsoleKeyState {
	char m_code;                        // 0x0
	char m_pressed;                     // 0x1
};
CHECK_SIZE(ConsoleKeyState, 0x2);

// The in-game developer console. The layout up to 0x294 is reconstructed
// from the constructor, Init and the backspace stack; the full size is not
// known yet.
class CConsole : public WSingleton<CConsole> {
	LAYOUT_CHECK_FRIEND;
public:
	// console.h:20
	static void Print(const char *text) {
		if (m_pInstance) {
			m_pInstance->AddText(text);
		}
	}

	CConsole(void);
	virtual ~CConsole(void);

	void Init(void);
	void ShutDown(void);
	void Display(void);
	void Process(float elapsed);

protected:
	bool PushIntoBsStack(char c);
	char PopFromBsStack(void);
	void ClearBsStack(void);
	int BrowseCommand(const char *partial, char *result);
	void AddText(const char *text);
	void BadArgument(int command);
	void Command(const char *line);
	int TweakerCommandProc(const char *line);
	void InputProcess(float elapsed);

	float m_alpha;                      // 0x28, the console's opacity (0..1)
	bool m_active;                      // 0x2C
	PADDING(0x2D, 0x30);
	int m_state;                        // 0x30
	void *m_historyPos;                 // 0x34, the history cursor of the up/down keys
	int m_historyEdge;                  // 0x38, -1 past the oldest entry, 1 past the newest

	char m_bsStack[0x80];               // 0x3C
	char m_input[0x104];                // 0xBC
	ConsoleKeyState *m_keyStates;       // 0x1C0
	WFont *m_pFont;                     // 0x1C4
	WList<char *> m_history;            // 0x1C8
	WList<char *> m_lines;              // 0x1E8
	bool m_inputLocked;                 // 0x208
	PADDING(0x209, 0x20C);
	int m_lineOffset;                   // 0x20C
	char m_inputLine[0x80];             // 0x210, the line being typed
	float m_repeatDelay;                // 0x290
};
BEGIN_LAYOUT_CHECK(CConsole)
	CHECK_FIELD(CConsole, m_alpha, 0x28);
	CHECK_FIELD(CConsole, m_active, 0x2C);
	CHECK_FIELD(CConsole, m_state, 0x30);
	CHECK_FIELD(CConsole, m_historyPos, 0x34);
	CHECK_FIELD(CConsole, m_historyEdge, 0x38);
	CHECK_FIELD(CConsole, m_bsStack, 0x3C);
	CHECK_FIELD(CConsole, m_input, 0xBC);
	CHECK_FIELD(CConsole, m_keyStates, 0x1C0);
	CHECK_FIELD(CConsole, m_pFont, 0x1C4);
	CHECK_FIELD(CConsole, m_history, 0x1C8);
	CHECK_FIELD(CConsole, m_lines, 0x1E8);
	CHECK_FIELD(CConsole, m_inputLocked, 0x208);
	CHECK_FIELD(CConsole, m_lineOffset, 0x20C);
	CHECK_FIELD(CConsole, m_inputLine, 0x210);
	CHECK_FIELD(CConsole, m_repeatDelay, 0x290);
END_LAYOUT_CHECK

#endif
