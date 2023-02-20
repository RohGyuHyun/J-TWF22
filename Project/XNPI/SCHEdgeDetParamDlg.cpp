// SCHeightParamDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "XNPI.h"
#include "SCHEdgeDetParamDlg.h"
#include "SCAlgoDialog.h"
#include "SCHEdgeDetAlgo.h"


// CSCHEdgeDetParamDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSCHEdgeDetParamDlg, CSCAlgoDialog)

CSCHEdgeDetParamDlg * CSCHEdgeDetParamDlg::m_pInstance = NULL;

CSCHEdgeDetParamDlg::CSCHEdgeDetParamDlg(CWnd* pParent /*=NULL*/)
	: CSCAlgoDialog(IDD_SCHEIGHTPARAM, pParent)
{

}

CSCHEdgeDetParamDlg::~CSCHEdgeDetParamDlg()
{
}

void CSCHEdgeDetParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBHEIGHTDIR, m_cHEDetDir);
	DDX_Control(pDX, IDOK, m_cApply);
	DDX_Control(pDX, IDCANCEL2, m_cCancel);
}


BEGIN_MESSAGE_MAP(CSCHEdgeDetParamDlg, CSCAlgoDialog)
	ON_BN_CLICKED(IDOK, &CSCHEdgeDetParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSCHEdgeDetParamDlg �޽��� ó�����Դϴ�.


CSCHEdgeDetParamDlg * CSCHEdgeDetParamDlg::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CSCHEdgeDetParamDlg;
		m_pInstance->Create(IDD_SCHEDGEDETPARAM, NULL);
	}
	return m_pInstance;
}


BOOL CSCHEdgeDetParamDlg::OnInitDialog()
{
	CSCAlgoDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_cHEDetDir.AddString(_T("Top->Down"));
	m_cHEDetDir.AddString(_T("Down->Top"));
	CheckDlgButton(IDC_CHK_USE_CNTRLINE, 0);
	m_cHEDetDir.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CSCHEdgeDetParamDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	((CSCHEdgeDetAlgo *)m_pSCAlgo)->m_bUseCenterLine = IsDlgButtonChecked(IDC_CHK_USE_CNTRLINE);
	((CSCHEdgeDetAlgo *)m_pSCAlgo)->m_iHEdgeScanDir = m_cHEDetDir.GetCurSel();
	OnOK();
}

void CSCHEdgeDetParamDlg::LoadParam(CSCAlgoBase * pAlgo)
{
	m_cHEDetDir.SetCurSel(((CSCHEdgeDetAlgo *)pAlgo)->m_iHEdgeScanDir);
	CheckDlgButton(IDC_CHK_USE_CNTRLINE, ((CSCHEdgeDetAlgo *)pAlgo)->m_bUseCenterLine);
}