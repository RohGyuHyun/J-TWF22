#include "stdafx.h"
#include "CXMCIOPort.h"

#pragma comment(lib, ".\\xmc\\x_motion.lib")

int CXMCIOPort::GetInputCount()
{
    // TODO: 여기에 구현 코드 추가.
    return 32;
}


int CXMCIOPort::GetOutputCount()
{
    // TODO: 여기에 구현 코드 추가.
    return 32;
}

//! return value 0:success -1:error
int CXMCIOPort::Init(void)
{
    // TODO: 여기에 구현 코드 추가.
    INT16 err;
    err = mmc_exio_initx();
    if (err) return -1;
    else return 0;
}


BOOL CXMCIOPort::GetBit(int nBitNo)
{
    // TODO: 여기에 구현 코드 추가.
    int nPortNo = nBitNo / 32;
    long Bit = 0x01 << (nBitNo % 32);
    INT32 rv = 0;
    get_option_io(nPortNo, &rv);
    if (rv & Bit)return FALSE;
    else return TRUE;
}


int CXMCIOPort::SetBit(int bitNo, int Value)
{
    // TODO: 여기에 구현 코드 추가.
    int rv;

    if (!Value)
        rv = set_option_bit(bitNo);
    else
        rv = reset_option_bit(bitNo);

    return rv;
}


BOOL CXMCIOPort::GetOutBit(int nBitNo)
{
    // TODO: 여기에 구현 코드 추가.
    long PortNo = nBitNo / 32;
    long Bit = 0x01 << (nBitNo % 32);
    long rv = 0;
    GetOutPort(PortNo, &rv);
    if (rv & Bit)return TRUE;
    else return FALSE;
}


int CXMCIOPort::GetOutPort(int nPort, long* pValue)
{
    // TODO: 여기에 구현 코드 추가.
    INT32 getv = 0;
    int rv = get_option_out_io(nPort, &getv);
    * pValue = ~getv;
    return rv;
}
