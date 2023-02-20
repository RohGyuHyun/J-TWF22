
// DemoDeviceDlg.h : ��� ����
//

#pragma once

#include "..\SerialLib\SerialMFC.h"
#include "..\SerialLib\ALMDPacket.h"
#include "afxwin.h"
#include "afxcmn.h"

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
	CALMDPacket	m_ALMDPacket;

	short		m_sTemperature;
	short		m_sRoll;
	short		m_sPitch;
	BYTE		m_byBattery;
	CStatic		m_wndState;
	CString		m_strCalDate;
	CString		m_strSerialNo;

	BOOL		m_bStart;
	BOOL		m_bTimerOn;

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
	CSpinButtonCtrl m_spRoll;
	CSpinButtonCtrl m_spPitch;
	CComboBox m_cKindOfWafer;
	CSliderCtrl m_cSldRoll;
	CSliderCtrl m_cSldPitch;
	afx_msg void OnTRBNThumbPosChangingSldRoll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTRBNThumbPosChangingSldPitch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSldRoll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSldPitch(NMHDR *pNMHDR, LRESULT *pResult);
};
