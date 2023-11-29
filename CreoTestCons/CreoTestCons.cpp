// CreoTestCons.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "CreoTestCons.h"
#include "CreoAsm.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CCreoTestConsApp

BEGIN_MESSAGE_MAP(CCreoTestConsApp, CWinApp)
END_MESSAGE_MAP()


// CCreoTestConsApp 构造

CCreoTestConsApp::CCreoTestConsApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCreoTestConsApp 对象

CCreoTestConsApp theApp;


// CCreoTestConsApp 初始化

BOOL CCreoTestConsApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}




//declare
uiCmdAccessState access_func(uiCmdAccessMode access_mode);
void btnAction();
// 函数入口，Creo与Visual Studio的接口
extern "C" int user_initialize()
{
    ProError status = ProError::PRO_TK_ABORT;
    ProFileName MsgFile;
    ProStringToWstring(MsgFile, "menu.txt");
    //声明按钮
    uiCmdCmdId btn1;
    //添加菜单
    status = ProMenubarMenuAdd("Menu1", "Menu1_Label", NULL, PRO_B_TRUE, MsgFile);//菜单标签名(第二个参数)与menu.txt的菜单标签名对应
    //添加动作
    status = ProCmdActionAdd("rewrite_button_action", (uiCmdCmdActFn)btnAction, uiProeImmediate, access_func, PRO_B_TRUE, PRO_B_TRUE, &btn1);
    //添加按钮
    status = ProMenubarmenuPushbuttonAdd("Menu1", "btn1Name", "Button_Label", "Button_Label", NULL, PRO_B_TRUE, btn1, MsgFile);//按钮标签名(第三个参数)和帮助名(第四个参数)都要与menu.txt的按钮标签名一致
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
