// InfoPanelDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SFW.h"
#include "InfoPanelDlg.h"
#include "afxdialogex.h"


// CInfoPanelDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CInfoPanelDlg, CDialogEx)

CInfoPanelDlg::CInfoPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfoPanelDlg::IDD, pParent)
{

}

CInfoPanelDlg::~CInfoPanelDlg()
{
}

void CInfoPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TINFOSEL, m_tabInfoSel);
}


BEGIN_MESSAGE_MAP(CInfoPanelDlg, CDialogEx)
END_MESSAGE_MAP()


// CInfoPanelDlg �޽��� ó�����Դϴ�.


BOOL CInfoPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_tabInfoSel.InsertItem(0, _T("Log"));
	m_tabInfoSel.InsertItem(1, _T("Detail"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
