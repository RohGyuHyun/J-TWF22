// MapDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SFW.h"
#include "MapDlg.h"
#include "afxdialogex.h"


// CMapDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapDlg, CDialogEx)

CMapDlg::CMapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMapDlg::IDD, pParent)
{

}

CMapDlg::~CMapDlg()
{
}

void CMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TMAPSEL, m_tabMapSel);
}


BEGIN_MESSAGE_MAP(CMapDlg, CDialogEx)
END_MESSAGE_MAP()


// CMapDlg �޽��� ó�����Դϴ�.


BOOL CMapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_tabMapSel.InsertItem(0, _T("X-Ray"));
	m_tabMapSel.InsertItem(1, _T("ViewCamera"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
