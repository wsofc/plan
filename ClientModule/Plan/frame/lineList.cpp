#include "stdafx.h"
#include "LineList.h"

extern Plan::Util*		g_GameUtil;
extern Plan::Resource*	R;
extern CStaticLogger	g_Logger;

LineList* LineList::g_instance = nullptr;
LineList::LineList()
	: m_pList(nullptr)
{
	g_Logger->TryInfo(_T("LineList - Create"));
	::ZeroMemory(m_pzBuffer, ArrayCount(m_pzBuffer) * sizeof(TCHAR));
}

LineList::~LineList()
{
	g_Logger->TryInfo(_T("LineList - Destroy"));
}

LineList* LineList::GetInstance()
{
	if (!LineList::g_instance)
	{
		LineList::g_instance = new (std::nothrow)LineList();
	}
	return LineList::g_instance;
}

void LineList::Release()
{
	if (!LineList::g_instance) return;

	LineList::g_instance->Clean();
	SAFE_DELETE(LineList::g_instance);
}

// 添加自定义列
void LineList::InsertSelfDefineHeadItem()
{
	// 添加第1列: 图标
	stuListColumnItem* pHeadItem_1 = new stuListColumnItem();
	pHeadItem_1->wColumnWidth = 25;
	pHeadItem_1->wDataDescribe = 0;
	this->InsertHeadItem(pHeadItem_1);
	SAFE_DELETE(pHeadItem_1);
}

// 添加表头数据至数组
bool LineList::InsertHeadItem(stuListColumnItem* pColumItem)
{
	ASSERT(pColumItem != nullptr);
	if (!pColumItem) return false;

	// 避免重复添加
	for (const auto& it : m_vHeadArry)
	{
		if (it->wDataDescribe == pColumItem->wDataDescribe)
			return true;
	}

	stuListColumnItem* pHeadItem = nullptr;
	pHeadItem = new stuListColumnItem;
	::memcpy(pHeadItem, pColumItem, sizeof(stuListColumnItem));
	m_vHeadArry.push_back(pHeadItem);

	return true;
}

stuListColumnItem* LineList::EnumHeadItem(DWORD& iNextKey)
{
	stuListColumnItem* pHeadItem = nullptr;
	if (iNextKey >= 0 && iNextKey < m_vHeadArry.size())
	{
		pHeadItem = m_vHeadArry[iNextKey];

		// iNextKey往后偏移
		++iNextKey;
	}
	return pHeadItem;
}

bool LineList::RemoveHeadItem(WORD wDataDescribe)
{
	bool bRes = false;
	stuListColumnItem* pHeadItem = nullptr;
	std::vector<stuListColumnItem*>::iterator it;
	size_t i = 0;
	for (it = m_vHeadArry.begin(), i = 0; it != m_vHeadArry.end(); ++i)
	{
		if ((*it)->wDataDescribe == wDataDescribe)
		{
			if (m_pList)
			{
				CListHeaderUI* pHeaderUI = m_pList->GetHeader();
				if (pHeaderUI)
				{
					pHeaderUI->RemoveAt(i);
				}
			}

			stuListColumnItem* pTagItem = *it;
			SAFE_DELETE(pTagItem);
			
			m_vHeadArry.erase(it++);
			bRes = true;

			break;
		}
	}

	return bRes;
}

void LineList::RemovaAllHeadItem()
{
	std::vector<stuListColumnItem*>::iterator it;
	for (it = m_vHeadArry.begin(); it != m_vHeadArry.end(); ++it)
	{
		stuListColumnItem* pTagItem = *it;
		SAFE_DELETE(pTagItem);
	}
	m_vHeadArry.clear();

	if (m_pList)
	{
		CListHeaderUI* pHeaderUI = m_pList->GetHeader();
		if (pHeaderUI)
		{
			pHeaderUI->RemoveAll();
		}
	}
}

CListHeaderItemUI* LineList::CreatFixedHeadItem(WORD wIndex, WORD wColumnWidth, bool bSep /* = true */)
{
	TCHAR pzName[MAX_PATH] = { 0 };
	CListHeaderItemUI* pHeadItem = nullptr;
	_stprintf_s(pzName, _T("linelist-fixed_head_btn_%02u"), wIndex);

	pHeadItem = new CListHeaderItemUI();
	pHeadItem->SetName(pzName);
	pHeadItem->SetText(_T(""));
	pHeadItem->SetFont(1);
	pHeadItem->SetMinWidth(LISTHEAD_MIN_WIDTH);
	pHeadItem->SetBkImage(_T("file='res/list_bg.png' corner='4,4,4,4'"));
	pHeadItem->SetHotImage(_T("file='res/btn_hot.png' corner='4,4,4,4'"));
	pHeadItem->SetPushedImage("file='res/btn_push.png' corner='4,4,4,4'");
	pHeadItem->SetEnabled(false);
	pHeadItem->SetDragable(false);

	if (bSep)
	{
		pHeadItem->SetFixedWidth(wColumnWidth);
		pHeadItem->SetSepWidth(LISTHEAD_SEP_WIDTH);
		pHeadItem->SetSepImage(_T("file='res/list_sep.png' aligncenter='true'"));
	}

	return pHeadItem;
}

CListHeaderItemUI* LineList::CreatFloatHeadItem(WORD wIndex, stuListColumnItem* pItem)
{
	if (!pItem) return nullptr;
	TCHAR pzName[MAX_PATH] = { 0 };
	CListHeaderItemUI* pHeadItem = nullptr;
	_stprintf_s(pzName, _T("linelist-float_head_btn_%02u"), wIndex);

	pHeadItem = new CListHeaderItemUI();
	pHeadItem->SetName(pzName);
	pHeadItem->SetText(pItem->szColumnName);
	pHeadItem->SetFixedWidth(pItem->wColumnWidth);
	pHeadItem->SetSepWidth(LISTHEAD_SEP_WIDTH);
	pHeadItem->SetFont(1);
	pHeadItem->SetTextColor(0xFFFFFFFF);
	pHeadItem->SetMinWidth(LISTHEAD_MIN_WIDTH);
	pHeadItem->SetBkImage(_T("file='res/list_bg.png' corner='4,4,4,4'"));
	pHeadItem->SetHotImage(_T("file='res/btn_hot.png' corner='4,4,4,4'"));
	pHeadItem->SetPushedImage("file='res/btn_push.png' corner='4,4,4,4'");
	pHeadItem->SetSepImage(_T("file='res/list_sep.png' aligncenter='true'"));

	return pHeadItem;
}

void LineList::LoadHeaderItem(CListUI* pList)
{
	if (!pList) return;
	m_pList = pList;
	m_pList->SetTextCallback(this);
	m_pList->GetHeader()->RemoveAll();
	m_pList->RemoveAll();

	// 加载 head
	WORD wIndex = 0;
	CListHeaderItemUI* pHeadItem = nullptr;
	for (size_t i = 0; i < m_vHeadArry.size(); ++i)
	{
		pHeadItem = CreatFloatHeadItem(wIndex++, m_vHeadArry[i]);
		m_pList->Add(pHeadItem);
	}
}

void LineList::SetHeaderItemWidth(WORD wIndex, WORD wColumnWidth)
{
	if (!m_pList) return;
	if (wIndex < 0 || wIndex >= m_pList->GetHeader()->GetCount()) return;
	CListHeaderUI* pHeader = m_pList->GetHeader();
	CListHeaderItemUI* pHeadItem = static_cast<CListHeaderItemUI*>(pHeader->GetItemAt(wIndex));
	if (pHeadItem) pHeadItem->SetFixedWidth(wColumnWidth);
}

void LineList::InsertBodyItem(WORD wFixedHeight, stuLineData* pBody)
{
	if (!m_pList || !pBody) return;

	// 先检测一遍,避免重复添加(主要针对多次重复调用时生效)
	for (int i = 0; i < m_vBodyArry.size(); ++i)
	{
		if (m_vBodyArry[i]->dwID == pBody->dwID)
		{
			return;
		}
	}

	// 添加表体数据
	m_vBodyArry.push_back(pBody);

	CListTextElementUI* pListElement = new CListTextElementUI();
	pListElement->SetFixedHeight(wFixedHeight);
	m_pList->Add(pListElement);

	// 刷新列表
	Invalidate();
}

void LineList::RemoveBodyItemByIndex(DWORD dwIndex)
{
	size_t iIndex = 0;
	std::vector<stuLineData*>::iterator it;
	for (it = m_vBodyArry.begin(); it != m_vBodyArry.end();)
	{
		if (iIndex == dwIndex)
		{
			m_pList->RemoveAt(iIndex);
			m_vBodyArry.erase(it++);
			break;
		}

		++iIndex;
		++it;
	}
}

void LineList::RemoveAllBodyItem()
{
	if (m_pList)
	{
		m_pList->RemoveAll();
	}

	m_vBodyArry.clear();
}

void LineList::Clean()
{
	g_Logger->TryInfo(_T("LineList - Clean"));
	RemovaAllHeadItem();
	RemoveAllBodyItem();
	g_Logger->TryInfo(_T("LineList - Clean Complete"));
}

bool LineList::SortItemFunc(stuLineData* item_a, stuLineData* item_b)
{
	bool bRes = false;
	return bRes;
}

void LineList::SortBodyItem(bool bOpen /* = true */)
{
	if (bOpen && m_pList)
	{
		// std::sort排序的坑(比较==时必须返回false，否则进入死循环)
		std::sort(m_vBodyArry.begin(), m_vBodyArry.end(), LineList::SortItemFunc);
		m_pList->Invalidate();
	}
}

void LineList::Invalidate()
{
	if (m_pList)
	{
		m_pList->Invalidate();
	}
}

LPCTSTR LineList::GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
{
	::ZeroMemory(m_pzBuffer, ArrayCount(m_pzBuffer) * sizeof(TCHAR));

	// 判断是否越界
	if (iIndex < 0 || iIndex >= m_vBodyArry.size()) return m_pzBuffer;

	if (pControl->GetParent()->GetParent()->GetName() == R->kLstMain_LineList)
	{
		UINT uFont			= 1;									// 字体索引
		DWORD dwTxtColor	= 0xFFFFFFFF;							// 文本颜色
		bool bGray			= false;								// 灰度显示(关闭)
		LPCTSTR pzGray		= bGray ? _T("true") : _T("false");

		// 按列 填充表体数据
		if(iSubItem >= 0 && iSubItem < m_vHeadArry.size())
		{
			WORD w_index = iSubItem;
			switch (m_vHeadArry[w_index]->wDataDescribe)
			{
				// 图标
				case DTP_ICON:
				{
					_stprintf_s(m_pzBuffer, _T("<bkimage file='res/icon_default.png' aligncenter='true' gray='%s'>"), pzGray);
				}break;

				// 线路名
				case DTP_LINE_NAME:
				{
					_stprintf_s(m_pzBuffer, _T("<text content='%s' font='%u' textcolor='#%lu'>")
						, m_vBodyArry[iIndex]->szLineName, uFont, dwTxtColor);
				}break;

				// 线路网址
				case DTP_LINE_WEBSITE:
				{
					_stprintf_s(m_pzBuffer, _T("<text content='%s' font='%u' textcolor='#%lu'>")
						, m_vBodyArry[iIndex]->szLineWebSite, uFont, dwTxtColor);
				}break;

				// 额外
				case DTP_LINE_EXTRA:
				{
					_stprintf_s(m_pzBuffer, _T("<text content='%s' font='%u' textcolor='#%lu'>")
						, _T(""), uFont, dwTxtColor);
				}break;
				default:
					break;
			}
		}
	}

	return m_pzBuffer;
}

void LineList::Notify(TNotifyUI& msg)
{
	CDuiString& sType = msg.sType;
	CDuiString& sName = msg.pSender->GetName();

	// 选中
	if (_tcscmp(sType, DUI_MSGTYPE_ITEMSELECT) == 0)
	{
		g_Logger->TryInfo(_T("LineList - Item Event - Select"));
	}

	// 双击
	else if (_tcscmp(sType, DUI_MSGTYPE_ITEMDBCLICK) == 0)
	{
		g_Logger->TryInfo(_T("LineList - Item Event - Double_Click"));
	}

	// enter键
	else if (_tcscmp(sType, DUI_MSGTYPE_ITEMACTIVATE) == 0)
	{
		g_Logger->TryInfo(_T("LineList - Item Event - Enter"));
	}

	// 右键弹起
	else if (_tcscmp(sType, DUI_MSGTYPE_ITEMRCLICK) == 0)
	{
		CListTextElementUI* pListElement = static_cast<CListTextElementUI*>(msg.pSender);
		if (pListElement)
		{
			int iIndex = pListElement->GetIndex();
			if (iIndex >= 0 && iIndex < m_vBodyArry.size())
			{
				g_Logger->TryInfo(_T("LineList - Item Event - Right_Click"));
			}
		}
	}
}
