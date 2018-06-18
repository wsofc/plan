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
		const TCHAR* const kBtnMain_Close						= _T("main-btn_close");							//�ر�
		const TCHAR* const kBtnMain_Min							= _T("main-btn_min");							//���»�
		const TCHAR* const kBtnMain_Max							= _T("main-btn_max");							//���
		const TCHAR* const kBtnMain_Restore						= _T("main-btn_restore");						//��ԭ

		const TCHAR* const kEdtMain_WebSite						= _T("main-edt_website");						//��վ
		const TCHAR* const kEdtMain_SafeCode					= _T("main-edt_safecode");						//��ȫ��
		const TCHAR* const kBtnMain_GetLine						= _T("main-btn_getline");						//��ȡ��·
		const TCHAR* const kLstMain_LineList					= _T("main-list_line");							//��·�б�

		// ����Ͷע

		// ��������
	private:

		Resource();
		static Resource* g_instance;
	};
}

#endif // __RESOURCE_H__