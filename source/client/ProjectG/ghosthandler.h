// Original: d:\build\custom\temp\code\client\projectg\ghosthandler.h
#ifndef PG_CLIENT_PROJECTG_GHOSTHANDLER_H
#define PG_CLIENT_PROJECTG_GHOSTHANDLER_H

#include <stdio.h>
#include <list>
#include <string>

#include "actor.h"
#include "commonutil.h"
#include "frcmdtarget.h"
#include "ghosttypes.h"
#include "singleton.h"

class CGhostDocument;
class MsgObject;
class FrForm;
class WReceivedPacket;

// ghosthandler.h:22
inline const char *MakeGhostGameFileName(const GHOST_GAME_BRIEF *brief) {
	return MakeStr("%d_%d_%d_%d_%d_%d.GST", brief->m_userId, brief->m_gameId, brief->m_fileNameParts[0], brief->m_fileNameParts[1], brief->m_fileNameParts[2], brief->m_fileNameParts[3]);
}

// ghosthandler.h:34
inline unsigned long GetGhostGameBriefFromFileName(GHOST_GAME_BRIEF *brief, const char *fileName) {
	sscanf(fileName, "%d_%d_%d_%d_%d_%d.GST", &brief->m_userId, &brief->m_gameId, &brief->m_fileNameParts[0], &brief->m_fileNameParts[1], &brief->m_fileNameParts[2], &brief->m_fileNameParts[3]);
	return brief->m_userId;
}

// CGhostRecorder derives from IActor (its constructor calls IActor::IActor);
// only the virtuals this unit defines so far are declared.
class CGhostRecorder : public IActor {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;

	CGhostRecorder(void);
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; } // ghosthandler.h:52
	virtual ~CGhostRecorder(void) {}                              // ghosthandler.h:56
	virtual void OnLoad(void);
	virtual void OnProcess(float) {} // ghosthandler.h:61
	virtual void OnDisplay(void) {}
	virtual void OnReload(void);
	virtual void OnInit(void);
	virtual void OnDestroy(void);

protected:
	virtual void HandleMsg(const MsgObject &msg);

private:
	bool IsSaveAvailable(void);

	CGhostDocument *m_pDocument; // 0x18
};

CHECK_SIZE(CGhostRecorder, 0x1c);
BEGIN_LAYOUT_CHECK(CGhostRecorder)
	CHECK_FIELD(CGhostRecorder, m_pDocument, 0x18);
END_LAYOUT_CHECK

// CGhostLoader: no members known yet (its constructor sets nothing).
class CGhostLoader {
public:
	CGhostLoader(void);

	bool LoadFromMemory(unsigned char *data, unsigned int size);
	bool LoadFromFile(const std::string &fileName);
};

// The ghost system's controller: a command target (owner of its dialogs'
// result callbacks) and a singleton (the constructor stores FrCmdTarget at 0
// and the WSingleton base at +4). Members past 0x2c not recovered yet, so no
// size check.
class CGhostHandler : public FrCmdTarget, public WSingleton<CGhostHandler> {
public:
	CGhostHandler(void);
	virtual ~CGhostHandler(void);

	void ClearLocalGhostFiles(void);
	void SendErrorReport(const std::string &error, const std::string &fileName);
	bool IsAvailableGhostSystem(void);
	void OpenNotifyDlg(int type);
	void LoadingComplete(bool success);
	void UploadComplete(bool success, const std::string &fileName);
	void ExceptionFinishGame(void);
	void RequestGhostGameList(unsigned long);
	bool OpenEventGhostGameDlg(void);
	void OnGhostPacket(WReceivedPacket &packet);
	bool IsExistEventGhost(void);
	void RecordComplete(void);

private:
	void OpenGhostGameDlg(unsigned long, const std::string &nick, const std::list<GHOST_GAME_BRIEF> &games, FRESH_RESULT_CALLBACK pfnResult);
	bool OnResultGhostGameDlg(int ret, FrForm *pForm);
	bool OnResultEventGhostGameDlg(int ret, FrForm *pForm);
};

#endif // PG_CLIENT_PROJECTG_GHOSTHANDLER_H
