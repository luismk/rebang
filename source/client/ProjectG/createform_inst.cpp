// Force explicit instantiation of CreateForm<FrTreasureCourse> to satisfy
// link references from dialogs that construct the treasure course form.
#include "frform.h"
#include "TreasureDlg.h"

template FrTreasureCourse* CreateForm<FrTreasureCourse>(FrWndManager*, FrCmdTarget*, const char*, FrWnd*);
