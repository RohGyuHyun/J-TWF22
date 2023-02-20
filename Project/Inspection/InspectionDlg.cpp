
// InspectionDlg.cpp: 구현 파일
//

/**
  @file     InspectionDlg.cpp
  @brief    InspectionDlg Source File
  @author   노규현

  서스 패드 검사기 메인 소스 파일.\n
  */

  /**
  @mainpage InspectionDlg 메인페이지
	@section intro 소개
	- 소개      :   서스 패드 검사기 프로그램.
	@section   Program 프로그램명
	- 프로그램명		:   Suspad Inspection
	- 프로그램내용		:   Suspad 유무, 겹침 검사
	@section  INOUTPUT I/O자료
	- INPUT           	:   없음.
	- OUTPUT      		:   없음.
	@section  CREATEINFO 작성정보
	- 작성자     		:   노규현
	- 작성일			:   2022/09/22
	@section  MODIFYINFO      수정정보
	- 수정자/수정일   	: 수정내역
	- 노규현/22.09.22	: 주석 추가
  */

#include "pch.h"
#include "framework.h"

#include "Inspection.h"
#include "InspectionDlg.h"
#include "afxdialogex.h"
#include "CDialogXRayWarmup.h"
#include "uniioport.h"


CInspectionDlg* m_This;

//Multi media Timer
DWORD _TimerCounterMain;
UINT _TimerIDMain;
TIMECAPS _CapsMain;
BOOL _TimerPostMain;

static void CALLBACK TimerReadFunc(UINT wID, UINT wUser, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	if (_TimerPostMain == FALSE)
	{
		_TimerCounterMain++;
	}
	else
	{
		AfxMessageBox(_T("Error : high resolution timer event loss!"));
	}
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CInspectionDlg 대화 상자



CInspectionDlg::CInspectionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INSPECTION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bAutoStatus = FALSE;
	m_bXRayOnOff = FALSE;
	m_bAutoStop = TRUE;
#ifndef HW_USE
	m_nTestIndex = 1;
#endif
	m_bXRayOnCheck = FALSE;
	for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
	{
#ifdef MIL_USE
		m_InspectionClass[i] = NULL;
#endif
	}

	m_dwPatternResultColor[0] = PATTERN_0_RESULT_COLOR;
	m_dwPatternResultColor[1] = PATTERN_1_RESULT_COLOR;
	m_dwPatternResultColor[2] = PATTERN_2_RESULT_COLOR;
	m_dwPatternResultColor[3] = PATTERN_3_RESULT_COLOR;
	m_dwPatternResultColor[4] = PATTERN_4_RESULT_COLOR;

	m_bFirstGrab = FALSE;
	m_bInSenser = FALSE;
	m_bOutSenser = FALSE;
	m_bMc01 = FALSE;
	m_bXrayLampCheck = FALSE;
	m_dlgNgView = NULL;
	m_bExit = FALSE;
	m_bAutoAlarm = FALSE;
	m_DetectorCalStep = 0;
#ifdef INVERTER_COM
	m_Inverter = NULL;
#endif
	m_AtDetector = NULL;
//	m_DlgRect = NULL;
	m_IOClass = NULL;
	m_XRayClass = NULL;
	m_bBeforeXRayOnOff = FALSE;
	m_bGrabEnd = FALSE;
	m_bGrabStart = FALSE;
	m_bInspectionEnd = FALSE;
	m_bInspectionStart = FALSE;
	m_bResultDisplayEnd = FALSE;
	m_dInspTackTime = 0.;
	m_dTotalTackTime = 0.;
	//m_dlgLog = NULL;
	m_dlgAlarm = NULL;
	m_dlgRecipe = NULL;
	m_dlgSystem = NULL;
	m_dlgTextBox = NULL;
	m_dlgManual = NULL;
	m_dlgResultNG = NULL;
	m_iCount = 0;
	m_iOKCount = 0;
	m_iPointNGConunt[0] = 0;
	m_iPointNGConunt[1] = 0;
	m_lInspStartTime = 0;
	m_lTotalStartTime = 0;
	m_pSequenceThread = NULL;

#ifdef MIL_USE
	//m_pRepository = NULL;
	m_MilDispBuff = NULL;
	m_MilDisplay = NULL;
	m_Mil16bitBuff = NULL;
#endif
	m_nSelectResultDisplayIndex = 0;
	m_bSelectResultDisplay = FALSE;

	m_bLogWriteThreadEnd = FALSE;
	m_HW_Connection[0] = FALSE;
	m_HW_Connection[1] = FALSE;
	m_LoginType = LOGIN_USER;
	m_bInspResult = FALSE;
#ifndef HW_USE
	m_bTestVal = 0;
#endif
	m_nInspNgIndex = 0;
	m_pInspectionLog = NULL;
	m_pMainLog = NULL;
	m_pLogWriteThread = NULL;

	m_bInterLockStatus = FALSE;

	m_nInSenserTime = 0;
	m_nOutSenserTime = 0;

	m_bRejecterThreadEnd = FALSE;
	m_bRejecterStart = FALSE;
	m_bEndRejecterStart = FALSE;
	m_dwEndRejecterStartTime = 0;

	m_bTestCmd = FALSE;
}

CInspectionDlg::~CInspectionDlg()
{
	
	
}

void CInspectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INSP_RESULT_REPORT, m_list_InspResultData);
	DDX_Control(pDX, IDC_LIST_INSP_LOG, m_list_Log);

	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_TACK_TIME, m_btn_TotalTackTime);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_TACK_TIME2, m_btn_InspTackTime);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_TOTAL_CNT, m_ctrMainTotalCnt);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_OK_CNT, m_ctrMainOkCnt);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_NG_CNT, m_ctrMainNgCnt1);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_NG_RATIO, m_ctrMainNGRatio1);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_NG_CNT2, m_ctrMainNgCnt2);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_NG_RATIO2, m_ctrMainNGRatio2);

	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_X_RAY_ON_OF, m_ctrMainXrayOnOff);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_CUR_TIME, m_ctrMainCurTime);

	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_CUR_RECIPE, m_ctrMainCurRecipe);

	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_CUR_KV, m_ctrMainCurKV);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_CUR_MA, m_ctrMainCurMA);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_PATTERN_0, m_ctrlSelectResultDisplayIdx[0]);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_PATTERN_1, m_ctrlSelectResultDisplayIdx[1]);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_PATTERN_2, m_ctrlSelectResultDisplayIdx[2]);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_PATTERN_3, m_ctrlSelectResultDisplayIdx[3]);
	DDX_Control(pDX, IDC_BTNENHCTRL_MAIN_PATTERN_4, m_ctrlSelectResultDisplayIdx[4]);

	DDX_Control(pDX, IDC_PROGRESS_HARD_DISK, m_ctr_HardDiskUseSize);
}

BEGIN_MESSAGE_MAP(CInspectionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_IMG_LOAD, &CInspectionDlg::OnBnClickedButtonImgLoad)
	ON_BN_CLICKED(IDC_BUTTON_IMG_SAVE, &CInspectionDlg::OnBnClickedButtonImgSave)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &CInspectionDlg::OnBnClickedButtonAuto)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL, &CInspectionDlg::OnBnClickedButtonManual)
	ON_BN_CLICKED(IDC_BUTTON_SYSTEM, &CInspectionDlg::OnBnClickedButtonSystem)
	ON_BN_CLICKED(IDC_BUTTON_RECIPE, &CInspectionDlg::OnBnClickedButtonRecipe)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CInspectionDlg::OnBnClickedButtonExit)
	ON_WM_TIMER()
	ON_MESSAGE(USE_MSG_DETECTOR_TO_MAIN, &CInspectionDlg::OnDetectorToMainMessage)
	ON_MESSAGE(USE_MSG_INSPECTION_END, &CInspectionDlg::OnInspectionEndMessage)
	ON_MESSAGE(USE_MSG_ALARM_SEQUENCE_TO_MAIN, &CInspectionDlg::OnAlarmSequenceMessage)
	ON_MESSAGE(USE_MSG_IMAGE_VIEW_TO_MAIN_DIALOG, &CInspectionDlg::OnImageViewToMainMessage)
	ON_MESSAGE(USE_MSG_LOG_TO_MAIN, &CInspectionDlg::OnLogToMainMessage)
	ON_MESSAGE(USE_MSG_LOG_TO_INSPECTION, &CInspectionDlg::OnLogToInspectionMessage)
	ON_MESSAGE(USE_MSG_LOG_WRITE, &CInspectionDlg::OnLogWriteMessage)
	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDC_BUTTON_TEST_X_RAY_STATUS, &CInspectionDlg::OnBnClickedButtonTestXRayStatus)
	ON_BN_CLICKED(IDC_BUTTON_TEST_X_RAY_STATUS2, &CInspectionDlg::OnBnClickedButtonTestXRayStatus2)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CInspectionDlg, CDialogEx)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_MAIN_COUNT_RESET, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlMainCountReset, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_MAIN_PATTERN_0, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlMainPattern, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_MAIN_PATTERN_1, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlMainPattern, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_MAIN_PATTERN_2, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlMainPattern, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_MAIN_PATTERN_3, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlMainPattern, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_MAIN_PATTERN_4, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlMainPattern, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_AUTO, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlAuto, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_MANUAL, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlManual, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_SYSTEM, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlSystem, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_RECIPE, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlRecipe, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_EXIT, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlExit, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_MAIN_LOGIN, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlMainLogin, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_NG_VIEW, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlNgView, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_MAIN_INTERLOCK_STATUS_2, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlMainInterlockStatus2, VTS_NONE)
	ON_EVENT(CInspectionDlg, IDC_BTNENHCTRL_ALARM_LOG_REPORT, DISPID_CLICK, CInspectionDlg::ClickBtnenhctrlAlarmLogReport, VTS_NONE)
END_EVENTSINK_MAP()
// CInspectionDlg 메시지 처리기

BOOL CInspectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	InitLog();
	MainLogWrite(_T("System Init !!"));
	CDialogLogo dlgLog;
	dlgLog.DoModal();

	_TimerPostMain = FALSE;
	timeGetDevCaps(&_CapsMain, sizeof(_CapsMain));
	_CapsMain.wPeriodMin = 1;
	timeBeginPeriod(_CapsMain.wPeriodMin);
	_TimerIDMain = timeSetEvent(_CapsMain.wPeriodMin, _CapsMain.wPeriodMin, (LPTIMECALLBACK)&TimerReadFunc, 0, (UINT)TIME_PERIODIC);

	if (!_TimerIDMain)
	{
		AfxMessageBox(_T("Error : high resolution timer set fail!"));
	}

	GetDlgItem(IDC_STATIC_TEST_0)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEST_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_TEST_PARA_0)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_TEST_PARA_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_TEST_X_RAY_STATUS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_TEST_X_RAY_STATUS2)->ShowWindow(SW_HIDE);

	m_ctr_HardDiskUseSize.SetRange(0, 100);
//#endif
	////////////////////////////////////////////////////////
	/**exe 파일 수정 날 짜를 버전으로 사용*/
	CString strFile;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	SYSTEMTIME sysTime;
	::GetModuleFileName(NULL, strTemp, 256);
	strFile.Format(_T("%s"), strTemp);
	GetWriteFileTime(strFile, &sysTime);

	strFile.Format(_T("SW Ver(%02d.%02d.%02d_%02d%02d%02d)"), sysTime.wYear - 2000, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	SetDlgItemText(IDC_BTNENHCTRL_SW_VER, strFile);
//#endif
	////////////////////////////////////////////////////////
	m_This = this;
	int rslt = InitClass();
	
	if (rslt != 0)
	{
		CString strText, strText2;

		switch (rslt)
		{
		case 10:
			strText2.Format(_T("0"));
			break;
		case 11:
			strText2.Format(_T("1"));
			break;
		case 12:
			strText2.Format(_T("2"));
			break;
		case 13:
			strText2.Format(_T("3"));
			break;
		}
		strText.Format(_T("Init Class(%s) Error"), strText2);
		AfxMessageBox(strText);
	}

	rslt = InitDisplay();


	if (m_dlgSystem->GetSystemPara().nInspectionType == 1)
	{
		CRect rect1[4], rect2[4];
		//show
		GetDlgItem(IDC_BTNENHCTRL_MAIN_CAP_10)->SetWindowText(_T("불량"));
		GetDlgItem(IDC_BTNENHCTRL_MAIN_CAP_7)->SetWindowText(_T("불량(%)"));

		//hide
		GetDlgItem(IDC_BTNENHCTRL_MAIN_CAP_11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_CAP_9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_NG_CNT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_NG_RATIO2)->ShowWindow(SW_HIDE);
		
		//get rect
		GetDlgItem(IDC_BTNENHCTRL_MAIN_CAP_11)->GetWindowRect(&rect1[0]);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_CAP_9)->GetWindowRect(&rect1[1]);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_NG_CNT2)->GetWindowRect(&rect1[2]);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_NG_RATIO2)->GetWindowRect(&rect1[3]);

		
		//move
		GetDlgItem(IDC_BTNENHCTRL_MAIN_CAP_12)->MoveWindow(rect1[0].left - 3, rect1[0].top - 2, rect1[0].Width(), rect1[0].Height());// 11
		GetDlgItem(IDC_BTNENHCTRL_MAIN_CAP_8)->MoveWindow(rect1[1].left - 3, rect1[1].top - 2, rect1[1].Width(), rect1[1].Height());// 9
		GetDlgItem(IDC_BTNENHCTRL_MAIN_TACK_TIME2)->MoveWindow(rect1[2].left - 3, rect1[2].top - 2, rect1[2].Width(), rect1[2].Height());//ng cnt2
		GetDlgItem(IDC_BTNENHCTRL_MAIN_TACK_TIME)->MoveWindow(rect1[3].left - 3, rect1[3].top - 2, rect1[3].Width(), rect1[3].Height());// ng ratio2


		//get rect
		GetDlgItem(IDC_BTNENHCTRL_MAIN_CAP_8)->GetWindowRect(&rect1[0]);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_TACK_TIME)->GetWindowRect(&rect1[1]);

		GetDlgItem(IDC_BTNENHCTRL_MAIN_X_RAY_CONNECT)->GetWindowRect(&rect2[0]);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_X_RAY_CONNECT2)->GetWindowRect(&rect2[1]);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_DETECTOR_CONNECT)->GetWindowRect(&rect2[2]);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_DETECTOR_CONNECT2)->GetWindowRect(&rect2[3]);

		//move
		GetDlgItem(IDC_BTNENHCTRL_MAIN_X_RAY_CONNECT)->MoveWindow(rect2[0].left - 3, rect1[0].bottom, rect2[0].Width(), rect2[0].Height());
		GetDlgItem(IDC_BTNENHCTRL_MAIN_X_RAY_CONNECT2)->MoveWindow(rect2[1].left - 3, rect1[0].bottom, rect2[1].Width(), rect2[1].Height());
		GetDlgItem(IDC_BTNENHCTRL_MAIN_DETECTOR_CONNECT)->MoveWindow(rect2[2].left - 3, rect1[0].bottom, rect2[2].Width(), rect2[2].Height());
		GetDlgItem(IDC_BTNENHCTRL_MAIN_DETECTOR_CONNECT2)->MoveWindow(rect2[3].left - 3, rect1[0].bottom, rect2[3].Width(), rect2[3].Height());

		GetDlgItem(IDC_BTNENHCTRL_NG_VIEW)->ShowWindow(SW_SHOW);
	}

	InitList();
	m_ctrMainCurRecipe.SetWindowText(m_dlgRecipe->GetCurrentRecipeInfo());

	CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_PATTERN_0);
	pBtnEnh->SetValue(1);

	SetConMove();
	
	SetTimer(TIMER_SHOW_MAIN_DIALOG, 100, NULL);
	SetTimer(TIMER_CURRENT_TIME, 100, NULL);

	SetTimer(TIMER_HW_CONNECTION_CHECK, 10, NULL);
	SetTimer(TIMER_X_RAY_INFO, 100, NULL);
#ifdef INVERTER_COM
	SetTimer(TIMER_INVERTER_STATUS, INVERTER_WRITE_MS, NULL);
#endif
	SetTimer(TIMER_INVERTER_INFO, 100, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CInspectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CInspectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CInspectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/**
  @brief XRayCallBackFunc
  X-ray Class 에서 사용될 callback 함수
*/
void XRayCallBackFunc(int nData)
{
	m_This->SetXrayEndTimeWrite(nData);
}

/**
  @brief SetXrayEndTimeWrite
  X-Ray off 시점의 Date 저장
*/
void CInspectionDlg::SetXrayEndTimeWrite(int nData)
{
	if (nData == 0)
	{
		CTime cTime = CTime::GetCurrentTime();
		TIME_DATE cDate;
		cDate.nYear = cTime.GetYear();
		cDate.nMonth = cTime.GetMonth();
		cDate.nDay = cTime.GetDay();
		cDate.nHour = cTime.GetHour();
		cDate.nMinute = cTime.GetMinute();
		cDate.nSec = cTime.GetSecond();

		m_dlgSystem->SetXrayEndTimeWrite(cDate);
		m_XRayClass->SetXrayEndTime(cDate);
	}
	else
	{
		SetTimer(TIMER_X_RAY_ON_OFF, 500, NULL);
	}
}
/**
  @brief InitClass
  프로그램에서 사용 되는 Class 초기화
*/
int CInspectionDlg::InitClass()
{
	m_dlgSystem = new CDialogSystem();
	m_dlgSystem->Create(IDD_DIALOG_SYSTEM, this);

	m_dlgManual = new CDialogManual();
	m_dlgManual->Create(IDD_DIALOG_MANUAL, this);
	m_dlgManual->SetCallBack(XRayCallBackFunc);

	m_ImageSize = m_dlgSystem->GetInspImageSize();
#ifdef MIL_USE
	//m_pRepository = CRepository::Instance();
	CRepository::Instance();
	if (CRepository::Instance()->m_MilLib.DefaultSetting())
	{
		AfxMessageBox(_T("Cann't open default setting"));
		return -1;
	}
#endif	
	m_dlgRecipe = new CDialogRecipe();
	m_dlgRecipe->Create(IDD_DIALOG_RECIPE, this);
	m_dlgRecipe->SetInspectionType(m_dlgSystem->GetSystemPara().nInspectionType);
	m_dlgRecipe->SetImageSize(m_ImageSize.cx, m_ImageSize.cy);
	m_dlgRecipe->SetCallBack(XRayCallBackFunc);

	m_AtDetector = CAtDetector::Instance();

	m_IOClass = CIOClass::Instance();
	m_XRayClass = CXRayClass::Instance();
	CString strPort;
	strPort.Format(_T("COM%d"), m_dlgSystem->GetSystemPara().stXRay.nPortIndex);
	//test
	m_XRayClass->OpenPort(strPort, m_dlgSystem->GetSystemPara().stXRay.nPortBaudrate);

	StartIOValue();
#ifdef INVERTER_COM
	m_Inverter = CInverterClass::Instance();
	strPort.Format(_T("COM%d"), 1);// m_dlgSystem->GetSystemPara().stXRay.nPortIndex);
	m_Inverter->OpenPort(strPort, 19200);// 19200);// m_dlgSystem->GetSystemPara().stXRay.nPortBaudrate);
#endif
	
	m_dlgSystem->SetRecipe(m_dlgRecipe->GetCurrentRecipeInfo());

	m_dlgAlarm = new CDialogAlarm();
	m_dlgAlarm->Create(IDD_DIALOG_ALARM, this);

	m_dlgTextBox = new CDialogTextBox();
	m_dlgTextBox->Create(IDD_DIALOG_MAIN_TEXT_BOX, this);

	m_dlgInterLockStatus = new CDialgInterLockStatus();
	m_dlgInterLockStatus->Create(IDD_DIALOG_INTER_LOCK_STATUS, this);

	m_dlgAlarmLogReport = new CDialogAlarmLogReport();
	m_dlgAlarmLogReport->Create(IDD_DIALOG_ALARM_LOG_REPORT, this);

	if (m_dlgSystem->GetSystemPara().nInspectionType == 1)
	{
		m_dlgNgView = new CDialogNgView();
		m_dlgNgView->Create(IDD_DIALOG_NG_VIEW, this);
		m_dlgNgView->SetImageSize(m_ImageSize);
	}

	for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
	{
#ifdef MIL_USE
		
		if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
		{
			m_InspectionClass[i] = DBG_NEW CInspectionClass();
			m_InspectionClass[i]->SetMainHwnd(this->m_hWnd, TRUE, TRUE);
			m_InspectionClass[i]->InitImage(m_ImageSize.cx, m_ImageSize.cy);
			m_InspectionClass[i]->SetInitEachInspectionThread();
		}
		else if (m_dlgSystem->GetSystemPara().nInspectionType == 1)
		{
			//if (i == 0)
			{
				m_InspectionClass[i] = DBG_NEW CInspectionClass();
				m_InspectionClass[i]->SetMainHwnd(this->m_hWnd, TRUE, TRUE);
				m_InspectionClass[i]->InitBlobsImage(m_ImageSize.cx * INSP_RET, m_ImageSize.cy * INSP_RET);
			}
		}
#endif
	}

	m_AtDetector->SetImageSize(m_ImageSize.cy, m_ImageSize.cx);
	m_AtDetector->Init(m_dlgSystem->GetSystemPara().stDetector, m_dlgRecipe->GetCurrentRecipeInfo());
	m_AtDetector->SetMessage(this->m_hWnd, USE_MSG_DETECTOR_TO_MAIN);

	m_dlgResultNG = new CDialogResultNG();
	m_dlgResultNG->Create(IDD_DIALOG_RESULT_NG, this);
	
	return 0;
}
/**
  @brief InitDisplay
  Main Display 초기화
*/
int CInspectionDlg::InitDisplay()
{
#ifdef MIL_USE
	m_MilDisplay = CRepository::Instance()->m_MilLib.NewDisplay();
	m_MilDispBuff = CRepository::Instance()->m_MilLib.NewImage();
	m_MilDispBuff->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));

	m_Mil16bitBuff = CRepository::Instance()->m_MilLib.NewImage(16);
	m_Mil16bitBuff->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
	m_Mil16bitBuff2 = CRepository::Instance()->m_MilLib.NewImage(16);
	m_Mil16bitBuff2->Alloc2D(CSize(m_ImageSize.cy, m_ImageSize.cx));
	m_Mil8bitBuff = CRepository::Instance()->m_MilLib.NewImage();
	m_Mil8bitBuff->Alloc2D(CSize(m_ImageSize.cx * INSP_RET, m_ImageSize.cy * INSP_RET));
	//m_MilDispBuff->SelectWindow(m_MilDisplay, m_DlgRect->m_hWnd);
	m_MilDispBuff->SelectWindow(m_MilDisplay, GetDlgItem(IDC_STATIC_DISPLAY)->m_hWnd);
	
	double dRet;
	CRect rect;
	GetDlgItem(IDC_STATIC_DISPLAY)->GetClientRect(&rect);
	dRet = (double)rect.Width() / m_ImageSize.cx;
	m_MilDisplay->SetZoomFactor(dRet, dRet);
	//m_MilDisplay->SetViewAutoScale();
	m_MilDisplay->EnableOverlay();
	m_MilDisplay->EnableGraph();
	m_MilDisplay->FontSize(M_FONT_DEFAULT_LARGE);
#endif
	return 0;
}


void CInspectionDlg::OnBnClickedButtonImgLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE, _T("*.bmp"), _T("*.bmp"), OFN_HIDEREADONLY, _T("bmp(*.bmp)"));

	if (dlg.DoModal() == IDOK)
	{
#ifdef MIL_USE
		m_MilDispBuff->BufImport(dlg.GetPathName());
#endif
	}
}


void CInspectionDlg::OnBnClickedButtonImgSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(FALSE, _T("*.tif"), _T("*.tif"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("bmp(*.bmp)"));

	if (dlg.DoModal() == IDOK)
	{
#ifdef MIL_USE
		m_MilDispBuff->Export(dlg.GetPathName());
#endif
	}
}


void CInspectionDlg::OnBnClickedButtonAuto()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_dlgRecipe->IsWindowVisible())
		m_dlgRecipe->ShowWindow(SW_HIDE);

	if (m_dlgSystem->IsWindowVisible())
		m_dlgSystem->ShowWindow(SW_HIDE);

	if (m_dlgManual->IsWindowVisible())
		m_dlgManual->ShowWindow(SW_HIDE);

	m_bXRayOnCheck = FALSE;
	m_bFirstGrab = TRUE;
	if (!m_bAutoStatus)
	{
		m_bAutoStatus = TRUE;
		SetDlgItemText(IDC_BTNENHCTRL_AUTO, _T("정지(F5)"));
#ifndef HW_USE
		m_nTestIndex = 1;
#endif
	}
	else
	{
		m_bAutoStop = TRUE;
		m_bAutoStatus = FALSE;
		//SetDlgItemText(IDC_BTNENHCTRL_AUTO, _T("AUTO(F6)"));
	}

	//CInverterClass::Instance()->Close();
	//CString strPort;
	//strPort.Format(_T("COM%d"), 1);// m_dlgSystem->GetSystemPara().stXRay.nPortIndex);
	//m_Inverter->OpenPort(strPort, 19200);

	StartAutoMode(m_bAutoStatus);
}


void CInspectionDlg::OnBnClickedButtonManual()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_dlgRecipe->IsWindowVisible())
		m_dlgRecipe->ShowWindow(SW_HIDE);

	if (m_dlgSystem->IsWindowVisible())
		m_dlgSystem->ShowWindow(SW_HIDE);

	m_dlgManual->SetLoginType(m_LoginType);
	if(m_dlgSystem->GetSystemPara().nInspectionType == 0)
		m_dlgManual->SetXRayValue(m_dlgRecipe->GetInspectionPara(0).dVoltage, m_dlgRecipe->GetInspectionPara(0).dCurrent);
	else
		m_dlgManual->SetXRayValue(m_dlgRecipe->GetInspectionPara().dVoltage, m_dlgRecipe->GetInspectionPara().dCurrent);
	m_dlgManual->SetXrayEndTime(m_dlgSystem->GetXrayEndTimeRead());
	
	
	m_dlgManual->SetCalParameter(m_dlgSystem->GetSystemPara().stDetector.dScanSpeed, m_dlgSystem->GetSystemPara().stDetector.nCamLineToAvg);
	m_dlgManual->ShowWindow(SW_SHOW);
}


void CInspectionDlg::OnBnClickedButtonSystem()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_dlgManual->IsWindowVisible())
		m_dlgManual->ShowWindow(SW_HIDE);

	if (m_dlgRecipe->IsWindowVisible())
		m_dlgRecipe->ShowWindow(SW_HIDE);

	m_dlgSystem->SetLoginType(m_LoginType);
	m_dlgSystem->ShowWindow(SW_SHOW);
}


void CInspectionDlg::OnBnClickedButtonRecipe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_dlgManual->IsWindowVisible())
		m_dlgManual->ShowWindow(SW_HIDE);

	if (m_dlgSystem->IsWindowVisible())
		m_dlgSystem->ShowWindow(SW_HIDE);

	double dX, dY;
	m_dlgSystem->GetPixelSize(&dX, &dY);

	m_dlgRecipe->SetPixelSize(dX, dY);
	m_dlgRecipe->SetLoginType(m_LoginType);
	m_dlgRecipe->ShowWindow(SW_SHOW);
	SetTimer(TIMER_HIDE_RECIPE_DLG_CHEC, 100, NULL);
}
/**
  @brief DeleteClass
  프로그램에서 사용 되는 Class 종료
*/
void CInspectionDlg::DeleteClass()
{
	KillTimer(TIMER_CURRENT_TIME);
	KillTimer(TIMER_X_RAY_INFO);
	KillTimer(TIMER_X_RAY_ON_OFF);
	KillTimer(TIMER_HW_CONNECTION_CHECK);
	KillTimer(TIMER_HIDE_RECIPE_DLG_CHEC);

	g_SeqModule.Close();

	/*if (m_dlgLog != NULL)
	{
		m_dlgLog->DestroyWindow();
		delete m_dlgLog;
		m_dlgLog = NULL;
	}*/

	if (m_dlgAlarm != NULL)
	{
		m_dlgAlarm->DestroyWindow();
		delete m_dlgAlarm;
		m_dlgAlarm = NULL;
	}

	if (m_dlgManual != NULL)
	{
		m_dlgManual->DestroyWindow();
		delete m_dlgManual;
		m_dlgManual = NULL;
	}

	if (m_dlgRecipe != NULL)
	{
		m_dlgRecipe->DestroyWindow();
		delete m_dlgRecipe;
		m_dlgRecipe = NULL;
	}

	if (m_dlgSystem != NULL)
	{
		m_dlgSystem->DestroyWindow();
		delete m_dlgSystem;
		m_dlgSystem = NULL;
	}

	if (m_dlgTextBox != NULL)
	{
		m_dlgTextBox->DestroyWindow();
		delete m_dlgTextBox;
		m_dlgTextBox = NULL;
	}

	/*if (m_DlgRect != NULL)
	{
		m_DlgRect->SetActiveImageRect(TRUE);
		m_DlgRect->DeleteAll();
		m_DlgRect->DestroyWindow();
		delete m_DlgRect;
		m_DlgRect = NULL;
	}*/

	if (m_dlgNgView != NULL)
	{
		m_dlgNgView->DestroyWindow();
		delete m_dlgNgView;
		m_dlgNgView = NULL;
	}

	if (m_dlgInterLockStatus != NULL)
	{
		m_dlgInterLockStatus->DestroyWindow();
		delete m_dlgInterLockStatus;
		m_dlgInterLockStatus = NULL;
	}

	if (m_dlgAlarmLogReport != NULL)
	{
		m_dlgAlarmLogReport->DestroyWindow();
		delete m_dlgAlarmLogReport;
		m_dlgAlarmLogReport = NULL;
	}

	if (m_AtDetector != NULL)
	{
		if (m_AtDetector->Close())
		{
			delete m_AtDetector;
			m_AtDetector = NULL;
		}
	}

	if (m_XRayClass != NULL)
	{
		m_XRayClass->Close();
		delete m_XRayClass;
		m_XRayClass = NULL;
	}
#ifdef INVERTER_COM
	if (m_Inverter != NULL)
	{
		m_Inverter->Close();
		delete m_Inverter;
		m_Inverter = NULL;
	}
#endif
	if (m_IOClass != NULL)
	{
		delete m_IOClass;
		m_IOClass = NULL;
	}

	for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
	{
#ifdef MIL_USE
		if (m_InspectionClass[i] != NULL)
		{
			m_InspectionClass[i]->InitImage(0, 0, FALSE);

			delete m_InspectionClass[i];
			m_InspectionClass[i] = NULL;
		}
#endif
	}

	if (m_pMainLog != NULL)
	{
		delete m_pMainLog;
		m_pMainLog = NULL;
	}

	if (m_pInspectionLog != NULL)
	{
		delete m_pInspectionLog;
		m_pInspectionLog = NULL;
	}

	if (m_pAlarmLog != NULL)
	{
		delete m_pAlarmLog;
		m_pAlarmLog = NULL;
	}

	if (m_dlgResultNG != NULL)
	{
		delete m_dlgResultNG;
		m_dlgResultNG = NULL;
	}

#ifdef MIL_USE
	if (m_MilDisplay != NULL)
	{
		m_MilDisplay->EnableGraph(FALSE);
		m_MilDisplay->EnableOverlay(FALSE);
		m_MilDisplay->Close();
		m_MilDisplay = NULL;
	}

	if (m_MilDispBuff != NULL)
	{
		m_MilDispBuff->Close();
		m_MilDispBuff = NULL;
	}

	if (m_Mil16bitBuff != NULL)
	{
		m_Mil16bitBuff->Close();
		m_Mil16bitBuff = NULL;
	}

	if (m_Mil16bitBuff2 != NULL)
	{
		m_Mil16bitBuff2->Close();
		m_Mil16bitBuff2 = NULL;
	}

	if (m_Mil8bitBuff != NULL)
	{
		m_Mil8bitBuff->Close();
		m_Mil8bitBuff = NULL;
	}

	CRepository* pRepository = CRepository::Instance();
	CRepository::Instance()->m_MilLib.Close();
#endif
}

void CInspectionDlg::OnBnClickedButtonExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bLogWriteThreadEnd = TRUE;
	m_bRejecterThreadEnd = TRUE;
	

	KillTimer(TIMER_SHOW_MAIN_DIALOG);
	KillTimer(TIMER_CURRENT_TIME);
	KillTimer(TIMER_X_RAY_ON_OFF);
	KillTimer(TIMER_X_RAY_INFO);
	KillTimer(TIMER_HW_CONNECTION_CHECK);
	KillTimer(TIMER_HIDE_RECIPE_DLG_CHEC);
	KillTimer(TIMER_STOP_MODE_INTERLOCK_CHECK);
	KillTimer(TIMER_ALARM_SHOW);
	KillTimer(TIMER_INVERTER_STATUS);
	KillTimer(TIMER_DETECTOR_CAL_END_CHECK);
	KillTimer(TIMER_INVERTER_INFO);
	KillTimer(TIMER_IN_OUT_SENSER_UNLOCK);
	KillTimer(TIMER_NG_BUZZER);
	KillTimer(TIMER_INVERTER_RECONNECT);
	KillTimer(TIMER_INVERTER_RECONNECT2);
	KillTimer(TIMER_ALARM_BUZZER);

	for (int i = 0; i < m_strMainLog.size(); i++)
		m_strMainLog.pop_front();

	for (int i = 0; i < m_strInspectionLog.size(); i++)
		m_strInspectionLog.pop_front();

	for (int i = 0; i < m_dwRejecterStartTime.size(); i++)
		m_dwRejecterStartTime.pop_front();

	CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_REJECTER_SOL, FALSE);

	SetConMove();
	SetMC01On();
	SetXRayOn();
	SetTowerLamp(-1);

	DeleteClass();
	_CrtDumpMemoryLeaks();
	//his->DestroyWindow();
	//EndDialog(0);
	exit(0);
}
/**
  @brief LoginTypeEnableBtn
  Logint Type에 따른 버튼 활성화
*/
void CInspectionDlg::LoginTypeEnableBtn(LOGIN_TYPE login_type)
{
	CBtnenhctrl* btn;
	m_LoginType = login_type;
	switch (login_type)
	{
	case LOGIN_USER:
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_AUTO);// ->EnableWindow(TRUE);
		btn->EnableWindow(TRUE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MANUAL);// ->EnableWindow(FALSE);
		btn->EnableWindow(FALSE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_SYSTEM);// ->EnableWindow(FALSE);
		btn->EnableWindow(FALSE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_RECIPE);// ->EnableWindow(FALSE);
		btn->EnableWindow(TRUE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_LOGIN);
		btn->SetWindowText(_T("작업자"));
		break;
	case LOGIN_MANUFACTURE:
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_AUTO);// ->EnableWindow(TRUE);
		btn->EnableWindow(TRUE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MANUAL);// ->EnableWindow(TRUE);
		btn->EnableWindow(TRUE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_SYSTEM);// ->EnableWindow(TRUE);
		btn->EnableWindow(TRUE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_RECIPE);// ->EnableWindow(TRUE);
		btn->EnableWindow(TRUE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_LOGIN);
		btn->SetWindowText(_T("메뉴펙쳐"));
		break;
	case LOGIN_ADMIN:
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_AUTO);// ->EnableWindow(TRUE);
		btn->EnableWindow(TRUE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MANUAL);// ->EnableWindow(TRUE);
		btn->EnableWindow(TRUE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_SYSTEM);// ->EnableWindow(TRUE);
		btn->EnableWindow(TRUE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_RECIPE);// ->EnableWindow(TRUE);
		btn->EnableWindow(TRUE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_LOGIN);
		btn->SetWindowText(_T("관리자"));
		break;
	}
}

void CInspectionDlg::PatternEnableBtn()
{
	
	for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
	{
		CBtnenhctrl* btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_PATTERN_0);;
		switch (i)
		{
		case 0:
			btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_PATTERN_0);
			break;
		case 1:
			btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_PATTERN_1);
			break;
		case 2:
			btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_PATTERN_2);
			break;
		case 3:
			btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_PATTERN_3);
			break;
		case 4:
			btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_PATTERN_4);
			break;
		}
		btn->EnableWindow(TRUE);
		if (!m_dlgRecipe->GetInspectionPara(i).bInspUse)
		{
			btn->EnableWindow(FALSE);
		}
	}
}

void CInspectionDlg::SetMainRecipeName(CString strName)
{
	m_ctrMainCurRecipe.SetWindowText(strName);
}

void CInspectionDlg::AutoEnableBtn(BOOL isAuto)
{
	CBtnenhctrl* btn;
	SetXrayOnOffLabel(isAuto);
	if (isAuto)
	{
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_AUTO);// ->EnableWindow(TRUE);
		btn->EnableWindow(TRUE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MANUAL);// ->EnableWindow(FALSE);
		btn->EnableWindow(FALSE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_SYSTEM);// ->EnableWindow(FALSE);
		btn->EnableWindow(FALSE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_RECIPE);// ->EnableWindow(FALSE);
		btn->EnableWindow(FALSE);
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_EXIT);// ->EnableWindow(FALSE);
		btn->EnableWindow(FALSE);
	}
	else
	{
		m_bXRayOnCheck = FALSE;
		m_bAutoStatus = FALSE;
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_AUTO);
		btn->SetWindowText(_T("자동모드(F5)"));
		switch (m_LoginType)
		{
		case LOGIN_USER:
			btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_AUTO);// ->EnableWindow(TRUE);
			btn->EnableWindow(TRUE);
			btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MANUAL);// ->EnableWindow(FALSE);
			btn->EnableWindow(FALSE);
			btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_SYSTEM);// ->EnableWindow(FALSE);
			btn->EnableWindow(FALSE);
			btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_RECIPE);// ->EnableWindow(TRUE);
			btn->EnableWindow(TRUE);
			break;
		case LOGIN_MANUFACTURE:
		case LOGIN_ADMIN:
			btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_AUTO);// ->EnableWindow(TRUE);
			btn->EnableWindow(TRUE);
			btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MANUAL);// ->EnableWindow(TRUE);
			btn->EnableWindow(TRUE);
			btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_SYSTEM);// ->EnableWindow(TRUE);
			btn->EnableWindow(TRUE);
			btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_RECIPE);// ->EnableWindow(TRUE);
			btn->EnableWindow(TRUE);
			break;
		}
		btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_EXIT);// ->EnableWindow(FALSE);
		btn->EnableWindow(TRUE);
	}
}

void CInspectionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch ((int)nIDEvent)
	{
	case TIMER_SHOW_MAIN_DIALOG:
		if (this->IsWindowVisible())
		{
			//m_dlgLog->ShowWindow(SW_HIDE);
			KillTimer(TIMER_SHOW_MAIN_DIALOG);

			
#ifdef _DEBUG
#ifdef HW_USE
			PatternEnableBtn();

			CDialogLogin m_dlgLogin;

			int nLogin = (int)m_dlgLogin.DoModal();

			if (nLogin == 1)
			{
				LoginTypeEnableBtn(LOGIN_USER);
				SetTimer(TIMER_STOP_MODE_INTERLOCK_CHECK, 10, NULL);
			}
			else if( nLogin == 2)
			{
				LoginTypeEnableBtn(LOGIN_MANUFACTURE);
				SetTimer(TIMER_STOP_MODE_INTERLOCK_CHECK, 10, NULL);
			}
			else if (nLogin == 3)
			{
				LoginTypeEnableBtn(LOGIN_ADMIN);
				SetTimer(TIMER_STOP_MODE_INTERLOCK_CHECK, 10, NULL);
			}
			else
			{
				ClickBtnenhctrlExit();// OnBnClickedButtonExit();
				if (!m_bExit)
				{
					SetTimer(TIMER_SHOW_MAIN_DIALOG, 100, NULL);
					break;
				}
			}
#else
			LoginTypeEnableBtn(LOGIN_ADMIN);
#endif
#else
			PatternEnableBtn();

			CDialogLogin m_dlgLogin;

			int nLogin = (int)m_dlgLogin.DoModal();

			if (nLogin == 1)
			{
				LoginTypeEnableBtn(LOGIN_USER);
				SetTimer(TIMER_STOP_MODE_INTERLOCK_CHECK, 10, NULL);
			}
			else if (nLogin == 2)
			{
				LoginTypeEnableBtn(LOGIN_MANUFACTURE);
				SetTimer(TIMER_STOP_MODE_INTERLOCK_CHECK, 10, NULL);
			}
			else if (nLogin == 3)
			{
				LoginTypeEnableBtn(LOGIN_ADMIN);
				SetTimer(TIMER_STOP_MODE_INTERLOCK_CHECK, 10, NULL);
			}
			else
			{
				ClickBtnenhctrlExit();// OnBnClickedButtonExit();
				if (!m_bExit)
				{
					SetTimer(TIMER_SHOW_MAIN_DIALOG, 100, NULL);
					break;
				}
			}
			
#endif	
			if(m_dlgRecipe != NULL)
				m_dlgRecipe->SetInspectionType(m_dlgSystem->GetSystemPara().nInspectionType);

			g_SeqModule.Open(this->m_hWnd, 0, &m_list_Log);
			m_pSequenceThread = AfxBeginThread(SequenceThread, this, THREAD_PRIORITY_NORMAL, 0);
			SetThreadAffinityMask(m_pSequenceThread->m_hThread, 1);
			if (m_pSequenceThread)
				m_pSequenceThread->m_bAutoDelete = TRUE;

			m_pNgRejecterThread = AfxBeginThread(NgRejecterThread, this, THREAD_PRIORITY_NORMAL, 0);
			SetThreadAffinityMask(m_pNgRejecterThread->m_hThread, 1);
			if (m_pNgRejecterThread)
				m_pNgRejecterThread->m_bAutoDelete = TRUE;

			m_pLogWriteThread = AfxBeginThread(LogWriteThread, this, THREAD_PRIORITY_NORMAL, 0);
			SetThreadAffinityMask(m_pLogWriteThread->m_hThread, 1);
			if (m_pLogWriteThread)
				m_pLogWriteThread->m_bAutoDelete = TRUE;

			if (CXRayClass::Instance()->GetXrayWarmupCheck())
			{
				CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);
				AfxMessageBox(_T("X-Ray Warmup 필요 합니다 !!"));
				if (AfxMessageBox(_T("X-Ray Warmup 을 진행 하시겠습니까 ??"), MB_YESNO) == IDYES)
				{
					CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, TRUE);
					Delay(1000);
					CDialogXRayWarmup dlg;
					dlg.SetXrayEndTime(m_dlgSystem->GetXrayEndTimeRead());
					if (dlg.DoModal() == 1)
					{
						SetXrayEndTimeWrite(0);
					}
				}
			}
			
		}
		break;
	case TIMER_CURRENT_TIME:
		{
			CTime cTime;
			cTime = CTime::GetCurrentTime();
			CString strTime, strTime1;
			m_ctrMainCurTime.GetWindowText(strTime1);
			strTime = cTime.Format("[%Y-%m-%d] %H:%M:%S %p");
			if (this->IsWindowVisible() && (strTime1.Compare(strTime)))
			{
				//m_ctrMainCurTime.SetWindowText(strTime);
				SetDlgItemText(IDC_BTNENHCTRL_MAIN_CUR_TIME, strTime);
			}
		}
		break;
/**
  @brief TIMER_X_RAY_ON_OFF
  X-ray on 시에 Teaching된 kv,ma 까지 변화되는 양을 체크하여 메인 화면에 표시
*/
	case TIMER_X_RAY_ON_OFF:
		{
		
#ifdef HW_USE
			X_RAY_INFO info;
			info = m_XRayClass->GetXrayInfo();
			double dKv, dMa;
			dKv = info.nNoScalingCurKV * info.dScalingCurKV;
			dMa = info.nNoScalingCurMA * info.dScalingCurMA;

			double dSettingKv, dSettingMa;
			if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
			{
				dSettingKv = m_dlgRecipe->GetInspectionPara(0).dVoltage;
				dSettingMa = m_dlgRecipe->GetInspectionPara(0).dCurrent;
			}
			else
			{
				dSettingKv = m_dlgRecipe->GetInspectionPara().dVoltage;
				dSettingMa = m_dlgRecipe->GetInspectionPara().dCurrent;
			}

			if (info.isOnOff &&
				((dSettingKv - X_RAY_KV_OFFSET <= dKv) && (dKv <= dSettingKv + X_RAY_KV_OFFSET)) &&
				((dSettingMa - X_RAY_MA_OFFSET <= dMa) && (dMa <= dSettingMa + X_RAY_MA_OFFSET)))
#else
			m_nTestXRayCnt++;
			if (m_nTestXRayCnt < 10)
				break;

			double dKv, dMa;
			dKv = m_dlgRecipe->GetInspectionPara(0).dVoltage;
			dMa = m_dlgRecipe->GetInspectionPara(0).dCurrent;

			CString strText, strText1;
			strText.Format(_T("%0.3f kV"), dKv);
			//GetDlgItemText(IDC_BTNENHCTRL_MAIN_CUR_KV, strText1);
			//if (strText1.Compare(strText))
			SetDlgItemText(IDC_BTNENHCTRL_MAIN_CUR_KV, strText);


			strText.Format(_T("%0.3f mA"), dMa);
			//GetDlgItemText(IDC_BTNENHCTRL_MAIN_CUR_MA, strText1);
			//if (strText1.Compare(strText))
			SetDlgItemText(IDC_BTNENHCTRL_MAIN_CUR_MA, strText);

			if (((m_dlgRecipe->GetInspectionPara(0).dVoltage - X_RAY_KV_OFFSET <= dKv) && (dKv <= m_dlgRecipe->GetInspectionPara(0).dVoltage + X_RAY_KV_OFFSET)) &&
				((m_dlgRecipe->GetInspectionPara(0).dCurrent - X_RAY_MA_OFFSET <= dMa) && (dMa <= m_dlgRecipe->GetInspectionPara(0).dCurrent + X_RAY_MA_OFFSET)))
#endif
			{
				
				if (m_bXRayOnOff != m_bBeforeXRayOnOff)
				{
					KillTimer(TIMER_X_RAY_ON_OFF);
					if (!m_bAutoStop)
					{
#ifndef INVERTER_COM
						SetConMove(m_dlgSystem->GetSystemPara().stInverter.nDirection, TRUE);
#endif
						SetConMove(TRUE);
						SetTowerLamp(2);
						SYSTEM_PARA sys;
						sys = GetSystemPara();
#ifdef HW_USE
						CAtDetector::Instance()->SetScanSpeed(sys.stDetector.dScanSpeed);
						//CAtDetector::Instance()->GrabStart();
#endif
						m_bAutoStop = FALSE;

						CAtDetector::Instance()->GrabStart();

						g_SeqModule.Start();
						m_bXRayOnCheck = TRUE;
						ShowTextBox(_T(""), FALSE);

#ifndef INVERTER_COM
						CString strText;
						strText.Format(_T("%0.3f m/min"), m_dlgSystem->GetSystemPara().stDetector.dScanSpeed);
						GetDlgItem(IDC_BTNENHCTRL_INVERTER_CONNECT2)->SetWindowText(strText);
#endif
					}
				}
			}
#ifdef HW_USE
			else if(m_bXRayOnOff &&
				(dKv <= (X_RAY_KV_OFFSET * 5)) &&
				(dMa <= (X_RAY_MA_OFFSET * 5)))
#else
			if(m_bXRayOnOff)
#endif
			{
				ShowTextBox(_T(""), FALSE);
				KillTimer(TIMER_X_RAY_ON_OFF);
				SetTowerLamp(1);
				//g_SeqModule.Stop();
				SetXRayOn();
				AutoEnableBtn(FALSE);
				//m_MilDisplay->Clear();
#ifndef HW_USE
				strText.Format(_T("0.000 kV"));
				SetDlgItemText(IDC_BTNENHCTRL_MAIN_CUR_KV, strText);

				strText.Format(_T("0.000 mA"));
				SetDlgItemText(IDC_BTNENHCTRL_MAIN_CUR_MA, strText);
#endif
			}
		}
		break;
	case TIMER_X_RAY_INFO:
	{
		CString strText, strText1;
		X_RAY_INFO info;
		info = m_XRayClass->GetXrayInfo();

		double dKv, dMa;
		dKv = info.nNoScalingCurKV * info.dScalingCurKV;
		dMa = info.nNoScalingCurMA * info.dScalingCurMA;
#ifdef HW_USE
		strText.Format(_T("%0.3f kV"), dKv);
		GetDlgItemText(IDC_BTNENHCTRL_MAIN_CUR_KV, strText1);
		if (strText1.Compare(strText))
			SetDlgItemText(IDC_BTNENHCTRL_MAIN_CUR_KV, strText);


		strText.Format(_T("%0.3f mA"), dMa);
		GetDlgItemText(IDC_BTNENHCTRL_MAIN_CUR_MA, strText1);
		if (strText1.Compare(strText))
			SetDlgItemText(IDC_BTNENHCTRL_MAIN_CUR_MA, strText);
#endif
	}
		break;
/**
  @brief TIMER_HW_CONNECTION_CHECK
  Detector, X-Ray Connection 표시
*/
	case TIMER_HW_CONNECTION_CHECK:
	{
		if (m_XRayClass->GetPorConnection() != m_HW_Connection[0])
		{
			CBtnenhctrl* btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_X_RAY_CONNECT);
			m_HW_Connection[0] = m_XRayClass->GetPorConnection();

			if(m_HW_Connection[0])
				btn->SetBackColor(RGB(0, 255, 0));
			else
				btn->SetBackColor(RGB(255, 0, 0));
		}

		if (m_AtDetector->GetConnection() != m_HW_Connection[1])
		{
			CBtnenhctrl* btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_DETECTOR_CONNECT);
			m_HW_Connection[1] = m_AtDetector->GetConnection();

			if (m_HW_Connection[1])
				btn->SetBackColor(RGB(0, 255, 0));
			else
				btn->SetBackColor(RGB(255, 0, 0));
		}
#ifdef INVERTER_COM
		if (m_Inverter->GetOpenPort() != m_HW_Connection[2])
		{
			CBtnenhctrl* btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_INVERTER_CONNECT0);
			m_HW_Connection[2] = m_Inverter->GetOpenPort();

			if (m_HW_Connection[2])
				btn->SetBackColor(RGB(0, 255, 0));
			else
				btn->SetBackColor(RGB(255, 0, 0));
		}
#else
		CBtnenhctrl* btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_INVERTER_CONNECT0);

		if (btn->GetBackColor() != RGB(0, 255, 0))
			btn->SetBackColor(RGB(0, 255, 0));
#endif
		if (m_bInterLockStatus)
		{
			CBtnenhctrl* btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_INTERLOCK_STATUS_1);

			if (btn->GetBackColor() != RGB(255, 0, 0))
				btn->SetBackColor(RGB(255, 0, 0));
		}
		else
		{
			CBtnenhctrl* btn = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MAIN_INTERLOCK_STATUS_1);

			if (btn->GetBackColor() != RGB(0, 255, 0))
				btn->SetBackColor(RGB(0, 255, 0));
		}


		CString strFile, strDrive;
		wchar_t strTemp[MAX_CHAR_LENG];
		::GetModuleFileName(NULL, strTemp, 256);
		strFile.Format(_T("%s"), strTemp);
		double dUseSpace = 100. - UseDriveFreeSize(strFile.Mid(0, 1));
		GetDlgItemText(IDC_BTNENHCTRL_MAIN_CUR_RECIPE7, strDrive);
		if(strDrive.Compare(strFile.Mid(0, 1)))
			SetDlgItemText(IDC_BTNENHCTRL_MAIN_CUR_RECIPE7, strFile.Mid(0, 1));

		m_ctr_HardDiskUseSize.SetPos((int)dUseSpace);

		if (dUseSpace >= 70.)
			m_ctr_HardDiskUseSize.SetState(PBST_ERROR);
		else if (dUseSpace >= 50)
			m_ctr_HardDiskUseSize.SetState(PBST_PAUSED);
		else
			m_ctr_HardDiskUseSize.SetState(PBST_NORMAL);


	}
		break;
	case TIMER_HIDE_RECIPE_DLG_CHEC:
		if (!m_dlgRecipe->IsWindowVisible())
		{
			KillTimer(TIMER_HIDE_RECIPE_DLG_CHEC);
			if (m_dlgRecipe->GetClose() > 0)
			{
				LoadDayCountData();
				CountView();
				PatternEnableBtn();
				m_AtDetector->Close();
				Sleep(1000);
				m_AtDetector->Init(m_dlgSystem->GetSystemPara().stDetector, m_dlgRecipe->GetCurrentRecipeInfo());
				m_dlgSystem->SetRecipe(m_dlgRecipe->GetCurrentRecipeInfo());
				m_dlgRecipe->GetInspectionPara();
			}
		}
		break;
/**
  @brief TIMER_STOP_MODE_INTERLOCK_CHECK
  정지 상태에서 interlock check
*/
	case TIMER_STOP_MODE_INTERLOCK_CHECK:
		if (m_bAutoStop)
		{
			if (!GetEMG())
			{
				SetXRayOn(FALSE);

				SetConMove();
				SetMC01On();
					
				CInverterClass::Instance()->Close();
					
				m_bInterLockStatus = TRUE;
				
				m_AtDetector->GrabStop();
				m_bAutoStop = FALSE;
				//SetTowerLamp(0, 0);
				m_bAutoAlarm = FALSE;
				if (!m_dlgAlarm->IsWindowVisible())
				{
					if (m_dlgTextBox->IsWindowVisible())
						m_dlgTextBox->ShowWindow(SW_HIDE);

					m_bBuzzerOff = FALSE;
					AutoEnableBtn(FALSE);
					SetXrayOnOffLabel(FALSE);
					SetTimer(TIMER_ALARM_SHOW, 100, NULL);
					m_dlgAlarm->SetAlarmString(1, ALARM_MSG_EMG_INTER_LOCK);
					SetTimer(TIMER_INVERTER_RECONNECT, 100, NULL);
					m_bAlarmBuzzer = TRUE;
					SetTimer(TIMER_ALARM_BUZZER, 1000, NULL);
				}
			}

#ifndef _DEBUG
			if (!GetDoor())
			{
				//SetTowerLamp(0, 0);
				
				SetXRayOn(FALSE);
				SetConMove();
				m_bInterLockStatus = TRUE;
				m_bAutoAlarm = FALSE;
				m_bAutoStop = FALSE;

				if (!m_dlgAlarm->IsWindowVisible())
				{
					AutoEnableBtn(FALSE);
					SetXrayOnOffLabel(FALSE);
					m_bBuzzerOff = FALSE;
					m_dlgAlarm->SetAlarmString(3, ALARM_MSG_DOOR_INTER_LOCK);	
					SetTimer(TIMER_ALARM_SHOW, 100, NULL);
					m_bAlarmBuzzer = TRUE;
					SetTimer(TIMER_ALARM_BUZZER, 1000, NULL);
				}
			}
#endif
			/*if (GetInverterAlarm())
			{
				if (!m_dlgAlarm->IsWindowVisible())
				{
					m_dlgAlarm->SetAlarmString(3, ALARM_MSG_INVERTER_ALARM_INTER_LOCK);

					AutoEnableBtn(FALSE);
					SetXrayOnOffLabel(FALSE);
					SetXRayOn(FALSE);
					SetConMove();
					m_bInterLockStatus = TRUE;
				}
			}*/


			/*if (GetXRayLampALM() && m_bXrayLampCheck)
			{
				if (!m_dlgAlarm->IsWindowVisible())
				{
					m_dlgAlarm->SetAlarmString(5, ALARM_MSG_X_RAY_LAMP_ALARM_INTER_LOCK);

					AutoEnableBtn(FALSE);
					SetXrayOnOffLabel(FALSE);
					SetXRayOn(FALSE);
					SetConMove();
				}
			}*/

			/*if (GetTubeAlarm() || GetXRayInterLock())
			{
				if (!m_dlgAlarm->IsWindowVisible())
				{
					m_dlgAlarm->SetAlarmString(5, ALARM_MSG_TUBE_ALARM_INTER_LOCK);

					AutoEnableBtn(FALSE);
					SetXrayOnOffLabel(FALSE);
					SetXRayOn(FALSE);
				}
			}*/
		}
		break;
	case TIMER_ALARM_SHOW:
		if (!m_bAutoAlarm && !m_dlgAlarm->IsWindowVisible())
		{
			KillTimer(TIMER_ALARM_SHOW);
			KillTimer(TIMER_ALARM_BUZZER);
			SetTowerLamp(1);
			
			m_bInterLockStatus = FALSE;

			if (m_dlgAlarm->GetAlarmString().Find(_T("EMG")) > 0)
			{
				SetMC01On(TRUE);
				SetTimer(TIMER_INVERTER_RECONNECT, 100, NULL);
			}
		}
		break;
	case TIMER_INVERTER_STATUS:
#ifdef INVERTER_COM
		if (m_Inverter->GetOpenPort())
		{
			if (m_Inverter->GetWriteCmdCount() == 0)
			{
				m_Inverter->SetReadCommand(INVERTER_READ_CMD::INVERTER_READ_STATUS1);
				//사용 안함//m_Inverter->SetReadCommand(INVERTER_READ_CMD::INVERTER_READ_STATUS2);
				m_Inverter->SetReadCommand(INVERTER_READ_CMD::INVERTER_READ_STATUS3);
				m_Inverter->SetReadCommand(INVERTER_READ_CMD::INVERTER_READ_ALARM_INFO);
				m_Inverter->SetReadCommand(INVERTER_READ_CMD::INVERTER_READ_MOTOR_SPEED);
			}
		}
#endif
		break;
	case TIMER_DETECTOR_CAL_END_CHECK:
		if (!m_AtDetector->GetCalStart() && m_AtDetector->GetCalEnd())
		{
			KillTimer(TIMER_DETECTOR_CAL_END_CHECK);
			m_dlgTextBox->ShowWindow(SW_HIDE);
			m_XRayClass->SetXrayOnOff(FALSE);
			AfxMessageBox(_T("Detector Cal 종료"));
			m_AtDetector->SetCalDataLoad();
		}
		break;
	case TIMER_INVERTER_INFO:
#ifdef INVERTER_COM
		if (m_Inverter->GetOpenPort())
		{
			CString strText;
			strText.Format(_T("%0.3f m/min"), m_Inverter->GetInverterSpeed());
			GetDlgItem(IDC_BTNENHCTRL_INVERTER_CONNECT2)->SetWindowText(strText);
		}
#endif
		break;
	case TIMER_IN_OUT_SENSER_UNLOCK:
		//if (m_dlgAlarm->IsWindowVisible())
		{
#ifdef INVERTER_COM
			if (m_bInSenser && !m_IOClass->GetInputValue(INPUT_INDEX_INPUT_SENSER))
			{
				KillTimer(TIMER_IN_SENSER_X_RAY_OFF);
				KillTimer(TIMER_ALARM_BUZZER);
				KillTimer(TIMER_IN_OUT_SENSER_UNLOCK);
				if (m_dlgAlarm->IsWindowVisible())
					m_dlgAlarm->ShowWindow(SW_HIDE);
				Sleep(200);
				ClickBtnenhctrlAuto();
			}

			if (m_bOutSenser && !m_IOClass->GetInputValue(INPUT_INDEX_OUTPUT_SENSER))
			{
				KillTimer(TIMER_OUT_SENSER_X_RAY_OFF);
				KillTimer(TIMER_ALARM_BUZZER);
				KillTimer(TIMER_IN_OUT_SENSER_UNLOCK);
				if (m_dlgAlarm->IsWindowVisible())
					m_dlgAlarm->ShowWindow(SW_HIDE);
				Sleep(200);
				ClickBtnenhctrlAuto();
			}
#endif
		}
		break;
	case TIMER_NG_BUZZER:
		{
			if (m_bNgBuzzer)
			{
				//Buzzer off

				m_nNGBuzzerCnt++;
				//m_bNgBuzzer = FALSE;
				if (m_nNGBuzzerCnt > 5)
				{
					KillTimer(TIMER_NG_BUZZER);
					m_dlgResultNG->ShowWindow(SW_HIDE);
					SetTowerLamp(2);
				}
			}
			else
			{
				//Buzzer on
				m_bNgBuzzer = TRUE;
				//20230209 test
				SetTowerLamp(2, 0);
			}
		}
		break;
	case TIMER_INVERTER_RECONNECT:
		if (!m_bAutoAlarm && !m_dlgAlarm->IsWindowVisible())
		{
			KillTimer(TIMER_INVERTER_RECONNECT);
			m_dlgTextBox->SetTextBoxString(_T("System 초기화 !!"));
			m_dlgTextBox->ShowWindow(SW_SHOW);
			SetMC01On(TRUE);
			SetTimer(TIMER_INVERTER_RECONNECT2, 5000, NULL);
		}
		break;
	case TIMER_INVERTER_RECONNECT2:
		if (!m_bAutoAlarm && m_dlgTextBox->IsWindowVisible())
		{
			KillTimer(TIMER_INVERTER_RECONNECT2);
			m_dlgTextBox->ShowWindow(SW_HIDE);
#ifdef INVERTER_COM
			//m_Inverter->SetReconnect();
			CString strPort;
			strPort.Format(_T("COM%d"), 1);// m_dlgSystem->GetSystemPara().stXRay.nPortIndex);
			m_Inverter->OpenPort(strPort, 19200);// 19200);// m_dlgSystem->GetSystemPara().stXRay.nPortBaudrate);
#endif
		}
		break;
	case TIMER_ALARM_BUZZER:
		if (m_bAlarmBuzzer)
		{
			m_bAlarmBuzzer = FALSE;
			if (!m_bBuzzerOff)
				SetTowerLamp(0, 0);
			else
				SetTowerLamp(0);
		}
		else
		{
			m_bAlarmBuzzer = TRUE;
			SetTowerLamp(0);
		}
		break;
	case TIMER_IN_SENSER_X_RAY_OFF:
		m_nInSenserTime++;
		if ((m_nInSenserTime / 10.) > (double)m_dlgSystem->GetSystemPara().nInSensingTime)
		{
			m_nInSenserTime = 0;
			KillTimer(TIMER_IN_SENSER_X_RAY_OFF);
			SetXrayOnOffLabel(FALSE);
			SetXRayOn();
		}
		break;
	case TIMER_OUT_SENSER_X_RAY_OFF:
		m_nOutSenserTime++;
		if ((m_nOutSenserTime / 10.) > (double)m_dlgSystem->GetSystemPara().nOutSensingTime)
		{
			m_nOutSenserTime = 0;
			KillTimer(TIMER_OUT_SENSER_X_RAY_OFF);
			SetXrayOnOffLabel(FALSE);
			SetXRayOn();
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CInspectionDlg::InitList()
{
	CString strCol;
	LV_COLUMN m_lvColumn;

	if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
	{
		strCol.Format(_T("Ptn Idx"));
		m_lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		m_lvColumn.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;
		m_lvColumn.pszText = strCol.GetBuffer();
		m_lvColumn.iSubItem = 0;
		m_lvColumn.cx = 55;
		m_list_InspResultData.InsertColumn(0, &m_lvColumn);


		strCol.Format(_T("Teach Cnt"));
		m_lvColumn.pszText = strCol.GetBuffer();
		m_lvColumn.cx = 69;
		m_lvColumn.iSubItem = 1;
		m_list_InspResultData.InsertColumn(1, &m_lvColumn);

		strCol.Format(_T("Insp Cnt"));
		m_lvColumn.pszText = strCol.GetBuffer();
		m_lvColumn.cx = 65;
		m_lvColumn.iSubItem = 2;
		m_list_InspResultData.InsertColumn(2, &m_lvColumn);

		strCol.Format(_T("Over Cnt"));
		m_lvColumn.pszText = strCol.GetBuffer();
		m_lvColumn.cx = 65;
		m_lvColumn.iSubItem = 3;
		m_list_InspResultData.InsertColumn(3, &m_lvColumn);

		for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
		{
			strCol.Format(_T("%d Ptn"), i + 1);
			m_list_InspResultData.InsertItem(i, strCol);
			for (int j = 0; j < 3; j++)
			{
				m_list_InspResultData.SetItemText(i, j + 1, _T("0"));
			}
		}
	}
	else if (m_dlgSystem->GetSystemPara().nInspectionType == 1)
	{
		GetDlgItem(IDC_BTNENHCTRL_MAIN_PATTERN_0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_PATTERN_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_PATTERN_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_PATTERN_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_PATTERN_4)->ShowWindow(SW_HIDE);


		GetDlgItem(IDC_BTNENHCTRL_MAIN_INTERLOCK_STATUS_1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTNENHCTRL_MAIN_INTERLOCK_STATUS_2)->ShowWindow(SW_SHOW);

		CRect rect1, rect2;
		GetDlgItem(IDC_BTNENHCTRL_MAIN_INTERLOCK_STATUS_2)->GetWindowRect(&rect1);

		m_list_InspResultData.GetWindowRect(&rect2);

		m_list_InspResultData.MoveWindow(rect2.left - 3, rect1.bottom, rect2.Width(), rect2.bottom - rect1.bottom - 7 - 30);

		m_list_InspResultData.EnableWindow(TRUE);
		m_lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		m_lvColumn.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;

		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:
				strCol.Format(_T("번호"));
				m_lvColumn.cx = 50;
				break;
			case 1:
				strCol.Format(_T("짧은 길이"));
				m_lvColumn.cx = 69;
				break;
			case 2:
				strCol.Format(_T("긴 길이"));
				m_lvColumn.cx = 69;
				break;
			case 3:
				strCol.Format(_T("전체 길이"));
				m_lvColumn.cx = 69;
				break;
			}
			m_lvColumn.pszText = strCol.GetBuffer();
			m_lvColumn.iSubItem = i;
			m_list_InspResultData.InsertColumn(i, &m_lvColumn);
		}
	}

	m_list_InspResultData.EnableWindow(FALSE);
}



int CInspectionDlg::CountLoad()		// Product Count
{
	CTime NowTime = CTime::GetCurrentTime();
	CString strCurtime, strCurtimeday;
	strCurtime = NowTime.Format("%Y") + "\\" + NowTime.Format("%m");// + "\\" + NowTime.Format("%d");// 일까지,
	strCurtimeday = NowTime.Format("%d");// 일까지,,

	LoadDayCountData();
	return m_iCount;
}

void CInspectionDlg::CountOKAdd()		// Product Count
{
	CTime NowTime = CTime::GetCurrentTime();
	CString strCurtime, strCurtimeday;
	strCurtime = NowTime.Format("%Y") + "\\" + NowTime.Format("%m");// + "\\" + NowTime.Format("%d");// 일까지,
	strCurtimeday = NowTime.Format("%d");// 일까지,,

	LoadDayCountData();
	m_iCount++;
	m_iOKCount++;
	SaveDayCountData();
	CountView();
}

void CInspectionDlg::CountNGAdd(int nNgIndex)		// Product Count
{
	CTime NowTime = CTime::GetCurrentTime();
	CString strCurtime, strCurtimeday;
	strCurtime = NowTime.Format("%Y") + "\\" + NowTime.Format("%m");// + "\\" + NowTime.Format("%d");// 일까지,
	strCurtimeday = NowTime.Format("%d");// 일까지,,

	LoadDayCountData();
	m_iCount++;
	m_iPointNGConunt[nNgIndex]++;

	SaveDayCountData();
	CountView();
}

void CInspectionDlg::CountReset()		// Product Count
{
	CTime NowTime = CTime::GetCurrentTime();
	CString strCurtime, strCurtimeday;
	strCurtime = NowTime.Format("%Y") + "\\" + NowTime.Format("%m");// + "\\" + NowTime.Format("%d");// 일까지,
	strCurtimeday = NowTime.Format("%d");// 일까지,,

	LoadDayCountData();
	m_iCount = 0;
	m_iOKCount = 0;

	for (int i = 0; i < MAX_RSLT_POINT_CNT; i++)
		m_iPointNGConunt[i] = 0;

	SaveDayCountData();
	CountView();
}

void CInspectionDlg::CountView()
{
	CString strText;
	//CountLoad();
	strText.Format(_T("%d"), m_iCount);
	m_ctrMainTotalCnt.SetWindowTextW(strText);

	strText.Format(_T("%d"), m_iOKCount);
	m_ctrMainOkCnt.SetWindowTextW(strText);
	strText.Format(_T("%d"), m_iPointNGConunt[0]);
	m_ctrMainNgCnt1.SetWindowTextW(strText);
	strText.Format(_T("%d"), m_iPointNGConunt[1]);
	m_ctrMainNgCnt2.SetWindowTextW(strText);

	if (m_iPointNGConunt[0] == 0 && m_iCount == 0)
		strText.Format(_T("0.00%%"));
	else
		strText.Format(_T("%.2f%%"), (double)m_iPointNGConunt[0] / (double)m_iCount * 100.);		// ng Ratio
	m_ctrMainNGRatio1.SetWindowTextW(strText);

	if (m_iPointNGConunt[1] == 0 && m_iCount == 0)
		strText.Format(_T("0.00%%"));
	else
		strText.Format(_T("%.2f%%"), (double)m_iPointNGConunt[1] / (double)m_iCount * 100.);		// ng Ratio
	m_ctrMainNGRatio2.SetWindowTextW(strText);
}

void CInspectionDlg::LoadDayCountData()
{
	CProfileDataIF cDataIf;

	CString strTmp(_T("")), strFileName(_T("")), strNode(_T(""));
	CTime NowTime = CTime::GetCurrentTime();
	CString strCurtime, strCurtimeday;
	strCurtime = NowTime.Format(_T("%Y")) + _T("\\") + NowTime.Format(_T("%m"));// + "\\" + NowTime.Format("%d");// 일까지,
	strCurtimeday = NowTime.Format(_T("%d"));// 일까지,,

	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	strFileName.Format(_T("%s\\Count\\%s\\%s\\%s.ini"), strTemp, strCurtime, m_dlgRecipe->GetCurrentRecipeInfo(), strCurtimeday);
	cDataIf.SetFilePath(strFileName);
	strNode.Format(_T("COUNT DATA"));
	strTmp.Format(_T("Total Count"));
	m_iCount = cDataIf.GetInt(strNode, strTmp, 0);
	strTmp.Format(_T("OK"));
	m_iOKCount = cDataIf.GetInt(strNode, strTmp, 0);

	if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
	{
		strTmp.Format(_T("Count NG"));
		m_iPointNGConunt[0] = cDataIf.GetInt(strNode, strTmp, 0);
		strTmp.Format(_T("overlap NG"));
		m_iPointNGConunt[1] = cDataIf.GetInt(strNode, strTmp, 0);
	}
	else if (m_dlgSystem->GetSystemPara().nInspectionType == 1)
	{
		strTmp.Format(_T("NG"));
		m_iPointNGConunt[0] = cDataIf.GetInt(strNode, strTmp, 0);
	}
}

void CInspectionDlg::SaveDayCountData()
{
	CProfileDataIF cDataIf;
	CStdioFile pFile;
	CFileFind finder;

	CString strTmp(_T("")), strFileName(_T("")), strNode(_T(""));
	CTime NowTime = CTime::GetCurrentTime();
	CString strCurtime, strCurtimeday, strCount;
	strCurtime = NowTime.Format(_T("%Y")) + _T("\\") + NowTime.Format(_T("%m"));// + "\\" + NowTime.Format("%d");// 일까지,
	strCurtimeday = NowTime.Format(_T("%d"));// 일까지,,
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	strTmp.Format(_T("%s\\Count\\%s\\%s"), strTemp, strCurtime, m_dlgRecipe->GetCurrentRecipeInfo());
	CreateDir(strTmp);

	strFileName.Format(_T("%s\\Count\\%s\\%s\\%s.ini"), strTemp, strCurtime, m_dlgRecipe->GetCurrentRecipeInfo(), strCurtimeday);
	cDataIf.SetFilePath(strFileName);
	//CreateDir(strFileName);//경로에따라 찾고 없으면 만든다...

	strNode.Format(_T("COUNT DATA"));
	strTmp.Format(_T("Total Count"));
	cDataIf.SetInt(strNode, strTmp, m_iCount);
	strTmp.Format(_T("OK"));
	cDataIf.SetInt(strNode, strTmp, m_iOKCount);

	if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
	{
		strTmp.Format(_T("Count NG"));
		cDataIf.SetInt(strNode, strTmp, m_iPointNGConunt[0]);
		strTmp.Format(_T("overlap NG"));
		cDataIf.SetInt(strNode, strTmp, m_iPointNGConunt[1]);
	}
	else if (m_dlgSystem->GetSystemPara().nInspectionType == 1)
	{
		strTmp.Format(_T("NG"));
		cDataIf.SetInt(strNode, strTmp, m_iPointNGConunt[0]);
	}
}

void CInspectionDlg::ClickBtnenhctrlMainCountReset()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
#ifndef _DEBUG
	CountReset();
	
#else
	CountReset();
#endif
}

void CInspectionDlg::SetXrayOnOffLabel(BOOL isOn)
{
	m_bBeforeXRayOnOff = isOn;
	if (isOn)
	{
		m_ctrMainXrayOnOff.SetWindowText(_T("X-Ray On"));
		m_ctrMainXrayOnOff.SetVistaColorGlossyEffectUpper(RGB(255, 0, 0));
	}
	else
	{
		m_ctrMainXrayOnOff.SetWindowText(_T("X-Ray Off"));
		m_ctrMainXrayOnOff.SetVistaColorGlossyEffectUpper(RGB(0, 255, 0));
	}
}
/**
  @brief SequenceStartIOCheck
  Sequence 시작시에 IO check
*/
int CInspectionDlg::SequenceStartIOCheck()
{
#ifndef HW_USE
	return 0;
#else
	int nRslt = 0;
	if (!GetEMG() && nRslt == 0)
	{
		nRslt = -1;
	}

	if (!GetKeySw() && nRslt == 0)
	{
		nRslt = -2;
	}

	if (!GetDoor() && nRslt == 0)
	{
		nRslt = -3;
	}

	if (GetInverterAlarm() && nRslt == 0)
	{
		//nRslt = -4;
	}

	if (GetTubeAlarm() && nRslt == 0)
	{
		nRslt = -5;
	}

	if (!GetXRayLampALM() && nRslt == 0)
	{
#ifdef INVETER_COM
		nRslt = -6;
#endif
	}

	if (m_XRayClass->GetXrayWarmupCheck())
	{
		m_IOClass->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);
		nRslt = -7;
	}

	if (!CIOClass::Instance()->GetOutputBit(6))
	{
		//nRslt = -8;
	}


	return nRslt;
#endif
}

void CInspectionDlg::MainLogWrite(CString strLog)
{
	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);
	CString strText;

	strText.Format(_T("%02d:%02d:%02d:%03d - %s"), lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds, strLog);

	SendMessage(USE_MSG_LOG_TO_MAIN, (WPARAM)strText.GetBuffer(), strText.GetLength());
}

void CInspectionDlg::StartAutoMode(BOOL isAuto)
{
	if (isAuto)
	{
		m_bAutoAlarm = FALSE;
		CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, TRUE);
		CString strText;
		int nAlarmCode = 0;
		int nRslt = SequenceStartIOCheck();

#ifndef HW_USE
		nRslt = -3;
#endif

		if (nRslt == 0 && !GetHardDiskFreeCheck())
			nRslt = -9;

		switch (nRslt)
		{
		case 0://ok
			//strText.Format(_T("Auto Start Interlock Check OK!"));
			break;
		case -1://emg
			strText.Format(_T("AlarmCode:01, 자동모드 Inter Lock 확인, EMG!"));
			nAlarmCode = ALARM_CODE_EMG;
			break;
		case -2://key sw
			strText.Format(_T("AlarmCode:02, 자동모드 Inter Lock 확인, Key SW!"));
			nAlarmCode = ALARM_CODE_KEY_SW;
			break;
		case -3://door
			strText.Format(_T("AlarmCode:03, 자동모드 Inter Lock 확인, Door!"));
			nAlarmCode = ALARM_CODE_DOOR;
			break;
		case -4://inverter alarm
			strText.Format(_T("AlarmCode:04, 자동모드 Inter Lock 확인, Inverter Alarm!"));
			nAlarmCode = ALARM_CODE_INVERTER_ALARM;
			break;
		case -5://tube alarm
			strText.Format(_T("AlarmCode:05, 자동모드 Inter Lock 확인, Tube Alarm!"));
			nAlarmCode = ALARM_CODE_TUBE_ALARM;
			break;
		case -6:
			strText.Format(_T("AlarmCode:06, 자동모드 Inter Lock 확인, X-Ray Lamp Alarm!"));
			nAlarmCode = ALARM_CODE_X_RAY_LAMP_ALARM;
			break;
		case -7:
			strText.Format(_T("X-Ray Warmup Not Used!\nWarmup Please!"));
			nAlarmCode = ALARM_CODE_X_RAY_WARMUP_NOT_USED;
			if (!m_dlgAlarm->IsWindowVisible())
			{
				m_dlgAlarm->SetAlarmString(nAlarmCode, strText);
			}
			break;
		case -8:
			strText.Format(_T("자동모드 Interlock 확인, MC01 Off!"));
			nAlarmCode = ALARM_CODE_MC01_ON_CHECK;
			break;
		case -9:
			strText.Format(_T("HDD 용량 확인!"));
			nAlarmCode = ALARM_CODE_HDD_SPACE;
			break;
		}

		if(!strText.IsEmpty())
			MainLogWrite(strText);

		if (nRslt == 0)
		{
			if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
			{
				for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
				{
#ifdef MIL_USE
					m_InspectionClass[i]->SetInspPara(CMilSystem::Instance()->GetSystemID(), m_dlgRecipe->GetInspectionPara(i), m_dlgRecipe->GetCurrentRecipeInfo(), i);
#endif
				}
			}
			else if (m_dlgSystem->GetSystemPara().nInspectionType == 1)
			{
				
				double dX, dY;
				m_dlgSystem->GetPixelSize(&dX, &dY);
				m_dlgRecipe->GetInspectionPara();

				int nInspTypeCnt = m_dlgRecipe->GetInspTypCnt();
				
				for (int i = 0; i < nInspTypeCnt; i++)
				{
					m_InspectionClass[i]->SetInspBlobsPara(m_dlgRecipe->GetInspectionTypePara(i));
					m_InspectionClass[i]->SetRecipeName(m_dlgRecipe->GetCurrentRecipeInfo());
					m_InspectionClass[i]->SetPixelSize(dX, dY);
				}
			}

			ShowTextBox(_T("X-Ray On 대기 중!!!"));
			m_bXRayOnCheck = FALSE;
			m_bXRayOnOff = FALSE;
			m_bBeforeXRayOnOff = FALSE;
			//m_XRayClass->SetXrayOnOff(TRUE);
			if (SetXRayOn(TRUE))
			{

				if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
				{
					if (CXRayClass::Instance()->SetXrayVolCur(m_dlgRecipe->GetInspectionPara(0).dVoltage, m_dlgRecipe->GetInspectionPara(0).dCurrent))
						CXRayClass::Instance()->SetXrayOnOff(TRUE);
				}
				else
				{
					if (CXRayClass::Instance()->SetXrayVolCur(m_dlgRecipe->GetInspectionPara().dVoltage, m_dlgRecipe->GetInspectionPara().dCurrent))
						CXRayClass::Instance()->SetXrayOnOff(TRUE);

					
				}
				//SetMC01On(TRUE);
				g_SeqModule.SetSeqFirst(FALSE);
				g_SeqModule.SetInSensingTime(m_dlgSystem->GetSystemPara().nInSensingTime);
				g_SeqModule.SetOutSensingTime(m_dlgSystem->GetSystemPara().nOutSensingTime);
				CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_REJECTER_SOL, FALSE);
				Delay(1000);

#ifndef HW_USE
				m_nTestXRayCnt = 0;
#endif

#ifdef HW_USE
				if (!GetXRayLampALM())
#else
				if(TRUE)
#endif
				{
					SetTimer(TIMER_X_RAY_ON_OFF, 100, NULL);
					m_bXrayLampCheck = TRUE;
				}
				else
				{
					m_bXrayLampCheck = FALSE;
					ShowTextBox(_T(""), FALSE);
					strText.Format(_T("자동모드 Interlock 확인, X-Ray Lamp Alarm!"));
					nAlarmCode = ALARM_CODE_X_RAY_LAMP_ALARM;
					MainLogWrite(strText);
					CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);
					isAuto = FALSE;
					m_dlgAlarm->SetAlarmString(nAlarmCode, strText);
					//if (!m_dlgAlarm->IsWindowVisible())
					{
						m_dlgAlarm->ShowWindow(SW_SHOW);
						SetXRayOn();
						AutoEnableBtn(FALSE);
						SetXrayOnOffLabel(FALSE);
						m_bAutoAlarm = TRUE;
						SetTowerLamp(0);
						SetTimer(TIMER_ALARM_SHOW, 100, NULL);
						m_bBuzzerOff = FALSE;
						m_bAlarmBuzzer = TRUE;
						SetTimer(TIMER_ALARM_BUZZER, 1000, NULL);
					}
				}
			}
			/*else
			{
				if (m_dlgTextBox->IsWindowVisible())
				{
					ShowTextBox(_T(""), FALSE, TRUE);
				}
				m_XRayClass->SetXrayOnOff(FALSE);

				m_bXRayOnOff = TRUE;
				m_bBeforeXRayOnOff = TRUE;
				m_bXrayLampCheck = FALSE;
				SetConMove();
				SetMC01On();
				m_AtDetector->GrabStop();
				ShowTextBox(_T("Stop Wait !!!"));
				Delay(500);
				SetTimer(TIMER_X_RAY_ON_OFF, 100, NULL);
			}*/
		}
		else
		{
			CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);
			isAuto = FALSE;
			m_bXRayOnOff = TRUE;
			m_bBeforeXRayOnOff = TRUE;
			AutoEnableBtn(FALSE);
			SetXrayOnOffLabel(FALSE);
			SetTowerLamp(0);
			m_dlgAlarm->SetAlarmString(nAlarmCode, strText);
			//m_dlgAlarm->ShowWindow(SW_SHOW);
			m_bBuzzerOff = FALSE;
			m_bAlarmBuzzer = TRUE;
			SetTimer(TIMER_ALARM_BUZZER, 1000, NULL);
			SetTimer(TIMER_ALARM_SHOW, 100, NULL);
		}
	}
	else
	{
		//ShowTextBox(_T("Stop Wait!!!"));
		if (m_dlgTextBox->IsWindowVisible())
		{
			ShowTextBox(_T(""), FALSE, TRUE);
		}
		CAtDetector::Instance()->GrabStop();
		g_SeqModule.Stop();

		m_XRayClass->SetXrayOnOff(FALSE);
		
		m_bXRayOnOff = TRUE;
		m_bBeforeXRayOnOff = TRUE;
		m_bXrayLampCheck = FALSE;
		SetConMove();
		//SetMC01On();
		//m_AtDetector->GrabStop();
		ShowTextBox(_T("정지 대기 중 !!!"));
		//Delay(500);
		SetTimer(TIMER_X_RAY_ON_OFF, 100, NULL);
#ifndef INVERTER_COM
		CString strText;
		strText.Format(_T("0.000 m/min"));
		GetDlgItem(IDC_BTNENHCTRL_INVERTER_CONNECT2)->SetWindowText(strText);
#endif
		
		//Delay(1000);
		//SetXRayOn();
		//g_SeqModule.Stop();
		
	}
	m_bAutoStop = !isAuto;
	if(isAuto)
		AutoEnableBtn(isAuto);
}
/**
  @brief GetInspectionThreadEnd
  검사 Thread 종료 확인
*/
BOOL CInspectionDlg::GetInspectionThreadEnd()
{
	if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
	{
		for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
		{
#ifdef MIL_USE
			if (!m_InspectionClass[i]->GetAllInspectionEnd())
				return FALSE;
#endif
		}
	}
	else if (m_dlgSystem->GetSystemPara().nInspectionType == 1)
	{

		int nInspTypeCnt = m_dlgRecipe->GetInspTypCnt();
		BOOL isEnd = TRUE;
		for (int i = 0; i < nInspTypeCnt; i++)
		{
			if (!m_InspectionClass[i]->GetBlobsInspectionEnd())
			{
				isEnd = FALSE;
				break;
			}
		}
		if (!isEnd)
			return FALSE;
	}

	return TRUE;
}

UINT CInspectionDlg::SequenceThread(LPVOID pParam)
{
	CInspectionDlg* pDlg = (CInspectionDlg*)pParam;
	pDlg->g_SeqModule.SequenceLoop();

	return 0;
}

UINT CInspectionDlg::NgRejecterThread(LPVOID pParam)
{
	CInspectionDlg* pDlg = (CInspectionDlg*)pParam;
	pDlg->NgRejecterSequence();

	return 0;
}

void CInspectionDlg::NgRejecterSequence()
{
	m_bRejecterStart = FALSE;
	m_bEndRejecterStart = FALSE;
	while (TRUE)
	{
		/*if (!m_bRejecterStart && !m_bEndRejecterStart)
		{
			if (m_dwRejecterStartTime.size() > 0)
				m_bRejecterStart = TRUE;
		}*/
		if (m_bRejecterQuePop)
		{
			m_bRejecterQuePop = FALSE;
			m_bRejecterStart = FALSE;
			m_bEndRejecterStart = FALSE;
			m_dwRejecterStartTime.pop_front();
		}

		if (m_bRejecterStart && m_dwRejecterStartTime.size() > 0)
		{
			if (RejecterDelayCheck(m_dwRejecterStartTime[0], m_dlgSystem->GetSystemPara().stRejecter.nDelayTime))
			{
				m_dwRejecterStartTime.pop_front();
				StartRejecterStart();
				m_bEndRejecterStart = TRUE;
			}
		}

		if (m_bEndRejecterStart)
		{
			if (RejecterDelayCheck(m_dwEndRejecterStartTime, m_dlgSystem->GetSystemPara().stRejecter.nEndDelayTime))
			{
				EndRejecterStart();
			}
		}

		if (m_bRejecterThreadEnd)
			break;

		Sleep(1);
	}
}

BOOL CInspectionDlg::RejecterDelayCheck(DWORD dwStartTime, int nDelay)
{
	BOOL rslt = FALSE;

	if (_TimerCounterMain - dwStartTime > (DWORD)nDelay)
	{
		rslt = TRUE;
	}

	return rslt;
}

BOOL CInspectionDlg::StartRejecterStart()
{
	BOOL rslt = TRUE;
	m_bRejecterStart = FALSE;
	m_dwEndRejecterStartTime = _TimerCounterMain;

	CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_REJECTER_SOL, TRUE);

	return rslt;
}

BOOL CInspectionDlg::EndRejecterStart()
{
	BOOL rslt = TRUE;
	m_bEndRejecterStart = FALSE;

	CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_REJECTER_SOL, FALSE);

	return rslt;
}

UINT CInspectionDlg::LogWriteThread(LPVOID pParam)
{
	CInspectionDlg* pDlg = (CInspectionDlg*)pParam;

	pDlg->LogWriteMessage();

	return 0;
}

void CInspectionDlg::LogWriteMessage()
{
	while (TRUE)
	{

		if (m_strMainLog.size() > 0)
		//if(g_SeqModule.m_strMainLogString.size() > 0)
		{
			SendMessage(USE_MSG_LOG_WRITE, 0, 0);
		}

		if (m_strInspectionLog.size() > 0)
		{
			SendMessage(USE_MSG_LOG_WRITE, 1, 0);
		}

		if (m_bLogWriteThreadEnd)
			break;

		Sleep(10);
	}

}

LRESULT CInspectionDlg::OnLogWriteMessage(WPARAM para0, LPARAM para1)
{
	int nMain = (int)para0;
	CString strLog;
	switch (nMain)
	{
	case 0://Main Log
		strLog.Format(_T("%s"), m_strMainLog[0]);
		//strLog = g_SeqModule.m_strMainLogString[0];
		m_pMainLog->WriteText(strLog, TRUE);

		if (strLog.Find(FIND_ALARM_LOG_STRING, 0) > 0)
			m_pAlarmLog->WriteText(strLog);

		m_strMainLog.pop_front();
		//g_SeqModule.m_strMainLogString.pop_front();
		break;
	case 1://Insepction Log
		strLog.Format(_T("%s"), m_strInspectionLog[0]);
		m_pInspectionLog->WriteText(strLog);
		m_strInspectionLog.pop_front();
		break;
	}
	strLog.Empty();
	return 0;
}

void CInspectionDlg::SetInitInspection()
{
#ifdef HW_USE
	m_bXRayOnCheck = FALSE;
#else
	m_bXRayOnCheck = TRUE;
#endif
	m_bGrabStart = FALSE;
	m_bGrabEnd = FALSE;
	m_bInspectionEnd = FALSE;
	m_bInspectionStart = FALSE;

	//m_DlgRect->SetActiveImageRect(FALSE);
	//m_DlgRect->SetImageSize(m_ImageSize.cx, m_ImageSize.cy);
	//m_DlgRect->DeleteAll();

	//CAtDetector::Instance()->GrabStart();
	//SetTimer(99999, 5000, NULL);
}

void CInspectionDlg::SelectDisplay(int nIdx)
{
	if (nIdx >= MAX_PATTERN_ADD_CNT)
		return;

	for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
		m_ctrlSelectResultDisplayIdx[i].SetValue(FALSE);

	m_ctrlSelectResultDisplayIdx[nIdx].SetValue(TRUE);

	m_nSelectResultDisplayIndex = nIdx;
	//SetResultDisplay();
	if (!m_dlgRecipe->GetInspectionPara(m_nSelectResultDisplayIndex).bInspUse)
		return;

	//m_DlgRect->SetActiveImageRect(TRUE);
#ifdef MIL_USE
	m_InspectionClass[m_nSelectResultDisplayIndex]->SetResultDisplay(TRUE);
	//m_DlgRect->DeleteAll();

	m_MilDisplay->ClearOverlay();
	m_MilDisplay->EnableGraph(FALSE);
	m_MilDisplay->EnableOverlay(FALSE);
	m_MilDisplay->EnableGraph();
	m_MilDisplay->EnableOverlay();
	m_MilDisplay->FontSize(M_FONT_DEFAULT_LARGE);
	m_InspectionClass[m_nSelectResultDisplayIndex]->SetResultDisplay(m_MilDisplay, NULL);
#endif
	//m_DlgRect->SetActiveImageRect(FALSE);

	//m_DlgRect->Invalidate();
}



void CInspectionDlg::SetResultDisplay()
{
	SendMessage(USE_MSG_INSPECTION_END, 0, 0);
}
#ifndef HW_USE
BOOL CInspectionDlg::LoadSqImage()
{
	CString strFilePath;

	if (m_nTestIndex > 124)
	{
		//return FALSE;
		//OnBnClickedButtonAuto();
		//return FALSE;
		m_nTestIndex = 1;
	}

	strFilePath.Format(_T("%s%s\\%03d.tiff"), TEST_IMAGE_FILE_PATH, m_dlgRecipe->GetCurrentRecipeInfo(), m_nTestIndex);
#ifdef MIL_USE
	m_Mil16bitBuff->BufImport(strFilePath, M_TIFF, M_LOAD);
#endif

	m_dwRejecterStartTime.push_back(_TimerCounterMain);
	m_bRejecterQuePop = FALSE;

	CSize imageSize;
	imageSize.SetSize(m_ImageSize.cx, m_ImageSize.cy);
	CMilBuffer* milTempBuff, *milTempBuff2, *milTempBuff3, *milTempBuff4;// , * milTempBuff2;
	milTempBuff = CRepository::Instance()->m_MilLib.NewImage();
	milTempBuff->Alloc2D(CSize(imageSize.cx, imageSize.cy));
	milTempBuff2 = CRepository::Instance()->m_MilLib.NewImage(16);
	milTempBuff2->Alloc2D(CSize(imageSize.cx * INSP_RET, imageSize.cy * INSP_RET));
	milTempBuff3 = CRepository::Instance()->m_MilLib.NewImage();
	milTempBuff3->Alloc2D(CSize(imageSize.cx * INSP_RET, imageSize.cy * INSP_RET));
	milTempBuff4 = CRepository::Instance()->m_MilLib.NewImage();
	milTempBuff4->Alloc2D(CSize(imageSize.cx, imageSize.cy));
	m_Mil16bitBuff->ResizeBuf(milTempBuff2, CdblSize(INSP_RET, INSP_RET));

	long nPitch;
#ifdef HW_USE
	nPitch = imageSize.cx;
#else
	MIL_INT milPitch;
	MbufInquire(milTempBuff2->GetIDBuffer(), M_PITCH, &milPitch);
	nPitch = (long)milPitch;
#endif
	BYTE* pData, *pData3;
	WORD* pData2, * pData4;
	pData2 = DBG_NEW WORD[(imageSize.cx * INSP_RET) * (imageSize.cy * INSP_RET)];
	pData4 = DBG_NEW WORD[(imageSize.cx * INSP_RET) * (imageSize.cy * INSP_RET)];

	milTempBuff2->Get2D(pData2, CRect(0, 0, imageSize.cx * INSP_RET, imageSize.cy * INSP_RET));
	pData = DBG_NEW BYTE[imageSize.cx * INSP_RET * imageSize.cy * INSP_RET];
	pData3 = DBG_NEW BYTE[imageSize.cx * imageSize.cy];

	CString strText;

	memset(pData2, 65535, sizeof(WORD) * (imageSize.cx * INSP_RET * 2));

	if (m_dlgRecipe->GetInspectionPara().isScale)
	{
		//strText.Format(_T("Scale Image Start"));
		//MainLogWrite(strText);
		ScaleImage((void*)pData2, (void*)pData4, CSize(imageSize.cx * INSP_RET, imageSize.cy * INSP_RET), m_dlgRecipe->GetInspectionPara().nScaleMin, m_dlgRecipe->GetInspectionPara().nScaleMax, 16);
		//strText.Format(_T("Scale Image End"));
		//MainLogWrite(strText);
	}
	else
	{
		//memcpy(pData4, pData2, sizeof(WORD) * (nPitch * m_ImageSize.cy));
		CMilBuffer* mil16TempBuff;
		mil16TempBuff = CRepository::Instance()->m_MilLib.NewImage(16);
		mil16TempBuff->Alloc2D(CSize(imageSize.cx, imageSize.cy));

		long lBrightMin, lBrightMax;
		CMilIm::GetLimitedMinMax(m_Mil16bitBuff, 3000, &lBrightMin, &lBrightMax);
		m_Mil16bitBuff->WndLvlTranform(mil16TempBuff, m_dlgRecipe->GetInspectionPara().nScaleMin, m_dlgRecipe->GetInspectionPara().nScaleMax, 0, 65535);

		m_Mil16bitBuff->Copy(mil16TempBuff);
		mil16TempBuff->Close();
		memcpy(pData4, m_Mil16bitBuff->GetAddr(), sizeof(WORD) * nPitch * imageSize.cy);
	}


	//strText.Format(_T("16bit -> 8bit Image Conv Start"));
	//MainLogWrite(strText);
	for (int y = 0; y < imageSize.cy * INSP_RET; y++)
	{
		for (int x = 0; x < imageSize.cx * INSP_RET; x++)
		{
			if (m_AtDetector->GetImageType() == 0 || m_AtDetector->GetImageType() == 2)
			{
				double dVal = (pData4[(y * (imageSize.cx * INSP_RET)) + x] / 256.);
				BYTE byVal = 0;
				if (dVal > 255)
					dVal = 255;

				byVal = (BYTE)dVal;
				pData[(y * (imageSize.cx * INSP_RET)) + x] = byVal;
			}
			else
				pData[(y * (imageSize.cx * INSP_RET)) + x] = (BYTE)pData4[(y * (imageSize.cx * INSP_RET)) + x];
		}
	}
	//strText.Format(_T("16bit -> 8bit Image Conv End"));
	//MainLogWrite(strText);

	milTempBuff3->Put2D(CSize(0, 0), CSize(imageSize.cx * INSP_RET, imageSize.cy * INSP_RET), pData);
	m_Mil8bitBuff->Put2D(CSize(0, 0), CSize(imageSize.cx * INSP_RET, imageSize.cy * INSP_RET), pData);


	Image16bitTo8bit(pData2, pData, m_ImageSize.cy, m_ImageSize.cx);
	m_MilDispBuff->Put(pData);
	
	milTempBuff->Close();
	milTempBuff2->Close();
	milTempBuff3->Close();
	milTempBuff4->Close();

	delete[] pData3;
	delete[] pData4;
	delete[] pData2;
	delete[] pData;

	m_bGrabStart = FALSE;
	m_bGrabEnd = TRUE;
	m_nTestIndex++;

	return TRUE;
}
#endif

BOOL CInspectionDlg::GetHardDiskFreeCheck()
{
	BOOL rslt = TRUE;
	CString strFile;
	wchar_t strTemp[MAX_CHAR_LENG];
	::GetModuleFileName(NULL, strTemp, 256);
	strFile.Format(_T("%s"), strTemp);
	double dUseSpace = 100. - UseDriveFreeSize(strFile.Mid(0, 1));

	if (dUseSpace >= m_dlgSystem->GetSystemPara().nHDDSpaceAlarmPer)
		rslt = FALSE;

	return rslt;
}

void CInspectionDlg::SetInspectionStart()
{
	long lStart = (long)GetTickCount64();
	SetInspStartTime(lStart);

	m_InspStartTime = CTime::GetCurrentTime();

	m_bInspectionStart = TRUE;
	m_bInspectionEnd = FALSE; 
	m_MilDisplay->ClearOverlay();

	if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
	{
		for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
		{
#ifdef MIL_USE
			m_InspectionClass[i]->StartInspectionThread(m_MilDispBuff);
#endif
		}
	}
	else if (m_dlgSystem->GetSystemPara().nInspectionType == 1)
	{
		int nInspTypeCnt = m_dlgRecipe->GetInspTypCnt();

		for (int i = 0; i < nInspTypeCnt; i++)
		{
			m_InspectionClass[i]->SetInspBlobsPara(m_dlgRecipe->GetInspectionTypePara(i));
			m_InspectionClass[i]->SetInspIndex(i);
			m_InspectionClass[i]->SetStartBlobsThread(m_Mil8bitBuff);
			Sleep(5);
		}
	}

	m_bInspectionStart = FALSE;

}

void CInspectionDlg::ShowTextBox(CString strText, BOOL isShow, BOOL isStop)
{
	if (isShow)
	{
		CRect win_rect, text_rect, move_rect;
		this->GetWindowRect(&win_rect);
		m_dlgTextBox->GetClientRect(&text_rect);

		CPoint cen_point;
		cen_point = win_rect.CenterPoint();

		move_rect.SetRect(cen_point.x - text_rect.Width() / 2, cen_point.y - text_rect.Height() / 2,
			cen_point.x + text_rect.Width() / 2, cen_point.y + text_rect.Height() / 2);
		m_dlgTextBox->MoveWindow(move_rect);

		m_dlgTextBox->SetTimerShowHide(FALSE);
		m_dlgTextBox->SetTimerShowWindow(strText);
	}
	else
	{
		if(isStop)
			g_SeqModule.Stop();

		m_dlgTextBox->SetTimerShowHide(TRUE);
	}
}

// HBITMAP 핸들을 파일로 저장하는 함수
BOOL CInspectionDlg::SaveHBITMAPToFile(HBITMAP hBitmap, LPCSTR lpszFileName)
{
	// output stream 선언  
	ofstream stream;
	// stream open (바이너리 모드)  
	stream.open(lpszFileName, ios::binary);
	// stream이 열리지 않으면 에러.  
	if (!stream.is_open())  
	{
		//cout << "File open error!!" << endl;
		return FALSE;  
	}
	// 저장할 bitmap 선언  
	BITMAP bitmap;
	memset(&bitmap, NULL, sizeof(BITMAP));
	// hBitmap으로 bitmap을 가져온다.  
	GetObject(hBitmap, sizeof(bitmap), (LPSTR)&bitmap);
	// Bitmap Header 정보 설정  
	BITMAPINFOHEADER bi;
	memset(&bi, NULL, sizeof(BITMAPINFOHEADER));
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bitmap.bmWidth;  bi.biHeight = bitmap.bmHeight;  bi.biPlanes = 1;  bi.biBitCount = BIT_COUNT;
	bi.biCompression = BI_RGB;  bi.biSizeImage = 0;  bi.biXPelsPerMeter = 0;  bi.biYPelsPerMeter = 0;  bi.biClrUsed = 0;  bi.biClrImportant = 0;
	// 컬러 사이즈  
	int PalSize = (bi.biBitCount == 24 ? 0 : 1 << bi.biBitCount) * sizeof(RGBQUAD);
	int Size = bi.biSize + PalSize + bi.biSizeImage;
	BITMAPFILEHEADER fh;
	memset(&fh, NULL, sizeof(BITMAPFILEHEADER));
	fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + PalSize;  fh.bfReserved1 = 0;
	fh.bfReserved2 = 0;  fh.bfSize = Size + sizeof(BITMAPFILEHEADER);  fh.bfType = 0x4d42;   
	// 파일에 bitmap 해더 작성  
	stream.write((LPSTR)&fh, sizeof(BITMAPFILEHEADER));
	// DC 취득  
	HDC hDC = ::GetDC(NULL);
	// 메모리 할당 (bitmap header)
	BITMAPINFO* header = (BITMAPINFO*)malloc(bi.biSize + PalSize);
	header->bmiHeader = bi;
	// hBitmap으로 부터 해더를 가져온다. 
	GetDIBits(hDC, hBitmap, 0, bitmap.bmHeight, NULL, header, DIB_RGB_COLORS);
	// 이미지 전체 사이즈를 취득한다. 
	bi = header->bmiHeader; 
	if (bi.biSizeImage == 0)
	{ 
		// 해더 사이즈 설정이 안되면 강제 계산 설정
		bi.biSizeImage = ((bitmap.bmWidth * bi.biBitCount + 31) & ~ 31) / 8 * bitmap.bmHeight;
	}
	// 이미지 영역 메모리 할당  
	Size = bi.biSize + PalSize + bi.biSizeImage;
	void* body = malloc(header->bmiHeader.biSizeImage);  
	// hBitmap의 데이터를 저장  
	GetDIBits(hDC, hBitmap, 0, header->bmiHeader.biHeight, body, header, DIB_RGB_COLORS);
	// 데이터 작성 
	stream.write((LPSTR)&header->bmiHeader, sizeof(BITMAPINFOHEADER));  stream.write((LPSTR)body, Size);
	// DC 해제  
	::ReleaseDC(NULL, hDC);  
	// stream 해제  
	stream.close();  
	// 메모리 해제 
	free(header);
	free(body);

	return TRUE;
}

// 스크린 샷
void CInspectionDlg::screenshot(LPCSTR filepath)
{
	// 스크린 크기를 저장하기 위한 변수  
	RECT desktop;
	// 데스크 탑의 핸들  
	const HWND hDesktop = ::GetDesktopWindow(); 
	// 핸들로 부터 크기를 받는다.  
	::GetWindowRect(hDesktop, &desktop); 

	//GetDlgItem(IDC_STATIC_DISPLAY)->GetWindowRect(&desktop);
	// 너비  
	int width = desktop.right;
	// 높이  
	int height = desktop.bottom;   
	// 스크린 핸들러 취득  
	HDC hScreen = ::GetDC(NULL); 
	// 핸들러 생성  
	HDC hDC = CreateCompatibleDC(hScreen);
	// 핸들러에 bitmap 핸들러 생성  
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, width, height);
	// 핸들러 -> bitmap 핸들러 선택  
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap); 
	// hDC는 출력될 핸들러.  // x, y, width, height -> 이건 위 hDC에 표시될 위치와 크기입니다. 
	// 스크린 핸들러의 좌표 x, y위치부터  // SRCCOPY - 원본 복사  
	BOOL bRet = BitBlt(hDC, desktop.left, desktop.top, width, height, hScreen, 0, 0, SRCCOPY);
	// bitmap 데이터를 저장한다.  
	SaveHBITMAPToFile(hBitmap, filepath);  
	//핸들러 -> 리소스 돌리기  
	SelectObject(hDC, old_obj);  
	// 핸들러 삭제 (리소스 해제)  
	DeleteDC(hDC);  
	// 리소스 해제  
	::ReleaseDC(NULL, hScreen);  
	// bitmap 리소스 해제  
	DeleteObject(hBitmap);
}

BOOL CInspectionDlg::WindowCapture(HWND hTargetWnd, LPCSTR strFileName)
{
	CRect rct;
	if (hTargetWnd)
		//::GetClientRect(hTargetWnd, &rct);
		::GetWindowRect(hTargetWnd, &rct);
	else
		return FALSE;

	CRect rcClient;
	GetClientRect(&rcClient);

	HBITMAP hBitmap = NULL;
	HBITMAP hOldBitmap = NULL;
	BOOL bSuccess = FALSE;

	HDC hDC = ::GetDC(hTargetWnd);
	HDC hMemDC = ::CreateCompatibleDC(hDC);
	hBitmap = ::CreateCompatibleBitmap(hDC, rct.Width(), rct.Height());

	if (!hBitmap)
		return FALSE;

	::SelectObject(hMemDC, hBitmap);

	if (::PrintWindow(hTargetWnd, hMemDC, 2))
		bSuccess = FALSE;
	else
	{
		StretchBlt(hMemDC, -7, 0, rct.right - rct.left + 7, rct.bottom - rct.top, hMemDC, 0, 0, rct.right - rct.left - 7, rct.bottom - rct.top - 7, SRCCOPY);
		bSuccess = TRUE;
	}

	hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);

	// 캡쳐한 윈도우 현재 클라이언트 화면 크기에 맞춰 그리기
	CClientDC dc(this);
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(0, 0, rcClient.Width(), rcClient.Height(), CDC::FromHandle(hDC),
		0, 0, rct.Width(), rct.Height(), SRCCOPY);
	::SelectObject(hMemDC, hOldBitmap);

	SaveHBITMAPToFile(hOldBitmap, strFileName);

	DeleteObject(hOldBitmap);
	DeleteObject(hBitmap);
	::DeleteDC(hMemDC);
	::ReleaseDC(hTargetWnd, hDC);

	return bSuccess;
}

LRESULT CInspectionDlg::OnInspectionEndMessage(WPARAM para0, LPARAM para1)
{
	m_bResultDisplayEnd = FALSE;
	BOOL isRslt = TRUE;
	if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
	{
		for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				m_list_InspResultData.SetItemText(i, j + 1, _T("0"));
			}
		}

		for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
		{
			if (m_dlgRecipe->GetInspectionPara(i).bInspUse)
			{
				//m_DlgRect->SetActiveImageRect(TRUE);
#ifdef MIL_USE
				m_InspectionClass[i]->SetResultDisplay(FALSE);
				//m_DlgRect->DeleteAll();

				m_InspectionClass[i]->SetResultDisplay(m_MilDisplay, NULL, &m_list_InspResultData);
#endif
				//m_DlgRect->SetActiveImageRect(FALSE);
			}
		}
		//m_DlgRect->Invalidate();
		BOOL isRslt = TRUE;
		int nNGIndex = -1;
		for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
		{
			if (m_dlgRecipe->GetInspectionPara(i).bInspUse)
			{
#ifdef MIL_USE
				isRslt = m_InspectionClass[i]->GetAllResult();

				if (!isRslt)
				{
					if (!m_InspectionClass[i]->GetCountResult())
						nNGIndex = 0;

					if (m_InspectionClass[i]->GetOverlapResult())
						nNGIndex = 1;

					SelectDisplay(i);

					break;
				}
#endif
			}
			//m_InspectionClass[i]->SetReleaseEachThread();
		}

		if (nNGIndex == -1)
		{
			for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
			{
				if (m_dlgRecipe->GetInspectionPara(i).bInspUse)
				{
					SelectDisplay(i);
					break;
				}
			}
		}

		m_bInspResult = isRslt;
		m_nInspNgIndex = nNGIndex;
	}
	else if (m_dlgSystem->GetSystemPara().nInspectionType == 1)
	{
		int nInspTypeCnt = m_dlgRecipe->GetInspTypCnt();

		vector<BLOB_RESULT_PARA> all_result;
		for (int i = 0; i < nInspTypeCnt; i++)
		{
			for (int j = 0; j < m_InspectionClass[i]->GetBlobsResultCount(); j++)
			{
				BLOB_RESULT_PARA each_result;
				each_result = m_InspectionClass[i]->GetBlobsResultData(j);

				if (all_result.size() == 0)
					all_result.push_back(each_result);
				else
				{
					CRect result_rect, result_rect2;
					result_rect.SetRect((int)(each_result.dCenX - (each_result.dLongSize / 2.) - 5),
						(int)(each_result.dCenY - (each_result.dLongSize / 2.) - 5),
						(int)(each_result.dCenX + (each_result.dLongSize / 2.) + 5),
						(int)(each_result.dCenY + (each_result.dLongSize / 2.) + 5));

					BOOL bInter = FALSE;
					for (int k = 0; k < all_result.size(); k++)
					{
						result_rect2.SetRect((int)(all_result[k].dCenX - (all_result[k].dLongSize / 2.) - 5),
							(int)(all_result[k].dCenY - (all_result[k].dLongSize / 2.) - 5),
							(int)(all_result[k].dCenX + (all_result[k].dLongSize / 2.) + 5),
							(int)(all_result[k].dCenY + (all_result[k].dLongSize / 2.) + 5));

						CRect result_rect3;
						result_rect3.IntersectRect(result_rect, result_rect2);

						if (result_rect3.Width() > 0 && result_rect3.Height() > 0)
						{
							bInter = TRUE;
							break;
						}
					}

					if (!bInter)
						all_result.push_back(each_result);
				}
			}
		}

		m_MilDisplay->ClearOverlay();
		m_MilDisplay->EnableGraph(FALSE);
		m_MilDisplay->EnableOverlay(FALSE);
		m_MilDisplay->EnableGraph();
		m_MilDisplay->EnableOverlay();
		m_MilDisplay->FontSize(M_FONT_DEFAULT_LARGE);
		m_InspectionClass[0]->SetBlobsResultDisplay(m_MilDisplay, all_result, &m_list_InspResultData);

		if (all_result.size() > 0)
			isRslt = FALSE;
		SetShowResultNG(isRslt);

		m_bRejecterStart = FALSE;
		if (!isRslt && m_dlgSystem->GetSystemPara().stRejecter.bUse)
		{
			//m_dwRejecterStartTime.push_back(_TimerCounterMain);
			m_bRejecterStart = TRUE;
		}
		else
		{
			m_bRejecterQuePop = TRUE;
		}

	}
	WriteRsltData();

	m_bSelectResultDisplay = FALSE;

	m_bResultDisplayEnd = TRUE;

	return 0;
}

void CInspectionDlg::WriteRsltData()
{
	CStdioFile pFile;
	CString strFileFullPath, strFilePath, strFileName;
	CString strText;
	BOOL bOpen, bNewFile = FALSE;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	strFilePath.Format(_T("%s\\ResultData\\%04d\\%02d\\%02d\\%s\\"), strTemp, m_InspStartTime.GetYear(), m_InspStartTime.GetMonth(), m_InspStartTime.GetDay(), m_dlgRecipe->GetCurrentRecipeInfo());
	MakeDirectories(strFilePath);

	

	if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
	{
		strFileName.Format(_T("%04d%02d%02d_%s"), m_InspStartTime.GetYear(), m_InspStartTime.GetMonth(), m_InspStartTime.GetDay(), RESULT_DATA__CSV_FILENAME);
		strFileFullPath.Format(_T("%s%s"), strFilePath, strFileName);

		if (FileSearch(strFilePath, strFileName))
		{
			bOpen = pFile.Open(strFileFullPath, CFile::modeNoInherit | CFile::modeWrite);
		}
		else
		{
			bOpen = pFile.Open(strFileFullPath, CFile::modeCreate | CFile::modeWrite);
			bNewFile = TRUE;
		}

		if (bNewFile)
		{
			WriteCsvLog(&pFile, RESULT_CSV_FILE_LABEL_NAME);
		}

		m_InspStartTime;

		CString strTime, strTotalRslt, strEachRslt;
		strTime.Format(_T("%04d%02d%02d-%02d%02d%02d,"), m_InspStartTime.GetYear(), m_InspStartTime.GetMonth(), m_InspStartTime.GetDay(), m_InspStartTime.GetHour(), m_InspStartTime.GetMinute(), m_InspStartTime.GetSecond());

		strText.Append(strTime);
	
		strTotalRslt.Format(_T("OK,"));
		for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
		{
			if (!m_InspectionClass[i]->GetAllResult())
			{
				strTotalRslt.Format(_T("NG,"));
				break;
			}
		}
		strText.Append(strTotalRslt);

		for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
		{
			if (m_InspectionClass[i]->GetAllResult())
				strEachRslt.Format(_T("OK,"));
			else
			{
				if (!m_InspectionClass[i]->GetCountResult())
					strEachRslt.Format(_T("Count_NG,"));

				if (m_InspectionClass[i]->GetOverlapResult())
					strEachRslt.Format(_T("Overlap_NG,"));
			}

			strText.Append(strEachRslt);

			if (m_dlgRecipe->GetInspectionPara(i).bInspUse)
				strText.AppendFormat(_T("%d,%d,%d"), m_dlgRecipe->GetInspectionPara(i).nSearchPatternCnt, m_InspectionClass[i]->GetSearchPatternCnt(), m_InspectionClass[i]->GetOverlapPatternCnt());
			else
				strText.Append(_T("0,0,0"));

			if (i != (MAX_PATTERN_ADD_CNT - 1))
			{
				strText.Append(_T(","));
			}
		}

		WriteCsvLog(&pFile, strText);
	}
	else if (m_dlgSystem->GetSystemPara().nInspectionType == 1)
	{
		m_bInspResult = FALSE;
		if (m_InspectionClass[0]->GetBlobsResultCount() == 0)
		{
			m_bInspResult = TRUE;
			return;
		}
#ifdef _DEBUG
		strFileName.Format(_T("%04d%02d%02d_%02d%02d%02d_%03d_%s"), m_InspStartTime.GetYear(), m_InspStartTime.GetMonth(), m_InspStartTime.GetDay(), m_InspStartTime.GetHour(), m_InspStartTime.GetMinute(), m_InspStartTime.GetSecond(), m_nTestIndex - 1, RESULT_DATA__CSV_FILENAME);
#else
		strFileName.Format(_T("%04d%02d%02d_%02d%02d%02d_%s"), m_InspStartTime.GetYear(), m_InspStartTime.GetMonth(), m_InspStartTime.GetDay(), m_InspStartTime.GetHour(), m_InspStartTime.GetMinute(), m_InspStartTime.GetSecond(), RESULT_DATA__CSV_FILENAME);
#endif
		strFileFullPath.Format(_T("%s%s"), strFilePath, strFileName);
		bOpen = pFile.Open(strFileFullPath, CFile::modeCreate | CFile::modeWrite);

		WriteCsvLog(&pFile, BLOBS_RESULT_CSV_FILE_LABEL_NAME);
		//WriteCsvLog(&pFile, strText);
		double dPx, dPy;
		m_dlgSystem->GetPixelSize(&dPx, &dPy);
		for (int i = 0; i < m_InspectionClass[0]->GetBlobsResultCount(); i++)
		{
			BLOB_RESULT_PARA rslt_data;
			rslt_data = m_InspectionClass[0]->GetBlobsResultData(i);
			strText.Format(_T("%d,"), i + 1);
			strText.AppendFormat(_T("%0.3f,"), rslt_data.dCenX);
			strText.AppendFormat(_T("%0.3f,"), rslt_data.dCenY);
			strText.AppendFormat(_T("%0.3f,"), rslt_data.dAngle);
			strText.AppendFormat(_T("%0.3f,"), rslt_data.dShortSize);
			strText.AppendFormat(_T("%0.3f,"), rslt_data.dLongSize);
			strText.AppendFormat(_T("%0.3f,"), rslt_data.dLength);
			strText.AppendFormat(_T("%0.3f,"), rslt_data.dShortSize * dPx);
			strText.AppendFormat(_T("%0.3f,"), rslt_data.dLongSize * dPx);
			strText.AppendFormat(_T("%0.3f"), rslt_data.dLength * dPx);
			WriteCsvLog(&pFile, strText);
		}
		
	}
	else
	{

	}
}

BOOL CInspectionDlg::WriteCsvLog(CStdioFile* pFile, CString strLog)
{
	BOOL rslt = TRUE;
	CString tempStr;

	tempStr.Format(_T("%s\n"), strLog);
	pFile->Seek(0L, CFile::end);
	pFile->WriteString(tempStr);
	
	return rslt;
}

void CInspectionDlg::SetInspImageSave()
{
	CString strFilePath, strFileName;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	if (m_bInspResult)
		strFilePath.Format(_T("%s\\Orig\\%04d\\%02d\\%02d\\OK\\%s\\"), strTemp, m_InspStartTime.GetYear(), m_InspStartTime.GetMonth(), m_InspStartTime.GetDay(), m_dlgRecipe->GetCurrentRecipeInfo());
	else
		strFilePath.Format(_T("%s\\Orig\\%04d\\%02d\\%02d\\NG\\%s\\"), strTemp, m_InspStartTime.GetYear(), m_InspStartTime.GetMonth(), m_InspStartTime.GetDay(), m_dlgRecipe->GetCurrentRecipeInfo());

	if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
	{
#ifndef HW_USE
		strFileName.Format(_T("%02d%02d%02d_Result_%d.bmp"), m_InspStartTime.GetHour(), m_InspStartTime.GetMinute(), m_InspStartTime.GetSecond(), m_nTestIndex - 1);
#else
		strFileName.Format(_T("%02d%02d%02d_Result.bmp"), m_InspStartTime.GetHour(), m_InspStartTime.GetMinute(), m_InspStartTime.GetSecond());
#endif
	}
	else if (m_dlgSystem->GetSystemPara().nInspectionType == 1)
	{
#ifdef _DEBUG
		strFileName.Format(_T("%04d%02d%02d_%02d%02d%02d_%03d.tiff"), m_InspStartTime.GetYear(), m_InspStartTime.GetMonth(), m_InspStartTime.GetDay(), m_InspStartTime.GetHour(), m_InspStartTime.GetMinute(), m_InspStartTime.GetSecond(), m_nTestIndex - 1);
#else
		strFileName.Format(_T("%04d%02d%02d_%02d%02d%02d.tiff"), m_InspStartTime.GetYear(), m_InspStartTime.GetMonth(), m_InspStartTime.GetDay(), m_InspStartTime.GetHour(), m_InspStartTime.GetMinute(), m_InspStartTime.GetSecond());
#endif
	}

	if (m_dlgSystem->GetSystemPara().bSaveInspectionRawImage)
	{
		CreateDir(strFilePath);
#ifdef MIL_USE
		m_Mil16bitBuff->Export(strFilePath + strFileName, M_TIFF);
#endif
	}

	SetInspScreenShotSave();
}

void CInspectionDlg::SetInspScreenShotSave()
{
	if (m_bInspResult)
		CountOKAdd();
	else
		CountNGAdd(m_nInspNgIndex);

	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	CString strFilePath, strFileName;
	if (m_bInspResult)
		strFilePath.Format(_T("%s\\Result\\%04d\\%02d\\%02d\\OK\\%s\\"), strTemp, m_InspStartTime.GetYear(), m_InspStartTime.GetMonth(), m_InspStartTime.GetDay(), m_dlgRecipe->GetCurrentRecipeInfo());
	else
		strFilePath.Format(_T("%s\\Result\\%04d\\%02d\\%02d\\NG\\%s\\"), strTemp, m_InspStartTime.GetYear(), m_InspStartTime.GetMonth(), m_InspStartTime.GetDay(), m_dlgRecipe->GetCurrentRecipeInfo());
#ifndef HW_USE
	strFileName.Format(_T("%02d%02d%02d_Result_%d.bmp"), m_InspStartTime.GetHour(), m_InspStartTime.GetMinute(), m_InspStartTime.GetSecond(), m_nTestIndex - 1);
#else
	strFileName.Format(_T("%02d%02d%02d_Result.bmp"), m_InspStartTime.GetHour(), m_InspStartTime.GetMinute(), m_InspStartTime.GetSecond());
#endif

	BOOL isSave = FALSE;
	if (m_dlgSystem->GetSystemPara().bSaveResultOKImage && m_bInspResult)
		isSave = TRUE;
	
	if (m_dlgSystem->GetSystemPara().bSaveResultNGImage && !m_bInspResult)
		isSave = TRUE;

	if (isSave)
	{
		CreateDir(strFilePath);
		strFilePath += strFileName;
		char strTemp[512];
		sprintf_s(strTemp, size_t(strFilePath.GetLength() + 1), "%S", strFilePath.GetBuffer());
		//Sleep(100);
		//Delay(100);
		//screenshot(strTemp);

		CString strAppName = _T("Inspection");
		CWnd* pWnd = FindWindow(NULL, strAppName);
		//if (pWnd)
		{
			//WindowCapture(GetDlgItem(IDC_STATIC_DISPLAY)->GetSafeHwnd(), strTemp);
			WindowCapture(this->GetSafeHwnd(), strTemp);
		}
	}

	m_bResultDisplayEnd = TRUE;
}

LRESULT CInspectionDlg::OnDetectorToMainMessage(WPARAM para0, LPARAM para1)
{
#
	if ((int)para0 == 1)//Alarm
	{
		if (m_AtDetector == NULL)
			return 0;

		CString strEvent, strText;
		strEvent.Format(_T("%s"), m_AtDetector->GetEventString());
		
		//cal
		//m_pMainLog->WriteText(strEvent, TRUE);
		SYSTEMTIME	lpSystemTime;
		GetLocalTime(&lpSystemTime);

		strText.Format(_T("%02d:%02d:%02d:%03d - %s"), lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds, strEvent);
		//SendMessage(USE_MSG_LOG_TO_MAIN, (WPARAM)strText.GetBuffer(), strText.GetLength());
		m_strMainLog.push_back(strText);
		if (m_AtDetector->GetCalStart() && !m_AtDetector->GetCalEnd())// && !strEvent.Mid(7, 1).Compare(_T("0")))
		{
			m_dlgTextBox->SetTextBoxString(strEvent);
			m_dlgTextBox->ShowWindow(SW_SHOW);
			
			if (strEvent.Find(_T("Save Succsed"), 0) > 0)
			{
				SetTimer(TIMER_DETECTOR_CAL_END_CHECK, 100, NULL);
			}
			else if (strEvent.Find(_T("Calibration Data Load Succsed"), 0) > 0)
			{
				m_dlgTextBox->ShowWindow(SW_HIDE);
			}
		}

		return 0;
	}


	if (m_dlgRecipe->IsWindowVisible())//Manual
	{
#ifdef HW_USE
		if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
			::SendMessage(m_dlgRecipe->GetTeachingDlgHwnd(), USE_MSG_IMAGE_VIEW_TO_TEACHING_DIALOG, 1, (LPARAM)m_AtDetector->GetImageBuff());
		else
			::SendMessage(m_dlgRecipe->GetTeachingDlgHwnd(), USE_MSG_IMAGE_VIEW_TO_TEACHING_BLOBS_DIALOG, 1, (LPARAM)m_AtDetector->GetImageBuff());
#else
		CString strText;
		strText.Format(_T("D:\\ngh\\TestImage\\Test_16bit.tif"));
		MIL_ID milTestBuf;
		MbufAlloc2d(CMilSystem::Instance()->GetSystemID(), m_dlgSystem->GetSystemPara().ImageSize.cx, m_dlgSystem->GetSystemPara().ImageSize.cy, 16 + M_UNSIGNED, M_IMAGE + M_PROC, &milTestBuf);
		//MbufImport(strText, M_TIFF, M_LOAD, CMilSystem::Instance()->GetSystemID(), &milTestBuf);
		WORD *wData;
		MbufInquire(milTestBuf, M_HOST_ADDRESS, &wData);
		if (m_dlgSystem->GetSystemPara().nInspectionType == 0)
			::SendMessage(m_dlgRecipe->GetTeachingDlgHwnd(), USE_MSG_IMAGE_VIEW_TO_TEACHING_DIALOG, 1, (LPARAM)wData);
		else
			::SendMessage(m_dlgRecipe->GetTeachingDlgHwnd(), USE_MSG_IMAGE_VIEW_TO_TEACHING_BLOBS_DIALOG, 1, (LPARAM)wData);
		MbufFree(milTestBuf);
#endif
	}
	else//Auto
	{
		if (m_bXRayOnOff && m_bBeforeXRayOnOff)
			return 0;

		if (!m_bAutoStatus)
			return 0;

		if (m_bInterLockStatus)
			return 0;

		/*if (m_bFirstGrab)
		{
			m_bFirstGrab = FALSE;
			return 0;
		}*/
		m_dwRejecterStartTime.push_back(_TimerCounterMain);
		m_bRejecterQuePop = FALSE;
		long lStart = (long)GetTickCount64();
		SetTotalStartTime(lStart);
#ifdef HW_USE
		SetMainDisplayImage();
		//CAtDetector::Instance()->GrabStop();
#endif
	}


	return 0;
}

LRESULT CInspectionDlg::OnAlarmSequenceMessage(WPARAM para0, LPARAM para1)
{
	int i, j;
	CString temp, temp2;
	
	switch (para0)
	{
		//Alarm
	case 0:
		for (i = 0; i < MOT_PROCESS_MAX_NUM; i++)
		{
			for (j = 0; j < MOT_STATUS_MAX_NUM; j++)
			{
				if (g_SeqModule.m_MDat[i].m_Seq[j].m_bAlarm)
				{
					m_bAutoAlarm = TRUE;
					temp.Format(_T("%S"), g_SeqModule.m_MDat[i].m_ProSts[j][g_SeqModule.m_MDat[i].m_Seq[j].m_iSubSeqIdx].m_cNGStatus);
					temp2.Format(_T("%S"), g_SeqModule.m_MDat[i].m_ProSts[j][g_SeqModule.m_MDat[i].m_Seq[j].m_iSubSeqIdx].m_cNGCode);

					if (m_dlgTextBox->IsWindowVisible())
						m_dlgTextBox->SetTimerShowHide(TRUE);
					
					if (!m_dlgAlarm->IsWindowVisible())
					{
						m_bInterLockStatus = TRUE;
						m_dlgAlarm->SetAlarmString(_wtoi(temp2), temp);
#ifdef INVERTER_COM
						m_Inverter->SetInverterMove();
#else
						//inverter stop
						CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_MOTOR_CCW, FALSE);
#endif
						if (!temp2.Compare(_T("1")))
						{
#ifndef CE_CONFIRM
							CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_MC01_ON, FALSE);
#endif
							CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);
						}

						AutoEnableBtn(FALSE);
						
#ifndef INVERTER_COM
						SetConMove();
#else
						
						//m_Inverter->SetInverterAlarmReset();
#endif

						if (!temp2.Compare(_T("1")))
						{
							SetMC01On();
							CInverterClass::Instance()->Close();
							SetTimer(TIMER_INVERTER_RECONNECT, 100, NULL);
						}
						m_bInterLockStatus = TRUE;
						m_AtDetector->GrabStop();
						SetTowerLamp(0);
						m_bBuzzerOff = FALSE;
						m_bAlarmBuzzer = TRUE;
						SetTimer(TIMER_ALARM_BUZZER, 1000, NULL);
						SetTimer(TIMER_ALARM_SHOW, 100, NULL);

						if (!temp.Compare(ALARM_MSG_IN_SENSER_INTER_LOCK) ||
							!temp.Compare(ALARM_MSG_OUT_SENSER_INTER_LOCK))
						{
							m_bInSenser = FALSE;
							m_bOutSenser = FALSE;
							if(!temp.Compare(ALARM_MSG_IN_SENSER_INTER_LOCK))
								m_bInSenser = TRUE;

							if(!temp.Compare(ALARM_MSG_OUT_SENSER_INTER_LOCK))
								m_bOutSenser = TRUE;

							KillTimer(TIMER_IN_OUT_SENSER_UNLOCK);
							SetTimer(TIMER_IN_OUT_SENSER_UNLOCK, 100, NULL);

							if (!temp.Compare(ALARM_MSG_IN_SENSER_INTER_LOCK))
							{
								m_nInSenserTime = 0;
								SetTimer(TIMER_IN_SENSER_X_RAY_OFF, 100, NULL);
							}

							if (!temp.Compare(ALARM_MSG_OUT_SENSER_INTER_LOCK))
							{
								m_nOutSenserTime = 0;
								SetTimer(TIMER_OUT_SENSER_X_RAY_OFF, 100, NULL);
							}
							
						}
						else
						{
							SetXrayOnOffLabel(FALSE);
							SetXRayOn();
						}
					}

					break;
				}
			}
		}
		break;
	}

	return 0;
}

LRESULT CInspectionDlg::OnImageViewToMainMessage(WPARAM para0, LPARAM para1)
{

	int nEvent = (int)para0;
	int nTrackerIdx = (int)para1;

	switch (nEvent)
	{
	case 999:
	{
		
	}
		break;
	}

	return 0;
}
/**
  @brief SetMainDisplayImage
  Detector에서 얻은 영상을 화면에 표시
*/
void CInspectionDlg::SetMainDisplayImage()
{
#ifdef MIL_USE
	CMilBuffer* milTempBuff;// , * milTempBuff2;
	milTempBuff = CRepository::Instance()->m_MilLib.NewImage();
	milTempBuff->Alloc2D(CSize(m_ImageSize.cy, m_ImageSize.cx));
	//milTempBuff2 = CRepository::Instance()->m_MilLib.NewImage(16);
	//milTempBuff2->Alloc2D(CSize(m_ImageSize.cy, m_ImageSize.cx));
	long nPitch;
#ifdef HW_USE
	nPitch = m_ImageSize.cy;
#else
	MbufInquire(milTempBuff->GetIDBuffer(), M_PITCH, &nPitch);
#endif
	BYTE* pData;
	WORD* pData2, *pData4;
	pData2 = DBG_NEW WORD[nPitch * m_ImageSize.cx];
	pData4 = DBG_NEW WORD[nPitch * m_ImageSize.cx];
	memcpy(pData2, m_AtDetector->GetImageBuff(), sizeof(WORD) * nPitch * m_ImageSize.cx);
	pData = DBG_NEW BYTE[m_ImageSize.cy * m_ImageSize.cx];
	m_Mil16bitBuff2->Put2D(CSize(0, 0), CSize(m_ImageSize.cy, m_ImageSize.cx), pData2);

	MimRotate(m_Mil16bitBuff2->GetIDBuffer(), m_Mil16bitBuff->GetIDBuffer(), 90, m_ImageSize.cy / 2, m_ImageSize.cx / 2, m_ImageSize.cx / 2, m_ImageSize.cy / 2, M_DEFAULT + M_OVERSCAN_DISABLE);
	//milTempBuff2->Close();
	CString strText;

	if (m_dlgRecipe->GetInspectionPara().isScale)
	{
		//strText.Format(_T("Scale Image Start"));
		//MainLogWrite(strText);
		ScaleImage((void*)pData2, (void*)pData4, CSize(m_ImageSize.cy, m_ImageSize.cx), m_dlgRecipe->GetInspectionPara().nScaleMin, m_dlgRecipe->GetInspectionPara().nScaleMax, 16);
		//strText.Format(_T("Scale Image End"));
		//MainLogWrite(strText);
	}
	else
		memcpy(pData4, pData2, sizeof(WORD) * (nPitch * m_ImageSize.cx));


	//strText.Format(_T("16bit -> 8bit Image Conv Start"));
	//MainLogWrite(strText);
	for (int y = 0; y < m_ImageSize.cx; y++)
	{
		for (int x = 0; x < m_ImageSize.cy; x++)
		{
			if (m_AtDetector->GetImageType() == 0 || m_AtDetector->GetImageType() == 2)
			{
				double dVal = (pData4[(y * m_ImageSize.cy) + x] / 256.);
				BYTE byVal = 0;
				if (dVal > 255)
					dVal = 255;

				byVal = (BYTE)dVal;
				pData[(y * m_ImageSize.cy) + x] = byVal;
			}
			else
				pData[(y * m_ImageSize.cy) + x] = (BYTE)pData4[(y * m_ImageSize.cy) + x];
		}
	}
	//strText.Format(_T("16bit -> 8bit Image Conv End"));
	//MainLogWrite(strText);

	milTempBuff->Put(pData);
	MimRotate(milTempBuff->GetIDBuffer(), m_Mil8bitBuff->GetIDBuffer(), 90, m_ImageSize.cy / 2, m_ImageSize.cx / 2, m_ImageSize.cx / 2, m_ImageSize.cy / 2, M_DEFAULT + M_OVERSCAN_DISABLE);
	
	Image16bitTo8bit(pData2, pData, m_ImageSize.cy, m_ImageSize.cx);
	milTempBuff->Put(pData);
	MimRotate(milTempBuff->GetIDBuffer(), m_MilDispBuff->GetIDBuffer(), 90, m_ImageSize.cy / 2, m_ImageSize.cx / 2, m_ImageSize.cx / 2, m_ImageSize.cy / 2, M_DEFAULT + M_OVERSCAN_DISABLE);
	
	milTempBuff->Close();
	milTempBuff = NULL;
	delete[] pData4;
	delete[] pData2;
	delete[] pData;
	m_bGrabEnd = TRUE;
#endif
}

void CInspectionDlg::SetDispTackTime(double dTime, int nType)
{
	CString strText;
	strText.Format(_T("%0.3f sec"), dTime);

	if (nType == 0)
	{
		m_dInspTackTime = dTime;
		m_btn_InspTackTime.SetWindowText(strText);
	}
	else
	{
		m_dTotalTackTime = dTime;
		m_btn_TotalTackTime.SetWindowText(strText);
	}
}

void CInspectionDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);
	if (this->IsWindowVisible())
	{
		CRect rect;
		
		GetDlgItem(IDC_STATIC_DISPLAY)->GetWindowRect(&rect);
		if (rect.left == 0 && rect.top == 0 && rect.right == 0 && rect.bottom == 0)
			return;

		//if(m_DlgRect != NULL)
			//m_DlgRect->MoveWindow(rect);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CInspectionDlg::ClickBtnenhctrlMainPattern()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetFocus();
	CString strText;
	pBtnEnh->GetWindowText(strText);

	int nIdx = _wtoi(strText.Right(1)) - 1;

	m_bSelectResultDisplay = TRUE;
	SelectDisplay(nIdx);
}


void CInspectionDlg::ClickBtnenhctrlAuto()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnBnClickedButtonAuto();
}


void CInspectionDlg::ClickBtnenhctrlManual()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnBnClickedButtonManual();
}


void CInspectionDlg::ClickBtnenhctrlSystem()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnBnClickedButtonSystem();
}


void CInspectionDlg::ClickBtnenhctrlRecipe()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnBnClickedButtonRecipe();
}


void CInspectionDlg::ClickBtnenhctrlExit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bExit = TRUE;
	if (AfxMessageBox(_T("검사 프로그램을 종료 하시겠습니까?"), MB_YESNO) == IDYES)
	{
		SYSTEMTIME	lpSystemTime;
		GetLocalTime(&lpSystemTime);
		CString strText;

		strText.Format(_T("%02d:%02d:%02d:%03d - System End !!"), lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds);
		m_pMainLog->WriteText(strText);
		OnBnClickedButtonExit();
	}
	else
		m_bExit = FALSE;
}

BOOL CInspectionDlg::GetXRayInterLock()
{
#ifdef HW_USE
	return m_XRayClass->GetXrayInfo().stXray_status.bOpenInterlock;
#else
	return m_XRayClass->GetXrayInfo().stXray_status.bOpenInterlock;;
#endif
}

BOOL CInspectionDlg::GetDoorInterLock()
{
#ifdef HW_USE
#ifdef CE_CONFIRM
	BOOL bRslt[4] = { FALSE, };
	bRslt[0] = m_IOClass->GetInputBit(INPUT_INDEX_DOOR1);
	bRslt[1] = m_IOClass->GetInputBit(INPUT_INDEX_DOOR2);
	bRslt[2] = m_IOClass->GetInputBit(INPUT_INDEX_DOOR3);

	if (bRslt[0] && bRslt[1] && bRslt[2])
		bRslt[3] = TRUE;

	return bRslt[3];

#else
	return m_IOClass->GetInputBit(INPUT_INDEX_DOOR);
#endif
#else
	return m_bTestVal;
#endif
}

BOOL CInspectionDlg::GetInSenser()
{
#ifdef HW_USE
#ifdef INVERTER_COM
	return m_IOClass->GetInputValue(INPUT_INDEX_INPUT_SENSER);
#else
	return FALSE;
#endif
#else
	return m_bTestVal;
#endif
}

BOOL CInspectionDlg::GetOutSenser()
{
#ifdef HW_USE
#ifdef INVERTER_COM
	return m_IOClass->GetInputValue(INPUT_INDEX_OUTPUT_SENSER);
#else
	return FALSE;
#endif
#else
	return m_bTestVal;
#endif
}

void CInspectionDlg::ClickBtnenhctrlMainLogin()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CDialogLogin m_dlgLogin;
	int nLogin = (int)m_dlgLogin.DoModal();
	if (nLogin == 1)
	{
		LoginTypeEnableBtn(LOGIN_USER);
	}
	else if (nLogin == 2)
	{
		LoginTypeEnableBtn(LOGIN_MANUFACTURE);
	}
	else if (nLogin == 3)
	{
		LoginTypeEnableBtn(LOGIN_ADMIN);
	}
	else
	{

	}
}


void CInspectionDlg::OnBnClickedButtonTestXRayStatus()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strText1, strText2;
	GetDlgItemText(IDC_EDIT_TEST_PARA_0, strText1);
	GetDlgItemText(IDC_EDIT_TEST_PARA_1, strText2);
#ifndef HW_USE
	if (_wtoi(strText1) > 7)
		m_bTestVal = _wtoi(strText2);
	else
		m_XRayClass->SetTestXRayStatus(_wtoi(strText1), _wtoi(strText2));
#endif
}


void CInspectionDlg::OnBnClickedButtonTestXRayStatus2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
#ifndef HW_USE
	for (int i = 0; i < 8; i++)
	{
		m_XRayClass->SetTestXRayStatus(i, FALSE);
	}
#endif
}

void CInspectionDlg::InitLog()
{
	CString strText;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	strText.Format(_T("%s\\%sSequence\\"), strTemp, LOG_FILE_PATH);
	m_pMainLog = new CLogFile(strText, _T("Sequence.log"), &m_list_Log, _T("Log"), 500, 50);

	strText.Format(_T("%s\\%sInspection\\"), strTemp, LOG_FILE_PATH);
	m_pInspectionLog = new CLogFile(strText, _T("Inspection.log"));

	strText.Format(_T("%s\\%sAlarm\\"), strTemp, LOG_FILE_PATH);
	m_pAlarmLog = new CLogFile(strText, _T("Alarm.log"));
	
}

LRESULT CInspectionDlg::OnLogToMainMessage(WPARAM para0, LPARAM para1)
{
	wchar_t *strLog;
	int nStrLen = (int)para1 + 1;
	strLog = new wchar_t[nStrLen];
	memset(strLog, NULL, sizeof(wchar_t) * nStrLen);
	memcpy(strLog, (wchar_t*)para0, sizeof(wchar_t) * nStrLen - 1);

	CString strText;
	strText.Format(_T("%s"), strLog);
	m_strMainLog.push_back(strText);

	delete[] strLog;
	strText.Empty();

	return 0;
}

LRESULT CInspectionDlg::OnLogToInspectionMessage(WPARAM para0, LPARAM para1)
{
	wchar_t* strLog;
	int nStrLen = (int)para1 + 1;
	strLog = DBG_NEW wchar_t[nStrLen];
	memset(strLog, NULL, sizeof(wchar_t) * nStrLen);
	memcpy(strLog, (wchar_t*)para0, sizeof(wchar_t) * nStrLen - 1);

	CString strText;
	strText.Format(_T("%s"), strLog);
	m_strInspectionLog.push_back(strText);

	delete[] strLog;
	strText.Empty();

	return 0;
}

void CInspectionDlg::SetTowerLamp(int nIndex, int nBuzzer)
{
	if (m_IOClass == NULL)
		return;

	m_IOClass->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_RED, FALSE);
	m_IOClass->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_YELLOW, FALSE);
	m_IOClass->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_GREEN, FALSE);
	switch (nIndex)
	{
	case 0:
		m_IOClass->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_RED, TRUE);
		break;
	case 1:
		m_IOClass->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_YELLOW, TRUE);
		break;
	case 2:
		m_IOClass->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_GREEN, TRUE);
		break;
	}

	if(nBuzzer == 0)
		m_IOClass->SetOutputValue(OUTPUT_INDEX_TOWER_BUZZER1, TRUE);
#ifndef INVERTER_COM
	else if(nBuzzer == 1)
		m_IOClass->SetOutputValue(OUTPUT_INDEX_TOWER_BUZZER2, TRUE);
#endif
	else
	{
		m_IOClass->SetOutputValue(OUTPUT_INDEX_TOWER_BUZZER1, FALSE);
#ifndef INVERTER_COM
		m_IOClass->SetOutputValue(OUTPUT_INDEX_TOWER_BUZZER2, FALSE);
#endif
	}

}

void CInspectionDlg::SetMC01On(BOOL isOnOff)
{
	m_IOClass->SetOutputValue(OUTPUT_INDEX_MC01_ON, isOnOff);
}

BOOL CInspectionDlg::SetXRayOn(BOOL isOnOff)
{
	BOOL rslt = TRUE;
	m_IOClass->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, isOnOff);
	if (!isOnOff)
	{
		SetXrayEndTimeWrite(0);
		m_XRayClass->SetXrayEndTime(m_dlgSystem->GetXrayEndTimeRead());
	}

	return rslt;
}

void CInspectionDlg::SetMotorCWTOCCW(int nCW)
{
#ifndef INVERTER_COM
	m_IOClass->SetOutputValue(OUTPUT_INDEX_MOTOR_CW, FALSE);
	m_IOClass->SetOutputValue(OUTPUT_INDEX_MOTOR_CCW, FALSE);

	switch (nCW)
	{
	case 0:
		m_IOClass->SetOutputValue(OUTPUT_INDEX_MOTOR_CW, TRUE);
		break;
	case 1:
		m_IOClass->SetOutputValue(OUTPUT_INDEX_MOTOR_CCW, TRUE);
		break;
	}
#endif
}

#ifndef INVERTER_COM
void CInspectionDlg::SetConMove(int nDirection, BOOL isMove)
{
	if (isMove)
	{
		if (nDirection == 0)
			m_IOClass->SetOutputValue(OUTPUT_INDEX_MOTOR_CW, TRUE);
		else
			m_IOClass->SetOutputValue(OUTPUT_INDEX_MOTOR_CCW, TRUE);
	}
	else
	{
		m_IOClass->SetOutputValue(OUTPUT_INDEX_MOTOR_CW, FALSE);
		m_IOClass->SetOutputValue(OUTPUT_INDEX_MOTOR_CCW, FALSE);
	}
}
#else
void CInspectionDlg::SetConMove(BOOL isMove)
{
	if (isMove && !m_Inverter->GetInverterRunning())
	{		
		m_Inverter->SetInverterMove(m_dlgSystem->GetSystemPara().stInverter.nDirection, m_dlgSystem->GetSystemPara().stInverter.dSpeed, isMove);
	}
	else
	{
		m_Inverter->SetInverterMove();
	}
}
#endif
void CInspectionDlg::SetInverterReset(BOOL isReset)
{
#ifdef INVERTER_COM
	m_Inverter->SetInverterAlarmReset();
#else
	m_IOClass->SetOutputValue(OUTPUT_INDEX_INVERTER_RESET, isReset);
#endif
}

BOOL CInspectionDlg::GetEMG()
{
#ifdef HW_USE
	return m_IOClass->GetInputValue(INPUT_INDEX_EMG);
#else
	return TRUE;
#endif
}

BOOL CInspectionDlg::GetKeySw()
{
#ifdef HW_USE
	return m_IOClass->GetInputValue(INPUT_INDEX_KEY_SW);
#else
	return TRUE;
#endif
}

BOOL CInspectionDlg::GetDoor()
{
#ifdef HW_USE
#ifdef CE_CONFIRM
	BOOL bRslt[4] = { FALSE, };
	bRslt[0] = m_IOClass->GetInputBit(INPUT_INDEX_DOOR1);
	bRslt[1] = m_IOClass->GetInputBit(INPUT_INDEX_DOOR2);
	bRslt[2] = m_IOClass->GetInputBit(INPUT_INDEX_DOOR3);

	if (bRslt[0] && bRslt[1] && bRslt[2])
		bRslt[3] = TRUE;

	return bRslt[3];

#else
	return m_IOClass->GetInputValue(INPUT_INDEX_DOOR);
#endif
#else
	return TRUE;
#endif
}

void CInspectionDlg::SetInverterAlarmReset()
{
#ifdef INVERTER_COM
	m_Inverter->SetInverterAlarmReset();
#else

#endif
}

BOOL CInspectionDlg::GetInverterAlarm()
{
#ifdef HW_USE
#ifndef INVERTER_COM
	return m_IOClass->GetInputValue(INPUT_INDEX_INVERTER_ALARM);
#else

	BOOL bRslt = FALSE;

	if(m_Inverter->GetInverterAlarm()  || m_Inverter->GetInverterTimeOutError())
	{
		//bRslt = TRUE;
	}

	return bRslt;
#endif
#else
	return FALSE;
#endif
}

BOOL CInspectionDlg::GetTubeAlarm()
{
#ifdef HW_USE
#ifndef CE_CONFIRM
	return m_IOClass->GetInputValue(INPUT_INDEX_TUBE_ALARM);
#else
	return FALSE;
#endif
#else
	return FALSE;
#endif
}

BOOL CInspectionDlg::GetXRayLampALM()
{
#ifdef HW_USE
#ifdef INVERTER_COM
	return m_IOClass->GetInputValue(INPUT_INDEX_X_RAY_LAMP_ALM);
#else
	return FALSE;
#endif
#else
	return FALSE;
#endif
}

BOOL CInspectionDlg::GetMC01OnAlarm()
{
#ifdef HW_USE
	return CIOClass::Instance()->GetOutputBit(6);
#else
	return FALSE;
#endif
}

BOOL CInspectionDlg::GetXRayWarmupAlaram()
{
#ifdef HW_USE
	return m_XRayClass->GetXrayWarmupCheck();
#else
	return FALSE;
#endif
}

void CInspectionDlg::StartIOValue()
{
	if (m_IOClass == NULL)
		return;

	m_IOClass->SetOutputValue(OUTPUT_INDEX_MC01_ON, FALSE);
	Sleep(100);
	m_IOClass->SetOutputValue(OUTPUT_INDEX_MC01_ON, TRUE);
	m_IOClass->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);
	SetTowerLamp(1);
	m_XRayClass->SetXrayEndTime(m_dlgSystem->GetXrayEndTimeRead());
	//SetXRayOn();
	//SetConMove();
}

BOOL CInspectionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F5)
			OnBnClickedButtonAuto();
		else if (pMsg->wParam == VK_F6)
			OnBnClickedButtonManual();
		else if (pMsg->wParam == VK_F7)
			OnBnClickedButtonSystem();
		else if (pMsg->wParam == VK_F8)
			OnBnClickedButtonRecipe();
		else if (pMsg->wParam == VK_F9)
		{
			if(m_dlgSystem->GetSystemPara().nInspectionType == 1)
				ClickBtnenhctrlNgView();
		}
		else if (pMsg->wParam == VK_F10)
		{
			ClickBtnenhctrlAlarmLogReport();
		}
		else if (pMsg->wParam == VK_F12)
			ClickBtnenhctrlExit();// OnBnClickedButtonExit();
		else if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CInspectionDlg::ClickBtnenhctrlNgView()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	double dX, dY;
	m_dlgSystem->GetPixelSize(&dX, &dY);
	m_dlgNgView->SetPixelSize(dX, dY);
	m_dlgNgView->SetRecipeName(m_dlgRecipe->GetCurrentRecipeInfo());
	m_dlgNgView->ShowWindow(SW_SHOW);
}

void CInspectionDlg::SetShowResultNG(BOOL isRslt)
{
#ifndef _DEBUG
	if (isRslt)
	{
		if (m_dlgResultNG->IsWindowVisible())
			m_dlgResultNG->ShowWindow(SW_HIDE);
	}
	else
	{
		m_dlgResultNG->ShowWindow(SW_SHOW);
	}
	SetNGBuzzer(isRslt);
#else
	if (m_dlgResultNG->IsWindowVisible())
		m_dlgResultNG->ShowWindow(SW_HIDE);
#endif
}

void CInspectionDlg::SetNGBuzzer(BOOL isRslt)
{
	if (!isRslt)
	{
		m_bNgBuzzer = TRUE;
		m_nNGBuzzerCnt = 0;
#ifndef _DEBUG
		SetTowerLamp(2, 0);
		SetTimer(TIMER_NG_BUZZER, 100, NULL);
#endif
	}
	else
	{
#ifndef _DEBUG
		SetTowerLamp(2);
		KillTimer(TIMER_NG_BUZZER);
#endif
	}
}

void CInspectionDlg::ClickBtnenhctrlMainInterlockStatus2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_dlgInterLockStatus->IsWindowVisible())
		m_dlgInterLockStatus->ShowWindow(SW_HIDE);
	else
		m_dlgInterLockStatus->ShowWindow(SW_SHOW);
}


void CInspectionDlg::ClickBtnenhctrlAlarmLogReport()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_dlgAlarmLogReport->IsWindowVisible())
		m_dlgAlarmLogReport->ShowWindow(SW_HIDE);
	else
		m_dlgAlarmLogReport->ShowWindow(SW_SHOW);
	
}
