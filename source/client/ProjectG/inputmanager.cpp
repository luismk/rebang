// Original: d:\build\custom\temp\code\client\projectg\inputmanager.cpp
#include <windows.h>
#include <mmsystem.h>

#include "InputManager.h"
#include "winput.h"
#include "wview.h"

// Wangreal's input devices (winput.h does not declare them yet).
extern WInputDev *g_mouse;
extern WInputDev *g_keyboard;

// The bindings GetList polls.
extern const char *aliasList[11];

// inputmanager.cpp:22
CInputManager::~CInputManager(void) {
	EraseAllList();
}

// inputmanager.cpp:27
void CInputManager::Reset(void) {



	KeyPair *pair = m_keyList.Start();
	while (pair) {
		pair->state = 0; pair->prevState = 0;
		pair->fake = false;
		pair = m_keyList.Next();
	}




	m_mouseDelta.x = m_mouseDelta.y = m_mouseDelta.z = 0.0f;

	m_mousePoint.x = g_view->GetWidth() * 0.625f;
	m_mousePoint.y = g_view->GetHeight() * 0.5f;
	m_mousePoint.z = 0.0f;
	m_fixPointer = false;

	for (int i = 0; i < 3; i++) {

		m_button[i].state = 0;
		m_button[i].downPos = WVector::ZERO;
		m_button[i].interval = 0;
		m_button[i].time = timeGetTime();
		m_button[i].fake = false;
		m_button[i].blocked = false;
	}

	m_blockFlags = 0;
	m_inputMode = INPUT_MODE_NORMAL;
	m_active = true;
}

// inputmanager.cpp:63
void CInputManager::SetMousePoint(float x, float y) {
	float width = 800.0f;
	float height = 600.0f;

	if (g_view) {

		width = g_view->GetWidth();
		height = g_view->GetHeight();
	}












	m_mousePoint.x = x < 2.0f ? 2.0f : (x > width - 2.0f ? width - 2.0f : x);
	m_mousePoint.y = y < 2.0f ? 2.0f : (y > height - 2.0f ? height - 2.0f : y);
}

// inputmanager.cpp:89
void CInputManager::SetMousePointToLoginBox(void) {
	SetMousePoint(500.0f, 300.0f);
}

// inputmanager.cpp:94
void CInputManager::Update(void) {
	if (!m_active)
		return;





	KeyPair *pair = m_keyList.Start();
	while (pair) {


		pair->prevState = pair->state & 1;


		pair->state = 0;
		for (std::vector<int>::iterator it = pair->keys.begin(); it != pair->keys.end(); ++it) {

			if (g_keyboard->GetState(pair->type, *it)) {

				pair->state = 1;
				break;
			}
		}

		if (pair->fake) {

			pair->fake = false;
			pair->state = 1;

		} else {

			if (pair->flags & (KEY_BLOCKED | KEY_EXCLUDED))
				pair->state = 0;
			if (pair->type == 3 && pair->state)
				pair->eventTime = g_keyboard->GetEventTime(pair->keys[0]);
		}


		pair = m_keyList.Next();
	}




	if (m_blockFlags & (BLOCK_MOUSE | 0x4)) {

		m_mouseDelta.x = 0.0f;
		m_mouseDelta.y = 0.0f;

	} else {

		m_mouseDelta.x = g_mouse->GetState(0, 1) * m_sensitivity;
		m_mouseDelta.y = g_mouse->GetState(0, 2) * m_sensitivity;
	}


	if (m_button[MIDDLE_BUTTON].blocked)
		m_mouseDelta.z = 0.0f;
	else
		m_mouseDelta.z = (float)g_mouse->GetState(0, 3);



	for (int i = 0; i < 3; i++) {

		int down = g_mouse->GetState(1, i);

		if (m_button[i].fake) {


			m_button[i].fake = false; m_button[i].state = (m_button[i].state & 1) * 2 + 1;

		} else {

			m_button[i].state = (m_button[i].state & 1) * 2 + (m_button[i].blocked ? 0 : (down != 0));
		}
	}

	if (!m_fixPointer) {

		float x = m_mousePoint.x + m_mouseDelta.x; m_mousePoint.x = x < 2.0f ? 2.0f : (x > g_view->GetWidth() - 2.0f ? g_view->GetWidth() - 2.0f : x);
		float y = m_mousePoint.y + m_mouseDelta.y; m_mousePoint.y = y < 2.0f ? 2.0f : (y > g_view->GetHeight() - 2.0f ? g_view->GetHeight() - 2.0f : y);
	}

	unsigned long now = timeGetTime();

	if (m_button[LEFT_BUTTON].state == 1) {

		m_button[LEFT_BUTTON].downPos = m_mousePoint;
		m_button[LEFT_BUTTON].interval = now - m_button[LEFT_BUTTON].time;
		m_button[LEFT_BUTTON].time = now;

	} else if (m_button[LEFT_BUTTON].state == 2) {
		m_button[LEFT_BUTTON].upPos = m_mousePoint;
	}

	if (m_button[RIGHT_BUTTON].state == 1) {

		m_button[RIGHT_BUTTON].downPos = m_mousePoint;
		m_button[RIGHT_BUTTON].interval = now - m_button[RIGHT_BUTTON].time;
		m_button[RIGHT_BUTTON].time = now;

	} else if (m_button[RIGHT_BUTTON].state == 2) {
		m_button[RIGHT_BUTTON].upPos = m_mousePoint;
	}

	if (m_button[MIDDLE_BUTTON].state == 1) {

		m_button[MIDDLE_BUTTON].downPos = m_mousePoint;
		m_button[MIDDLE_BUTTON].interval = now - m_button[MIDDLE_BUTTON].time;
		m_button[MIDDLE_BUTTON].time = now;

	} else if (m_button[MIDDLE_BUTTON].state == 2) {
		m_button[MIDDLE_BUTTON].upPos = m_mousePoint;
	}
}

// inputmanager.cpp:212
unsigned long CInputManager::GetEventTime(const char *key) {
	KeyPair *pair = m_keyList.Find(key);

	unsigned long time = pair ? pair->eventTime : 0;

	if (m_inputMode == INPUT_MODE_CHAT && !(pair->flags & KEY_ALLOW_IN_CHAT))
		time = 0;


	return time;
}

// inputmanager.cpp:224
unsigned long CInputManager::GetEventTime(eButton button) {
	return g_mouse->GetEventTime(button);
}

// inputmanager.cpp:231
bool CInputManager::GetDoubleClick(eButton button) {
	if (!m_active)
		return false;

	if (m_swapButton) {

		if (button == LEFT_BUTTON)
			button = RIGHT_BUTTON;
		else if (button == RIGHT_BUTTON)
			button = LEFT_BUTTON;
	}


	if (m_button[button].state == 1 && m_button[button].interval < 300)
		return true;

	return false;
}

// inputmanager.cpp:250
int CInputManager::GetButton(eButton button) {
	if (!m_active)
		return 0;

	if (m_swapButton) {

		if (button == LEFT_BUTTON)
			button = RIGHT_BUTTON;
		else if (button == RIGHT_BUTTON)
			button = LEFT_BUTTON;
	}


	return m_button[button].state;
}

// inputmanager.cpp:266
int CInputManager::Get(const char *key, bool consume) {
	if (!m_active)
		return 0;

	KeyPair *pair = m_keyList.Find(key);








	int state = pair ? pair->state : 0;

	if (consume && m_inputMode == INPUT_MODE_CHAT && state && !(pair->flags & KEY_ALLOW_IN_CHAT))
		state = 0;

	return state & 1;
}

// inputmanager.cpp:302
int CInputManager::GetList(void) {
	for (int i = 0; i < sizeof(aliasList) / sizeof(aliasList[0]); i++) {

		if (Get(aliasList[i], true))
			return 1;
	}


	return 0;
}

// inputmanager.cpp:313
int CInputManager::Get(int key) {
	if (!m_active)
		return 0;

	return g_keyboard->GetState(2, key);
}

// inputmanager.cpp:321
int CInputManager::GetDown(const char *key, bool consume) {
	if (!m_active)
		return 0;

	KeyPair *pair = m_keyList.Find(key);

	int down = pair ? (pair->state ^ pair->prevState) & pair->state : 0;

	if (consume && m_inputMode == INPUT_MODE_CHAT && !(pair->flags & KEY_ALLOW_IN_CHAT))
		return 0;


	return down;
}

// inputmanager.cpp:336
void CInputManager::ExclusiveGetDownUseDone(const char *key) {
	if (!m_active)
		return;

	KeyPair *pair = m_keyList.Find(key);

	if (pair && pair->state == 1)
		pair->state = 3;
}

// inputmanager.cpp:347
int CInputManager::GetUp(const char *key) {
	if (!m_active)
		return 0;

	KeyPair *pair = m_keyList.Find(key);

	int up = pair ? (pair->state ^ pair->prevState) & pair->prevState : 0;

	if (m_inputMode == INPUT_MODE_CHAT && !(pair->flags & KEY_ALLOW_IN_CHAT))
		return 0;


	return up;
}

// inputmanager.cpp:362
WVector CInputManager::GetMouseDelta(void) {
	return m_mouseDelta;
}

// inputmanager.cpp:367
void CInputManager::Register(const char *key, int code, unsigned long flags, int type) {

	KeyPair *pair = m_keyList.Find(key);

	if (pair == NULL) {


		pair = new KeyPair;
		pair->name = key;
		pair->keys.push_back(code);

		pair->prevState = 0;
		pair->state = 0;
		pair->flags = flags;
		pair->type = type;
		pair->eventTime = 0;
		pair->fake = false;
		m_keyList.AddItem(pair, key, false);
	} else if (pair->type != 3) {







		pair->keys.push_back(code);
	}
}

// inputmanager.cpp:400
void CInputManager::IncludeKey(const char *key) {
	if (key) {


		KeyPair *pair = m_keyList.Find(key);

		if (pair)
			pair->flags &= ~KEY_EXCLUDED;
	} else {


		KeyPair *pair = m_keyList.Start();
		while (pair) {

			pair->flags &= ~KEY_EXCLUDED;
			pair = m_keyList.Next();
		}
	}
}

// inputmanager.cpp:421
void CInputManager::ExcludeKey(const char *key) {

	KeyPair *pair = m_keyList.Find(key);

	if (pair)
		pair->flags |= KEY_EXCLUDED;
}

// inputmanager.cpp:430
void CInputManager::BlockKey(const char *key) {

	KeyPair *pair = m_keyList.Find(key);

	if (pair)
		pair->flags |= KEY_BLOCKED;
}

// inputmanager.cpp:439
void CInputManager::FreeKey(const char *key) {

	KeyPair *pair = m_keyList.Find(key);

	if (pair)
		pair->flags &= ~KEY_BLOCKED;
}

// inputmanager.cpp:448
void CInputManager::BlockKey(void) {
	KeyPair *pair = m_keyList.Start();
	while (pair) {

		pair->flags |= KEY_BLOCKED;
		pair = m_keyList.Next();
	}
}

// inputmanager.cpp:458
void CInputManager::FreeKey(void) {
	KeyPair *pair = m_keyList.Start();
	while (pair) {

		pair->flags &= ~KEY_BLOCKED;
		pair = m_keyList.Next();
	}
}

// inputmanager.cpp:468
void CInputManager::BlockMouse(void) {
	m_blockFlags |= BLOCK_MOUSE;
	for (int i = 0; i < 3; i++) {

		m_button[i].blocked = true;
	}
}

// inputmanager.cpp:476
void CInputManager::FreeMouse(void) {
	m_blockFlags &= ~BLOCK_MOUSE;
	for (int i = 0; i < 3; i++) {

		m_button[i].blocked = false;
	}
}

// inputmanager.cpp:484
void CInputManager::EraseAllList(void) {
	KeyPair *pair = m_keyList.Start();

	while (pair) {
		delete pair;
		pair = m_keyList.Next();
	}

	m_keyList.Reset();
}

// inputmanager.cpp:500
void CInputManager::SetFakeInput(eButton button) {
	if (m_swapButton) {

		if (button == LEFT_BUTTON)
			button = RIGHT_BUTTON;
		else if (button == RIGHT_BUTTON)
			button = LEFT_BUTTON;
	}


	m_button[button].fake = true;
}

// inputmanager.cpp:513
void CInputManager::SetFakeInput(const char *key, bool down) {

	KeyPair *pair = m_keyList.Find(key);

	if (pair) {



		pair->fake = true;
		if (down) {
			pair->state = 1;
			pair->prevState = 0;
		}


		if (pair->type == 3)
			pair->eventTime = timeGetTime();
	}
}

// inputmanager.cpp:533
void CInputManager::SwapMouseButton(bool swap) {
	m_swapButton = swap;
}
