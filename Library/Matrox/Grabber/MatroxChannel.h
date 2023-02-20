#pragma once
#include <mil.h>
#include "..\MilBase.h"
#include "GrabChannel.h"
#include "atltypes.h"

class CMatroxChannel;

typedef struct
{
	MIL_ID  MilDigitizer;
	MIL_ID  MilImageDisp;
	CMilBuffer *pParentImg;
	CMilBuffer **ppSplitLineImg;
	int		nOfSplitBuffer;
	TCHAR	szChName[40];
	BOOL	bIsSaveImg;
	CSize	sizeDCF;
	MIL_INT ProcessedImageCount;
	int		lsWholeRealLength; //Line Scan 할 실제 길이
	CMatroxChannel *pMatroxChannel;
	CWnd	*pWndInspDlg;
} HookDataStruct;

class CMilSystem;
class CMilBuffer;
///Mil Digitizer를 활용한다.
class MIL_DLLDECL CMatroxChannel :
	public CGrabChannel
{
public:
	CMatroxChannel(void);
	~CMatroxChannel(void);
	int Create(CMilSystem* pMilSystem);
	int GetResolution(CSize& Rect);
	int StopGrab(void);
	int Grab(void);
	int GrabContinuous(void);
	int Close(void);

	MIL_ID m_milIDApplication, m_milIDDigitizer;
	CMilSystem *m_pMilSystem;
	CString m_strCameraFile;
	int Alloc(void);
	CMilBuffer *m_pMilGrabBuffer;
	int m_bIsLineScan;
	int m_lscanRealLength;
	void SetGrabTimeInfinite();
	int GrabLine();
	void SplitGBChildren();
	int m_nOfSplitBuffer;
	CSize m_sizeCam;
	MIL_ID *m_pMilIDSplitGBList;
	HookDataStruct m_UserHookData;
	int Create();
	void SetCoverGlassBuffer(CMilBuffer ** ppCGSplitImgs);
	CMilBuffer **m_ppCGSplitImgs;
	// 실제 Scan해야할 Line수 설정 (CG검사기의 경우 Image Y길이의 3배입력)
	void SetRealLineToScan(int nRealLineToScan);
	int m_nSelectedLight;
	BOOL m_bIsDataAquMode;
	void SetSelectedLightNo(int nSelectedLight);
	void SetDataAquMode(BOOL bMode);
	void ProcessStop();
	BOOL m_bGrabOpen;
	CWnd *m_pCamDlg;
};
