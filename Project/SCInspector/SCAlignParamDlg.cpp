// SCAlignParamDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "SCAlignParamDlg.h"
#include "SCAlgoDialog.h"
#include "SCAlignAlgo.h"

// CSCAlignParamDlg ��ȭ �����Դϴ�.

CSCAlignParamDlg * CSCAlignParamDlg::m_pInstance = NULL;

IMPLEMENT_DYNAMIC(CSCAlignParamDlg, CSCAlgoDialog)

CSCAlignParamDlg::CSCAlignParamDlg(CWnd* pParent /*=NULL*/)
	: CSCAlgoDialog(CSCAlignParamDlg::IDD, pParent)
{
}

CSCAlignParamDlg::~CSCAlignParamDlg()
{
}

void CSCAlignParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELSCANDIR, m_cSelScanDir);
	DDX_Control(pDX, IDOK, m_cApply);
	DDX_Control(pDX, IDCANCEL, m_cCancel);
}


BEGIN_MESSAGE_MAP(CSCAlignParamDlg, CSCAlgoDialog)
	ON_BN_CLICKED(IDOK, &CSCAlignParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSCAlignParamDlg �޽��� ó�����Դϴ�.


CSCAlignParamDlg * CSCAlignParamDlg::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CSCAlignParamDlg;
		m_pInstance->Create(IDD_SCALIGNPARAM, NULL);
	}
	return m_pInstance;
}


BOOL CSCAlignParamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_cSelScanDir.AddString(_T("Horizontal"));
	m_cSelScanDir.AddString(_T("Vertical"));
	SetDlgItemInt(IDC_INSPALIGNVAL, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CSCAlignParamDlg::LoadParam(CSCAlgoBase * pAlgo)
{
	m_cSelScanDir.SetCurSel(((CSCAlignAlgo *)pAlgo)->m_iScanDir);
	SetDlgItemInt(IDC_INSPALIGNVAL, ((CSCAlignAlgo *)pAlgo)->m_iCriteria);
}


void CSCAlignParamDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	((CSCAlignAlgo *)m_pSCAlgo)->m_iScanDir = m_cSelScanDir.GetCurSel();
	((CSCAlignAlgo *)m_pSCAlgo)->m_iCriteria = GetDlgItemInt(IDC_INSPALIGNVAL);
	OnOK();
}
