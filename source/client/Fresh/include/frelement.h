// Original: d:\build\custom\temp\code\client\fresh\include\frelement.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRELEMENT_H
#define PG_CLIENT_FRESH_INCLUDE_FRELEMENT_H

#include <algorithm>
#include <list>
#include <map>
#include <string>

#include "..\..\defines.h"
#include "../coord.h"
#include "../client/ProjectG/commonutil.h"

class Bitmap;
class TiXmlNode;

template <class T> struct sameElement;

enum enumElement {
	ED_NONE = 0,
	ED_FRAME = 1,
	ED_LAYOUT = 2,
	ED_FORM = 3,
	ED_BITMAP = 4,
	ED_ICON = 5,
	ED_INCLUDE = 6,
	ED_MACROITEM = 7,
	ED_LAST = 8
};

enum enumGuiType {
	GI_NONE = 0,
	GI_FORM = 1,
	GI_STATIC = 2,
	GI_TEXTBUTTON = 3,
	GI_EDIT = 4,
	GI_COMBOBOX = 5,
	GI_COMBOCTLEX = 6,
	GI_BUTTON = 7,
	GI_FRAME = 8,
	GI_RESOURCE = 9,
	GI_BITMAP = 10,
	GI_AREA = 11,
	GI_LISTBOX = 12,
	GI_GAUGEBAR = 13,
	GI_GAUGEBAREX = 14,
	GI_GAUGEBARIMAGE = 15,
	GI_VIEWER = 16,
	GI_CONTEXTMENU = 17,
	GI_TABBUTTON = 18,
	GI_GROUPBOX = 19,
	GI_MACROITEM = 20,
	GI_LAST = 21
};

// Two-way table between a type enum and its name in the XML (PDB: size 0xc).
template <class T>
struct FrTypeID {
	FrTypeID(void) {}                                    // frelement.h:98
	~FrTypeID(void) {}                                   // frelement.h:99

	std::string operator[](const T& type);

	// frelement.h:102
	T operator[](const std::string& typeId) {
		for (typename std::map<T, std::string>::iterator it = m_map.begin(); it != m_map.end(); ++it) {
			if (it->second == typeId) {
				return it->first;
			}
		}
		return (T)0;
	}

protected:
	std::map<T, std::string> m_map;                      // 0x0
};

// PDB: size 0xc, base FrTypeID<enumElement>.
struct FrElementTID : public FrTypeID<enumElement> {
	FrElementTID(void);
};

CHECK_SIZE(FrElementTID, 0xc);

// PDB: size 0xc, base FrTypeID<enumGuiType>.
struct FrGuiTID : public FrTypeID<enumGuiType> {
	FrGuiTID(void);
};

CHECK_SIZE(FrGuiTID, 0xc);

extern FrElementTID ElementType;
extern FrGuiTID GuiType;

// One item of a GUI description (PDB: size 0x74).
class FrGuiItem {
public:
	FrGuiItem(void) : m_type(GI_NONE), m_flag(0) {}      // frelement.h:158
	virtual ~FrGuiItem(void) {}
	virtual void Init(const TiXmlNode* pSrc);
	virtual const std::list<FrGuiItem*>* GetChildList(void);

	enumGuiType m_type;                                  // 0x4
	std::string m_resource;                              // 0x8
	std::string m_caption;                               // 0x24
	std::string m_name;                                  // 0x40
	std::map<std::string, std::string> m_param;          // 0x5c
	unsigned long m_flag;                                // 0x68
	_RectangleSHORT m_rect;                              // 0x6c
};

CHECK_SIZE(FrGuiItem, 0x74);
CHECK_FIELD(FrGuiItem, m_type, 0x4);
CHECK_FIELD(FrGuiItem, m_resource, 0x8);
CHECK_FIELD(FrGuiItem, m_caption, 0x24);
CHECK_FIELD(FrGuiItem, m_name, 0x40);
CHECK_FIELD(FrGuiItem, m_param, 0x5c);
CHECK_FIELD(FrGuiItem, m_flag, 0x68);
CHECK_FIELD(FrGuiItem, m_rect, 0x6c);

// A GUI item with children (PDB: size 0x80, base FrGuiItem).
class FrGuiItemNested : public FrGuiItem {
	LAYOUT_CHECK_FRIEND;
public:
	virtual ~FrGuiItemNested(void);
	virtual void Init(const TiXmlNode* pSrc);
	virtual const std::list<FrGuiItem*>* GetChildList(void);

protected:
	std::list<FrGuiItem*> m_childList;                   // 0x74
};

CHECK_SIZE(FrGuiItemNested, 0x80);
BEGIN_LAYOUT_CHECK(FrGuiItemNested)
	CHECK_FIELD(FrGuiItemNested, m_childList, 0x74);
END_LAYOUT_CHECK

// Base of every element of a Fresh document (PDB: size 0x24). The PDB has no
// ??_7FrElement and its constructor/destructor store no vptr: novtable.
class __declspec(novtable) FrElement {
public:
	FrElement(enumElement type) : m_type(type) {}       // frelement.h:226
	virtual ~FrElement(void) {}                          // frelement.h:228
	virtual void Init(const TiXmlNode* pSrc) = 0;

	std::string m_name;                                  // 0x4
	enumElement m_type;                                  // 0x20
};

CHECK_SIZE(FrElement, 0x24);
CHECK_FIELD(FrElement, m_name, 0x4);
CHECK_FIELD(FrElement, m_type, 0x20);

// PDB: size 0x100, base FrElement.
class FrElementFrame : public FrElement {
public:
	FrElementFrame(void) : FrElement(ED_FRAME) {}      // frelement.h:239
	virtual ~FrElementFrame(void) {}                     // frelement.h:242
	virtual void Init(const TiXmlNode* pSrc);

	unsigned long m_color;                               // 0x24
	int m_layers;                                        // 0x28
	int m_aType[8];                                      // 0x2c
	int m_aPos[8];                                       // 0x4c
	int m_aHeight[8];                                    // 0x6c
	int m_aInvisible[8];                                 // 0x8c
	std::string m_bfrmName;                              // 0xac
	std::string m_sfrmName;                              // 0xc8
	std::string m_cfrmName;                              // 0xe4
};

CHECK_SIZE(FrElementFrame, 0x100);
CHECK_FIELD(FrElementFrame, m_color, 0x24);
CHECK_FIELD(FrElementFrame, m_layers, 0x28);
CHECK_FIELD(FrElementFrame, m_aType, 0x2c);
CHECK_FIELD(FrElementFrame, m_aPos, 0x4c);
CHECK_FIELD(FrElementFrame, m_aHeight, 0x6c);
CHECK_FIELD(FrElementFrame, m_aInvisible, 0x8c);
CHECK_FIELD(FrElementFrame, m_bfrmName, 0xac);
CHECK_FIELD(FrElementFrame, m_sfrmName, 0xc8);
CHECK_FIELD(FrElementFrame, m_cfrmName, 0xe4);

// PDB: size 0xcc, base FrElement.
class FrElementLayout : public FrElement {
public:
	struct sSize {
		int cx;                                          // 0x0
		int cy;                                          // 0x4
	};

	FrElementLayout(const std::string& name);
	FrElementLayout(void) : FrElement(ED_LAYOUT) {}    // frelement.h:259
	// frelement.h:262
	virtual ~FrElementLayout(void) {
		sequence_delete(m_guiList.begin(), m_guiList.end());
		m_guiList.clear();
	}
	virtual void Init(const TiXmlNode* pSrc);

	sSize m_bgSize;                                      // 0x24
	unsigned long m_bgColor;                             // 0x2c
	std::string m_bgFilename;                            // 0x30
	std::list<FrGuiItem*> m_guiList;                     // 0x4c
	FrGuiItem m_aniBg;                                   // 0x58
};

CHECK_SIZE(FrElementLayout, 0xcc);
CHECK_FIELD(FrElementLayout, m_bgSize, 0x24);
CHECK_FIELD(FrElementLayout, m_bgColor, 0x2c);
CHECK_FIELD(FrElementLayout, m_bgFilename, 0x30);
CHECK_FIELD(FrElementLayout, m_guiList, 0x4c);
CHECK_FIELD(FrElementLayout, m_aniBg, 0x58);

// PDB: size 0x40, base FrElement.
class FrElementBitmap : public FrElement {
public:
	FrElementBitmap(void) : FrElement(ED_BITMAP) {}    // frelement.h:284
	virtual ~FrElementBitmap(void) {}                    // frelement.h:285

	bool Create(const char* name, const char* fileName);
	virtual void Init(const TiXmlNode* pSrc);

	std::string m_bitmapFilename;                        // 0x24
};

CHECK_SIZE(FrElementBitmap, 0x40);
CHECK_FIELD(FrElementBitmap, m_bitmapFilename, 0x24);

// PDB: size 0xe4, base FrElement.
class FrElementForm : public FrElement {
public:
	struct sSize {
		int w;                                           // 0x0
		int h;                                           // 0x4
	};

	FrElementForm(void) : FrElement(ED_FORM) {}        // frelement.h:303
	// frelement.h:305
	virtual ~FrElementForm(void) {
		sequence_delete(m_guiList.begin(), m_guiList.end());
		m_guiList.clear();
	}
	virtual void Init(const TiXmlNode* pSrc);

	std::string m_caption;                               // 0x24
	std::string m_desc;                                  // 0x40
	sSize m_bgSize;                                      // 0x5c
	FrGuiItem m_bgFrame;                                 // 0x64
	std::list<FrGuiItem*> m_guiList;                     // 0xd8
};

CHECK_SIZE(FrElementForm, 0xe4);
CHECK_FIELD(FrElementForm, m_caption, 0x24);
CHECK_FIELD(FrElementForm, m_desc, 0x40);
CHECK_FIELD(FrElementForm, m_bgSize, 0x5c);
CHECK_FIELD(FrElementForm, m_bgFrame, 0x64);
CHECK_FIELD(FrElementForm, m_guiList, 0xd8);

// PDB: size 0x24, base FrElement.
class FrElementIcon : public FrElement {
public:
	FrElementIcon(void);
	virtual ~FrElementIcon(void);
	virtual void Init(const TiXmlNode* pSrc);
};

CHECK_SIZE(FrElementIcon, 0x24);

// PDB: size 0x30, base FrElement.
class FrElementMacroItem : public FrElement {
public:
	FrElementMacroItem(void) : FrElement(ED_MACROITEM) {} // frelement.h:327
	// frelement.h:329
	virtual ~FrElementMacroItem(void) {
		sequence_delete(m_guiList.begin(), m_guiList.end());
		m_guiList.clear();
	}
	virtual void Init(const TiXmlNode* pSrc);

	std::list<FrGuiItem*> m_guiList;                     // 0x24
};

CHECK_SIZE(FrElementMacroItem, 0x30);
CHECK_FIELD(FrElementMacroItem, m_guiList, 0x24);

// A bitmap loaded by a document (PDB: size 0x24).
struct resBitmap_t {
	Bitmap* bitmap;                                      // 0x0
	int size;                                            // 0x4
	std::string name;                                    // 0x8
};

CHECK_SIZE(resBitmap_t, 0x24);
CHECK_FIELD(resBitmap_t, bitmap, 0x0);
CHECK_FIELD(resBitmap_t, size, 0x4);
CHECK_FIELD(resBitmap_t, name, 0x8);

// frelement.h:388
template <class L, class T>
T* FindElement(L& eList, enumElement eType, const std::string& resID) {
	typename L::iterator itEnd = eList.end();
	typename L::iterator it;
	it = std::find_if(eList.begin(), itEnd, sameElement<typename L::value_type>(eType, resID));
	if (it != itEnd) {
		return static_cast<T*>(*it);
	}
	return 0;
}

// The elements of a Fresh XML document (PDB: size 0x38).
class FrElementDoc {
	LAYOUT_CHECK_FRIEND;
public:
	FrElementDoc(void) {}                                // frelement.h:402
	virtual ~FrElementDoc(void);

	bool Load(const char* fileName);
	FrElementLayout* GetLayout(const std::string& resID);
	FrElementFrame* GetFrame(const std::string& resID);
	FrElementForm* GetForm(const std::string& resID);
	const Bitmap* GetBitmap(const std::string& resID);
	FrElementMacroItem* GetMacroItem(const std::string& resID);
	const std::map<std::string, resBitmap_t>& GetBitmapList(void) const { return m_mapBitmap; }   // frelement.h:411

protected:
	bool LoadXml(const char* fileName, std::list<FrElement*>& elementList);
	int LoadPic(const char* fileName);
	void ClearBitmaps(void);

	std::list<FrElement*> m_elementList;                 // 0x4
	std::map<std::string, resBitmap_t> m_mapBitmap;      // 0x10
	std::string m_xmlFileName;                           // 0x1c
};

CHECK_SIZE(FrElementDoc, 0x38);
BEGIN_LAYOUT_CHECK(FrElementDoc)
	CHECK_FIELD(FrElementDoc, m_elementList, 0x4);
	CHECK_FIELD(FrElementDoc, m_mapBitmap, 0x10);
	CHECK_FIELD(FrElementDoc, m_xmlFileName, 0x1c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRELEMENT_H
