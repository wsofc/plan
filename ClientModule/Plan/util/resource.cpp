#include "stdafx.h"
#include "Resource.h"

namespace Plan
{
	Resource*			Resource::g_instance					= NULL;									//����
	UILIB_RESOURCETYPE	Resource::g_ResourceType				= UILIB_RESOURCETYPE::UILIB_FILE;		//��Դ����
	HINSTANCE			Resource::g_ResourceInst				= NULL;									//��Դ���
	LPCTSTR				Resource::g_ResourceID					= _T("");								//��ԴID
	int					Resource::g_SkinResID					= 0;									//Ƥ��ID
	TCHAR				Resource::g_ResourcePath[MAX_PATH]		= { 0 };								//��Դ·��
	TCHAR				Resource::g_ResourceDllPath[MAX_PATH]	= { 0 };								//��ԴDll·��

	Resource* Resource::GetInstance()
	{
		if (Resource::g_instance)
		{
			return Resource::g_instance;
		}
		Resource::g_instance = new Resource();
		return Resource::g_instance;
	}

	Resource::Resource()
	{ }

	Resource::~Resource()
	{ }
}