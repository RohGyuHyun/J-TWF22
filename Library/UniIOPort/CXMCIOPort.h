#pragma once
#include "IOPort.h"
#include ".\XMC\x_motion.h"

class CXMCIOPort : public CIOPort
{
public:
    virtual int GetInputCount();
    virtual int GetOutputCount();
    virtual int Init(void);
    BOOL GetBit(int nBitNo);
    virtual int SetBit(int bitNo, int Value);
    virtual BOOL GetOutBit(int nBitNo);
    virtual int GetOutPort(int nPort, long* pValue);
};

