// Original: d:\build\custom\temp\code\client\projectg\avatar_mainui.cpp
// This object keeps the out-of-line copy of CSharedDoc::GetTradeUID.
#define AVATAR_MAINUI_SHAREDDOC_INLINE
#include <string.h>
#include "ClientSetting.h"
#include "Avatar_MainUI.h"
#include "frwnd.h"
#include "frform.h"
#include "avatar_avatar.h"
#include "frbutton.h"
#include "frarea.h"
#include "fredit.h"
#include "frcombobox.h"
#include "wresrcmng.h"
#include "JukeBoxDlg.h"
#include "AvatarChatLogDlg.h"
#include "ChatMsg.h"
#include "fresh.h"
#include "AvatarUserListDlg.h"
#include "NoteDlg.h"
#include "Packet.h"
#include "wview.h"
#include "AvatarTipDlg.h"
#include "EmoticonDlg.h"
#include "ReportDlg.h"
#include "messengerdlg.h"
#include "user_info.h"
#include "SharedDoc.h"
#include "frframe.h"
#include "frwndmanager.h"
#include "winput.h"
#include "commonutil.h"

// The IME input device (public ?g_ime@@3PAVWInputDev@@A).
extern WInputDev* g_ime;

// clientsetting.h:408
int COption::gGetAvatarNewBie(void) {
	return m_option.m_avatarNewBie;
}

// avatar_mainui.cpp:42
const sFRESH_MSGMAP* ntAvatarChatMain::GetMessageMap(void) const {
	return &ntAvatarChatMain::_MsgMap;
}

sFRESH_MSGMAP ntAvatarChatMain::_MsgMap = { &CTaskMain::_MsgMap, &ntAvatarChatMain::_MsgEntries[0] };

// avatar_mainui.cpp:45-262, the rows in the original order (initializer sub_0098e4a0).
sFRESH_ENTRY ntAvatarChatMain::_MsgEntries[] = {
	{ "AVATARCHAT_MAIN", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnAvatarChatMainInit },
	{ "AVATARCHAT_MAIN", FRCMD_FINISH, FrFuncVV, (FRESH_PMSG)&ntAvatarChatMain::OnAvatarChatMainFinish },
	{ "AVATARCHAT_MAIN.portrait", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnPortraitInit },
	{ "AVATARCHAT_MAIN.portrait_mask_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnPortraitMaskAreaInit },
	{ "AVATARCHAT_MAIN.portrait_mask", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnPortraitMaskInit },
	{ "AVATARCHAT_MAIN.portrait_mask", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&ntAvatarChatMain::OnPortraitMaskBtnUp },
	{ "AVATARCHAT_MAIN.portrait_tipbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnPortraitTipButtonInit },
	{ "AVATARCHAT_MAIN.portrait_tipbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&ntAvatarChatMain::OnPortraitTipButtonBtnUp },
	{ "AVATARCHAT_MAIN.portrait_jukeboxbtn_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnPortraitJukeBoxButtonAreaInit },
	{ "AVATARCHAT_MAIN.portrait_jukeboxbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnPortraitJukeBoxButtonInit },
	{ "AVATARCHAT_MAIN.portrait_jukeboxbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&ntAvatarChatMain::OnPortraitJukeBoxButtonBtnUp },
	{ "AVATARCHAT_MAIN.portrait_chatlogbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnPortraitChatLogButtonInit },
	{ "AVATARCHAT_MAIN.portrait_chatlogbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&ntAvatarChatMain::OnPortraitChatLogButtonBtnUp },
	{ "AVATARCHAT_MAIN.userlistbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnAvatarUserListButtonInit },
	{ "AVATARCHAT_MAIN.userlistbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&ntAvatarChatMain::OnAvatarUserListButtonBtnUp },
	{ "AVATARCHAT_MAIN.chatframe", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnChatFrameInit },
	{ "AVATARCHAT_MAIN.chatview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnChatViewInit },
	{ "AVATARCHAT_MAIN.emoticon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnEmoticonInit },
	{ "AVATARCHAT_MAIN.emoticon", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&ntAvatarChatMain::OnEmoticonBtnUp },
	{ "AVATARCHAT_MAIN.language", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnLanguageInit },
	{ "AVATARCHAT_MAIN.report", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnReportInit },
	{ "AVATARCHAT_MAIN.report", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&ntAvatarChatMain::OnReportBtnUp },
	{ "AVATARCHAT_MAIN.chattarget", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnChatTargetInit },
	{ "AVATARCHAT_MAIN.chattarget", FRCMD_LBUTTONDOWN, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnChatTargetBtnDown },
	{ "AVATARCHAT_MAIN.chattarget", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&ntAvatarChatMain::OnChatTargetEnterKey },
	{ "AVATARCHAT_MAIN.chatinput", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnChatInputInit },
	{ "AVATARCHAT_MAIN.chatinput", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&ntAvatarChatMain::OnChatInputEnterKey },
	{ "AVATARCHAT_MAIN.rmenu", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntAvatarChatMain::OnRightMenuInit },
	{ "AVATARCHAT_MAIN.rmenu", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&ntAvatarChatMain::OnRightMenuLBtnDown },
	{ "AVATARCHAT_MAIN.rmenu", FRCMD_RBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&ntAvatarChatMain::OnRightMenuRBtnDown },
	{ "AVATARCHAT_MAIN.ubar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBarInit },
	{ "AVATARCHAT_MAIN.ubar.back", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_BackInit },
	{ "AVATARCHAT_MAIN.ubar.back", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_BackUp },
	{ "AVATARCHAT_MAIN.ubar.back", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.ubar.back", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.ubar.front", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_FrontInit },
	{ "AVATARCHAT_MAIN.ubar.front", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_FrontUp },
	{ "AVATARCHAT_MAIN.ubar.front", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.ubar.front", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.ubar.shop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ShopInit },
	{ "AVATARCHAT_MAIN.ubar.shop", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ShopUp },
	{ "AVATARCHAT_MAIN.ubar.shop", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.ubar.shop", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.ubar.myroom", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MyRoomInit },
	{ "AVATARCHAT_MAIN.ubar.myroom", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MyRoomUp },
	{ "AVATARCHAT_MAIN.ubar.myroom", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.ubar.myroom", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.ubar.itemstorage", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ItemStorageInit },
	{ "AVATARCHAT_MAIN.ubar.itemstorage", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ItemStorageUp },
	{ "AVATARCHAT_MAIN.ubar.itemstorage", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.ubar.itemstorage", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.ubar.gift", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GiftInit },
	{ "AVATARCHAT_MAIN.ubar.gift", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GiftUp },
	{ "AVATARCHAT_MAIN.ubar.gift", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.ubar.gift", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.ubar.cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_CookieInit },
	{ "AVATARCHAT_MAIN.ubar.cookie", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_CookieUp },
	{ "AVATARCHAT_MAIN.ubar.cookie", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.ubar.cookie", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.ubar.game", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GameInit },
	{ "AVATARCHAT_MAIN.ubar.game", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GameUp },
	{ "AVATARCHAT_MAIN.ubar.game", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.ubar.game", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.ubar.server", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ServerInit },
	{ "AVATARCHAT_MAIN.ubar.server", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ServerUp },
	{ "AVATARCHAT_MAIN.ubar.server", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.ubar.server", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.ubar.exit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ExitInit },
	{ "AVATARCHAT_MAIN.ubar.exit", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ExitUp },
	{ "AVATARCHAT_MAIN.ubar.exit", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.ubar.exit", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.ubar.tabbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_TabBtnInit },
	{ "AVATARCHAT_MAIN.ubar.tabbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_TabBtnUp },
	{ "AVATARCHAT_MAIN.utab", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderTab_Init },
	{ "AVATARCHAT_MAIN.utab.magicbox", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MagicBoxInit },
	{ "AVATARCHAT_MAIN.utab.magicbox", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MagicBoxUp },
	{ "AVATARCHAT_MAIN.utab.magicbox", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.utab.magicbox", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.utab.tikimagicbox", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_TikiMagicBoxInit },
	{ "AVATARCHAT_MAIN.utab.tikimagicbox", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_TikiMagicBoxUp },
	{ "AVATARCHAT_MAIN.utab.tikimagicbox", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.utab.tikimagicbox", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.utab.scratch", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_ScratchInit },
	{ "AVATARCHAT_MAIN.utab.scratch", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_ScratchUp },
	{ "AVATARCHAT_MAIN.utab.scratch", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.utab.scratch", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.utab.bongdarishop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_BongdariShopInit },
	{ "AVATARCHAT_MAIN.utab.bongdarishop", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_BongdariShopUp },
	{ "AVATARCHAT_MAIN.utab.bongdarishop", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.utab.bongdarishop", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.utab.maket", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_MaketInit },
	{ "AVATARCHAT_MAIN.utab.maket", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_MaketUp },
	{ "AVATARCHAT_MAIN.utab.maket", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.utab.maket", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.utab.ranking", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_RankingInit },
	{ "AVATARCHAT_MAIN.utab.ranking", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_RankingUp },
	{ "AVATARCHAT_MAIN.utab.ranking", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.utab.ranking", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.utab.guild", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_GuildInit },
	{ "AVATARCHAT_MAIN.utab.guild", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_GuildUp },
	{ "AVATARCHAT_MAIN.utab.guild", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.utab.guild", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.utab.notice", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnUnderBar_NoticeInit },
	{ "AVATARCHAT_MAIN.utab.notice", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnUnderBar_NoticeUp },
	{ "AVATARCHAT_MAIN.utab.notice", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.utab.notice", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.obar.title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_TitleInit },
	{ "AVATARCHAT_MAIN.obar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_Init },
	{ "AVATARCHAT_MAIN.obar.bar1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_Bar1Init },
	{ "AVATARCHAT_MAIN.obar.bar2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_Bar2Init },
	{ "AVATARCHAT_MAIN.obar.bar3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_Bar3Init },
	{ "AVATARCHAT_MAIN.obar.frame_1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_Frame1Init },
	{ "AVATARCHAT_MAIN.obar.frame_2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_Frame2Init },
	{ "AVATARCHAT_MAIN.obar.frame_3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_Frame3Init },
	{ "AVATARCHAT_MAIN.obar.site", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_SiteInit },
	{ "AVATARCHAT_MAIN.obar.char", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharInit },
	{ "AVATARCHAT_MAIN.obar.char", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_CharDown },
	{ "AVATARCHAT_MAIN.obar.char", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharOwnerDraw },
	{ "AVATARCHAT_MAIN.obar.caddie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieInit },
	{ "AVATARCHAT_MAIN.obar.caddie", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_CaddieDown },
	{ "AVATARCHAT_MAIN.obar.caddie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieOwnerDraw },
	{ "AVATARCHAT_MAIN.obar.club", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubInit },
	{ "AVATARCHAT_MAIN.obar.club", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_ClubDown },
	{ "AVATARCHAT_MAIN.obar.club", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubOwnerDraw },
	{ "AVATARCHAT_MAIN.obar.aztec", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecInit },
	{ "AVATARCHAT_MAIN.obar.aztec", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_AztecDown },
	{ "AVATARCHAT_MAIN.obar.aztec", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecOwnerDraw },
	{ "AVATARCHAT_MAIN.obar.mascot", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotInit },
	{ "AVATARCHAT_MAIN.obar.mascot", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MascotDown },
	{ "AVATARCHAT_MAIN.obar.mascot", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotOwnerDraw },
	{ "AVATARCHAT_MAIN.obar.char_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharSelInit },
	{ "AVATARCHAT_MAIN.obar.char_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CharSelOwnerDraw },
	{ "AVATARCHAT_MAIN.obar.caddie_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieSelInit },
	{ "AVATARCHAT_MAIN.obar.caddie_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_CaddieSelOwnerDraw },
	{ "AVATARCHAT_MAIN.obar.club_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubSelInit },
	{ "AVATARCHAT_MAIN.obar.club_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_ClubSelOwnerDraw },
	{ "AVATARCHAT_MAIN.obar.aztec_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecSelInit },
	{ "AVATARCHAT_MAIN.obar.aztec_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_AztecSelOwnerDraw },
	{ "AVATARCHAT_MAIN.obar.mascot_sel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotSelInit },
	{ "AVATARCHAT_MAIN.obar.mascot_sel", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MascotSelOwnerDraw },
	{ "AVATARCHAT_MAIN.obar.pang_cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_PangCookieInit },
	{ "AVATARCHAT_MAIN.obar.pang_cookie", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_PangCookieOwnerDraw },
	{ "AVATARCHAT_MAIN.obar.onelinemsg_req", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineReqBtnInit },
	{ "AVATARCHAT_MAIN.obar.onelinemsg_req", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_OneLineReqBtnUp },
	{ "AVATARCHAT_MAIN.obar.onelineview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineViewInit },
	{ "AVATARCHAT_MAIN.obar.onelineview", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OneLineViewOwnerDraw },
	{ "AVATARCHAT_MAIN.obar.spcard", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_SpecialCardInit },
	{ "AVATARCHAT_MAIN.obar.spcard", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_SpecialCardUp },
	{ "AVATARCHAT_MAIN.obar.spcard", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.obar.spcard", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.obar.messenger", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MessengerInit },
	{ "AVATARCHAT_MAIN.obar.messenger", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MessengerUp },
	{ "AVATARCHAT_MAIN.obar.messenger", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.obar.messenger", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.obar.myinfo", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_MyInfoInit },
	{ "AVATARCHAT_MAIN.obar.myinfo", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_MyInfoUp },
	{ "AVATARCHAT_MAIN.obar.myinfo", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.obar.myinfo", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.obar.option", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnOverBar_OptionInit },
	{ "AVATARCHAT_MAIN.obar.option", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnOverBar_OptionUp },
	{ "AVATARCHAT_MAIN.obar.option", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOn },
	{ "AVATARCHAT_MAIN.obar.option", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarHoverOff },
	{ "AVATARCHAT_MAIN.obar.messenger_bubble", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleInit },
	{ "AVATARCHAT_MAIN.obar.messenger_bubble", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleOwnerDraw },
	{ "AVATARCHAT_MAIN.obar.messenger_desc", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerBubbleEditInit },
	{ "AVATARCHAT_MAIN.obar.messenger_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerAlarmBtnInit },
	{ "AVATARCHAT_MAIN.obar.messenger_alarm", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnMessengerAlarmBtnUp },
	{ "AVATARCHAT_MAIN.obar.gift_alarm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnMessengerGiftAlarmBtnInit },
	{ "AVATARCHAT_MAIN.obar.gift_alarm", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CTaskMain::OnMessengerGiftAlarmBtnUp },
	{ "AVATARCHAT_MAIN.obar.tooltip", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTaskMain::OnBarTooltipOwnerDraw },
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// avatar_mainui.cpp:267
void ntAvatarChatMain::InitUI(void) {
	ResetFrWndPointers();

	m_uiCycle = 0;
	m_portraitBitmaps[0] = g_resrcmng->LoadBitmap("m_def.tga", 0, false);
	m_portraitBitmaps[1] = g_resrcmng->LoadBitmap("f_def.tga", 0, false);
	m_portraitBitmaps[2] = g_resrcmng->LoadBitmap("a_def.tga", 0, false);
	m_portraitBitmaps[3] = g_resrcmng->LoadBitmap("c_def.tga", 0, false);
	m_portraitBitmaps[4] = g_resrcmng->LoadBitmap("d_def.tga", 0, false);
	m_portraitBitmaps[5] = g_resrcmng->LoadBitmap("e_def.tga", 0, false);
	m_portraitBitmaps[6] = g_resrcmng->LoadBitmap("g_def.tga", 0, false);
	m_portraitBitmaps[7] = g_resrcmng->LoadBitmap("h_def.tga", 0, false);
	m_portraitBitmaps[8] = g_resrcmng->LoadBitmap("i_def.tga", 0, false);
	m_portraitBitmaps[9] = g_resrcmng->LoadBitmap("j_def.tga", 0, false);
}

// avatar_mainui.cpp:286
void ntAvatarChatMain::ResetFrWndPointers(void) {
	m_jukeBoxButton = NULL;
	m_portrait = NULL;
	m_portraitMask = NULL;

	m_chatView = NULL;
	m_chatFrame = NULL;
	m_chatTarget = NULL;
	m_chatInput = NULL;
	m_emoticonButton = NULL;
	m_language = NULL;
	m_reportButton = NULL;
	m_emoticonDlg = NULL;
	m_reportDlg = NULL;
	m_noteDlg = NULL;
	m_addFriendDlg = NULL;
	m_dlg49C = NULL;
	m_passwordDlg = NULL;
	m_tradeDlg = NULL;
	m_jukeBoxDlg = NULL;
	m_avatarTipDlg = NULL;
	m_userListDlg = NULL;
	m_chatLogDlg = NULL;
	m_userListButton = NULL;
	m_tipButton = NULL;
	m_chatLogButton = NULL;

	m_rightMenu = NULL;
}

// avatar_mainui.cpp:323
void ntAvatarChatMain::OnAvatarChatMainInit(int) {
}

// avatar_mainui.cpp:327
void ntAvatarChatMain::OnAvatarChatMainFinish(void) {
	m_titleArea->SetBgImg("title_chat");
}

// avatar_mainui.cpp:333
void ntAvatarChatMain::OnPortraitInit(intptr_t param) {
	m_portrait = DYNAMIC_CAST(FrArea, param);
	if (m_portrait) {
		WRect rect = m_portrait->GetRect();
		m_portrait->MoveWindow(WPoint(rect.x, rect.y + m_uiOffsetY));
	}

	if (WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
		int index = WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar->m_charTypeId & 0x3FFFFFF;
		if (index >= 0 && index < sizeof(m_portraitBitmaps) / sizeof(m_portraitBitmaps[0]) && m_portrait) {
			m_portrait->SetBgImg(m_portraitBitmaps[index]);
		}
	}
}

// avatar_mainui.cpp:347
void ntAvatarChatMain::OnPortraitMaskAreaInit(int param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (pArea) {
		pArea->SetVisible(true);
		WRect rect = pArea->GetRect();
		pArea->MoveWindow(WPoint(rect.x, rect.y + m_uiOffsetY));
	}
}

// avatar_mainui.cpp:358
void ntAvatarChatMain::OnPortraitMaskInit(int param) {
	m_portraitMask = DYNAMIC_CAST(FrButton, param);
	if (m_portraitMask) {
		m_portraitMask->SetVisible(true);
		WRect rect = m_portraitMask->GetRect();
		m_portraitMask->MoveWindow(WPoint(rect.x, rect.y + m_uiOffsetY));
	}
}

// avatar_mainui.cpp:369
void ntAvatarChatMain::OnPortraitMaskBtnUp(void) {
	if (WSingleton<CChatMsg>::Instance()->IsClosed()) {
		WSingleton<CChatMsg>::Instance()->Open(true, true);
	} else {
		WSingleton<CChatMsg>::Instance()->Close(true, true);
	}
}

// avatar_mainui.cpp:378
void ntAvatarChatMain::OnPortraitTipButtonInit(int param) {
	m_tipButton = DYNAMIC_CAST(FrButton, param);
	if (m_tipButton) {
		WRect rect = m_tipButton->GetRect();
		m_tipButton->MoveWindow(WPoint(rect.x, rect.y + m_uiOffsetY));

		if (WSingleton<COption>::Instance()->gGetAvatarNewBie()) {
			OnPortraitTipButtonBtnUp();
		}
	}
}

// avatar_mainui.cpp:390
bool ntAvatarChatMain::OnAvatarTipDlgResult(int, FrForm*) {
	if (m_tipButton) {
		m_tipButton->SetStatus(FrButton::NORMAL);
	}
	m_avatarTipDlg = NULL;
	return true;
}

// avatar_mainui.cpp:400
void ntAvatarChatMain::OnPortraitTipButtonBtnUp(void) {
	if (m_avatarTipDlg) {
		m_avatarTipDlg->Close(true);
		return;
	}

	if (m_tipButton) {
		m_tipButton->SetStatus((FrButton::eButMode)2);

		if (WSingleton<CChatMsg>::Instance()->IsOpened()) {
			WSingleton<CChatMsg>::Instance()->Close(true, true);
		}

		m_avatarTipDlg = CreateForm<FrAvatarTipDlg>(g_pFresh->GetManager(), this, "avatartip", 0);
		m_avatarTipDlg->EnableDrag(false);
		m_avatarTipDlg->SetFixed(true);
		m_avatarTipDlg->Open((FRESH_RESULT_CALLBACK)&ntAvatarChatMain::OnAvatarTipDlgResult, WPoint(10.0f, 65.0f), 0x15);
		m_avatarTipDlg->SetIconRect(m_tipButton->GetRect());
	}
}

// avatar_mainui.cpp:429
void ntAvatarChatMain::OnPortraitJukeBoxButtonAreaInit(int param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (pArea) {
		pArea->SetVisible(true);
		WRect rect = pArea->GetRect();
		pArea->MoveWindow(WPoint(rect.x, rect.y + m_uiOffsetY));
	}
}

// avatar_mainui.cpp:441
void ntAvatarChatMain::OnPortraitJukeBoxButtonInit(int param) {
	m_jukeBoxButton = DYNAMIC_CAST(FrButton, param);
	if (m_jukeBoxButton) {
		WRect rect = m_jukeBoxButton->GetRect();
		m_jukeBoxButton->MoveWindow(WPoint(rect.x, rect.y + m_uiOffsetY));
		m_jukeBoxButton->SetPushDelay(0.0f);

		m_jukeBoxButton->SetButtonImg(FrJukeBoxDlg::m_preserve_isPlaying ? "btn_jukebox_o" : "btn_jukebox_n", FrButton::NORMAL);
	}
}

// avatar_mainui.cpp:453
void ntAvatarChatMain::OnPortraitJukeBoxButtonBtnUp(void) {
	ToggleJukeBox();
}

// avatar_mainui.cpp:458
bool ntAvatarChatMain::OnJukeBoxDlgResult(int, FrForm*) {
	if (m_jukeBoxButton) {
		m_jukeBoxButton->SetStatus(FrButton::NORMAL);
	}
	m_jukeBoxDlg = NULL;
	return true;
}

// avatar_mainui.cpp:468
void ntAvatarChatMain::ToggleJukeBox(void) {
	if (m_jukeBoxDlg) {
		m_jukeBoxDlg->Close(true);
		return;
	}

	if (m_jukeBoxButton) {
		m_jukeBoxDlg = CreateForm<FrJukeBoxDlg>(g_pFresh->GetManager(), this, "jukeboxdlg", 0);
		m_jukeBoxDlg->Open((FRESH_RESULT_CALLBACK)&ntAvatarChatMain::OnJukeBoxDlgResult, 0x11);
		m_jukeBoxDlg->SetFixed(true);
		m_jukeBoxDlg->SetIconRect(m_jukeBoxButton->GetRect());
	}
}

// avatar_mainui.cpp:490
void ntAvatarChatMain::OnPortraitChatLogButtonInit(int param) {
	m_chatLogButton = DYNAMIC_CAST(FrButton, param);
	if (m_chatLogButton) {
		WRect rect = m_chatLogButton->GetRect();
		m_chatLogButton->MoveWindow(WPoint(rect.x, rect.y + m_uiOffsetY));
		m_chatLogButton->SetPushDelay(0.0f);
	}
}

// avatar_mainui.cpp:501
void ntAvatarChatMain::OnPortraitChatLogButtonBtnUp(void) {
	ToggleAvatarChatLog();
}

// avatar_mainui.cpp:506
bool ntAvatarChatMain::OnAvatarChatLogDlgResult(int, FrForm*) {
	if (m_chatLogButton) {
		m_chatLogButton->SetStatus(FrButton::NORMAL);
	}
	m_chatLogDlg = NULL;
	return true;
}

// avatar_mainui.cpp:516
void ntAvatarChatMain::ToggleAvatarChatLog(void) {
	if (m_chatLogDlg) {
		m_chatLogDlg->Close(true);
		return;
	}

	if (m_chatLogButton) {
		m_chatLogDlg = CreateForm<FrAvatarChatLogDlg>(g_pFresh->GetManager(), this, "avatarchatlogdlg", 0);
		m_chatLogDlg->SetFixed(true);
		m_chatLogDlg->Open((FRESH_RESULT_CALLBACK)&ntAvatarChatMain::OnAvatarChatLogDlgResult, 0x15);
		m_chatLogDlg->SetIconRect(m_chatLogButton->GetRect());

		if (!WSingleton<CChatMsg>::Instance()->IsClosed()) {
			WSingleton<CChatMsg>::Instance()->Close(true, true);
		}
	}
}

// avatar_mainui.cpp:542
void ntAvatarChatMain::OnAvatarUserListButtonInit(int param) {
	m_userListButton = DYNAMIC_CAST(FrButton, param);
	if (m_userListButton) {
		WRect rect = m_userListButton->GetRect();
		m_userListButton->MoveWindow(WPoint(g_view->GetWidth() - 38.0f, rect.y));
		m_userListButton->SetPushDelay(0.5f);
	}
}

// avatar_mainui.cpp:552
void ntAvatarChatMain::OnAvatarUserListButtonBtnUp(void) {
	ToggleAvatarUserList();
}

// avatar_mainui.cpp:557
bool ntAvatarChatMain::OnAvatarUserListDlgResult(int, FrForm*) {
	if (m_userListButton) {
		m_userListButton->SetStatus(FrButton::NORMAL);
	}
	m_userListDlg = NULL;
	return true;
}

// avatar_mainui.cpp:567
void ntAvatarChatMain::ToggleAvatarUserList(void) {
	if (m_userListDlg) {
		m_userListDlg->Close(true);
		return;
	}

	if (m_userListButton) {
		m_userListDlg = CreateForm<FrAvatarUserListDlg>(g_pFresh->GetManager(), this, "avataruserlistdlg", 0);

		WPoint pt(g_view->GetWidth() - 252.0f, 108.0f);

		m_userListDlg->EnableDrag(false);
		m_userListDlg->SetFixed(true);
		m_userListDlg->Open((FRESH_RESULT_CALLBACK)&ntAvatarChatMain::OnAvatarUserListDlgResult, pt, 0x15);
		m_userListDlg->SetIconRect(m_userListButton->GetRect());
		m_userListDlg->UpdateUserList();
	}
}

// avatar_mainui.cpp:595
void ntAvatarChatMain::OnChatFrameInit(int param) {
	m_chatFrame = DYNAMIC_CAST(FrFrame, param);
	if (m_chatFrame) {
		m_chatFrame->SetVisible(WSingleton<CChatMsg>::Instance()->IsOpened());

		WRect rect = m_chatFrame->GetRect();
		m_chatFrame->MoveWindow(WPoint(rect.x, rect.y + m_uiOffsetY));
	}
}

// avatar_mainui.cpp:627
void ntAvatarChatMain::OnChatTargetInit(intptr_t param) {
	m_chatTarget = DYNAMIC_CAST(FrComboBox, param);
	if (m_chatTarget) {
		m_chatTarget->SetVisible(WSingleton<CChatMsg>::Instance()->IsOpened());
		CSharedDoc::Instance()->InitReservedChatList(m_chatTarget, (eReservedChatPartner)2);

		WRect rect = m_chatTarget->GetRect();
		m_chatTarget->MoveWindow(WPoint(rect.x, rect.y + m_uiOffsetY));
	}
}

// avatar_mainui.cpp:642
void ntAvatarChatMain::OnChatTargetBtnDown(int) {
	if (m_chatInput) {
		m_chatInput->SetKeyFocus(true);
	}
}

// avatar_mainui.cpp:648
bool ntAvatarChatMain::OnChatTargetEnterKey(int) {
	if (m_chatTarget && strlen(m_chatTarget->GetLine(1, false)) == 0) {
		m_chatTarget->SetLine(1, "\xB8\xF0\xB5\xCE\xBF\xA1\xB0\xD4", 0, false, 0); // "to everyone"
	}

	if (m_chatInput) {
		m_chatInput->SetKeyFocus(true);
	}

	return false;
}

// avatar_mainui.cpp:660
void ntAvatarChatMain::OnChatInputInit(intptr_t param) {
	m_chatInput = DYNAMIC_CAST(FrEdit, param);
	if (m_chatInput) {
		m_chatInput->SetVisible(WSingleton<CChatMsg>::Instance()->IsOpened());

		WRect rect = m_chatInput->GetRect();
		m_chatInput->MoveWindow(WPoint(rect.x, rect.y + m_uiOffsetY));
	}
}

// avatar_mainui.cpp:716
void ntAvatarChatMain::OnEmoticonInit(intptr_t param) {
	m_emoticonButton = DYNAMIC_CAST(FrButton, param);
	if (m_emoticonButton) {
		m_emoticonButton->SetVisible(WSingleton<CChatMsg>::Instance()->IsOpened());

		WRect rect = m_emoticonButton->GetRect();
		m_emoticonButton->MoveWindow(WPoint(rect.x, rect.y + m_uiOffsetY));
	}
}

// avatar_mainui.cpp:727
void ntAvatarChatMain::OnEmoticonBtnUp(void) {
	if (m_emoticonDlg == NULL) {
		m_emoticonDlg = CreateForm<FrEmoticonDlg>(g_pFresh->GetManager(), this, "emoticon", 0);
		if (m_emoticonDlg == NULL) {
			return;
		}

		WPoint pt(0.0f, 0.0f);
		if (m_emoticonButton) {
			const WRect& rect = m_emoticonButton->GetRect();
			pt.x = rect.x - 200.0f;
			pt.y = rect.y - 260.0f;
		}

		if (!g_pFresh->GetManager()->IsKeyExclusive()) {
			g_pFresh->GetManager()->SetExclusiveKey(true);
		}

		m_emoticonDlg->Open((FRESH_RESULT_CALLBACK)&ntAvatarChatMain::OnEmoticonResult, pt, 3);
	} else {
		m_emoticonDlg->Close(FrCANCEL, true);
	}
}

// avatar_mainui.cpp:799
void ntAvatarChatMain::OnLanguageInit(intptr_t param) {
	m_language = DYNAMIC_CAST(FrArea, param);
	if (m_language) {
		m_language->SetVisible(WSingleton<CChatMsg>::Instance()->IsOpened());

		if (g_ime->IsAlphaNumericMode()) {
			m_language->SetBgImg("chat_eng");
		} else {
			m_language->SetBgImg("chat_kor");
		}

		WRect rect = m_language->GetRect();
		m_language->MoveWindow(WPoint(rect.x, rect.y + m_uiOffsetY));
	}
}

// avatar_mainui.cpp:816
void ntAvatarChatMain::OnReportInit(int param) {
	m_reportButton = DYNAMIC_CAST(FrButton, param);
	if (m_reportButton) {
		m_reportButton->SetVisible(WSingleton<CChatMsg>::Instance()->IsOpened());

		WRect rect = m_reportButton->GetRect();
		m_reportButton->MoveWindow(WPoint(rect.x, rect.y + m_uiOffsetY));
	}
}

// avatar_mainui.cpp:827
void ntAvatarChatMain::OnReportBtnUp(void) {
	m_reportDlg = CreateForm<FrReportDlg>(g_pFresh->GetManager(), this, "report", 0);

	std::string text;
	text = "1. \xB4\xEB\xC8\xAD\xC3\xA2\xC0\xC7 \xC1\xF6\xB3\xAD \xB3\xBB\xBF\xEB\xB5\xB5 \xB8\xF0\xB5\xCE \xBC\xAD\xB9\xF6\xBF\xA1 \xB1\xE2\xB7\xCF\xB5\xCB\xB4\xCF\xB4\xD9.\n"; // all chat is logged on the server
	text += "2. \xC1\xB6\xC0\xDB, \xC7\xE3\xC0\xA7, \xB9\xDD\xBA\xB9\xBD\xC5\xB0\xED\xB4\xC2 \xC3\xB3\xB9\xFA\xC0\xBB \xB9\xDE\xBD\xC0\xB4\xCF\xB4\xD9.\n"; // false or repeated reports are punished
	text += "3. \xBD\xC5\xB0\xED\xC7\xCF\xBD\xC5 \xB3\xBB\xBF\xEB\xC0\xBA 48\xBD\xC3\xB0\xA3 \xC0\xCC\xB3\xBB\xBF\xA1 \xC3\xB3\xB8\xAE\xB5\xCB\xB4\xCF\xB4\xD9."; // reports are handled within 48 hours

	m_reportDlg->SetMessage(text.c_str(), false);
	m_reportDlg->Open((FRESH_RESULT_CALLBACK)&ntAvatarChatMain::OnReportResult, 3);
}

// avatar_mainui.cpp:841
bool ntAvatarChatMain::OnReportResult(int, FrForm*) {
	m_reportDlg = NULL;
	return true;
}

// avatar_mainui.cpp:849
void ntAvatarChatMain::OnRightMenuInit(int param) {
	m_rightMenu = DYNAMIC_CAST(FrWnd, param);
	if (m_rightMenu) {
		m_rightMenu->SetVisible(false);
		m_rightMenu->SetFixed(true);
	}
}

// avatar_mainui.cpp:860
void ntAvatarChatMain::OnNoteToFriend(void) {
	if (CSharedDoc::Instance()->m_myPang < 10) {
		// "Sending a note needs %d pang."
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		pForm->SetMessage(MakeStr("\xC2\xCA\xC1\xF6\xB8\xA6 \xBA\xB8\xB3\xBB\xB1\xE2 \xC0\xA7\xC7\xD8\xBC\xAD\xB4\xC2 %d\xC6\xCE\xC0\xCC \xC7\xCA\xBF\xE4\xC7\xD5\xB4\xCF\xB4\xD9.", 10), false);
		pForm->SetTimeLimit(5.0f);
		pForm->SetFixed(true);

		WPoint pt((g_view->GetWidth() - pForm->GetRect().w) * 0.5f, (g_view->GetHeight() - pForm->GetRect().h) * 0.5f);
		pForm->Open((FRESH_RESULT_CALLBACK)0, pt, 3);
		return;
	}

	ntAvatar* pAvatar = WSingleton<ntAvatarManager>::Instance()->FindAvatarByNickName(m_targetNickName);
	if (m_noteDlg == NULL && pAvatar) {
		m_noteDlg = CreateForm<FrNoteDlg>(g_pFresh->GetManager(), this, "note", 0);
		m_noteDlg->SetNick(pAvatar->m_nickName, 0);
		m_noteDlg->SetFixed(true);

		WPoint pt((g_view->GetWidth() - m_noteDlg->GetRect().w) * 0.5f, (g_view->GetHeight() - m_noteDlg->GetRect().h) * 0.5f);
		m_noteDlg->Open((FRESH_RESULT_CALLBACK)&ntAvatarChatMain::OnNoteDlgResult, pt, 1);

		WRect rect = m_noteDlg->GetRect();
		m_noteDlg->Adjust(rect);
		m_noteDlg->MoveWindow(WPoint(rect.x, rect.y));
	}

	if (!g_pFresh->GetManager()->IsKeyExclusive()) {
		g_pFresh->GetManager()->SetExclusiveKey(true);
	}
}

// avatar_mainui.cpp:893
bool ntAvatarChatMain::OnNoteDlgResult(int result, FrForm*) {
	if (result == 1 && WSingleton<ntAvatarManager>::Instance()->FindAvatarByNickName(m_targetNickName)) {
		WSendPacket packet((enumClientPacket)0x3C);
		packet.Encode2(0x111);

		packet.Encode4(m_targetUid);
		packet.EncodeStr(std::string(m_noteDlg->GetNote()));
		packet.Encode1(0);
		packet.Send(SEND_TO_DEFAULT);
	}

	m_noteDlg = NULL;
	return true;
}

// avatar_mainui.cpp:912
void ntAvatarChatMain::OnAddFriend(void) {
	ntAvatar* pAvatar = WSingleton<ntAvatarManager>::Instance()->FindAvatarByNickName(m_targetNickName);
	if (pAvatar) {
		if (!WSingleton<CMessengerInfo>::Instance()->IsVisible()) {
			WSingleton<CMessengerInfo>::Instance()->Open(true);
			if (WSingleton<CMessengerInfo>::Instance()->IsVisible() == true) {
				FrMessengerDlg* pDlg = WSingleton<CMessengerInfo>::Instance()->GetMessengerDlg();
				if (pDlg) {
					pDlg->OpenAddFriendNickDlg();
					WSingleton<CMessengerInfo>::Instance()->SetTargetNick(pAvatar->m_nickName);
				}
			}
		}
	}

	if (!g_pFresh->GetManager()->IsKeyExclusive()) {
		g_pFresh->GetManager()->SetExclusiveKey(true);
	}
}

// avatar_mainui.cpp:945
bool ntAvatarChatMain::OnAddFriendDlgResult(int, FrForm*) {
	m_addFriendDlg = NULL;
	return true;
}

// avatar_mainui.cpp:951
void ntAvatarChatMain::OnAvatarInfo(void) {
	ntAvatar* pAvatar = WSingleton<ntAvatarManager>::Instance()->FindAvatarByNickName(m_targetNickName);
	if (pAvatar) {
		if (WSingleton<CUserInfo>::Instance()) {
			std::map<unsigned long, sBriefUserInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_userMap.find(pAvatar->m_guid);
			if (it != WSingleton<CSharedDoc>::Instance()->m_userMap.end()) {
				WSingleton<CUserInfo>::Instance()->SetInfo(it->second.id, pAvatar->m_guid, true, true, false, false, it->second.nick);
			}
		}
	}
}

// avatar_mainui.cpp:967
void ntAvatarChatMain::OnFollowAvatar(void) {
	if (WSingleton<ntAvatarManager>::Instance()->FindAvatarByNickName(m_targetNickName)
		&& (WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar->m_state & 0x7E) == 0) {
		WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar->Follow(m_targetNickName);
	}
}

// avatar_mainui.cpp:976
void ntAvatarChatMain::OnKickAvatar(void) {
	ntAvatar* pAvatar = WSingleton<ntAvatarManager>::Instance()->FindAvatarByNickName(m_targetNickName);
	if (WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar->m_canKick && pAvatar) {
		WSendPacket packet((enumClientPacket)0x26);
		packet.Encode4(pAvatar->m_uid);
		packet.Send(SEND_TO_DEFAULT);
	}
}

// avatar_mainui.cpp:987
bool ntAvatarChatMain::OnTradeDlgResult(int, FrForm*) {
	m_tradeDlg = NULL;
	return true;
}

// avatar_mainui.cpp:1122
bool ntAvatarChatMain::OnPasswordDlgResult(int result, FrForm*) {
	if (result == 1) {
		WSingleton<ntAvatarManager>::Instance()->FindAvatarByNickName(m_targetNickName);
		if (m_passwordDlg) {
			FrWnd* pWnd = m_passwordDlg->FindChildByName("password");
			FrEdit* pEdit = DYNAMIC_CAST(FrEdit, pWnd);

			std::string password;
			if (pEdit) {
				const char* text = pEdit->GetLine(1, false);
				password = text;
				this << MsgObject(0, 0x27, (intptr_t)text, 0, 0, 0, 0);
			}

			FrWnd* pOk = m_passwordDlg->FindChildByName("ok");
			if (pOk) {
				pOk->Enable(false);
			}

			WSendPacket packet((enumClientPacket)9);
			packet.Encode2(CSharedDoc::Instance()->m_roomInfo.m_roomNumber);
			packet.EncodeStr(password);
			packet.Send(SEND_TO_DEFAULT);

			this << MsgObject(0, 0x16, 0, 0, 0, 0, 0);
			this << MsgObject(0, 1, 0, 0, 0, 0, 0);
		}
	}

	m_passwordDlg = NULL;
	return true;
}

// avatar_mainui.cpp:1245
void ntAvatarChatMain::OnRightMenuRBtnDown(void) {
	if (m_rightMenu) {
		m_rightMenu->SetVisible(false);
	}
}

// avatar_mainui.cpp:1323
void ntAvatarChatMain::PopupRightMenu(int x, int y) {
	if (m_rightMenu) {
		m_rightMenu->MoveWindow(WPoint((float)x, (float)y));
		m_rightMenu->SetVisible(true);
		m_rightMenu->SetTopmost(true);
		UpdateRightMenu();
	}
}

// avatar_mainui.cpp:1340
void ntAvatarChatMain::ChangePortraitImage(int index) {
	if (index >= 0 && index < sizeof(m_portraitBitmaps) / sizeof(m_portraitBitmaps[0])) {
		if (m_portrait) {
			m_portrait->SetBgImg(m_portraitBitmaps[index]);
		}
	}
}

// avatar_mainui.cpp:1353
void ntAvatarChatMain::CloseAllDialogs(void) {
	if (m_avatarTipDlg) {
		m_avatarTipDlg->Close(true);
	}
	if (m_userListDlg) {
		m_userListDlg->Close(true);
	}
	if (m_chatLogDlg) {
		m_chatLogDlg->Close(true);
	}
	CTaskMain::CloseAllDialogs();
}

// avatar_mainui.cpp:1367
void ntAvatarChatMain::OnUnderBar_ShopUp(void) {
	if (WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
		WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar->Stop();
	}
	CTaskMain::OnUnderBar_ShopUp();
}

// avatar_mainui.cpp:1374
void ntAvatarChatMain::OnUnderBar_MyRoomUp(void) {
	if (WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
		WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar->Stop();
	}
	CTaskMain::OnUnderBar_MyRoomUp();
}

// avatar_mainui.cpp:1381
void ntAvatarChatMain::OnUnderBar_CookieUp(void) {
	if (WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
		WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar->Stop();
	}
	CTaskMain::OnUnderBar_CookieUp();
}

// avatar_mainui.cpp:1388
void ntAvatarChatMain::OnUnderBar_RankingUp(void) {
	if (WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
		WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar->Stop();
	}
	CTaskMain::OnUnderBar_RankingUp();
}
