/**********************************************************************
* @title:		����libcurl��ĸ�����
*
* @author:		Aries
* @data:		2048/06/16
* @breif:		֧���첽���󡢶��������ء���ͣ���ϵ��������������ص�
*
***********************************************************************/

#ifndef __LIBCURL_DOWNLOADER_H__
#define __LIBCURL_DOWNLOADER_H__

#include <thread>
#include <mutex>

#define WM_MY_LIBCURL		WM_USER + 0x1000		// libcurl �Զ�����Ϣ(�������߳�PostMessage֪ͨ���̸߳���UI��)

// ����ö��
// �����������߳�,��PostMessage�����̸߳���UI
enum eDownloadStatus
{
	E_DOWNLOAD_STATUS_NONE = 0						// ��
	, E_DOWNLOAD_STATUS_CREATE_DIR_FAILED			// ����Ŀ¼ʧ��
	, E_DOWNLOAD_STATUS_OPENFILE_FAILED				// ���ļ�ʧ��
	, E_DOWNLOAD_STATUS_GETLOCALSIZE_FAILED			// ��ȡ�����������ļ���Сʧ��
	, E_DOWNLOAD_STATUS_SETFILEPOINTER_FAILED		// �����ļ�ƫ�ƶ�дʧ��(����)
	, E_DOWNLOAD_STATUS_REQ_SIZE_BLOCK				// �����ȡ�����ļ���С�߳�����
	, E_DOWNLOAD_STATUS_CONTINUE_TRANSFER			// ��������
	, E_DOWNLOAD_STATUS_DOWNLOAD_FAILED				// ����ʧ��
	, E_DOWNLOAD_STATUS_DOWNLOAD_OK					// ���سɹ�
	, E_DOWNLOAD_STATUS_DOWNLOAD_ABORT				// ��ֹ����
	, E_DOWNLOAD_STATUS_DOWNLOAD_PAUSE				// ��ͣ����
	, E_DOWNLOAD_STATUS_DOWNLOAD_RESUME				// ��������
	, E_DOWNLOAD_STATUS_EXTRA						// ����Ԥ����
};

// ͨ�ŷ�ʽ
enum LibcurlFlag
{
	LIBCURLFLAG_NONE = 0							// ��
	, LIBCURLFLAG_DOWNLOAD							// ����
	, LIBCURLFLAG_POST								// Post����
	, LIBCURLFLAG_GET								// Get����
};

class LibcurlHelper
{
public:
	LibcurlHelper();
	~LibcurlHelper();

public:
	static bool InitGlobal();																						// ��ʼ��libcurlȫ�ֻ���
	static int CleanGlobal();																						// ����ȫ�ֻ���
	
	bool InitOption();																								// ��ʼ������
	DOUBLE GetLocalFileSize(std::string& strFilePath);																// ��ȡ�����ļ���С
	void CalculateByteDesc(double dlSize, std::string& strDesc);													// �����ֽڴ�С��Ӧ���ַ�������
	std::string HttpGet(const char* url);
	std::string HttpPost(const char* url, const char* param);

protected:
	static size_t handleWrite(void *buffer, size_t size, size_t nmemb, void *userp);								// д�����ݻص�(���ж�����ӿ�)
	static size_t handleProgress(void *buffer, double dltotal, double dlnow, double ultotal, double ulnow);			// ���Ȼص�(���ж�����ӿ�)

private:
	void _CloseCurlHander();																						// �ر�libcurl���

private:
	static CURLSH*		g_share_handle;																				// curl������
	static bool			g_bInitGlobal;																				// ȫ�ֻ����Ƿ��ʼ��

private:
	CURL*				m_pCurl;																					// curl ���
	CURLcode			m_wCurlCode;																				// curl ������

	LibcurlFlag			m_wFlag;																					// ͨ�ŷ�ʽ
	std::string			m_strRespons;																				// ��Ӧ����
};

#endif // __LIBCURL_DOWNLOADER_H__

