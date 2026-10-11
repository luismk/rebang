// Original: d:\build\custom\temp\code\client\projectg\ghostmanager.h
#ifndef PG_CLIENT_PROJECTG_GHOSTMANAGER_H
#define PG_CLIENT_PROJECTG_GHOSTMANAGER_H

#include <string>

#include "..\..\defines.h"
#include "singleton.h"

class CGhostDocument;
struct GHOST_GAME_BRIEF;

// CGhostManager owns the current ghost document.
class CGhostManager : public WSingleton<CGhostManager> {
	LAYOUT_CHECK_FRIEND;
public:
	CGhostManager(void);
	virtual ~CGhostManager(void);

	CGhostDocument *GetDocument(void);
	void ReleaseDocument(void);
	void LoadDocument(const std::string &fileName);
	void LoadDocumentFromLocalFile(const std::string &fileName);
	void SaveDocument(const std::string &fileName);

	// ghostmanager.h:16
	void SetRecentSelectedGhostNick(const std::string &nick) { m_recentSelectedGhostNick = nick; }
	// ghostmanager.h:20
	const GHOST_GAME_BRIEF *GetLatestGameBrief(void) const { return m_pLatestGameBrief; }
	// ghostmanager.h:21
	const std::string &GetLatestFileName(void) const { return m_latestFileName; }

private:
	CGhostDocument *m_pDocument;                // 0x28
	std::string m_recentSelectedGhostNick;      // 0x2c
	std::string m_latestFileName;               // 0x48
	GHOST_GAME_BRIEF *m_pLatestGameBrief;       // 0x64
};

CHECK_SIZE(CGhostManager, 0x68);
BEGIN_LAYOUT_CHECK(CGhostManager)
	CHECK_FIELD(CGhostManager, m_pDocument, 0x28);
	CHECK_FIELD(CGhostManager, m_recentSelectedGhostNick, 0x2c);
	CHECK_FIELD(CGhostManager, m_latestFileName, 0x48);
	CHECK_FIELD(CGhostManager, m_pLatestGameBrief, 0x64);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GHOSTMANAGER_H
