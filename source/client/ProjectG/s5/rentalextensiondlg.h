// Original: d:\build\custom\temp\code\client\projectg\s5\rentalextensiondlg.h
#ifndef PG_CLIENT_PROJECTG_S5_RENTALEXTENSIONDLG_H
#define PG_CLIENT_PROJECTG_S5_RENTALEXTENSIONDLG_H

#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "tweaker.h"

struct sItemInfo;

namespace S5 {

// Rental extension dialog; its size (0x118) is the `new` of
// FrRentalExtensionDlgMakeInstance. Its constructor stores the
// tweaker_cmd_target vtable at 0x110, after the FrForm part.
class FrRentalExtensionDlg : public FrForm, public tweaker_cmd_target {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // rentalextensiondlg.h:16

	FrRentalExtensionDlg(void);
	virtual ~FrRentalExtensionDlg(void);

	const sItemInfo* GetItemInfo(void);   // rentalextensiondlg.h:22
	void Initialize(const sItemInfo* itemInfo);

protected:
	static sFRESH_MSGMAP _MsgMap;
	static sFRESH_ENTRY _MsgEntries[];
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnDialogAreaDraw(void);
	void OnExtensionBtnUp(void);
	void OnDestroyBtnUp(void);
	void OnCancelBtnUp(void);

	std::string SetChildWindowPos(const tweaker_param& param);
	std::string GetChildWindowPos(const tweaker_param& param);

private:
	sItemInfo* m_itemInfo;                // 0x114

	LAYOUT_CHECK_FRIEND;
};

} // namespace S5

BEGIN_LAYOUT_CHECK(S5::FrRentalExtensionDlg)
	CHECK_FIELD(S5::FrRentalExtensionDlg, m_itemInfo, 0x114);
END_LAYOUT_CHECK
CHECK_SIZE(S5::FrRentalExtensionDlg, 0x118);

#endif // PG_CLIENT_PROJECTG_S5_RENTALEXTENSIONDLG_H
