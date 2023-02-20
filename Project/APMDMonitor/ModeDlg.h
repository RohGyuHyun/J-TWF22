#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include <UniUtility.h>
#include "APMDMessages.h"
#include "StatusDlg.h"
#include "..\..\Library\SerialLib\SerialMFC.h"	// �ʼ�
#include "..\..\Library\SerialLib\APMDPacket.h"	// �ʼ�


// CModeDlg ��ȭ �����Դϴ�.
class CGraphDlg;
class CViewOptionDlg;

class CModeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModeDlg)

public:
	CModeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CModeDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CSerialMFC	m_serial;				// �ʼ�
	CAPMDPacket	m_APMDPacket;			// �ʼ�
	CButtonST m_cProgramMode;
	int m_iProgramMode;
	CComboBox m_cPurgeUnit;
	CSpinButtonCtrl m_cSpinPurgeTime;
	afx_msg void OnCbnSelchangeCbdgtu();
	CTextProgressCtrl m_progPurge;
	CTextProgressCtrl m_progMeasure;
	afx_msg void OnEnChangeEdttimelabel();
	CSpinButtonCtrl m_SpinGatheringTime;
	afx_msg void OnCbnSelchangeCbpt();
	CComboBox m_cGTUnit;
//	UINT m_iGatheringTime;
	afx_msg void OnBnClickedBtnss();
	afx_msg LRESULT OnSerialMsg(WPARAM wParam, LPARAM lParam);	// �ʼ�
	LRESULT OnUpdateStatusMsg(WPARAM wParam, LPARAM lParam);	// �ʼ�
	static UINT ThreadTrigger(LPVOID pParam);
	int ThreadDataGathering();
	CWinThread *m_pWThd;
	LRESULT SendTestRecord(WPARAM wparam, LPARAM lparam);
	void SerialCheckNInit();
	void SetStatusDlg(CStatusDlg * pStatusDlg);
	CStatusDlg *m_pStatusDlg;
	void SetRepository(CRepository * pRepository);
	CRepository *m_pRepository;
	void SetGraphDlg(CGraphDlg * pGraphDlg);
	CGraphDlg *m_pGraphDlg;
	CString m_strAPMDStatus;
//	CString m_strTimeLeft;
	int m_iPurgeSec;
	int m_iMeasureSec;
	CViewOptionDlg *m_pViewOptionDlg;
	void SetViewOptionDlg(CViewOptionDlg * pViewOption);
	void CheckWarning(bool & bUseWarning, CString & strErrMsg);
	void SetReady();
	bool m_bStart;
	CString GetPurgeTimeMsg(int ElapsedSecs);
	CString GetMeasureTimeMsg(int ElapsedSecs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int NewMeasureDataAdd(DVC_PACKET_MEASURE* pPacket, bool IsFirst);
	int MakeDisconnectedData(int prevSec, int curSec);
	int CheckCalDate(CString strCalDate);
	int DispDataUpdate(tAPMDDispData &DispData, int KindOfData=0);
	int m_nPurgeStep;
};
