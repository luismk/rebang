// Original: d:\build\custom\temp\code\client\projectg\weather.cpp
#include "weather.h"
#include "woverlay.h"
#include "wview.h"

// weather.cpp:15
void CCloud::Display(void) {
}

// weather.cpp:19
void CCloud::DisplayFullScreenOverlay(void) {
	WOverlay::DrawBox(g_view, WRect(0, 0, g_view->GetWidth(), g_view->GetHeight()), 0, 0x60303030, 0.001f);
}
