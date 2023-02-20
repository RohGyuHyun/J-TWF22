// ReportDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "ReportDlg.h"
#include "afxdialogex.h"
#include "ALMDMonitorDoc.h"
#include "ALMDMonitorView.h"
#include "MainFrm.h"
#include "Status.h"


// CReportDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CReportDlg, CDialogEx)

CReportDlg::CReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REPORT, pParent)
	, m_nReportData(0)
	, m_strExcelFile(_T(""))
	, m_bIsSelectLog(FALSE)
{

}

CReportDlg::~CReportDlg()
{
}

void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NREPORTDATA, m_nReportData);
}


BEGIN_MESSAGE_MAP(CReportDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTNLOAD, &CReportDlg::OnBnClickedBtnload)
	ON_BN_CLICKED(IDC_BTN_DATAAPPLY, &CReportDlg::OnBnClickedBtnDataapply)
	ON_BN_CLICKED(IDOK, &CReportDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CReportDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CReportDlg �޽��� ó�����Դϴ�.


void CReportDlg::OnBnClickedBtnload()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog FileLoad(TRUE, _T("*.LOG"), NULL, OFN_FILEMUSTEXIST, _T("DATA File|*.LOG|Files(*.*)|*.*|"));
	if (FileLoad.DoModal() == IDOK)
	{
		m_bIsSelectLog = TRUE;
		CString strFileName = FileLoad.GetPathName();
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CALMDMonitorDoc *pDoc = (CALMDMonitorDoc *)pFrame->GetActiveDocument();
		pDoc->OnOpenDocument(strFileName);
	}
}


void CReportDlg::OnBnClickedBtnDataapply()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CALMDMonitorView *pView = (CALMDMonitorView *)pFrame->GetActiveView();
	if (m_nReportData == 7)
	{
		AfxMessageBox(_T("Maximum is 7 in report data"));
		return;
	}
	if (!m_bIsSelectLog)
	{
		AfxMessageBox(_T("Please select the LOG file"));
		return;
	}
	pView->m_ExcelReport.InseartNewRecord(++m_nReportData);
	UpdateData(FALSE);
}


BOOL CReportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CReportDlg::Initialize(CString strExcelFile)
{
	m_strExcelFile = strExcelFile;
	m_nReportData = 0;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CALMDMonitorView *pView = (CALMDMonitorView *)pFrame->GetActiveView();
	pView->m_ExcelReport.SetFileName(strExcelFile);
	pView->m_ExcelReport.OpenExcelFile();
	pView->m_ExcelReport.MakeHeader();
	m_bIsSelectLog = FALSE;
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
	CALMDMonitorView *pView = (CALMDMonitorView *)pFrame->GetActiveView();
	if (FileSave.DoModal() == IDOK)
	{
		pView->m_ExcelReport.MakeChart();
		pView->m_ExcelReport.RemoveDummy();
		pView->m_ExcelReport.SetFileName(FileSave.GetPathName());
		pView->m_ExcelReport.CloseExcelFile();
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
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CALMDMonitorView *pView = (CALMDMonitorView *)pFrame->GetActiveView();
	pView->m_ExcelReport.CloseExcelNoSave();
	CDialogEx::OnCancel();
}
