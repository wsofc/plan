#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

// ----------------------------------------------------------------------------------------------------------
// 数据结构

// 表体结构
struct stuLineData
{
	stuLineData()
	{
		memset(this, 0, sizeof(*this));
	}

	DWORD	dwID;						// 线路id
	TCHAR	szLineName[MAX_PATH];		// 线路名
	TCHAR	szLineWebSite[MAX_PATH];	// 线路网址
};

// ----------------------------------------------------------------------------------------------------------
// 数据管理类
class DataManager
{
private:
	DataManager();
	static DataManager*					g_pDataManager;							// 实例指针
private:
	std::vector<stuLineData*>			m_vLineListBody;						// 线路数据列表
	std::vector<stuListColumnItem*>		m_vLineListHead;						// 线路数据表头

public:
	~DataManager();
	static DataManager * GetInstance();											// 获取单实例

public:

	// 线路数据表头
	bool InsertLineHead(stuListColumnItem * pData);
	bool DeleteLineHead(DWORD dwIndex);
	void DeleteAllLineHead();
	stuListColumnItem* SearchLineHead(DWORD dwIndex);
	std::vector<stuListColumnItem*>& GetLineListHead(){ return m_vLineListHead; }

	// 插入线路数据表体
	bool InsertLineBody(stuLineData * pData);
	bool DeleteLineBody(DWORD id);
	void DeleteAllLineBody();
	stuLineData* SearchLineBody(DWORD id);
	std::vector<stuLineData*>& GetLineListBody(){ return m_vLineListBody; }	
};

#endif // !__DATA_MANAGER_H__