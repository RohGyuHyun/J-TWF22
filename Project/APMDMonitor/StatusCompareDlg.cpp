// StatusCompareDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "APMDMonitor.h"
#include "StatusCompareDlg.h"
#include "afxdialogex.h"
#include "Status.h"

struct tAPMDDispData;

// CStatusCompareDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CStatusCompareDlg, CDialogEx)

CStatusCompareDlg::CStatusCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STATUS_COMPARE, pParent)
{

}

CStatusCompareDlg::~CStatusCompareDlg()
{
}

void CStatusCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_STATCMP, m_lstStatChart);
}


BEGIN_MESSAGE_MAP(CStatusCompareDlg, CDialogEx)
END_MESSAGE_MAP()


// CStatusCompareDlg �޽��� ó�����Դϴ�.


BOOL CStatusCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_lstStatChart.SetExtendedStyle(m_lstStatChart.GetExStyle() || LVS_EX_GRIDLINES);
	m_lstStatChart.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 85);
	m_lstStatChart.InsertColumn(1, _T("Main"), LVCFMT_CENTER, 90);
	m_lstStatChart.InsertColumn(2, _T("Compare"), LVCFMT_CENTER, 90);
	m_lstStatChart.InsertColumn(3, _T("Main Acc"), LVCFMT_CENTER, 90);
	m_lstStatChart.InsertColumn(4, _T("Compare Acc"), LVCFMT_CENTER, 90);
	m_lstStatChart.InsertItem(0, _T("0.1um"));
	m_lstStatChart.InsertItem(1, _T("0.3um"));
	m_lstStatChart.InsertItem(2, _T("0.5um"));
	m_lstStatChart.InsertItem(3, _T("Sum"));
	m_lstStatChart.InsertItem(4, _T("Pressure"));
	m_lstStatChart.InsertItem(5, _T("Temperature"));
	m_lstStatChart.InsertItem(6, _T("Humidity"));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CStatusCompareDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


int CStatusCompareDlg::SetDispData(tAPMDDispData & dispMain, tAPMDDispData & dispCompare)
{
	SetDispCore(&dispMain, 0);
	SetDispCore(&dispCompare, 1);

	UpdateData(FALSE);
	return 0;
}


void CStatusCompareDlg::SetDispCore(tAPMDDispData * pAPMDDispData, int KindOfData)
{
	CString strTemp01, strTemp03, strTemp05, strHumi, strPressure, strTemperature;
	CString strAcc01, strAcc03, strAcc05, strAccSum;
	int AddPos = 0;
	if (KindOfData == 1)AddPos = 1;
	strTemp01.Format(_T("%d"), pAPMDDispData->iParticle01);
	strTemp03.Format(_T("%d"), pAPMDDispData->iParticle03);
	strTemp05.Format(_T("%d"), pAPMDDispData->iParticle05);
	strPressure.Format(_T("%.1f"), pAPMDDispData->dblPressure);
	strTemperature.Format(_T("%.2f"), pAPMDDispData->dblTemp);
	strHumi.Format(_T("%d%%"), pAPMDDispData->iHumi);
	m_lstStatChart.SetItem(0, 1 + AddPos, LVIF_TEXT, strTemp01, NULL, NULL, NULL, NULL);
	m_lstStatChart.SetItem(1, 1 + AddPos, LVIF_TEXT, strTemp03, NULL, NULL, NULL, NULL);
	m_lstStatChart.SetItem(2, 1 + AddPos, LVIF_TEXT, strTemp05, NULL, NULL, NULL, NULL);

	m_lstStatChart.SetItem(4, 1 + AddPos, LVIF_TEXT, strPressure, NULL, NULL, NULL, NULL);
	m_lstStatChart.SetItem(5, 1 + AddPos, LVIF_TEXT, strTemperature, NULL, NULL, NULL, NULL);
	m_lstStatChart.SetItem(6, 1 + AddPos, LVIF_TEXT, strHumi, NULL, NULL, NULL, NULL);

	strAcc01.Format(_T("%d"), pAPMDDispData->nAcc01);
	strAcc03.Format(_T("%d"), pAPMDDispData->nAcc03);
	strAcc05.Format(_T("%d"), pAPMDDispData->nAcc05);
	strAccSum.Format(_T("%d"), pAPMDDispData->nAcc01 + pAPMDDispData->nAcc03 + pAPMDDispData->nAcc05);
	m_lstStatChart.SetItem(0, 3 + AddPos, LVIF_TEXT, strAcc01, NULL, NULL, NULL, NULL);
	m_lstStatChart.SetItem(1, 3 + AddPos, LVIF_TEXT, strAcc03, NULL, NULL, NULL, NULL);
	m_lstStatChart.SetItem(2, 3 + AddPos, LVIF_TEXT, strAcc05, NULL, NULL, NULL, NULL);
	m_lstStatChart.SetItem(3, 3 + AddPos, LVIF_TEXT, strAccSum, NULL, NULL, NULL, NULL);
}
