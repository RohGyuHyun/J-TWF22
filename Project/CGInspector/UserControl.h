#pragma once
#include "StdAfx.h"
#include "CGInspector.h"


static CString constr;

class CUserControl
{
public:
	CUserControl(void);
	~CUserControl(void);
	void LEVEL0(CString data);
	void LEVEL1(CString data);
	void LEVEL2(CString data);	

};
