#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

// ----------------------------------------------------------------------------------------------------------
// ���ݽṹ

// ����ṹ
struct stuLineData
{
	stuLineData()
	{
		memset(this, 0, sizeof(*this));
	}

	DWORD	dwID;						// ��·id
	TCHAR	szLineName[MAX_PATH];		// ��·��
	TCHAR	szLineWebSite[MAX_PATH];	// ��·��ַ
};

// ----------------------------------------------------------------------------------------------------------
// ���ݹ�����
class DataManager
{
private:
	DataManager();
	static DataManager*					g_pDataManager;							// ʵ��ָ��
private:
	std::vector<stuLineData*>			m_vLineListBody;						// ��·�����б�
	std::vector<stuListColumnItem*>		m_vLineListHead;						// ��·���ݱ�ͷ

public:
	~DataManager();
	static DataManager * GetInstance();											// ��ȡ��ʵ��

public:

	// ��·���ݱ�ͷ
	bool InsertLineHead(stuListColumnItem * pData);
	bool DeleteLineHead(DWORD dwIndex);
	void DeleteAllLineHead();
	stuListColumnItem* SearchLineHead(DWORD dwIndex);
	std::vector<stuListColumnItem*>& GetLineListHead(){ return m_vLineListHead; }

	// ������·���ݱ���
	bool InsertLineBody(stuLineData * pData);
	bool DeleteLineBody(DWORD id);
	void DeleteAllLineBody();
	stuLineData* SearchLineBody(DWORD id);
	std::vector<stuLineData*>& GetLineListBody(){ return m_vLineListBody; }	
};

#endif // !__DATA_MANAGER_H__