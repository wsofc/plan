#include "stdafx.h"
#include "MainFrame.h"

extern CStaticLogger		g_Logger;
extern Plan::Resource*		R;
extern Plan::Util*			g_Util;

MainFrame* MainFrame::g_instance = nullptr;
MainFrame::MainFrame()
{ }

MainFrame::~MainFrame()
{ }

MainFrame* MainFrame::GetInstance()
{
	if (MainFrame::g_instance == nullptr)
	{
		MainFrame::g_instance = new (std::nothrow)MainFrame();
	}
	return MainFrame::g_instance;
}

void MainFrame::CreateFrame()
{
	if (FALSE == IsWindow(m_hWnd))
	{
		int style = UI_CLASSSTYLE_DIALOG | WS_MINIMIZEBOX;
		m_hWnd = this->Create(NULL, R->MainWndTitleName, style, 0L);

		this->CenterWindow();
		this->ShowWindow(true);
	}
	else
	{
		this->ShowWindow(true);
	}
}

CDuiString MainFrame::GetSkinFile()
{
	return _T("main.xml");
}

CDuiString MainFrame::GetSkinFolder()
{
	return _T("");
}

UILIB_RESOURCETYPE MainFrame::GetResourceType() const
{
	return Plan::Resource::g_ResourceType;
}

LPCTSTR MainFrame::GetResourceID() const
{
	return Plan::Resource::g_ResourceID;
}

LRESULT MainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		//CControlUI* pFocusCtl = m_PaintManager.GetFocus();
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	return FALSE;
}

void MainFrame::InitData(TNotifyUI& msg)
{
	g_Logger->TryInfo(_T("MainFrame - Open Complete"));

	// 设置窗口图标
	this->SetIcon(IDI_PLAN);
}

CControlUI* MainFrame::CreateControl(LPCTSTR pstrClassName)
{
	//if (_tcsicmp(pstrClassName, DUI_CTR_WKEWEBKIT) == 0)
	//{
	//	return new CWkeWebkitUI();
	//}

	return NULL;
}

void MainFrame::InitWindow()
{
	g_Logger->TryInfo(_T("MainFrame - Open"));
}

LRESULT MainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (CMW_CLOSE_SURE == wParam)
	{
		return __super::OnClose(uMsg, wParam, lParam, bHandled);
	}
	else
	{
		g_Util->SafeExit();
	}

	return FALSE;
}

void MainFrame::OnFinalMessage(HWND hWnd)
{
	g_Logger->TryInfo(_T("MainFrame - Close"));
	__super::OnFinalMessage(hWnd);

	SAFE_DELETE(g_instance);
}

LRESULT MainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
		case WM_CHAR:
		case WM_IME_CHAR:
		{
		}break;

		case WM_KEYDOWN:
		{
			if (wParam == VK_DELETE || wParam == VK_BACK)
			{
			}
		}break;

		default:
			break;
	}
	
	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

void MainFrame::Notify(TNotifyUI& msg)
{
	CDuiString& type = msg.sType;
	CDuiString& sName = msg.pSender->GetName();
	if (_tcscmp(type, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		this->InitData(msg);
	}
	else if (_tcscmp(msg.sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcscmp(sName, R->kBtnMain_Close) == 0)			// 关闭
		{
			this->Close();
		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_ITEMSELECT) == 0)
	{
	}
	else if (_tcscmp(type, DUI_MSGTYPE_DROPDOWN) == 0)
	{
	}
}