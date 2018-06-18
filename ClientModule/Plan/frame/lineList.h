#ifndef __LINE_LIST_H__
#define __LINE_LIST_H__

#define LISTHEAD_SEP_WIDTH	2
#define LISTHEAD_MIN_WIDTH	2

// 表体结构
struct stuLineListBody
{
	stuLineListBody()
	{
		memset(this, 0, sizeof(*this));
	}

	TCHAR	szLineName[MAX_PATH];		// 线路名
	TCHAR	szLineWebSite[MAX_PATH];	// 线路网址
};

class LineList : public IListCallbackUI
{
public:
	static LineList* GetInstance();
	static void Release();
	
public:
	void InsertSelfDefineHeadItem();
	bool InsertHeadItem(stuListColumnItem* pColumItem);
	int GetHeadItemCount(){ return m_vHeadArry.size(); }
	stuListColumnItem* EnumHeadItem(DWORD& iNextKey);
	bool RemoveHeadItem(WORD wDataDescribe);
	void RemovaAllHeadItem();

	CListHeaderItemUI* CreatFixedHeadItem(WORD wIndex, WORD wColumnWidth, bool bSep = true);
	CListHeaderItemUI* CreatFloatHeadItem(WORD wIndex, stuListColumnItem* pItem);
	void LoadHeaderItem(CListUI* pList);

	const std::vector<stuListColumnItem*>& GetListHeadArray() const { return m_vHeadArry; }	// 获取表头数组
	const std::vector<stuLineListBody*>& GetListBodyArray() const { return m_vBodyArry; }	// 获取表体数组

	void InsertBodyItem(WORD wFixedHeight, stuLineListBody* pBody);
	void RemoveBodyItemByIndex(DWORD dwIndex);
	void RemoveAllBodyItem();

	static bool SortItemFunc(stuLineListBody* item_a, stuLineListBody* item_b);
	void SortBodyItem(bool bOpen = true);
	void Update();
	LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);

	void Clean();
	void Notify(TNotifyUI& msg);

private:
	LineList();
	~LineList();
	static LineList* g_instance;

private:
	CListUI*						m_pList;					// 列表控件
	TCHAR							m_pzBuffer[2 * MAX_PATH];	// 字符串缓冲区
	std::vector<stuListColumnItem*>	m_vHeadArry;				// 表头数组
	std::vector<stuLineListBody*>	m_vBodyArry;				// 表体数组
};

#endif // !__LINE_LIST_H__
