#include "stdafx.h"
#include "Resource.h"

namespace Plan
{
	Resource*			Resource::g_instance					= NULL;									//单例
	UILIB_RESOURCETYPE	Resource::g_ResourceType				= UILIB_RESOURCETYPE::UILIB_FILE;		//资源类型
	HINSTANCE			Resource::g_ResourceInst				= NULL;									//资源句柄
	LPCTSTR				Resource::g_ResourceID					= _T("");								//资源ID
	int					Resource::g_SkinResID					= 0;									//皮肤ID
	TCHAR				Resource::g_ResourcePath[MAX_PATH]		= { 0 };								//资源路径
	TCHAR				Resource::g_ResourceDllPath[MAX_PATH]	= { 0 };								//资源Dll路径

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