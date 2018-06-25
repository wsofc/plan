/**********************************************************************
* @title:		基于libcurl库的辅助类
*
* @author:		Aries
* @data:		2048/06/16
* @breif:		支持异步请求、多任务下载、暂停、断点续传、重新下载等
*
***********************************************************************/

#ifndef __LIBCURL_DOWNLOADER_H__
#define __LIBCURL_DOWNLOADER_H__

#include <thread>
#include <mutex>

#define WM_MY_LIBCURL		WM_USER + 0x1000		// libcurl 自定义消息(用于子线程PostMessage通知主线程更新UI等)

// 错误枚举
// 若创建了子线程,则PostMessage到主线程更新UI
enum eDownloadStatus
{
	E_DOWNLOAD_STATUS_NONE = 0						// 无
	, E_DOWNLOAD_STATUS_CREATE_DIR_FAILED			// 创建目录失败
	, E_DOWNLOAD_STATUS_OPENFILE_FAILED				// 打开文件失败
	, E_DOWNLOAD_STATUS_GETLOCALSIZE_FAILED			// 获取本地已下载文件大小失败
	, E_DOWNLOAD_STATUS_SETFILEPOINTER_FAILED		// 设置文件偏移读写失败(续传)
	, E_DOWNLOAD_STATUS_REQ_SIZE_BLOCK				// 请求获取下载文件大小线程阻塞
	, E_DOWNLOAD_STATUS_CONTINUE_TRANSFER			// 继续下载
	, E_DOWNLOAD_STATUS_DOWNLOAD_FAILED				// 下载失败
	, E_DOWNLOAD_STATUS_DOWNLOAD_OK					// 下载成功
	, E_DOWNLOAD_STATUS_DOWNLOAD_ABORT				// 终止下载
	, E_DOWNLOAD_STATUS_DOWNLOAD_PAUSE				// 暂停下载
	, E_DOWNLOAD_STATUS_DOWNLOAD_RESUME				// 继续下载
	, E_DOWNLOAD_STATUS_EXTRA						// 额外预留项
};

// 通信方式
enum LibcurlFlag
{
	LIBCURLFLAG_NONE = 0							// 无
	, LIBCURLFLAG_DOWNLOAD							// 下载
	, LIBCURLFLAG_POST								// Post请求
	, LIBCURLFLAG_GET								// Get请求
};

class LibcurlHelper
{
public:
	LibcurlHelper();
	~LibcurlHelper();

public:
	static bool InitGlobal();																						// 初始化libcurl全局环境
	static int CleanGlobal();																						// 清理全局环境
	
	bool InitOption();																								// 初始化设置
	DOUBLE GetLocalFileSize(std::string& strFilePath);																// 获取本地文件大小
	void CalculateByteDesc(double dlSize, std::string& strDesc);													// 计算字节大小对应的字符串描述
	std::string HttpGet(const char* url);
	std::string HttpPost(const char* url, const char* param);

protected:
	static size_t handleWrite(void *buffer, size_t size, size_t nmemb, void *userp);								// 写入数据回调(所有对象共享接口)
	static size_t handleProgress(void *buffer, double dltotal, double dlnow, double ultotal, double ulnow);			// 进度回调(所有对象共享接口)

private:
	void _CloseCurlHander();																						// 关闭libcurl句柄

private:
	static CURLSH*		g_share_handle;																				// curl共享句柄
	static bool			g_bInitGlobal;																				// 全局环境是否初始化

private:
	CURL*				m_pCurl;																					// curl 句柄
	CURLcode			m_wCurlCode;																				// curl 错误码

	LibcurlFlag			m_wFlag;																					// 通信方式
	std::string			m_strRespons;																				// 响应内容
};

#endif // __LIBCURL_DOWNLOADER_H__

