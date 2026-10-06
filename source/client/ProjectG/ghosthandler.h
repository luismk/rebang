#pragma once
struct GHOST_GAME_BRIEF;
class CGhostHandler : public FrCmdTarget, public WSingleton<CGhostHandler>
{
public:
	CGhostHandler();
	virtual ~CGhostHandler();

	bool IsAvailableGhostSystem();
	void RequestGhostGameList(unsigned long uid);
	bool OpenEventGhostGameDlg();
	bool IsExistEventGhost();
	void OpenNotifyDlg(int type);
	void OnGhostPacket(WReceivedPacket& packet);
	void LoadingComplete(bool bSuccess);
	void UploadComplete(bool bSuccess, const std::string& fileName);
	void RecordComplete();
	void ExceptionFinishGame();
	void SendErrorReport(const std::string& error, const std::string& fileName);
	void ClearLocalGhostFiles();

private:
	void OpenGhostGameDlg(unsigned long uid, const std::string& nick,
		const std::list<GHOST_GAME_BRIEF>& gameList,
		bool (FrCmdTarget::*callback)(int, FrForm*));
	bool OnResultGhostGameDlg(int result, FrForm* form);
	bool OnResultEventGhostGameDlg(int result, FrForm* form);
};
