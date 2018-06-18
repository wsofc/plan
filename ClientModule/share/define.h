#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <tchar.h>

#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif // ASSERT

#ifndef ArrayCount
#define	ArrayCount(Array) (sizeof(Array)/sizeof(Array[0]))
#endif // ArrayCount

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	{ try { delete p; } catch (...) { } p = nullptr; } 
#endif // SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(pArr) { if (pArr) { delete [] (pArr);  (pArr) = nullptr; } }
#endif // SAFE_DELETE_ARRAY

// 转换宏字符
#define _TO_S(x)	(#x)
#define TO_S(x)		_TO_S(x)

// 大厅版本信息
#define Plan_VERSION_1(wServerHigh)			((wServerHigh & 0xFF) / 10)
#define Plan_VERSION_2(wServerHigh)			((wServerHigh & 0xFF) % 10)
#define Plan_VERSION_3(wServerLow)			((wServerLow & 0xFF) / 10)
#define Plan_VERSION_4(wServerLow)			((wServerLow & 0xFF) % 10)

#define Plan_VERSION_S_1(wServerHigh)		TO_S(Plan_VERSION_1(wServerHigh))
#define Plan_VERSION_S_2(wServerHigh)		TO_S(Plan_VERSION_2(wServerHigh))
#define Plan_VERSION_S_3(wServerLow)		TO_S(Plan_VERSION_3(wServerLow))
#define Plan_VERSION_S_4(wServerLow)		TO_S(Plan_VERSION_4(wServerLow))

#define MAKELONGLONG(l, h)			((LONGLONG)(((((DWORD_PTR)(l)) & 0xffffffff)) | (((ULONGLONG)(((DWORD_PTR)(h)) & 0xffffffff)) << 32)))

// 自定义消息
#define CMW_CLOSE_SURE				0x00000100				// 关闭当前窗口

// new callbacks based on C++11
#define CC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ##__VA_ARGS__)

// 字符串
#if defined(UNICODE) || defined(_UNICODE)
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif

// 拷贝LPCTSTR至TCHAR数组中(含大小检测)
template <size_t size>
int StrToCharArray(TCHAR(&buffer)[size], LPCTSTR lpStr)
{
	if (!lpStr) return -1;

	int iSize = _tcslen(lpStr) + 1;
	iSize = min(ArrayCount(buffer) - 1, iSize);
	memcpy(buffer, lpStr, iSize * sizeof(TCHAR));

	return iSize;
}

#endif //__DEFINE_H__