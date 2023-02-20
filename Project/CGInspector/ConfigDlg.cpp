// ConfigDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CGInspector.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"
#include "Status.h"


// CConfigDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONFIG, pParent)
	, m_nCGLength(0)
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_CVYFRONT, m_cbCvyrFront);
	DDX_Control(pDX, IDC_CB_CVYEND, m_cbCvyrEnd);
	DDX_Text(pDX, IDC_CGLENGTH, m_nCGLength);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CConfigDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CConfigDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigDlg �޽��� ó�����Դϴ�.


void CConfigDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CStatus::Instance()->LoadCFG();
	EndDialog(-1);
}


BOOL CConfigDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	return CDialogEx::OnNotify(wParam, lParam, pResult);
}


BOOL CConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CString strAxis;
	for (int i = 0; i < 4; i++)
	{
		strAxis.Format(_T("%d"), i);
		m_cbCvyrFront.AddString(strAxis);
		m_cbCvyrEnd.AddString(strAxis);
	}

	m_cbCvyrFront.SetCurSel(CStatus::Instance()->m_nCvyrFrontAxis);
	m_cbCvyrEnd.SetCurSel(CStatus::Instance()->m_nCvyrEndAxis);
	m_nCGLength = CStatus::Instance()->GetCGLength();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CConfigDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	CStatus::Instance()->m_nCvyrFrontAxis = m_cbCvyrFront.GetCurSel();
	CStatus::Instance()->m_nCvyrEndAxis = m_cbCvyrEnd.GetCurSel();
	CStatus::Instance()->SetCGLength(m_nCGLength);
	CStatus::Instance()->SaveCFG();
	CDialogEx::OnOK();
}
