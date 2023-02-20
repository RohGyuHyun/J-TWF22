// StationInfomationDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "StationInfomationDlg.h"
#include "afxdialogex.h"
#include "Repository.h"
#include "Status.h"

// CStationInfomationDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CStationInfomationDlg, CDialogEx)

CStationInfomationDlg::CStationInfomationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STATIONINFOMATION, pParent)
	, m_strOperator(_T(""))
	, m_strTool(_T(""))
	, m_strStation(_T(""))
	, m_strComment(_T(""))
{
	CStatus::Instance()->SetStationInfomationDlg(this);
}

CStationInfomationDlg::~CStationInfomationDlg()
{
}

void CStationInfomationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPERATOR, m_strOperator);
	DDX_Text(pDX, IDC_EDIT_TOOL, m_strTool);
	DDX_Text(pDX, IDC_EDIT_STATION, m_strStation);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
}


BEGIN_MESSAGE_MAP(CStationInfomationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CStationInfomationDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CStationInfomationDlg::OnBnClickedBtnApply)
	ON_MESSAGE(WM_WORK_UPDATE, &CStationInfomationDlg::WorkUpdate)
END_MESSAGE_MAP()


// CStationInfomationDlg �޽��� ó�����Դϴ�.


void CStationInfomationDlg::OnBnClickedBtnClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_strOperator.Empty();
	m_strTool.Empty();
	m_strStation.Empty();
	m_strComment.Empty();
	UpdateData(FALSE);
}


void CStationInfomationDlg::OnBnClickedBtnApply()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	tWorkEnvInfo WorkEnvInfo;
	wsprintf(WorkEnvInfo.szOperator, m_strOperator.GetBuffer());
	wsprintf(WorkEnvInfo.szTool, m_strTool.GetBuffer());
	wsprintf(WorkEnvInfo.szStation, m_strStation.GetBuffer());
	wsprintf(WorkEnvInfo.szComment, m_strComment.GetBuffer());
	CStatus::Instance()->SetWorkEnvInfo(&WorkEnvInfo);
}


BOOL CStationInfomationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CStationInfomationDlg::Initialize()
{
	if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_REVIEW)
	{
		GetDlgItem(IDC_BTN_CLEAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_APPLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OPERATOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TOOL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMMENT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_CLEAR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_APPLY)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OPERATOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TOOL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMMENT)->EnableWindow(TRUE);
	}
	tWorkEnvInfo WorkEnvInfo = CStatus::Instance()->GetWorkEnvInfo();
	m_strOperator = WorkEnvInfo.szOperator;
	m_strTool = WorkEnvInfo.szTool;
	m_strStation = WorkEnvInfo.szStation;
	m_strComment = WorkEnvInfo.szComment;
	UpdateData(FALSE);
}

LRESULT CStationInfomationDlg::WorkUpdate(WPARAM wParam, LPARAM lParam)
{
	tWorkEnvInfo WorkEnvInfo = CStatus::Instance()->GetWorkEnvInfo();
	m_strOperator = WorkEnvInfo.szOperator;
	m_strTool = WorkEnvInfo.szTool;
	m_strStation = WorkEnvInfo.szStation;
	m_strComment = WorkEnvInfo.szComment;
	UpdateData(FALSE);
	return 0;
}