// TabContainerDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SFW.h"
#include "TabContainerDlg.h"
#include "afxdialogex.h"


// CTabContainerDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTabContainerDlg, CDialogEx)

CTabContainerDlg::CTabContainerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabContainerDlg::IDD, pParent)
{

}

CTabContainerDlg::~CTabContainerDlg()
{
}

void CTabContainerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TPANELCONTAINER, m_tabPanelContainer);
}


BEGIN_MESSAGE_MAP(CTabContainerDlg, CDialogEx)
END_MESSAGE_MAP()


// CTabContainerDlg �޽��� ó�����Դϴ�.


BOOL CTabContainerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_tabPanelContainer.InsertItem(0, _T("Parameter"));
	m_tabPanelContainer.InsertItem(1, _T("Auto"));
	m_tabPanelContainer.InsertItem(1, _T("Result"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
