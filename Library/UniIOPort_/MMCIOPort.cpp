#include "StdAfx.h"
#include "MMCIOPort.h"
#include "MMCWHP201.h"

#ifdef _MMC
CMMCIOPort::CMMCIOPort(void)
{
}

CMMCIOPort::~CMMCIOPort(void)
{
}

int CMMCIOPort::SetPort(int nPort, long value)
{
#ifdef BIT_REV
	return set_io(nPort, ~value);
#else
	return set_io(nPort, value);
#endif
}

int CMMCIOPort::GetPort(int nPort, long * pValue)
{
	long getv=0;
	int rv=get_io(nPort, &getv);
#ifdef BIT_REV
	*pValue=~getv;
#else
	*pValue=getv;
#endif
	return rv;
}

int CMMCIOPort::GetOutPort(int nPort, long * pValue)
{
	long getv=0;
	int rv=get_out_io(nPort, &getv);
#ifdef BIT_REV
	*pValue=~getv;
#else
	*pValue=getv;
#endif
	return rv;
}

int CMMCIOPort::SetBit(int bitNo, int Value)
{
	int rv;
#ifdef BIT_REV
	if(!Value)
		rv=set_bit(bitNo);
	else
		rv=reset_bit(bitNo);
#else
	if(Value)
		rv=set_bit(bitNo);
	else
		rv=reset_bit(bitNo);
#endif
	return rv;
}

BOOL CMMCIOPort::GetBit(int nBitNo)
{
	long PortNo=nBitNo/32;
	long Bit=0x01<<(nBitNo%32);
	long rv=0;
	GetPort(PortNo, &rv);
	if(rv&Bit)return TRUE;
	else return FALSE;
}

BOOL CMMCIOPort::GetOutBit(int nBitNo)
{
	long PortNo=nBitNo/32;
	long Bit=0x01<<(nBitNo%32);
	long rv=0;
	GetOutPort(PortNo, &rv);
	if(rv&Bit)return TRUE;
	else return FALSE;
}

#endif
