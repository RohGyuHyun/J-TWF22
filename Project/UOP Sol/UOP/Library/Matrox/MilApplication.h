#pragma once
#include "Mil.h"
#include "MilBase.h"

/*!
\brief 
* Mil�� Application���� �Լ��� Classȭ��
@warning � Mil�Լ��� ����ϴ��� ���� Application�ʱ�ȭ �Ǿ����
@author ������
*/
class MIL_DLLDECL CMilApplication:public CMilBase
{
public:
	CMilApplication(void);
	~CMilApplication(void);
	int Create(void);
	void Close(void);
};
