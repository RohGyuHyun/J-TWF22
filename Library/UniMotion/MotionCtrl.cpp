#include "StdAfx.h"
#include "MotionCtrl.h"
#include "MMCMotionCtrl.h"

 int CMotionCtrl::m_MOTION_AXIS_BASE_X= 99;
 int CMotionCtrl::m_MOTION_AXIS_BASE_Y= 99;
 int CMotionCtrl::m_MOTION_AXIS_Z= 99;
 int CMotionCtrl::m_MOTION_AXIS_R= 99;
 int CMotionCtrl::m_MOTION_AXIS_T= 99;
 int CMotionCtrl::m_MOTION_AXIS_STAGE_X= 99;
 int CMotionCtrl::m_MOTION_AXIS_STAGE_Y= 99;
 int CMotionCtrl::m_MOTION_AXIS_Convayor= 99;   
 int CMotionCtrl::m_MOTION_AXIS_CONEBEAM= 99;
 bool CMotionCtrl::m_Offline=0;

CMotionCtrl * CMotionCtrl::m_pInstance=NULL;

CMotionCtrl::CMotionCtrl(void)
{
}

CMotionCtrl::~CMotionCtrl(void)
{
}

CMotionCtrl * CMotionCtrl::Instance(void)
{
	if(m_pInstance==NULL) m_pInstance = new CMMCMotionCtrl;
	return m_pInstance;
}

int CMotionCtrl::StateAndFrameClearAxis(int nAxis)
{
	return 0;
}
int CMotionCtrl::StateAndFrameClearAll(int AxisTotalnum)
{
  return 0;
}

int CMotionCtrl::OffLineCheck(void)
{
  return 0;
}
int CMotionCtrl::Init(void)
{
	return NULL;
}

int CMotionCtrl::AbsMove(int axis , double distance, double velocity, double accelerate)
{
	return 0;
}

int CMotionCtrl::AbsMove(int axis , double distance, double velocity, double accelerate, double decelerate)
{
	return 0;
}

int CMotionCtrl::StartAbsMove(int axis , double distance, double velocity, double accelerate)
{
	return 0;
}

int CMotionCtrl::StartAbsMove(int axis , double distance, double velocity, double accelerate, double decelerate)
{
	return 0;
}

int CMotionCtrl::StartRelMove(int axis , double distance, double velocity, double accelerate)
{
	return 0;
}

int CMotionCtrl::RelMove(int axis , double distance, double velocity, double accelerate)
{
	return 0;
}

int CMotionCtrl::RelMove(int axis , double distance, double velocity, double accelerate, double decelerate)
{
	return 0;
}

int CMotionCtrl::StartRelMove(int axis , double distance, double velocity, double accelerate, double decelerate)
{
	return 0;
}



int CMotionCtrl::MotionDone(int axis)
{
	return 0;
}

int CMotionCtrl::SetPosition(int axis, double position)
{
	return 0;
}

double CMotionCtrl::GetPosition(int axis, double * pPosition)
{
	
	return 0;
}

int CMotionCtrl::SetCommand(int axis, double command)
{
	return 0;
}

int CMotionCtrl::GetCommand(int axis, double * pCommand)
{
	return 0;
}

int CMotionCtrl::GetError(int axis, double * pErr)
{
	return 0;
}

int CMotionCtrl::MotionStop(int nAxis)
{
	return 0;
}
 int MotionStop(int nAxis)
 {
	 return 0;
 }


 int CMotionCtrl::GoPosSensor(int nAxis, double velo, int accel)
 {
   return 0;
 }

 int CMotionCtrl::GoPositionMove(int nAxis,double pos, double velo, int accel,int decel)
 {
	 return 0;
 }


int CMotionCtrl::GoMoveAxisVelo(int nAxis, double velo, int Accel)
{
	return 0;

}

int CMotionCtrl::VMotionStop(int nAxis)
{
    return 0;
}

int CMotionCtrl::GoNHomeSetZero(int nAxis)
{
    return 0;
}

int CMotionCtrl::CamDriveRHomeSetZero(int nAxis)
{
 return 0;
}

int CMotionCtrl::GoHomeSetZeroNegHome(int nAxis)
{
    return 0;
}

int CMotionCtrl::GetHomeSensor(int nAxis)
{
   return 0;
}

int CMotionCtrl::GetNegSensor(int nAxis)
{
   return 0;
}

int CMotionCtrl::GetPosSensor(int nAxis)
{
return 0 ;
}
int CMotionCtrl::VeloStop(int axis)
{return 0;}

int CMotionCtrl::EStop(int Axis)
{
	return 0;
}


int CMotionCtrl::VeloMove(int axis, double velo, int accel)
{
return 0;
}

int CMotionCtrl::Roate360(int nAxis,double pos, double velo, int accel,int decel)

{
return 0;
}

int CMotionCtrl::SetNowPosHome(int nAxis)
{
	return 0;
}

int  CMotionCtrl::Roate360_VeloMoving(int nAxis, double velo, int Accel)
{
	return 0;
}
int CMotionCtrl::GoNonMultiPositionMove(int nAxis,double pos, double velo, int accel,int decel)
{
  return 0;
}

int CMotionCtrl::SetAmpEnable(int axis, int state)
{
   return 0;
}
int CMotionCtrl::GetAmpFault(int axis,short* value)// 엠프의 폴트 상태를 표시한다. 
{
	return 0;
}
int CMotionCtrl::GetAmpEnable(int axis, short *value)// 축의 엠프를 온오프상태를 체크한다
{
 return 0;
}
int CMotionCtrl::SetAmpReset(int axis)//축의 엠프를 리셋시킨다. 
{
	return 0;
}

int CMotionCtrl::AllMotionMoveDone()
{
   return 0;
}

int CMotionCtrl::In_MotionCheck(int axis)
{
  return 0;
}

int CMotionCtrl::In_SequenceCheck(int axis)
{
  return 0;
}
int CMotionCtrl::Set_Stop_Rate(int axis,int rate)
{
  return 0;
}



int CMotionCtrl::Set_electroGear(int nAxis , double ratio)
{
  return 0;
}

 double  CMotionCtrl::Get_electroGear(int nAxis)
 {
   
	 return 0;
 
 }


 int CMotionCtrl::SetSoftLimtNeg(int nAxis,double pos)
 {
   return 0;
 }
 int CMotionCtrl::SetSoftLimtPos(int nAxis,double pos)
 {
   return 0;
 }


 int CMotionCtrl::MotionMoveDone(int nAxis0 , int nAxis1, int nAxis2 ,int nAxis3,int nAxis4,int nAxis5,int nAxis6 ,int nAxis7)
 {
 
   return 0;
 }


 int CMotionCtrl::Wait_For_Done(int nAxis)
 {
 
  return 0;
 }

 int CMotionCtrl::ClearStatus(int nAxis)
 {
	 return 0;
 }
