#pragma once
#include "afxwin.h"


// CSetGoNoGoDlg ��ȭ �����Դϴ�.

class CSetEventSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetEventSettingDlg)

public:
	CSetEventSettingDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSetEventSettingDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SET_EVENT_SETTING};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strRMS_above;
	CString m_strX_above;
	CString m_strY_above;
	CString m_strZ_above;
	CString m_strX_below;
	CString m_strY_below;
	CString m_strZ_below;
	CString m_strNoGoqualityTime;
	BOOL m_bRMS_above;
	BOOL m_bX_above;
	BOOL m_bY_above;
	BOOL m_bZ_above;
	BOOL m_bX_below;
	BOOL m_bY_below;
	BOOL m_bZ_below;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
private:
	void GetGoNoGoInfo(CString strTimeDomainUnit);
	void SetGoNoGoInfo(CString strTimeDomainUnit);
	void GetTriggerInfo(CString strTimeDomainUnit);
	void SetTriggerInfo(CString strTimeDomainUnit);
public:
	CStatic m_edit_TimeType;
private:
	BOOL RangeCheck(CString strTimeDomainUnit);
public:
	afx_msg void OnBnClickedRadioNone();
public:
	afx_msg void OnBnClickedRadioGonogo();
	afx_msg void OnBnClickedRadioTrigger();
	BOOL m_bIsSave;
	int m_iEventMode; // 0 : None , 1: Go/Nogo, 2: Trigger
private:
	void EvtNoneReset();
public:
	afx_msg void OnBnClickedCheckSave();
	afx_msg void OnBnClickedCancel();
	CString m_strTimeDomainUnit;
private:
	void CtrlValueEnableWindow(bool bIsEnable);
	void GoNoGoGroupBoxShow(bool bIsShow);
public:
	CComboBox m_CB_NoGoSecond;
	CString m_strSecond;
};
