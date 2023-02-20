#pragma once
#include "afxcmn.h"
#include <UniUtility.h>

// CProgressBarDlg ��ȭ �����Դϴ�.

class CProgressBarDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressBarDlg)

public:
	CProgressBarDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CProgressBarDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROGRESSBARDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTextProgressCtrl m_ctrl_Progress;
	void SetTextMesaage(CString strMessage);
	void SetMaxRange(int nMaxCount);
	void ProgressBarStart();
	static BOOL m_bIsActive;
	void ProgressBarFinish();
	CWinThread *m_pThread;
	static UINT ProcessThread(LPVOID lParam);
	static int m_iValue;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	CString m_strMessage;
	virtual BOOL OnInitDialog();
	int m_nDataCount;
	int m_iTimeSecond;
};
