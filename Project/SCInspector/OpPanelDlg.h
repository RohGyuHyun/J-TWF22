#pragma once
#include "afxwin.h"
#include "..\Library\SerialLib\SerialMFC.h"	// �ʼ�
#include "..\Library\SerialLib\SCInspPacket.h"	// �ʼ�
#include "SCButton.h"
#include "SCGroupBox.h"

// COpPanelDlg ��ȭ �����Դϴ�.

class COpPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpPanelDlg)

public:
	COpPanelDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COpPanelDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPPANEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CSerialMFC		m_serial;				// �ʼ�
	CSCInspPacket	m_SCInspPacket;			// �ʼ�
	CComboBox m_ctrlSelCam;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeSelcam();
	afx_msg void OnBnClickedBtnLoadimage();
	afx_msg void OnBnClickedSetenv();
	afx_msg void OnBnClickedSaveimg();
	afx_msg void OnBnClickedBtnInspimage();
	int InitLight();
	BOOL m_bReceived;
	int SerialCheckNInit();
	void ProcessCMD(int nEvent);
	void SetFlagReceived();
	BOOL IsCMDReceived();
	int RcvSerialMsg(WPARAM wParam);
	int CheckoutPacket();
	void LightOn();
	afx_msg void OnBnClickedBtnTeaching();
	afx_msg void OnBnClickedBtnLoadjobfile();
	afx_msg void OnBnClickedBtnSavejobfile();
	afx_msg void OnBnClickedBtnGrab();
	afx_msg void OnBnClickedChkactiveallcam();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRunstop();
	afx_msg void OnBnClickedBtnSaveas();
	afx_msg void OnBnClickedBtntrial();
	afx_msg void OnBnClickedBtnOptest();
	void DeleteOldFile();
	BOOL IsDate(CString strName, CTime *pTime);
	int DiffDate(CTime * pDate);
	void DelFolder(CString strFolder);
	CSCButton m_cRunStop;
	CSCButton m_cGrabImage;
	CSCButton m_cSaveImage;
	CSCButton m_cSaveInspImg;
	CSCButton m_cLoadImage;
	CSCButton m_cLoadJobFile;
	CSCButton m_cSaveJobFile;
	CSCButton m_cSaveAs;
	CSCButton m_cTeaching;
	CSCButton m_cTrial;
	CSCButton m_cSetEnv;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	static UINT ThreadTest(LPVOID pParam);
};
