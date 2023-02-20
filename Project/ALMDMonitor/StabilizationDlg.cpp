// StabilizationDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "StabilizationDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "Stabilization.h"

#define STABLE_LOW 0.002
#define STABLE_HIGH 1.000
#define STR_STABLE_RANGE _T("Enter the value from 0.002 to 1.000")

// CStabilizationDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CStabilizationDlg, CDialogEx)

CStabilizationDlg::CStabilizationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STABILIZATION, pParent)
	, m_dblStblzSensitivity(0)
	, m_dblSensitivity(0)
{

}

CStabilizationDlg::~CStabilizationDlg()
{
}

void CStabilizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_SESITIVITY, m_cStblzSensitivity);
	DDX_Text(pDX, IDC_EDT_SESITIVITY, m_dblSensitivity);
}


BEGIN_MESSAGE_MAP(CStabilizationDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CStabilizationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_STBLZDEFAULT, &CStabilizationDlg::OnBnClickedBtnStblzdefault)
END_MESSAGE_MAP()


// CStabilizationDlg �޽��� ó�����Դϴ�.


BOOL CStabilizationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_dblSensitivity = CStatus::Instance()->GetStblzSensitivity();
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CStabilizationDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!IsInTheRange())return;
	UpdateData(true);
	CStatus::Instance()->SetStblzSensitivity(m_dblSensitivity);
	CDialogEx::OnOK();
}


void CStabilizationDlg::OnBnClickedBtnStblzdefault()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_dblSensitivity = (CStabilization::Instance()->GetDefaultSensitivity()/1000.);
	UpdateData(false);
}


BOOL CStabilizationDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			if (pMsg->hwnd == GetDlgItem(IDC_EDT_SESITIVITY)->GetSafeHwnd())
			{
				StableEdtChange();
			}
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CStabilizationDlg::IsInTheRange()
{
	CString strNum;
	GetDlgItemText(IDC_EDT_TOL, strNum);
	double dblNum = _wtof(strNum.GetBuffer());
	if ((dblNum<STABLE_LOW) || (dblNum>STABLE_HIGH))
	{
		AfxMessageBox(STR_STABLE_RANGE);
		return FALSE;
	}
	return TRUE;
}


BOOL CStabilizationDlg::StableEdtChange()
{
	if (!IsInTheRange()) return FALSE;
	UpdateData(true);
	UpdateData(false);
	return TRUE;
}
