// LogIn.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SFW.h"
#include "LogIn.h"
#include "afxdialogex.h"


// CLogIn ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CLogIn, CDialogEx)

CLogIn::CLogIn(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogIn::IDD, pParent)
{

}

CLogIn::~CLogIn()
{
}

void CLogIn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLogIn, CDialogEx)
END_MESSAGE_MAP()


// CLogIn �޽��� ó�����Դϴ�.
