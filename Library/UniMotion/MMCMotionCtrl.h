#pragma once
#include "motionctrl.h"

 #include "UniUtility.h"

//Star9800L 축정의 
#define AXIS_DCENT_Z 0
#define AXIS_DSIDE_X 1
#define AXIS_DSIDE_Z 2
#define AXIS_DSIDE_R 3

class CMMCMotionCtrl :
	public CMotionCtrl
{
public:
	CMMCMotionCtrl(void);
	~CMMCMotionCtrl(void);
	///////////////////// 이부분의함수 들은 Low level 에서 동작 하는것으로 사용하지 않는다////////////////////////////////
	void InitAxisNum(void);
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
	virtual int SetCommand(int axis, double command);
	virtual int GetCommand(int axis, double * pCommand);
	virtual int GetError(int axis, double * pErr);
	virtual int Roate360(int nAxis,double pos, double velo, int accel,int decel);
	int VMotionStop(int nAxis);
	int GoHomeSetZero(int nAxis);
	int GoHomePos(int nAxis);
	int LeaveHomePos(int nAxis);
	int MotionStop(int nAxis);
	int ClearStatus(int nAxis);
	int GoHomeSetZeroXAxis(int nAxis);
	int GoHomeSetZeroRAxis(int nAxis);
	int GoNHomeSetZero(int nAxis);
	int Set_Stop_Rate(int axis,int rate);
	int GoMoveAxisVelo(int nAxis, double velo, int Accel);
    double  Get_electroGear(int nAxis);
    int Wait_For_Done(int nAxis);
	int MotionMoveDone(int nAxis0, int nAxis1, int nAxis2, int nAxis3,int nAxis4,int nAxis5,int nAxis6 ,int nAxis7);
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	/////////////// 주로사용하는 함수 정리////12년 11월28일 안지훈 정리함//////////////////


	//<축의 설정 관련 함수 들///

	int AxisNumeInit();         // 전체 축번호를 관하는 함수 
							// <주의> 장비의 모델명이 다른경우 축설정이 안되므로 이함수안에 들어가 수정해야한다.
	
	int SetSoftLimtNeg(int nAxis,double pos); // 네거티프 소프트웨어리밋
	int SetSoftLimtPos(int nAxis,double pos); // 포지티브 소프트웨어리밋
	int Set_electroGear(int nAxis , double ratio);// 기어비설정 
	virtual int StateAndFrameClearAxis(int nAxis);// 축의 전체의 프레임 과 스테이트를 클리어하는 함수 
	virtual int StateAndFrameClearAll(int AxisTotalnum); // 지정 축의 프레임과 스테이트를 클리어하는 함수 
	
	//<축의 설정함수 끝////////

	
	// 축의 홈체크 관련 함수들 ///
	
	virtual int SetNowPosHome(int nAxis);//현재의 위치를 홈으로 등록하기 
	int CamDriveRHomeSetZero(int nAxis); // 캠브라이브를 이용하는 로테이트 축 즉 OCT 축의 홈체크를하는 함수로 홈을 벗어나기까지 (+)방향으로 움직이다 멈춘다.
	int GoHomeSetZeroNegHome(int nAxis);  // 네거티브 센서를 홈센서로 인식하고 우선 네거티브 센서를 만날때까지 천천히 이동하며 네거티브를 만나면 네거티브를 만날때까지 
										  // 포지티브 방향으로 이동하다 멈춘다.
	
	// 축의 홈체크 관련 함수들끝 ///


	//<축의 이동 관련함수////
    
	
	int GetHomeSensor(int nAxis);		  // 홈센서 읽어와 감지됬는지 확인
	int GetPosSensor(int nAxis);		  // 포지티브 센서를 읽어옴
	int GetNegSensor(int nAxis);		  // 네거티브 센서를 읽어옴 	

	virtual double GetPosition(int axis, double * pPosition);// 축의 현재 위치를 가져온다.

	int AllMotionMoveDone();// 전체 모션이 동작 하는지를 감지하는 함수 리턴값이 0 이면 모든축이 정지한것, 1 이면 한축이라도 움직이거나 명령을 받는중임
							// <주의>내부에 7000, H100, H160, 용으로 축을 검사하는 부분이 나누어져 있기때문 각 프로덕트네임으로 등록하나 해주어야한다. 
	int GoPosSensor(int nAxis, double velo, int accel); // 포지티브 센서를 만날때까지 지정된 Velo 속도로 Acecl 가속 으로 이동하여 멈춤
	int Roate360_VeloMoving(int nAxis, double velo, int Accel);// CT 를 촬영하기위해서 한쪽방향으로 일정 속도로 Velo 로 무한 회전 시키기위함 멈추기위해선 
															   //,Velostop 이나 Set_Rate_Stop 함수를 이용한다.
	virtual int GoNonMultiPositionMove(int nAxis,double pos, double velo, int accel,int decel); // 지정축을 velo 속도만큼 가속 감속 Acel Decel 만큼 이동한다 
																								// 지정축이 이동이 완료할때가지 다른 동작은 모두 멈추게된다.
	int GoPositionMove(int nAxis,double pos, double velo, int accel,int decel);					// 지정축을 velo 속도만큼 가속. 감속 만큼 이동한다.
																								// 지정축에게 이동 명령을 내리기만하고 다음 코드로 이동가능한다 (마치 스레드처럼 동작)
	
	virtual int EStop(int Axis);   // 긴급정지 
	virtual int In_MotionCheck(int axis);// 모션의 움직이는치 확인 리턴밸류가 1 이면 모션이 이동하는중
	virtual int In_SequenceCheck(int axis);// 모션이 미리 받은 시퀀스가 MMC 안에 존재하는지 검사 1이 리턴되면 명령 이 들어가있는상태
	virtual int VeloMove(int axis, double velo, int accel); // 무한 회전이동 시작
	virtual int VeloStop(int axis);// 무한 회전 이동 정지

	//<축의 이동 관련함수들 끝////
	virtual int SetAmpEnable(int axis, int state);// 축의 엠프를 온오프시킬수 있음
	virtual int GetAmpEnable(int axis, short *value);// 축의 엠프를 온오프상태를 체크한다
	virtual int SetAmpReset(int axis); //축의 엠프를 리셋시킨다. 
	virtual int GetAmpFault(int axis,short* value);// 엠프의 폴트 상태를 표시한다. 
};
