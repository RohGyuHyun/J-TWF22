// TeachingDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "XFMS.h"
#include "XFMSDoc.h"
#include "XFMSView.h"
#include "MainFrm.h"
#include "TeachingDlg.h"
#include "CamDlg.h"
#include "afxdialogex.h"
#include "Status.h"


// CTeachingDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTeachingDlg, CDialogEx)

CTeachingDlg::CTeachingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEACHING, pParent)
	, m_iTeachingMode(0)
{

}

CTeachingDlg::~CTeachingDlg()
{
}

void CTeachingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_MODESETFID, m_iTeachingMode);
	DDX_Control(pDX, IDC_RETOPMODE, m_cOPMode);
	DDX_Control(pDX, IDC_BTN_TRIAL, m_cTrial);
}


BEGIN_MESSAGE_MAP(CTeachingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RETOPMODE, &CTeachingDlg::OnBnClickedRetopmode)
	ON_BN_CLICKED(IDC_MODESETFID, &CTeachingDlg::OnBnClickedModes)
	ON_BN_CLICKED(IDC_MODESETHT, &CTeachingDlg::OnBnClickedModes)
	ON_BN_CLICKED(IDC_MODEALIGN, &CTeachingDlg::OnBnClickedModes)
	ON_BN_CLICKED(IDC_MODEHEIGHT, &CTeachingDlg::OnBnClickedModes)
	ON_BN_CLICKED(IDC_MODEGAPGAGE, &CTeachingDlg::OnBnClickedModes)
	ON_BN_CLICKED(IDC_BTN_TRIAL, &CTeachingDlg::OnBnClickedBtnTrial)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTeachingDlg �޽��� ó�����Դϴ�.


void CTeachingDlg::OnBnClickedRetopmode()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	pView->SetOPMode(CStatus::OPSTOP);


}

BOOL CTeachingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_iTeachingMode = 0;
	SetWindowTheme(GetDlgItem(IDC_MODESETFID)->m_hWnd, _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_MODESETHT)->m_hWnd, _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_MODEALIGN)->m_hWnd, _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_MODEHEIGHT)->m_hWnd, _T(""), _T(""));
	SetWindowTheme(GetDlgItem(IDC_MODEGAPGAGE)->m_hWnd, _T(""), _T(""));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTeachingDlg::OnBnClickedModes()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	CStatus::Instance()->SetCurAddAlgoIndex(m_iTeachingMode);
}


void CTeachingDlg::OnBnClickedBtnTrial()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CXFMSView *pView = (CXFMSView *)pFrame->GetActiveView();
	pView->DoTrial();

}



BOOL CTeachingDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));

	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH CTeachingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	switch (nCtlColor)
	{
	case CTLCOLOR_BTN:    // ��ư�� ������ ��������...
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	case CTLCOLOR_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(90, 107, 135));  // static text ���ڻ� ����
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}
