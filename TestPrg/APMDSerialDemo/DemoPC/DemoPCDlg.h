
// DemoPCDlg.h : ��� ����
//

#pragma once

#include "..\SerialLib\SerialMFC.h"		// �ʼ�
#include "..\SerialLib\APMDPacket.h"	// �ʼ�
#include "afxwin.h"

// CDemoPCDlg ��ȭ ����
class CDemoPCDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CDemoPCDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMOPC_DIALOG };
#endif

protected:
	CSerialMFC	m_serial;				// �ʼ�
	CAPMDPacket	m_APMDPacket;			// �ʼ�

	CString		m_strCount01;
	CString		m_strCount03;
	CString		m_strCount05;
	CString		m_strTemperature;
	CString		m_strPressure;
	CString		m_strBattery;
	CString		m_strHumidity;
	CString		m_strCalDate;
	CString		m_strSerialNo;
	CString		m_strState;
	CString		m_strTime;

	CEdit		m_wndPruge;
	CEdit		m_wndMeasure;

	CComboBox	m_wndComPorts;

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
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnCbnSelendokComports();
	afx_msg LRESULT OnSerialMsg(WPARAM wParam, LPARAM lParam);	// �ʼ�
};
