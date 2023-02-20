#pragma once
#include <mil.h>
#include "MilGraph.h"
#include <UniUtility.h>

/*!
\brief 
* Mil�� Display����  �Լ����� Classȭ�Ͽ���
@author ������
*/
class MIL_DLLDECL CMilDisplay:public CMilGraph
{
public:
	CMilDisplay(void);
	CMilDisplay(CMilApplication * pMilApplication, CMilSystem * pMilSystem);
	~CMilDisplay(void);
	int Create(void);
	int Create(CMilApplication * pMilApplication, CMilSystem * pMilSystem);
	void Control(MIL_INT ctrlType, MIL_DOUBLE ctrlValue);
	int EnableOverlay(BOOL bState=TRUE);
	int Close(void);
	MIL_ID m_milIDApplication;
	int GetDisplayID(void);
	int GetIDOverlay(void);
	int ClearOverlay(void);
	int ClearOverlay(CMilDisplay *pMilDisplay);
	BOOL IsSelected(void);
	void UnSelectDisp(void);
	MIL_ID m_milIDDisplay;
	int SetViewShift(int vShift);
	int SetViewAutoScale(BOOL bState=TRUE);
	int SetZoomFactor(double XFactor, double YFactor);
	int SetPanFactor(double XOffset, double YOffset);
	UINT GetTransparencyColor(void);
	void FilWindow(bool bStatus=TRUE);
	void DispCenter(bool bStatus = TRUE);
	CdblSize GetZoomFactor();
	int GetMousePosImgXY(CdblPoint * pDblPoint);
	int EnableAutoPanZoom();
};
