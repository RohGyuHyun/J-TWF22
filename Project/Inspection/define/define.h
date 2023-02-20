#pragma once
#include "ProfileDataIF.h"
#include "NghUtil.h"

/**
  @file     define.h
  @brief    define header File
  @author   노규현

  message, timer, struct define 파일.\n
  */



#define X_RAY_KV_OFFSET										0.5
#define X_RAY_MA_OFFSET										0.05

  /**
	  @brief MESSAGE
	  각종 Message define
  */
#define TEACHING_TO_MAIN_MESSAGE							WM_USER + 100
#define DETECTOR_TO_MAIN_MESSAGE							WM_USER + 101
#define USE_MSG_IMAGE_VIEW_TO_TEACHING_DIALOG				WM_USER + 102
#define USE_MSG_DETECTOR_TO_MAIN							WM_USER + 103
#define USE_MSG_INSPECTION_END								WM_USER + 104
#define USE_MSG_ALARM_SEQUENCE_TO_MAIN						WM_USER + 105
#define USE_MSG_IMAGE_VIEW_TO_MAIN_DIALOG					WM_USER + 106
#define USE_MSG_LOG_TO_MAIN									WM_USER + 107
#define USE_MSG_LOG_TO_INSPECTION							WM_USER + 108
#define USE_MSG_LOG_WRITE									WM_USER + 109
#define USE_MSG_IMAGE_VIEW_TO_TEACHING_BLOBS_DIALOG			WM_USER + 110
#define USE_MSG_IMAGE_VIEW_TO_MASKING_BLOBS_DIALOG			WM_USER + 111
#define USE_MSG_LOG_READ_END_AALARM_LOG_VIEW_DIALOG			WM_USER + 113

  /**
	  @brief Pattern Color
	  각 Pattern 별 Color define
  */
#define PATTERN_0_RESULT_COLOR			RGB(0, 255, 0)
#define PATTERN_1_RESULT_COLOR			RGB(0, 255, 255)
#define PATTERN_2_RESULT_COLOR			RGB(255, 255, 0)
#define PATTERN_3_RESULT_COLOR			RGB(255, 0, 255)
#define PATTERN_4_RESULT_COLOR			RGB(100, 255, 100)

  /**
	  @brief Blobs Color
	  Blobs Result Color define
  */
#define BLOBS_RESULT_COLOR				RGB(255, 0, 0)


#define MAX_RECT_CNT										20

  /**
	  @brief Timer
	  각 Timer 별 define
  */
#define TIMER_SHOW_MAIN_DIALOG					0
#define TIMER_CURRENT_TIME						10
#define TIMER_X_RAY_ON_OFF						11
#define TIMER_X_RAY_INFO						12
#define TIMER_HW_CONNECTION_CHECK				13
#define TIMER_HIDE_RECIPE_DLG_CHEC				14
#define TIMER_STOP_MODE_INTERLOCK_CHECK			15
#define TIMER_ALARM_SHOW						16
#define TIMER_INVERTER_STATUS					17
#define TIMER_DETECTOR_CAL_END_CHECK			18
#define TIMER_INVERTER_INFO						19
#define TIMER_IN_OUT_SENSER_UNLOCK				20
#define TIMER_NG_BUZZER							21
#define TIMER_INVERTER_RECONNECT				22
#define TIMER_INVERTER_RECONNECT2				23
#define TIMER_ALARM_BUZZER						24
#define TIMER_IN_SENSER_X_RAY_OFF				25
#define TIMER_OUT_SENSER_X_RAY_OFF				26

#define DEFAULT_IMAGE_WIDTH								1126
#define DEFAULT_IMAGE_HEIGHT							1024

#define IMAGE_CUT_OFFSET						150

  /**
	  @brief Input, Output Count
	  in, output Max Count
  */
#define MAX_INPUT_NUM							16
#define MAX_OUTPUT_NUM							16


#define MAX_CHAR_LENG							256

#define MAX_RSLT_POINT_CNT						2
#define MAX_PATTERN_ADD_CNT						5

const CString ADMIN_PW_STRING					= _T("ADMIN");
const CString MANUFACTURE_PW_STRING				= _T("MANU");
const CString USER_PW_STRING					= _T("USER");
const CString SYSTEM_FILE_VERSION				= _T("ver_20220719_0");
#ifdef CE_CONFIRM
const CString IO_STR_PARA_FILENAME				= _T("DIOLabel_CE.ini");
#else
const CString IO_STR_PARA_FILENAME				= _T("DIOLabel.ini");
#endif
const CString SYSTEM_STR_PARA_FILENAME			= _T("System.ini");
const CString RECIPE_STR_PARA_FILENAME			= _T("Recipe.ini");
const CString RESULT_DATA__CSV_FILENAME			= _T("ResultData.csv");
const CString LOG_FILE_PATH						= _T("LOG\\");


/**
	@brief Alarm Message String
	Alarm Message String define
*/
const CString ALARM_MSG_EMG_INTER_LOCK					= _T("AlarmCode:01, EMG Inter Lock");
const CString ALARM_MSG_KEY_SW_INTER_LOCK				= _T("AlarmCode:02, Key SW Inter Lock");
const CString ALARM_MSG_DOOR_INTER_LOCK					= _T("AlarmCode:03, Door Open Inter Lock");
const CString ALARM_MSG_INVERTER_ALARM_INTER_LOCK		= _T("AlarmCode:04, Inverter Alarm Inter Lock");
const CString ALARM_MSG_TUBE_ALARM_INTER_LOCK			= _T("AlarmCode:05, Tube Alarm Inter Lock");
const CString ALARM_MSG_X_RAY_LAMP_ALARM_INTER_LOCK		= _T("AlarmCode:06, X_Ray Lamp Alarm Inter Lock");
const CString ALARM_MSG_IN_SENSER_INTER_LOCK			= _T("AlarmCode:07, In Senser Inter Lock");
const CString ALARM_MSG_OUT_SENSER_INTER_LOCK			= _T("AlarmCode:08, Out Senser Inter Lock");

const CString FIND_ALARM_LOG_STRING						= _T("Inter Lock");

const CString RESULT_CSV_FILE_LABEL_NAME = _T("Date,Total_Result,1_Ptn_Result,1_Ptn_Teach_Cnt,1_Ptn_Insp_Cnt,1_Ptn_Over_Cnt,2_Ptn_Result,2_Ptn_Teach_Cnt,2_Ptn_Insp_Cnt,2_Ptn_Over_Cnt,3_Ptn_Result,3_Ptn_Teach_Cnt,3_Ptn_Insp_Cnt,3_Ptn_Over_Cnt,4_Ptn_Result,4_Ptn_Teach_Cnt,4_Ptn_Insp_Cnt,4_Ptn_Over_Cnt,5_Ptn_Teach_Cnt,5_Ptn_Result,5_Ptn_Insp_Cnt,5_Ptn_Over_Cnt");
const CString BLOBS_RESULT_CSV_FILE_LABEL_NAME = _T("Index,CenX,CenY,Angle,ShortSize(Pixel),LongSize(Pixel),Lenght(Pixel),ShortSize(mm),LongSize(mm),Lenght(mm)");
#define TEST_IMAGE_LOAD

#ifdef TEST_IMAGE_LOAD
const CString TEST_IMAGE_FILE_PATH = _T("D:\\OneDrive\\ngh\\VRI-I410\\test\\");
#endif

/**
	@brief LOGIN_TYPE
	LOGIN_USER, LOGIN_ADMIN define\n
	user, admin mode로 x-ray 제어 가능 유무(on,off제외 출력 변화)
*/
enum LOGIN_TYPE
{
	LOGIN_USER,
	LOGIN_MANUFACTURE,
	LOGIN_ADMIN
};

/**
	@brief IO_PARA struct
	io 라벨 및 번호
*/
typedef struct IO_PARA
{
	wchar_t strIOLabel[MAX_CHAR_LENG];
	int nIONum;
}IO_PARA;

/**
	@brief EACH_RESULT_DATA struct
	검사 결과 개별 정보\n
	score, center posX,Y, center avg val, overlap
*/
typedef struct EACH_RESULT_DATA
{
	double dScore;
	double dCenPosX;
	double dCenPosY;
	double dCenter5x5AvgVal;
	BOOL isOverlap;
}EACH_RESULT_DATA;

/**
	@brief RESULT_DATA struct
	전체 검사 결과 정보\n
	전체 ok,ng, 수량 ok,ng, 겹침 ok,ng, 개별 결과 정보 struct
*/
typedef struct RESULT_DATA
{
	BOOL isAllResult;
	BOOL isCountResult;
	BOOL isOverlapResult;
	int	nOverlapCnt;
	vector<EACH_RESULT_DATA> stEachResultData;
}RESULT_DATA;

/**
	@brief DETECTOR_PARA struct
	detector 연결 parameter
*/
typedef struct DETECTOR_PARA
{
	int nUsbIndex;
	double dScanSpeed;
	long ICamWidth;
	long lCamHeight;
	int nCamPixelSize;
	int nCamBinnig;
	int nCamMode;
	BOOL bCamObjectDetection;
	int nCamObjectDetectionThreshHold;
	int nCamIntegrationTime;
	int nCamSensitivity;
	int nCamImageFormat;
	int nCamAverage;
	int nCamLineToAvg;
	wchar_t strCamCalFileName[MAX_CHAR_LENG];
	BOOL bAutoCalibration;
	double dTotalLength;
	double dObjectLength;
	BOOL bDirection;
}DETECTOR_PARA;

/**
	@brief X_RAY_PARA struct
	x-ray 연결 parameter
*/
typedef struct X_RAY_PARA
{
	int nPortIndex;
	int nPortBaudrate;
}X_RAY_PARA;

/**
	@brief INVERTOR_PARA struct
	INVERTOR 연결 parameter
*/
typedef struct INVERTER_PARA
{
	int nPortIndex;
	int nPortBaudrate;
	double dSpeed;
	int nDirection;
}INVERTER_PARA;

/**
	@brief REJECTER_PARA struct
	REJECTER parameter
*/
typedef struct REJECTER_PARA
{
	BOOL bUse;
	int nDelayTime;
	int nEndDelayTime;
}REJECTER_PARA;


/**
	@brief SYSTEM_PARA struct
	system parameter\n
	x-ray, detector, inverter, image save, image size
*/
typedef struct SYSTEM_PARA
{
	X_RAY_PARA		stXRay;
	DETECTOR_PARA	stDetector;
	INVERTER_PARA	stInverter;
	REJECTER_PARA	stRejecter;
	wchar_t			strSveImageDirectory[MAX_CHAR_LENG];
	BOOL			bSaveInspectionRawImage;
	BOOL			bSaveResultOKImage;
	BOOL			bSaveResultNGImage;
	CSize			ImageSize;
	int				nInspectionType;//0 : Pattern, 1 : Blobs
	int				nInSensingTime;
	int				nOutSensingTime;
	int				nHDDSpaceAlarmPer;
	double dPixelSizeX;
	double dPixelSizeY;
}SYSTEM_PARA;

/**
	@brief INSPEC_PARA struct
	inspection parameter\n
	검사에 사용 되는 x-ray kv,ma, 검사 parameter
*/
typedef struct INSPEC_PARA
{
	double dVoltage;
	double dCurrent;
	CRect nPatternAddRect;
	double dSmoothness;
	double dActiveEdge;
	
	BOOL bInspUse;
	int nSearchPatternCnt;
	double dSearchScore;
	int nSearchPatternThreshHold;
	int nSearchPatternOverlapThreshHold;
}INSPEC_PARA;

/**
	@brief RECIPE_PARA struct
	recipe parameter
*/
typedef struct RECIPE_PARA
{
	int nIndex;
	wchar_t strRecipeName[MAX_CHAR_LENG];
}RECIPE_PARA;


//////////////////////////////////////////////////////////////
//20220921 ngh 식품 이물검사 용 parameter

/**
	@brief SEARCH_AREA_PARA struct
	search area 검출 parameter
*/
typedef struct SEARCH_AREA_PARA
{
	BOOL bAutoSearchArea;
	int nThreshHold;
	double dShortLenght;
	double dLongLenght;
	int nAreaVal;
	int nBlobOpenClose;
}SEARCH_AREA_PARA;

/**
	@brief BLOB_PREPROCESS_PARA struct
	이물 검사 preprocess parameter
*/
typedef struct BLOB_PREPROCESS_PARA
{
	BOOL bPreProcess;
	int nRank;
	float fGain;
	int nOffset;
	int nIlluminateWH;
	double dIlluminateFactor;
}BLOB_PREPROCESS_PARA;

/**
	@brief MASKING_PARA struct
	이물 검사 parameter
*/

typedef struct MASKING_PARA
{
	BOOL isMasking;
	double dCenX;
	double dCenY;
	double dAngle;
}MASKING_PARA;

/**
	@brief BLOB_INSPEC_PARA struct
	이물 검사 parameter
*/
typedef struct BLOB_INSPEC_PARA
{
	SEARCH_AREA_PARA stSearchAreaPara;
	BLOB_PREPROCESS_PARA stPreProcess;
	MASKING_PARA stMaskingPara;

	//PreProcess 사용 안할시에 사용
	//////////////////////////////
	int nBlobMinThreshHold;
	int nBlobMaxThreshHold;
	//////////////////////////////

	double dBlobMinArea;
	double dBlobMaxArea;
	double dBlobMinLenght;
	double dBlobMaxLenght;
	int nBlobNearbyVal;

	double dVoltage;
	double dCurrent;

	BOOL isScale;
	int nScaleMin;
	int nScaleMax;
}BLOB_INSPEC_PARA;

typedef struct BLOB_RESULT_PARA
{
	double dCenX;
	double dCenY;
	double dShortSize;
	double dLongSize;
	double dLength;
	double dAngle;
}BLOB_RESULT_PARA;
