//#include "StdAfx.h"
#include <Mil.h>
#include "GrabChannel.h"
#include "MatroxChannel.h"
#include "..\MilSystem.h"
#define CAM_WIDTH 8192

CMatroxChannel::CMatroxChannel(void)
	: m_bIsLineScan(TRUE)
	, m_lscanRealLength(30000)
	, m_pMilGrabBuffer(NULL)
	, m_pMilIDSplitGBList(NULL)
	, m_nOfSplitBuffer(0)
	, m_ppCGSplitImgs(NULL)
	, m_nSelectedLight(0)
	, m_bIsDataAquMode(FALSE)
	, m_bGrabOpen(FALSE)
	, m_pCamDlg(NULL)
{
}

CMatroxChannel::~CMatroxChannel(void)
{
}


int CMatroxChannel::Create()
{
	Create(m_pMilSystem);
	return 0;
}


int CMatroxChannel::Create(CMilSystem* pMilSystem)
{
	m_pMilSystem = pMilSystem;
	m_milIDApplication = m_pMilSystem->m_milIDApplication;
	m_milIDSystem = pMilSystem->m_milIDSystem;
	Alloc();
	m_nChannel;
	return 0;
}

int CMatroxChannel::GetResolution(CSize& Rect)
{
	MIL_INT cx, cy;
	MdigInquire(m_milIDDigitizer, M_SIZE_X, &cx); 
	MdigInquire(m_milIDDigitizer, M_SIZE_Y, &cy);
	Rect.SetSize(cx,cy);
	return 0;
}

int CMatroxChannel::StopGrab(void)
{
	MdigHalt(m_milIDDigitizer);
	return 0;
}

int CMatroxChannel::Grab(void)
{
	MdigGrab(m_milIDDigitizer, m_pMilBufProc->GetIDBuffer());
	return 0;
}

int CMatroxChannel::Close(void)
{
	for (int i = 0; i < 3; i++)
	{
		MbufFree(m_pMilIDSplitGBList[i]);
	}
	delete m_pMilIDSplitGBList;
	m_pMilGrabBuffer->Close();
	delete m_pMilGrabBuffer;
	MdigFree(m_milIDDigitizer);
	return 0;
}

int CMatroxChannel::GrabContinuous(void)
{
	MdigGrabContinuous(m_milIDDigitizer, m_pMilBufProc->GetIDBuffer());
	return 0;
}
//스켄받을 2배짜리 버퍼를 설정
int CMatroxChannel::Alloc(void)
{
	MdigAlloc(m_milIDSystem, M_DEV0, m_strCameraFile, M_DEFAULT, &m_milIDDigitizer);
	GetResolution(m_sizeCam);
	CSize sizeDBuffer = m_sizeCam;  //Double Buffer를 위해 화면길의의 2배를 갖는다.
	sizeDBuffer.cy *= 3;
	m_pMilGrabBuffer = new CMilBuffer;
	m_pMilGrabBuffer->m_milIDSystem = m_milIDSystem;
	m_pMilGrabBuffer->SetAttribute(M_IMAGE + M_GRAB + M_PROC);
	m_pMilGrabBuffer->SetDepth(8 + M_UNSIGNED);
	m_pMilGrabBuffer->Alloc2D(sizeDBuffer);
	m_pMilGrabBuffer->Clear(0xff);
	return 0;
}

void CMatroxChannel::SetGrabTimeInfinite()
{
	MdigControl(m_milIDDigitizer, M_GRAB_TIMEOUT, M_INFINITE);
}

//2개의 Child버퍼작성
void CMatroxChannel::SplitGBChildren()
{
	m_pMilIDSplitGBList = new MIL_ID[3];
	for (int i = 0; i < 3; i++)
	{
		MbufChild2d(m_pMilGrabBuffer->GetIDBuffer(), 0, i*m_sizeCam.cy, m_sizeCam.cx, m_sizeCam.cy, &m_pMilIDSplitGBList[i]);
	}
}



/* User's processing function called every time a grab buffer is ready. */
/* -------------------------------------------------------------------- */

/* Local defines. */
#define STRING_LENGTH_MAX  220
#define STRING_POS_X       20
#define STRING_POS_Y       20

MIL_INT MFTYPE ProcessingFunctionTopCam(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
{
	static int TopImageIndex = 0;
	static int BottomImageIndex = 0;
	static int accumulation= 0;
	static int recur = 0;
	HookDataStruct *UserHookDataPtr = (HookDataStruct *)HookDataPtr;
	MIL_ID ModifiedBufferId;
	CString strChName = UserHookDataPtr->szChName;
	MIL_TEXT_CHAR Save_Text[STRING_LENGTH_MAX] = { MIL_TEXT('\0'), };
	MIL_TEXT_CHAR Save_TextH[STRING_LENGTH_MAX] = { MIL_TEXT('\0'), };
	MIL_TEXT_CHAR Save_Text0[STRING_LENGTH_MAX] = { MIL_TEXT('\0'), };
	MIL_TEXT_CHAR Save_Text1[STRING_LENGTH_MAX] = { MIL_TEXT('\0'), };
	MIL_TEXT_CHAR Save_Text2[STRING_LENGTH_MAX] = { MIL_TEXT('\0'), };
	CMatroxChannel * MatroxChannel = UserHookDataPtr->pMatroxChannel;
	int nSelectedLight = MatroxChannel->m_nSelectedLight;
	BOOL bIsDataAquMode = MatroxChannel->m_bIsDataAquMode;
	CSize sizeDCF = UserHookDataPtr->sizeDCF;
	int nWidth = sizeDCF.cx;
	int nDCFYLength = sizeDCF.cy;
	int lsRealLength = UserHookDataPtr->lsWholeRealLength;
	int lsOneRealLength = lsRealLength / 3;
	CWnd *pWndInspDlg = UserHookDataPtr->pWndInspDlg;

	/* Retrieve the MIL_ID of the grabbed buffer. */
	MdigGetHookInfo(HookId, M_MODIFIED_BUFFER + M_BUFFER_ID, &ModifiedBufferId);

	/* Increment the frame counter. */
	UserHookDataPtr->ProcessedImageCount++;
	MIL_UINT32 PixelToGet[CAM_WIDTH];

	MIL_ID ParentBufferID = M_NULL;
	MbufInquire(ModifiedBufferId, M_PARENT_ID, &ParentBufferID);
	int index;
	int nLine;
	for (int i=0;i < nDCFYLength; i++)
	{
		MbufGet2d(ModifiedBufferId, 0, i, nWidth, 1, PixelToGet);
		index = i % 3;
		nLine = i / 3;
		MbufPut2d(UserHookDataPtr->ppSplitLineImg[index]->GetIDBuffer(), 0, accumulation + nLine, nWidth, 1, PixelToGet);
	}
	accumulation += (nLine+1);
	TRACE("ACC:%5d, nLine:%05d", accumulation, nLine);

	if (accumulation >= lsOneRealLength)
	{
		accumulation = 0;
		TRACE(("\nCapture Top"));
		TRACE("\nTOP SAVE  ACC:%5d, nLine:%05d", accumulation, nLine);

		MosSprintf(Save_Text0, STRING_LENGTH_MAX, MIL_TEXT("d:\\SaveImage\\%04d-%sSplit0.BMP"), TopImageIndex, UserHookDataPtr->szChName);
		MosSprintf(Save_Text1, STRING_LENGTH_MAX, MIL_TEXT("d:\\SaveImage\\%04d-%sSplit1.BMP"), TopImageIndex, UserHookDataPtr->szChName);
		MosSprintf(Save_Text2, STRING_LENGTH_MAX, MIL_TEXT("d:\\SaveImage\\%04d-%sSplit2.BMP"), TopImageIndex++, UserHookDataPtr->szChName);
		MbufCopy(UserHookDataPtr->ppSplitLineImg[nSelectedLight]->GetIDBuffer(), UserHookDataPtr->MilImageDisp);
		if (bIsDataAquMode)
		{
			MbufExport(Save_Text0, M_BMP, UserHookDataPtr->ppSplitLineImg[0]->GetIDBuffer());
			MbufExport(Save_Text1, M_BMP, UserHookDataPtr->ppSplitLineImg[1]->GetIDBuffer());
			MbufExport(Save_Text2, M_BMP, UserHookDataPtr->ppSplitLineImg[2]->GetIDBuffer());
		}
		pWndInspDlg->SendMessage(WM_REQ_INSP);
	}
	return 0;
}

MIL_INT MFTYPE ProcessingFunctionBottomCam(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
{
	static int TopImageIndex = 0;
	static int BottomImageIndex = 0;
	static int accumulation = 0;
	static int recur = 0;
	HookDataStruct *UserHookDataPtr = (HookDataStruct *)HookDataPtr;
	MIL_ID ModifiedBufferId;
	CString strChName = UserHookDataPtr->szChName;
	MIL_TEXT_CHAR Save_Text[STRING_LENGTH_MAX] = { MIL_TEXT('\0'), };
	MIL_TEXT_CHAR Save_TextH[STRING_LENGTH_MAX] = { MIL_TEXT('\0'), };
	MIL_TEXT_CHAR Save_Text0[STRING_LENGTH_MAX] = { MIL_TEXT('\0'), };
	MIL_TEXT_CHAR Save_Text1[STRING_LENGTH_MAX] = { MIL_TEXT('\0'), };
	MIL_TEXT_CHAR Save_Text2[STRING_LENGTH_MAX] = { MIL_TEXT('\0'), };
	CMatroxChannel * MatroxChannel = UserHookDataPtr->pMatroxChannel;
	int nSelectedLight = MatroxChannel->m_nSelectedLight;
	BOOL bIsDataAquMode = MatroxChannel->m_bIsDataAquMode;
	CSize sizeDCF = UserHookDataPtr->sizeDCF;
	int nWidth = sizeDCF.cx;
	int nDCFYLength = sizeDCF.cy;
	int lsRealLength = UserHookDataPtr->lsWholeRealLength;
	int lsOneRealLength = lsRealLength / 3;
	CWnd *pWndInspDlg = UserHookDataPtr->pWndInspDlg;

	/* Retrieve the MIL_ID of the grabbed buffer. */
	MdigGetHookInfo(HookId, M_MODIFIED_BUFFER + M_BUFFER_ID, &ModifiedBufferId);

	/* Increment the frame counter. */
	UserHookDataPtr->ProcessedImageCount++;
	MIL_UINT32 PixelToGet[CAM_WIDTH];

	MIL_ID ParentBufferID = M_NULL;
	MbufInquire(ModifiedBufferId, M_PARENT_ID, &ParentBufferID);
	int index;
	int nLine;
	for (int i = 0; i < nDCFYLength; i++)
	{
		MbufGet2d(ModifiedBufferId, 0, i, nWidth, 1, PixelToGet);
		index = i % 3;
		nLine = i / 3;
		MbufPut2d(UserHookDataPtr->ppSplitLineImg[index]->GetIDBuffer(), 0, accumulation + nLine, nWidth, 1, PixelToGet);
	}
	accumulation += (nLine + 1);
	TRACE("ACC:%5d, nLine:%05d", accumulation, nLine);

	if (accumulation >= lsOneRealLength)
	{
		accumulation = 0;
		TRACE(("\nCapture Bottom"));
		TRACE("\nBottom SAVE  ACC:%5d, nLine:%05d", accumulation, nLine);
		MbufCopy(UserHookDataPtr->ppSplitLineImg[0]->GetIDBuffer(), UserHookDataPtr->MilImageDisp);
		MosSprintf(Save_Text, STRING_LENGTH_MAX, MIL_TEXT("d:\\SaveImage\\%04d-%s.BMP"), BottomImageIndex++, UserHookDataPtr->szChName);
		if (bIsDataAquMode)
			MbufExport(Save_Text, M_BMP, UserHookDataPtr->ppSplitLineImg[0]->GetIDBuffer());
		pWndInspDlg->SendMessage(WM_REQ_INSP);
	}
	return 0;
}

int CMatroxChannel::GrabLine()
{
	if (m_bGrabOpen)return -1;
	m_UserHookData.MilDigitizer = m_milIDDigitizer;
	m_UserHookData.MilImageDisp = m_pMilBufDisp->GetIDBuffer();
	m_UserHookData.pParentImg	 = m_pMilGrabBuffer;
	_tcscpy_s(m_UserHookData.szChName, 20, m_strChannelName.GetBuffer());
	m_UserHookData.lsWholeRealLength = m_lscanRealLength;
	m_UserHookData.ppSplitLineImg = m_ppCGSplitImgs;
	m_UserHookData.nOfSplitBuffer = m_nOfSplitBuffer;
	m_UserHookData.sizeDCF = m_sizeCam;
	m_UserHookData.pMatroxChannel = this;
	m_UserHookData.pWndInspDlg =  m_pCamDlg;
	m_UserHookData.ProcessedImageCount = 0;
	if(m_strChannelName.Compare(_T("TOP"))==0)
		MdigProcess(m_milIDDigitizer, m_pMilIDSplitGBList, 2, M_START, M_ASYNCHRONOUS, ProcessingFunctionTopCam, &m_UserHookData);
	else
		MdigProcess(m_milIDDigitizer, m_pMilIDSplitGBList, 2, M_START, M_ASYNCHRONOUS, ProcessingFunctionBottomCam, &m_UserHookData);
	m_bGrabOpen = TRUE;
	return 0;
}


void CMatroxChannel::ProcessStop()
{
	if (m_strChannelName.Compare(_T("TOP"))==0)
		MdigProcess(m_milIDDigitizer, m_pMilIDSplitGBList, 2, M_STOP, M_DEFAULT, ProcessingFunctionTopCam, &m_UserHookData);
	else
		MdigProcess(m_milIDDigitizer, m_pMilIDSplitGBList, 2, M_STOP, M_DEFAULT, ProcessingFunctionBottomCam, &m_UserHookData);
	m_bGrabOpen = FALSE;
}



void CMatroxChannel::SetCoverGlassBuffer(CMilBuffer ** ppCGSplitImgs)
{
	m_ppCGSplitImgs = ppCGSplitImgs;
}

// 실제 Scan해야할 Line수 설정(CG검사기의 경우 Image Y길이의 3배입력)
void CMatroxChannel::SetRealLineToScan(int nRealLineToScan)
{
	m_lscanRealLength = nRealLineToScan;
}

void CMatroxChannel::SetSelectedLightNo(int nSelectedLight)
{
	m_nSelectedLight = nSelectedLight;
}

void CMatroxChannel::SetDataAquMode(BOOL bMode)
{
	m_bIsDataAquMode = bMode;
}

