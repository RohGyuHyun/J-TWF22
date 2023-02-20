#pragma once
#include "afxdialogex.h"

#define X_RAY_CUR_VALUE_READ	0
#define X_RAY_WARMUP_START		1
#define X_RAY_WARMUP_INIT		2
#define X_RAY_WARMUP_TOWER_LAMP	3
// CDialogXRayWarmup 대화 상자

class CDialogXRayWarmup : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogXRayWarmup)

public:
	CDialogXRayWarmup(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogXRayWarmup();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_X_RAY_WARMUP };
#endif

protected:
	int m_nWarmupStepTimeSec;
	int m_nWarmupStepCnt;
	BOOL m_bWarmupStart;
	int m_nWarmupStepTimeSecCnt;
	TIME_DATE m_cXrayEndTime;
	BOOL m_bWarmupEnd;
	int m_nInitCnt;
	BOOL m_bTowerLampOn;
private:


public:
	void SetXrayEndTime(TIME_DATE cXrayEndTime) {m_cXrayEndTime = cXrayEndTime;};
	void SetWarmnup();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlWarmupExit();
	CProgressCtrl m_ProgressWarmup;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
