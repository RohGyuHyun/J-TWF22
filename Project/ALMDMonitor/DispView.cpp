// DispView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "DispView.h"
#include "afxdialogex.h"


// CDispView ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDispView, CDialogEx)

CDispView::CDispView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIEW, pParent)
{

}

CDispView::~CDispView()
{
}

void CDispView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDispView, CDialogEx)
END_MESSAGE_MAP()


// CDispView �޽��� ó�����Դϴ�.
