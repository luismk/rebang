// Reconstructed explicit singleton storage definitions.
// These definitions are required by the linker when the corresponding
// translation units are absent from the supplied source package.
#include "buddymanager.h"
#include "camerawork.h"
#include "cardsystem.h"
#include "console.h"
#include "icontentsdoc.h"
#include "topiconmanager.h"
#include "intrusion.h"
#include "networkmonitor.h"
#include "noticeboard.h"
#include "ppl.h"
#include "splash.h"
#include "../../shared/tikimagicboxtable.h"
#include "wtimer.h"
#include "messagemanager.h"

// One storage definition per WSingleton specialization reported as unresolved
// by the latest ProjectG link log. Keep these in one translation unit so that
// no duplicate static-storage definitions are introduced.
template <> BuddyManager* WSingleton<BuddyManager>::m_pInstance = 0;
template <> CCameraWork* WSingleton<CCameraWork>::m_pInstance = 0;
template <> CCardManager* WSingleton<CCardManager>::m_pInstance = 0;
template <> CConsole* WSingleton<CConsole>::m_pInstance = 0;
template <> CContentsDoc* WSingleton<CContentsDoc>::m_pInstance = 0;
template <> CIconManager* WSingleton<CIconManager>::m_pInstance = 0;
template <> CIntrusion* WSingleton<CIntrusion>::m_pInstance = 0;
template <> CNetworkMonitor* WSingleton<CNetworkMonitor>::m_pInstance = 0;
template <> CNoticeBoard* WSingleton<CNoticeBoard>::m_pInstance = 0;
template <> CPPL* WSingleton<CPPL>::m_pInstance = 0;
template <> CSplash* WSingleton<CSplash>::m_pInstance = 0;
template <> CTikiMagicBoxDoc* WSingleton<CTikiMagicBoxDoc>::m_pInstance = 0;
template <> CTimerManager* WSingleton<CTimerManager>::m_pInstance = 0;
template <> _systemmsg::CMessageManager* WSingleton<_systemmsg::CMessageManager>::m_pInstance = 0;

// Room-list filter state recovered from the uses and comments in lobbymain.cpp.
// Slot [2] stores the active category (4 = ALL); slot [3] stores the optional
// exact game type (-1 = no exact-type filter). Slots [0] and [1] are reserved
// because their original meaning is not identified in the supplied source.
extern "C" int data_aa9eac[4] = { 0, 0, 4, -1 };

// Initial chat-language/IME state. The original binary's exact initial value
// cannot be established from the source alone; false matches the default
// non-alphanumeric state used by the UI's Korean/English indicator.
extern "C" bool data_ab2830 = false;
