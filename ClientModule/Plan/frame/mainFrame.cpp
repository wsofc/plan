#include "stdafx.h"
#include "mainFrame.h"
#include "lineList.h"

extern CStaticLogger		g_Logger;
extern Plan::Resource*		R;
extern Plan::Util*			g_Util;

MainFrame* MainFrame::g_instance = nullptr;
MainFrame::MainFrame()
	: m_pEdtWebSite(nullptr)
	, m_pEdtSafeCode(nullptr)
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
		int style = UI_CLASSSTYLE_DIALOG | WS_MAXIMIZEBOX;
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

	// 初始化ui
	m_pEdtWebSite = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtMain_WebSite));
	//m_pEdtWebSite->SetReadOnly(true);

	m_pEdtSafeCode = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtMain_SafeCode));
	m_PaintManager.SetFocusNeeded(m_pEdtWebSite);

	// 测试线路列表
	do 
	{
		// 先清除列表UI
		LineList* pLineList = LineList::GetInstance();
		pLineList->Clean();

		// 插入表头数据
		std::vector<stuListColumnItem*> pHeadItemArr;
		do
		{
			stuListColumnItem* pHeadItem = new stuListColumnItem();
			pHeadItem->wColumnWidth = 100;
			pHeadItem->wDataDescribe = DTP_ICON;
			sprintf_s(pHeadItem->szColumnName, "%s", "默认");
			pHeadItemArr.push_back(pHeadItem);
		} while (0);

		do
		{
			stuListColumnItem* pHeadItem = new stuListColumnItem();
			pHeadItem->wColumnWidth = 100;
			pHeadItem->wDataDescribe = DTP_LINE_NAME;
			sprintf_s(pHeadItem->szColumnName, "%s", "线路");
			pHeadItemArr.push_back(pHeadItem);
		} while (0);

		do
		{
			stuListColumnItem* pHeadItem = new stuListColumnItem();
			pHeadItem->wColumnWidth = 500;
			pHeadItem->wDataDescribe = DTP_LINE_WEBSITE;
			sprintf_s(pHeadItem->szColumnName, "%s", "会员网址");
			pHeadItemArr.push_back(pHeadItem);
		} while (0);

		for (size_t i = 0; i < pHeadItemArr.size(); ++i)
		{
			stuListColumnItem* pItem = pHeadItemArr[i];
			pLineList->InsertHeadItem(pItem);
			SAFE_DELETE(pItem);
		}pHeadItemArr.clear();

		// 加载列表控件UI
		CListUI* pListUI = static_cast<CListUI*>(m_PaintManager.FindControl(R->kLstMain_LineList));
		pLineList->LoadHeaderItem(pListUI);

		// 插入表体
		for (int i = 0; i < 5; ++i)
		{
			stuLineListBody* pBody = new stuLineListBody();
			sprintf_s(pBody->szLineName, "线路%d", i);
			sprintf_s(pBody->szLineWebSite, "%s", "www.xxxxxxxxxxyybaidu.com");
			pLineList->InsertBodyItem(20, pBody);
		}
	} while (0);
}

CControlUI* MainFrame::CreateControl(LPCTSTR pstrClassName)
{
	CDialogBuilder builder;
	CControlUI* pControl = nullptr;

	//if (_tcsicmp(pstrClassName, DUI_CTR_WKEWEBKIT) == 0)
	//{
		//pControl = static_cast<CControlUI*>(builder.Create(_T("hall/sidebar_hall.xml"), (UINT)0, this, &m_PaintManager));
	//}

	return pControl;
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

LRESULT MainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_CLOSE)
	{
		bHandled = TRUE;
		SendMessage(WM_CLOSE);
		return 0;
	}

#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed)
	{
		CControlUI* pbtnMax = static_cast<CControlUI*>(m_PaintManager.FindControl(R->kBtnMain_Max));			// 最大化按钮
		CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(R->kBtnMain_Restore));	// 还原按钮

		// 切换最大化按钮和还原按钮的状态
		if (pbtnMax && pbtnRestore)
		{
			pbtnMax->SetVisible(TRUE == bZoomed);       // 此处用表达式是为了避免编译器BOOL转换的警告
			pbtnRestore->SetVisible(FALSE == bZoomed);
		}
	}
#else
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
	if (SC_RESTORE == (wParam & 0xfff0))
	{
		bHandled = FALSE;
	}
	return lRes;
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
		if (_tcscmp(sName, R->kBtnMain_Close) == 0)					// 关闭
		{
			this->Close();
		}
		else if (_tcscmp(sName, R->kBtnMain_Min) == 0)				// 最小化
		{
			this->SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if (_tcscmp(sName, R->kBtnMain_Max) == 0)				// 最大化
		{
			this->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
		else if (_tcscmp(sName, R->kBtnMain_Restore) == 0)			// 复原
		{
			this->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else if (_tcscmp(sName, R->kBtnMain_GetLine) == 0)			// 获取线路
		{

		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_ITEMSELECT) == 0)
	{
	}
	else if (_tcscmp(type, DUI_MSGTYPE_DROPDOWN) == 0)
	{
	}
}