#pragma once

enum eButton
{
	LEFT_BUTTON,
	RIGHT_BUTTON,
	MIDDLE_BUTTON
};

class CInputManager
{
public:
	enum eInputMode
	{
		INPUT_NORMAL,
		INPUT_EXCLUSIVE
	};

	int GetButton(eButton button);
	WVector GetButtonDownPos(eButton button)
	{
		return m_button[button].downPos;
	}
	WVector GetMouseDelta();
	void SetInputMode(eInputMode mode) { m_inputMode = mode; }
	WVector GetMousePoint() const { return m_mousePoint; }
	void FixPointer(bool fix) { m_fixPointer = fix; }
	void SetSensitivity(float sensitivity) { m_sensitivity = sensitivity; }
	float GetSensitivity() const { return m_sensitivity; }
	void BlockMouse(eButton button) { m_button[button].blocked = true; }
	void FreeMouse(eButton button) { m_button[button].blocked = false; }
	void SetActive(bool active) { m_active = active; }
	int Get(const char* action, bool exclusive);
	int GetDown(const char* action, bool exclusive);
	void ExclusiveGetDownUseDone(const char* action);

private:
	struct sButton
	{
		int state;
		WVector downPos;
		char unknown_10[0x15];
		bool blocked;
	};

	char unknown_00[0x54];
	WVector m_mousePoint;
	bool m_fixPointer;
	sButton m_button[3];
	char unknown_dc[4];
	eInputMode m_inputMode;
	float m_sensitivity;
	bool m_active;
};

extern CInputManager* g_input;

class WInputDev;
extern WInputDev* g_ime;
