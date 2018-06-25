#include "stdafx.h"
#include "mainFrame.h"
#include "lineList.h"

extern CStaticLogger		g_Logger;
extern Plan::Resource*		R;
extern Plan::Util*			g_Util;
MainFrame* MainFrame::g_instance = nullptr;
MainFrame::MainFrame()
	: m_pLibcurlHelper(nullptr)
	, m_pEdtWebSite(nullptr)
	, m_pEdtSafeCode(nullptr)
{
	m_pLibcurlHelper = new LibcurlHelper();
	bool bInitCurlRes = m_pLibcurlHelper->InitOption();
	g_Logger->TryInfo(_T("MainFrame - Init Curl Option = %u"), bInitCurlRes);
}

MainFrame::~MainFrame()
{
	SAFE_DELETE(m_pLibcurlHelper);
}

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
	// 设置窗口图标
	this->SetIcon(IDI_PLAN);

	// 初始化ui
	m_pEdtWebSite = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtMain_WebSite));
	m_pEdtWebSite->SetText(_T("http://sf33.qr68.us/"));
	//m_pEdtWebSite->SetReadOnly(true);

	m_pEdtSafeCode = static_cast<CRichEditUI*>(m_PaintManager.FindControl(R->kEdtMain_SafeCode));
	m_pEdtSafeCode->SetText(_T("23366"));
	m_PaintManager.SetFocusNeeded(m_pEdtSafeCode);

	// 插入线路表头
	InsertLineHead();
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
			AdaptHeaderWidth(false);
		}
		else if (_tcscmp(sName, R->kBtnMain_Restore) == 0)			// 复原
		{
			this->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
			AdaptHeaderWidth(true);
		}
		else if (_tcscmp(sName, R->kBtnMain_GetLine) == 0)			// 获取线路
		{
			this->RequesLineList();
			this->InsertLineBody();
		}
	}
	else if (_tcscmp(type, DUI_MSGTYPE_ITEMSELECT) == 0)
	{
	}
	else if (_tcscmp(type, DUI_MSGTYPE_DROPDOWN) == 0)
	{
	}
}

void MainFrame::RequesLineList()
{
	g_Logger->TryInfo(_T("MainFrame - RequesLineList - Start..."));

	std::string url = m_pEdtWebSite->GetText();
	std::string code = m_pEdtSafeCode->GetText();

	std::string strResponce;
	TCHAR pzBuf[MAX_PATH] = { 0 };
	sprintf_s(pzBuf, _T("code=%s&submit_bt=搜索"), code.c_str());
	//strResponce = m_pLibcurlHelper->HttpPost(url.c_str(), pzBuf);
	
	// 测试
	do 
	{
		std::ifstream is;
		is.open("D:\\code\\plan\\Bin\\Debug\\html_test.txt", std::ios::binary);
		if (is.good())
		{
			int chr = 0;
			while ((chr = is.get()) != EOF)
			{
				strResponce.push_back(chr);
			}
		}
		is.close();
		is.clear();
	} while (0);
	
	_ParseHTMLString(strResponce, _T("div"), _T("class"), _T("datalist-contain datalist"));
}

void MainFrame::InsertLineHead()
{
	DataManager* pDataMgr = DataManager::GetInstance();
	do
	{
		stuListColumnItem tHeadItem;
		tHeadItem.wColumnWidth = 40;
		tHeadItem.wDataDescribe = DTP_ICON;
		sprintf_s(tHeadItem.szColumnName, "%s", "默认");
		pDataMgr->InsertLineHead(&tHeadItem);
	} while (0);

	do
	{
		stuListColumnItem tHeadItem;
		tHeadItem.wColumnWidth = 100;
		tHeadItem.wDataDescribe = DTP_LINE_NAME;
		sprintf_s(tHeadItem.szColumnName, "%s", "线路");
		pDataMgr->InsertLineHead(&tHeadItem);
	} while (0);

	do
	{
		stuListColumnItem tHeadItem;
		tHeadItem.wColumnWidth = 310;
		tHeadItem.wDataDescribe = DTP_LINE_WEBSITE;
		sprintf_s(tHeadItem.szColumnName, "%s", "网址");
		pDataMgr->InsertLineHead(&tHeadItem);
	} while (0);

	do
	{
		stuListColumnItem tHeadItem;
		tHeadItem.wColumnWidth = 50;
		tHeadItem.wDataDescribe = DTP_LINE_EXTRA;
		sprintf_s(tHeadItem.szColumnName, "%s", "其他");
		pDataMgr->InsertLineHead(&tHeadItem);
	} while (0);

	// 先清除列表UI
	LineList* pLineList = LineList::GetInstance();
	pLineList->Clean();

	// 插入列表头
	std::vector<stuListColumnItem*> vLineHeadList = pDataMgr->GetLineListHead();
	for (size_t i = 0; i < vLineHeadList.size(); ++i)
	{
		pLineList->InsertHeadItem(vLineHeadList[i]);
	}

	// 加载列表控件UI
	CListUI* pListUI = static_cast<CListUI*>(m_PaintManager.FindControl(R->kLstMain_LineList));
	pLineList->LoadHeaderItem(pListUI);
}

void MainFrame::InsertLineBody()
{
	// 插入表体
	LineList* pLineList = LineList::GetInstance();
	DataManager* pDataMgr = DataManager::GetInstance();
	std::vector<stuLineData*>& vLineBodyList = pDataMgr->GetLineListBody();
	for (int i = 0; i < vLineBodyList.size(); ++i)
	{
		pLineList->InsertBodyItem(20, vLineBodyList[i]);
	}
}

// 随着屏幕适配列表头宽度
void MainFrame::AdaptHeaderWidth(bool bRestore)
{
	DataManager* pDataMgr = DataManager::GetInstance();
	std::vector<stuListColumnItem*> vLineHeadList = pDataMgr->GetLineListHead();
	LineList* pLineList = LineList::GetInstance();
	for (int i = 0; i < vLineHeadList.size(); ++i)
	{
		int iScale = bRestore ? 1 : 2;
		pLineList->SetHeaderItemWidth(i, iScale * vLineHeadList[i]->wColumnWidth);
	}
}

void MainFrame::_ParseHTMLString(std::string& strValue, char* pTag, char* pKey, char* pKeyValue)
{
	if (!pTag || !pKeyValue) return;

	// 解析html字符串
	htmlcxx::HTML::ParserDom parser;
	htmlcxxNodeTree nodeTree = parser.parseTree(strValue);

	//输出整棵DOM树
	for (htmlcxxNodeTree::iterator it = nodeTree.begin(); it != nodeTree.end(); ++it)
	{
		it->parseAttributes();
		std::string strTag = it->tagName();
		std::map<std::string, std::string> strMap = it->attributes();

		if (_tcscmp(strTag.c_str(), pTag) == 0 && strMap.size() > 0
			&& strMap.find(pKey) != strMap.end()
			&& _tcscmp(strMap[pKey].c_str(), pKeyValue) == 0)
		{
			htmlcxxNodeTree::iterator it_begin = nodeTree.begin(it);
			htmlcxxNodeTree::iterator it_end = nodeTree.end(it);
			_ParseLineData(nodeTree, it_begin, it_end);
			break;
		}
	}
}

void MainFrame::_ParseLineData(htmlcxxNodeTree nodeTree, htmlcxxNodeTree::iterator& it_begin, htmlcxxNodeTree::iterator& it_end)
{
	std::vector<std::string> vLineName;
	std::vector<std::vector<std::string>> vLineWebSite;

	for (; it_begin != it_end; ++it_begin)
	{
		it_begin->parseAttributes();
		std::string strTag = it_begin->tagName();
		std::map<std::string, std::string> strMap = it_begin->attributes();

		std::vector<std::string> vTmpLineList;
		if (_tcscmp(strTag.c_str(), _T("h2")) == 0)
		{
			htmlcxxNodeTree::iterator it_b = nodeTree.begin(it_begin);
			htmlcxxNodeTree::iterator it_e = nodeTree.end(it_begin);
			for (; it_b != it_e; ++it_b)
			{
				vLineName.push_back(it_b->tagName());
			}
		}
		else if (_tcscmp(strTag.c_str(), _T("ul")) == 0)
		{
			htmlcxxNodeTree::iterator it_b = nodeTree.begin(it_begin);
			htmlcxxNodeTree::iterator it_e = nodeTree.end(it_begin);
			for (; it_b != it_e; ++it_b)
			{
				it_b->parseAttributes();
				std::map<std::string, std::string> tmpMap = it_b->attributes();
				if (tmpMap.find(_T("href")) != tmpMap.end())
				{
					vTmpLineList.push_back(tmpMap[_T("href")]);
				}
			}
			vLineWebSite.push_back(vTmpLineList);
		}
	}

	// 保存线路至表体
	DataManager* pDataMgr = DataManager::GetInstance();
	for (int i = 0; i < vLineName.size(); ++i)
	{
		for (int j = 0; j < vLineWebSite[i].size(); ++j)
		{
			LPWSTR wstrLineName = StringConvertor::Utf8ToWide(vLineName[i].c_str());
			LPSTR strLineName = StringConvertor::WideToAnsi(wstrLineName);

			stuLineData* pBody = new stuLineData();
			pBody->dwID = i * vLineWebSite[i].size() + j;
			sprintf_s(pBody->szLineName, _T("%s%d"), strLineName, j + 1);
			sprintf_s(pBody->szLineWebSite, _T("%s"), vLineWebSite[i][j].c_str());
			pDataMgr->InsertLineBody(pBody);
			SAFE_DELETE(pBody);

			StringConvertor::StringFree(wstrLineName);
			StringConvertor::StringFree(strLineName);
		}
	}
}