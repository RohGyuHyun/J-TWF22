#pragma once

#define ATDETECTOR_USE
#ifdef ATDETECTOR_USE
#include "ATXrayDetector.h"
#pragma comment(lib, "ATXrayDetector.lib")
#pragma comment(lib, "FTD3XX.lib")

#endif
class ATXrayDetector;
class CAtDetector
{
protected:
	ATXrayDetector m_ATXrayDetector;
	USHORT* m_pImageBuff;
	DETECTOR_PARA m_DetectorPara;

	HWND m_MainHwnd;
	int m_nMessage;

	CString m_strEvent;
	CSize m_ImageSize;
	BOOL m_bConnection;
	BOOL m_bCalStart;
	BOOL m_bCalEnd;
	CString m_strRecipe;
private:
	

	
public:
	CAtDetector();
	~CAtDetector();

	CString GetEventString() { return m_strEvent; };
	USHORT* GetImageData() { return m_pImageBuff; };

	static CAtDetector* Instance();
	static CAtDetector* m_pInstance;

	void SetMessage(HWND hWnd, int nMessage) { m_MainHwnd = hWnd; m_nMessage = nMessage; }
	void SetDetectorPara(DETECTOR_PARA detectorPara);// { m_DetectorPara = detectorPara; };
	void ImageCopy();
	void EventProcess(int code, const char* event);
	BOOL Init(DETECTOR_PARA detectorPara, CString strRecipe);
	USHORT* GetImageBuff() { return m_pImageBuff; };
	BOOL GrabStart();
	BOOL GrabStop();
	void SetImageSize(int nWidth, int nHeight) { m_ImageSize.SetSize(nWidth, nHeight); };
	int GetImageType() { return m_DetectorPara.nCamImageFormat; };
	BOOL GetConnection() {
#ifdef HW_USE
		return m_bConnection;
#else
		return TRUE;;
#endif
	};
	BOOL SetCal(double dScanSpeed, int nCamLineToAvg);
	BOOL Close();
	double GetScanSpeed() { return m_DetectorPara.dScanSpeed; };
	BOOL GetCalStart() { return m_bCalStart; };
	BOOL GetCalEnd() { return m_bCalEnd; };
	BOOL SetCalDataSave();
	BOOL SetCalDataLoad();
	void SetScanSpeed(double dScanSpeed);
};