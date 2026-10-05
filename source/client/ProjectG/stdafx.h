#pragma once

#define WINVER 0x0400
#define _WIN32_WINNT 0x0400
#define _ATL_STATIC_REGISTRY

#define WLIST_ALLOC(size) malloc(size)
#define WLIST_FREE(ptr) free(ptr)

#include <winsock2.h>
#include <windows.h>
#include "../Wangreal/include/wtypes.h"
#include <atlbase.h>
#include <atlapp.h>
extern WTL::CAppModule _Module;
#include <atlwin.h>
#include <atlmisc.h>
#include <atldlgs.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlctrlx.h>

#include "wmath.h"
#include "gamath.h"
#include "wutil.h"
#include "wminmax.h"
#include "wlist.h"
#include "wmemblock.h"
#include "wlock.h"
#include "wflag.h"
#include "wmempak.h"
#include "rectmng.h"
#include "wscene.h"
#include "wvideo.h"
#include "wmesh.h"
#include "wdevmng.h"
#include "wxtnlbuffer.h"
#include "xzip.h"
#include "cfile.h"
#include "wreg.h"
#include "bitmap.h"
#include "wavi.h"
#include "westpak.h"
#include "wblockmodel.h"
#include "wboneset.h"
#include "wpetfile.h"
#include "soundmanager.h"
#include "objectfactory.h"
#include "background.h"
#include "wview.h"
#include "w3danispr.h"
#include "wpuppet.h"
#include "woverlay.h"
#include "singleton.h"

inline void WVector2D::operator+=(const WVector2D& v)
{
	x += v.x;
	y += v.y;
}

#include <string>
#include <list>
#include <map>
#include <vector>
#include <algorithm>

#include "basicdebug.h"
#include "commonutil.h"

#include "tinyxml.h"

#include "../../shared/exceptionreport.h"
#include "lock.hpp"
#include "packet.h"
#include "titles_client.h"
#include "networksystem.h"
#include "taskmanager.h"
#include "frbutton.h"
#include "frwndinl.h"
#include "frgaugebar.h"
#include "frlistbox.h"
#include "frviewer.h"
#include "frstatic.h"
#include "frframe.h"
#include "fremoticon.h"
#include "frgraphicinterface.h"
#include "fredit.h"
#include "frarea.h"
#include "fresh.h"
#include "wfont.h"
#include "ucclibrary.h"
#include "hackingmanager.h"
#include "frwndmanager.h"
#include "inputmanager.h"
#include "ccrc32.h"
#include "z_ilfill.h"
ILFILL1
#include "tweaker.h"
#include "gdvoiceitem.h"
#include "actor.h"
#include "polysoup.h"
#include "golfrule.h"
#include "scenemanager.h"
#include "courseorder.h"

#include "autotarget.h"
#include "golfdoc.h"
#include "shareddoc.h"
#include "frtext.h"
#include "mainframe.h"
#include "wtemplate.h"
