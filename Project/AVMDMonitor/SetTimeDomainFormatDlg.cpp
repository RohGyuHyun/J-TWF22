// SetTimeDomainFormatDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "SetTimeDomainFormatDlg.h"
#include "afxdialogex.h"
#include "Status.h"

// CSetTimeDomainFormatDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSetTimeDomainFormatDlg, CDialogEx)

CSetTimeDomainFormatDlg::CSetTimeDomainFormatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SET_TIME_DOMAIN_FORMAT, pParent)
{

}

CSetTimeDomainFormatDlg::~CSetTimeDomainFormatDlg()
{
}

void CSetTimeDomainFormatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TIME, m_ctrl_combo_TimeDomain);
}


BEGIN_MESSAGE_MAP(CSetTimeDomainFormatDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetTimeDomainFormatDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetTimeDomainFormatDlg �޽��� ó�����Դϴ�.


BOOL CSetTimeDomainFormatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_ctrl_combo_TimeDomain.AddString(_T("GAL"));
	m_ctrl_combo_TimeDomain.AddString(_T("g-Force"));
	
	if(CStatus::Instance()->m_strTimeDomainUnit == "GAL")
		m_ctrl_combo_TimeDomain.SetCurSel(0);
	if (CStatus::Instance()->m_strTimeDomainUnit == "g-Force")
		m_ctrl_combo_TimeDomain.SetCurSel(1);
	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CSetTimeDomainFormatDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strTimedoMain;
	m_ctrl_combo_TimeDomain.GetLBText(this->m_ctrl_combo_TimeDomain.GetCurSel(), strTimedoMain);

	CStatus::Instance()->m_strTimeDomainUnit = strTimedoMain;
	//CStatus::Instance()->m_iRMSStandardValue = 0;
	//CStatus::Instance()->m_iXStandardValue = 0;
	//CStatus::Instance()->m_iYStandardValue = 0;
	//CStatus::Instance()->m_iZStandardValue = 0;
	//CStatus::Instance()->m_iCompareStandardValue = 0;
	CStatus::Instance()->DisplayDlgUpdateUI();
	CStatus::Instance()->SpectrumComboBoxUpdate();
	CStatus::Instance()->UpdateUITimeDomain();
	CStatus::Instance()->SetEventMode(CStatus::tEventMode::Evt_NONE);
	CStatus::Instance()->UpdateScrn();

	UpdateData(FALSE);
	CDialogEx::OnOK();
}
