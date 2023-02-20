
// DemoDeviceDlg.h : ��� ����
//

#pragma once

#include "..\SerialLib\SerialMFC.h"
#include "..\SerialLib\APMDPacket.h"
#include "afxwin.h"

// CDemoDeviceDlg ��ȭ ����
class CDemoDeviceDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CDemoDeviceDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMODEVICE_DIALOG };
#endif

protected:
	CSerialMFC	m_serial;
	CAPMDPacket	m_APMDPacket;

	DWORD		m_dwCount01;
	DWORD		m_dwCount03;
	DWORD		m_dwCount05;
	short		m_sTemperature;
	DWORD		m_dwPressure;
	BYTE		m_byBattery;
	BYTE		m_byHumidity;
	CEdit		m_wndSecond;
	CStatic		m_wndState;
	CString		m_strCalDate;
	CString		m_strSerialNo;

	BOOL		m_bStart;
	BOOL		m_bTimerOn;

	WORD		m_wSecond;
	WORD		m_wPurgeTime;
	WORD		m_wMeasureTime;

	CComboBox	m_wndComPorts;

	CButton		m_wndStart;

protected:
	void UpdateAvailableComPort();

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelendokComports();
	afx_msg LRESULT OnSerialMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedStart();
};
