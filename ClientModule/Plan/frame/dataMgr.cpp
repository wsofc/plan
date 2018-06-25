#include "stdafx.h"
#include "dataMgr.h"

DataManager* DataManager::g_pDataManager = nullptr;
DataManager::DataManager()
{ }

DataManager::~DataManager()
{
	DeleteAllLineBody();
}

DataManager* DataManager::GetInstance()
{
	if (DataManager::g_pDataManager == nullptr)
		DataManager::g_pDataManager = new (std::nothrow)DataManager();
	return DataManager::g_pDataManager;
}

bool DataManager::InsertLineHead(stuListColumnItem * pData)
{
	ASSERT(pData != nullptr);
	if (!pData) return false;

	stuListColumnItem* pDestData = nullptr;
	if (!pDestData) pDestData = new stuListColumnItem();
	::CopyMemory(pDestData, pData, sizeof(stuListColumnItem));
	m_vLineListHead.push_back(pDestData);

	return true;
}

bool DataManager::DeleteLineHead(DWORD dwIndex)
{
	std::vector<stuListColumnItem*>::iterator it = m_vLineListHead.begin();
	for (int i = 0; it != m_vLineListHead.end(); ++it, ++i)
	{
		if (i == dwIndex)
		{
			SAFE_DELETE(*it);
			m_vLineListHead.erase(it);
			return true;
		}
	}

	return false;
}

void DataManager::DeleteAllLineHead()
{
	for (const auto& it : m_vLineListHead)
	{
		stuListColumnItem* pData = it;
		SAFE_DELETE(pData);
	}
	m_vLineListHead.clear();
}

stuListColumnItem* DataManager::SearchLineHead(DWORD dwIndex)
{
	stuListColumnItem* pData = nullptr;
	for (int i = 0; i < m_vLineListHead.size(); ++i)
	{
		if (i == dwIndex)
		{
			pData = m_vLineListHead[i];
			break;
		}
	}

	return pData;
}

bool DataManager::InsertLineBody(stuLineData * pData)
{
	ASSERT(pData != nullptr);
	if (!pData) return false;
	
	stuLineData* pDestData = nullptr;
	for (const auto& it : m_vLineListBody)
	{
		if (it->dwID == pData->dwID)
		{
			it->szLineWebSite;
			::StrToCharArray(it->szLineWebSite, pData->szLineWebSite);
			return true;
		}
	}

	if (!pDestData) pDestData = new stuLineData();
	::CopyMemory(pDestData, pData, sizeof(stuLineData));
	m_vLineListBody.push_back(pDestData);

	return true;
}

bool DataManager::DeleteLineBody(DWORD id)
{
	std::vector<stuLineData*>::iterator it = m_vLineListBody.begin();
	if (it != m_vLineListBody.end())
	{
		SAFE_DELETE(*it);
		m_vLineListBody.erase(it);
		return true;
	}
	else
	{
		return false;
	}
}

void DataManager::DeleteAllLineBody()
{
	for (const auto& it : m_vLineListBody)
	{
		stuLineData* pData = it;
		SAFE_DELETE(pData);
	}
	m_vLineListBody.clear();
}

stuLineData* DataManager::SearchLineBody(DWORD id)
{
	stuLineData* pData = nullptr;
	for (const auto& it : m_vLineListBody)
	{
		if (it->dwID == id)
		{
			pData = it;
			break;
		}
	}

	return pData;
}
