// EnvironmentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CGInspector.h"
#include "EnvironmentDlg.h"
#include "afxdialogex.h"
#include <UniUtility.h>
#include "Status.h"


// CEnvironmentDlg dialog

IMPLEMENT_DYNAMIC(CEnvironmentDlg, CDialogEx)

CEnvironmentDlg::CEnvironmentDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ENVIRONMENT, pParent)
	, m_iCvySpeed(0)
	, m_strG1WSRFile(_T(""))
	, m_strDataSaveFolder(_T(""))
	, m_iAccDecel(0)
{

}

CEnvironmentDlg::~CEnvironmentDlg()
{
}

void CEnvironmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_CVYSPEED, m_iCvySpeed);
	DDX_Text(pDX, IDC_ET_G1WPFILE, m_strG1WSRFile);
	DDX_Text(pDX, IDC_EDT_DATASAVEFOLDER, m_strDataSaveFolder);
	DDX_Text(pDX, IDC_EDT_ACCDCEL, m_iAccDecel);
	DDV_MinMaxUInt(pDX, m_iAccDecel, 10, 40);
}


BEGIN_MESSAGE_MAP(CEnvironmentDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEnvironmentDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SAVEIMGFOLDER, &CEnvironmentDlg::OnBnClickedBtnSaveimgfolder)
	ON_EN_CHANGE(IDC_EDT_ACCDCEL, &CEnvironmentDlg::OnEnChangeEdtAccdcel)
	ON_BN_CLICKED(IDC_BTN_GP1WPSEL, &CEnvironmentDlg::OnBnClickedBtnGp1wpsel)
END_MESSAGE_MAP()


// CEnvironmentDlg message handlers


void CEnvironmentDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString MachineInputName, MachineOutputName, MachineInputLableName, MachineOutputLabelName, strProductName;
	CCfgFile* pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	pMyCfg->Open();
	pMyCfg->SetDepth(2);

	strProductName = PRODUCT_NAME;
	MachineInputName.Format(_T("%s%s"), strProductName, _T("_INPUTSET"));
	MachineOutputName.Format(_T("%s%s"), strProductName, _T("_OUTPUTSET"));
	MachineInputLableName = MachineInputName + _T("_NAME");
	MachineOutputLabelName = MachineOutputName + _T("_NAME");

	//! Front, End Conveyor Speed and Work End and Velocity, Max Speed parameter Saving
	pMyCfg->Set((long)m_iCvySpeed, (TCHAR*)(LPCTSTR)MachineInputName, _T("FrontConveyor-Velocity-"));
	pMyCfg->Set((long)m_iAccDecel, (TCHAR*)(LPCTSTR)MachineInputName, _T("FrontConveyor-Accelerate-"));
	for (int i = 0; i < 4; i++)
	{
		CStatus::Instance()->m_paramCvyMotion[i].vel = m_iCvySpeed;
		CStatus::Instance()->m_paramCvyMotion[i].acc = m_iAccDecel;
		CStatus::Instance()->m_paramCvyMotion[i].dec = m_iAccDecel;
	}

	CStatus::Instance()->m_strWorkSpaceRuntime = m_strG1WSRFile;
	pMyCfg->Set(m_strG1WSRFile.GetBuffer(), _T("Vision"), _T("WorkSpaceRuntime"));

	pMyCfg->Set(CStatus::Instance()->m_strDataSaveFolder.GetBuffer(), _T("Vision"), _T("DataImageSaveFolder"));

	pMyCfg->Flush();

	CDialogEx::OnOK();
}


BOOL CEnvironmentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CString MachineInputName, MachineOutputName, MachineInputLableName, MachineOutputLabelName, ProductName;
	CCfgFile* pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	pMyCfg->Open();
	pMyCfg->SetDepth(2);

	ProductName = PRODUCT_NAME;
	MachineInputName = ProductName + _T("_INPUTSET");
	MachineOutputName = ProductName + _T("_OUTPUTSET");
	MachineInputLableName = MachineInputName + _T("_NAME");
	MachineOutputLabelName = MachineOutputName + _T("_NAME");

	// Front, End Conveyor Speed and Work End and Velocity, Max Speed parameter Loading
	m_iCvySpeed = pMyCfg->Get(180000L, (TCHAR*)(LPCTSTR)MachineInputName, _T("FrontConveyor-Velocity-"));
	m_iAccDecel = pMyCfg->Get(20L, (TCHAR*)(LPCTSTR)MachineInputName, _T("FrontConveyor-Accelerate-"));
	m_strG1WSRFile = CStatus::Instance()->m_strWorkSpaceRuntime = pMyCfg->Get(CStatus::Instance()->m_strWorkSpaceRuntime.GetBuffer(), _T("Vision"), _T("WorkSpaceRuntime"));
	m_strDataSaveFolder = pMyCfg->Get(CStatus::Instance()->m_strDataSaveFolder.GetBuffer(), _T("Vision"), _T("DataImageSaveFolder"));

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CEnvironmentDlg::OnBnClickedBtnSaveimgfolder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFolderDialog FolderDlg(_T("Select the Data Image folder."), CStatus::Instance()->GetModelRepository().GetBuffer());
	CString SaveFolder;
	switch (FolderDlg.DoModal())
	{
	case IDOK:
		SaveFolder = FolderDlg.GetFolderPath();
		m_strDataSaveFolder = CStatus::Instance()->m_strDataSaveFolder = SaveFolder;
		UpdateData(FALSE);
		break;
	case IDCANCEL:
		break;
	}
}



void CEnvironmentDlg::OnEnChangeEdtAccdcel()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CEnvironmentDlg::OnBnClickedBtnGp1wpsel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog LoadFile(TRUE, _T(""), _TEXT("*.vrws"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _TEXT("Runtime Workspace|*.vrws"));
	CString strWorkDir = CAppInfo::GetAppDir();
	LoadFile.m_ofn.lpstrInitialDir = strWorkDir;
	if (LoadFile.DoModal() == IDOK)
	{
		m_strG1WSRFile = LoadFile.GetFileName();
	}
	UpdateData(FALSE);
}
