// SCHeightParamDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "XNPI.h"
#include "SCHeightParamDlg.h"
#include "SCAlgoDialog.h"
#include "SCHeightAlgo.h"


// CSCHeightParamDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSCHeightParamDlg, CSCAlgoDialog)

CSCHeightParamDlg * CSCHeightParamDlg::m_pInstance = NULL;

CSCHeightParamDlg::CSCHeightParamDlg(CWnd* pParent /*=NULL*/)
	: CSCAlgoDialog(IDD_SCHEIGHTPARAM, pParent)
{

}

CSCHeightParamDlg::~CSCHeightParamDlg()
{
}

void CSCHeightParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBHEIGHTDIR, m_cScanDir);
	DDX_Control(pDX, IDOK, m_cApply);
}


BEGIN_MESSAGE_MAP(CSCHeightParamDlg, CSCAlgoDialog)
	ON_BN_CLICKED(IDOK, &CSCHeightParamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSCHeightParamDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSCHeightParamDlg �޽��� ó�����Դϴ�.


CSCHeightParamDlg * CSCHeightParamDlg::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CSCHeightParamDlg;
		m_pInstance->Create(IDD_SCHEIGHTPARAM, NULL);
	}
	return m_pInstance;
}


BOOL CSCHeightParamDlg::OnInitDialog()
{
	CSCAlgoDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetDlgItemInt(IDC_INSPHEIGHTLOW, 0);
	SetDlgItemInt(IDC_INSPHEIGHTHI, 0);
	m_cScanDir.AddString(_T("Top->Down"));
	m_cScanDir.AddString(_T("Down->Top"));
	m_cScanDir.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CSCHeightParamDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	((CSCHeightAlgo *)m_pSCAlgo)->m_iCriteriaLow = GetDlgItemInt(IDC_INSPHEIGHTLOW);
	((CSCHeightAlgo *)m_pSCAlgo)->m_iCriteriaHi = GetDlgItemInt(IDC_INSPHEIGHTHI);
	((CSCHeightAlgo *)m_pSCAlgo)->m_iScanDir = m_cScanDir.GetCurSel();
	OnOK();
}


void CSCHeightParamDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}

void CSCHeightParamDlg::LoadParam(CSCAlgoBase * pAlgo)
{
	SetDlgItemInt(IDC_INSPHEIGHTLOW, ((CSCHeightAlgo *)pAlgo)->m_iCriteriaLow);
	SetDlgItemInt(IDC_INSPHEIGHTHI, ((CSCHeightAlgo *)pAlgo)->m_iCriteriaHi);
}