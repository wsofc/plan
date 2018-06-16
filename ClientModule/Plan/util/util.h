#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include "../share/share.h"
#include "Resource.h"

namespace Plan
{
	struct FILE_STRUCT
	{
		FILE_STRUCT()
		{
			memset(this, 0, sizeof(*this));
		}

		std::string fileName;	//文件全名(文件名 + 后缀名)
		std::string name;		//文件名
		std::string exName;		//后缀名
		int			size;		//大小
	};

	class Util
	{
	public:

		~Util();
		static Util* GetInstance();

	public:

		DWORD TransARGBToRGB(DWORD dwValue);														// 转换ARGB至RichEdit控件的RGB
		DWORD TransRGBToARGB(DWORD dwValue);														// 转换RichEdit控件的RGB至ARGB
		DWORD TransARGBToRGBA(DWORD dwValue);														// 转换ARGB至RGBA

		void SafeExit();																			// 退出程序
		const TCHAR * FormatTime(time_t lTime, LPCTSTR lpFmt);										// 返回当前系统时间 格式为: "%Y-%m-%d %H:%M:%S"
		
		void AddTimer(HWND hWnd, ProFuncTimer pCallBack, UINT_PTR nIDEvent, UINT uElapse);			// 添加定时器
		void KillTimer(HWND hWnd, UINT_PTR nIDEvent);												// 移出定时器
		void RemoveAllTimers();																		// 移出所有定时器

		void Util::ShowTopWindow(HWND hWnd, bool bShow /* = true */, bool bTakeFocus /* = true */);	// 置顶窗口
		void Util::RestoreTopWindow(HWND hWnd);														// 复原窗口

		bool GetSkinResPath(LPCTSTR lpFolderName, CDuiString& pzOutPath);							// 获取皮肤路径
		int ReadSkinConfig();																		// 读取皮肤配置
		void SaveSkinConfig(LPCTSTR lpAccount);														// 保存皮肤配置
		bool LoadSkinResFromDll(LPCTSTR lpFileName);												// 加载资源dll

		///////////////////////////////////////////////////////// 文件操作 /////////////////////////////////////////////////////////

		/*@breif 遍历指定文件夹
		*@param	lpPath		文件目录名
		*@param files		所有子文件的结构体
		*@param	isCircle	是否循环遍历子目录(默认:FALSE)
		*@param lpExName	要筛选的文件后缀名(例如:dll,则返回的数组里面只包含dll文件,为null或_T("")则返回全部文件)
		*/
		void traverseFolder(LPCTSTR lpstrFilter, std::vector<FILE_STRUCT>& files, BOOL isCircle = FALSE, LPCTSTR lpExName = nullptr);

		/*@breif 文件打开对话框
		*@param lpstrFilter	过滤字符串
		*@param hwndOwner	父窗口句柄
		*@param fileNames	完整文件路径
		*@param isMulti		是否标记选择多个文件
		*/
		bool openFileDialog(LPCTSTR lpstrFilter, HWND hwndOwner, std::vector<CDuiString> &fileNames, bool isMulti = false);

		/*@breif 文件保存对话框
		*@param hwndOwner		父窗口句柄
		*@param lpstrFilter		过滤字符串
		*@param lpszDefExt		扩展名
		*@param lpszTitle		标题
		*@param lpOutFilePath	输出的文件路径
		*/
		static bool saveFileDialog(HWND hwndOwner, LPCTSTR lpszFilter, LPCTSTR lpszDefExt, LPCTSTR lpszTitle, LPTSTR lpOutFilePath);

		// 文件路径接口

		static BOOL IsDirectory(LPCTSTR lpszPath);															// 检测指定路径是否目录
		static BOOL IsFileExist(LPCTSTR lpszFileName);														// 检测指定文件是否存在
		static BOOL IsDirectoryExist(LPCTSTR lpszPath);														// 检测指定目录是否存在
		static BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);		// 创建指定文件目录
		static BOOL DeleteDirectory(LPCTSTR lpPathName);													// 删除文件夹所有内容

		static std::string GetAppPath();																	// 获取应用程序执行路径
		static std::string GetCurDir();																		// 获取应用程序当前目录
		static std::string GetTempPath();																	// 获取当前系统的临时文件夹的路径
		static std::string GetTempFileName(LPCTSTR lpszFileName);											// 获取当前系统的临时文件夹的路径下的唯一命名的临时文件名(全路径)
		static std::string GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);						// 获取随机文件名(全路径)
		static std::string GetPathRoot(LPCTSTR lpszPath);													// 获取指定路径的根目录信息
		static std::string GetDirectoryName(LPCTSTR lpszPath);												// 返回指定路径字符串的目录信息
		static std::string GetFileName(LPCTSTR lpszPath);													// 返回指定路径字符串的文件名和扩展名
		static std::string GetFileNameWithoutExtension(LPCTSTR lpszPath);									// 返回不具有扩展名的路径字符串的文件名
		static std::string GetExtension(LPCTSTR lpszPath);													// 返回指定的路径字符串的扩展名
		static std::string GetFullPath(LPCTSTR lpszPath);													// 根据指定的相对路径获取绝对路径

	private:

		static Util* g_instance;
		Util();

	private:

		WndTimmer m_WndTimmer;
	};
}
#endif // __UTIL_H__