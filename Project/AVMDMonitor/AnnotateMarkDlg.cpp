// AnnotateMarkDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "AnnotateMarkDlg.h"
#include "afxdialogex.h"


// CAnnotateMarkDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAnnotateMarkDlg, CDialogEx)

CAnnotateMarkDlg::CAnnotateMarkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANNOTATE_MARK, pParent)
{

}

CAnnotateMarkDlg::~CAnnotateMarkDlg()
{
}

void CAnnotateMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAnnotateMarkDlg, CDialogEx)
END_MESSAGE_MAP()


// CAnnotateMarkDlg �޽��� ó�����Դϴ�.


BOOL CAnnotateMarkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
