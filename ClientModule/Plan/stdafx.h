// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#ifndef __STDAFX_H__
#define __STDAFX_H__

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <iostream>
#include<atlstr.h>
#include <fstream>
#include <regex>
#include <direct.h>
#include "resource.h"

//����vc��ȡ.rc�汾��Ϣ��
#pragma comment(lib, "version.lib")

// �Ƿ�����Դ����ģʽ(��������ģʽ��Ӵ��̶�ȡ,�����Dll�ж�ȡ,ȡ��ע�ͼ�����Ч)
#define SKIN_RES_TEST

#ifndef SKIN_RES_TEST
	#define SKINE_ZIPRE_ID	101		// Ƥ����Դ��ID(ֵ����Դdll�ж���)
#endif

/*
*	Duilib��
*/
#include "..\\DuiLib\\UIlib.h"
using namespace DuiLib;
#ifdef DEBUG || _DEBUG
#   ifdef UNICODE || _UNICODE
#       pragma comment(lib, "..\\..\\PublicLibrary\\debug\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\..\\PublicLibrary\\debug\\DuiLib_d.lib")
#   endif
#else
#   ifdef UNICODE || _UNICODE
#       pragma comment(lib, "..\\..\\PublicLibrary\\release\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\..\\PublicLibrary\\release\\DuiLib.lib")
#   endif
#endif

/*
*	Libcurl��
*/
#include "..\\..\\ThirdParty\\libcurl\\include\\curl\\curl.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\libcurld.lib")
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\libcurld_imp.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\libcurl.lib")
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\libcurl_imp.lib")
#endif

/*
*	VC-Logge��(�����־)
*/
#include "..\\..\\ThirdParty\\vc-logge\\StaticLogger.h"
#include "..\\..\\ThirdParty\\vc-logge\\DynamicLogger.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\log_d.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\log.lib")
#endif

/*
*	Json��
*/
#include "..\\..\\ThirdParty\\json\\include\\json\\json.h"
#	ifdef DEBUG || _DEBUG
#		pragma comment(lib, "..\\..\\PublicLibrary\\debug\\json_vc71_libmtd.lib")
#	else
#		pragma comment(lib, "..\\..\\PublicLibrary\\release\\json_vc71_libmt.lib")
#endif

#include "util/util.h"

#endif //__STDAFX_H__