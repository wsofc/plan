#include "stdafx.h"
#include "util.h"
#include <commdlg.h>
#include <winioctl.h>

extern CStaticLogger	g_Logger;
extern HANDLE			g_hMutex;

namespace Plan
{
	Util* Util::g_instance = NULL;
	Util::Util()
	{ }

	Util::~Util()
	{ }

	Util* Util::GetInstance()
	{
		if (NULL == Util::g_instance)
		{
			Util::g_instance = new (std::nothrow)Util();
		}

		return Util::g_instance;
	}

	DWORD Util::TransARGBToRGB(DWORD dwValue)
	{
		//int a = LOBYTE((dwValue) >> 24);
		BYTE r = GetBValue(dwValue);
		BYTE g = GetGValue(dwValue);
		BYTE b = GetRValue(dwValue);
		return RGB(r, g, b);
	}

	DWORD Util::TransRGBToARGB(DWORD dwValue)
	{
		BYTE a = 0xFF;
		BYTE r = GetRValue(dwValue);
		BYTE g = GetGValue(dwValue);
		BYTE b = GetBValue(dwValue);

		int dwReturn = (DWORD)(a << 24) | (DWORD)(r << 16) | (DWORD)(g << 8) | (DWORD)(b);
		return dwReturn;
	}

	DWORD Util::TransARGBToRGBA(DWORD dwValue)
	{
		BYTE a = LOBYTE((dwValue) >> 24);
		BYTE r = GetBValue(dwValue);
		BYTE g = GetGValue(dwValue);
		BYTE b = GetRValue(dwValue);

		int dwReturn = (DWORD)(r << 24) | (DWORD)(g << 16) | (DWORD)(b << 8) | (DWORD)(a);
		return dwReturn;
	}

	void Util::SafeExit()
	{
		RemoveAllTimers();

		// �ͷŽ��̻������
		::CloseHandle(g_hMutex);

		// �˳���Ϣѭ��(�����������Ϣ���շ�)
		::PostQuitMessage(0);
	}

	const TCHAR * Util::FormatTime(time_t lTime, LPCTSTR lpFmt)
	{
		struct tm * lpTimeInfo;
		static TCHAR cTime[32];

		memset(cTime, 0, sizeof(cTime));

		lpTimeInfo = localtime(&lTime);
		if (lpTimeInfo != NULL)
		{
			_tcsftime(cTime, sizeof(cTime) / sizeof(TCHAR), lpFmt, lpTimeInfo);
		}
		return cTime;
	}

	void Util::AddTimer(HWND hWnd, ProFuncTimer pCallBack, UINT_PTR nIDEvent, UINT uElapse)
	{
		m_WndTimmer.SetTimer(hWnd, pCallBack, nIDEvent, uElapse);
	}

	void Util::KillTimer(HWND hWnd, UINT_PTR nIDEvent)
	{
		m_WndTimmer.KillTimer(hWnd, nIDEvent);
	}

	void Util::RemoveAllTimers()
	{
		m_WndTimmer.RemoveAllTimers();
	}

	// ���ü̳�CWindowWnd��ֱ�ӰѸ÷��������д�빫������,�������
	void Util::ShowTopWindow(HWND hWnd, bool bShow /* = true */, bool bTakeFocus /* = true */)
	{
		if (!::IsWindow(hWnd)) return;

		WPARAM wParam = bShow ? (bTakeFocus ? SW_SHOW : SW_SHOWNOACTIVATE) : SW_HIDE;
		::SendMessage(hWnd, WM_SYSCOMMAND, wParam, 0);

		::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		::UpdateWindow(hWnd);
		::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}

	void Util::RestoreTopWindow(HWND hWnd)
	{
		if (!::IsWindow(hWnd)) return;

		::SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		::UpdateWindow(hWnd);
		::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}

	bool Util::GetSkinResPath(LPCTSTR lpFolderName, CDuiString& pzOutPath)
	{
		std::vector<FILE_STRUCT> files;
		LPCTSTR lpExName = _T("");

#ifndef SKIN_RES_TEST
		lpExName = _T("dll");
#endif
		this->traverseFolder(lpFolderName, files, FALSE, lpExName);

		if (files.size() > 0)
		{
			if (Plan::Resource::g_SkinResID >= files.size())
			{
				Plan::Resource::g_SkinResID = 0;
			}

			pzOutPath = files[Plan::Resource::g_SkinResID].fileName.c_str();
			return true;
		}

		return false;
	}

	int Util::ReadSkinConfig()
	{
		int iSkinIndex = 0;
		CDuiString pzModulePath = CPaintManagerUI::GetInstancePath();
		pzModulePath += _T("Plan\\data\\user\\INFO.lm");

		Json::Value vRoot;
		JsonFile* pJsonFile = new JsonFile(pzModulePath, true);
		bool bRes = pJsonFile->initRootNode(vRoot);
		SAFE_DELETE(pJsonFile);
		if (bRes)
		{
			if (!vRoot["AccountInfo"].empty() && vRoot["AccountInfo"].isArray())
			{
				Json::Value& vNode = vRoot["AccountInfo"][0];
				if (!vNode.empty() && !vNode["iSkinIndex"].empty() && vNode["iSkinIndex"].isInt())
				{
					iSkinIndex = vNode["iSkinIndex"].asInt();
				}
			}
		}

		return iSkinIndex;
	}

	void Util::SaveSkinConfig(LPCTSTR lpAccount)
	{
		CDuiString pzModulePath = CPaintManagerUI::GetInstancePath();
		pzModulePath += _T("Plan\\data\\user\\INFO.lm");

		Json::Value vRoot;
		JsonFile* pJsonFile = new JsonFile(pzModulePath, true);
		bool bRes = pJsonFile->initRootNode(vRoot);
		if (bRes)
		{
			if (!vRoot["AccountInfo"].empty() && vRoot["AccountInfo"].isArray())
			{
				for (int i = 0; i < vRoot["AccountInfo"].size(); ++i)
				{
					Json::Value& vNode = vRoot["AccountInfo"][i];
					LPCTSTR pzAccount = vNode["pzAccount"].asCString();
					if (_tcscmp(pzAccount, lpAccount) == 0)
					{
						vNode["iSkinIndex"] = Plan::Resource::g_SkinResID;
						pJsonFile->SaveFile(vRoot);
						break;
					}
				}
			}
		}
		SAFE_DELETE(pJsonFile);
	}

	bool Util::LoadSkinResFromDll(LPCTSTR lpFileName)
	{
		if (!lpFileName || _tcscmp(lpFileName, _T("")) == 0) return false;

		//GameResource::g_ResourceInst = ::LoadLibraryEx(lpFileName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
		Plan::Resource::g_ResourceInst = ::LoadLibrary(lpFileName);
		if (Plan::Resource::g_ResourceInst == NULL)
		{
			int e_code = ::GetLastError();
			g_Logger->TryError(_T("Load Skin Dll Failed [%u]"), e_code);

			::FreeLibrary(Plan::Resource::g_ResourceInst);
			return false;
		}

		// ��Dll��Դѹ�����ж�ȡ
		Plan::Resource::g_ResourceType = UILIB_RESOURCETYPE::UILIB_ZIPRESOURCE;

		// ѹ������Դid
#ifdef SKINE_ZIPRE_ID
		Plan::Resource::g_ResourceID = MAKEINTRESOURCE(SKINE_ZIPRE_ID);
#else
		Plan::Resource::g_ResourceID = _T("");
#endif //SKINE_ZIPRE_ID

		return true;
	}

	////////////////////////////////////////////// �ļ����� //////////////////////////////////////////////
	void Util::traverseFolder(LPCTSTR lpstrFilter, std::vector<FILE_STRUCT>& files, BOOL isCircle /* = FALSE */, LPCTSTR lpExName /* = nullptr */)
	{
		TCHAR szFind[MAX_PATH] = { _T("\0") };
		WIN32_FIND_DATA findFileData;
		BOOL bRet;

		_tcscpy_s(szFind, MAX_PATH, lpstrFilter);
		_tcscat_s(szFind, _T("\\*.*"));     //����һ��Ҫָ��ͨ�������Ȼ�����ȡ�����ļ���Ŀ¼

		HANDLE hFind = ::FindFirstFile(szFind, &findFileData);
		if (INVALID_HANDLE_VALUE == hFind)
		{
			return;
		}

		//�����ļ���
		FILE_STRUCT file;
		while (TRUE)
		{
			//���ǵ�ǰ·�����߸�Ŀ¼�Ŀ�ݷ�ʽ
			if (findFileData.cFileName[0] != _T('.'))
			{
				_tprintf(_T("%s\\%s\n"), lpstrFilter, findFileData.cFileName);

				//����һ����ͨĿ¼(����������Ŀ¼)
				if (isCircle && findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					//������һ����Ҫɨ����ļ���·��
					_tcscpy_s(szFind, MAX_PATH, lpstrFilter);
					_tcscat_s(szFind, _T("\\"));
					_tcscat_s(szFind, findFileData.cFileName);
					///_tcscat_s(szNextDir, _T("\\*"));

					//������Ŀ¼
					traverseFolder(szFind, files, isCircle);
				}
				else
				{
					file.fileName = findFileData.cFileName;
					file.size = findFileData.nFileSizeLow - findFileData.nFileSizeHigh;
					file.name = this->GetFileNameWithoutExtension(file.fileName.c_str()).c_str();
					file.exName = this->GetExtension(file.fileName.c_str()).c_str();
					if (!lpExName || _tcscmp(lpExName, _T("")) == 0 || _tcscmp(lpExName, file.exName.c_str()) == 0)
					{
						files.push_back(file);
					}
				}
			}
			//����ǵ�ǰ·�����߸�Ŀ¼�Ŀ�ݷ�ʽ����������ͨĿ¼����Ѱ����һ��Ŀ¼�����ļ�
			bRet = ::FindNextFile(hFind, &findFileData);
			if (!bRet)
			{//��������ʧ��
				//cout << "FindNextFile failed, error code: " 
				//  << GetLastError() << endl;
				break;
			}
		}

		::FindClose(hFind);
	}

	bool Util::openFileDialog(LPCTSTR lpstrFilter, HWND hwndOwner, std::vector<CDuiString> &fileNames, bool isMulti /* = false */)
	{
		DWORD dwFlag = isMulti ? OFN_ALLOWMULTISELECT : 0;
		TCHAR szFileName[MAX_PATH * 101 + 1] = _T("");

		OPENFILENAME openfilename = { 0 };

		::ZeroMemory(&openfilename, sizeof(OPENFILENAME));

		CDuiString s_title;

		openfilename.lStructSize = sizeof(OPENFILENAME);
		openfilename.hwndOwner = hwndOwner;
		openfilename.hInstance = NULL;
		openfilename.lpstrFilter = lpstrFilter;
		openfilename.lpstrCustomFilter = NULL;
		openfilename.nMaxCustFilter = 0L;
		openfilename.nFilterIndex = 1L;
		openfilename.lpstrFile = szFileName;
		openfilename.nMaxFile = MAX_PATH * 101 + 1;
		openfilename.lpstrFileTitle = NULL;
		openfilename.nMaxFileTitle = 0;
		openfilename.lpstrInitialDir = NULL;
		openfilename.lpstrTitle = s_title;
		openfilename.nFileOffset = 0;
		openfilename.nFileExtension = 0;
		openfilename.lpstrDefExt = _T("*.*");
		openfilename.lCustData = 0;
		openfilename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_READONLY | OFN_EXPLORER | dwFlag;

		// �������ļ��ĶԻ���
		CDuiString str;

		if (::GetOpenFileName(&openfilename))
		{
			LPTSTR p = szFileName;
			CDuiString TempPath;
			if (*p != NULL)
			{
				TempPath = p;
				p += TempPath.GetLength() + 1;
			}

			if (*p == NULL)
			{
				//	TempPath = TempPath.Left(TempPath.ReverseFind(L'\\'));
				fileNames.push_back(TempPath);
			}


			while (*p != NULL)
			{
				CDuiString str = p;

				p += str.GetLength() + 1;

				fileNames.push_back(TempPath + _T("\\") + str);
			}

			return TRUE;
		}
		else
			return FALSE;
	}

	bool Util::saveFileDialog(HWND hwndOwner, LPCTSTR lpszFilter, LPCTSTR lpszDefExt, LPCTSTR lpszTitle, LPTSTR lpOutFilePath)
	{
		TCHAR szFilePath[MAX_PATH] = TEXT("");
		TCHAR szFileTitle[MAX_PATH] = TEXT("");
		OPENFILENAME openfilename = { 0 };
		::ZeroMemory(&openfilename, sizeof(OPENFILENAME));

		openfilename.lStructSize = sizeof(OPENFILENAME);
		openfilename.hwndOwner = hwndOwner;					// �����߶ԵĻ��򴰿ھ��
		openfilename.hInstance = NULL;						// һ��ΪNULL����ʾ����ʹ�õ���ͨ�öԻ���,����ָ���Զ���Ի���
		openfilename.lpstrFilter = lpszFilter;				// ���͹���
		openfilename.lpstrCustomFilter = NULL;				// �û��Զ����������ʽ
		openfilename.nMaxCustFilter = 0L;					// �û��Զ����������ʽ��С
		openfilename.nFilterIndex = 0; // 1L				// ��ǰѡ��Ĺ�����������
		openfilename.lpstrFile = szFilePath;				// �ļ�·��
		openfilename.nMaxFile = MAX_PATH;					// �ļ�·����С
		openfilename.lpstrFileTitle = szFileTitle;			// �ļ���
		openfilename.nMaxFileTitle = MAX_PATH;				// �ļ�����С
		openfilename.lpstrInitialDir = NULL;				// ��ʼ��·��
		openfilename.lpstrTitle = lpszTitle;				// �Ի������
		openfilename.nFileOffset = 0;						// �ļ�·����ȥ�ļ�����ƫ�Ƴ���
		openfilename.nFileExtension = 0;					// �ļ�·����ȥ��չ�����ƫ�Ƴ���
		openfilename.lpstrDefExt = lpszDefExt;				// Ĭ����չ��
		openfilename.lCustData = 0;							// Ӧ�ó����������
		openfilename.Flags = 0;

		//�����ļ�����
		BOOL bRes = False;
		bRes = ::GetSaveFileName(&openfilename);

		if (lpOutFilePath)
			_tcscpy(lpOutFilePath, szFilePath);

		return bRes;
	}

	BOOL Util::IsDirectory(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return FALSE;

		DWORD dwAttr = ::GetFileAttributes(lpszPath);

		if ((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
			return TRUE;
		else
			return FALSE;
	}

	BOOL Util::IsFileExist(LPCTSTR lpszFileName)
	{
		if (NULL == lpszFileName || NULL == *lpszFileName)
			return FALSE;

		if (::GetFileAttributes(lpszFileName) != 0xFFFFFFFF)
			return TRUE;
		else
			return FALSE;
	}

	BOOL Util::IsDirectoryExist(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return FALSE;

		DWORD dwAttr = ::GetFileAttributes(lpszPath);

		if ((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
			return TRUE;
		else
			return FALSE;
	}

	BOOL Util::CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		TCHAR cPath[MAX_PATH] = { 0 };
		TCHAR cTmpPath[MAX_PATH] = { 0 };
		TCHAR * lpPos = NULL;
		TCHAR cTmp = _T('\0');

		if (NULL == lpPathName || NULL == *lpPathName)
			return FALSE;

		_tcsncpy(cPath, lpPathName, MAX_PATH);

		for (int i = 0; i < (int)_tcslen(cPath); i++)
		{
			if (_T('\\') == cPath[i])
				cPath[i] = _T('/');
		}

		lpPos = _tcschr(cPath, _T('/'));
		while (lpPos != NULL)
		{
			if (lpPos == cPath)
			{
				lpPos++;
			}
			else
			{
				cTmp = *lpPos;
				*lpPos = _T('\0');
				_tcsncpy(cTmpPath, cPath, MAX_PATH);
				::CreateDirectory(cTmpPath, lpSecurityAttributes);
				*lpPos = cTmp;
				lpPos++;
			}
			lpPos = _tcschr(lpPos, _T('/'));
		}

		return TRUE;
	}

	BOOL Util::DeleteDirectory(LPCTSTR lpPathName)
	{
		BOOL bRet = FALSE;
		WIN32_FIND_DATA tFindFileData;
		memset(&tFindFileData, 0, sizeof(WIN32_FIND_DATA));

		// ����һ��Ҫָ��ͨ�������Ȼ�����ȡ�����ļ���Ŀ¼
		TCHAR szFind[MAX_PATH] = { _T("\0") };
		_tcscpy_s(szFind, MAX_PATH, lpPathName);
		_tcscat_s(szFind, _T("\\*.*"));

		HANDLE hFind = ::FindFirstFile(szFind, &tFindFileData);
		if (INVALID_HANDLE_VALUE == hFind)
		{
			return FALSE;
		}

		do
		{
			if (tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (_tcscmp(tFindFileData.cFileName, ".") != 0 && _tcscmp(tFindFileData.cFileName, "..") != 0)
				{
					//������Ŀ¼���ݹ�֮
					CString strFile;
					strFile += lpPathName;
					strFile += _T("\\");
					strFile += tFindFileData.cFileName;
					Util::DeleteDirectory(strFile);
				}
			}
			else
			{
				//�ҵ��ļ�������֮
				CString strFile;
				strFile += lpPathName;
				strFile += _T("\\");
				strFile += tFindFileData.cFileName;

				::DeleteFile(strFile);
			}
		} while (::FindNextFile(hFind, &tFindFileData));

		::FindClose(hFind);

		// ɾ����Ŀ¼
		if (!RemoveDirectory(lpPathName))
		{
			return FALSE;
		}

		return TRUE;
	}

	std::string Util::GetAppPath()
	{
		static TCHAR szPath[MAX_PATH] = { 0 };

		if (szPath[0] == _T('\0'))
		{
			::GetModuleFileName(NULL, szPath, MAX_PATH);

			std::string strPath = GetDirectoryName(szPath);
			_tcsncpy(szPath, strPath.c_str(), MAX_PATH);
		}

		return szPath;
	}

	std::string Util::GetCurDir()
	{
		TCHAR szCurDir[MAX_PATH] = { 0 };
		::GetCurrentDirectory(MAX_PATH, szCurDir);

		if (szCurDir[_tcslen(szCurDir) - 1] != _T('\\'))
			_tcscat(szCurDir, _T("\\"));

		return szCurDir;
	}

	std::string Util::GetTempPath()
	{
		TCHAR szTempPath[MAX_PATH] = { 0 };

		::GetTempPath(MAX_PATH, szTempPath);

		if (szTempPath[_tcslen(szTempPath) - 1] != _T('\\'))
			_tcscat(szTempPath, _T("\\"));

		return szTempPath;
	}

	std::string Util::GetTempFileName(LPCTSTR lpszFileName)
	{
		return GetRandomFileName(GetTempPath().c_str(), lpszFileName);
	}

	std::string Util::GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName)
	{
		std::string strPath, strFileName, strExtFileName, strFullPath;
		TCHAR szBuf[MAX_PATH] = { 0 };

		if (!IsDirectoryExist(lpszPath))
			strPath = GetCurDir();
		else
			strPath = lpszPath;

		strFileName = GetFileNameWithoutExtension(lpszFileName);
		strExtFileName = GetExtension(lpszFileName);

		for (int i = 2; i < 10000; i++)
		{
			if (strExtFileName.empty())
			{
				strFullPath = strPath;
				strFullPath += strFileName;
				sprintf_s(szBuf, _T("%d"), i);
				strFullPath += szBuf;
			}
			else
			{
				strFullPath = strPath;
				strFullPath += strFileName;
				sprintf_s(szBuf, _T("%d."), i);
				strFullPath += szBuf;
				strFullPath += strExtFileName;
			}

			if (!IsFileExist(strFullPath.c_str()))
				return strFullPath;
		}

		return _T("");
	}

	std::string Util::GetPathRoot(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		std::string strPath(lpszPath);

		std::string::iterator iter;
		for (iter = strPath.begin(); iter < strPath.end(); iter++)
		{
			if (_T('\\') == *iter)
				*iter = _T('/');
		}

		std::string::size_type nPos = strPath.find(_T('/'));
		if (nPos != std::string::npos)
			strPath = strPath.substr(0, nPos + 1);

		return strPath;
	}

	std::string Util::GetDirectoryName(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		std::string strPath(lpszPath);

		std::string::iterator iter;
		for (iter = strPath.begin(); iter < strPath.end(); iter++)
		{
			if (_T('\\') == *iter)
				*iter = _T('/');
		}

		std::string::size_type nPos = strPath.rfind(_T('/'));
		if (nPos != std::string::npos)
			strPath = strPath.substr(0, nPos + 1);

		return strPath;
	}

	std::string Util::GetFileName(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		std::string strPath(lpszPath);

		std::string::iterator iter;
		for (iter = strPath.begin(); iter < strPath.end(); iter++)
		{
			if (_T('\\') == *iter)
				*iter = _T('/');
		}

		std::string::size_type nPos = strPath.rfind(_T('/'));
		if (nPos != std::string::npos)
			strPath = strPath.substr(nPos + 1);

		return strPath;
	}

	std::string Util::GetFileNameWithoutExtension(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		std::string strPath(lpszPath);

		std::string::iterator iter;
		for (iter = strPath.begin(); iter < strPath.end(); iter++)
		{
			if (_T('\\') == *iter)
				*iter = _T('/');
		}

		std::string::size_type nPos = strPath.rfind(_T('/'));
		if (nPos != std::string::npos)
			strPath = strPath.substr(nPos + 1);

		nPos = strPath.rfind(_T('.'));
		if (nPos != std::string::npos)
			strPath = strPath.substr(0, nPos);

		return strPath;
	}

	std::string Util::GetExtension(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		std::string strPath(lpszPath);

		std::string::size_type nPos = strPath.rfind(_T('.'));
		if (nPos != std::string::npos)
		{
			strPath = strPath.substr(nPos + 1);
		}
		else
		{
			strPath.clear();
		}

		return strPath;
	}

	std::string Util::GetFullPath(LPCTSTR lpszPath)
	{
		if (NULL == lpszPath || NULL == *lpszPath)
			return _T("");

		std::string strPath(lpszPath);

		std::string::iterator iter;
		for (iter = strPath.begin(); iter < strPath.end(); iter++)
		{
			if (_T('\\') == *iter)
				*iter = _T('/');
		}

		TCHAR cFirstChar = strPath.at(0);
		TCHAR cSecondChar = strPath.at(1);

		if (cFirstChar == _T('/'))
		{
			std::string strCurDir = GetAppPath();
			std::string strRootPath = GetPathRoot(strCurDir.c_str());
			return strRootPath + strPath;
		}
		else if (::IsCharAlpha(cFirstChar) && cSecondChar == _T(':'))
		{
			return strPath;
		}
		else
		{
			std::string strCurDir = GetAppPath();
			return strCurDir + strPath;
		}
	}
}