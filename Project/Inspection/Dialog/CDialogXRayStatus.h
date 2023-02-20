#pragma once
#include "afxdialogex.h"
#include "CXRayClass.h"

// CDialogXRayStatus 대화 상자



class CDialogXRayStatus : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogXRayStatus)

public:
	CDialogXRayStatus(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogXRayStatus();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_X_RAY_STATUS };
#endif

protected:
	//CXRayClass* m_XRayClass;
	X_RAY_STATUS m_stXray_status;
	typedef void(*CALLBACK_FUNCTION_Event)(int);
	CALLBACK_FUNCTION_Event callEventfunc;
	BOOL m_bNumPd;
private:
	void SetXrayStatus(CBtnenhctrl* pBtnEnh, BOOL isOnOff);

public:
	void SettingKVtoMA(double dKV, double dMA);
	void GetKVtoMA(double* dKV, double *dMA);
	void SetCallBack(CALLBACK_FUNCTION_Event event) { callEventfunc = event; };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonXRayReset();
	afx_msg void OnBnClickedButtonXRayOn();
	afx_msg void OnBnClickedButtonXRayOff();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnSetfocusEditXRayPara2();
	afx_msg void OnEnSetfocusEditXRayPara3();
	afx_msg void OnEnKillfocusEditXRayPara2();
	afx_msg void OnEnKillfocusEditXRayPara3();
};
