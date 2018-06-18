#ifndef __RESOURCE_H__
#define __RESOURCE_H__

// --------------------------------------------------------------------
// Btn	- Button		- 按钮
// Tls	- TabLayout		- 切换面板层控件
// Til	- TileLayout	- 平铺块控件
// Opt	- Option		- 单选控件
// Edt	- RichEdit		- 富文本编辑框
// Lab	- Label			- 标签控件
// Cmb	- Combo			- 下拉框控件
// Tex	- Text			- 文本控件
// Ctl	- Control		- 基控件
// Ctn	- Container		- 基容器
// Tew	- TreeView		- 树状控件
// Ten	- TreeNode		- 树状节点(依附于TreeView上)
// Wke	- CWkeWebkitUI	- 基于谷歌的webkit
// Pro	- Progress		- 进度条
// Menu	- Menu			- 菜单
// Str	- String		- 字符串
// Tvw	- TreeView		- 树状视图

namespace Plan
{
	/*
	*	全局资源
	*/
	class Resource
	{
	public:
		~Resource();
		static Resource* GetInstance();

		const TCHAR* const			MainWndTitleName			= _T("Plan测试");								//主窗标题名
		const TCHAR* const			MainWndClassName			= _T("MainFrame");								//主窗口类名
		const TCHAR* const			MainExeMoudleName			= _T("Plan.exe");								//执行程序名
		
		static UILIB_RESOURCETYPE	g_ResourceType;																//资源类型
		static HINSTANCE			g_ResourceInst;																//资源句柄
		static LPCTSTR				g_ResourceID;																//资源ID
		static int					g_SkinResID;																//皮肤ID
		static TCHAR				g_ResourcePath[MAX_PATH];													//资源路径
		static TCHAR				g_ResourceDllPath[MAX_PATH];												//资源Dll路径

	public:

		// 主面板
		const TCHAR* const kBtnMain_Close						= _T("main-btn_close");							//关闭
		const TCHAR* const kBtnMain_Min							= _T("main-btn_min");							//最下化
		const TCHAR* const kBtnMain_Max							= _T("main-btn_max");							//最大化
		const TCHAR* const kBtnMain_Restore						= _T("main-btn_restore");						//复原

		const TCHAR* const kEdtMain_WebSite						= _T("main-edt_website");						//网站
		const TCHAR* const kEdtMain_SafeCode					= _T("main-edt_safecode");						//安全码
		const TCHAR* const kBtnMain_GetLine						= _T("main-btn_getline");						//获取线路
		const TCHAR* const kLstMain_LineList					= _T("main-list_line");							//线路列表

		// 网案投注

		// 方案设置
	private:

		Resource();
		static Resource* g_instance;
	};
}

#endif // __RESOURCE_H__