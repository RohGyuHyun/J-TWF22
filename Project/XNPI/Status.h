#pragma once
#include "..\Library\SerialLib\SerialMFC.h"	// �ʼ�
#include "..\Library\SerialLib\XFMSPacket.h"	// �ʼ�
#include <UniUtility.h>
class CMilVLib;
class CModelData;
class CSCAlgoBase;
class CRepository;
struct tInfoCamIng
{
	int nCamNo;
	CdblPoint posImage; //Mouse point�� ����Ű�� ���� �̹�������(Zoom�����������) ��ǥ
	int GrayValue;      //Mouse point��ġ�� Gray��
};

class CStatus
{
public:
	enum tOpMode { OPAUTO, OPTEACHING, OPSTOP };
	enum tInspType { INSPFID, INSPHEDET, INSPALIGN, INSPHEIGHT, INSPGAPGAGE };
	enum tDispMode { DISP_AUTO, DISP_MANUAL };
	CStatus();
	~CStatus();
	static CStatus * Instance();
	static CStatus *m_pInstance;
	void SetOPMode(tOpMode opMode);
	int GetOpMode(void);
	void SetSelCam(int nCam);
	int m_iSelCam;
	int GetSelCamNo();
	CString GetModelRepository();
	void SetModelRepository(TCHAR * szDataSaveFolder);
	CString m_strModelRepository;
	CString m_strModelName;
	CString GetModelName();
	void SetModelName(TCHAR * szModelName);
	int SaveCFG();
	int LoadCFG();
	long m_nCountOfCam;
	CString GetComportNo();
	void SetBaudRate(int iBaudRate);
	int GetBaudRate();
	void SetComportNo(TCHAR* szComportNo);
	long m_iBaudRate;
	CString m_strComportNo;
	long m_LightBrightness[4];
	CXFMSPacket * GetComm();
	CXFMSPacket *m_pSCInspPacket;
	void SetComm(CXFMSPacket * pSCInspPacket);
	int GetRVOption();
	CModelData * GetModelData();
	CModelData *m_pModelData;
	void SetModelData(CModelData * pModelData);
	void SetCurAddAlgoIndex(int nInspType);
	int m_nInspTypeToAdd;
	int GetCurAlgoToAddIndex();
	CSCAlgoBase * GetCurAlgoToAdd();
	CSCAlgoBase * GetAlgoInfo(int nInspType, CXmlNode* pNode = NULL);
	void SetAlgoInfo(int nInspType, CSCAlgoBase *pAlgo, CXmlNode* pNode);
	BOOL m_bIsOffLine;
	CString m_ProductName;
	tInfoCamIng m_infoCamMousePos;
	int m_opMode;
	CString m_strIMGSaveFolder;
	BOOL m_MsgWndOpen;
	int GetCamNo();
	long m_nCountOfSensors;
	BOOL m_bSaveTestImage;
	BOOL m_bDrawCross;
	long m_bUseDelOldImg;
	long m_nDaysDel;
	CdblSize GetPixelCalData(int iCamIndex);
	long m_lIPAddr[4];
	CString GetTempSensorIPString();
	CString m_strRemoteImgName;
	long m_lTemperature[6];
	long m_ulGraphXRange;
	long m_ulGraphYMax;
	long m_ulGraphYMin;
	long m_ulDispMode;
	CString m_strAVISaveFolder;
	int GetAutoMarkPos();
	void SetAutoMarkPos(int nPos);
	void IncAutoMarkPos();
	int m_nAutoMarkPos;
	CString m_strMarkToAdd;
	void SetMarkString(CString strToAdd);
	CString GetMarkString();
	CRepository *m_pRepository;
	CRepository * GetRepository();
	void SetRepository(CRepository * pRepository);
	long m_bUseImgEnhance;
	BOOL m_bIsAVIRecording;
	CString m_strSensorNamePx[6];
	long m_wlvLow;
	long m_wlvHigh;

	//kiy plc ���ü���
	CString m_strPlcIp;
	CString m_strPlcPort;
	long m_ulPlcReadCycleDelay_ms;

	//kiy 190327 - XNPIView - CMD_INVAILD_INTERVAL_MS ��� �� ����, PLC�κ��� �о�ö��� �����ð�,
	//�� ��ġ�� ������ PLC���� Ŀ�ǵ带 1�ʰ� �����Ҷ� �ش� Ŀ�ǵ带 �ѹ��� �ν����� �ʰ� ���������� �ν��Ҽ� �ִ�.
	//���� 0�� �̹����� �ٸ� ������ ����ȴٴ°� ����, Start Ŀ�ǵ带 plc���� 1�ʰ� �����Ҷ�, 2������ �ν��ϴ°����� �����ȴ�.
	long m_ulPlcReadCMDHoldingTime;

	//kiy wndLev ���ü���
	long m_wlnLowPresetArr[5];
	long m_wlnHighPresetArr[5];

	//kiy Image save ���� ���� ����
	long m_bImgSaveFormatJPG;
	long m_bImgSaveFormatTIF;

	//kiy Info ���ü���
	CString m_strComp;
	CString m_strSEPC;
	CString m_strSize;
	CString m_strSerial;
	CString m_strPipeNo;
	CString m_strInfoLine1;		//181019 line1,2 ��û���� �߰�
	CString m_strInfoLine2;	    


	long m_avg;

	//kiy Avg Min Limit - avg �ּҰ� ���� 0906
	long m_avgMinLimit;
	//kiy - ���������� ������ �������(�߰��� �������� ���� �˻� �̾ �ϱ����ؼ�) 0911
	CString m_strLastOpendFolder;

	void SaveOnlyPipeInfoCFG();
	void SaveLastOpendFolder();

	//kiy CamDlg���� Region���� ��Ⱚ �������� ���� ���� 0919
	long m_gvAvgRegionWidth;
	long m_gvAvgRegionHeight;

	//kiy - 0919
	long m_bSaveWithRaw;	//�˻� �̹��� ����ÿ� Raw���ϵ� ���� ���� �� ���ΰ�
	long m_bShowGrayAvg;	//�������� ���콺 �÷����� ������� �� ������ ��� ��հ� ǥ��

	//kiy - 1016, �ȼ� ������ �߰� Tiff�±� XResolution, YResolution�� ���� ����
	double m_dPixelXSize_mm;	//XPixelSize
	double m_dPixelYSize_mm;	//YPixelSize

	//kiy - 1016
	double m_pipeStartPos_mm;			//������ ���� ��ġ
	double m_pipeSectEachDistance_mm;	//������ �� ���� ���� �Ÿ�
};

