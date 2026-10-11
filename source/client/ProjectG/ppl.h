// Original: d:\build\custom\temp\code\client\projectg\ppl.h
#ifndef PG_CLIENT_PROJECTG_PPL_H
#define PG_CLIENT_PROJECTG_PPL_H

#include "..\..\defines.h"
#include "singleton.h"

// The PPL check: downloads a probe file over WinINet and records in COption
// whether the download worked and how large it was. No members of its own
// (the constructor stores only the BaseObject part and the vptr).
class CPPL : public WSingleton<CPPL> {
public:
	CPPL(void);
	virtual ~CPPL(void);

	bool IsPPLEnable(void);
};

#endif // PG_CLIENT_PROJECTG_PPL_H
