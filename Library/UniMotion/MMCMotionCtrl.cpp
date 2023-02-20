#include "StdAfx.h"
#include "MMCMotionCtrl.h"
#include <UniUtility.h>
BOOL alreadCheck_MachinName = false;

// 축 공용변수 

double OfflineModeBaseXpos=0;
double OfflineModeBaseYpos=0;
double OfflineModeStageXpos=0;
double OfflineModeStageYpos=0;
double OfflineModeZpos=0;
double OfflineModeTpos=0;
double OfflineModeORpos=0;
double OfflineModeCRpos=0;


CString ProductName;

CMMCMotionCtrl::CMMCMotionCtrl(void)
{
}

CMMCMotionCtrl::~CMMCMotionCtrl(void)
{
}


int CMMCMotionCtrl::StateAndFrameClearAll(int AxisTotalnum)
{
    for(int i=0; i<AxisTotalnum ; i++)
	{
	ClearStatus(i);
	Sleep(100);
    amp_fault_reset(i);
	Sleep(100);
	frames_clear(i);
	Sleep(100);
	}
	
	return 0;
}

int CMMCMotionCtrl::StateAndFrameClearAxis(int nAxis)
{
    
	ClearStatus(nAxis);
	Sleep(50);
    amp_fault_reset(nAxis);
	Sleep(50);
	frames_clear(nAxis);
	Sleep(50);
	
	
	return 0;
}


int CMMCMotionCtrl::OffLineCheck(void)

{
	if(Init() != CMotionCtrl::XaMotionOK)		//
	{	CMotionCtrl::m_Offline=true;	}													//
	else																//		
	{	CMotionCtrl::m_Offline=false;	
	}

	return 0;
}

int CMMCMotionCtrl::Init(void)
{

	long addr[4] = {0xd8000000, 0xd8400000, 0xd8800000, 0xd8c00000};
	int err = mmc_initx(1, addr);
	if(err!=MMC_OK)
	{
		if(err==MMC_TIMEOUT_ERR){
			set_dpram_addr(0, 0xd8000000);
		}

		if(!alreadCheck_MachinName)
		{
			CCfgFile* pMyCfg=CCfgFile::Instance();
			pMyCfg->Open(); 
			pMyCfg->SetDepth(2);

			ProductName = pMyCfg->Get(_T("None"),_T("ConfigSet"),_T("ProductName"));

			CMotionCtrl::m_Offline=true;
			AxisNumeInit();
			alreadCheck_MachinName=true;
		}// 축설정을 먼저 봐준다 
		return err;                                                                                                                                                       
	}

	SetAmpEnable(0, TRUE);
	SetAmpEnable(1, TRUE);
	SetAmpEnable(2, TRUE);
	SetAmpEnable(3, TRUE);
	SetAmpEnable(4, TRUE);
	SetAmpEnable(5, TRUE);
	SetAmpEnable(6, TRUE);
	SetAmpEnable(7, TRUE);

	if(!alreadCheck_MachinName)
	{
		CCfgFile* pMyCfg=CCfgFile::Instance();
		pMyCfg->Open(); 
		pMyCfg->SetDepth(2);

		ProductName = pMyCfg->Get(_T("None"),_T("ConfigSet"),_T("ProductName"));

		CMotionCtrl::m_Offline=false;
		AxisNumeInit();
		alreadCheck_MachinName=true;
	}// 축설정을 먼저 봐준다 
	return XaMotionOK;
}


int CMMCMotionCtrl::AxisNumeInit()
{


	if(ProductName==_T("H100"))
	{
		CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_X = 0;
		CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_Y = 1;
		CMotionCtrl::Instance()->m_MOTION_AXIS_Z = 2;
		CMotionCtrl::Instance()->m_MOTION_AXIS_R = 3;//dd 모터용 지우지마세요

		// CamDrive, 축넘버 7// 임시 
		// DD 일결우에는 3 번이다.
		//CMotionCtrl::Instance()->m_MOTION_AXIS_R = 7;//
		// 지우지마세요

		CMotionCtrl::Instance()->m_MOTION_AXIS_T = 4;
		CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X =5;
		CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y =6;
		CMotionCtrl::Instance()->m_MOTION_AXIS_Convayor=99;  
		if(!CMotionCtrl::m_Offline)StateAndFrameClearAll(8);
	}

	else if(ProductName==_T("H160"))
	{
		CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_X = 0;
		CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_Y = 1;
		CMotionCtrl::Instance()->m_MOTION_AXIS_Z = 2;
		// CamDrive, 축넘버 3
		CMotionCtrl::Instance()->m_MOTION_AXIS_R = 3;
		///
		CMotionCtrl::Instance()->m_MOTION_AXIS_T = 4;
		CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X =5;
		CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y =6;
		CMotionCtrl::Instance()->m_MOTION_AXIS_CONEBEAM=7;
		if(!CMotionCtrl::m_Offline)StateAndFrameClearAll(8);
	}

	else if(ProductName==_T("Star9800A"))
	{
		CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_X = 99;
		CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_Y = 99;
		CMotionCtrl::Instance()->m_MOTION_AXIS_Z = 2;
		CMotionCtrl::Instance()->m_MOTION_AXIS_R = 99;
		CMotionCtrl::Instance()->m_MOTION_AXIS_T = 3;
		CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X =0;
		CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y =1;
		CMotionCtrl::Instance()->m_MOTION_AXIS_Convayor=4;  
		if(!CMotionCtrl::m_Offline)StateAndFrameClearAll(8);

	}


	else if(ProductName==_T("Xscan7000") || ProductName == _T("XscanA100"))
	{
		CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_X = 99;
		CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_Y = 99;
		CMotionCtrl::Instance()->m_MOTION_AXIS_Z = 2;
		CMotionCtrl::Instance()->m_MOTION_AXIS_R = 99;
		CMotionCtrl::Instance()->m_MOTION_AXIS_T = 3;
		CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X =0;
		CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y =1;
		CMotionCtrl::Instance()->m_MOTION_AXIS_Convayor=99; 
		if(!CMotionCtrl::m_Offline)StateAndFrameClearAll(4);

	}
	/*
	//Star9800L 축정의 
	#define AXIS_DCENT_Z 0
	#define AXIS_DSIDE_X 1
	#define AXIS_DSIDE_Z 2
	#define AXIS_DSIDE_R 3

	// Star9800A 축정의 
	#define MOTION_AXIS_X 0
	#define MOTION_AXIS_Y 1
	#define MOTION_AXIS_Z 2
	#define MOTION_AXIS_T 3
	#define MOTION_AXIS_Conv 4
	#define MOTION_AXIS_R 5

	// H100 축정의 


	#define MOTION_AXIS_H100_X 0
	#define MOTION_AXIS_H100_Y 1
	#define MOTION_AXIS_H100_Z 2
	#define MOTION_AXIS_H100_R 3
	#define MOTION_AXIS_H100_T 4
	#define MOTION_AXIS_H100_STAGE_X 5
	#define MOTION_AXIS_H100_STAGE_Y 6
	*/
	return 0;
}

int CMMCMotionCtrl::AbsMove(int axis , double distance, double velocity, double accelerate)
{   
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return move(axis, distance, velocity, (short)accelerate);
}

int CMMCMotionCtrl::AbsMove(int axis , double distance, double velocity, double accelerate, double decelerate)
{
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return t_move(axis, distance, velocity, (short)accelerate, (short)decelerate);
}

int CMMCMotionCtrl::StartAbsMove(int axis , double distance, double velocity, double accelerate)
{
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return start_move(axis, distance, velocity, (short)accelerate);
}

int CMMCMotionCtrl::StartAbsMove(int axis , double distance, double velocity, double accelerate, double decelerate)
{
	
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return start_t_move(axis, distance, velocity, (short)accelerate, (short)decelerate);
}

int CMMCMotionCtrl::StartRelMove(int axis , double distance, double velocity, double accelerate)
{
	
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return start_r_move(axis, distance, velocity, (short)accelerate);
}

int CMMCMotionCtrl::RelMove(int axis , double distance, double velocity, double accelerate)
{
	
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return r_move(axis, distance, velocity, (short)accelerate);
}

int CMMCMotionCtrl::RelMove(int axis , double distance, double velocity, double accelerate, double decelerate)
{
	
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return tr_move(axis, distance, velocity, (short)accelerate, (short)decelerate);
}

int CMMCMotionCtrl::StartRelMove(int axis , double distance, double velocity, double accelerate, double decelerate)
{
	if(axis==99)return 0;
	if(CMotionCtrl::m_Offline)return 0;
	else return start_tr_move(axis, distance, velocity, (short)accelerate, (short)decelerate);
}

int CMMCMotionCtrl::MotionDone(int axis)
{
	
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return motion_done(axis);
}

int CMMCMotionCtrl::SetPosition(int axis, double position)
{
	
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return set_position(axis, position);
}

double CMMCMotionCtrl::GetPosition(int axis, double * pPosition)
{
	if(axis==99)return 0;
	if(CMotionCtrl::m_Offline)
	{
		
		double pos=0;
			switch(axis)
			{
			case 0:
				pos=OfflineModeBaseXpos;
				break;
			case 1:
			   pos=OfflineModeBaseYpos;
				break;
			case 2:
				pos=OfflineModeZpos;
				break;
			case 3:
				pos=OfflineModeORpos;
				break;
			case 4:
				pos=OfflineModeTpos;
				break;
				
			case 5:
				pos=OfflineModeStageXpos;
				break;
				
			case 6:
				pos=OfflineModeStageYpos;
				break;
				
			case 7:
				pos=OfflineModeCRpos;
				break;
			}
		
	
		return pos;

	}
	
	else return get_command(axis, pPosition);
}

int CMMCMotionCtrl::SetCommand(int axis, double command)
{
	
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return set_command(axis, command);
}

int CMMCMotionCtrl::GetCommand(int axis, double * pCommand)
{
//	return get_command(axis, pCommand);
	if(axis==99)return 0;
	return 0;
}

int CMMCMotionCtrl::GetError(int axis, double * pErr)
{
	
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return get_error(axis, pErr);
}

int CMMCMotionCtrl::SetAmpEnable(int axis, int state)
{
	
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return set_amp_enable(axis, state);
}
int CMMCMotionCtrl::GetAmpFault(int axis,short* value)// 엠프의 폴트 상태를 표시한다. 
{
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return get_amp_fault_level(axis, value);
	return 0;
}
int CMMCMotionCtrl::GetAmpEnable(int axis, short *value)// 축의 엠프를 온오프상태를 체크한다
{
	
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else return get_amp_enable(axis, value);
	
}
int CMMCMotionCtrl::SetAmpReset(int axis)//축의 엠프를 리셋시킨다. 
{   
	
	if(CMotionCtrl::m_Offline)return 0;
	if(axis==99)return 0;
	else 
	{
		amp_fault_reset(axis);
		Sleep(10);
		amp_fault_set(axis);
	}
	return 0;
	
}

int CMMCMotionCtrl::GetHomeSensor(int nAxis)
{
	
	if(CMotionCtrl::m_Offline)return 0;
	if(nAxis==99)return 0;
	else return home_switch(nAxis);
}

int CMMCMotionCtrl::GetPosSensor(int nAxis)
{
	
	if(CMotionCtrl::m_Offline)return 0;
	if(nAxis==99)return 0;
	else return pos_switch(nAxis);
}

int CMMCMotionCtrl::GetNegSensor(int nAxis)
{
	
	if(CMotionCtrl::m_Offline)return 0;
	if(nAxis==99)return 0;
	else return neg_switch(nAxis);
}

int CMMCMotionCtrl::VeloMove(int axis, double velo, int accel)

{   
	if(axis==99)return 0;
       
    ClearStatus(axis);
	Sleep(50);
	//ClearStatus(axis);
    //amp_fault_reset(axis);
	//ClearStatus(axis);
	//amp_fault_reset(axis);
	frames_clear(axis);

	
	if(CMotionCtrl::m_Offline)return 0;
	else 
	{
		
		//v_move(axis, velo, accel);
		if(axis == CMotionCtrl::m_MOTION_AXIS_Z && (ProductName==_T("Xscan7000") || ProductName == _T("XscanA100")))
				{v_move(axis, -1*velo, accel);}
				else
				{v_move(axis, velo, accel);}
	return 0; 
	}
}

int CMMCMotionCtrl::EStop(int axis)
{
	
    if(axis==99)return 0;
	if(CMotionCtrl::m_Offline)return 0;
	else  return set_e_stop(axis);
   

}



int CMMCMotionCtrl::VeloStop(int axis)
{
    if(axis==99)return 0;
	if(CMotionCtrl::m_Offline)return 0;
	else  return v_move_stop(axis);
}

int CMMCMotionCtrl::Set_Stop_Rate(int axis,int rate)
{   
	if(axis==99)return 0;

	set_stop_rate(axis,rate);
	set_stop(axis);
	while(!axis_done(axis));
	clear_status(axis);

    return 0;
}

int CMMCMotionCtrl::GoNHomeSetZero(int nAxis)
{    
	if(nAxis==99)return 0;

	if(CMotionCtrl::m_Offline)return 0;
	else {
		StateAndFrameClearAxis(nAxis);
		frames_clear(nAxis);
		v_move(nAxis, -10000.0, 20);
		while(!GetNegSensor(nAxis));
		MotionStop(nAxis);
		while(!MotionDone(nAxis));
		StateAndFrameClearAxis(nAxis);
		SetPosition(nAxis, 0);
		SetCommand(nAxis, 0);

		AbsMove(nAxis,10000,3000, 20);

		frames_clear(nAxis);
		v_move(nAxis, -1000.0, 20);
		while(!GetNegSensor(nAxis));
		MotionStop(nAxis);
		while(!MotionDone(nAxis));
		StateAndFrameClearAxis(nAxis);
		SetPosition(nAxis, 0);
		SetCommand(nAxis, 0);
		return 0;}
}


int CMMCMotionCtrl::CamDriveRHomeSetZero(int nAxis)
{
    if(nAxis==99)return 0;
	if(CMotionCtrl::m_Offline)return 0;
	else 
	{
				StateAndFrameClearAxis(nAxis);
				ClearStatus(nAxis);
				Sleep(50);
				v_move(nAxis,+8000.0, 20);
				while(!GetHomeSensor(nAxis));
				MotionStop(nAxis);
				while(!MotionDone(nAxis));
				ClearStatus(nAxis);
				Sleep(50);
				SetPosition(nAxis, 0);
				SetCommand(nAxis, 0);
				
	}
	return 0;
}

int CMMCMotionCtrl::GoHomeSetZeroNegHome(int nAxis)
{
	if(nAxis==99)return 0;


	if(ProductName==_T("Star9800A"))
	{
		if(CMotionCtrl::m_Offline)return 0;
		else 
		{
			StateAndFrameClearAxis(nAxis);
			frames_clear(nAxis);
			AbsMove(nAxis,10000,50000, 800);
			frames_clear(nAxis);
			v_move(nAxis, -50000.0, 800);
			while(!GetHomeSensor(nAxis));
			MotionStop(nAxis);
			while(!MotionDone(nAxis));
			StateAndFrameClearAxis(nAxis);
			SetPosition(nAxis, 0);
			SetCommand(nAxis, 0);

			AbsMove(nAxis,10000,50000, 800);

			frames_clear(nAxis);
			v_move(nAxis, -50000.0, 20);
			while(!GetHomeSensor(nAxis));
			MotionStop(nAxis);
			while(!MotionDone(nAxis));
			StateAndFrameClearAxis(nAxis);
			SetPosition(nAxis, 0);
			SetCommand(nAxis, 0);


			return 0;}
	}


	else if(ProductName==_T("H100") || ProductName==_T("H160") || ProductName==_T("Xscan7000") || ProductName == _T("XscanA100"))
		//H100용 
	{
		if(CMotionCtrl::m_Offline)return 0;
		else 
		{


			if(nAxis == CMotionCtrl::m_MOTION_AXIS_Z && ProductName==_T("Xscan7000") || ProductName == _T("XscanA100"))
			{

				if(GetPosSensor(nAxis))

				{      
					StateAndFrameClearAxis(nAxis);
					frames_clear(nAxis);
					v_move(nAxis, -1000.0, 800);
					while(1)
					{if(GetPosSensor(nAxis)==0)
					{MotionStop(nAxis);break;}
					};
					while(!MotionDone(nAxis));
					StateAndFrameClearAxis(nAxis);
					SetPosition(nAxis, 0);
					SetCommand(nAxis, 0);


				}

				else{
					StateAndFrameClearAxis(nAxis);
					frames_clear(nAxis);
					//AbsMove(nAxis,10000,50000, 20);
					v_move(nAxis, +15000.0, 800);
					while(!GetPosSensor(nAxis));
					MotionStop(nAxis);
					while(!MotionDone(nAxis));
					StateAndFrameClearAxis(nAxis);
					SetPosition(nAxis, 0);
					SetCommand(nAxis, 0);
					frames_clear(nAxis);
					v_move(nAxis, -1000.0, 800);
					while(1)
					{
						if(GetPosSensor(nAxis)==0)
						{MotionStop(nAxis);break;}
					}
					while(!MotionDone(nAxis));
					StateAndFrameClearAxis(nAxis);
					SetPosition(nAxis, 0);
					SetCommand(nAxis, 0);

				}



			}
			else
			{

				if(GetNegSensor(nAxis))

				{      
					StateAndFrameClearAxis(nAxis);
					frames_clear(nAxis);
					v_move(nAxis, +1000.0, 800);
					while(1)
					{if(GetNegSensor(nAxis)==0)
					{MotionStop(nAxis);break;}
					};
					while(!MotionDone(nAxis));
					StateAndFrameClearAxis(nAxis);
					SetPosition(nAxis, 0);
					SetCommand(nAxis, 0);


				}

				else{
					StateAndFrameClearAxis(nAxis);
					frames_clear(nAxis);
					//AbsMove(nAxis,10000,50000, 20);
					v_move(nAxis, -15000.0, 800);
					while(!GetNegSensor(nAxis));
					MotionStop(nAxis);
					while(!MotionDone(nAxis));
					StateAndFrameClearAxis(nAxis);
					SetPosition(nAxis, 0);
					SetCommand(nAxis, 0);
					frames_clear(nAxis);
					v_move(nAxis, +1000.0, 800);
					while(1)
					{
						if(GetNegSensor(nAxis)==0)
						{MotionStop(nAxis);break;}
					}
					while(!MotionDone(nAxis));
					StateAndFrameClearAxis(nAxis);
					SetPosition(nAxis, 0);
					SetCommand(nAxis, 0);

				}
			}

		}



		return 0;}
	///////////

	else{return 0;}
}

int CMMCMotionCtrl::GoHomeSetZero(int nAxis)
{
	// CString ProductName;

	if(nAxis==99)return 0;

	if(ProductName==_T("Star9800A")|| ProductName==_T("Xscan7000")  || ProductName == _T("XscanA100"))
	{
		// 9800L용
		if(CMotionCtrl::m_Offline)return 0;
		else 
		{StateAndFrameClearAxis(nAxis);
		frames_clear(nAxis);
		//AbsMove(nAxis,10000,50000, 20);
		v_move(nAxis, -20000.0, 20);
		while(!GetHomeSensor(nAxis));
		MotionStop(nAxis);
		while(!MotionDone(nAxis));
		StateAndFrameClearAxis(nAxis);
		SetPosition(nAxis, 0);
		SetCommand(nAxis, 0);

		AbsMove(nAxis,10000,10000, 20);

		frames_clear(nAxis);
		v_move(nAxis, -1000.0, 20);
		while(!GetHomeSensor(nAxis));
		MotionStop(nAxis);
		while(!MotionDone(nAxis));
		StateAndFrameClearAxis(nAxis);
		SetPosition(nAxis, 0);
		SetCommand(nAxis, 0);

		return 0;
		}
	}


	else if(ProductName==_T("H100") || ProductName==_T("H160"))
		//H100용 
	{
		if(CMotionCtrl::m_Offline)return 0;
		else 
		{

			if(GetNegSensor(nAxis))

			{      
				StateAndFrameClearAxis(nAxis);
				frames_clear(nAxis);
				v_move(nAxis, +1000.0, 20);
				while(1)
				{if(GetNegSensor(nAxis)==0)
				{MotionStop(nAxis);break;}
				};
				while(!MotionDone(nAxis));
				StateAndFrameClearAxis(nAxis);
				SetPosition(nAxis, 0);
				SetCommand(nAxis, 0);


			}

			else{
				StateAndFrameClearAxis(nAxis);
				frames_clear(nAxis);
				//AbsMove(nAxis,10000,50000, 20);
				v_move(nAxis, -5000.0, 20);
				while(!GetNegSensor(nAxis));
				MotionStop(nAxis);
				while(!MotionDone(nAxis));
				StateAndFrameClearAxis(nAxis);
				SetPosition(nAxis, 0);
				SetCommand(nAxis, 0);
				frames_clear(nAxis);
				v_move(nAxis, +1000.0, 20);
				while(1)
				{
					if(GetNegSensor(nAxis)==0)
					{MotionStop(nAxis);break;}
				}
				while(!MotionDone(nAxis));
				StateAndFrameClearAxis(nAxis);
				SetPosition(nAxis, 0);
				SetCommand(nAxis, 0);

			}

		}



		return 0;}

	else{return 0;}
}


int CMMCMotionCtrl::GoHomePos(int nAxis)
{  
    if(nAxis==99)return 0;
	if(nAxis==99)return 0;

	if(CMotionCtrl::m_Offline)return 0;
	else 
	{StateAndFrameClearAxis(nAxis);
	frames_clear(nAxis);
	set_home(0,STOP_EVENT);
	v_move(nAxis, m_HomeMoveParam.Speed, m_HomeMoveParam.Acc);
	while(!MotionDone(nAxis));
	MotionStop(nAxis);
	set_home(0, NO_EVENT); 
	StateAndFrameClearAxis(nAxis);
	
	if(!GetHomeSensor(nAxis))
	{
		set_home(0,STOP_EVENT);
		v_move(nAxis, -m_HomeMoveParam.Speed/5,m_HomeMoveParam.Acc/5);
		while(!MotionDone(nAxis));
		MotionStop(nAxis);
		set_home(0, NO_EVENT); 
		StateAndFrameClearAxis(nAxis);
	}
	return 0;}
}

int CMMCMotionCtrl::LeaveHomePos(int nAxis)
{
	if(nAxis==99)return 0;
	if(CMotionCtrl::m_Offline)return 0;
	else 
	{StateAndFrameClearAxis(nAxis);
	frames_clear(nAxis);
	v_move(nAxis, 1000, 10);
	while(GetHomeSensor(nAxis));
	MotionStop(nAxis);
	while(!MotionDone(nAxis));
	StateAndFrameClearAxis(nAxis);
	return 0;}
}

int CMMCMotionCtrl::MotionStop(int nAxis)
{
	if(nAxis==99)return 0;
 
	if(CMotionCtrl::m_Offline)return 0;
	else 
	{
	StateAndFrameClearAxis(nAxis);
	frames_clear(nAxis);
	
	return set_e_stop(nAxis);
	}
}
int CMMCMotionCtrl::VMotionStop(int nAxis)
{     	
	if(nAxis==99)return 0;

	if(CMotionCtrl::m_Offline)return 0;
	else return v_move_stop(nAxis);
}
int CMMCMotionCtrl::ClearStatus(int nAxis)
{   
	if(nAxis==99)return 0;
	
	if(CMotionCtrl::m_Offline)return 0;
	if(nAxis==99)return 0;
	else return clear_status(nAxis);
}

int CMMCMotionCtrl::GoHomeSetZeroXAxis(int nAxis)

{  

	if(nAxis==99)return 0;
	
	if(CMotionCtrl::m_Offline)return 0;
	else 
	{StateAndFrameClearAxis(nAxis);
	frames_clear(nAxis);
	v_move(nAxis, 40000.0, 20);
	while(!GetHomeSensor(nAxis));
	MotionStop(nAxis);
	while(!MotionDone(nAxis));
	StateAndFrameClearAxis(nAxis);
	SetPosition(nAxis, 0);
	SetCommand(nAxis, 0);
	
	AbsMove(nAxis, -20000, 40000, 20);

	frames_clear(nAxis);
	v_move(nAxis, 6000.0, 20);
	while(!GetHomeSensor(nAxis));
	MotionStop(nAxis);
	while(!MotionDone(nAxis));
	StateAndFrameClearAxis(nAxis);
	SetPosition(nAxis, 0);
	SetCommand(nAxis, 0);
	return 0;}
}

int CMMCMotionCtrl::GoHomeSetZeroRAxis(int nAxis)
{   
	if(nAxis==99)return 0;
	if(CMotionCtrl::m_Offline)return 0;
	else {
	StateAndFrameClearAxis(nAxis);
	//frames_clear(nAxis);
	v_move(nAxis, -10000.0, 20);

	while(!GetNegSensor(nAxis));
	StateAndFrameClearAxis(nAxis);
	SetPosition(nAxis, 0);
	SetCommand(nAxis, 0);

	v_move(nAxis, 10000.0, 20);
	while(!GetHomeSensor(nAxis));
	MotionStop(nAxis);
	while(!MotionDone(nAxis));
	StateAndFrameClearAxis(nAxis);
	SetPosition(nAxis, 0);
	SetCommand(nAxis, 0);
	
	AbsMove(nAxis, -20000, 10000, 20);

	frames_clear(nAxis);
	v_move(nAxis, 6000.0, 20);
	while(!GetHomeSensor(nAxis));
	MotionStop(nAxis);
	while(!MotionDone(nAxis));
	StateAndFrameClearAxis(nAxis);
	SetPosition(nAxis, 0);
	SetCommand(nAxis, 0);
	return 0;}
}


int CMMCMotionCtrl::GoPositionMove(int nAxis,double pos, double velo, int accel,int decel)
{   
	if(nAxis==99)return 0;

	if(CMotionCtrl::m_Offline)

	{
		switch(nAxis)
		{
		case 0:
			OfflineModeBaseXpos=pos;
			break;
		case 1:
			OfflineModeBaseYpos=pos;
			break;
		case 2:
			break;
			OfflineModeZpos=pos;
		case 3:
			OfflineModeORpos=pos;
			break;
		case 4:
			OfflineModeTpos=pos;
			break;

		case 5:
			OfflineModeStageXpos=pos;
			break;

		case 6:
			OfflineModeStageYpos=pos;
			break;

		case 7:
			OfflineModeCRpos=pos;
			break;
		}


		return 0;
	}
	else {
		//ClearStatus(nAxis);
		//amp_fault_reset(nAxis);
		ClearStatus(nAxis);
		Sleep(50);
		amp_fault_reset(nAxis);
		frames_clear(nAxis);
		if(nAxis == CMotionCtrl::m_MOTION_AXIS_Z && (ProductName==_T("Xscan7000") || ProductName == _T("XscanA100")))
		{start_t_move(nAxis,(-1)*pos,velo,accel,decel);}
		else
		{start_t_move(nAxis,pos,velo,accel,decel);}



		return 0;}
}

int CMMCMotionCtrl::GoNonMultiPositionMove(int nAxis,double pos, double velo, int accel,int decel)
{  

	if(nAxis==99)return 0;
	if(CMotionCtrl::m_Offline)
	{
		switch(nAxis)
		{
		case 0:
			OfflineModeBaseXpos=pos;
			break;
		case 1:
			OfflineModeBaseYpos=pos;
			break;
		case 2:
			break;
			OfflineModeZpos=pos;
		case 3:
			OfflineModeORpos=pos;
			break;
		case 4:
			OfflineModeTpos=pos;
			break;

		case 5:
			OfflineModeStageXpos=pos;
			break;

		case 6:
			OfflineModeStageYpos=pos;
			break;

		case 7:
			OfflineModeCRpos=pos;
			break;
		}

		return 0;
	}
	else 
	{
		//ClearStatus(nAxis);
		//amp_fault_reset(nAxis);
		ClearStatus(nAxis);
		Sleep(50);
		frames_clear(nAxis);
		if(nAxis == CMotionCtrl::m_MOTION_AXIS_Z && (ProductName==_T("Xscan7000") || ProductName == _T("XscanA100")))
		{t_move(nAxis,(-1)*pos,velo,accel,decel);}
		else
		{t_move(nAxis,pos,velo,accel,decel);}

	}

	return 0;
}



int CMMCMotionCtrl::GoPosSensor(int nAxis, double velo, int accel)

{   

	if(nAxis==99)return 0;
	
	if(CMotionCtrl::m_Offline)return 0;
	else {
    ClearStatus(nAxis);
	Sleep(50);
	//amp_fault_reset(nAxis);
	//ClearStatus(nAxis);
	//amp_fault_reset(nAxis);
	frames_clear(nAxis);
	v_move(nAxis,velo, accel);
	while(!GetPosSensor(nAxis));
	VMotionStop(nAxis);
	//ClearStatus(nAxis);
	
	return 0;}
}


int CMMCMotionCtrl::GoMoveAxisVelo(int nAxis,double velo,int Accel)
{    


	if(nAxis==99)return 0;

	if(CMotionCtrl::m_Offline)return 0;
	else {
	double pos;
    GetPosition(nAxis,&pos);

		if(pos<0)
		{
			VeloStop(nAxis);
			Sleep(500);
		 GoNonMultiPositionMove(nAxis,0,5000,40,40);
		}

		else{
			if(velo!=0)
			{
				//ClearStatus(nAxis);
				//amp_fault_reset(nAxis);
				//ClearStatus(nAxis);
				//amp_fault_reset(nAxis);
				frames_clear(nAxis);
				//if(pos<=15000&&velo<0){velo=-5000;}
				
				//if(pos<=10000&&velo>0){velo=5000;}

				//v_move(nAxis,velo, Accel);
				
				if(nAxis == CMotionCtrl::m_MOTION_AXIS_Z && (ProductName==_T("Xscan7000")  || ProductName == _T("XscanA100")))
				{v_move(nAxis,(-1)*velo, Accel);}
				else
				{v_move(nAxis,velo, Accel);}
  
				
				//ClearStatus(nAxis);
			}
	
			else {MotionStop(nAxis);	
			}
	
	
		}

		return 0;}
}



int CMMCMotionCtrl::Roate360_VeloMoving(int nAxis,double velo,int Accel)
{    
	if(nAxis==99)return 0;
	if(CMotionCtrl::m_Offline)return 0;
	else {
			
		                  	
			if(velo!=0)
			{
				//ClearStatus(nAxis);
				frames_clear(nAxis);
								
				

				v_move(nAxis,velo, Accel);
				//ClearStatus(nAxis);
			}
	
			else {
				  v_move_stop(nAxis);
			     
			     }	
	}
	
		


	return 0;

}



int CMMCMotionCtrl::Roate360(int nAxis,double pos, double velo, int accel,int decel)
{  
	if(nAxis==99)return 0;
	if(CMotionCtrl::m_Offline)return 0;
	else 
	{			//ClearStatus(nAxis);
				//amp_fault_reset(nAxis);
				//ClearStatus(nAxis);
				//amp_fault_reset(nAxis);
				frames_clear(nAxis);
	            t_move(nAxis,pos,velo,accel,decel);
				MotionStop(nAxis);
	}

return 0;
}

int CMMCMotionCtrl::SetNowPosHome(int nAxis)
{   

    
	if(nAxis==99)return 0;
	if(CMotionCtrl::m_Offline)return 0;
	else {
	while(!MotionDone(nAxis));
	//ClearStatus(nAxis);
	SetPosition(nAxis, 0);
	SetCommand(nAxis, 0);

	return 0;}
}


void CMMCMotionCtrl::InitAxisNum(void)
{

}


int CMMCMotionCtrl::In_MotionCheck(int axis)
{  if(axis==99)return 0;
	if(CMotionCtrl::m_Offline)return 0;
	else {  return in_motion(axis);}
}

int CMMCMotionCtrl::In_SequenceCheck(int axis)
{ 
	if(axis==99)return 0;
	if(CMotionCtrl::m_Offline)return 0;
	else {  return in_sequence(axis);}
}


int CMMCMotionCtrl::AllMotionMoveDone()
{  

	int MotionDone=0; // 모션이 돌고 있을때 1, 완전히 정지 했을때 0;
	BOOL m_Stage_X,m_Stage_Y,m_Base_X,m_Base_Y,m_ZAixs,m_RAxis,m_TAxis;

	if(CMotionCtrl::m_Offline)return 0;
	else
	{
		while(1)
		{

			if(ProductName==_T("H100") || ProductName==_T("H160"))
			{    

				if(In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_X) && In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_X))
				{ m_Base_X =true;   }
				else { m_Base_X=false; }

				if(In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_Y) && In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_Y))
				{ m_Base_Y =true;   }
				else { m_Base_Y=false; }

				if(In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_Z) && In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_Z))
				{ m_ZAixs =true;   }
				else { m_ZAixs=false; }

				if(In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_R) && In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_R))
				{ m_RAxis =true;   }
				else { m_RAxis=false; }

				if(In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X) && In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X))
				{ m_Stage_X =true;   }
				else { m_Stage_X=false; }

				if(In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y) && In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y))
				{ m_Stage_Y =true;   }
				else { m_Stage_Y=false; }

				if(In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_T) && In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_T))
				{ m_TAxis =true;   }
				else { m_TAxis=false; }

				if(!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_X) 
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_Y) 
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_Z) 
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_R) 
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_T)
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X) 
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y)
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_CONEBEAM)

					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_X) 
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_BASE_Y) 
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_Z) 
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_R) 
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_T)
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X) 
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y)
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_CONEBEAM)
					){MotionDone=0;break;}
				else{MotionDone=1;break; }


		 }

			else if(ProductName==_T("Star9800A"))
			{    
				if(!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_Z)
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_T)
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X)
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y)
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_Convayor)
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_Z)
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_T)
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X)
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y)
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_Convayor)
					){MotionDone=0;break;}
				else{MotionDone=1;break;}
		 }


			else if(ProductName == _T("Xscan7000") || ProductName == _T("XscanA100"))
			{

				if(!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_Z)
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_T)
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X)
					&&!In_MotionCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y)
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_Z)
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_T)
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_X)
					&&!In_SequenceCheck(CMotionCtrl::Instance()->m_MOTION_AXIS_STAGE_Y)
					){MotionDone=0;break;}
				else{MotionDone=1;break;}


		 }
		}


		return MotionDone;

	}

}

	
int CMMCMotionCtrl::Set_electroGear(int nAxis , double ratio)
{   
	if(nAxis==99)return 0;
    set_electric_gear(nAxis,ratio);   // 전자 기어비셋팅 

	return 0;
}

double  CMMCMotionCtrl::Get_electroGear(int nAxis)
{  
	if(nAxis==99)return 0;
       double pratio;
	   get_electric_gear(nAxis,&pratio);   // 전자 기어비셋팅 
	   return pratio;

}
	    

	    

int CMMCMotionCtrl::SetSoftLimtNeg(int nAxis,double pos)
{   
	if(nAxis==99)return 0;
    set_negative_sw_limit(nAxis,pos,STOP_EVENT);
	Sleep(100);
	return 0;
}
int CMMCMotionCtrl::SetSoftLimtPos(int nAxis,double pos)
{   
	if(nAxis==99)return 0;
     set_positive_sw_limit(nAxis,pos,STOP_EVENT);
    return 0;
}



int CMMCMotionCtrl::MotionMoveDone(int nAxis0 , int nAxis1, int nAxis2, int nAxis3,int nAxis4,int nAxis5,int nAxis6 ,int nAxis7)
{  
    int MotionDone=1; // 모션이 돌고 있을때 1, 완전히 정지 했을때 0;
		
		if(!(nAxis0==99))Wait_For_Done(nAxis0);
		
		if(!(nAxis1==99))Wait_For_Done(nAxis1);
		
		if(!(nAxis2==99))Wait_For_Done(nAxis2);
		
		if(!(nAxis3==99))Wait_For_Done(nAxis3);
		
		if(!(nAxis4==99))Wait_For_Done(nAxis4);
		
		if(!(nAxis5==99))Wait_For_Done(nAxis5);
		
		if(!(nAxis6==99))Wait_For_Done(nAxis6);
		
		if(!(nAxis7==99))Wait_For_Done(nAxis7);
	

	return MotionDone;

}

int CMMCMotionCtrl::Wait_For_Done(int nAxis)

{
   
    if(nAxis==99)return 0;
    wait_for_done(nAxis);
  return 0;
}