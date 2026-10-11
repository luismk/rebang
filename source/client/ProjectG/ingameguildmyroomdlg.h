// Original: d:\build\custom\temp\code\client\projectg\ingameguildmyroomdlg.h
#ifndef PG_CLIENT_PROJECTG_INGAMEGUILDMYROOMDLG_H
#define PG_CLIENT_PROJECTG_INGAMEGUILDMYROOMDLG_H

#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class FrArea;
class FrButton;
class FrEdit;
class FrStatic;

namespace _guild {

// The "create a guild" form: guild name and introduction edits, the name
// check / register / cancel buttons. Its size (0x140) is the `new` of
// FrCreateGuildKitMakeInstance.
class FrCreateGuildKit : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ingameguildmyroomdlg.h:18

	FrCreateGuildKit(void);
	virtual ~FrCreateGuildKit(void);

	void SetConfirmGuildName(const char* name);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitExistGuildNameButton(intptr_t param);
	void OnLButtonUpExistGuildNameButton(void);
	void OnExistGuildName_EnterKey(int param);
	void OnGuildRegister_ButtonInit(intptr_t param);
	void OnGuildRegister_LButtonUp(void);
	void OnCancel_ButtonInit(intptr_t param);
	void OnCancel_LButtonUp(void);
	void OnInitInputGuildNameEdit(intptr_t param);
	void OnInitInputGuildIntroduceEdit(intptr_t param);

private:
	void InitControl(void);

	FrButton* m_pButtons[3];          // 0x110 ("exist", "confirm", "cancel")
	FrEdit* m_pEdits[2];              // 0x11c ("guildname", "introduce")
	std::string m_confirmGuildName;   // 0x124 (SetConfirmGuildName)
};

CHECK_SIZE(FrCreateGuildKit, 0x140);
BEGIN_LAYOUT_CHECK(FrCreateGuildKit)
	CHECK_FIELD(FrCreateGuildKit, m_pButtons, 0x110);
	CHECK_FIELD(FrCreateGuildKit, m_pEdits, 0x11c);
	CHECK_FIELD(FrCreateGuildKit, m_confirmGuildName, 0x124);
END_LAYOUT_CHECK

// The "register a guild mark" form: the mark file path edit, the mark
// preview area and the search / confirm / cancel buttons. Its size (0x150)
// is the `new` of FrRegisterGuildMarkMakeInstance.
class FrRegisterGuildMark : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ingameguildmyroomdlg.h:73

	FrRegisterGuildMark(void);
	virtual ~FrRegisterGuildMark(void);

	virtual bool OnInit(void);

	void RequestUpload(int guildNumber, const std::string& path);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnMarkSearch_ButtonInit(intptr_t param);
	void OnMarkSearch_LButtonUp(void);
	void OnConfirm_ButtonInit(intptr_t param);
	void OnConfirm_LButtonUp(void);
	void OnCancel_ButtonInit(intptr_t param);
	void OnCancel_LButtonUp(void);
	void OnMarkViewer_AreaInit(intptr_t param);
	void OnMarkViewer_OwnerDraw(int param);
	void OnMarkPath_EditInit(intptr_t param);

private:
	FrButton* m_pButtons[3];          // 0x110 ("search", "confirm", "cancel")
	FrEdit* m_pMarkPathEdit;          // 0x11c ("mark_path")
	FrArea* m_pMarkViewerArea;        // 0x120 ("mark_viewer")
	Bitmap* m_pMarkBitmap;            // 0x124 (the "guildmark" preview)
	std::string m_markPath;           // 0x128
	int m_windowed;                   // 0x144 (sOption::m_windowed when the form opened)
	bool m_bUploading;                // 0x148 (zeroed by the constructor)
	UNKNOWN(0x149, 0x150);
};

CHECK_SIZE(FrRegisterGuildMark, 0x150);
BEGIN_LAYOUT_CHECK(FrRegisterGuildMark)
	CHECK_FIELD(FrRegisterGuildMark, m_pButtons, 0x110);
	CHECK_FIELD(FrRegisterGuildMark, m_pMarkPathEdit, 0x11c);
	CHECK_FIELD(FrRegisterGuildMark, m_pMarkViewerArea, 0x120);
	CHECK_FIELD(FrRegisterGuildMark, m_pMarkBitmap, 0x124);
	CHECK_FIELD(FrRegisterGuildMark, m_markPath, 0x128);
	CHECK_FIELD(FrRegisterGuildMark, m_windowed, 0x144);
	CHECK_FIELD(FrRegisterGuildMark, m_bUploading, 0x148);
END_LAYOUT_CHECK

// The "change the guild name" form: the current name static, the new name
// edit and the name check / confirm / cancel buttons. Its size (0x140) is
// the `new` of FrChangeGuildNameMakeInstance.
class FrChangeGuildName : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ingameguildmyroomdlg.h:122

	FrChangeGuildName(void);
	virtual ~FrChangeGuildName(void);

	bool IsHaveGuild(void);
	void SetConfirmedName(const char* name);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnOriginalName_StaticInit(intptr_t param);
	void OnInitExistGuildNameButton(intptr_t param);
	void OnLButtonUpExistGuildNameButton(void);
	void OnGuildNameConfirm_ButtonInit(intptr_t param);
	void OnGuildNameConfirm_LButtonUp(void);
	void OnGuildNameCancel_ButtonInit(intptr_t param);
	void OnGuildNameCancel_LButtonUp(void);
	void OnInitChangeGuildNameEdit(intptr_t param);

private:
	FrStatic* m_pOriginalNameStatic;  // 0x110 ("original")
	FrEdit* m_pGuildNameEdit;         // 0x114 ("guildname")
	FrButton* m_pButtons[3];          // 0x118 ("exist", "confirm", "cancel")
	std::string m_confirmedName;      // 0x124 (SetConfirmedName)
};

CHECK_SIZE(FrChangeGuildName, 0x140);
BEGIN_LAYOUT_CHECK(FrChangeGuildName)
	CHECK_FIELD(FrChangeGuildName, m_pOriginalNameStatic, 0x110);
	CHECK_FIELD(FrChangeGuildName, m_pGuildNameEdit, 0x114);
	CHECK_FIELD(FrChangeGuildName, m_pButtons, 0x118);
	CHECK_FIELD(FrChangeGuildName, m_confirmedName, 0x124);
END_LAYOUT_CHECK

} // namespace _guild

#endif // PG_CLIENT_PROJECTG_INGAMEGUILDMYROOMDLG_H
