// ViewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CGInspector.h"
#include "ViewDlg.h"
#include "afxdialogex.h"


// CViewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CViewDlg, CDialogEx)

CViewDlg::CViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIEWDLG, pParent)
{

}

CViewDlg::~CViewDlg()
{
}

void CViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewDlg, CDialogEx)
END_MESSAGE_MAP()


// CViewDlg 메시지 처리기입니다.
