#ifndef __RESOURCE_H__
#define __RESOURCE_H__

// --------------------------------------------------------------------
// Btn	- Button		- ��ť
// Tls	- TabLayout		- �л�����ؼ�
// Til	- TileLayout	- ƽ�̿�ؼ�
// Opt	- Option		- ��ѡ�ؼ�
// Edt	- RichEdit		- ���ı��༭��
// Lab	- Label			- ��ǩ�ؼ�
// Cmb	- Combo			- ������ؼ�
// Tex	- Text			- �ı��ؼ�
// Ctl	- Control		- ���ؼ�
// Ctn	- Container		- ������
// Tew	- TreeView		- ��״�ؼ�
// Ten	- TreeNode		- ��״�ڵ�(������TreeView��)
// Wke	- CWkeWebkitUI	- ���ڹȸ��webkit
// Pro	- Progress		- ������
// Menu	- Menu			- �˵�
// Str	- String		- �ַ���
// Tvw	- TreeView		- ��״��ͼ

namespace Plan
{
	/*
	*	ȫ����Դ
	*/
	class Resource
	{
	public:
		~Resource();
		static Resource* GetInstance();

		const TCHAR* const			MainWndTitleName			= _T("Plan����");								//����������
		const TCHAR* const			MainWndClassName			= _T("MainFrame");								//����������
		const TCHAR* const			MainExeMoudleName			= _T("Plan.exe");								//ִ�г�����
		
		static UILIB_RESOURCETYPE	g_ResourceType;																//��Դ����
		static HINSTANCE			g_ResourceInst;																//��Դ���
		static LPCTSTR				g_ResourceID;																//��ԴID
		static int					g_SkinResID;																//Ƥ��ID
		static TCHAR				g_ResourcePath[MAX_PATH];													//��Դ·��
		static TCHAR				g_ResourceDllPath[MAX_PATH];												//��ԴDll·��

	public:

		// �����
		const TCHAR* const kBtnMain_Close						= _T("main-close_btn");							//�ر�
		const TCHAR* const kBtnMain_Close = _T("main-close_btn");							//�ر�
		const TCHAR* const kBtnMain_Close = _T("main-close_btn");							//�ر�

		// ����Ͷע

		// ��������
	private:

		Resource();
		static Resource* g_instance;
	};
}

#endif // __RESOURCE_H__