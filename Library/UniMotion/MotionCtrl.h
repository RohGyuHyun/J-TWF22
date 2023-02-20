#pragma once
#include "UniMotionBase.h"

#ifdef _MMC_HALF
#include "MMCWHP201.h"
#pragma comment(lib, "MMCWHP230")
#else
#include "MMCWFP39.h"
#pragma comment(lib, "VC60FP39")
#endif

#ifdef _UNIMOTION_DLL
#define UNIMOTION_DLLDECL __declspec(dllexport)
#else
#define UNIMOTION_DLLDECL __declspec(dllimport)

#endif


struct tMotionParam
{
	int Acc;
	int Dec;
	int Speed;
};

class UNIMOTION_DLLDECL CMotionCtrl
{
public:
	enum tMotionErrType{XaMotionOK, XaMotionErr, XAMotionInitErr, XaMotionTimeOut, XaMotionNotExist};
	CMotionCtrl(void);
	~CMotionCtrl(void);
	virtual int StateAndFrameClearAxis(int nAxis);
	virtual int StateAndFrameClearAll(int AxisTotalnum);
    virtual int Roate360(int nAxis,double pos, double velo, int accel,int decel);
	virtual int OffLineCheck(void);
	virtual int Init(void);
	virtual int AbsMove(int axis , double distance, double velocity, double accelerate);
	virtual int AbsMove(int axis , double distance, double velocity, double accelerate, double decelerate);
	virtual int StartAbsMove(int axis , double distance, double velocity, double accelerate);
	virtual int StartAbsMove(int axis , double distance, double velocity, double accelerate, double decelerate);
	virtual int StartRelMove(int axis , double distance, double velocity, double accelerate);
	virtual int RelMove(int axis , double distance, double velocity, double accelerate);
	virtual int RelMove(int axis , double distance, double velocity, double accelerate, double decelerate);
	virtual int StartRelMove(int axis , double distance, double velocity, double accelerate, double decelerate);
	virtual int MotionDone(int axis);
	virtual int SetPosition(int axis, double position);
	virtual double GetPosition(int axis, double * pPosition);
	virtual int SetCommand(int axis, double command);
	virtual int GetCommand(int axis, double * pCommand);
	virtual int GetError(int axis, double * pErr);
	virtual int GoHomeSetZero(int nAxis)=NULL;
	virtual int GoHomeSetZeroNegHome(int nAxis)=NULL;
	virtual int CamDriveRHomeSetZero(int nAxis)=NULL;
	virtual int GoHomePos(int nAxis)=NULL;
	virtual int LeaveHomePos(int nAxis)=NULL;
	virtual int GetHomeSensor(int nAxis)=NULL;
	virtual int GetPosSensor(int nAxis)=NULL;
	virtual int GetNegSensor(int nAxis)=NULL;
	virtual int VeloStop(int axis);
	virtual int EStop(int Axis); 
	virtual int SetNowPosHome(int nAxis);
	virtual int Roate360_VeloMoving(int nAxis, double velo, int Accel);
    virtual int VeloMove(int axis, double velo, int accel);
	virtual int GoNonMultiPositionMove(int nAxis,double pos, double velo, int accel,int decel);
	virtual int SetAmpEnable(int axis, int state);
	virtual int GetAmpEnable(int axis, short *value);// 축의 엠프를 온오프상태를 체크한다
	virtual int SetAmpReset(int axis) ;//축의 엠프를 리셋시킨다. 
	virtual int GetAmpFault(int axis,short* value);// 엠프의 폴트 상태를 표시한다. 
	virtual int AllMotionMoveDone();
	virtual int Wait_For_Done(int nAxis);
	virtual int SetSoftLimtNeg(int nAxis,double pos);
	virtual int SetSoftLimtPos(int nAxis,double pos);
    virtual int MotionMoveDone(int nAxis0 , int nAxis1, int nAxis2, int nAxis3,int nAxis4,int nAxis5,int nAxis6 ,int nAxis7);
    virtual int     Set_electroGear(int nAxis , double ratio);

    virtual double  Get_electroGear(int nAxis);

   	static CMotionCtrl * Instance(void);
	static CMotionCtrl * m_pInstance;
    
	int VMotionStop(int nAxis);
	virtual int MotionStop(int nAxis);
    virtual int GoPosSensor(int nAxis, double velo, int accel);
	virtual int GoPositionMove(int nAxis,double pos, double velo, int accel,int decel);
	virtual int GoMoveAxisVelo(int nAxis, double velo, int Accel);
	virtual int GoNHomeSetZero(int nAxis);
	virtual	int In_MotionCheck(int axis);
	virtual	int In_SequenceCheck(int axis);
	tMotionParam m_HomeMoveParam;
	virtual int Set_Stop_Rate(int axis,int rate);

	static int m_MOTION_AXIS_BASE_X;
	static int m_MOTION_AXIS_BASE_Y;
	static int m_MOTION_AXIS_Z;
	static int m_MOTION_AXIS_R;
	static int m_MOTION_AXIS_T;
	static int m_MOTION_AXIS_STAGE_X;
	static int m_MOTION_AXIS_STAGE_Y;
	static int m_MOTION_AXIS_Convayor;   
	static int m_MOTION_AXIS_CONEBEAM;
	static bool m_Offline;

	
	virtual int ClearStatus(int nAxis);
};
