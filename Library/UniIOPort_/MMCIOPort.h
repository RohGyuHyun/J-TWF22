#pragma once
#include "Defines.h"
#include "ioport.h"

#ifdef _MMC
#ifdef _MMC_HALF
#include "MMCWHP201.h"
#pragma comment(lib, "MMCWHP230")
#else
#include "MMCWFP39.h"
#pragma comment(lib, "VC60FP39")
#endif


#define BIT_REV

class UNIIOPORT_DLLDECL CMMCIOPort : public CIOPort
{
public:
	CMMCIOPort(void);
	~CMMCIOPort(void);
	virtual int SetPort(int port, long value);
	virtual int GetPort(int nPort, long * pValue);
	virtual int GetOutPort(int nPort, long * pValue);
	virtual int SetBit(int bitNo, int value);
	virtual BOOL GetBit(int nBitNo);
	virtual BOOL GetOutBit(int nBitNo);
	
   

};
#endif
