#include "StdAfx.h"
#include "DblAxisPos.h"

CdblAxisPos::CdblAxisPos(void)
{
	x=y=z=t=r=0;
}

CdblAxisPos::CdblAxisPos(double vx, double vy, double vz, double vt, double vr)
{
	x=vx;
	y=vy;
	z=vz;
	t=vt;
	r=vr;
}

CdblAxisPos::~CdblAxisPos(void)
{
}

/*! @fn void CdblAxisPos::Offset(CdblAxisPos *pPos, OffsetType)
	@param 
	@return 
*/
void CdblAxisPos::Offset(CdblAxisPos *pPos)
{   

	//x=x-(pPos->x); //9800 �ϰ��
	//y=y+(pPos->y);


	x=x-pPos->x;    // H100 �ϰ�� ���� �ݴ� 
	y=y+(pPos->y);

	z=z+(pPos->z);
	r=r+(pPos->r);
	t=t+(pPos->t);
}


CdblAxisPos CdblAxisPos::operator+(CdblAxisPos &Pos)
{
	return CdblAxisPos(x+Pos.x, y+Pos.y, z+Pos.z, t+Pos.t, r+Pos.r);
}

CdblAxisPos CdblAxisPos::operator-(CdblAxisPos &Pos)
{
	return CdblAxisPos(x-Pos.x, y-Pos.y, z-Pos.z, t-Pos.t, r-Pos.r);
}

void CdblAxisPos::operator=(CdblAxisPos &Pos)
{
	x=Pos.x;
	y=Pos.y;
	z=Pos.z;
	t=Pos.t;
	r=Pos.r;
}
