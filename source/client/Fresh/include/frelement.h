#pragma once
#include <string>
#include <map>
#include <list>
#include "../../../shared/coord.h"

class FrElement;
class Bitmap;
class TiXmlNode;

struct resBitmap_t
{
	Bitmap* bitmap;
	int size;
	std::string name;
};

typedef std::map<std::string, resBitmap_t> FreshBitmapMap;

class FrElementDoc
{
public:
	FrElementDoc() { }
	virtual ~FrElementDoc();
	bool Load(const char* pszXmlFilename);
	const FreshBitmapMap& GetBitmapList() const { return m_mapBitmap; }
	const Bitmap* GetBitmap(const std::string& id);

protected:
	int LoadPic(const char* filename);

	std::list<FrElement*> m_elementList;
	FreshBitmapMap m_mapBitmap;
	std::string m_xmlFileName;
};

enum enumGuiType
{
	GI_NONE = 0x0,
	GI_FORM = 0x1,
	GI_STATIC = 0x2,
	GI_TEXTBUTTON = 0x3,
	GI_EDIT = 0x4,
	GI_COMBOBOX = 0x5,
	GI_COMBOCTLEX = 0x6,
	GI_BUTTON = 0x7,
	GI_FRAME = 0x8,
	GI_RESOURCE = 0x9,
	GI_BITMAP = 0xA,
	GI_AREA = 0xB,
	GI_LISTBOX = 0xC,
	GI_GAUGEBAR = 0xD,
	GI_GAUGEBAREX = 0xE,
	GI_GAUGEBARIMAGE = 0xF,
	GI_VIEWER = 0x10,
	GI_CONTEXTMENU = 0x11,
	GI_TABBUTTON = 0x12,
	GI_GROUPBOX = 0x13,
	GI_MACROITEM = 0x14,
	GI_LAST = 0x15,
};

class FrGuiItem
{
public:
	FrGuiItem()
		: m_type(GI_NONE), m_flag(0)
	{
	}
	virtual ~FrGuiItem() { }
	virtual void Init(const TiXmlNode* node);
	virtual const std::list<FrGuiItem*>* GetChildList();

	enumGuiType m_type;
	std::string m_resource;
	std::string m_caption;
	std::string m_name;
	std::map<std::string, std::string> m_param;
	unsigned long m_flag;
	_RectangleSHORT m_rect;
};
