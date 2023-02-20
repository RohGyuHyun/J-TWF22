#pragma once
#include "afxdialogex.h"
#include "CDialogXRayStatus.h"

// CDialogManual 대화 상자



class CDialogManual : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogManual)

public:
	CDialogManual(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogManual();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MANUAL };
#endif
protected:
	CDialogXRayStatus* m_dlgXRayStatus;

	IO_PARA m_stInput[MAX_INPUT_NUM];
	IO_PARA m_stOutput[MAX_OUTPUT_NUM];
	BOOL m_bInputState[MAX_INPUT_NUM];
	BOOL m_bOutputState[MAX_OUTPUT_NUM];

	BOOL m_bListInputState[MAX_INPUT_NUM];
	BOOL m_bListOutputState[MAX_OUTPUT_NUM];

	CImageList* m_Img_list_slct;
	TIME_DATE m_cXrayEndTime;
	typedef void(*CALLBACK_FUNCTION_Event)(int);
	CALLBACK_FUNCTION_Event callEventfunc;
	LOGIN_TYPE m_LoginType;

	CWinThread* m_pCalThread;
	BOOL m_bCalEnd;
	double m_dScanSpeed;
	int m_nLineToAvg;
private:

	void SetIOListLabel();
	BOOL LoadIOLabelPara();
	BOOL SaveIOLabelPara();

	void UpdateInputDisplay();
	void UpdateOutputDisplay();
public:
	void SetCallBack(CALLBACK_FUNCTION_Event event) { callEventfunc = event; m_dlgXRayStatus->SetCallBack(event); };
	void SetXrayEndTime(TIME_DATE end_time) { m_cXrayEndTime = end_time; };
	void SetXRayValue(double dKv, double dMa);
	void SetLoginType(LOGIN_TYPE login_type) { m_LoginType = login_type; };

	UINT static ThreadCalStart(LPVOID pParam);
	void CalStartThread();
	BOOL GetCalEnd() { return m_bCalEnd; };
	void SetCalParameter(double dScanSpeed, int nLineToAvg) { m_dScanSpeed = dScanSpeed; m_nLineToAvg = nLineToAvg; };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	CListCtrl m_listctrl_IO_Input;
	CListCtrl m_listctrl_IO_Output;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMClickListInput(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonXRayWarmup();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonDetectorCal();
};
