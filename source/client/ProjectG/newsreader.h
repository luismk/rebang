// Original: d:\build\custom\temp\code\client\projectg\newsreader.h
#ifndef PG_CLIENT_PROJECTG_NEWSREADER_H
#define PG_CLIENT_PROJECTG_NEWSREADER_H

#include "..\..\defines.h"
#include <string>
#include <vector>

class TiXmlNode;

// Reads a list of news articles (six text fields each) from an XML file.
class NewsReader {
	LAYOUT_CHECK_FRIEND;

public:
	struct sArticle {
		std::string m_title;       // 0x00 <title>
		std::string m_url;         // 0x1c <url>
		std::string m_content;     // 0x38 <content>
		std::string m_regDate;     // 0x54 <RegDate>
		std::string m_category;    // 0x70 <Cate>
		std::string m_userId;      // 0x8c <Userid>
	};

	NewsReader(void);
	virtual ~NewsReader(void);

	unsigned int Load(const char* fileName, unsigned int maxArticles);
	unsigned int GetNumArticles(void) const;
	const sArticle* GetArticle(unsigned int index) const;
	void Clear(void);

protected:
	static bool GetChildNodeText(const TiXmlNode* pNode, const char* name, std::string& text);

private:
	std::vector<sArticle> m_articles;    // 0x04
};

CHECK_SIZE(NewsReader::sArticle, 0xa8);
CHECK_FIELD(NewsReader::sArticle, m_userId, 0x8c);
CHECK_SIZE(NewsReader, 0x14);
BEGIN_LAYOUT_CHECK(NewsReader)
	CHECK_FIELD(NewsReader, m_articles, 0x04);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_NEWSREADER_H
