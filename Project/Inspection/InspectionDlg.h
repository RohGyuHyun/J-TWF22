
// InspectionDlg.h: 헤더 파일
//

#pragma once
#include "SequenceModule.h"
#include "CDialogAlarm.h"
#include "CDialogManual.h"
#include "CDialogRecipe.h"
#include "CDialogSystem.h"
#include "CATDetector.h"
#include "CXRayClass.h"
#include "CInverterClass.h"
#include "Repository.h"
#include "CIOClass.h"
#include "btnenhctrl.h"
#include "CDialogTextBox.h"
#include "CDialogLogin.h"
#include "CInspectionClass.h"
#include "NghUtil.h"
#include "CDialogNgView.h"
#include "CDialogResultNG.h"
#include "CDialgInterLockStatus.h"
#include "CDialogLogo.h"
#include "CDialogAlarmLogReport.h"


// CInspectionDlg 대화 상자
class CInspectionDlg : public CDialogEx
{
// 생성입니다.
public:
	CInspectionDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CInspectionDlg();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSPECTION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


protected:
	CSequenceModule g_SeqModule;/**<Sequenc Class*/
	BOOL m_bTestCmd;
	//CDialogLogo* m_dlgLog;
	CDialogAlarm* m_dlgAlarm;
	CDialogManual* m_dlgManual;
	CDialogRecipe* m_dlgRecipe;
	CDialogSystem* m_dlgSystem;
	CDialogTextBox* m_dlgTextBox;
	//CDialogRect* m_DlgRect;
	CDialogNgView* m_dlgNgView;
	CDialogResultNG* m_dlgResultNG;
	CDialgInterLockStatus* m_dlgInterLockStatus;
	CDialogAlarmLogReport* m_dlgAlarmLogReport;

	CAtDetector* m_AtDetector;
	CXRayClass* m_XRayClass;
	CIOClass* m_IOClass;
#ifdef INVERTER_COM
	CInverterClass* m_Inverter;
#endif

#ifdef MIL_USE
	//CRepository* m_pRepository;
	CMilDisplay* m_MilDisplay;
	CMilBuffer* m_MilDispBuff;
	CMilBuffer* m_Mil16bitBuff;
	CMilBuffer* m_Mil16bitBuff2;
	CMilBuffer* m_Mil8bitBuff;
#endif
	BOOL m_bAutoStatus;
	BOOL m_bXRayOnOff;
	BOOL m_bBeforeXRayOnOff;

	BOOL m_bFirstGrab;

	CBtnenhctrl m_btn_TotalTackTime;
	CBtnenhctrl m_btn_InspTackTime;
	CBtnenhctrl m_ctrMainTotalCnt;
	CBtnenhctrl m_ctrMainOkCnt;
	CBtnenhctrl m_ctrMainNgCnt1;//count ng
	CBtnenhctrl m_ctrMainNgCnt2;//overlap ng
	CBtnenhctrl m_ctrMainNGRatio1;//count ng
	CBtnenhctrl m_ctrMainNGRatio2;//overlap ng

	CBtnenhctrl m_ctrMainXrayOnOff;
	CBtnenhctrl m_ctrMainCurTime;

	CBtnenhctrl m_ctrMainCurRecipe;

	CBtnenhctrl m_ctrMainCurKV;
	CBtnenhctrl m_ctrMainCurMA;

	int m_iCount;	// Total Count
	int m_iOKCount;
	int m_iPointNGConunt[MAX_RSLT_POINT_CNT];


	BOOL m_bXRayOnCheck;
	BOOL m_bGrabStart;
	BOOL m_bGrabEnd;
	BOOL m_bInspectionStart;
	BOOL m_bInspectionEnd;
	BOOL m_bResultDisplayEnd;
#ifdef MIL_USE
	CInspectionClass *m_InspectionClass[MAX_PATTERN_ADD_CNT];
#endif
	CWinThread* m_pSequenceThread;
	CWinThread* m_pNgRejecterThread;

	long m_lInspStartTime;
	double m_dInspTackTime;
	long m_lTotalStartTime;
	double m_dTotalTackTime;
	CTime m_cStart;
	DWORD m_dwPatternResultColor[5];

	LOGIN_TYPE m_LoginType;
	int m_nSelectResultDisplayIndex;
	CBtnenhctrl m_ctrlSelectResultDisplayIdx[MAX_PATTERN_ADD_CNT];
	BOOL m_bSelectResultDisplay;

	CSize m_ImageSize;

	BOOL m_HW_Connection[3];
	BOOL m_bInspResult;
	int m_nInspNgIndex;

	CTime m_InspStartTime;

	CWinThread* m_pLogWriteThread;
	BOOL m_bLogWriteThreadEnd;
	CLogFile* m_pMainLog;
	CLogFile* m_pInspectionLog;
	CLogFile* m_pAlarmLog;

	int m_DetectorCalStep;

	BOOL m_bExit;
	BOOL m_bAutoStop;
	BOOL m_bAutoAlarm;
	BOOL m_bXrayLampCheck;

	BOOL m_bInSenser;
	BOOL m_bOutSenser;

	BOOL m_bMc01;

	int m_nNGBuzzerCnt;
	BOOL m_bNgBuzzer;
	BOOL m_bAlarmBuzzer;
	BOOL m_bBuzzerOff;

	BOOL m_bInterLockStatus;

	int m_nInSenserTime;
	int m_nOutSenserTime;

	BOOL m_bRejecterThreadEnd;
	BOOL m_bRejecterQuePop;
	BOOL m_bRejecterStart;
	BOOL m_bEndRejecterStart;
	deque<DWORD> m_dwRejecterStartTime;
	DWORD m_dwEndRejecterStartTime;

#ifndef HW_USE
	BOOL m_bTestVal;
#endif
private:
	int InitClass();
	void DeleteClass();
	int InitDisplay();
	void InitList();
	void StartIOValue();
	int SequenceStartIOCheck();

	void CountView();
	void CountOKAdd();
	void CountNGAdd(int nNgIndex);
	void CountReset();
	int CountLoad();
	void LoadDayCountData();
	void SaveDayCountData();

	void StartAutoMode(BOOL isAuto);

	BOOL SaveHBITMAPToFile(HBITMAP hBitmap, LPCSTR lpszFileName);
	void screenshot(LPCSTR filepath);

	void SetMainDisplayImage();

	void LoginTypeEnableBtn(LOGIN_TYPE login_type);
	void AutoEnableBtn(BOOL isAuto);
	void PatternEnableBtn();
	BOOL WindowCapture(HWND hTargetWnd, LPCSTR strFileName);


	void InitLog();
	void MainLogWrite(CString strLog);

	deque<CString> m_strMainLog;
	deque<CString> m_strInspectionLog;

	void SetTowerLamp(int nIndex, int nBuzzer = -1);//nIndex -> 0 : red 1 : yellow 2 : green, nBuzzer -> -1 : Buzzer Off, 0 : Buzzer 1, 1 : Buzzer 2
	void SetMC01On(BOOL isOnOff = FALSE);
	BOOL SetXRayOn(BOOL isOnOff = FALSE);
	void SetMotorCWTOCCW(int nCW = 0);//0 : CW 1 : CCW

	void WriteRsltData();
	BOOL WriteCsvLog(CStdioFile* pFile, CString strLog);
	
public:

	void SetShowResultNG(BOOL isRslt);
	void SetNGBuzzer(BOOL isRslt);

	void SetXrayOnOffLabel(BOOL isOn);
	BOOL GetInspectionThreadEnd();

	UINT static   SequenceThread(LPVOID pParam);
	UINT static   NgRejecterThread(LPVOID pParam);
	void		NgRejecterSequence();
	BOOL		RejecterDelayCheck(DWORD dwStartTime, int nDelay);
	BOOL		StartRejecterStart();
	BOOL		EndRejecterStart();
	UINT static   LogWriteThread(LPVOID pParam);
	void LogWriteMessage();

	BOOL GetHardDiskFreeCheck();
	BOOL GetXRayOnCheck() { return m_bXRayOnCheck; };
	void SetGrabStart() { m_bGrabStart = TRUE; m_bGrabEnd = FALSE; };
	BOOL GetGrabStart() { return m_bGrabStart; };
	void SetInspectionStart();
	BOOL GetInspectionStart() { return m_bInspectionStart; };
	BOOL GetGrabEnd() { return m_bGrabEnd; };
	BOOL GetInspectionEnd() { return GetInspectionThreadEnd(); };
	void SetResultDisplay();
	BOOL GetResultDisplayEnd() { return m_bResultDisplayEnd; };
	void SetInspScreenShotSave();
	void SetInspImageSave();
	void SetInitInspection();
	void SetInspStartTime(long lStart) { m_lInspStartTime = lStart; };
	long GetInspStartTime() { return m_lInspStartTime; };
	void SetTotalStartTime(long lStart) { m_lTotalStartTime = lStart; };
	long GetTotalStartTime() { return m_lTotalStartTime; };
	void SetDispTackTime(double dTime, int nType = 0);//0 = Insp, 1 = Total

	void ShowTextBox(CString strText, BOOL isShow = TRUE, BOOL isStop = FALSE);
	BOOL GetStopWait() { return m_dlgTextBox->IsWindowVisible(); };

	void SelectDisplay(int nIdx);

	BOOL GetXRayInterLock();
	BOOL GetDoorInterLock();
	BOOL GetInSenser();
	BOOL GetOutSenser();

#ifndef INVERTER_COM
	void SetConMove(int nDirection = 0, BOOL isMove = FALSE);//0 : CW, 1 : CCW
#else
	void SetConMove(BOOL isMove = FALSE);
#endif
	void SetInverterReset(BOOL isReset = TRUE);
	BOOL GetEMG();
	BOOL GetKeySw();
	BOOL GetDoor();
	BOOL GetInverterAlarm();
	BOOL GetTubeAlarm();
	BOOL GetXRayLampALM();
	BOOL GetMC01OnAlarm();
	BOOL GetXRayWarmupAlaram();

	void SetInverterAlarmReset();

	void SetXrayEndTimeWrite(int nData);

	void SetMainRecipeName(CString strName);
	void SetAlarmStatus(BOOL isStatus) { m_bAutoAlarm = isStatus; };

	BOOL SetSystemParaSave() { return m_dlgSystem->SetSaveSystemPara(); };
	SYSTEM_PARA GetSystemPara() { return m_dlgSystem->GetSystemPara(); };
	void SetSystemPara(SYSTEM_PARA sys) { m_dlgSystem->SetSystemPara(sys); };

	void SetAlarmBuzzerOff(BOOL isOff) { m_bBuzzerOff = isOff; };

	int GetInspectionType() { return m_dlgSystem->GetSystemPara().nInspectionType; };

#ifndef HW_USE
	int m_nTestXRayCnt;
	int m_nTestIndex;
	BOOL LoadSqImage();
#endif
// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	LRESULT OnDetectorToMainMessage(WPARAM para0, LPARAM para1);
	LRESULT OnInspectionEndMessage(WPARAM para0, LPARAM para1);
	LRESULT OnAlarmSequenceMessage(WPARAM para0, LPARAM para1);
	LRESULT OnImageViewToMainMessage(WPARAM para0, LPARAM para1);
	LRESULT OnLogToMainMessage(WPARAM para0, LPARAM para1);
	LRESULT OnLogToInspectionMessage(WPARAM para0, LPARAM para1);
	LRESULT OnLogWriteMessage(WPARAM para0, LPARAM para1);
public:
	afx_msg void OnBnClickedButtonImgLoad();
	afx_msg void OnBnClickedButtonImgSave();
	afx_msg void OnBnClickedButtonAuto();
	afx_msg void OnBnClickedButtonManual();
	afx_msg void OnBnClickedButtonSystem();
	afx_msg void OnBnClickedButtonRecipe();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListCtrl m_list_InspResultData;
	CListCtrl m_list_Log;
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlMainCountReset();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	void ClickBtnenhctrlMainPattern();
	void ClickBtnenhctrlAuto();
	void ClickBtnenhctrlManual();
	void ClickBtnenhctrlSystem();
	void ClickBtnenhctrlRecipe();
	void ClickBtnenhctrlExit();
	void ClickBtnenhctrlMainLogin();
	afx_msg void OnBnClickedButtonTestXRayStatus();
	afx_msg void OnBnClickedButtonTestXRayStatus2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickBtnenhctrlNgView();
	void ClickBtnenhctrlMainInterlockStatus2();
	CProgressCtrl m_ctr_HardDiskUseSize;
	void ClickBtnenhctrlAlarmLogReport();
};
