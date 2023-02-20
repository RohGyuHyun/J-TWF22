// CDialogSystem.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogSystem.h"
#include "ProfileDataIF.h"
#include "GetNumDlg.h"
#include "GetTextDlg.h"
#include "CATDetector.h"
#include "CInverterClass.h"

// CDialogSystem 대화 상자

IMPLEMENT_DYNAMIC(CDialogSystem, CDialogEx)

CDialogSystem::CDialogSystem(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SYSTEM, pParent)
	, m_chk_SysPara0(FALSE)
	, m_chk_SysPara1(FALSE)
	, m_chk_SysPara2(FALSE)
	, m_chk_SysPara3(FALSE)
	, m_chk_SysPara4(FALSE)
	, m_chk_SysPara5(FALSE)
{
	//memset(&m_stSystemPara, NULL, sizeof(SYSTEM_PARA));
	m_bInverterMove = FALSE;
}

CDialogSystem::~CDialogSystem()
{
}

void CDialogSystem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SYSTEM_PARA_0, m_chk_SysPara0);
	DDX_Check(pDX, IDC_CHECK_SYSTEM_PARA_1, m_chk_SysPara1);
	DDX_Check(pDX, IDC_CHECK_SYSTEM_PARA_2, m_chk_SysPara2);
	DDX_Check(pDX, IDC_CHECK_SYSTEM_PARA_3, m_chk_SysPara3);
	DDX_Check(pDX, IDC_CHECK_SYSTEM_PARA_4, m_chk_SysPara4);
	DDX_Control(pDX, IDC_COMBO_SYSTEM_PARA0, m_combo_SystemPara0);
	DDX_Control(pDX, IDC_COMBO_SYSTEM_PARA1, m_combo_SystemPara1);
	DDX_Control(pDX, IDC_COMBO_SYSTEM_PARA2, m_combo_SystemPara2);
	DDX_Control(pDX, IDC_COMBO_SYSTEM_PARA3, m_combo_SystemPara3);
	DDX_Control(pDX, IDC_COMBO_SYSTEM_PARA4, m_combo_SystemPara4);
	DDX_Control(pDX, IDC_COMBO_PORT, m_combo_SystemPara5);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_combo_SystemPara6);
	DDX_Control(pDX, IDC_COMBO_DIRECTION, m_combo_SystemPara7);
	DDX_Check(pDX, IDC_CHECK_SYSTEM_PARA_5, m_chk_SysPara5);
}


BEGIN_MESSAGE_MAP(CDialogSystem, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDialogSystem::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDialogSystem::OnBnClickedButtonSave)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK_SYSTEM_PARA_0, &CDialogSystem::OnBnClickedCheckSystemPara0)
	ON_BN_CLICKED(IDC_CHECK_SYSTEM_PARA_1, &CDialogSystem::OnBnClickedCheckSystemPara1)
	ON_BN_CLICKED(IDC_CHECK_SYSTEM_PARA_2, &CDialogSystem::OnBnClickedCheckSystemPara2)
	ON_BN_CLICKED(IDC_CHECK_SYSTEM_PARA_3, &CDialogSystem::OnBnClickedCheckSystemPara3)
	ON_BN_CLICKED(IDC_CHECK_SYSTEM_PARA_4, &CDialogSystem::OnBnClickedCheckSystemPara4)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogSystem::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK_SYSTEM_PARA_5, &CDialogSystem::OnBnClickedCheckSystemPara5)
END_MESSAGE_MAP()


// CDialogSystem 메시지 처리기


void CDialogSystem::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bInverterMove = FALSE;
	CInverterClass::Instance()->SetInverterMove();
	this->ShowWindow(SW_HIDE);
}


BOOL CDialogSystem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitCombo();
	LoadSystemPara();
	UpdatePara(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogSystem::InitCombo()
{
	CString strText;
	int i = 0;
	m_combo_SystemPara0.AddString(_T("400um"));
	m_combo_SystemPara0.AddString(_T("200um"));
	m_combo_SystemPara0.AddString(_T("100um"));

	m_combo_SystemPara1.AddString(_T("Original"));
	m_combo_SystemPara1.AddString(_T("Sum x 2"));
	m_combo_SystemPara1.AddString(_T("Avg x 2"));
	m_combo_SystemPara1.AddString(_T("Sum x 4"));
	m_combo_SystemPara1.AddString(_T("Avg x 4"));

	m_combo_SystemPara2.AddString(_T("Single"));
	m_combo_SystemPara2.AddString(_T("Dual"));
	m_combo_SystemPara2.AddString(_T("TDI"));

	m_combo_SystemPara3.AddString(_T("Low"));
	m_combo_SystemPara3.AddString(_T("High"));

	m_combo_SystemPara4.AddString(_T("16bit Raw Data"));
	m_combo_SystemPara4.AddString(_T("8bit Correction Data"));
	m_combo_SystemPara4.AddString(_T("16bit Correction Data"));
}

void CDialogSystem::UpdatePara(BOOL isUpdate)
{
	CString strText;
	int nData;
	if (isUpdate)
	{
		UpdateData(TRUE);
		//GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA0, strText);
		//m_stSystemPara.stXRay.nPortIndex = _wtoi(strText);
		//GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA1, strText);
		//m_stSystemPara.stDetector.nUsbIndex = _wtoi(strText);
		//GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA2, strText);
		//m_stSystemPara.stDetector.dScanSpeed = _wtof(strText);
		//GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA3, strText);
		////wprintf_s(m_stSystemPara.strSveImageDirectory, strText.GetLength() + 1, "%s", strText);
		//memcpy(m_stSystemPara.strSveImageDirectory, strText.GetBuffer(), sizeof(wchar_t) * (strText.GetLength() + 1));

		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA0, strText);
		m_stSystemPara.stDetector.nUsbIndex = _wtoi(strText);
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA1, strText);
		m_stSystemPara.stDetector.ICamWidth = _wtoi(strText);
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA2, strText);
		m_stSystemPara.stDetector.lCamHeight = _wtoi(strText);

		nData = m_combo_SystemPara0.GetCurSel();
		switch (nData)
		{
		case 0:
			m_stSystemPara.stDetector.nCamPixelSize = 400;
			break;
		case 1:
			m_stSystemPara.stDetector.nCamPixelSize = 200;
			break;
		case 2:
			m_stSystemPara.stDetector.nCamPixelSize = 100;
			break;
		}

		m_stSystemPara.stDetector.nCamBinnig = m_combo_SystemPara1.GetCurSel();
		m_stSystemPara.stDetector.nCamMode = m_combo_SystemPara2.GetCurSel();

		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA3, strText);
		m_stSystemPara.stDetector.nCamObjectDetectionThreshHold = _wtoi(strText);
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA4, strText);
		m_stSystemPara.stDetector.nCamIntegrationTime = _wtoi(strText);

		m_stSystemPara.stDetector.nCamSensitivity = m_combo_SystemPara3.GetCurSel();
		m_stSystemPara.stDetector.nCamImageFormat = m_combo_SystemPara4.GetCurSel();

		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA5, strText);
		m_stSystemPara.stDetector.dScanSpeed = _wtof(strText);
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA6, strText);
		m_stSystemPara.stDetector.nCamAverage = _wtoi(strText);
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA7, strText);
		m_stSystemPara.stDetector.dTotalLength = _wtof(strText);
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA8, strText);
		m_stSystemPara.stDetector.dObjectLength = _wtof(strText);
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA9, strText);
		memcpy(m_stSystemPara.stDetector.strCamCalFileName, strText.GetBuffer(), sizeof(wchar_t) * (strText.GetLength() + 1));
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA10, strText);
		m_stSystemPara.stDetector.nCamLineToAvg = _wtoi(strText);

		m_stSystemPara.stXRay.nPortIndex = m_combo_SystemPara5.GetCurSel() + 1;
		nData = m_combo_SystemPara6.GetCurSel();

		if (nData == 0)
			m_stSystemPara.stXRay.nPortBaudrate = 9600;
		else
			m_stSystemPara.stXRay.nPortBaudrate = 115200;

		m_stSystemPara.stInverter.nDirection = m_combo_SystemPara7.GetCurSel();
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA11, strText);
		m_stSystemPara.stInverter.dSpeed = _wtof(strText);

		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA12, strText);
		m_stSystemPara.nInSensingTime = _wtoi(strText);
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA13, strText);
		m_stSystemPara.nOutSensingTime = _wtoi(strText);

		m_stSystemPara.bSaveInspectionRawImage = m_chk_SysPara0;
		m_stSystemPara.bSaveResultOKImage = m_chk_SysPara1;
		m_stSystemPara.bSaveResultNGImage = m_chk_SysPara2;
		m_stSystemPara.stDetector.bCamObjectDetection = m_chk_SysPara3;
		m_stSystemPara.stDetector.bAutoCalibration = m_chk_SysPara4;

		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA17, strText);
		m_stSystemPara.dPixelSizeX = _wtof(strText);
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA18, strText);
		m_stSystemPara.dPixelSizeY = _wtof(strText);

		m_stSystemPara.stRejecter.bUse = m_chk_SysPara5;
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA19, strText);
		m_stSystemPara.stRejecter.nDelayTime = _wtoi(strText);
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA20, strText);
		m_stSystemPara.stRejecter.nEndDelayTime = _wtoi(strText);

		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA21, strText);
		m_stSystemPara.nHDDSpaceAlarmPer = _wtoi(strText);
		
		m_stSystemPara.stDetector.bDirection = m_stSystemPara.stInverter.nDirection;
	}
	else
	{
		
		/*strText.Format(_T("%d"), m_stSystemPara.stXRay.nPortIndex);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA0, strText);
		strText.Format(_T("%d"), m_stSystemPara.stDetector.nUsbIndex);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA1, strText);
		strText.Format(_T("%d"), m_stSystemPara.stDetector.dScanSpeed);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA2, strText);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA3, m_stSystemPara.strSveImageDirectory);
		*/
		strText.Format(_T("%d"), m_stSystemPara.stDetector.nUsbIndex);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA0, strText);
		strText.Format(_T("%d"), m_stSystemPara.stDetector.ICamWidth);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA1, strText);
		strText.Format(_T("%d"), m_stSystemPara.stDetector.lCamHeight);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA2, strText);

		switch (m_stSystemPara.stDetector.nCamPixelSize)
		{
		case 100:
			m_combo_SystemPara0.SetCurSel(2);
			break;
		case 200:
			m_combo_SystemPara0.SetCurSel(1);
			break;
		case 400:
		default:
			m_combo_SystemPara0.SetCurSel(0);
			break;
		}

		m_combo_SystemPara1.SetCurSel(m_stSystemPara.stDetector.nCamBinnig);
		m_combo_SystemPara2.SetCurSel(m_stSystemPara.stDetector.nCamMode);

		strText.Format(_T("%d"), m_stSystemPara.stDetector.nCamObjectDetectionThreshHold);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA3, strText);
		strText.Format(_T("%d"), m_stSystemPara.stDetector.nCamIntegrationTime);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA4, strText);

		m_combo_SystemPara3.SetCurSel(m_stSystemPara.stDetector.nCamSensitivity);
		m_combo_SystemPara4.SetCurSel(m_stSystemPara.stDetector.nCamImageFormat);

		strText.Format(_T("%0.3f"), m_stSystemPara.stDetector.dScanSpeed);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA5, strText);
		strText.Format(_T("%d"), m_stSystemPara.stDetector.nCamAverage);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA6, strText);
		strText.Format(_T("%0.3f"), m_stSystemPara.stDetector.dTotalLength);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA7, strText);
		strText.Format(_T("%0.3f"), m_stSystemPara.stDetector.dObjectLength);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA8, strText);
		strText.Format(_T("%s"), m_stSystemPara.stDetector.strCamCalFileName);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA9, strText);
		strText.Format(_T("%d"), m_stSystemPara.stDetector.nCamLineToAvg);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA10, strText);


		m_combo_SystemPara5.SetCurSel(m_stSystemPara.stXRay.nPortIndex - 1);
		if (m_stSystemPara.stXRay.nPortBaudrate == 9600)
			m_combo_SystemPara6.SetCurSel(0);
		else
			m_combo_SystemPara6.SetCurSel(1);

		m_combo_SystemPara7.SetCurSel(m_stSystemPara.stInverter.nDirection);
		strText.Format(_T("%0.3f"), m_stSystemPara.stInverter.dSpeed);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA11, strText);

		strText.Format(_T("%d"), m_stSystemPara.nInSensingTime);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA12, strText);
		strText.Format(_T("%d"), m_stSystemPara.nOutSensingTime);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA13, strText);

		m_chk_SysPara0 = m_stSystemPara.bSaveInspectionRawImage;
		m_chk_SysPara1 = m_stSystemPara.bSaveResultOKImage;
		m_chk_SysPara2 = m_stSystemPara.bSaveResultNGImage;
		m_chk_SysPara3 = m_stSystemPara.stDetector.bCamObjectDetection;
		m_chk_SysPara4 = m_stSystemPara.stDetector.bAutoCalibration;

		strText.Format(_T("%0.3f"), m_stSystemPara.dPixelSizeX);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA17, strText);
		strText.Format(_T("%0.3f"), m_stSystemPara.dPixelSizeY);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA18, strText);

		m_chk_SysPara5 = m_stSystemPara.stRejecter.bUse;
		strText.Format(_T("%d"), m_stSystemPara.stRejecter.nDelayTime);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA19, strText);
		strText.Format(_T("%d"), m_stSystemPara.stRejecter.nEndDelayTime);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA20, strText);

		strText.Format(_T("%d"), m_stSystemPara.nHDDSpaceAlarmPer);
		SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA21, strText);
		UpdateData(FALSE);
	}
}

BOOL CDialogSystem::LoadSystemPara()
{
	CString key, dat;
	CString file_path, pat_path, extention, master_path, cur_path;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	file_path.Format(_T("%s\\System\\%s"), strTemp, SYSTEM_STR_PARA_FILENAME);

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(file_path);
	CString strNode, strKey, strLabel;

	strNode.Format(_T("SYSTEM_PARAMETER"));

	strKey.Format(_T("Inspection_Type"));
	m_stSystemPara.nInspectionType = cDataIf.GetInt(strNode, strKey, 1);

	strKey.Format(_T("Detector_Port_Number"));
	m_stSystemPara.stDetector.nUsbIndex = cDataIf.GetInt(strNode, strKey, 0);
	strKey.Format(_T("Detector_Cam_Width"));
	m_stSystemPara.stDetector.ICamWidth = cDataIf.GetInt(strNode, strKey, 1024);
	strKey.Format(_T("Detector_Cam_Height"));
	m_stSystemPara.stDetector.lCamHeight = cDataIf.GetInt(strNode, strKey, 1126);
	strKey.Format(_T("Detector_Pixel_Size"));
	m_stSystemPara.stDetector.nCamPixelSize = cDataIf.GetInt(strNode, strKey, 400);
	strKey.Format(_T("Detector_Cam_Binning"));
	m_stSystemPara.stDetector.nCamBinnig = cDataIf.GetInt(strNode, strKey, 0);
	strKey.Format(_T("Detector_Cam_Mode"));
	m_stSystemPara.stDetector.nCamMode = cDataIf.GetInt(strNode, strKey, 0);
	strKey.Format(_T("Detector_Object_Detection"));
	m_stSystemPara.stDetector.bCamObjectDetection = cDataIf.GetInt(strNode, strKey, 0);
	strKey.Format(_T("Detector_Object_Detection_Thresh_Hold"));
	m_stSystemPara.stDetector.nCamObjectDetectionThreshHold = cDataIf.GetInt(strNode, strKey, 3);
	strKey.Format(_T("Detector_Intergration_Time"));
	m_stSystemPara.stDetector.nCamIntegrationTime = cDataIf.GetInt(strNode, strKey, 100);
	strKey.Format(_T("Detector_Sensitivity"));
	m_stSystemPara.stDetector.nCamSensitivity = cDataIf.GetInt(strNode, strKey, 1);
	strKey.Format(_T("Detector_Image_Format"));
	m_stSystemPara.stDetector.nCamImageFormat = cDataIf.GetInt(strNode, strKey, 0);
	strKey.Format(_T("Detector_Scan_Speed"));
	m_stSystemPara.stDetector.dScanSpeed = cDataIf.GetDouble(strNode, strKey, 10);
	strKey.Format(_T("Detector_Average"));
	m_stSystemPara.stDetector.nCamAverage = cDataIf.GetInt(strNode, strKey, 1);
	strKey.Format(_T("Detector_Total_Length"));
	m_stSystemPara.stDetector.dTotalLength = cDataIf.GetDouble(strNode, strKey, 1);
	strKey.Format(_T("Detector_Object_Length"));
	m_stSystemPara.stDetector.dObjectLength = cDataIf.GetDouble(strNode, strKey, 1);
	strKey.Format(_T("Detector_AutoCalibration"));
	m_stSystemPara.stDetector.bAutoCalibration = cDataIf.GetInt(strNode, strKey, 0);
	strKey.Format(_T("Detector_Cal_File_Name"));
	strLabel = cDataIf.GetString(strNode, strKey, _T("Calibration_20200808"));
	memcpy(m_stSystemPara.stDetector.strCamCalFileName, strLabel.GetBuffer(), sizeof(wchar_t) * (strLabel.GetLength() + 1));
	strKey.Format(_T("Detector_Line_To_Avg"));
	m_stSystemPara.stDetector.nCamLineToAvg = cDataIf.GetInt(strNode, strKey, 30);





	strKey.Format(_T("X-Ray_Port_Number"));
	m_stSystemPara.stXRay.nPortIndex = cDataIf.GetInt(strNode, strKey, 2);
	strKey.Format(_T("X-Ray_Port_Baudrate"));
	m_stSystemPara.stXRay.nPortBaudrate = cDataIf.GetInt(strNode, strKey, 9600);


	strKey.Format(_T("Inverter_Direction"));
	m_stSystemPara.stInverter.nDirection = cDataIf.GetInt(strNode, strKey, 0);
	strKey.Format(_T("Inverter_Speed"));
	m_stSystemPara.stInverter.dSpeed = cDataIf.GetDouble(strNode, strKey, 10);

	strKey.Format(_T("Save_Image_Directory"));
	strLabel = cDataIf.GetString(strNode, strKey, _T("D:\\Result\\"));
	//wprintf_s(m_stSystemPara.strSveImageDirectory, strLabel.GetLength() + 1, "%S", strLabel);
	memcpy(m_stSystemPara.strSveImageDirectory, strLabel.GetBuffer(), sizeof(wchar_t) * (strLabel.GetLength() + 1));
	strKey.Format(_T("Save_Inspection_Raw_Image"));
	m_stSystemPara.bSaveInspectionRawImage = cDataIf.GetInt(strNode, strKey, 0);
	strKey.Format(_T("Save_Result_OK_Image"));
	m_stSystemPara.bSaveResultOKImage = cDataIf.GetInt(strNode, strKey, 0);
	strKey.Format(_T("Save_Result_NG_Image"));
	m_stSystemPara.bSaveResultNGImage = cDataIf.GetInt(strNode, strKey, 0);

	strKey.Format(_T("In_Sensing_Time"));
	m_stSystemPara.nInSensingTime = cDataIf.GetInt(strNode, strKey, 3);
	strKey.Format(_T("Out_Sensing_Time"));
	m_stSystemPara.nOutSensingTime = cDataIf.GetInt(strNode, strKey, 3);

	strKey.Format(_T("PIXEL_SIZE_X"));
	m_stSystemPara.dPixelSizeX = cDataIf.GetDouble(strNode, strKey, 0.4);
	strKey.Format(_T("PIXEL_SIZE_Y"));
	m_stSystemPara.dPixelSizeY = cDataIf.GetDouble(strNode, strKey, 0.4);

	strKey.Format(_T("REJECTER_USE"));
	m_stSystemPara.stRejecter.bUse = cDataIf.GetInt(strNode, strKey, 0);
	strKey.Format(_T("REJECTER_DELAY_TIME"));
	m_stSystemPara.stRejecter.nDelayTime = cDataIf.GetInt(strNode, strKey, 2000);
	strKey.Format(_T("REJECTER_END_DELAY_TIME"));
	m_stSystemPara.stRejecter.nEndDelayTime = cDataIf.GetInt(strNode, strKey, 500);

	strKey.Format(_T("HDD_ALARM_USE_PER"));
	m_stSystemPara.nHDDSpaceAlarmPer = cDataIf.GetInt(strNode, strKey, 80);

	m_stSystemPara.stDetector.bDirection = m_stSystemPara.stInverter.nDirection;

	return TRUE;
}

BOOL CDialogSystem::SaveSystemPara()
{
	CString key, dat;
	CString file_path, pat_path, extention, master_path, cur_path;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	file_path.Format(_T("%s\\System\\%s"), strTemp, SYSTEM_STR_PARA_FILENAME);

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(file_path);
	CString strNode, strKey, strLabel;

	strNode.Format(_T("SYSTEM_PARAMETER"));

	strKey.Format(_T("Inspection_Type"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.nInspectionType);

	strKey.Format(_T("Detector_Port_Number"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.nUsbIndex);
	strKey.Format(_T("Detector_Cam_Width"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.ICamWidth);
	strKey.Format(_T("Detector_Cam_Height"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.lCamHeight);
	strKey.Format(_T("Detector_Pixel_Size"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.nCamPixelSize);
	strKey.Format(_T("Detector_Cam_Binning"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.nCamBinnig);
	strKey.Format(_T("Detector_Cam_Mode"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.nCamMode);
	strKey.Format(_T("Detector_Object_Detection"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.bCamObjectDetection);
	strKey.Format(_T("Detector_Object_Detection_Thresh_Hold"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.nCamObjectDetectionThreshHold);
	strKey.Format(_T("Detector_Intergration_Time"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.nCamIntegrationTime);
	strKey.Format(_T("Detector_Sensitivity"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.nCamSensitivity);
	strKey.Format(_T("Detector_Image_Format"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.nCamImageFormat);
	strKey.Format(_T("Detector_Scan_Speed"));
	cDataIf.SetDouble(strNode, strKey, m_stSystemPara.stDetector.dScanSpeed);
	strKey.Format(_T("Detector_Average"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.nCamAverage);
	strKey.Format(_T("Detector_Total_Length"));
	cDataIf.SetDouble(strNode, strKey, m_stSystemPara.stDetector.dTotalLength);
	strKey.Format(_T("Detector_Object_Length"));
	cDataIf.SetDouble(strNode, strKey, m_stSystemPara.stDetector.dObjectLength);
	strKey.Format(_T("Detector_AutoCalibration"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.bAutoCalibration);
	strKey.Format(_T("Detector_Cal_File_Name"));
	strLabel.Format(_T("%s"), m_stSystemPara.stDetector.strCamCalFileName);
	cDataIf.SetString(strNode, strKey, strLabel);
	strKey.Format(_T("Detector_Line_To_Avg"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stDetector.nCamLineToAvg);




	strKey.Format(_T("X-Ray_Port_Number"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stXRay.nPortIndex);
	strKey.Format(_T("X-Ray_Port_Baudrate"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stXRay.nPortBaudrate);

	strKey.Format(_T("Inverter_Direction"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stInverter.nDirection);
	strKey.Format(_T("Inverter_Speed"));
	cDataIf.SetDouble(strNode, strKey, m_stSystemPara.stInverter.dSpeed);


	strKey.Format(_T("Save_Image_Directory"));
	cDataIf.SetString(strNode, strKey, m_stSystemPara.strSveImageDirectory);
	strKey.Format(_T("Save_Inspection_Raw_Image"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.bSaveInspectionRawImage);
	strKey.Format(_T("Save_Result_OK_Image"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.bSaveResultOKImage);
	strKey.Format(_T("Save_Result_NG_Image"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.bSaveResultNGImage);

	strKey.Format(_T("In_Sensing_Time"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.nInSensingTime);
	strKey.Format(_T("Out_Sensing_Time"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.nOutSensingTime);

	strKey.Format(_T("PIXEL_SIZE_X"));
	cDataIf.SetDouble(strNode, strKey, m_stSystemPara.dPixelSizeX);
	strKey.Format(_T("PIXEL_SIZE_Y"));
	cDataIf.SetDouble(strNode, strKey, m_stSystemPara.dPixelSizeY);


	strKey.Format(_T("REJECTER_USE"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stRejecter.bUse);
	strKey.Format(_T("REJECTER_DELAY_TIME"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stRejecter.nDelayTime);
	strKey.Format(_T("REJECTER_END_DELAY_TIME"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.stRejecter.nEndDelayTime);


	strKey.Format(_T("HDD_ALARM_USE_PER"));
	cDataIf.SetInt(strNode, strKey, m_stSystemPara.nHDDSpaceAlarmPer);
	return TRUE;
}


void CDialogSystem::SetXrayEndTimeWrite(TIME_DATE cEndTime)
{
	CString key, dat;
	CString file_path, pat_path, extention, master_path, cur_path;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	file_path.Format(_T("%s\\System\\%s"), strTemp, SYSTEM_STR_PARA_FILENAME);

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(file_path);
	CString strNode, strKey, strLabel;

	strNode.Format(_T("X_RAY_END_TIME"));

	strKey.Format(_T("X_RAY_END_TIME_YEAR"));
	cDataIf.SetInt(strNode, strKey, cEndTime.nYear);
	strKey.Format(_T("X_RAY_END_TIME_MONTH"));
	cDataIf.SetInt(strNode, strKey, cEndTime.nMonth);
	strKey.Format(_T("X_RAY_END_TIME_DAY"));
	cDataIf.SetInt(strNode, strKey, cEndTime.nDay);
	strKey.Format(_T("X_RAY_END_TIME_HOUR"));
	cDataIf.SetInt(strNode, strKey, cEndTime.nHour);
	strKey.Format(_T("X_RAY_END_TIME_MINUTE"));
	cDataIf.SetInt(strNode, strKey, cEndTime.nMinute);
	strKey.Format(_T("X_RAY_END_TIME_SECOND"));
	cDataIf.SetInt(strNode, strKey, cEndTime.nSec);
}

TIME_DATE CDialogSystem::GetXrayEndTimeRead()
{
	TIME_DATE cEndTime;
	CString key, dat;
	CString file_path, pat_path, extention, master_path, cur_path;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	file_path.Format(_T("%s\\System\\%s"), strTemp, SYSTEM_STR_PARA_FILENAME);

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(file_path);
	CString strNode, strKey, strLabel;

	strNode.Format(_T("X_RAY_END_TIME"));

	strKey.Format(_T("X_RAY_END_TIME_YEAR"));
	cEndTime.nYear = cDataIf.GetInt(strNode, strKey, 2000);
	strKey.Format(_T("X_RAY_END_TIME_MONTH"));
	cEndTime.nMonth = cDataIf.GetInt(strNode, strKey, 1);
	strKey.Format(_T("X_RAY_END_TIME_DAY"));
	cEndTime.nDay = cDataIf.GetInt(strNode, strKey, 1);
	strKey.Format(_T("X_RAY_END_TIME_HOUR"));
	cEndTime.nHour = cDataIf.GetInt(strNode, strKey, 0);
	strKey.Format(_T("X_RAY_END_TIME_MINUTE"));
	cEndTime.nMinute = cDataIf.GetInt(strNode, strKey, 0);
	strKey.Format(_T("X_RAY_END_TIME_SECOND"));
	cEndTime.nSec = cDataIf.GetInt(strNode, strKey, 0);

	return cEndTime;
}

void CDialogSystem::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdatePara(TRUE);
	SaveSystemPara();
	CAtDetector::Instance()->Init(m_stSystemPara.stDetector, m_strRecipe);
}

void CDialogSystem::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		LoadSystemPara();
		UpdatePara(FALSE);
		LoginTypeBtnEnable();
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BEGIN_EVENTSINK_MAP(CDialogSystem, CDialogEx)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA3, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaString, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA0, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaInt, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA1, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaInt, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA2, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaInt, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA3, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaInt, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA4, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaInt, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA5, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaDouble, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA7, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaInt, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA6, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaDouble, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA8, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaDouble, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA9, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaDouble, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA10, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaInt, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA11, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaDouble, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA12, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaInt, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA13, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaInt, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA17, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaDouble, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA18, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaDouble, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA19, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaInt, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA20, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaInt, VTS_NONE)
	ON_EVENT(CDialogSystem, IDC_BTNENHCTRL_SYSTEM_PARA21, DISPID_CLICK, CDialogSystem::ClickBtnenhctrlSystemParaInt, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogSystem::ClickBtnenhctrlSystemParaString()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetFocus();
	CGetTextDlg	dlg(30, _T(""), _T("Calibration File Name"), NULL, FALSE);

	if (dlg.DoModal() == IDCANCEL) return;
	CString strText = dlg.GetStringValue();
	pBtnEnh->SetWindowTextW(strText);
	 
	//BROWSEINFO BrInfo;
	//TCHAR szBuffer[512];                                      // 경로저장 버퍼 

	//::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	//::ZeroMemory(szBuffer, 512);

	//BrInfo.hwndOwner = GetSafeHwnd();
	//BrInfo.lpszTitle = _T("파일이 저장될 폴더를 선택하세요");
	//BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	//LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	//::SHGetPathFromIDList(pItemIdList, szBuffer);               // 파일경로 읽어오기

	//if (pItemIdList == NULL)
	//	return;

	//// 경로를 가져와 사용할 경우, Edit Control 에 값 저장
	//CString str;
	//str.Format(_T("%s"), szBuffer);
	//if (str.Mid(str.GetLength() - 1, str.GetLength()).Compare(_T("\\")))
	//	str += _T("\\");

	//SetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA3, str);
	//UpdateData(FALSE);
}


void CDialogSystem::ClickBtnenhctrlSystemParaInt()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetFocus();
	CGetNumDlg dlg;
	CString strText;
	pBtnEnh->GetWindowTextW(strText);
	int nVal = _wtoi(strText);

	dlg.SetstrNum(nVal);

	if (dlg.DoModal() != IDOK) return;

	strText = dlg.GetstrNum();

	if (pBtnEnh->GetDlgCtrlID() == IDC_BTNENHCTRL_SYSTEM_PARA20)
	{
		nVal = _wtoi(strText);
		GetDlgItemText(IDC_BTNENHCTRL_SYSTEM_PARA19, strText);
		if (nVal > _wtoi(strText))
		{
			pBtnEnh->SetWindowText(strText);
		}
	}
	else if (pBtnEnh->GetDlgCtrlID() == IDC_BTNENHCTRL_SYSTEM_PARA21)
	{
		if (_wtoi(strText) > 100)
			strText.Format(_T("100"));
	}

	pBtnEnh->SetWindowTextW(strText);
}

void CDialogSystem::ClickBtnenhctrlSystemParaDouble()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetFocus();
	CGetNumDlg dlg;
	CString strText;
	pBtnEnh->GetWindowTextW(strText);
	double nVal = _wtof(strText);

	dlg.SetstrNum(nVal);

	if (dlg.DoModal() != IDOK) return;

	if (dlg.GetstrNum().Find(_T("."), 0) > 0)
		strText = dlg.GetstrNum();
	else
		strText.Format(_T("%s.000"), dlg.GetstrNum());

	pBtnEnh->SetWindowTextW(strText);

	if (pBtnEnh->GetDlgCtrlID() == IDC_BTNENHCTRL_SYSTEM_PARA11 || pBtnEnh->GetDlgCtrlID() == IDC_BTNENHCTRL_SYSTEM_PARA5)
	{
		/*pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA5);
		pBtnEnh->SetWindowText(strText);
		pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA11);
		pBtnEnh->SetWindowText(strText);*/
	}
}



void CDialogSystem::OnBnClickedCheckSystemPara0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_chk_SysPara0)
		m_chk_SysPara0 = TRUE;
	else
		m_chk_SysPara0 = FALSE;
}


void CDialogSystem::OnBnClickedCheckSystemPara1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_chk_SysPara1)
		m_chk_SysPara1 = TRUE;
	else
		m_chk_SysPara1 = FALSE;
}


void CDialogSystem::OnBnClickedCheckSystemPara2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_chk_SysPara2)
		m_chk_SysPara2 = TRUE;
	else
		m_chk_SysPara2 = FALSE;
}

void CDialogSystem::OnBnClickedCheckSystemPara3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_chk_SysPara3)
		m_chk_SysPara3 = TRUE;
	else
		m_chk_SysPara3 = FALSE;
}

void CDialogSystem::OnBnClickedCheckSystemPara4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_chk_SysPara4)
		m_chk_SysPara4 = TRUE;
	else
		m_chk_SysPara4 = FALSE;
}

void CDialogSystem::SetCalFileName(CString strFileName)
{
	CString key, dat;
	CString file_path, pat_path, extention, master_path, cur_path;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	file_path.Format(_T("%s\\System\\%s"), strTemp, SYSTEM_STR_PARA_FILENAME);

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(file_path);
	CString strNode, strKey, strLabel;

	strNode.Format(_T("SYSTEM_PARAMETER"));
	strKey.Format(_T("Detector_Cal_File_Name"));
	memcpy(m_stSystemPara.stDetector.strCamCalFileName, strFileName.GetBuffer(), sizeof(wchar_t) * (strFileName.GetLength() + 1));
	strLabel.Format(_T("%s"), m_stSystemPara.stDetector.strCamCalFileName);
	cDataIf.SetString(strNode, strKey, strLabel);
}

BOOL CDialogSystem::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDialogSystem::LoginTypeBtnEnable()
{

	if (m_LoginType == LOGIN_USER || m_LoginType == LOGIN_MANUFACTURE)
	{
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA0)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA4)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA5)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA6)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA7)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA8)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA9)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA10)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA11)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA12)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA13)->EnableWindow(FALSE);

		GetDlgItem(IDC_COMBO_SYSTEM_PARA0)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SYSTEM_PARA1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SYSTEM_PARA2)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SYSTEM_PARA3)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SYSTEM_PARA4)->EnableWindow(FALSE);

		GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(FALSE);

		GetDlgItem(IDC_CHECK_SYSTEM_PARA_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SYSTEM_PARA_4)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA0)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA4)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA5)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA6)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA7)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA8)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA9)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA10)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA11)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA12)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNENHCTRL_SYSTEM_PARA13)->EnableWindow(TRUE);

		GetDlgItem(IDC_COMBO_SYSTEM_PARA0)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SYSTEM_PARA1)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SYSTEM_PARA2)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SYSTEM_PARA3)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SYSTEM_PARA4)->EnableWindow(TRUE);

		GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow(TRUE);

		GetDlgItem(IDC_CHECK_SYSTEM_PARA_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SYSTEM_PARA_4)->EnableWindow(TRUE);
	}
}

void CDialogSystem::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdatePara(TRUE);
	if (!m_bInverterMove)
	{
		m_bInverterMove = TRUE;
		CInverterClass::Instance()->SetInverterMove(m_stSystemPara.stInverter.nDirection, m_stSystemPara.stInverter.dSpeed, TRUE);
	}
	else
	{
		m_bInverterMove = FALSE;
		CInverterClass::Instance()->SetInverterMove();
	}
}


void CDialogSystem::OnBnClickedCheckSystemPara5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_chk_SysPara5)
		m_chk_SysPara5 = TRUE;
	else
		m_chk_SysPara5 = FALSE;
}
