#pragma once
#include "MilClass.h"

/*!
\brief
* Mil�� Code(���ڵ� ����)���� �Լ��� Classȭ��
@author ���ο�
*/

class MIL_DLLDECL MilCode :public CMilBase
{
public:
	MilCode();
	~MilCode();

	MIL_ID m_milIDCode;
	int Alloc();
	int Free();
	long m_lCodeType;

};

