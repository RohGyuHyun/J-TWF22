
// DemoPCDlg.h : ��� ����
//

#pragma once

#include "SerialMFC.h"		// �ʼ�
#include "XFMSPacket.h"	//kiy
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
	CXFMSPacket m_XFMSPacket;			//kiy

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

	CEdit m_wndKV;
	CEdit m_wndMA;

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
	afx_msg void OnBnClickedXrayOn();
	afx_msg void OnBnClickedXrayOff();
	afx_msg void OnBnClickedFocalStandard();
	afx_msg void OnBnClickedFocalSmall();
	afx_msg void OnBnClickedSetKv();
	afx_msg void OnBnClickedSetMa();
};
