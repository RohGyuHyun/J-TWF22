
// DemoPCDlg.h : ��� ����
//

#pragma once

#include "..\SerialLib\SerialMFC.h"		// �ʼ�
#include "..\SerialLib\AVMDPacket.h"	// �ʼ�
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
	CSerialMFC	m_serial;				// �ʼ�
	CAGMDPacket	m_AVMDPacket;			// �ʼ�

	CString m_editXValue;
	CString m_editYValue;
	CString m_editZValue;
	CComboBox m_wndComPorts;
	CString m_strTemperature;
	CString m_strBattery;
	CString m_strNum;
	CString m_strCalDate;
	CString m_strSerialNo;
	CString m_strState;
	CString m_strTime;
	void UpdateAvailableComPort();
	afx_msg void OnCbnSelendokComports();
	afx_msg LRESULT OnSerialMsg(WPARAM wParam, LPARAM lParam);	// �ʼ�
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedStart();
};
