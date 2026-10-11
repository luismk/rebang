// Original: d:\build\custom\temp\code\client\fresh\include\fresh.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRESH_H
#define PG_CLIENT_FRESH_INCLUDE_FRESH_H

#include <map>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frelement.h"
#include "frwndmanager.h"

class Bitmap;
class FrCmdTarget;
class FrDesktop;

// The Fresh UI system object: the element document, the window manager and
// the visibility switch. Layout, access and methods from the PDB (size 0x60,
// vfptr at 0). The bodies written here are the inline ones whose COMDATs
// carry fresh.h line marks (lines 63-67).
class Fresh {
	LAYOUT_CHECK_FRIEND;
public:
	Fresh(void);
	virtual ~Fresh(void);

	bool Init(const char* pszXmlFilename, bool exclusiveKey, const char* wallPaper);
	void ShutDown(void);
	void OnProcess(const float deltaTime);
	void OnDisplay(bool drawDesktop);
	bool OpenLayout(const char* layout, FrCmdTarget* owner, bool first);
	void CloseLayout(void);
	bool IsCurrentLayout(const char* layout) const;
	const Bitmap* RegisterBitmap(const char* bitmap);
	const Bitmap* GetBitmap(const char* bitmap);
	FrDesktop* GetDesktop(void) const { return m_pWndManager->GetDesktop(); }   // fresh.h:63
	FrWndManager* GetManager(void) const { return m_pWndManager; }             // fresh.h:64
	bool IsDesktopFocused(void) const;
	void Toggle(bool bShow) { m_bShow = bShow; }                               // fresh.h:66
	bool IsVisible(void) const { return m_bShow; }                             // fresh.h:67
	const std::map<std::string, resBitmap_t>& ListBres(std::string& name);
	void HidePrivacy(bool hide);

protected:
	FrElementDoc m_doc;                 // 0x4
	FrWndManager* m_pWndManager;        // 0x3c
	bool m_bShow;                       // 0x40
	std::string m_xmlFileName;          // 0x44
};

CHECK_SIZE(Fresh, 0x60);
BEGIN_LAYOUT_CHECK(Fresh)
	CHECK_FIELD(Fresh, m_doc, 0x4);
	CHECK_FIELD(Fresh, m_pWndManager, 0x3c);
	CHECK_FIELD(Fresh, m_bShow, 0x40);
	CHECK_FIELD(Fresh, m_xmlFileName, 0x44);
END_LAYOUT_CHECK

extern Fresh* g_pFresh;

#endif // PG_CLIENT_FRESH_INCLUDE_FRESH_H
