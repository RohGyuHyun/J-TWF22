#pragma once

#define ALARM_CODE_EMG							1
#define ALARM_CODE_KEY_SW						2
#define ALARM_CODE_DOOR							3
#define ALARM_CODE_INVERTER_ALARM				4
#define ALARM_CODE_TUBE_ALARM					5
#define ALARM_CODE_X_RAY_LAMP_ALARM				6
#define ALARM_CODE_X_RAY_WARMUP_NOT_USED		7
#define ALARM_CODE_MC01_ON_CHECK				8
#define ALARM_CODE_HDD_SPACE					9

const CString INPUT_EMG					 = _T("EMS");
const CString INPUT_KEY_SW				 = _T("KEY SW");
#ifndef CE_CONFIRM
const CString INPUT_DOOR				 = _T("DOOR");
#else
const CString INPUT_DOOR1 = _T("DOOR1");
const CString INPUT_DOOR2 = _T("DOOR2");
const CString INPUT_DOOR3 = _T("DOOR3");
#endif
#ifndef INVERTER_COM
const CString INPUT_INVERTER_ALARM		 = _T("INVERTER ALARM");
#endif
#ifndef CE_CONFIRM
const CString INPUT_TUBE_ALARM			 = _T("TUBE ALARM");
#endif
const CString INPUT_X_RAY_LAMP_ALARM	 = _T("X_RAY_LAMP_ALM");
#ifdef INVERTER_COM
const CString INPUT_INPUT_SENSER		 = _T("INPUT_SENSER");
const CString INPUT_OUTPUT_SENSER		 = _T("OUTPUT_SENSER");
#endif

const CString OUTPUT_TOWER_LAMP_RED		 = _T("TOWER LAMP RED");
const CString OUTPUT_TOWER_LAMP_YELLOW	 = _T("TOWER LAMP YELLOW");
const CString OUTPUT_TOWER_LAMP_GREEN	 = _T("TOWER LAMP GREEN");
const CString OUTPUT_TOWER_BUZZER1		 = _T("TOWER BUZZER1");
#ifndef INVERTER_COM
const CString OUTPUT_TOWER_BUZZER2		 = _T("TOWER BUZZER2");
#endif
const CString OUTPUT_MC01_ON			 = _T("MC01 ON(INVERTER POWER)");
const CString OUTPUT_X_RAY_O			 = _T("X-RAY ON");
#ifndef INVERTER_COM
const CString OUTPUT_MOTOR_CW			 = _T("MOTOR CW");
const CString OUTPUT_MOTOR_CCW			 = _T("MOTOR CCW");
const CString OUTPUT_INVERTER_RESET			 = _T("INVERTER RESET");
#endif
const CString OUTPUT_REJECTER			 = _T("REJECTER_SOL");
enum Input_Enum
{
	INPUT_INDEX_EMG,
	INPUT_INDEX_KEY_SW,
#ifndef CE_CONFIRM
	INPUT_INDEX_DOOR,
#else
	INPUT_INDEX_DOOR1,
	INPUT_INDEX_DOOR2,
	INPUT_INDEX_DOOR3,
#endif
#ifndef INVERTER_COM
	INPUT_INDEX_INVERTER_ALARM,
#endif
#ifndef CE_CONFIRM
	INPUT_INDEX_TUBE_ALARM,
#endif
	INPUT_INDEX_X_RAY_LAMP_ALM,
#ifdef INVERTER_COM
	INPUT_INDEX_INPUT_SENSER,
	INPUT_INDEX_OUTPUT_SENSER,
#endif
};

enum Output_Enum
{
	OUTPUT_INDEX_TOWER_LAMP_RED,
	OUTPUT_INDEX_TOWER_LAMP_YELLOW,
	OUTPUT_INDEX_TOWER_LAMP_GREEN,
	OUTPUT_INDEX_TOWER_BUZZER1,
#ifndef INVERTER_COM
	OUTPUT_INDEX_TOWER_BUZZER2,
#endif
	OUTPUT_INDEX_MC01_ON,
	OUTPUT_INDEX_X_RAY_ON,
#ifndef INVERTER_COM
	OUTPUT_INDEX_MOTOR_CW,
	OUTPUT_INDEX_MOTOR_CCW,
	OUTPUT_INDEX_INVERTER_RESET,
#endif
	OUTPUT_INDEX_REJECTER_SOL,
};

class CIOClass
{
protected:
	IO_PARA m_stInput[MAX_INPUT_NUM];
	IO_PARA m_stOutput[MAX_OUTPUT_NUM];
private:


public:
	CIOClass();
	~CIOClass();

	static CIOClass* Instance();
	static CIOClass* m_pInstance;

	void SetIOPara(IO_PARA* Input, IO_PARA* Output);
	BOOL GetInputBit(int nIdx);
	BOOL GetOutputBit(int nIdx);
	void SetOutputBit(int nIdx, BOOL isOnOff);

	int GetInputValue(Input_Enum nIdx);// -1 NoLabel Text;
	int SetOutputValue(Output_Enum nIdx, BOOL isOnOff);// -1 NoLabel Text;
};

