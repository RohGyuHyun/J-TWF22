// BrowserDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "BrowserDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include <locale.h>
// CBrowserDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CBrowserDlg, CDialogEx)

CBrowserDlg::CBrowserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BROWSERDLG, pParent)
	, m_strBrowser(_T(""))
	, m_strDisplayNum(_T(""))
	, m_strTotalDisplayNum(_T(""))
{
	CStatus::Instance()->SetBrowserDlg(this);
}

CBrowserDlg::~CBrowserDlg()
{
}

void CBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_CB_BROWSER, m_strBrowser);
	DDX_Text(pDX, IDC_EDIT_DISPLAY_NUM, m_strDisplayNum);
	DDX_Text(pDX, IDC_STATIC_TOTAL_DISPLAY_NUM, m_strTotalDisplayNum);
	DDX_Control(pDX, IDC_CB_BROWSER, m_ctrl_CBBrowser);
}


BEGIN_MESSAGE_MAP(CBrowserDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_PREV, &CBrowserDlg::OnBnClickedBtnPrev)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CBrowserDlg::OnBnClickedBtnNext)
	ON_MESSAGE(WM_INITIALIZE, &CBrowserDlg::OnInitBrowser)
	ON_MESSAGE(WM_DLG_INITIALIZE, &CBrowserDlg::OnDlgInit)
	ON_CBN_SELCHANGE(IDC_CB_BROWSER, &CBrowserDlg::OnCbnSelchangeCbBrowser)
END_MESSAGE_MAP()


// CBrowserDlg �޽��� ó�����Դϴ�.


void CBrowserDlg::OnBnClickedBtnPrev()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iDispIndex = CStatus::Instance()->GetRepository()->m_iDispIndex;
	int nCount = CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetSize();
	if (nCount == 0)
		return;
	if (iDispIndex == 0)
		return;
	CStatus::Instance()->GetRepository()->m_iDispIndex--;
	tAGMDRecord *pMeaInfo = CStatus::Instance()->GetRepository()->GetMeasureInfo(CStatus::Instance()->GetRepository()->m_iDispIndex);
	CStatus::Instance()->SetMeasureData(pMeaInfo);
	CStatus::Instance()->OnInitBrowserUpdate();
	CStatus::Instance()->StationInfoUpdate();
	CStatus::Instance()->UpdateScrn();
}


void CBrowserDlg::OnBnClickedBtnNext()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	int iDispIndex = CStatus::Instance()->GetRepository()->m_iDispIndex;
	int nCount = CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetSize();
	if (nCount == 0)
		return;
	if ((iDispIndex + 1) == nCount)
		return;

	CStatus::Instance()->GetRepository()->m_iDispIndex++;
	tAGMDRecord *pMeaInfo = CStatus::Instance()->GetRepository()->GetMeasureInfo(CStatus::Instance()->GetRepository()->m_iDispIndex);
	CStatus::Instance()->SetMeasureData(pMeaInfo);
	CStatus::Instance()->OnInitBrowserUpdate();
	CStatus::Instance()->StationInfoUpdate();
	CStatus::Instance()->UpdateScrn();
}

#pragma warning(disable: 4996)
void CBrowserDlg::BrowserUpdate()
{
	int nCount = CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetSize();
	int nDisplayNum = CStatus::Instance()->GetRepository()->m_iDispIndex;
	m_strDisplayNum.Format(_T("%d"), nDisplayNum+1);
	m_strTotalDisplayNum.Format(_T("of %d"), nCount);

	m_ctrl_CBBrowser.ResetContent();
	POSITION pos;
	tAGMDRecord *pNewRecord;
	CString strBrowser, strDate, strTime;
	struct tm RecordDateTime;
	for (pos = CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetHeadPosition(); pos != NULL;)
	{
		pNewRecord = (tAGMDRecord *)CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetNext(pos);
		RecordDateTime = *localtime(&(pNewRecord->CurTime));

		strDate.Format(_T("%04d-%02d-%02d"), RecordDateTime.tm_year + 1900, RecordDateTime.tm_mon + 1, RecordDateTime.tm_mday); // ���� �� ��ȯ
		strTime.Format(_T("%02d:%02d:%02d"), RecordDateTime.tm_hour, RecordDateTime.tm_min, RecordDateTime.tm_sec);
		strBrowser = strDate + _T(" ") + strTime;

		m_ctrl_CBBrowser.AddString(strBrowser);
	}
	m_ctrl_CBBrowser.SetCurSel(nDisplayNum);
	UpdateData(FALSE);
}

LRESULT CBrowserDlg::OnInitBrowser(WPARAM wParam, LPARAM lParam)
{
	BrowserUpdate();
	return 0;
}

LRESULT CBrowserDlg::OnDlgInit(WPARAM wParam, LPARAM lParam)
{
	m_ctrl_CBBrowser.ResetContent();
	SetDlgItemText(IDC_STATIC_TOTAL_DISPLAY_NUM, _T(""));
	SetDlgItemText(IDC_EDIT_DISPLAY_NUM, _T(""));
	return 0;
}

void CBrowserDlg::OnCbnSelchangeCbBrowser()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	int nPos = m_ctrl_CBBrowser.GetCurSel();

	CStatus::Instance()->GetRepository()->m_iDispIndex = nPos;
	tAGMDRecord *pMeaInfo = CStatus::Instance()->GetRepository()->GetMeasureInfo(CStatus::Instance()->GetRepository()->m_iDispIndex);
	CStatus::Instance()->SetMeasureData(pMeaInfo);
	CStatus::Instance()->OnInitBrowserUpdate();
	CStatus::Instance()->UpdateScrn();
}


BOOL CBrowserDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		// ���⿡ ���ϴ� ������ �ڵ带 ����
		return true;
	}


	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN))
	{
		CString strDisplayNum;
		int iDisplayNum;
		GetDlgItemText(IDC_EDIT_DISPLAY_NUM, strDisplayNum);
		iDisplayNum = _ttoi(strDisplayNum);

		int nCount = CStatus::Instance()->GetRepository()->m_DataRecords.m_AGMDRecords.GetSize();
		if (nCount == 0)
			return TRUE;
		
		if (iDisplayNum<0 || iDisplayNum>nCount)
			return TRUE;
		CStatus::Instance()->GetRepository()->m_iDispIndex = iDisplayNum - 1;
		tAGMDRecord *pMeaInfo = CStatus::Instance()->GetRepository()->GetMeasureInfo(CStatus::Instance()->GetRepository()->m_iDispIndex);
		CStatus::Instance()->SetMeasureData(pMeaInfo);
		CStatus::Instance()->OnInitBrowserUpdate();
		CStatus::Instance()->UpdateScrn();
		return TRUE;
	}
		
	return CDialogEx::PreTranslateMessage(pMsg);
}
