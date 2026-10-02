#pragma once
#include "../../shared/titlemanager.h"

class Title
{
public:
	Title() { }
	virtual ~Title() { }
	virtual bool CanEquip() { return false; }
};

#define DECLARE_TITLE(num) \
	class Title##num : public Title \
	{ \
	public: \
		Title##num(); \
		virtual ~Title##num(); \
		virtual bool CanEquip(); \
	};

#define IMPLEMENT_TITLE(num) \
	Title##num::Title##num() \
	{ \
	} \
	Title##num::~Title##num() \
	{ \
	} \
	bool Title##num::CanEquip()

DECLARE_TITLE(21)
DECLARE_TITLE(22)
DECLARE_TITLE(23)
DECLARE_TITLE(24)
DECLARE_TITLE(25)
DECLARE_TITLE(26)
DECLARE_TITLE(27)
DECLARE_TITLE(28)
DECLARE_TITLE(357)
DECLARE_TITLE(358)
DECLARE_TITLE(359)
DECLARE_TITLE(360)
DECLARE_TITLE(361)

void RegisterTitles(TitleManager* titlemanager);
