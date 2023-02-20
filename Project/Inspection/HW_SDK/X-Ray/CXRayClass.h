#pragma once
#include "commthread.h"
#include "TimeCheck.h"
//#include "SerialPort.h"

enum X_RAY_CMD
{
	PROGRAM_KV,
	PROGRAM_MA,
	REQUEST_KV_SEPOINT,
	REQUEST_MA_SEPOINT,
	REQUEST_KV_MONITOR,
	REQUEST_MA_MONITOR,
	REQUEST_FILAMENT_MONITOR,
	TURN_X_RAY_ON_OFF,
	ENABLE_COMM_WATCHDOG,
	TICKLE_COMM_WATCHDOG,
	RESET_FAULTS,
	REQUEST_FAULTS,
	READ_X_RAY_STATUS,
	REQUEST_SOFTWARE_VERSION,
	REQUEST_VOLTAGE_SCALING,
	REQUEST_CURRENT_SCALING,
	REQUEST_MODEL_NUMBER,
	REQUEST_HARDWARE_VERSION,
	REQUEST_SOFTWARE_BULID_VERSION,
	REQUEST_15V_LVPS,
	REQUEST_TANK_TEMPERATURE,
	PROGRAM_BAUD_RATE,
	NON
};

typedef struct X_RAY_STATUS
{
	BOOL bArc;
	BOOL bOverTemperature;
	BOOL bOverVoltage;
	BOOL bUnderVoltage;
	BOOL bOverCurrent;
	BOOL bUnderCurrent;
	BOOL bWatchdogTimeOut;
	BOOL bOpenInterlock;
}X_RAY_STATUS;

typedef struct X_RAY_INFO
{
	X_RAY_STATUS stXray_status;
	char strSWVersion[MAX_CHAR_LENG];
	char strModelNumber[MAX_CHAR_LENG];
	char strHWVersion[MAX_CHAR_LENG];
	char strSWBuildVersion[MAX_CHAR_LENG];
	int nNoScalingSetKV;
	int nNoScalingSetMA;
	int nNoScalingCurKV;
	int nNoScalingCurMA;
	int nFilament;

	double dScalingCurKV;
	double dScalingCurMA;
	
	BOOL isOnOff;
	double dTemp;
}X_RAY_INFO;

class CXRayClass
{
protected:
	CCommThread m_Comport;
	//CSerialPort m_Comport;

	X_RAY_CMD m_SendCommand;
	X_RAY_INFO m_XrayInfo;
	int m_nSendDataValue;

	CWinThread* m_pStatusPollingThread;
	int m_nPollingInterval;
	BOOL m_bThreadEnd;
	BOOL m_bWarmUpState;
	TIME_DATE m_XrayEndTime;
	int m_nSendCmdIndex;

	CTimerCheck m_TimeOutCheck;

	CString m_strPortName;
	DWORD m_dwBaud;

private:
	BOOL ReadCommand(BYTE* pData, int nCnt);
	int ReadData(BYTE* pData, void* nReadData, int nReadCnt = 0, int nReadType = 0);//nReadType = 0 -> int, 1 -> double, 2 -> BOOL Array, 3 -> String // return string count
	BOOL SendCommand(X_RAY_CMD cmd, int nData = -1);
	BYTE Checksums(BYTE* pData, int nCnt = -1);
public:
	CXRayClass();
	~CXRayClass();
	static CXRayClass* Instance();
	static CXRayClass* m_pInstance;

	UINT static ThreadStatusPolling(LPVOID pParam);
	void StatusPollingThread();

	BOOL SetSendCommand(X_RAY_CMD cmd, int nData = -1) { return SendCommand(cmd, nData); };
	BOOL SetReadCommand(BYTE* pData, int nCnt);
	void OpenPort(CString strPortName = _T("COM2"), DWORD dwBaud = 9600, int nReadInterval = 1);
	void Close();

	X_RAY_INFO GetXrayInfo() { return m_XrayInfo; };

	BOOL GetPorConnection() {
#ifdef HW_USE
		return m_Comport.m_bPortOpen;	
#else
		return TRUE;
#endif
	};

	BOOL SetXrayOnOff(BOOL isOnOff);
	BOOL SetXrayVolCur(double dVol, double dCur);
	BOOL SetXrayRestFault();
	void SetWarmupState(BOOL isWarmup) { m_bWarmUpState = isWarmup; };
	BOOL GetWarmupState() { return m_bWarmUpState; };
	void SetXrayEndTime(TIME_DATE cEndTime) { m_XrayEndTime = cEndTime; };
	TIME_DATE GetXrayEndTime() { return m_XrayEndTime; };
#ifndef HW_USE
	void SetTestXRayStatus(int nIdx, BOOL bVal);
#endif
	BOOL GetXrayWarmupCheck();
};

