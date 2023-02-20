// ReportDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "ReportDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "AGMDMonitorDoc.h"
#include "AGMDMonitorView.h"

// CReportDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CReportDlg, CDialogEx)

CReportDlg::CReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REPORT, pParent)
	, m_nReportData(0)
{

}

CReportDlg::~CReportDlg()
{
}

void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NREPORTDATA, m_nReportData);
	DDX_Text(pDX, IDC_NREPORTDATA, m_nReportData);
}


BEGIN_MESSAGE_MAP(CReportDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTNLOAD, &CReportDlg::OnBnClickedBtnload)
	ON_BN_CLICKED(IDC_BTN_DATAAPPLY, &CReportDlg::OnBnClickedBtnDataapply)
	ON_BN_CLICKED(IDOK, &CReportDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CReportDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CReportDlg �޽��� ó�����Դϴ�.
BOOL CReportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//Initialize();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CReportDlg::Initialize()
{
	UpdateData(TRUE);
	m_nReportData = 0;
	m_ExcelReport.SetFileName();
	m_ExcelReport.OpenExcelFile();
	m_ExcelReport.MakeHeader();
	
	UpdateData(FALSE);
}

void CReportDlg::OnBnClickedBtnload()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog FileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG|Files(*.*)|*.*|"));
	if (FileLoad.DoModal() == IDOK)
	{
		CStatus::Instance()->GetRepository()->ReadFromLogFile(FileLoad.GetPathName());
		tAGMDRecord *pMeaInfo = CStatus::Instance()->GetRepository()->GetMeasureInfo(CStatus::Instance()->GetRepository()->m_iDispIndex);
		CStatus::Instance()->SetMeasureData(pMeaInfo);
		CStatus::Instance()->OnInitBrowserUpdate();
		CStatus::Instance()->UpdateScrn();
	}
}

void CReportDlg::OnBnClickedBtnDataapply()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nCount = CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetSize();
	if (nCount == 0)
	{
		AfxMessageBox(_T("There is no data"));
		return;
	}
	if (m_nReportData == 7)
	{
		AfxMessageBox(_T("Maximum is 7 in report data"));
		return;
	}
	m_nReportData++;
	m_ExcelReport.InseartNewRecord(m_nReportData);
	UpdateData(FALSE);
}

void CReportDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!m_nReportData)
	{
		AfxMessageBox(_T("There is no data"));
		return;
	}
	CFileDialog FileSave(FALSE, _T("*.XLSX"), NULL, OFN_FILEMUSTEXIST, _T("EXCEL File|*.XLSX;|Files(*.*)|*.*|"));
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	if (FileSave.DoModal() == IDOK)
	{
		m_ExcelReport.DataReport();
		m_ExcelReport.RemoveDummy();
		m_ExcelReport.SetFileName(FileSave.GetPathName());
		m_ExcelReport.CloseExcelFile();
	}
	else
	{
		return;
	}
	CDialogEx::OnOK();
}


void CReportDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_ExcelReport.CloseExcelNoSave();
	CDialogEx::OnCancel();
}
