#include "pch.h"
#include "TimeCheck.h"
#include <time.h>

//_____________________________________________________________________________
CTimerCheck::CTimerCheck(long msec) // msec
{
	m_CheckTime = msec;
}

CTimerCheck::~CTimerCheck()
{
}

//_____________________________________________________________________________
void CTimerCheck::StartTimer()
{
	StartTime = GetCurrentTime();
}

//_____________________________________________________________________________
bool CTimerCheck::IsLessThan(long msec)
{
	//End Time ���� [20090209 LJY]
//	EndTime = GetCurrentTime();

	if (TimePassed() < msec)
		return TRUE;
	else
		return FALSE;
}

//_____________________________________________________________________________
bool CTimerCheck::IsMoreThan(long msec)
{
	//End Time ���� [20090209 LJY]
//	EndTime = GetCurrentTime();

	if (TimePassed() >= msec)
		return TRUE;
	else
		return FALSE;
}

//_____________________________________________________________________________
long CTimerCheck::TimePassed()
{
	//End Time �߰�[20090209 LJY]
	EndTime = GetCurrentTime();
	return (EndTime - StartTime);
}

//_____________________________________________________________________________
// �ð��� ����Ȱ�� 'TRUE' �� ���� �Ѵ�.
bool CTimerCheck::IsTimeOver()
{
	//End Time ���� [20090209 LJY]
//	EndTime = GetCurrentTime();

	if (TimePassed() < m_CheckTime)
		return FALSE;
	else
		return TRUE;
}


void CTimerCheck::SetCheckTime(long msec)
{
	m_CheckTime = msec;
}

long CTimerCheck::GetCheckTime()
{
	return m_CheckTime;
}