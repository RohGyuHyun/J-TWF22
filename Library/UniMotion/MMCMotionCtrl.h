#pragma once
#include "motionctrl.h"

 #include "UniUtility.h"

//Star9800L ������ 
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
	///////////////////// �̺κ����Լ� ���� Low level ���� ���� �ϴ°����� ������� �ʴ´�////////////////////////////////
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



	/////////////// �ַλ���ϴ� �Լ� ����////12�� 11��28�� ������ ������//////////////////


	//<���� ���� ���� �Լ� ��///

	int AxisNumeInit();         // ��ü ���ȣ�� ���ϴ� �Լ� 
							// <����> ����� �𵨸��� �ٸ���� �༳���� �ȵǹǷ� ���Լ��ȿ� �� �����ؾ��Ѵ�.
	
	int SetSoftLimtNeg(int nAxis,double pos); // �װ�Ƽ�� ����Ʈ�����
	int SetSoftLimtPos(int nAxis,double pos); // ����Ƽ�� ����Ʈ�����
	int Set_electroGear(int nAxis , double ratio);// ������ 
	virtual int StateAndFrameClearAxis(int nAxis);// ���� ��ü�� ������ �� ������Ʈ�� Ŭ�����ϴ� �Լ� 
	virtual int StateAndFrameClearAll(int AxisTotalnum); // ���� ���� �����Ӱ� ������Ʈ�� Ŭ�����ϴ� �Լ� 
	
	//<���� �����Լ� ��////////

	
	// ���� Ȩüũ ���� �Լ��� ///
	
	virtual int SetNowPosHome(int nAxis);//������ ��ġ�� Ȩ���� ����ϱ� 
	int CamDriveRHomeSetZero(int nAxis); // ķ����̺긦 �̿��ϴ� ������Ʈ �� �� OCT ���� Ȩüũ���ϴ� �Լ��� Ȩ�� �������� (+)�������� �����̴� �����.
	int GoHomeSetZeroNegHome(int nAxis);  // �װ�Ƽ�� ������ Ȩ������ �ν��ϰ� �켱 �װ�Ƽ�� ������ ���������� õõ�� �̵��ϸ� �װ�Ƽ�긦 ������ �װ�Ƽ�긦 ���������� 
										  // ����Ƽ�� �������� �̵��ϴ� �����.
	
	// ���� Ȩüũ ���� �Լ��鳡 ///


	//<���� �̵� �����Լ�////
    
	
	int GetHomeSensor(int nAxis);		  // Ȩ���� �о�� ��������� Ȯ��
	int GetPosSensor(int nAxis);		  // ����Ƽ�� ������ �о��
	int GetNegSensor(int nAxis);		  // �װ�Ƽ�� ������ �о�� 	

	virtual double GetPosition(int axis, double * pPosition);// ���� ���� ��ġ�� �����´�.

	int AllMotionMoveDone();// ��ü ����� ���� �ϴ����� �����ϴ� �Լ� ���ϰ��� 0 �̸� ������� �����Ѱ�, 1 �̸� �����̶� �����̰ų� ����� �޴�����
							// <����>���ο� 7000, H100, H160, ������ ���� �˻��ϴ� �κ��� �������� �ֱ⶧�� �� ���δ�Ʈ�������� ����ϳ� ���־���Ѵ�. 
	int GoPosSensor(int nAxis, double velo, int accel); // ����Ƽ�� ������ ���������� ������ Velo �ӵ��� Acecl ���� ���� �̵��Ͽ� ����
	int Roate360_VeloMoving(int nAxis, double velo, int Accel);// CT �� �Կ��ϱ����ؼ� ���ʹ������� ���� �ӵ��� Velo �� ���� ȸ�� ��Ű������ ���߱����ؼ� 
															   //,Velostop �̳� Set_Rate_Stop �Լ��� �̿��Ѵ�.
	virtual int GoNonMultiPositionMove(int nAxis,double pos, double velo, int accel,int decel); // �������� velo �ӵ���ŭ ���� ���� Acel Decel ��ŭ �̵��Ѵ� 
																								// �������� �̵��� �Ϸ��Ҷ����� �ٸ� ������ ��� ���߰Եȴ�.
	int GoPositionMove(int nAxis,double pos, double velo, int accel,int decel);					// �������� velo �ӵ���ŭ ����. ���� ��ŭ �̵��Ѵ�.
																								// �����࿡�� �̵� ����� �����⸸�ϰ� ���� �ڵ�� �̵������Ѵ� (��ġ ������ó�� ����)
	
	virtual int EStop(int Axis);   // ������� 
	virtual int In_MotionCheck(int axis);// ����� �����̴�ġ Ȯ�� ���Ϲ���� 1 �̸� ����� �̵��ϴ���
	virtual int In_SequenceCheck(int axis);// ����� �̸� ���� �������� MMC �ȿ� �����ϴ��� �˻� 1�� ���ϵǸ� ��� �� ���ִ»���
	virtual int VeloMove(int axis, double velo, int accel); // ���� ȸ���̵� ����
	virtual int VeloStop(int axis);// ���� ȸ�� �̵� ����

	//<���� �̵� �����Լ��� ��////
	virtual int SetAmpEnable(int axis, int state);// ���� ������ �¿�����ų�� ����
	virtual int GetAmpEnable(int axis, short *value);// ���� ������ �¿������¸� üũ�Ѵ�
	virtual int SetAmpReset(int axis); //���� ������ ���½�Ų��. 
	virtual int GetAmpFault(int axis,short* value);// ������ ��Ʈ ���¸� ǥ���Ѵ�. 
};
