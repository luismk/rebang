// Original: d:\build\custom\temp\code\client\projectg\ingameguildmyroomdlg.cpp
#include <windows.h>
#include <atlbase.h>	// stand-in for the precompiled header: _AtlWinModule initializer
#include <string.h>

#include "guilddefine.h"

#include "ingameguildmyroomdlg.h"
#include "ClientSetting.h"
#include "ObjectFactory.h"
#include "UccLibrary.h"
#include "bitmap.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frstatic.h"
#include "frwndmanager.h"
#include "wresrcmng.h"
#include "Packet.h"
#include "TaskManager.h"
#include "actor.h"
#include "SharedDoc.h"
#include "NetResourceManager.h"
#include "UWIN.h"
#include "commonutil.h"
#include "winput.h"

extern char g_executeDirectory[];
extern HWND g_hwnd;
extern WInputDev* g_mouse;

namespace _guild {

// Name of the guild actor the forms message (a pointer in this unit's data,
// next to "CGuildActor"; its header of origin is not recovered yet).
static const char* s_guildActorName = "GuildActor";
// Line separator of the guild introduction (the pointer after it).
static const char* s_newLine = "\n";
// File filter of the guild mark file dialog (OnMarkSearch_LButtonUp).
static const char* s_markFileFilter = "png Files\0*.png\0";

// ingameguildmyroomdlg.cpp:24 (run-time type and factory registration of FrCreateGuildKit)
const WRTTI FrCreateGuildKit::m_RTTI("FrCreateGuildKit", &FrForm::m_RTTI);

IObject* FrCreateGuildKitMakeInstance(void) {
	return new FrCreateGuildKit;
}

struct __sFrCreateGuildKit {
	__sFrCreateGuildKit(void) {
		ObjectFactory().AddObjectFunctor(FrCreateGuildKitMakeInstance, "FrCreateGuildKit");
	}
};
static __sFrCreateGuildKit s_registerFrCreateGuildKit;

// ingameguildmyroomdlg.cpp:26
const sFRESH_MSGMAP* FrCreateGuildKit::GetMessageMap(void) const {
	return &FrCreateGuildKit::_MsgMap;
}

sFRESH_MSGMAP FrCreateGuildKit::_MsgMap = { &FrForm::_MsgMap, &FrCreateGuildKit::_MsgEntries[0] };

sFRESH_ENTRY FrCreateGuildKit::_MsgEntries[] = {
	// ingameguildmyroomdlg.cpp:28
	{ "exist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCreateGuildKit::OnInitExistGuildNameButton },
	// ingameguildmyroomdlg.cpp:29
	{ "exist", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCreateGuildKit::OnLButtonUpExistGuildNameButton },
	// ingameguildmyroomdlg.cpp:30
	{ "guildname", FRCMD_ENTERKEY, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCreateGuildKit::OnExistGuildName_EnterKey },

	// ingameguildmyroomdlg.cpp:32
	{ "confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCreateGuildKit::OnGuildRegister_ButtonInit },
	// ingameguildmyroomdlg.cpp:33
	{ "confirm", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCreateGuildKit::OnGuildRegister_LButtonUp },
	// ingameguildmyroomdlg.cpp:34
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCreateGuildKit::OnCancel_ButtonInit },
	// ingameguildmyroomdlg.cpp:35
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCreateGuildKit::OnCancel_LButtonUp },

	// ingameguildmyroomdlg.cpp:37
	{ "guildname", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCreateGuildKit::OnInitInputGuildNameEdit },
	// ingameguildmyroomdlg.cpp:38
	{ "introduce", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCreateGuildKit::OnInitInputGuildIntroduceEdit },

	// ingameguildmyroomdlg.cpp:40
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// ingameguildmyroomdlg.cpp:43
FrCreateGuildKit::FrCreateGuildKit(void) {
	memset(m_pButtons, 0, sizeof(m_pButtons));
	memset(m_pEdits, 0, sizeof(m_pEdits));
}

// ingameguildmyroomdlg.cpp:48
FrCreateGuildKit::~FrCreateGuildKit(void) {
}

// ingameguildmyroomdlg.cpp:53
void FrCreateGuildKit::SetConfirmGuildName(const char* name) {
	// ingameguildmyroomdlg.cpp:56
	if (!m_confirmGuildName.empty()) {
		m_confirmGuildName.clear();
	}

	m_confirmGuildName = name;

	if (m_pEdits[0]) {
		m_pEdits[0]->ClearLine();
		m_pEdits[0]->AddLine(name, 0, false);
		m_pEdits[1]->SetKeyFocus(true);
	}
}

// ingameguildmyroomdlg.cpp:71
void FrCreateGuildKit::InitControl(void) {
	memset(m_pButtons, 0, sizeof(m_pButtons));
	memset(m_pEdits, 0, sizeof(m_pEdits));
}

// ingameguildmyroomdlg.cpp:77
void FrCreateGuildKit::OnInitExistGuildNameButton(intptr_t param) {
	m_pButtons[0] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildmyroomdlg.cpp:82
void FrCreateGuildKit::OnLButtonUpExistGuildNameButton(void) {
	std::string name = m_pEdits[0]->GetLine(1, false);

	if (name.empty()) {
		AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x3e8, 0, 0, 0);
		return;
	}

	int result = WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringGuildString(name.c_str(), 4, 0x14, true);
	if (result != 1) {
		IActor* pActor = AfxGetTask()->GetActor(s_guildActorName);
		if (pActor && result > 0) {
			pActor << MsgObject(0, 0x263, 0x1a, result, 0, 0, 0);
		}
		return;
	}

	WSendPacket packet((enumClientPacket)0xff);
	packet.EncodeStr(name);
	packet.Send(SEND_TO_DEFAULT);

	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
}

// ingameguildmyroomdlg.cpp:113
void FrCreateGuildKit::OnExistGuildName_EnterKey(int param) {
	OnLButtonUpExistGuildNameButton();
}

// ingameguildmyroomdlg.cpp:118
void FrCreateGuildKit::OnGuildRegister_ButtonInit(intptr_t param) {
	m_pButtons[1] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildmyroomdlg.cpp:123
void FrCreateGuildKit::OnGuildRegister_LButtonUp(void) {
	// ingameguildmyroomdlg.cpp:126
	if (m_confirmGuildName.empty()) {
		std::string name = m_pEdits[0]->GetLine(1, false);
		if (name.empty()) {
			AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x3e8, 0, 0, 0);
		} else {
			AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x404, 0, 0, 0);
		}
		return;
	}

	// ingameguildmyroomdlg.cpp:134
	const char* name = m_pEdits[0]->GetLine(1, false);
	if (m_confirmGuildName != name) {
		AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x404, 0, 0, 0);
		return;
	}

	// ingameguildmyroomdlg.cpp:141
	std::string introduce;
	int lineNum = m_pEdits[1]->GetLineNum();
	for (int i = 1; i < lineNum + 1; i++) {
		introduce += m_pEdits[1]->GetLine(i, false);
		if (i != lineNum) {
			introduce += s_newLine;
		}
	}

	// ingameguildmyroomdlg.cpp:150
	if (introduce.empty()) {
		AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x3e9, 0, 0, 0);
		return;
	}

	// ingameguildmyroomdlg.cpp:156
	WSendPacket packet((enumClientPacket)0xfe);
	packet.EncodeStr(m_confirmGuildName);
	packet.EncodeStr(introduce);
	packet.Send(SEND_TO_DEFAULT);

	Close(true);

	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
}

// ingameguildmyroomdlg.cpp:167
void FrCreateGuildKit::OnCancel_ButtonInit(intptr_t param) {
	m_pButtons[2] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildmyroomdlg.cpp:172
void FrCreateGuildKit::OnCancel_LButtonUp(void) {
	Close(true);
}

// ingameguildmyroomdlg.cpp:177
void FrCreateGuildKit::OnInitInputGuildNameEdit(intptr_t param) {
	m_pEdits[0] = DYNAMIC_CAST(FrEdit, param);
}

// ingameguildmyroomdlg.cpp:182
void FrCreateGuildKit::OnInitInputGuildIntroduceEdit(intptr_t param) {
	m_pEdits[1] = DYNAMIC_CAST(FrEdit, param);
}

// ingameguildmyroomdlg.cpp:191 (run-time type and factory registration of FrRegisterGuildMark)
const WRTTI FrRegisterGuildMark::m_RTTI("FrRegisterGuildMark", &FrForm::m_RTTI);

IObject* FrRegisterGuildMarkMakeInstance(void) {
	return new FrRegisterGuildMark;
}

struct __sFrRegisterGuildMark {
	__sFrRegisterGuildMark(void) {
		ObjectFactory().AddObjectFunctor(FrRegisterGuildMarkMakeInstance, "FrRegisterGuildMark");
	}
};
static __sFrRegisterGuildMark s_registerFrRegisterGuildMark;

// ingameguildmyroomdlg.cpp:192
const sFRESH_MSGMAP* FrRegisterGuildMark::GetMessageMap(void) const {
	return &FrRegisterGuildMark::_MsgMap;
}

sFRESH_MSGMAP FrRegisterGuildMark::_MsgMap = { &FrForm::_MsgMap, &FrRegisterGuildMark::_MsgEntries[0] };

sFRESH_ENTRY FrRegisterGuildMark::_MsgEntries[] = {
	// ingameguildmyroomdlg.cpp:194
	{ "search", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRegisterGuildMark::OnMarkSearch_ButtonInit },
	// ingameguildmyroomdlg.cpp:195
	{ "search", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrRegisterGuildMark::OnMarkSearch_LButtonUp },
	// ingameguildmyroomdlg.cpp:196
	{ "confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRegisterGuildMark::OnConfirm_ButtonInit },
	// ingameguildmyroomdlg.cpp:197
	{ "confirm", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrRegisterGuildMark::OnConfirm_LButtonUp },
	// ingameguildmyroomdlg.cpp:198
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRegisterGuildMark::OnCancel_ButtonInit },
	// ingameguildmyroomdlg.cpp:199
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrRegisterGuildMark::OnCancel_LButtonUp },
	// ingameguildmyroomdlg.cpp:200
	{ "mark_viewer", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRegisterGuildMark::OnMarkViewer_AreaInit },
	// ingameguildmyroomdlg.cpp:201
	{ "mark_viewer", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRegisterGuildMark::OnMarkViewer_OwnerDraw },
	// ingameguildmyroomdlg.cpp:202
	{ "mark_path", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRegisterGuildMark::OnMarkPath_EditInit },

	// ingameguildmyroomdlg.cpp:204
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// ingameguildmyroomdlg.cpp:208
FrRegisterGuildMark::FrRegisterGuildMark(void) {
	memset(m_pButtons, 0, sizeof(m_pButtons));
	m_pMarkPathEdit = 0;
	m_pMarkViewerArea = 0;
	m_pMarkBitmap = 0;
	m_windowed = 0;
	m_bUploading = false;
}

// ingameguildmyroomdlg.cpp:218
FrRegisterGuildMark::~FrRegisterGuildMark(void) {
	SetCurrentDirectory(g_executeDirectory);

	if (m_windowed == 0) {
		// ingameguildmyroomdlg.cpp:223
		if (WSingleton<COption>::Instance()) {
			sOption option = *WSingleton<COption>::Instance()->GetOption();
			option.m_windowed = 0;
			WSingleton<COption>::Instance()->ApplyChange(option, false);
		}
	}

	// ingameguildmyroomdlg.cpp:231
	ClearTexCache(m_pMarkBitmap); delete m_pMarkBitmap; m_pMarkBitmap = 0;
}

// ingameguildmyroomdlg.cpp:236
bool FrRegisterGuildMark::OnInit(void) {
	WSingleton<COption>::Instance()->vApplyLobbyScreenSize();

	sOption option = *WSingleton<COption>::Instance()->GetOption();
	m_windowed = option.m_windowed;
	if (m_windowed == 0) {
		// ingameguildmyroomdlg.cpp:243
		option.m_windowed = 1;
		WSingleton<COption>::Instance()->ApplyChange(option, false);
	}

	// ingameguildmyroomdlg.cpp:247
	return FrForm::OnInit();
}

// ingameguildmyroomdlg.cpp:251
void FrRegisterGuildMark::RequestUpload(int guildNumber, const std::string& path) {
	sResource resource;

	// ingameguildmyroomdlg.cpp:255
	resource.type = RESOURCE_TYPE_EMBLEM;
	resource.path = m_markPath;
	resource.id = guildNumber;
	sprintf(resource.data, "%s", path.c_str());

	// ingameguildmyroomdlg.cpp:259
	WSingleton<NetResourceManager>::Instance()->Upload(resource, "http://qa.contents.pangya.gametree.co.kr:50006/Guild/upload.asp");
	Close(FrOK, true);
}

// ingameguildmyroomdlg.cpp:264
void FrRegisterGuildMark::OnMarkSearch_ButtonInit(intptr_t param) {
	m_pButtons[0] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildmyroomdlg.cpp:269
void FrRegisterGuildMark::OnMarkSearch_LButtonUp(void) {
	// Releases the mouse to the file dialog for the scope of the search.
	struct CGuard {
		// ingameguildmyroomdlg.cpp:273
		CGuard(void) {
			g_mouse->InitDevice(g_hwnd, false);
		}

		// ingameguildmyroomdlg.cpp:279
		~CGuard(void) {
			g_mouse->InitDevice(g_hwnd, true);
		}
	};

	// ingameguildmyroomdlg.cpp:285
	char fileName[MAX_PATH] = { 0 };

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hwnd;
	ofn.lpstrFilter = s_markFileFilter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;

	// ingameguildmyroomdlg.cpp:295
	ClearTexCache(m_pMarkBitmap); delete m_pMarkBitmap; m_pMarkBitmap = 0;
	m_markPath.clear();
	m_bUploading = false;

	CGuard guard;
	if (GetOpenFileName(&ofn)) {
		char name[256] = { 0 };
		char ext[256] = { 0 };
		_splitpath(ofn.lpstrFile, 0, 0, name, ext);
		if (strcmp(".png", ext) != 0) {
			// ingameguildmyroomdlg.cpp:312
			AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x41c, 0, 0, 0);
			return;
		}

		// ingameguildmyroomdlg.cpp:316
		m_markPath = ofn.lpstrFile;

		HANDLE hFile = CreateFile(ofn.lpstrFile, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE) {
			// ingameguildmyroomdlg.cpp:322
			AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x421, 0, 0, 0);
			return;
		}

		// ingameguildmyroomdlg.cpp:327
		unsigned int size = UWIN::GetFileSize(ofn.lpstrFile);

		if (size == 0 || size > 0x2000000) {
			// ingameguildmyroomdlg.cpp:332
			AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x422, 0, 0, 0);
			return;
		}

		// ingameguildmyroomdlg.cpp:336
		char* buffer = new char[size];

		DWORD readSize = 0;
		ReadFile(hFile, buffer, size, &readSize, 0);
		if (readSize == 0) {
			// ingameguildmyroomdlg.cpp:343
			if (buffer) delete[] buffer;
			AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x420, 0, 0, 0);
			return;
		}

		// ingameguildmyroomdlg.cpp:348
		m_pMarkBitmap = g_resrcmng->LoadPNG(buffer, true, false);
		if (!m_pMarkBitmap) {
			// ingameguildmyroomdlg.cpp:352
			if (buffer) delete[] buffer;
			AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x420, 0, 0, 0);
			return;
		}

		// ingameguildmyroomdlg.cpp:357
		if (m_pMarkBitmap->BitsPerPixel() != 32) {
			// ingameguildmyroomdlg.cpp:359
			if (buffer) delete[] buffer;
			AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x423, 0, 0, 0);
			return;
		}

		// ingameguildmyroomdlg.cpp:365
		bool bTooLarge = false;
		if (m_pMarkBitmap->Width() > 22) {
			bTooLarge = true;
		}
		if (m_pMarkBitmap->Height() > 20) {
			bTooLarge = true;
		}

		// ingameguildmyroomdlg.cpp:375
		if (bTooLarge == true) {
			ClearTexCache(m_pMarkBitmap); delete m_pMarkBitmap;
			m_pMarkBitmap = 0; if (buffer) delete[] buffer;
			AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x41b, 0, 0, 0);
			return;
		}

		// ingameguildmyroomdlg.cpp:383
		std::string markName(name);
		markName += ext;
		m_pMarkPathEdit->ClearLine();
		m_pMarkPathEdit->AddLine(markName.c_str(), 0, false);
		if (buffer) delete[] buffer;

		// ingameguildmyroomdlg.cpp:390
		m_bUploading = true;
	}
}

// ingameguildmyroomdlg.cpp:394
void FrRegisterGuildMark::OnConfirm_ButtonInit(intptr_t param) {
	m_pButtons[1] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildmyroomdlg.cpp:399
void FrRegisterGuildMark::OnConfirm_LButtonUp(void) {
	// ingameguildmyroomdlg.cpp:403
	if (m_bUploading == true) {
		if (!m_markPath.empty()) {
			// ingameguildmyroomdlg.cpp:409
			unsigned long guildNumber = 0;
			AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x20, (intptr_t)&guildNumber, 0, 0, 0);

			WSendPacket packet((enumClientPacket)0x112);
			packet.Encode4(guildNumber);
			packet.Send(SEND_TO_DEFAULT);

			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
		}
	} else {
		// ingameguildmyroomdlg.cpp:420
		AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x41a, 0, 0, 0);

		m_bUploading = false;
		m_pMarkPathEdit->ClearLine();
	}
}

// ingameguildmyroomdlg.cpp:427
void FrRegisterGuildMark::OnCancel_ButtonInit(intptr_t param) {
	m_pButtons[2] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildmyroomdlg.cpp:432
void FrRegisterGuildMark::OnCancel_LButtonUp(void) {
	Close(FrCANCEL, true);
}

// ingameguildmyroomdlg.cpp:437
void FrRegisterGuildMark::OnMarkViewer_AreaInit(intptr_t param) {
	m_pMarkViewerArea = DYNAMIC_CAST(FrArea, param);
	if (m_pMarkViewerArea) {
		// ingameguildmyroomdlg.cpp:444
		m_pMarkBitmap = g_resrcmng->LoadPNG("guildmark", false, false);
	}
}

// ingameguildmyroomdlg.cpp:448
void FrRegisterGuildMark::OnMarkViewer_OwnerDraw(int param) {
	// ingameguildmyroomdlg.cpp:451
	if (m_pMarkBitmap) {
		const WRect& rect = m_pMarkViewerArea->GetRect();

		g_pFresh->GetManager()->GetGDI()->DrawTexture(m_pMarkBitmap,
			WRect(rect.x, rect.y, m_pMarkBitmap->Width(), m_pMarkBitmap->Height()), 0xffffffff, 0);
	}
}

// ingameguildmyroomdlg.cpp:460
void FrRegisterGuildMark::OnMarkPath_EditInit(intptr_t param) {
	m_pMarkPathEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pMarkPathEdit) {
		// ingameguildmyroomdlg.cpp:467
		m_pMarkPathEdit->SetReadOnly(true);
	}
}

// ingameguildmyroomdlg.cpp:475 (run-time type and factory registration of FrChangeGuildName)
const WRTTI FrChangeGuildName::m_RTTI("FrChangeGuildName", &FrForm::m_RTTI);

IObject* FrChangeGuildNameMakeInstance(void) {
	return new FrChangeGuildName;
}

struct __sFrChangeGuildName {
	__sFrChangeGuildName(void) {
		ObjectFactory().AddObjectFunctor(FrChangeGuildNameMakeInstance, "FrChangeGuildName");
	}
};
static __sFrChangeGuildName s_registerFrChangeGuildName;

// ingameguildmyroomdlg.cpp:476
const sFRESH_MSGMAP* FrChangeGuildName::GetMessageMap(void) const {
	return &FrChangeGuildName::_MsgMap;
}

sFRESH_MSGMAP FrChangeGuildName::_MsgMap = { &FrForm::_MsgMap, &FrChangeGuildName::_MsgEntries[0] };

sFRESH_ENTRY FrChangeGuildName::_MsgEntries[] = {
	// ingameguildmyroomdlg.cpp:478
	{ "original", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeGuildName::OnOriginalName_StaticInit },
	// ingameguildmyroomdlg.cpp:479
	{ "exist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeGuildName::OnInitExistGuildNameButton },
	// ingameguildmyroomdlg.cpp:480
	{ "exist", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrChangeGuildName::OnLButtonUpExistGuildNameButton },
	// ingameguildmyroomdlg.cpp:481
	{ "confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeGuildName::OnGuildNameConfirm_ButtonInit },
	// ingameguildmyroomdlg.cpp:482
	{ "confirm", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrChangeGuildName::OnGuildNameConfirm_LButtonUp },
	// ingameguildmyroomdlg.cpp:483
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeGuildName::OnGuildNameCancel_ButtonInit },
	// ingameguildmyroomdlg.cpp:484
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrChangeGuildName::OnGuildNameCancel_LButtonUp },
	// ingameguildmyroomdlg.cpp:485
	{ "guildname", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeGuildName::OnInitChangeGuildNameEdit },

	// ingameguildmyroomdlg.cpp:487
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// ingameguildmyroomdlg.cpp:491
FrChangeGuildName::FrChangeGuildName(void) {
	memset(m_pButtons, 0, sizeof(m_pButtons));
	m_pOriginalNameStatic = 0;
	m_pGuildNameEdit = 0;
}

// ingameguildmyroomdlg.cpp:498
FrChangeGuildName::~FrChangeGuildName(void) {
}

// ingameguildmyroomdlg.cpp:503
bool FrChangeGuildName::IsHaveGuild(void) {
	IActor* pActor = AfxGetTask()->GetActor(s_guildActorName);

	// ingameguildmyroomdlg.cpp:508
	GUILD_INFO* pInfo = 0;
	pActor << MsgObject(0, 0x263, 0x21, (intptr_t)&pInfo, 0, 0, 0);
	if (pInfo->guildNumber == 0) {
		// ingameguildmyroomdlg.cpp:511
		pActor << MsgObject(0, 0x263, 0x1a, 0x410, 0, 0, 0);
		return false;
	}

	// ingameguildmyroomdlg.cpp:515
	return true;
}

// ingameguildmyroomdlg.cpp:519
void FrChangeGuildName::SetConfirmedName(const char* name) {
	// ingameguildmyroomdlg.cpp:522
	m_confirmedName = name;
}

// ingameguildmyroomdlg.cpp:526
void FrChangeGuildName::OnOriginalName_StaticInit(intptr_t param) {
	m_pOriginalNameStatic = DYNAMIC_CAST(FrStatic, param);
	if (m_pOriginalNameStatic) {
		// ingameguildmyroomdlg.cpp:533
		IActor* pActor = AfxGetTask()->GetActor(s_guildActorName);
		GUILD_INFO* pInfo = 0;
		pActor << MsgObject(0, 0x263, 0x21, (intptr_t)&pInfo, 0, 0, 0);
		if (pInfo->guildNumber == 0) {
			// ingameguildmyroomdlg.cpp:538
			Close(true);
			return;
		}

		// ingameguildmyroomdlg.cpp:542
		m_pOriginalNameStatic->SetCaption(MakeStr("%s", pInfo->name));
	}
}

// ingameguildmyroomdlg.cpp:547
void FrChangeGuildName::OnInitExistGuildNameButton(intptr_t param) {
	m_pButtons[0] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildmyroomdlg.cpp:552
void FrChangeGuildName::OnLButtonUpExistGuildNameButton(void) {
	std::string name;
	name = m_pGuildNameEdit->GetLine(1, false);
	if (name.empty()) {
		AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x3e8, 0, 0, 0);
		return;
	}

	IActor* pActor = AfxGetTask()->GetActor(s_guildActorName);
	if (!pActor) {
		return;
	}

	int result = WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringGuildString(name.c_str(), 4, 0x14, true);
	if (result != 1) {
		IActor* pGuildActor = AfxGetTask()->GetActor(s_guildActorName);
		if (pGuildActor && result > 0) {
			pGuildActor << MsgObject(0, 0x263, 0x1a, result, 0, 0, 0);
		}
		return;
	}

	int exist;
	pActor << MsgObject(0, 0x263, 0x20, (intptr_t)&exist, 0, 0, 0);

	WSendPacket packet((enumClientPacket)0xff);
	packet.EncodeStr(name);
	packet.Send(SEND_TO_DEFAULT);

	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
}

// ingameguildmyroomdlg.cpp:590
void FrChangeGuildName::OnGuildNameConfirm_ButtonInit(intptr_t param) {
	m_pButtons[1] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildmyroomdlg.cpp:595
void FrChangeGuildName::OnGuildNameConfirm_LButtonUp(void) {
	std::string newName;
	newName = m_pGuildNameEdit->GetLine(1, false);
	if (newName.empty()) {
		// ingameguildmyroomdlg.cpp:600
		AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x3e8, 0, 0, 0);
		return;
	}

	// ingameguildmyroomdlg.cpp:605
	std::string originalName(m_pOriginalNameStatic->GetCaption());
	if (m_confirmedName == originalName) {
		// ingameguildmyroomdlg.cpp:608
		AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0xd2f3, 0, 0, 0);
		return;
	}

	if (m_confirmedName != newName) {
		// ingameguildmyroomdlg.cpp:615
		AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1a, 0x404, 0, 0, 0);
		return;
	}

	// ingameguildmyroomdlg.cpp:619
	IActor* pActor = AfxGetTask()->GetActor(s_guildActorName);
	if (!pActor) {
		return;
	}

	// ingameguildmyroomdlg.cpp:626
	unsigned long guildNumber;
	pActor << MsgObject(0, 0x263, 0x20, (intptr_t)&guildNumber, 0, 0, 0);

	WSendPacket packet((enumClientPacket)0x100);
	packet.Encode4(guildNumber);
	packet.EncodeStr(newName);
	packet.Send(SEND_TO_DEFAULT);

	Close(FrOK, true);

	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
}

// ingameguildmyroomdlg.cpp:639
void FrChangeGuildName::OnGuildNameCancel_ButtonInit(intptr_t param) {
	m_pButtons[2] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildmyroomdlg.cpp:644
void FrChangeGuildName::OnGuildNameCancel_LButtonUp(void) {
	Close(true);
}

// ingameguildmyroomdlg.cpp:649
void FrChangeGuildName::OnInitChangeGuildNameEdit(intptr_t param) {
	m_pGuildNameEdit = DYNAMIC_CAST(FrEdit, param);
}

} // namespace _guild
