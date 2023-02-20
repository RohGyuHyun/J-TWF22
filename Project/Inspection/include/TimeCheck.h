#ifndef	_CTIMER_CHECK
#define	_CTIMER_CHECK

class CTimerCheck
{
public:
	CTimerCheck(long checktime = 100); // ������ msec...
	~CTimerCheck();

public:
	long TimePassed();	// ���� ������ �ð��� ����....
	long GetCheckTime();
	void SetCheckTime(long ctime);
	bool IsTimeOver();
	bool IsMoreThan(long ctime);	// ������ msec..
	bool IsLessThan(long ctime);	// ������ msec..
	void StartTimer();
private:
	long m_CheckTime;
	long StartTime;
	long EndTime;
};

#endif