// Cal2Dlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "Cal2Dlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "MonitorDlg.h"


// CCal2Dlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCal2Dlg, CDialogEx)

CCal2Dlg::CCal2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAL2, pParent)
	, m_iNewCal1(0)
	, m_iNewCal2(0)
	, m_iNewCal3(0)
	, m_iNewJigGap(0)
{

}

CCal2Dlg::~CCal2Dlg()
{
}

void CCal2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_CALIBRATIONRESULT, m_cResult);
}


BEGIN_MESSAGE_MAP(CCal2Dlg, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CCal2Dlg �޽��� ó�����Դϴ�.


void CCal2Dlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CalculateCalData();
	MakeChart();
}


void CCal2Dlg::MakeChart()
{
	int iCalGap1, iCalGap2, iCalGap3, iJigGap;
	iJigGap = CStatus::Instance()->m_CalData.uJigGap;
	iCalGap1 = CStatus::Instance()->m_CalData.iGap1;
	iCalGap2 = CStatus::Instance()->m_CalData.iGap2;
	iCalGap3 = CStatus::Instance()->m_CalData.iGap3;
	CString strCalGap1, strCalGap2, strCalGap3, strJigGap;
	CString strNewCalGap1, strNewCalGap2, strNewCalGap3, strNewJigGap;
	strCalGap1.Format(_T("%d"), iCalGap1);
	strCalGap2.Format(_T("%d"), iCalGap2);
	strCalGap3.Format(_T("%d"), iCalGap3);
	strJigGap.Format(_T("%d"), iJigGap);

	strNewCalGap1.Format(_T("%d"), m_iNewCal1);
	strNewCalGap2.Format(_T("%d"), m_iNewCal2);
	strNewCalGap3.Format(_T("%d"), m_iNewCal3);
	strNewJigGap.Format(_T("%d"), m_iNewJigGap);


	m_cResult.SetItem(0, 1, LVIF_TEXT, strJigGap, 0, 0, 0, NULL);
	m_cResult.SetItem(0, 2, LVIF_TEXT, strCalGap1, 0, 0, 0, NULL);
	m_cResult.SetItem(0, 3, LVIF_TEXT, strCalGap2, 0, 0, 0, NULL);
	m_cResult.SetItem(0, 4, LVIF_TEXT, strCalGap3, 0, 0, 0, NULL);

	m_cResult.SetItem(1, 1, LVIF_TEXT, strNewJigGap, 0, 0, 0, NULL);
	m_cResult.SetItem(1, 2, LVIF_TEXT, strNewCalGap1, 0, 0, 0, NULL);
	m_cResult.SetItem(1, 3, LVIF_TEXT, strNewCalGap2, 0, 0, 0, NULL);
	m_cResult.SetItem(1, 4, LVIF_TEXT, strNewCalGap3, 0, 0, 0, NULL);

}


void CCal2Dlg::MakeChartFrame()
{
	m_cResult.DeleteAllItems();
	m_cResult.DeleteAllItems();
	m_cResult.SetExtendedStyle(LVS_EX_GRIDLINES);

	m_cResult.InsertColumn(0, _T("Item"), LVCFMT_CENTER, 90, -1);
	m_cResult.InsertColumn(1, _T("JigGap"), LVCFMT_RIGHT, 70, -1);
	m_cResult.InsertColumn(2, _T("GAP1"), LVCFMT_RIGHT, 70, -1);
	m_cResult.InsertColumn(3, _T("GAP2"), LVCFMT_RIGHT, 70, -1);
	m_cResult.InsertColumn(4, _T("GAP3"), LVCFMT_RIGHT, 70, -1);

	m_cResult.InsertItem(1, _T("Before"));
	m_cResult.InsertItem(2, _T("After"));
}


void CCal2Dlg::CalculateCalData()
{
	int iGap1, iGap2, iGap3;
	int iOrgGap1, iOrgGap2, iOrgGap3;
	int iOldCal1, iOldCal2, iOldCal3;

	m_iNewJigGap = CStatus::Instance()->m_iNewJigGap;
	iGap1 = CStatus::Instance()->m_curMeasureData.usGap1;
	iGap2 = CStatus::Instance()->m_curMeasureData.usGap2;
	iGap3 = CStatus::Instance()->m_curMeasureData.usGap3;

	iOldCal1 = CStatus::Instance()->m_CalData.iGap1;
	iOldCal2 = CStatus::Instance()->m_CalData.iGap2;
	iOldCal3 = CStatus::Instance()->m_CalData.iGap3;

	iOrgGap1 = iGap1 - iOldCal1;
	iOrgGap2 = iGap2 - iOldCal2;
	iOrgGap3 = iGap3 - iOldCal3;

	m_iNewCal1 = m_iNewJigGap - iOrgGap1;
	m_iNewCal2 = m_iNewJigGap - iOrgGap2;
	m_iNewCal3 = m_iNewJigGap - iOrgGap3;
}

#pragma warning(disable: 4996)
void CCal2Dlg::SendCalSetCmd()
{
	struct tm CalDateTime;
	time_t now;
	time(&now);
	CalDateTime = *localtime(&now);
	CString strCalDate;
	strCalDate.Format(_T("%d%d%d"), CalDateTime.tm_year, CalDateTime.tm_mon, CalDateTime.tm_mday);
	CStatus::Instance()->m_pMonitorDlg->m_AGMDPacket.SendCalSetCmd(CT2A(strCalDate), m_iNewJigGap, m_iNewCal1, m_iNewCal2, m_iNewCal3);
}


BOOL CCal2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	MakeChartFrame();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CCal2Dlg::PreTranslateMessage(MSG* pMsg)
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
