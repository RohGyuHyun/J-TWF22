// ConfigDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CGInspector.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"
#include "Status.h"


// CConfigDlg 대화 상자입니다.

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


// CConfigDlg 메시지 처리기입니다.


void CConfigDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CStatus::Instance()->LoadCFG();
	EndDialog(-1);
}


BOOL CConfigDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CDialogEx::OnNotify(wParam, lParam, pResult);
}


BOOL CConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CConfigDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CStatus::Instance()->m_nCvyrFrontAxis = m_cbCvyrFront.GetCurSel();
	CStatus::Instance()->m_nCvyrEndAxis = m_cbCvyrEnd.GetCurSel();
	CStatus::Instance()->SetCGLength(m_nCGLength);
	CStatus::Instance()->SaveCFG();
	CDialogEx::OnOK();
}
