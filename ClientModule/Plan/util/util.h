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

		std::string fileName;	//�ļ�ȫ��(�ļ��� + ��׺��)
		std::string name;		//�ļ���
		std::string exName;		//��׺��
		int			size;		//��С
	};

	class Util
	{
	public:

		~Util();
		static Util* GetInstance();

	public:

		DWORD TransARGBToRGB(DWORD dwValue);														// ת��ARGB��RichEdit�ؼ���RGB
		DWORD TransRGBToARGB(DWORD dwValue);														// ת��RichEdit�ؼ���RGB��ARGB
		DWORD TransARGBToRGBA(DWORD dwValue);														// ת��ARGB��RGBA

		void SafeExit();																			// �˳�����
		const TCHAR * FormatTime(time_t lTime, LPCTSTR lpFmt);										// ���ص�ǰϵͳʱ�� ��ʽΪ: "%Y-%m-%d %H:%M:%S"
		
		void AddTimer(HWND hWnd, ProFuncTimer pCallBack, UINT_PTR nIDEvent, UINT uElapse);			// ��Ӷ�ʱ��
		void KillTimer(HWND hWnd, UINT_PTR nIDEvent);												// �Ƴ���ʱ��
		void RemoveAllTimers();																		// �Ƴ����ж�ʱ��

		void Util::ShowTopWindow(HWND hWnd, bool bShow /* = true */, bool bTakeFocus /* = true */);	// �ö�����
		void Util::RestoreTopWindow(HWND hWnd);														// ��ԭ����

		bool GetSkinResPath(LPCTSTR lpFolderName, CDuiString& pzOutPath);							// ��ȡƤ��·��
		int ReadSkinConfig();																		// ��ȡƤ������
		void SaveSkinConfig(LPCTSTR lpAccount);														// ����Ƥ������
		bool LoadSkinResFromDll(LPCTSTR lpFileName);												// ������Դdll

		///////////////////////////////////////////////////////// �ļ����� /////////////////////////////////////////////////////////

		/*@breif ����ָ���ļ���
		*@param	lpPath		�ļ�Ŀ¼��
		*@param files		�������ļ��Ľṹ��
		*@param	isCircle	�Ƿ�ѭ��������Ŀ¼(Ĭ��:FALSE)
		*@param lpExName	Ҫɸѡ���ļ���׺��(����:dll,�򷵻ص���������ֻ����dll�ļ�,Ϊnull��_T("")�򷵻�ȫ���ļ�)
		*/
		void traverseFolder(LPCTSTR lpstrFilter, std::vector<FILE_STRUCT>& files, BOOL isCircle = FALSE, LPCTSTR lpExName = nullptr);

		/*@breif �ļ��򿪶Ի���
		*@param lpstrFilter	�����ַ���
		*@param hwndOwner	�����ھ��
		*@param fileNames	�����ļ�·��
		*@param isMulti		�Ƿ���ѡ�����ļ�
		*/
		bool openFileDialog(LPCTSTR lpstrFilter, HWND hwndOwner, std::vector<CDuiString> &fileNames, bool isMulti = false);

		/*@breif �ļ�����Ի���
		*@param hwndOwner		�����ھ��
		*@param lpstrFilter		�����ַ���
		*@param lpszDefExt		��չ��
		*@param lpszTitle		����
		*@param lpOutFilePath	������ļ�·��
		*/
		static bool saveFileDialog(HWND hwndOwner, LPCTSTR lpszFilter, LPCTSTR lpszDefExt, LPCTSTR lpszTitle, LPTSTR lpOutFilePath);

		// �ļ�·���ӿ�

		static BOOL IsDirectory(LPCTSTR lpszPath);															// ���ָ��·���Ƿ�Ŀ¼
		static BOOL IsFileExist(LPCTSTR lpszFileName);														// ���ָ���ļ��Ƿ����
		static BOOL IsDirectoryExist(LPCTSTR lpszPath);														// ���ָ��Ŀ¼�Ƿ����
		static BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);		// ����ָ���ļ�Ŀ¼
		static BOOL DeleteDirectory(LPCTSTR lpPathName);													// ɾ���ļ�����������

		static std::string GetAppPath();																	// ��ȡӦ�ó���ִ��·��
		static std::string GetCurDir();																		// ��ȡӦ�ó���ǰĿ¼
		static std::string GetTempPath();																	// ��ȡ��ǰϵͳ����ʱ�ļ��е�·��
		static std::string GetTempFileName(LPCTSTR lpszFileName);											// ��ȡ��ǰϵͳ����ʱ�ļ��е�·���µ�Ψһ��������ʱ�ļ���(ȫ·��)
		static std::string GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);						// ��ȡ����ļ���(ȫ·��)
		static std::string GetPathRoot(LPCTSTR lpszPath);													// ��ȡָ��·���ĸ�Ŀ¼��Ϣ
		static std::string GetDirectoryName(LPCTSTR lpszPath);												// ����ָ��·���ַ�����Ŀ¼��Ϣ
		static std::string GetFileName(LPCTSTR lpszPath);													// ����ָ��·���ַ������ļ�������չ��
		static std::string GetFileNameWithoutExtension(LPCTSTR lpszPath);									// ���ز�������չ����·���ַ������ļ���
		static std::string GetExtension(LPCTSTR lpszPath);													// ����ָ����·���ַ�������չ��
		static std::string GetFullPath(LPCTSTR lpszPath);													// ����ָ�������·����ȡ����·��

	private:

		static Util* g_instance;
		Util();

	private:

		WndTimmer m_WndTimmer;
	};
}
#endif // __UTIL_H__