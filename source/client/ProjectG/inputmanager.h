// Original: d:\build\custom\temp\code\client\projectg\inputmanager.h
#ifndef PG_CLIENT_PROJECTG_INPUTMANAGER_H
#define PG_CLIENT_PROJECTG_INPUTMANAGER_H

#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "wlist.h"
#include "wmath.h"

// The mouse buttons (`W4eButton@@`: a global enum). LEFT and RIGHT trade
// places when the buttons are swapped.
enum eButton {
	LEFT_BUTTON,                   // 0
	RIGHT_BUTTON,                  // 1
	MIDDLE_BUTTON                  // 2
};

// The game's input (key binding) manager: named key bindings over the
// keyboard and mouse devices, plus the state of the three mouse buttons.
class CInputManager : public WSingleton<CInputManager> {
public:
	// Who may read the bindings (m_inputMode).
	enum eInputMode {
		INPUT_MODE_NORMAL,         // 0, every binding
		INPUT_MODE_CHAT            // 1, only the bindings flagged KEY_ALLOW_IN_CHAT
	};

	// KeyPair::flags.
	enum {
		KEY_ALLOW_IN_CHAT = 0x1,
		KEY_BLOCKED       = 0x2,
		KEY_EXCLUDED      = 0x4
	};

	// m_blockFlags.
	enum {
		BLOCK_MOUSE = 0x2
	};

	// A named binding: one or more key codes of a device.
	struct KeyPair {
		const char *name;          // 0x0
		std::vector<int> keys;     // 0x4
		int type;                  // 0x14, 3 records the event time
		unsigned long flags;       // 0x18, KEY_*
		int state;                 // 0x1C
		int prevState;             // 0x20
		unsigned long eventTime;   // 0x24
		bool fake;                 // 0x28, SetFakeInput
	};

	// One mouse button.
	struct sButton {
		int state;                 // 0x0
		WVector downPos;           // 0x4
		WVector upPos;             // 0x10
		unsigned long interval;    // 0x1C, time between the last two clicks
		unsigned long time;        // 0x20, time of the last click
		bool fake;                 // 0x24, SetFakeInput
		bool blocked;              // 0x25
	};

	// inputmanager.h:27
	CInputManager(void) : m_keyList(32, 128) {
		m_sensitivity = 1.0f;
		m_swapButton = false;
		Reset();
	}
	virtual ~CInputManager(void);

	void Reset(void);
	void SetMousePointToLoginBox(void);
	void SetMousePoint(float x, float y);
	void Update(void);
	unsigned long GetEventTime(const char *key);
	unsigned long GetEventTime(eButton button);
	bool GetDoubleClick(eButton button);
	int GetButton(eButton button);
	int Get(const char *key, bool consume);
	int GetList(void);
	int Get(int key);
	int GetDown(const char *key, bool consume);
	void ExclusiveGetDownUseDone(const char *key);
	int GetUp(const char *key);
	WVector GetMouseDelta(void);
	void Register(const char *key, int code, unsigned long flags, int type);
	void IncludeKey(const char *key);
	void ExcludeKey(const char *key);
	void BlockKey(const char *key);
	void FreeKey(const char *key);
	void BlockKey(void);
	void FreeKey(void);
	void BlockMouse(void);
	void FreeMouse(void);
	void EraseAllList(void);
	void SetFakeInput(eButton button);
	void SetFakeInput(const char *key, bool down);
	void SwapMouseButton(bool swap);

	// inputmanager.h:57
	WVector GetButtonDownPos(eButton button) { return m_button[button].downPos; }
	// inputmanager.h:66
	void SetInputMode(eInputMode mode) { m_inputMode = mode; }
	// inputmanager.h:69
	WVector GetMousePoint(void) const { return m_mousePoint; }
	// inputmanager.h:70
	void FixPointer(bool fix) { m_fixPointer = fix; }
	// inputmanager.h:86
	void SetSensitivity(float sensitivity) { m_sensitivity = sensitivity; }
	// inputmanager.h:87
	float GetSensitivity(void) const { return m_sensitivity; }
	// inputmanager.h:93
	void BlockMouse(eButton button) { m_button[button].blocked = true; }
	// inputmanager.h:94
	void FreeMouse(eButton button) { m_button[button].blocked = false; }
	// inputmanager.h:101
	void SetActive(bool active) { m_active = active; }

protected:
	WList<KeyPair *> m_keyList;    // 0x28
	WVector m_mouseDelta;          // 0x48
	WVector m_mousePoint;          // 0x54
	bool m_fixPointer;             // 0x60
	sButton m_button[3];           // 0x64, by eButton
	unsigned long m_blockFlags;    // 0xDC, BLOCK_*
	eInputMode m_inputMode;        // 0xE0
	float m_sensitivity;           // 0xE4
	bool m_active;                 // 0xE8
	bool m_swapButton;             // 0xE9

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(CInputManager::KeyPair, 0x2C);
CHECK_SIZE(CInputManager::sButton, 0x28);
CHECK_SIZE(CInputManager, 0xEC);
BEGIN_LAYOUT_CHECK(CInputManager)
	CHECK_FIELD(CInputManager::KeyPair, keys, 0x4);
	CHECK_FIELD(CInputManager::KeyPair, type, 0x14);
	CHECK_FIELD(CInputManager::KeyPair, flags, 0x18);
	CHECK_FIELD(CInputManager::KeyPair, state, 0x1C);
	CHECK_FIELD(CInputManager::KeyPair, eventTime, 0x24);
	CHECK_FIELD(CInputManager::KeyPair, fake, 0x28);
	CHECK_FIELD(CInputManager::sButton, upPos, 0x10);
	CHECK_FIELD(CInputManager::sButton, interval, 0x1C);
	CHECK_FIELD(CInputManager::sButton, fake, 0x24);
	CHECK_FIELD(CInputManager::sButton, blocked, 0x25);
	CHECK_FIELD(CInputManager, m_keyList, 0x28);
	CHECK_FIELD(CInputManager, m_mouseDelta, 0x48);
	CHECK_FIELD(CInputManager, m_mousePoint, 0x54);
	CHECK_FIELD(CInputManager, m_fixPointer, 0x60);
	CHECK_FIELD(CInputManager, m_button, 0x64);
	CHECK_FIELD(CInputManager, m_blockFlags, 0xDC);
	CHECK_FIELD(CInputManager, m_inputMode, 0xE0);
	CHECK_FIELD(CInputManager, m_sensitivity, 0xE4);
	CHECK_FIELD(CInputManager, m_active, 0xE8);
	CHECK_FIELD(CInputManager, m_swapButton, 0xE9);
END_LAYOUT_CHECK

extern CInputManager *g_input;

#endif // PG_CLIENT_PROJECTG_INPUTMANAGER_H
