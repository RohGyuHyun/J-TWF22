#pragma once
#include"MilBase.h"

/*!
\brief 
* Mil�� Edge Finder���� �Լ��� Classȭ��
@todo PCB�̿��� �˻���� ���� �۾��ؾ� ��.
@author ������
*/
class MIL_DLLDECL CMilEdgeFinder:public CMilBase
{
public:
	CMilEdgeFinder(void);
	~CMilEdgeFinder(void);
	int Draw(void);
};
