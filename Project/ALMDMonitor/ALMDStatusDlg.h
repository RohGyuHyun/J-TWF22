#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "..\..\Library\SerialLib\SerialMFC.h"	// �ʼ�
#include "..\..\Library\SerialLib\ALMDPacket.h"	// �ʼ�
#include "LedONOFF.h"
#include "Status.h"
class CViewDlg;
class CRepository;

// CALMDStatusDlg ��ȭ �����Դϴ�.

class CALMDStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CALMDStatusDlg)

public:
	CALMDStatusDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CALMDStatusDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CSerialMFC	m_serial;				// �ʼ�
	CALMDPacket	m_ALMDPacket;			// �ʼ�
	CComboBox m_ctrlIntervalUnit;
	virtual BOOL OnInitDialog();
	CComboBox m_ctrlIntNo;
	CFont m_BigFont;
	CEdit m_edtRoll;
	CEdit m_edtPitch;
	CStatic m_labelRoll;
	CStatic m_labelPitch;
	CProgressCtrl m_ctrlBattery;
	int m_iALMDTemp;
	LRESULT OnSerialMsg(WPARAM wParam, LPARAM lParam);
	void SerialCheckNInit();
	afx_msg void OnBnClickedBtnRead();
	void SetStateInfo(DVC_PACKET_MEAINFO * pMeaInfo);
	int SetDispValue(double iRoll, double iPitch);
	CString m_strRollMag;
	CString m_strPitchDir;
	CString m_strLabelRollMag;
	CString m_strLabelPitchDir;
	CLedONOFF m_cStatConnect;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void LabelUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strRollUnit;
	CString m_strPitchUnit;
	void ResetStatConnect();
	int m_iProgramMode;
	afx_msg void OnBnClickedRpromodev();
	afx_msg void OnBnClickedRpromode();
	void SerialClose();
	CViewDlg *m_pViewDlg;
	CString m_strBatStat;
	CRepository *m_pRepository;
	afx_msg void OnBnClickedChkUseac();
	BOOL m_bUseAutoCmd;
	BOOL m_bConnected;
	void SetProgramMode(CStatus::tProgramMode ProgramMode);
	CButton m_cBtnRead;
	CButton m_cBtnUseAC;
	LRESULT OnPMChange(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateStatusInfo(WPARAM wParam, LPARAM lParam);
	int MakeWriteInfo(tALMDRecord *pNewRecord, DVC_PACKET_MEAINFO *pPacketInfo);
	double GetDispXValue();
	double GetDispYValue();
	double m_dblDispRollValue;
	double m_dblDispPitchValue;
	void GetGoNoGoDispInitData();
	CString m_strDispTolerance;
	int CheckDateLicense();
	void GetDispXYValue(tALMDRecord * pMeaInfo);
	afx_msg void OnClose();
};
