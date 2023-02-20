#include "pch.h"
#include "CATDetector.h"

CAtDetector* m_This;
CAtDetector* CAtDetector::m_pInstance = NULL;
CAtDetector::CAtDetector()
{
	m_pImageBuff = NULL;
	memset(&m_DetectorPara, NULL, sizeof(DETECTOR_PARA));
	m_bCalStart = FALSE;
	m_bCalEnd = FALSE;
}

CAtDetector::~CAtDetector()
{
	if (m_pImageBuff != NULL)
	{
		delete m_pImageBuff;
		m_pImageBuff = NULL;
	}

	BOOL rslt = TRUE;;
	if (m_ATXrayDetector.atd_isOpen())
	{
		rslt = m_ATXrayDetector.atd_Close();
	}
}

void AtCallBackEvent(int code, const char* event)
{
	m_This->EventProcess(code, event);
}

void AtCallBackFrame()
{
	m_This->ImageCopy();
}


BOOL CAtDetector::Close()
{
	if (m_pImageBuff != NULL)
	{
		delete m_pImageBuff;
		m_pImageBuff = NULL;
	}

	BOOL rslt = TRUE;;
	if (m_ATXrayDetector.atd_isOpen())
	{
		m_ATXrayDetector.atd_Close();
	}

	return rslt;
}

CAtDetector* CAtDetector::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = DBG_NEW CAtDetector;
	}
	return m_pInstance;
}

void CAtDetector::EventProcess(int code, const char* event)
{
	m_strEvent.Format(_T("code : %d, Event : %S"), code, event);
	::SendMessage(m_MainHwnd, m_nMessage, 1, code);
}

void CAtDetector::ImageCopy()
{
	
	memcpy(m_pImageBuff, m_ATXrayDetector.atd_Getadress(), sizeof(USHORT) * m_ImageSize.cx * m_ImageSize.cy);
	::SendMessage(m_MainHwnd, m_nMessage, 0, 0);
	//m_ATXrayDetector.atd_Stop();
}

BOOL CAtDetector::Init(DETECTOR_PARA detectorPara, CString strRecipe)
{
	m_This = this;
	SetDetectorPara(detectorPara);

	m_ATXrayDetector.atd_SetCallback(&AtCallBackFrame, &AtCallBackEvent);
	m_ATXrayDetector.atd_Version();
	m_ATXrayDetector.atd_SetUsbIndex(m_DetectorPara.nUsbIndex);
	if (m_ATXrayDetector.atd_isOpen())
	{
		EventProcess(100, "Detector isOpen Error!");
		m_ATXrayDetector.atd_Close();
		Sleep(100);
		if (m_ATXrayDetector.atd_isOpen())
		{
			EventProcess(100, "Detector isOpen Error!");
			m_bConnection = FALSE;
			return FALSE;
		}
		SetDetectorPara(detectorPara);
		m_ATXrayDetector.atd_SetUsbIndex(m_DetectorPara.nUsbIndex);
	}
	
	if (!m_ATXrayDetector.atd_Connect())
	{
		EventProcess(101, "Detector Connect Error!");
		m_bConnection = FALSE;
		return FALSE;
	}
	SetDetectorPara(detectorPara);
	//m_ATXrayDetector.atd_SetScanSpeed(m_DetectorPara.dScanSpeed);

	m_strRecipe.Format(_T("%s"), strRecipe);

	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	CString file_path;
	file_path.Format(_T("%s\\Recipe\\%s\\Cal.txt"), strTemp, m_strRecipe);
	char cFilePath[512];
	sprintf_s(cFilePath, file_path.GetLength() + 1, "%S", file_path.GetBuffer());
	//WideCharToMultiByteChar(file_path, cFilePath);
	m_ATXrayDetector.atd_LoadCalibrationData(cFilePath);

	if (m_pImageBuff != NULL)
		delete[] m_pImageBuff;

	m_pImageBuff = DBG_NEW USHORT[m_ImageSize.cx * m_ImageSize.cy];
	m_bConnection = TRUE;
	return TRUE;
}

void CAtDetector::SetDetectorPara(DETECTOR_PARA detectorPara)
{
	m_DetectorPara = detectorPara; 

	//m_ATXrayDetector.atd_HWND(this->m_MainHwnd);
	//m_ATXrayDetector.atd_SetAutoCalibration(m_DetectorPara.bAutoCalibration);
	//m_ATXrayDetector.atd_SetAverage(m_DetectorPara.nCamAverage);
	//m_ATXrayDetector.atd_SetBinning(m_DetectorPara.nCamBinnig);
	m_ATXrayDetector.atd_SetFrameHeight(m_DetectorPara.lCamHeight);
	m_ATXrayDetector.atd_SetImageFormat(m_DetectorPara.nCamImageFormat);
	m_ATXrayDetector.atd_SetIntegrationTimeRate(m_DetectorPara.nCamIntegrationTime);
	//m_ATXrayDetector.atd_SetLinetoAvg(m_DetectorPara.nCamLineToAvg);
	m_ATXrayDetector.atd_SetMode(m_DetectorPara.nCamMode);
	m_ATXrayDetector.atd_SetObjectDetection((bool)m_DetectorPara.bCamObjectDetection);
	//m_ATXrayDetector.atd_SetObjectDetectionThreshold(m_DetectorPara.nCamObjectDetectionThreshHold);
	//m_ATXrayDetector.atd_SetObjectLength(m_DetectorPara.dObjectLength);
	m_ATXrayDetector.atd_SetPixelSize(m_DetectorPara.nCamPixelSize);
	m_ATXrayDetector.atd_SetScanSpeed((m_DetectorPara.dScanSpeed));// -1));// / 2);
	//m_ATXrayDetector.atd_SetSensitivity(m_DetectorPara.nCamSensitivity);
	//m_ATXrayDetector.atd_SetTotalLength(m_DetectorPara.dTotalLength);
	m_ATXrayDetector.atd_SetUsbIndex(m_DetectorPara.nUsbIndex);
	m_ATXrayDetector.atd_SetWidth(m_DetectorPara.ICamWidth);
	m_ATXrayDetector.atd_SetFlipvertical(!m_DetectorPara.bDirection);
}

BOOL CAtDetector::GrabStart()
{
#ifdef HW_USE
	if (!m_ATXrayDetector.atd_isOpen())
	{
		EventProcess(100, "Detector isOpen Error!");
		return FALSE;
	}

	if (m_ATXrayDetector.atd_isRunning())
	{
		EventProcess(102, "Detector isRunning Error!");
		return FALSE;
	}

	m_ATXrayDetector.atd_SetImageFormat(m_DetectorPara.nCamImageFormat);
	m_ATXrayDetector.atd_SetObjectDetection((bool)m_DetectorPara.bCamObjectDetection);
	//m_ATXrayDetector.atd_SetObjectDetectionThreshold(m_DetectorPara.nCamObjectDetectionThreshHold);

	if (m_ATXrayDetector.atd_Start())
	{
		return TRUE;
	}
#else
	::SendMessage(m_MainHwnd, m_nMessage, 0, 0);
#endif
	return FALSE;
}

BOOL CAtDetector::GrabStop()
{
	BOOL rslt = FALSE;
	if (m_ATXrayDetector.atd_isRunning())
	{
		rslt = m_ATXrayDetector.atd_Stop();
	}
	return rslt;
}

BOOL CAtDetector::SetCal(double dScanSpeed, int nCamLineToAvg)
{
	m_bCalStart = TRUE;
	m_bCalEnd = FALSE;
	
	m_DetectorPara.dScanSpeed = dScanSpeed;
	m_DetectorPara.nCamLineToAvg = nCamLineToAvg;

	m_ATXrayDetector.atd_SetScanSpeed((m_DetectorPara.dScanSpeed));// -1));// / 2);
	m_ATXrayDetector.atd_SetLinetoAvg(m_DetectorPara.nCamLineToAvg);
	if (m_ATXrayDetector.atd_Calibration())
	{
		SetCalDataSave();
		Delay(1000);
		m_bCalEnd = TRUE;
		m_bCalStart = FALSE;
	}

	return m_bCalEnd;
}

BOOL CAtDetector::SetCalDataSave()
{
	char cFilePath[512];
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	CString file_path;
	//file_path.Format(_T("%s\\System\\Cal.txt"), strTemp);
	file_path.Format(_T("%s\\Recipe\\%s\\Cal.txt"), strTemp, m_strRecipe);

	//WideCharToMultiByteChar(file_path, cFilePath);
	sprintf_s(cFilePath, size_t(file_path.GetLength() + 1), "%S", file_path.GetBuffer());
	BOOL isRslt = m_ATXrayDetector.atd_SaveCalibrationData(cFilePath);

	return isRslt;
}

BOOL CAtDetector::SetCalDataLoad()
{
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	CString file_path;
	//file_path.Format(_T("%s\\System\\Cal.txt"), strTemp);
	file_path.Format(_T("%s\\Recipe\\%s\\Cal.txt"), strTemp, m_strRecipe);
	char cFilePath[512];
	sprintf_s(cFilePath, file_path.GetLength() + 1, "%S", file_path.GetBuffer());
	//WideCharToMultiByteChar(file_path, cFilePath);
	BOOL isRslt = m_ATXrayDetector.atd_LoadCalibrationData(cFilePath);

	return isRslt;
}

void CAtDetector::SetScanSpeed(double dScanSpeed)
{
	m_DetectorPara.dScanSpeed = dScanSpeed;
	m_ATXrayDetector.atd_SetScanSpeed((m_DetectorPara.dScanSpeed));// -1));// / 2);
}