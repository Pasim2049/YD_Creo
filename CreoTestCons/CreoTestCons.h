// CreoTestCons.h : CreoTestCons DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCreoTestConsApp
// �йش���ʵ�ֵ���Ϣ������� CreoTestCons.cpp
//

class CCreoTestConsApp : public CWinApp
{
public:
	CCreoTestConsApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
