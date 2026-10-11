// Original: d:\build\custom\temp\code\client\projectg\newsreader.cpp
#include "NewsReader.h"
#include "tinyxml.h"

// newsreader.cpp:18
NewsReader::NewsReader(void) {
}

// newsreader.cpp:23
NewsReader::~NewsReader(void) {
	Clear();
}

// newsreader.cpp:28
unsigned int NewsReader::Load(const char* fileName, unsigned int maxArticles) {
	Clear();

	TiXmlDocument doc;
	if (!doc.LoadFile(fileName)) {
		return 0;
	}

	unsigned int count = 0;
	TiXmlNode* pRss = doc.FirstChild("rss");
	if (!pRss) {
		return 0;
	}

	TiXmlNode* pNotice = pRss->FirstChild("notice");

	while (pNotice) {
		sArticle article;
		GetChildNodeText(pNotice, "title", article.m_title);
		GetChildNodeText(pNotice, "url", article.m_url);
		GetChildNodeText(pNotice, "content", article.m_content);
		GetChildNodeText(pNotice, "RegDate", article.m_regDate);
		GetChildNodeText(pNotice, "Cate", article.m_category);
		GetChildNodeText(pNotice, "Userid", article.m_userId);

		m_articles.push_back(article);
		++count;

		if (maxArticles && count >= maxArticles) {
			break;
		}

		pNotice = pNotice->NextSibling();
	}

	return count;
}

// newsreader.cpp:64
unsigned int NewsReader::GetNumArticles(void) const {
	return m_articles.size();
}

// newsreader.cpp:69
const NewsReader::sArticle* NewsReader::GetArticle(unsigned int index) const {
	if (index < m_articles.size()) {
		return &m_articles[index];
	}
	return 0;
}

// newsreader.cpp:77
void NewsReader::Clear(void) {
	m_articles.clear();
}

// newsreader.cpp:83
bool NewsReader::GetChildNodeText(const TiXmlNode* pNode, const char* name, std::string& text) {
	if (!pNode || !name) {
		return false;
	}

	TiXmlNode* pChild = pNode->FirstChild(name);

	if (!pChild) {
		return false;
	}
	TiXmlNode* pText = pChild->FirstChild();

	if (!pText) {
		return false;
	}

	text = pText->Value();
	return true;
}
