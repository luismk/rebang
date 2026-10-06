#pragma once
class CGhostDocument;
struct GHOST_GAME_BRIEF;
class CGhostManager : public WSingleton<CGhostManager>
{
public:
	CGhostManager();
	virtual ~CGhostManager();
	CGhostDocument* GetDocument();
	void ReleaseDocument();
	void SetRecentSelectedGhostNick(const std::string& nick)
	{
		m_recentSelectedGhostNick = nick;
	}
	void LoadDocument(const std::string& fileName);
	void LoadDocumentFromLocalFile(const std::string& fileName);
	void SaveDocument(const std::string& fileName);
	const GHOST_GAME_BRIEF* GetLatestGameBrief() const
	{
		return m_pLatestGameBrief;
	}
	const std::string& GetLatestFileName() const { return m_latestFileName; }

private:
	CGhostDocument* m_pDocument;
	std::string m_recentSelectedGhostNick;
	std::string m_latestFileName;
	GHOST_GAME_BRIEF* m_pLatestGameBrief;
};
