// CamDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CGInspector.h"
#include "CamDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "Repository.h"
#include "CGInspectorDoc.h"
#include "CGInspectorView.h"
#include "MainFrm.h"
#include "SCAlgoBase.h"
#include "Compo.h"
#include "CamPart.h"
#include "CompoTracker.h"
#include "TabDetailLogDlg.h"
#include "SCFidAlgo.h"
#include "LineEquation.h"
#include "SCResult.h"
#include "Status.h"
#include <UniUtility.h>
#include <stdlib.h>
#include <Matrox.h>
#include <vidi_runtime.h>
#include <iostream>
#include <fstream>
#include <atlconv.h>
#include "Markup.h"


// CCamDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCamDlg, CDialogEx)

BOOL CCamDlg::m_bRun = FALSE;
CCamDlg::CCamDlg(tCam nTypeCam, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAM, pParent)
//	, m_pMilCamView(NULL)
	, m_strCamName(_T(""))
	, m_strCamTag(_T(""))
	, m_pOverlayDC(NULL)
	, m_bInitialized(FALSE)
	, m_pSelectedCompo(NULL)
	, m_iCamBuildNo(-1)
	, m_bRequestReconnect(FALSE)
	, m_nTypeCam(0)
	, m_iCAM_HRES(0)
	, m_iCAM_VRES(0)
	, m_pMatroxChannel(NULL)
	, m_nSelectedLight(0)
	, m_nTopSideInspType(0)
{
	m_nTypeCam = nTypeCam;
	m_strCamName = _T("Cam");
	for (int i = 0; i < 3; i++)
		m_ppSplitLineImg[i] = NULL;
}

CCamDlg::~CCamDlg()
{

}

void CCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCamDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_REQ_INSP, &CCamDlg::OnReqInsp)
END_MESSAGE_MAP()


// CCamDlg 메시지 처리기입니다.


BOOL CCamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	if (!m_ppSplitLineImg[0])
	{
		m_pMilCamView = CRepository::Instance()->m_MilLib.NewImage();
		m_pMilCamProc = CRepository::Instance()->m_MilLib.NewImage();
		m_ppSplitLineImg[0] = CRepository::Instance()->m_MilLib.NewImage();
		m_ppSplitLineImg[0]->SetAttribute(M_IMAGE + M_PROC + M_DISP + M_DIB);
		m_ppSplitLineImg[1] = CRepository::Instance()->m_MilLib.NewImage();
		m_ppSplitLineImg[1]->SetAttribute(M_IMAGE + M_PROC + M_DISP + M_DIB);
		m_ppSplitLineImg[2] = CRepository::Instance()->m_MilLib.NewImage();
		m_ppSplitLineImg[2]->SetAttribute(M_IMAGE + M_PROC + M_DISP + M_DIB);
	}

	SetCamType((tCam)m_nTypeCam);//Camera Type Select And m_pMilCamView, m_pMilCamDraw, m_pMilCamProc Alloc

	if (!CStatus::Instance()->m_bIsOffLine)
	{
		CString strDCFFile;
		strDCFFile = CAppInfo::GetAppDir() + _T("\\LA-CM-08K08A_mono.dcf");
		m_pMatroxChannel = CRepository::Instance()->m_Grabber[m_nTypeCam].NewChannel(strDCFFile.GetBuffer());
		m_pMatroxChannel->m_strChannelName = m_szChName[m_nTypeCam];
		m_pMatroxChannel->SetMilBuffer(m_pMilCamProc, m_pMilCamView); //Display용 MilBuffer설정
		m_pMatroxChannel->Create();
		((CMatroxChannel *)(m_pMatroxChannel))->SetRealLineToScan(CStatus::Instance()->GetCGLength()*3);
		((CMatroxChannel *)(m_pMatroxChannel))->m_ppCGSplitImgs = m_ppSplitLineImg;  ///실제 이미지를 가지고 있을 버퍼
		((CMatroxChannel *)(m_pMatroxChannel))->SplitGBChildren();
		((CMatroxChannel *)(m_pMatroxChannel))->SetGrabTimeInfinite();
		((CMatroxChannel *)(m_pMatroxChannel))->m_pCamDlg = this;
	}

	m_pMilDisp = CRepository::Instance()->m_MilLib.NewDisplay();
	m_pMilCamView->EnableGraph();
	m_pMilCamView->SelectWindow(m_pMilDisp, this);
	m_pMilDisp->EnableOverlay();
	m_pMilDisp->EnableGraph();
	m_pMilDisp->FilWindow();
	
	ClearView();
	CombineViDi();

	m_strCamTag = m_strCamName;
	m_pMilDisp->FontScale(50, 50);
	m_pMilDisp->SetColor(M_COLOR_YELLOW);
	m_pMilDisp->OutputText(30, 30, m_strCamTag.GetBuffer());
	m_pMilDisp->EnableAutoPanZoom();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCamDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CCamDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CCamDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialogEx::OnMouseMove(nFlags, point);
}


void CCamDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCamDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rectMy;
	GetClientRect(&rectMy);
//	DrawCross(m_pMilCamView);
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}

// 하나라도 걸리는것이 있으면 양수 걸리는것이 하나도 없으면 음수
int CCamDlg::CamViewHitTest(CPoint Point, CCompo * &pFindedCompo)
{
	return -1;
}

BOOL CCamDlg::IsTeachingMode()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	CGInspectorView *pView = (CGInspectorView *)pFrame->GetActiveView();
//	return pView->IsTeachingMode();
	return TRUE;
}


BOOL CCamDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CCamDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//if (pMsg->hwnd == this)
	//{

	//}
	return CDialogEx::PreTranslateMessage(pMsg);
}



int CCamDlg::LoadBufImage(CString strFileName, MIL_INT FileFormat)
{
//	m_pMilCamProc->BufImport(strFileName, FileFormat);
	//m_pMilCamView->Copy(m_pMilCamProc);
	//m_pMilCamDraw->Copy(m_pMilCamProc);
//	m_pMilCamView->BufImport(strFileName, FileFormat);
	return 0;
}

int CCamDlg::LoadBufImage(CMilBuffer *pImage, int nLight)
{
	if(nLight == -1)
		m_ppSplitLineImg[0]->Copy(pImage);
	else
		m_ppSplitLineImg[nLight]->Copy(pImage);

	m_pMilCamProc->Copy(pImage);
	m_pMilCamView->Copy(pImage);
//m_pMilCamDraw->Copy(pImage);
	return 0;
}


void CCamDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

// RV: -1 no Inspect data 
//     0 Good
//    1~ NG
int CCamDlg::Inspect(CSCResult *pResult, int nLight)
{
	int iStartTime = GetTickCount();
	if (nLight == -1)nLight = 0;
#ifndef _NOVIDI
	USES_CONVERSION;
	VIDI_UINT status;
	char *pWSName = CStatus::Instance()->m_szWSName;
	char pStreamName[10] = { 0 };
	switch (m_nTypeCam)
	{
	case CAMTYPE_TOP:
		sprintf(pStreamName, "Top%d", nLight);
		break;
	case CAMTYPE_BOTTOM:
		sprintf(pStreamName, "Bottom");
		break;
	}

	VIDI_BUFFER result_buffer;
	status = vidi_init_buffer(&result_buffer);
	if (status != VIDI_SUCCESS)
	{
		clog << "failed to initialize result buffer" << endl;
		vidi_deinitialize();
		return -1;
	}

	// as of ViDi Suite 3.0.0, samples are processed in a few steps instead of just calling vidi_runtime_process
	// the first step is to initialize the sample

	status = vidi_runtime_create_sample(pWSName, pStreamName, "my_sample");
	if (status != VIDI_SUCCESS)
	{
		clog << "failed to initialize sample" << endl;
		vidi_deinitialize();
		return -1;
	}

	//// then add the image to be processed
	status = vidi_runtime_sample_add_image(pWSName, pStreamName, "my_sample", &m_vidiCamImg[nLight]);
	if (status != VIDI_SUCCESS)
	{
		clog << "failed to add image" << endl;
		vidi_deinitialize();
		return -1;
	}

	// create and initialize a buffer to be used whenever data is returned from the library
	VIDI_BUFFER buffer;
	status = vidi_init_buffer(&buffer);
	if (status != VIDI_SUCCESS)
	{
		clog << "failed to initialize vidi buffer" << endl;
		return -1;
	}

		
	/**
	* subsequently process the sample for each tool
	* here, we know that the tool that's being processed is called "analyze". If you do not know the name of the
	* tool or tools that are being processed, you can use vidi_runtime_list_tools(). Since there is only one tool,
	* this is called only once. You can also trigger the whole chain to process by calling
	* vidi_runtime_process_sample for the last tool in the chain.
	*/
	status = vidi_runtime_list_tools(pWSName, pStreamName, &buffer);
	status = vidi_runtime_sample_process(pWSName, pStreamName, "Analyze", "my_sample", "");
	if (status != VIDI_SUCCESS)
	{
		vidi_get_error_message(status, &buffer);
		clog << "failed to process sample: " << buffer.data << endl;
		vidi_deinitialize();
		return -1;
	}

	// the next step is to get the results
	status = vidi_runtime_get_sample(pWSName, pStreamName, "my_sample", &result_buffer);
	if (status != VIDI_SUCCESS)
	{
		clog << "failed to get results from sample" << endl;
		vidi_deinitialize();
		return -1;
	}

	//Save Result for test.
	//ofstream ofs("d:\\result.xml");
	//ofs << result_buffer.data;

	//VIDI_IMAGE LastImage;
	//vidi_init_image(&LastImage);
	//LastImage.channels = 1;
	//LastImage.channel_depth = VIDI_IMG_8U;
	//LastImage.height = 20000;
	//LastImage.width = 8192;
	//LastImage.step = LastImage.width * 1;
	//LastImage.data = new uint8_t[LastImage.height * LastImage.width * LastImage.channels];
	//status = vidi_runtime_sample_get_image(pWSName, pStreamName,"Analyze", "my_sample|0:-1", "", &LastImage);
	//if (status != VIDI_SUCCESS)
	//{
	//	std::cerr << "failed to get LastImage";
	//	vidi_deinitialize();
	//	return -1;
	//}
	//status = vidi_save_image("d:\\LastImage.png", &LastImage);
	//if (status != VIDI_SUCCESS)
	//{
	//	std::cerr << "failed to save image";
	//	vidi_deinitialize();
	//	return -1;
	//}
	//vidi_free_image(&LastImage);


	size_t sizeStr = strlen(result_buffer.data);
	TCHAR *pszResultBuffer = A2W(result_buffer.data);
	CPtrArray listPointsGroup;
	CPtrList *pListPoints;
	tScoreSet dblScore;
	ParseResult(pszResultBuffer, dblScore, listPointsGroup);
	int nDefect = dblScore.nDefect;
	INT_PTR nList = listPointsGroup.GetCount();
	for (int i = 0; i < nList; i++)
	{
		pListPoints = (CPtrList *)listPointsGroup.GetAt(i);
		if (CStatus::Instance()->IsDrawDefect())
		{
			m_pMilCamView->SetColor(M_COLOR_RED);
			m_pMilCamView->DrawPolygon(pListPoints);
		}
		delete pListPoints;
	}


	vidi_free_buffer(&result_buffer);
	vidi_free_buffer(&buffer);
	// now that we've gotten the results, we can free the sample
	status = vidi_runtime_free_sample(pWSName, pStreamName, "my_sample");
	if (status != VIDI_SUCCESS)
	{
		clog << "failed to free sample" << endl;
		vidi_deinitialize();
		return -1;
	}
	int iEndTime = GetTickCount();
	CString strResult;
	strResult.Format(_T("%s:LightNo.[%d], Inspection time:%dms, nDefect=%d[Score:Low=%4.2f,High=%4.2f,this=%4.2f]"), 
		                  m_strCamName, nLight, iEndTime-iStartTime, dblScore.nDefect, dblScore.dblThresholdLow, dblScore.dblThresholdHight, dblScore.dblScore);
	CTabDetailLogDlg::Instance()->Puts(strResult);
#endif
	return 0;
}

void CCamDlg::RefreshView()
{
//	m_pMilCamView->Copy(m_pMilCamProc);
}


void CCamDlg::DrawToView()
{
//	DrawCross(m_pMilCamDraw);
//	m_pMilCamView->Copy(m_pMilCamDraw);
}



//bIsNG true->NG, false->good
int CCamDlg::SaveImage(BOOL bIsNG)
{
	CString strNGSaveFolder = CStatus::Instance()->m_strNGSaveFolder;
	CString strTestImgSaveFolder;
	CString strFileName;
	static CString strPrevFileName = _T("");
	static int iIndex = 0;
	CTime curTime = CTime::GetCurrentTime();

	CString strState;
	if (bIsNG)
	{
		strState = _T("NG");
	}
	else
	{
		strState = _T("Good");
	}
	do {
		strFileName = strState + curTime.Format(_T("%y%m%d%H%M%S"));
		strFileName.Format(_T("%s-%s-%d"), m_strCamName.GetBuffer(), strFileName.GetBuffer(), iIndex);
		iIndex++;
	} while (strFileName == strPrevFileName);

	iIndex = 0;
	strPrevFileName = strFileName;
	CTime tmCurrent = CTime::GetCurrentTime();
	CString strDate = tmCurrent.Format(_T("%y%m%d"));
	CString strSaveFolder = strNGSaveFolder + _T("\\") + strDate;
	CString strRemoteFolder = strDate;
	CFileInfo::CreateFolder(strSaveFolder.GetBuffer());

	strTestImgSaveFolder = strSaveFolder + _T("\\") + strFileName + _T(".bmp");
	strNGSaveFolder = strSaveFolder + _T("\\") + strFileName + _T(".jpg");
	strRemoteFolder = strRemoteFolder + _T("\\") + strFileName + _T(".jpg");
	int iStartTime = GetTickCount();
//	m_pMilCamDraw->Export(strNGSaveFolder, M_JPEG_LOSSY);
//	if(CStatus::Instance()->m_bSaveTestImage)m_pMilCamProc->Export(strTestImgSaveFolder, M_BMP);
	int iEndTime = GetTickCount();
	TRACE(_T("\nSave Time %d"), iEndTime - iStartTime);
	BOOL bErr;
	if (strRemoteFolder.GetLength() > 29)
	{
		bErr = TRUE;
	}
	return 0;
}


int CCamDlg::Grab()
{
	if (CStatus::Instance()->m_bIsOffLine)return -1;
//	m_pEuresysChannel->Grab();
	return 0;
}

BOOL CCamDlg::IsGrabDone()
{
//	return m_pEuresysChannel->IsGrabDone();
	return true;
}

//촤영과 전송대기, 밀버퍼로 옮기는 것까지수행
int CCamDlg::DoGrab()
{
	if (CStatus::Instance()->m_bIsOffLine)return -1;

//	m_pEuresysChannel->Grab();
	while (!IsGrabDone());
	DrawToView();
	return 0;
}


void CCamDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CCamDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pMilDisp->Close();
	if (m_pMatroxChannel)m_pMatroxChannel->Close();
	m_pMilCamView->Close();
	m_pMilCamProc->Close();
	m_ppSplitLineImg[0]->Close();
	m_ppSplitLineImg[1]->Close();
	m_ppSplitLineImg[2]->Close();
	ViDiClose();

	CDialogEx::OnClose();
}

void CCamDlg::ClearView()
{
	m_pMilCamView->Copy(m_pMilCamProc);
	m_ppSplitLineImg[0]->Copy(m_pMilCamProc);
	m_ppSplitLineImg[1]->Copy(m_pMilCamProc);
	m_ppSplitLineImg[2]->Copy(m_pMilCamProc);
	//DrawCross(m_pMilCamDraw);
}

void CCamDlg::DrawCross(CMilBuffer *pDrawBuffer)
{
	if (!CStatus::Instance()->m_bDrawCross)return;
	CRect rectScrn;
	GetClientRect(&rectScrn);
	double width = m_iCAM_HRES - 1;
	double height = m_iCAM_VRES - 1;
	pDrawBuffer->SetColor(RGB(100,100,100));
	pDrawBuffer->Line(0, 0, m_iCAM_HRES-1, m_iCAM_VRES-1);
	pDrawBuffer->Line(0, m_iCAM_VRES - 1, m_iCAM_HRES - 1, 0);
	CRect rectMiddle((int)(width/4), (int)(height/4), (int)(width - width/4), (int)(height - height/4));
	rectMiddle.DeflateRect(2, 2);
	for (int i = -2; i < 3; i++)
	{
		pDrawBuffer->Rect(rectMiddle);
		pDrawBuffer->Line(0, (height /2) + i, width, (height / 2) + i);
		pDrawBuffer->Line((width / 2) + i, 0, (width / 2) + i, height);
		rectMiddle.InflateRect(1, 1);
	}
//		pDrawBuffer->Line(, 1.0, (width / 2) +(double)i, height-1);
}


BOOL CCamDlg::IsPixelCalibrated()
{
//	if (CStatus::Instance()->m_CamFov[m_iCamBuildNo].dblCX == 0)return FALSE;
//	if (CStatus::Instance()->m_CamFov[m_iCamBuildNo].dblCY == 0)return FALSE;
	return TRUE;
}


BOOL CCamDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));

	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CCamDlg::SetImageSize(int cx, int cy)
{
	if (!CStatus::Instance()->m_bIsOffLine)
	{ //Online
		m_pMilCamProc->Alloc2D(CSize(cx, cy));
		m_pMilCamView->SetAttribute(M_IMAGE + M_DISP + M_PACKED + M_BGR24);
		m_pMilCamView->AllocColor2D(CSize(cx, cy));
		//m_pMilCamDraw->AllocColor2D(CSize(cx, cy));
	}
	else
	{ //OffLine
		m_pMilCamProc->Alloc2D(CSize(cx, cy));
		m_pMilCamView->SetAttribute(M_IMAGE + M_DISP + M_PACKED + M_BGR24);
		m_pMilCamView->AllocColor2D(CSize(cx, cy));
		//m_pMilCamDraw->AllocColor2D(CSize(cx, cy));
	}
}

void CCamDlg::SetCamType(tCam nTypeCam)
{
	int rv = 0;
	CSize sizeImg(CStatus::Instance()->m_sizeTopCam.cx, CStatus::Instance()->GetCGLength());
	CSize sizeSideImg(CStatus::Instance()->m_sizeSideLeftCam.cx, CStatus::Instance()->GetCGLength());

	switch (nTypeCam)
	{
	case CAMTYPE_TOP:
	case CAMTYPE_BOTTOM:
		m_ppSplitLineImg[0]->Alloc2D(sizeImg);
		m_ppSplitLineImg[1]->Alloc2D(sizeImg);
		m_ppSplitLineImg[2]->Alloc2D(sizeImg);
		SetImageSize(sizeImg.cx, sizeImg.cy);
		break;
	case CAMTYPE_LEFTSIDE:
	case CAMTYPE_RIGHTSIDE:
		SetImageSize(sizeSideImg.cx, sizeSideImg.cy);
		break;
	}
}


void CCamDlg::SwitchLightImage(int nSelectedLight)
{
	m_nSelectedLight = nSelectedLight;
	((CMatroxChannel *)(m_pMatroxChannel))->SetSelectedLightNo(nSelectedLight);
	m_pMilCamView->Copy(m_ppSplitLineImg[nSelectedLight]);
}

void CCamDlg::SetDataAquMode(BOOL bMode)
{
	((CMatroxChannel *)(m_pMatroxChannel))->SetDataAquMode(bMode);
}


void CCamDlg::SetRunStop(int OPMode)
{
	if (CStatus::OPAUTO != OPMode)
	{//OPManual  
		((CMatroxChannel *)(m_pMatroxChannel))->ProcessStop();
	}
	else
	{//OPAuto
		ClearView();
		((CMatroxChannel *)(m_pMatroxChannel))->GrabLine();
	}

}

int CCamDlg::ParseResult(TCHAR * szData, tScoreSet & dblViDiScore, CPtrArray &listPointGroup)
{
	CString strPoints;
	CMarkup xml;
	CdblPoint dblOffset(0.,0.);

	xml.SetDoc(szData);
	int pos = 0;
	dblViDiScore.nDefect = 0;
	
	if (xml.FindElem(_T("sample")))
	{
		if (xml.FindChildElem(_T("image")))
		{
			xml.IntoElem();
			if (xml.FindChildElem(_T("marking")))
			{
				xml.IntoElem();
				if (xml.FindChildElem(_T("view")))
				{
					xml.IntoElem();
					CString strPose = xml.GetAttrib(_T("pose"));
					GetViewOffset(strPose, dblOffset);

					if (xml.FindChildElem(_T("red")))
					{
						xml.IntoElem();
						CString strThreshold = xml.GetAttrib(_T("threshold"));
						CString strTemp;
						strThreshold.TrimLeft(_T("["));
						strThreshold.TrimRight(_T("]"));
						strTemp = strThreshold.Tokenize(_T(","), pos);
						dblViDiScore.dblThresholdLow = _wtof(strTemp.GetBuffer());
						strTemp = strThreshold.Tokenize(_T(","), pos);
						dblViDiScore.dblThresholdHight = _wtof(strTemp.GetBuffer());;
						strThreshold = xml.GetAttrib(_T("score"));
						dblViDiScore.dblScore = _wtof(strThreshold.GetBuffer());
						if (xml.FindChildElem(_T("region")))
						{
							int index = dblViDiScore.nDefect;
							CPtrList *plistPoint = new CPtrList;
							xml.IntoElem();
							strPoints = xml.GetAttrib(_T("points"));
							GetPointList(strPoints, plistPoint, dblOffset);
							listPointGroup.Add(plistPoint);
							dblViDiScore.nDefect++;
							while (xml.FindElem(_T("region")))
							{
								plistPoint = new CPtrList;
								strPoints = xml.GetAttrib(_T("points"));
								GetPointList(strPoints, plistPoint, dblOffset);
								listPointGroup.Add(plistPoint);
								dblViDiScore.nDefect++;
							}
						}

					}
				}

			}
		}
	}




	return 0;
}


void CCamDlg::GetViewOffset(CString strPoints, CdblPoint& dblOffset)
{
	CString strToken;
	int pos = 0;
	int iIndex = 0;

	CdblPoint *pDblPoint = NULL;

	while ((strToken = strPoints.Tokenize(_T(" ,"), pos)) != _T(""))
	{
		switch (iIndex)
		{
		case 4:
			dblOffset.dblPosX = _wtof(strToken);
			break;
		case 5:
			dblOffset.dblPosY = _wtof(strToken);
			break;
		}
		iIndex++;
	}
}


void CCamDlg::GetPointList(CString strPoints, CPtrList *plistPoint, CdblPoint& dblOffset)
{
	CString strToken;
	int pos = 0;
	int iIndex = 0;

	CdblPoint *pDblPoint = NULL;

	while ((strToken = strPoints.Tokenize(_T(" ,"), pos)) != _T(""))
	{
		if ((iIndex % 2) == 0)
		{
			pDblPoint = new(CdblPoint);
			pDblPoint->dblPosX = _wtof(strToken)+dblOffset.dblPosX;
		}
		else
		{
			pDblPoint->dblPosY = _wtof(strToken)+dblOffset.dblPosY;
			plistPoint->AddTail(pDblPoint);
		}
		//		wprintf(_T("\n%d:%s"), iIndex, strToken.GetBuffer());
		iIndex++;
	}
}


void CCamDlg::ViDiClose()
{
#ifndef _NOVIDI
	char *pWSName = CStatus::Instance()->m_szWSName;

	char *pStreamName = NULL;
	switch (m_nTypeCam)
	{
	case CAMTYPE_TOP:
		pStreamName = "Top0";
		break;
	case CAMTYPE_BOTTOM:
		pStreamName = "Bottom";
		break;
	}

	// this will free all images and buffers which have not yet been freed
	vidi_deinitialize();
#endif
}


void CCamDlg::CombineViDi()
{
#ifndef _NOVIDI
	int nWidth = 0;
	switch (m_nTypeCam)
	{
	case CAMTYPE_TOP:
		nWidth = CStatus::Instance()->m_sizeTopCam.cx;
		break;
	case CAMTYPE_BOTTOM:
		nWidth = CStatus::Instance()->m_sizeBottomCam.cx;
		break;
	}

	for (int i = 0; i < 3; i++)
	{
		vidi_init_image(&m_vidiCamImg[i]);
		m_vidiCamImg[i].channels = 1;
		m_vidiCamImg[i].channel_depth = VIDI_IMG_8U;
		m_vidiCamImg[i].height = CStatus::Instance()->GetCGLength();
		m_vidiCamImg[i].width = nWidth;
		m_vidiCamImg[i].step = m_vidiCamImg[i].width * 1;
		m_vidiCamImg[i].data = (BYTE *)m_ppSplitLineImg[i]->GetAddr();
	}
#endif
}

LRESULT CCamDlg::OnReqInsp(WPARAM wparam, LPARAM lparam)
{
	switch (m_nTypeCam)
	{
	case CAMTYPE_TOP:
		if(m_nTopSideInspType == 0)
			Inspect(NULL, m_nSelectedLight);
		else
		{
			Inspect(NULL, 0);
			Inspect(NULL, 1);
			Inspect(NULL, 2);
		}
		break;
	case CAMTYPE_BOTTOM:
		Inspect();
		break;
	}


	return 0;
}


void CCamDlg::SetTopSideInspType(int nInspType)
{
	m_nTopSideInspType = nInspType;
}
