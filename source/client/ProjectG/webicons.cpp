// Original: d:\build\custom\temp\code\client\projectg\webicons.cpp
#include "WebIcons.h"

#include "frbutton.h"

// webicons.cpp:16
void CWebIcons_WebEvent1::OnInit(FrButton& button) {
	button.SetVisible(true);
	button.SetPushSound("ui_desktop_icon_click");
	button.SetStyle(FrButton::BT_NORMAL);
	button.SetButtonImg(m_normalImage.c_str(), FrButton::NORMAL);
	button.SetButtonImg(m_overImage.c_str(), FrButton::OVER);
}

// webicons.cpp:25
void CWebIcons_WebEvent1::OnButtonDown(void) {
}

// webicons.cpp:56
void CWebIcons_WebEvent2::OnInit(FrButton& button) {
	button.SetVisible(true);
	button.SetPushSound("ui_desktop_icon_click");
	button.SetStyle(FrButton::BT_NORMAL);
	button.SetButtonImg(m_normalImage.c_str(), FrButton::NORMAL);
	button.SetButtonImg(m_overImage.c_str(), FrButton::OVER);
}

// webicons.cpp:65
void CWebIcons_WebEvent2::OnButtonDown(void) {
}
