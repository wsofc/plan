#include "stdafx.h"
#include "libcurlHelper.h"
#include <io.h>

bool LibcurlHelper::g_bInitGlobal = false;
CURLSH* LibcurlHelper::g_share_handle = nullptr;

LibcurlHelper::LibcurlHelper(CPaintManagerUI* pManager)
	: m_hWnd(NULL)
	, m_pManager(nullptr)
	, m_pCurl(nullptr)
	, m_wCurlCode(CURLE_OK)
	, m_wFlag(LibcurlFlag::LIBCURLFLAG_NONE)
{
	ASSERT(pManager);
	m_pManager = pManager;
	m_hWnd = m_pManager->GetPaintWindow();
}

LibcurlHelper::~LibcurlHelper()
{ }

bool LibcurlHelper::InitGlobal()
{
	if (g_bInitGlobal) return true;

	// 初始化全局CURL
	CURLcode wCurlCode = curl_global_init(CURL_GLOBAL_DEFAULT);

	// 初始化CURL共享句柄
	LibcurlHelper::g_share_handle = curl_share_init();
	if (LibcurlHelper::g_share_handle)
	{
		// 设置DNS共享
		CURLSHcode wCURLSHcode = curl_share_setopt(LibcurlHelper::g_share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
	}
	
	g_bInitGlobal = wCurlCode == CURLE_OK;
	return g_bInitGlobal;
}

int LibcurlHelper::CleanGlobal()
{
	// 关闭CURL共享句柄
	CURLSHcode wCURLSHcode = curl_share_cleanup(LibcurlHelper::g_share_handle);

	// 释放全局CURL资源
	curl_global_cleanup();
	g_bInitGlobal = false;

	return wCURLSHcode;
}

bool LibcurlHelper::InitOption()
{
	if (!LibcurlHelper::InitGlobal())	return false;

	UINT wCurlCode = 0;
	m_pCurl = curl_easy_init();
	ASSERT(m_pCurl);

	// 执行写入文件流操作
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this);			//指定回调参数
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, handleWrite);	//指定回调函数

	// 设置重定向的最大次数(查找次数，防止查找太深)
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_MAXREDIRS, 5);

	// 设置301、302跳转跟随location
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);

	// 禁用掉alarm信号，防止多线程中使用超时崩溃
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);

	// 禁掉alarm后会有大量CLOSE_WAIT
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_FORBID_REUSE, 1);

	// 跳过服务器SSL验证，不使用CA证书
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);

	// 验证服务器端发送的证书，默认是 2(高)，1（中），0（禁用）
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);

	// 启用时会汇报所有的信息，存放在STDERR或指定的CURLOPT_STDERR中
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L);

	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, "SUREN:SUREN");

	// 使用DNS共享
	// 由于每个CURL对象都会连接一次服务器,如果发送1000次HTTP请求都连接到同一服务器,libcurl就会返回大量连接错误和接收错误,为此使用DNS共享是很有必要的
	if (LibcurlHelper::g_share_handle)
	{
		UINT  wCURLSHcode = 0;
		wCURLSHcode |= curl_easy_setopt(m_pCurl, CURLOPT_SHARE, LibcurlHelper::g_share_handle);
		wCURLSHcode |= curl_easy_setopt(m_pCurl, CURLOPT_DNS_CACHE_TIMEOUT, 60 * 5);
	}

	m_wCurlCode = (CURLcode)wCurlCode;

	return m_wCurlCode == CURLE_OK;
}

DOUBLE LibcurlHelper::GetLocalFileSize(std::string& strFilePath)
{
	// 打开文件
	DWORD dwLocalFileSize = 0;
	HANDLE hLocalFile = ::CreateFile(strFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hLocalFile)
	{
		dwLocalFileSize = ::GetFileSize(hLocalFile, NULL);
		if (INVALID_FILE_SIZE == dwLocalFileSize)
		{
			dwLocalFileSize = 0;
		}
	}
	::CloseHandle(hLocalFile);

	return dwLocalFileSize;
}

void LibcurlHelper::CalculateByteDesc(double dlSize, std::string& strDesc)
{
	std::string unit = "B";

	DWORD dw_N = 1024;
	DWORD dw_K = dw_N;
	DWORD dw_M = dw_K * dw_N;
	DWORD dw_G = dw_M * dw_N;

	if (dlSize > dw_G)
	{
		unit = "G";
		dlSize /= dw_G;
	}
	else if (dlSize > dw_M)
	{
		unit = "M";
		dlSize /= dw_M;
	}
	else if (dlSize > dw_K)
	{
		unit = "KB";
		dlSize /= dw_K;
	}

	TCHAR pzBuff[MAX_PATH] = { 0 };
	sprintf_s(pzBuff, _T("%.2f%s"), dlSize, unit.c_str());
	strDesc = pzBuff;
}

size_t LibcurlHelper::handleWrite(void *buffer, size_t size, size_t nmemb, void *userp)
{
	// 把下载到的数据以追加的方式写入文件(一定要有a，否则前面写入的内容就会被覆盖了)
	LibcurlHelper* pThis = static_cast<LibcurlHelper*>(userp);
	ASSERT(pThis);

	DWORD dwWritten = 0;
	switch (pThis->m_wFlag)
	{
		case LibcurlFlag::LIBCURLFLAG_NONE:
			break;

			// 下载
		case LibcurlFlag::LIBCURLFLAG_DOWNLOAD:
			break;

		// Post
		case LibcurlFlag::LIBCURLFLAG_POST:
			break;

			// Get
		case LibcurlFlag::LIBCURLFLAG_GET:
		{
			dwWritten = size * nmemb;
			pThis->m_strRespons.append((const char*)buffer, dwWritten);
		}break;

		default:
			break;
	}

	//if (pThis->IsAbortDownload())
	{
		dwWritten = CURL_READFUNC_ABORT;
	}

	return dwWritten;
}

size_t LibcurlHelper::handleProgress(void *buffer, double dltotal, double dlnow, double ultotal, double ulnow)
{
	size_t uReturnCode = 0;
	return uReturnCode;
}

void LibcurlHelper::_CloseCurlHander()
{
	if (m_pCurl)
	{
		curl_easy_cleanup(m_pCurl);
		m_pCurl = NULL;
	}
}