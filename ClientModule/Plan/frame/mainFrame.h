#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

typedef htmlcxx::HTML::Node htmlcxxNode;
typedef tree<htmlcxx::HTML::Node> htmlcxxNodeTree;

class MainFrame : public WindowImplBase
{
public:
	~MainFrame();
	static MainFrame*	GetInstance();
	void				CreateFrame();

public:
	void				InitWindow(){}
	void				InitData(TNotifyUI& msg);
	CControlUI*			CreateControl(LPCTSTR pstrClassName);
	LRESULT				OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void				OnFinalMessage(HWND hWnd);
	LRESULT				OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	UINT				GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	LRESULT				HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void				Notify(TNotifyUI& msg);

public:
	void				RequesLineList();
	void				InsertLineHead();
	void				InsertLineBody();
	void				AdaptHeaderWidth(bool bRestore);

	void				_ParseHTMLString(std::string& strValue, char* pTag, char* pKey, char* pKeyValue);
	void				_ParseLineData(htmlcxxNodeTree nodeTree, htmlcxxNodeTree::iterator& it_begin, htmlcxxNodeTree::iterator& it_end);

protected:

	CDuiString			GetSkinFile();
	CDuiString			GetSkinFolder();
	UILIB_RESOURCETYPE	GetResourceType() const;
	LPCTSTR				GetResourceID() const;
	LPCTSTR				GetWindowClassName(void) const{ return _T("MainFrame"); }
	LRESULT				ResponseDefaultKeyEvent(WPARAM wParam);

private:
	MainFrame();
	static MainFrame*	g_instance;

private:
	LibcurlHelper*		m_pLibcurlHelper;	// libcurl¶ÌÁ¬½Ó¿â¸¨Öú

	CRichEditUI*		m_pEdtWebSite;		// ÍøÕ¾
	CRichEditUI*		m_pEdtSafeCode;		// °²È«Âë

	std::vector<std::string> m_vLineName;
	std::vector<std::vector<std::string>> m_vLineWebSite;
};

#endif //__MAIN_FRAME_H__