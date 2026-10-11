// Original: d:\build\custom\temp\code\client\projectg\topiconinterface.h
#ifndef PG_CLIENT_PROJECTG_TOPICONINTERFACE_H
#define PG_CLIENT_PROJECTG_TOPICONINTERFACE_H

#include <string>

#include "..\..\defines.h"
#include "frbutton.h"

// Base of every lobby top-page icon. Vtable order from the calls of
// CIconManager: destructor, OnInit (+4), OnButtonDown (+8), SetInformation
// (+0xc), SetIconImage (+0x10).
class CIconInterface {
public:
	// topiconinterface.h:29
	CIconInterface(void) {}
	// topiconinterface.h:30
	virtual ~CIconInterface(void) {}
	virtual void OnInit(FrButton& button) = 0;
	virtual void OnButtonDown(void) = 0;
	// topiconinterface.h:34
	virtual void SetInformation(std::string& normalImage, std::string& overImage, std::string& blinkImage, std::string& url, std::string& title) {}
	// topiconinterface.h:35
	virtual void SetIconImage(FrButton& button) {}
};

CHECK_SIZE(CIconInterface, 0x4);

// An icon whose images come from the game data.
class CTopIcon : public CIconInterface {
public:
	// topiconinterface.h:42
	CTopIcon(void) {}
	// topiconinterface.h:43
	virtual ~CTopIcon(void) {}
};

CHECK_SIZE(CTopIcon, 0x4);

// An icon described by the web event data (images and link).
class CWebIcon : public CIconInterface {
	LAYOUT_CHECK_FRIEND;
public:
	// topiconinterface.h:57
	CWebIcon(void) {}
	// topiconinterface.h:58
	virtual ~CWebIcon(void) {}

	// topiconinterface.h:61
	virtual void SetInformation(std::string& normalImage, std::string& overImage, std::string& blinkImage, std::string& url, std::string& title) {
		m_normalImage = normalImage;
		m_overImage = overImage;
		m_blinkImage = blinkImage;
		m_url = url;
		m_title = title;
	}

	// topiconinterface.h:70
	virtual void SetIconImage(FrButton& button) {
		button.SetButtonImg(m_normalImage.c_str(), FrButton::NORMAL);
		button.SetButtonImg(m_overImage.c_str(), FrButton::OVER);
		button.SetButtonImg(m_blinkImage.c_str(), FrButton::BLINK);
	}

protected:
	std::string m_normalImage;          // 0x04
	std::string m_overImage;            // 0x20
	std::string m_blinkImage;           // 0x3c
	std::string m_title;                // 0x58
	std::string m_url;                  // 0x74
};

CHECK_SIZE(CWebIcon, 0x90);

BEGIN_LAYOUT_CHECK(CWebIcon)
	CHECK_FIELD(CWebIcon, m_normalImage, 0x04);
	CHECK_FIELD(CWebIcon, m_overImage, 0x20);
	CHECK_FIELD(CWebIcon, m_blinkImage, 0x3c);
	CHECK_FIELD(CWebIcon, m_title, 0x58);
	CHECK_FIELD(CWebIcon, m_url, 0x74);
END_LAYOUT_CHECK

// Declares a top icon class C<name> deriving from base, with its factory
// pair and its registration in the CIconManager under the key "<name>".
// The whole class is one source line where the macro is used.
#define DECLARE_ICON_CLASS(cls, base, key) \
	class cls : public base { \
	public: \
		cls(void) {} \
		virtual ~cls(void) {} \
		static CIconInterface* CreateTopIconInstance(void) { return new cls; } \
		static void DestoryTopIconInstance(CIconInterface* icon) { delete icon; } \
		static void RegisterToTopIconDB(void) { \
			CIconManager::Instance()->RegisterInstanceFunction(key, CreateTopIconInstance, DestoryTopIconInstance); \
		} \
		virtual void OnInit(FrButton& button); \
		virtual void OnButtonDown(void); \
	}

#define DECLARE_TOP_ICON(name) DECLARE_ICON_CLASS(C##name, CTopIcon, #name)
#define DECLARE_WEB_ICON(name) DECLARE_ICON_CLASS(CWebIcons_##name, CWebIcon, #name)

#endif // PG_CLIENT_PROJECTG_TOPICONINTERFACE_H
