#ifndef __LINE_LIST_H__
#define __LINE_LIST_H__

#define LISTHEAD_SEP_WIDTH	2
#define LISTHEAD_MIN_WIDTH	2

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
	void SetHeaderItemWidth(WORD wIndex, WORD wColumnWidth);

	const std::vector<stuListColumnItem*>& GetListHeadArray() const { return m_vHeadArry; }	// ��ȡ��ͷ����
	const std::vector<stuLineData*>& GetListBodyArray() const { return m_vBodyArry; }	// ��ȡ��������

	void InsertBodyItem(WORD wFixedHeight, stuLineData* pBody);
	void RemoveBodyItemByIndex(DWORD dwIndex);
	void RemoveAllBodyItem();

	static bool SortItemFunc(stuLineData* item_a, stuLineData* item_b);
	void SortBodyItem(bool bOpen = true);
	void Invalidate();
	LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);

	void Clean();
	void Notify(TNotifyUI& msg);

private:
	LineList();
	~LineList();
	static LineList* g_instance;

private:
	CListUI*						m_pList;					// �б�ؼ�
	TCHAR							m_pzBuffer[2 * MAX_PATH];	// �ַ���������
	std::vector<stuListColumnItem*>	m_vHeadArry;				// ��ͷ����
	std::vector<stuLineData*>		m_vBodyArry;				// ��������
};

#endif // !__LINE_LIST_H__
