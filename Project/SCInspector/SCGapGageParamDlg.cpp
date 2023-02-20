// SCAlignParamDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "SCGapGageParamDlg.h"
#include "SCAlgoDialog.h"
#include "SCGapGageAlgo.h"

// CSCGapGageParamDlg ��ȭ �����Դϴ�.

CSCGapGageParamDlg * CSCGapGageParamDlg::m_pInstance = NULL;

IMPLEMENT_DYNAMIC(CSCGapGageParamDlg, CSCAlgoDialog)

CSCGapGageParamDlg::CSCGapGageParamDlg(CWnd* pParent /*=NULL*/)
	: CSCAlgoDialog(CSCGapGageParamDlg::IDD, pParent)
{
}

CSCGapGageParamDlg::~CSCGapGageParamDlg()
{
}

void CSCGapGageParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELSCANDIR, m_cSelScanDir);
	DDX_Control(pDX, IDOK, m_cApply);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
}


BEGIN_MESSAGE_MAP(CSCGapGageParamDlg, CSCAlgoDialog)
	ON_BN_CLICKED(IDOK, &CSCGapGageParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSCGapGageParamDlg �޽��� ó�����Դϴ�.


CSCGapGageParamDlg * CSCGapGageParamDlg::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CSCGapGageParamDlg;
		m_pInstance->Create(IDD_SCALIGNPARAM, NULL);
	}
	return m_pInstance;
}


BOOL CSCGapGageParamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_cSelScanDir.AddString(_T("Horizontal"));
	m_cSelScanDir.AddString(_T("Vertical"));
	SetDlgItemInt(IDC_INSPALIGNVAL, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CSCGapGageParamDlg::LoadParam(CSCAlgoBase * pAlgo)
{
	m_cSelScanDir.SetCurSel(((CSCGapGageAlgo *)pAlgo)->m_iScanDir);
	SetDlgItemInt(IDC_INSPALIGNVAL, ((CSCGapGageAlgo *)pAlgo)->m_iCriteria);
}


void CSCGapGageParamDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	((CSCGapGageAlgo *)m_pSCAlgo)->m_iScanDir = m_cSelScanDir.GetCurSel();
	((CSCGapGageAlgo *)m_pSCAlgo)->m_iCriteria = GetDlgItemInt(IDC_INSPALIGNVAL);
	OnOK();
}
