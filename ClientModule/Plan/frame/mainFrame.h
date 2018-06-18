#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

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

protected:

	CDuiString			GetSkinFile();
	CDuiString			GetSkinFolder();
	UILIB_RESOURCETYPE	GetResourceType() const;
	LPCTSTR				GetResourceID() const;
	LPCTSTR				GetWindowClassName(void) const{ return _T("MainFrame"); }
	LRESULT				ResponseDefaultKeyEvent(WPARAM wParam);

private:
	MainFrame();
	static MainFrame* g_instance;

private:
	CRichEditUI*		m_pEdtWebSite;		// ÍøÕ¾
	CRichEditUI*		m_pEdtSafeCode;		// °²È«Âë
};

#endif //__MAIN_FRAME_H__