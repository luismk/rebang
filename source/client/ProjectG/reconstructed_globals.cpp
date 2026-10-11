// Reconstructed global storage for symbols that were declared extern in the
// supplied source but had no defining translation unit in the link manifest.
// The runtime initializes the resource/input/view pointers during CProjectG::Init.
#include <windows.h>
#include <string>

#include "projectg.h"
#include "inputmanager.h"
#include "buddymanager.h"
#include "lobbybg.h"
#include "miles.h"
#include "../shared/titlemanager.h"
#include "../Wangreal/include/wview.h"
#include "../Wangreal/include/wresrcmng.h"
#include "../Wangreal/include/wfont.h"
#include "../Wangreal/include/winput.h"
#include "../Wangreal/include/wscene.h"

// Core runtime pointers. They are populated by the normal startup sequence.
WView* g_view = 0;
WResourceManager* g_resrcmng = 0;
CInputManager* g_input = 0;
WFont* g_font = 0;
WInputDev* g_keyboard = 0;
WInputDev* g_mouse = 0;
WInputDev* g_ime = 0;
WMilesSoundSystem* g_miles = 0;
TitleManager* g_TitleManager = 0;
BuddyManager* BuddyMGR = 0;

// Process/window state. These are assigned by WinMain and the application.
HWND g_hwnd = 0;
HINSTANCE g_instance = 0;
char g_executeDirectory[MAX_PATH] = { 0 };
bool g_bQuit = false;
bool g_bFocus = true;
bool g_bOldFocus = true;
bool g_bAviCapture = false;
bool g_showTime = false;
unsigned long g_CurrentTime = 0;
int g_iSendPacketType = 0;
int g_iRecvPacketType = 0;
int g_iRecvDisconCode = 0;
int g_iSendFailType = 0;
int g_last_screen_width = 0;
int g_last_screen_height = 0;

// Shared render state used by the game, lobby and scene modules.
WMatrix g_camera;
LightSet g_lightset;

// Lobby map selection is set from the current course during startup.
int LOBBYBG_MAP = 0;
char* LOBBYBG_MAPNAME = 0;

// These seven localized labels are copied from the existing room-creation
// dialog table; the room-list category tabs use indexes 0..6.
std::string g_strGameType[7] = {
    "\xbd\xba\xc6\xae\xb7\xce\xc5\xa9",
    "\xb8\xc5\xc4\xa1",
    "\xc3\xa4\xc6\xc3\xb9\xe6",
    "\xb7\xa1\xb4\xf5",
    "\xb4\xeb\xc8\xb8",
    "\xb4\xeb\xc8\xb8\xc6\xc0\xc0\xfc",
    "\xb1\xe6\xb5\xe5\xb4\xeb\xc0\xfc"
};

// Lake caustics state read by the console and consumed by pool rendering.
float causticSpeed = 0.0f;
int causticAlpha = 0;
