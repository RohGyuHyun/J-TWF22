#pragma once

#include <UniUtility.h>

/** 
\brief 
�ϵ����� ���� ������ȸ(�Ϲ������� Config �� Setting�� ����) �� ������ Ȱ���Ѵ�.@n
\date 2014.06.27
\author �� ����
*/
class CMachineInfo
{
public:
	enum tOpMode{OPAUTO, OPTEACHING, OPSTOP};
	CMachineInfo(void);
	~CMachineInfo(void);
	static CMachineInfo* Instance(void);
	static CMachineInfo* m_pMachineInfo;
	void SetOPMode(tOpMode opMode);

	CCfgFile *pMyCfg;

	
	int GetOpMode(void);
	int m_opMode;
	int IsOnline(void);
	void SetOnline(bool currStatus);
	int m_bIsOnline;
    
	static  int m_XrayPort ;
	static	int m_TriggerPort ;
	static	int m_BarCodePort ;
	static BOOL m_bAutoReverse;
	static BOOL m_bSaveResult;
	static BOOL m_bStopOnDefect;
	static BOOL m_bSilent;
	static BOOL m_bUseLaser;
	static BOOL m_bDrawCross;
	static BOOL m_bImgEnhance;   //ImageEnhance���
	static BOOL m_bRuler;
	static int m_iAvg;
	static int m_iAvgSoft;
	static int m_Filter;
	static tBNState m_BrightNormalizeState;
	static BOOL m_bWarpCal;
	static BOOL m_bScrnNormalize;
	static BOOL m_EnableHFillter;
	static BOOL m_SetVerticalPriority;
	static int m_moduleNum;
	static int m_frameNum;
	static long m_iExpostime;
	static BOOL m_bBarcode;
	static BOOL m_bNGImageSave ;
	static BOOL m_bGoodImageSave;
	static BOOL m_bCompoImageSave;
	static BOOL m_bInfomationViewer;
	static CString m_PIPCAM_NAME; // pip cam �̸�
	static int m_PIPCAMNUM;
	static int m_ImageMargin_V;   // �̹��� ���� �� 
	static int m_ImageMargin_H;   // �̹��� ���� �� 

	static CString m_ModelName;   //�������� ����
	static CString m_BarCodeNUM;  //PCB ���ڵ� �ѹ� ����
	
	static CString m_stXraytube_type;      // ���� ����ϰ� �ִ� �������� Ʃ������ ���� 
	static CString m_stOIborad_type;      // ���� ����ϰ� �ִ� IO ����
	static CString m_stMotion_type;      // ���� ����ϰ� �ִ�  ��Ǻ��� 
    static CString m_LengDir ;           // ���� ����ϰ� �ִ� ���������ϰ�� 
	static CString m_ProductName;        // ���� ��ġ�� ����
	static double m_MahcineFID;         // ���� ����� FID ��
    
	static double m_Angle_Per_One_Pulse ;          //���޽��� ƿƮ���� 
	static double m_mm_Per_pulseZ;                 //���޽��� z ���� �̵��Ÿ� mm
	static double m_Limited_ZPos_Max;              //z ���� ����Ʈ (h100)��
	static double m_Limited_ZPos_Min;
	static double m_RotateZMin0DgressFor2D;
	static double m_RoRateZMaxODgreesFor2D;
	static double m_RateCalValeFor2dZeroDgres;
	
     
    static double m_bControllerXrayType; ///> �������� ��Ʈ�ѷ� Ÿ��
	static double E_GearSX; ///> ���� ���� X ; 


	static int m_bTiltAxisEnable; // ƿƮ �� ��� �Ұ��� �Է�
	static double m_iBreaktypeZ; // ���� �극��ũ Ÿ���� z �� 
    static double m_RotateCenterforview_BaseX;     // OCT �߰������߽��� ȭ���� �߽��̵Ǵ� ���̽���ǥ
	static double m_RotateCenterForview_BaseY;
	static double m_ZeroAngleForTilt;              // T ����� ��������Ȩ��
    static double m_Tanx;
	static double m_RotateCenterforviewORG_BaseX;
    static double m_RotateCenterForviewORG_BaseY;
    static double m_ZeroAngleForTiltORG;
	static double m_AxisRTotalPulse ;// R ����1ȸ���޽� 
	static double m_RlimitPosMin;// R���� -����
	static double m_RlimitPosMax;// r���� +����
	static double m_AxisR_ZeroPos; //r���� 0�� ��ġ 
	static CSize m_CameraPicxel ; // ī�޶��� �ȼ� ���� Ex) �ٽ����� 1000x1000 
	static double m_Offset_3d_MeasurePoint;  
	static double m_DetectorPixelSize;
	static double m_bDDMotorEnable	;
	/////////// �������� x-y ���� ���� ���� ��/////
	static double m_Limit_Stage_X_Pos;
	static double m_Limit_Stage_X_Neg;
	static double m_Limit_Stage_Y_Pos;
	static double m_Limit_Stage_Y_Neg;
	///////////////////////////////////////////////////
	static double m_lfLimit_T_Neg;
	static double m_lfLimit_T_Pos;

	///CT �̹��� ���� ��� 
	static CString m_stCTdataDir;
	static CString m_st3dPC_CTDir;
	static CString m_CTDATA_Save_Mode;
	static CString m_RamDrive_CTDir;
	//////////////// 
	static double m_NGBufferCounter;
	static double m_LOADERCounter;
	///// �ε� �ټ� �� ���ۿ� ���ִ� NG ī�����ϴ� �Ѱ踦 �������ִ� ���� �߰� ������ 12 06 21 
     /// ���ǵ� ���� 

	static double m_SpeedRate ;
	static double m_SpeedRate2; 
	static double m_SpeedRate3 ;
    

	static double m_AccDccRate  ;
	static double m_AccDccRate2 ;
	static double m_AccDccRate3;
	static int m_OverKillRecheck;
	static int m_AlgoTimeover;
	static BOOL m_bRVShowDim;
 ///////////////////////////////////////

	/////////////// �ε� ��ε� ��ġ �������� 
	static double LoadPosX_Stage;
	static double LoadPosY_Stage;
	static double LoadPosX_Base;
	static double LoadPosY_Base;
	static double LoadPosZ;
	static double LoadPosT;
	static double LoadPosR;

    static double unLoadPosX_Stage;
	static double unLoadPosY_Stage;
	static double unLoadPosX_Base;
	static double unLoadPosY_Base;
	static double unLoadPosZ;
	static double unLoadPosT;
	static double unLoadPosR;
	/////////////////////////////////

	/////////////// Setup â ����
    static BOOL m_bEnableInlineSoftavr;
	BOOL GetAutoReverse();
	BOOL GetSaveResult();
	BOOL GetStopDefect();
	BOOL GetSilentMode();
	BOOL GetUseLaser();
	BOOL GetDrawCross();
	BOOL GetImageEnhance();
	tBNState GetBrightNormalize();
	BOOL GetRuler();
	CString GetModelName();
	void SetModelName(TCHAR * nameModel);
	int GetAverage();
	int GetAverageSoft();
	int GetExposTime();
	int GetFilter();
	int GetScrnNormalize();
	int GetWarpCal();
	void SaveCFG(void);
	void LoadCFG(void);
	void CamerainfoLoad();
	CString GetBarcodeNum(void);
	////////////////////////////////////

	////////////////CBT ��� ����///////////
    static double m_CBTLimitPosStageX;
	static double m_CBTLimitPosStageY;
	static double m_CBTLimitPosStageZ;
	static double m_CBTLimitPosStageR;

	static double m_CBTLimitnNegStageX;
	static double m_CBTLimitnNegStageY;
	static double m_CBTLimitnNegStageZ;
	static double m_CBTLimitnNegStageR;

	static double m_CBTStartDefualtStageXpos;
	static double m_CBTStartDefualtStageYpos;
	static double m_CBTStartDefualtStageRpos;

    static double m_CBTStartStartStageXpos;
	static double m_CBTStartStartStageYpos;
	static double m_CBTStartStartStageRpos;

	static double m_CBTAxisRMotionPulse;
	static double m_ConBeam5AxisEnable;
	///////////////////////////////////////


	///////////////h160 ƿƮ z �� ���� �� ���� �� ///////////
	 static double m_30DgressZNegPosLimitForTilt;    //30�� �ɼ�
	 static double m_35DgressZNegPosLimitForTilt;    //35�� �ɼ�
	 static double m_40DgressZNegPosLimitForTilt;    //40�� �ɼ�
	 static double m_45DgressZNegPosLimitForTilt;    //45�� �ɼ�
	 static double m_50DgressZNegPosLimitForTilt;    //50�� �ɼ�
	 static double m_55DgressZNegPosLimitForTilt;    //55�� �ɼ�
	 static double m_60DgressZNegPosLimitForTilt;    //60�� �ɼ�

	 unsigned long GetRVOption(void);
	static double m_FrameMoveStageXSpeed;
	static double m_FrameMoveStageYSpeed;
	static double m_FrameMoveZSpeed;
	static double m_FrameMoveTSpeed;
	static double m_FrameMoveORSpeed;
	static double m_FrameMoveCBTCONVSpeed;
	static double m_DETECTORSKEWANGLE;

	//////////////////////////9800 ������Ÿ�� ���̺� �� ����/////
	static double m_Limit_ConvayerStage_X_Pos;
	static double m_Limit_ConvayerStage_Y_Pos;
	static double m_Limit_ConvayerStage_X_Neg;
    static double m_Limit_ConvayerStage_Y_Neg;
	static double m_Limit_ConvayerStage_Z_Pos;
	static double m_Limit_ConvayerStage_Z_Neg;

	////////////////
	
	///////////////////////////////////////////
};
