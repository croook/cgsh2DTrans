
// cgsh2DTrans.h : cgsh2DTrans Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// Ccgsh2DTransApp:
// �йش����ʵ�֣������ cgsh2DTrans.cpp
//

class Ccgsh2DTransApp : public CWinAppEx
{
public:
	Ccgsh2DTransApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Ccgsh2DTransApp theApp;
