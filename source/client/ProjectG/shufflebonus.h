// Original: d:\build\custom\temp\code\client\projectg\shufflebonus.h
#ifndef PG_CLIENT_PROJECTG_SHUFFLEBONUS_H
#define PG_CLIENT_PROJECTG_SHUFFLEBONUS_H

#include "..\..\defines.h"
#include "singleton.h"

class WOverlay;

// Shuffle Bonus notice: while the room plays its holes in shuffled order
// (first hole of the order being played), draws the bonus overlay and posts
// the "+10%" chat notice. No members beyond the singleton base.
class CShuffleBonus : public WSingleton<CShuffleBonus> {
public:
	enum eVaildType {
		eVaild_FirstHole = 0,   // also requires the first hole of the order to be the current one
		eVaild_Room = 1,
	};

	CShuffleBonus(void);
	virtual ~CShuffleBonus(void);

	void Render(WOverlay* pOverlay, float alpha);
	void ShowNoticeMsg(void);

private:
	bool _Validity(eVaildType type);
};
CHECK_SIZE(CShuffleBonus, 0x28);

#endif
