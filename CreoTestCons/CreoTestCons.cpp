// CreoTestCons.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "CreoTestCons.h"
#include "CreoAsm.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CCreoTestConsApp

BEGIN_MESSAGE_MAP(CCreoTestConsApp, CWinApp)
END_MESSAGE_MAP()


// CCreoTestConsApp ����

CCreoTestConsApp::CCreoTestConsApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCreoTestConsApp ����

CCreoTestConsApp theApp;


// CCreoTestConsApp ��ʼ��

BOOL CCreoTestConsApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}




//declare
uiCmdAccessState access_func(uiCmdAccessMode access_mode);
void btnAction();
// ������ڣ�Creo��Visual Studio�Ľӿ�
extern "C" int user_initialize()
{
    ProError status = ProError::PRO_TK_ABORT;
    ProFileName MsgFile;
    ProStringToWstring(MsgFile, "menu.txt");
    //������ť
    uiCmdCmdId btn1;
    //��Ӳ˵�
    status = ProMenubarMenuAdd("Menu1", "Menu1_Label", NULL, PRO_B_TRUE, MsgFile);//�˵���ǩ��(�ڶ�������)��menu.txt�Ĳ˵���ǩ����Ӧ
    //��Ӷ���
    status = ProCmdActionAdd("rewrite_button_action", (uiCmdCmdActFn)btnAction, uiProeImmediate, access_func, PRO_B_TRUE, PRO_B_TRUE, &btn1);
    //��Ӱ�ť
    status = ProMenubarmenuPushbuttonAdd("Menu1", "btn1Name", "Button_Label", "Button_Label", NULL, PRO_B_TRUE, btn1, MsgFile);//��ť��ǩ��(����������)�Ͱ�����(���ĸ�����)��Ҫ��menu.txt�İ�ť��ǩ��һ��
    return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
}

uiCmdAccessState access_func(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}
std::vector<CADConstraint> vecCadCons;
void btnAction()
{
    CreoGetAllAsmConstraints();
    return;
}
