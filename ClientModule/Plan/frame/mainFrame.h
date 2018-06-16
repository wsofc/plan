#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

class MainFrame : public WindowImplBase
{
public:
	~MainFrame();
	static MainFrame*	GetInstance();
	void				CreateFrame();

public:
	void				InitData(TNotifyUI& msg);
	CControlUI*			CreateControl(LPCTSTR pstrClassName);
	void				InitWindow();
	LRESULT				OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void				OnFinalMessage(HWND hWnd);
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

};

#endif //__MAIN_FRAME_H__