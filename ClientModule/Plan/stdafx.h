// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#ifndef __STDAFX_H__
#define __STDAFX_H__

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO:  在此处引用程序需要的其他头文件
#include <iostream>
#include<atlstr.h>
#include <fstream>
#include <regex>
#include <direct.h>
#include "resource.h"

//加载vc读取.rc版本信息库
#pragma comment(lib, "version.lib")

// 是否开启资源测试模式(开启测试模式则从磁盘读取,否则从Dll中读取,取消注释即可生效)
#define SKIN_RES_TEST

#ifndef SKIN_RES_TEST
	#define SKINE_ZIPRE_ID	101		// 皮肤资源包ID(值在资源dll中定义)
#endif

/*
*	Duilib库
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
*	Libcurl库
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
*	VC-Logge库(输出日志)
*/
#include "..\\..\\ThirdParty\\vc-logge\\StaticLogger.h"
#include "..\\..\\ThirdParty\\vc-logge\\DynamicLogger.h"
#ifdef DEBUG || _DEBUG
#	pragma comment(lib, "..\\..\\PublicLibrary\\debug\\log_d.lib")
#else
#	pragma comment(lib, "..\\..\\PublicLibrary\\release\\log.lib")
#endif

/*
*	Json库
*/
#include "..\\..\\ThirdParty\\json\\include\\json\\json.h"
#	ifdef DEBUG || _DEBUG
#		pragma comment(lib, "..\\..\\PublicLibrary\\debug\\json_vc71_libmtd.lib")
#	else
#		pragma comment(lib, "..\\..\\PublicLibrary\\release\\json_vc71_libmt.lib")
#endif

#include "util/util.h"

#endif //__STDAFX_H__