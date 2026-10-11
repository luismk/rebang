// Original: d:\build\custom\temp\code\client\projectg\guildformcontroller.h
#ifndef PG_CLIENT_PROJECTG_GUILDFORMCONTROLLER_H
#define PG_CLIENT_PROJECTG_GUILDFORMCONTROLLER_H

#include <map>
#include <vector>

#include "..\..\defines.h"
#include "frelement.h"

class FrArea;
class FrButton;
class FrEdit;
class FrListBox;
class FrWnd;

// A guild history state (the argument of _guild::ConvertGuildHistory);
// enumerators not recovered yet, original header not known.
enum GUILD_STATE_IDX {
};

namespace _guild {

// The page a guild form shows (CGuildMainForm::m_formStyle; names invented).
enum eFormStyle {
	FORM_GUILD_INFO = 1,
	FORM_GUILD_MEMBER = 2,
	FORM_GUILD_SEARCH = 4
};

// The guild rights a form is shown with, one bit per guild rank
// (CGuildMainForm::ConvertGuildClass; names invented).
enum eAuthority {
	AUTHORITY_CLASS0 = 0x1,
	AUTHORITY_MASTER = 0x2,
	AUTHORITY_CLASS2 = 0x4,
	AUTHORITY_CLASS3 = 0x8,
	AUTHORITY_CLASS9 = 0x10,
	AUTHORITY_ALL = 0x1F,
	AUTHORITY_NONE = 0xFFFF
};

// The Fresh control type (enumGuiType) InsertPostAction records for a control
// class (sPostAction::m_type; the original's mapping is not known, name invented).
template <class T> struct sGuiTypeOf;
template <> struct sGuiTypeOf<FrEdit> { enum { value = GI_EDIT }; };
template <> struct sGuiTypeOf<FrButton> { enum { value = GI_BUTTON }; };
template <> struct sGuiTypeOf<FrArea> { enum { value = GI_AREA }; };
template <> struct sGuiTypeOf<FrListBox> { enum { value = GI_LISTBOX }; };

// Enables and shows the controls of a guild form by page and rights.
class CGuildFormController {
	LAYOUT_CHECK_FRIEND;

public:
	// A control registered with the action applied to it when a form with
	// one of m_formStyle's pages and m_authority's rights is shown.
	struct sPostAction {
		// guildformcontroller.h:47
		void operator()(CGuildFormController* pController) {
			(pController->*m_pfnAction)(this);
		}

		int m_type;                                                 // 0x0: the control's type (4 = FrEdit)
		int m_formStyle;                                            // 0x4: eFormStyle bits
		int m_authority;                                            // 0x8: eAuthority bits
		FrWnd* m_pWnd;                                              // 0xc
		void (CGuildFormController::*m_pfnAction)(sPostAction*);   // 0x10
	};

	CGuildFormController(void);
	~CGuildFormController(void);

	void ControllForm(eFormStyle formStyle, eAuthority authority);
	void ReadOnlywindow(sPostAction* pAction);
	void WritableWindow(sPostAction* pAction);
	void EnableWindow(sPostAction* pAction);
	void DisableWindow(sPostAction* pAction);
	void MoveLeftSide(sPostAction* pAction);
	void MoveRightSide(sPostAction* pAction);
	void ClearEdit(sPostAction* pAction);
	void VisibleWindow(sPostAction* pAction);
	void InVisibleWindow(sPostAction* pAction);
	void InsertControl(FrWnd* pWnd);

	// guildformcontroller.h:76
	template <class T>
	void InsertPostAction(T* pWnd, int formStyle, int authority, void (CGuildFormController::*pfnAction)(sPostAction*)) {
		sPostAction* pAction = new sPostAction;
		pAction->m_type = sGuiTypeOf<T>::value;
		pAction->m_formStyle = formStyle;
		pAction->m_authority = authority;
		pAction->m_pWnd = pWnd;
		pAction->m_pfnAction = pfnAction;

		m_postActions.insert(std::make_pair(pAction, pAction));
	}

private:
	std::vector<FrWnd*> m_controls;                      // 0x0: hidden and disabled before each ControllForm
	std::map<sPostAction*, sPostAction*> m_postActions;  // 0x10: owned
	eFormStyle m_formStyle;                              // 0x1c: the page last shown
};

CHECK_SIZE(CGuildFormController::sPostAction, 0x14);
CHECK_FIELD(CGuildFormController::sPostAction, m_type, 0x0);
CHECK_FIELD(CGuildFormController::sPostAction, m_formStyle, 0x4);
CHECK_FIELD(CGuildFormController::sPostAction, m_authority, 0x8);
CHECK_FIELD(CGuildFormController::sPostAction, m_pWnd, 0xc);
CHECK_FIELD(CGuildFormController::sPostAction, m_pfnAction, 0x10);
CHECK_SIZE(CGuildFormController, 0x20);
BEGIN_LAYOUT_CHECK(CGuildFormController)
	CHECK_FIELD(CGuildFormController, m_controls, 0x0);
	CHECK_FIELD(CGuildFormController, m_postActions, 0x10);
	CHECK_FIELD(CGuildFormController, m_formStyle, 0x1c);
END_LAYOUT_CHECK

int ConvertGuildHistory(GUILD_STATE_IDX state);

// The left/right page buttons of a paged guild list and which of them are enabled.
class CPageArrow {
	LAYOUT_CHECK_FRIEND;

public:
	CPageArrow(void) : m_arrowFlags(0) {}   // guildformcontroller.h:122
	~CPageArrow(void) {}              // guildformcontroller.h:123

	void InitControl(void);
	void RefreshArrowButton(int page, int pageCount);

	void SetLeftArrow(FrButton* pButton) { m_pLeftArrow = pButton; }     // guildformcontroller.h:128
	void SetRightArrow(FrButton* pButton) { m_pRightArrow = pButton; }   // guildformcontroller.h:129

private:
	FrButton* m_pLeftArrow;    // 0x0
	FrButton* m_pRightArrow;   // 0x4
	unsigned int m_arrowFlags; // 0x8: 1 = left arrow enabled, 2 = right arrow enabled
};

CHECK_SIZE(CPageArrow, 0xc);
BEGIN_LAYOUT_CHECK(CPageArrow)
	CHECK_FIELD(CPageArrow, m_pLeftArrow, 0x0);
	CHECK_FIELD(CPageArrow, m_pRightArrow, 0x4);
	CHECK_FIELD(CPageArrow, m_arrowFlags, 0x8);
END_LAYOUT_CHECK

} // namespace _guild

#endif // PG_CLIENT_PROJECTG_GUILDFORMCONTROLLER_H
