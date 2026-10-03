#pragma once

#include <string>

class CIconInterface
{
public:
	CIconInterface() { }
	virtual ~CIconInterface() { }

	virtual void OnInit(FrButton& button) = 0;
	virtual void OnButtonDown() = 0;
	virtual void SetInformation(std::string& normal, std::string& over,
		std::string& selected, std::string& qaUrl, std::string& url)
	{
	}
	virtual void SetIconImage(FrButton& button) { }
};

class CTopIcon : public CIconInterface
{
public:
	CTopIcon() { }
	virtual ~CTopIcon() { }
};

class CWebIcon : public CIconInterface
{
public:
	CWebIcon() { }
	virtual ~CWebIcon() { }

	virtual void SetInformation(std::string& normal, std::string& over,
		std::string& selected, std::string& qaUrl, std::string& url)
	{
		m_normalImage = normal;
		m_overImage = over;
		m_selectedImage = selected;
		m_qaUrl = qaUrl;
		m_url = url;
	}

	virtual void SetIconImage(FrButton& button)
	{
		button.SetButtonImg(m_normalImage.c_str(), FrButton::NORMAL);
		button.SetButtonImg(m_overImage.c_str(), FrButton::OVER);
		button.SetButtonImg(m_selectedImage.c_str(), FrButton::BLINK);
	}

protected:
	std::string m_normalImage;
	std::string m_overImage;
	std::string m_selectedImage;
	std::string m_url;
	std::string m_qaUrl;
};
