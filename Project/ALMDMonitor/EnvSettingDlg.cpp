// EnvSettingDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "EnvSettingDlg.h"
#include "afxdialogex.h"
#include "Status.h"

// CEnvSettingDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEnvSettingDlg, CDialogEx)

CEnvSettingDlg::CEnvSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ENVSETTING, pParent)
	, m_strSaveFolder(_T(""))
	, m_strDataFileName(_T(""))
{

}

CEnvSettingDlg::~CEnvSettingDlg()
{
}

void CEnvSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_SAVEFOLDER, m_strSaveFolder);
	DDX_Text(pDX, IDC_EDT_DFNAME, m_strDataFileName);
}


BEGIN_MESSAGE_MAP(CEnvSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SETFOLDER, &CEnvSettingDlg::OnBnClickedBtnSetfolder)
	ON_BN_CLICKED(IDOK, &CEnvSettingDlg::OnBnClickedOk)
//	ON_WM_PAINT()
END_MESSAGE_MAP()


// CEnvSettingDlg �޽��� ó�����Դϴ�.


void CEnvSettingDlg::OnBnClickedBtnSetfolder()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFolderDialog FolderDlg(_T("Select the log data save folder."), CStatus::Instance()->GetDataSaveFolder().GetBuffer());
	CString SaveFolder;
	switch(FolderDlg.DoModal())
	{
	case IDOK:
		SaveFolder = FolderDlg.GetFolderPath();
		m_strSaveFolder = SaveFolder;
		UpdateData(FALSE);
		break;
	case IDCANCEL:
		break;
	}


}


BOOL CEnvSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_strSaveFolder = CStatus::Instance()->GetDataSaveFolder();
	m_strDataFileName = CStatus::Instance()->GetDataFileName();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CEnvSettingDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	CStatus::Instance()->SetDataSaveFolder(m_strSaveFolder.GetBuffer());
	CStatus::Instance()->SetDataFileName(m_strDataFileName.GetBuffer());
	CStatus::Instance()->SetDefault();
	CDialogEx::OnOK();
}

