// Original: d:\build\custom\temp\code\client\projectg\uccclothes.h
#ifndef PG_CLIENT_PROJECTG_UCCCLOTHES_H
#define PG_CLIENT_PROJECTG_UCCCLOTHES_H

#include "UccBase.h"

// UCC item drawing for clothes parts. Overrides nothing of CUccBaseItemDraw
// (its vtable ??_7CUccClothes@@6B@ only replaces the destructor).
class CUccClothes : public CUccBaseItemDraw {
public:
	CUccClothes(void);
	virtual ~CUccClothes(void);
};

#endif // PG_CLIENT_PROJECTG_UCCCLOTHES_H
