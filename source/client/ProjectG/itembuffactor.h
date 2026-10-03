#pragma once

#include <list>
#include "actor.h"
#include "frcmdtarget.h"

struct sSCardAvilityPeriodInfo;
struct sItemInfo;
class FrSPCardBuffDlg;
class WReceivedPacket;
struct FrListItem;
class FrForm;
struct Bitmap;

class CItemBuff : public IActor, public FrCmdTarget
{
protected:
	CItemBuff();

public:
	DECLARE_OBJECT(CItemBuff)

	virtual ~CItemBuff();

	virtual void OnLoad();
	virtual void HandleMsg(const MsgObject& msg);

	static void ResetBuffList();
	static void ProcessPacket(WReceivedPacket& packet);
	static void AddBuffItem(sSCardAvilityPeriodInfo& info);
	static void RemoveBuffItem(unsigned long typeId);
	static void UpdateBuffDlg(FrSPCardBuffDlg* dlg);
};
