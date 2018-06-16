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

	// ��ʼ��ȫ��CURL
	CURLcode wCurlCode = curl_global_init(CURL_GLOBAL_DEFAULT);

	// ��ʼ��CURL������
	LibcurlHelper::g_share_handle = curl_share_init();
	if (LibcurlHelper::g_share_handle)
	{
		// ����DNS����
		CURLSHcode wCURLSHcode = curl_share_setopt(LibcurlHelper::g_share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
	}
	
	g_bInitGlobal = wCurlCode == CURLE_OK;
	return g_bInitGlobal;
}

int LibcurlHelper::CleanGlobal()
{
	// �ر�CURL������
	CURLSHcode wCURLSHcode = curl_share_cleanup(LibcurlHelper::g_share_handle);

	// �ͷ�ȫ��CURL��Դ
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

	// ִ��д���ļ�������
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this);			//ָ���ص�����
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, handleWrite);	//ָ���ص�����

	// �����ض����������(���Ҵ�������ֹ����̫��)
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_MAXREDIRS, 5);

	// ����301��302��ת����location
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);

	// ���õ�alarm�źţ���ֹ���߳���ʹ�ó�ʱ����
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);

	// ����alarm����д���CLOSE_WAIT
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_FORBID_REUSE, 1);

	// ����������SSL��֤����ʹ��CA֤��
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);

	// ��֤�������˷��͵�֤�飬Ĭ���� 2(��)��1���У���0�����ã�
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);

	// ����ʱ��㱨���е���Ϣ�������STDERR��ָ����CURLOPT_STDERR��
	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L);

	wCurlCode |= curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, "SUREN:SUREN");

	// ʹ��DNS����
	// ����ÿ��CURL���󶼻�����һ�η�����,�������1000��HTTP�������ӵ�ͬһ������,libcurl�ͻ᷵�ش������Ӵ���ͽ��մ���,Ϊ��ʹ��DNS�����Ǻ��б�Ҫ��
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
	// ���ļ�
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
	// �����ص���������׷�ӵķ�ʽд���ļ�(һ��Ҫ��a������ǰ��д������ݾͻᱻ������)
	LibcurlHelper* pThis = static_cast<LibcurlHelper*>(userp);
	ASSERT(pThis);

	DWORD dwWritten = 0;
	switch (pThis->m_wFlag)
	{
		case LibcurlFlag::LIBCURLFLAG_NONE:
			break;

			// ����
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