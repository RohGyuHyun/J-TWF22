// SenSorStatusDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "SenSorStatusDlg.h"
#include "afxdialogex.h"
#include "Status.h"


// CSenSorStatusDlg ��ȭ �����Դϴ�.
#define TID_CNTCHECK 1
#define TIME_CNTCHECK 1500
IMPLEMENT_DYNAMIC(CSenSorStatusDlg, CDialogEx)

CSenSorStatusDlg::CSenSorStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SENSORSTATUSDLG, pParent)
{
	CStatus::Instance()->SetSensorStatusDlg(this);
}

CSenSorStatusDlg::~CSenSorStatusDlg()
{
}

void CSenSorStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STAT_CONNECT, m_cCntStat);
	DDX_Control(pDX, IDC_STATIC_TEMPERATURE, m_cTempStat);
	DDX_Control(pDX, IDC_PROGRESS_BATTERY, m_cBatt);
	DDX_Control(pDX, IDC_BATTNUM, m_cBattNUM);
}


BEGIN_MESSAGE_MAP(CSenSorStatusDlg, CDialogEx)
	ON_MESSAGE(WM_UIUPDATE, &CSenSorStatusDlg::OnReceiveUpdateUI)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSenSorStatusDlg �޽��� ó�����Դϴ�.


BOOL CSenSorStatusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_cCntStat.Set(FALSE);
	SetTimer(TID_CNTCHECK, TIME_CNTCHECK, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

LRESULT CSenSorStatusDlg::OnReceiveUpdateUI(WPARAM wParam, LPARAM lParam)
{
	SetSensorState((int)((signed char)CStatus::Instance()->m_curMeasureData.byTemperature), CStatus::Instance()->m_curMeasureData.byBattery);
	return LRESULT();
}
int CSenSorStatusDlg::SetSensorState(int iTemp, int nBatt)
{
	CString strTemp;
	m_cCntStat.Set(TRUE);
	SetTimer(TID_CNTCHECK, TIME_CNTCHECK, NULL);
	strTemp.Format(_T("%d"), iTemp);
	m_cTempStat.SetWindowTextW(strTemp.GetBuffer());
	m_cBatt.SetPos(nBatt);
	CString strBattNum;
	strBattNum.Format(_T("%d%%"), nBatt);
	m_cBattNUM.SetWindowTextW(strBattNum.GetBuffer());
	return 0;
}


void CSenSorStatusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch(nIDEvent)
	{
		case TID_CNTCHECK:
			m_cCntStat.Set(FALSE);
			break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CSenSorStatusDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN))
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
