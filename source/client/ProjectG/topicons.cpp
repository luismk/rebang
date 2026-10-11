// Original: d:\build\custom\temp\code\client\projectg\topicons.cpp
#include "TopIcons.h"
#include "TaskManager.h"
#include "actor.h"
#include "frbutton.h"

// Target actor of the Wizcity open event message.
static const char* s_gatewayActorName = "Gateway";

// topicons.cpp:38
void CWizcityOpenEvent::OnInit(FrButton& button) {
	button.SetButtonImg("hat_event_top_n", FrButton::NORMAL);
	button.SetButtonImg("hat_event_top_o", FrButton::OVER);
	button.SetButtonImg("hat_event_top_o", FrButton::PRESSED);
	button.SetButtonImg("hat_event_top_o", FrButton::BLINK);
}

// topicons.cpp:48
void CWizcityOpenEvent::OnButtonDown(void) {
	AfxGetTask()->GetActor(s_gatewayActorName) << MsgObject(0, 0x26F, 0xD, 0, 0, 0, 0);
}
