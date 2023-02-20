/**
  @file		CInspectionPattern.cpp
  @brief    CInspectionPattern Source File
  @author   노규현

  메인 검사 소스 파일.\n
  */

#include "pch.h"
#include "Repository.h"
#include "CInspectionClass.h"
#include <list>

#ifdef _DEBUG
//#define SAVE_INSP_IMAGE
#endif
#define NGH_NEW_TEST

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif
#ifdef MIL_USE
CInspectionClass* g_MainClass;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInspectionPattern::CInspectionPattern()
{
	m_dwPatternResultColor[0] = PATTERN_0_RESULT_COLOR;
	m_dwPatternResultColor[1] = PATTERN_1_RESULT_COLOR;
	m_dwPatternResultColor[2] = PATTERN_2_RESULT_COLOR;
	m_dwPatternResultColor[3] = PATTERN_3_RESULT_COLOR;
	m_dwPatternResultColor[4] = PATTERN_4_RESULT_COLOR;

	m_ResultData.isAllResult = FALSE;
	m_ResultData.isCountResult = FALSE;
	m_ResultData.isOverlapResult = FALSE;
	m_ResultData.stEachResultData.clear();
	memset(&m_InspPara, NULL, sizeof(INSPEC_PARA));

	m_nPatternModelIndex = 0;

	for (int i = 0; i < EACH_INSP_THREAD_CNT; i++)
	{
		m_bEachInspecting[i] = FALSE;
		m_bEachInspectionEnd[i] = FALSE;
		m_pThreadEachInsp[i] = NULL;
		m_milModelFile[i] = NULL;
		m_milInspImage[i] = NULL;
	}

	m_bResultDisplay = FALSE;
}

CInspectionPattern::~CInspectionPattern()
{
	for (int i = 0; i < EACH_INSP_THREAD_CNT; i++)
	{
		if (m_milModelFile[i] != NULL)
		{
			MmodFree(m_milModelFile[i]);
			m_milModelFile[i] = NULL;
		}

		if (m_EachResultData[i].stEachResultData.size() > 0)
			m_EachResultData[i].stEachResultData.clear();


		
		if (m_milInspImage[i] != NULL)
		{
			m_milInspImage[i]->Close();
			m_milInspImage[i] = NULL;
		}

		m_bEachInspectionStart[i] = FALSE;
		m_bEachInspectionThreadEnd[i] = TRUE;
	}

	ReleaseEachThread();
	
	if (m_ResultData.stEachResultData.size() > 0)
		m_ResultData.stEachResultData.clear();
}

void CInspectionPattern::InitImage(long nWidth, long nHeight, BOOL isInit)
{
	m_ImageSize.SetSize(nWidth, nHeight);
}
/**
  @brief PatternAddFunc
  Pattern 등록 함수
*/
BOOL CInspectionPattern::PatternAddFunc(MIL_ID milGraphicList, MIL_ID milPatternBuff, MIL_ID milMaskBuff, int nPtnIdx, BOOL isMask)
{
	MappControl(M_ERROR, M_PRINT_DISABLE);
	CString strText;
	strText.Format(_T("%d_PatternAddFunc Start"), m_nPatternModelIndex + 1);
	g_MainClass->WriteLogMessage(strText);

	if (m_milModelFile[nPtnIdx] != NULL)
		MmodFree(m_milModelFile[nPtnIdx]);

	MmodAlloc(CMilSystem::Instance()->GetSystemID(), M_GEOMETRIC, M_DEFAULT, &m_milModelFile[nPtnIdx]);

	CMilBuffer* milInspImage;
	milInspImage = CRepository::Instance()->m_MilLib.NewImage();
	milInspImage->Alloc2D(CSize(m_InspPara.nPatternAddRect.Width(), m_InspPara.nPatternAddRect.Height()));

	//MimConvolve(milPatternBuff, milInspImage->GetIDBuffer(), M_SHARPEN);


	/* Define the model. */
	//MIL_ID milFilterImage;
	//MbufAlloc2d(CMilSystem::Instance()->GetSystemID(), m_InspPara.nPatternAddRect.Width(), m_InspPara.nPatternAddRect.Height(), 8 + M_UNSIGNED, M_IMAGE + M_PROC, &milFilterImage);
	//MimConvolve(*milPatternBuff, milFilterImage, M_SHARPEN2);
	//MmodDefine(m_milModelFile[nPtnIdx], M_IMAGE, milFilterImage, 0, 0, m_InspPara.nPatternAddRect.Width(), m_InspPara.nPatternAddRect.Height());
	MmodDefine(m_milModelFile[nPtnIdx], M_IMAGE, milPatternBuff, 0, 0, m_InspPara.nPatternAddRect.Width(), m_InspPara.nPatternAddRect.Height());

	/* Set the search speed. */
	MmodControl(m_milModelFile[nPtnIdx], M_CONTEXT, M_SPEED, M_HIGH);

	MmodControl(m_milModelFile[nPtnIdx], M_CONTEXT, M_SMOOTHNESS, m_InspPara.dSmoothness);

	MmodControl(m_milModelFile[nPtnIdx], M_CONTEXT, M_SEARCH_SCALE_RANGE, M_ENABLE);
	MmodControl(m_milModelFile[nPtnIdx], M_CONTEXT, M_ACTIVE_EDGELS, m_InspPara.dActiveEdge);
	MmodControl(m_milModelFile[nPtnIdx], M_ALL, M_NUMBER, M_ALL);
	

	MmodControl(m_milModelFile[nPtnIdx], M_ALL, M_ANGLE, 0);
	MmodControl(m_milModelFile[nPtnIdx], M_ALL, M_ANGLE_DELTA_NEG, 180);
	MmodControl(m_milModelFile[nPtnIdx], M_ALL, M_ANGLE_DELTA_POS, 180);

	MmodControl(m_milModelFile[nPtnIdx], M_ALL, M_SCALE_MIN_FACTOR, 0.8);
	MmodControl(m_milModelFile[nPtnIdx], M_ALL, M_SCALE_MAX_FACTOR, 1.2);

	MmodControl(m_milModelFile[nPtnIdx], M_ALL, M_POLARITY, M_SAME);

	MmodControl(m_milModelFile[nPtnIdx], M_ALL, M_ACCEPTANCE, 60);
	MmodControl(m_milModelFile[nPtnIdx], M_ALL, M_CERTAINTY, 90);

	/* Preprocess the search context. */
	MmodPreprocess(m_milModelFile[nPtnIdx], M_DEFAULT);

	if (!isMask)
		MmodMask(m_milModelFile[nPtnIdx], M_DEFAULT, milMaskBuff, M_DONT_CARE, M_DEFAULT);

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0));
	MmodDraw(M_DEFAULT, m_milModelFile[nPtnIdx], milGraphicList, M_DRAW_EDGES, 0, M_ORIGINAL);
	//m_milModelFile[0] = m_milModelFile[nPtnIdx];

	MmodPreprocess(m_milModelFile[nPtnIdx], M_RESET);

	strText.Format(_T("%d_PatternAddFunc End"), m_nPatternModelIndex + 1);
	g_MainClass->WriteLogMessage(strText);

	milInspImage->Close();
	milInspImage = NULL;

	//MbufFree(milFilterImage);
	MappControl(M_ERROR, M_PRINT_ENABLE);
	return TRUE;
}
/**
  @brief OverlapResultPoint
  Search 된 Pattern 중에 겹치는 패턴 제외
*/
BOOL CInspectionPattern::OverlapResultPoint(double dX, double dY, int nIdx)
{
	int nWidth, nHeight;
	nWidth = m_InspPara.nPatternAddRect.Width() / 2;
	nHeight = m_InspPara.nPatternAddRect.Height() / 2;
	double XPosition, YPosition;
	CRect result_rect;
	POINT pt;
	memset(&pt, NULL, sizeof(POINT));
	pt.x = (LONG)dX;
	pt.y = (LONG)dY;

	for (int i = 0; i < m_EachResultData[nIdx].stEachResultData.size(); i++)
	{
		XPosition = m_EachResultData[nIdx].stEachResultData[i].dCenPosX;
		YPosition = m_EachResultData[nIdx].stEachResultData[i].dCenPosY;
		result_rect.SetRect((int)XPosition - nWidth + 5, (int)YPosition - nHeight + 5, (int)XPosition + nWidth - 5, (int)YPosition + nHeight - 5);

		if (result_rect.PtInRect(pt))
			return TRUE;
	}

	return FALSE;
}

BOOL CInspectionPattern::OverlapResultPoint(double dX, double dY)
{
	int nWidth, nHeight;
	nWidth = m_InspPara.nPatternAddRect.Width() / 2;
	nHeight = m_InspPara.nPatternAddRect.Height() / 2;
	double XPosition, YPosition;
	CRect result_rect;
	POINT pt;
	memset(&pt, NULL, sizeof(POINT));
	pt.x = (LONG)dX;
	pt.y = (LONG)dY;

	for (int i = 0; i < m_ResultData.stEachResultData.size(); i++)
	{
		XPosition = m_ResultData.stEachResultData[i].dCenPosX;
		YPosition = m_ResultData.stEachResultData[i].dCenPosY;
		result_rect.SetRect((int)XPosition - nWidth - 5, (int)YPosition - nHeight - 5, (int)XPosition + nWidth + 5, (int)YPosition + nHeight + 5);
		
		if (result_rect.PtInRect(pt))
			return TRUE;
	}

	return FALSE;
}
/**
  @brief ResultDisplay
  Search 결과 Display
*/
void CInspectionPattern::ResultDisplay(CMilDisplay* milDisplay, CDialogRect *dlgRect, CListCtrl* list_ctrl)
{
	m_ResultData.isOverlapResult = FALSE;
	m_ResultData.isAllResult = TRUE;

	if ((m_bResultDisplay && list_ctrl == NULL) || list_ctrl != NULL)
	{
		m_ResultData.stEachResultData.clear();
		milDisplay->ClearOverlay();
		milDisplay->EnableGraph(FALSE);
		milDisplay->EnableOverlay(FALSE);
		milDisplay->EnableGraph();
		milDisplay->EnableOverlay();
		milDisplay->FontSize(M_FONT_DEFAULT_LARGE);

		for (int i = 0; i < m_EachResultData[0].stEachResultData.size(); i++)
			m_ResultData.stEachResultData.push_back(m_EachResultData[0].stEachResultData[i]);

		for (int j = 0; j < EACH_INSP_THREAD_CNT - 1; j++)
		{
			for (int i = 0; i < m_EachResultData[j + 1].stEachResultData.size(); i++)
			{
				double dX, dY;
				dX = m_EachResultData[j + 1].stEachResultData[i].dCenPosX;
				dY = m_EachResultData[j + 1].stEachResultData[i].dCenPosY;

				if (!OverlapResultPoint(dX, dY))
				{
					m_ResultData.stEachResultData.push_back(m_EachResultData[j + 1].stEachResultData[i]);
				}
			}
		}
	}

	int nOverLapCnt = 0;
	int nIndex = m_nPatternModelIndex * 200;
	for (int i = 0; i < m_ResultData.stEachResultData.size(); i++)
	{
		CRect each_rect;
		int nWidth, nHeight;
		nWidth = 5;// m_InspPara.nPatternAddRect.Width() / 2 / 2 / 2 - 3;// / 2 / 2 / 2;
		nHeight = 5;// m_InspPara.nPatternAddRect.Height() / 2 / 2 / 2 - 3;// / 2 / 2 / 2;

		for (int j = 0; j < 1; j++)
		{
			each_rect.left = (long)m_ResultData.stEachResultData[i].dCenPosX - nWidth - j;
			each_rect.top = (long)m_ResultData.stEachResultData[i].dCenPosY - nHeight - j;
			each_rect.right = (long)m_ResultData.stEachResultData[i].dCenPosX + nWidth + j;
			each_rect.bottom = (long)m_ResultData.stEachResultData[i].dCenPosY + nHeight + j;

			if (m_bResultDisplay)
			{
				if (dlgRect != NULL)
				{
					dlgRect->SetActiveTracker(nIndex + i, TRUE);
					dlgRect->AddTracker(0, each_rect, m_dwPatternResultColor[m_nPatternModelIndex], "");
				}
			}
		}

		nWidth = 14;// (m_InspPara.nPatternAddRect.Width() < m_InspPara.nPatternAddRect.Height()) ? m_InspPara.nPatternAddRect.Width() / 2 / 2 / 2 : m_InspPara.nPatternAddRect.Height() / 2 / 2 / 2;
		if (m_ResultData.stEachResultData[i].isOverlap)
		{
			m_ResultData.isOverlapResult = TRUE;
			nOverLapCnt++;
			for (int j = 0; j < 3; j++)
			{
				each_rect.left = (long)m_ResultData.stEachResultData[i].dCenPosX - (nWidth / 2) - j;
				each_rect.top = (long)m_ResultData.stEachResultData[i].dCenPosY - (nWidth / 2) - j;
				each_rect.right = (long)m_ResultData.stEachResultData[i].dCenPosX + (nWidth / 2) + j;
				each_rect.bottom = (long)m_ResultData.stEachResultData[i].dCenPosY + (nWidth / 2) + j;
				if (m_bResultDisplay)
				{
					milDisplay->SetColor(M_RGB888(255, 0, 0));
					milDisplay->Rect(each_rect);
				}
			}
		}
	}

	CString strText, strText2, strText3, strText4, strText5;
	if (m_ResultData.stEachResultData.size() == m_InspPara.nSearchPatternCnt)
	{
		strText.Format(_T("Teaching Count : %d, Search Count : %d"), m_InspPara.nSearchPatternCnt, (int)m_ResultData.stEachResultData.size());
		m_ResultData.isCountResult = TRUE;
	}
	else
	{
		m_ResultData.isCountResult = FALSE;
		strText.Format(_T("Teaching Count : %d, Search Count : %d"), m_InspPara.nSearchPatternCnt, (int)m_ResultData.stEachResultData.size());
	}

	if (m_ResultData.isCountResult)
		strText2.Format(_T("Count : OK!"));
	else
		strText2.Format(_T("Count : NG!"));


	if (m_ResultData.isOverlapResult)
		strText3.Format(_T("Overlap : NG!(Search Count : %d)"), nOverLapCnt);
	else
		strText3.Format(_T("Overlap : OK!"));


	if (!m_ResultData.isCountResult || m_ResultData.isOverlapResult)
		m_ResultData.isAllResult = FALSE;

	if (m_bResultDisplay)
	{
		
	}

	if (m_ResultData.isAllResult)
	{
		strText4.Format(_T("Total : OK!"));
		milDisplay->SetColor(M_RGB888(0, 255, 0));
	}
	else
	{
		strText4.Format(_T("Total : NG!"));
		milDisplay->SetColor(M_RGB888(255, 0, 0));
	}

	strText5.Format(_T("%d_Pattern_Inspection_Result"), m_nPatternModelIndex + 1);

	if (m_bResultDisplay)
	{
		milDisplay->OutputText(10., 10., strText5.GetBuffer());
		milDisplay->OutputText(10., 40., strText.GetBuffer());
		milDisplay->OutputText(10., 70, strText2.GetBuffer());
		milDisplay->OutputText(10., 100, strText3.GetBuffer());
		milDisplay->OutputText(10., 130, strText4.GetBuffer());
	}

	if (!m_bResultDisplay)
	{
		g_MainClass->WriteLogMessage(strText5);
		g_MainClass->WriteLogMessage(strText);
		g_MainClass->WriteLogMessage(strText2);
		g_MainClass->WriteLogMessage(strText3);
		g_MainClass->WriteLogMessage(strText4);
	}
	m_ResultData.nOverlapCnt = nOverLapCnt;

	if (list_ctrl)
	{
		strText.Format(_T("%d"), m_InspPara.nSearchPatternCnt);
		list_ctrl->SetItemText(m_nPatternModelIndex, 1, strText);
		strText.Format(_T("%d"), (int)m_ResultData.stEachResultData.size());
		list_ctrl->SetItemText(m_nPatternModelIndex, 2, strText);
		strText.Format(_T("%d"), m_ResultData.nOverlapCnt);
		list_ctrl->SetItemText(m_nPatternModelIndex, 3, strText);
	}

	strText.Empty(); strText2.Empty(); strText3.Empty(); strText4.Empty(); strText5.Empty();
}

BOOL CInspectionPattern::PatternModelLoad()
{
	BOOL rslt = TRUE;

	CString strFilePath;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	strFilePath.Format(_T("%s\\Recipe\\%s\\PatternModel_%d.mmf"), strTemp, m_strCurrentRecipe, m_nPatternModelIndex);


	for (int i = 0; i < EACH_INSP_THREAD_CNT; i++)
	{
		if (m_milModelFile[i] != NULL)
		{
			MmodFree(m_milModelFile[i]);
			m_milModelFile[i] = NULL;
		}
		MmodAlloc(CMilSystem::Instance()->GetSystemID(), M_GEOMETRIC, M_DEFAULT, &m_milModelFile[i]);
		MmodStream(strFilePath.GetBuffer(), M_NULL, M_LOAD, M_FILE, M_DEFAULT, M_DEFAULT, &m_milModelFile[i], M_NULL);
	}

	return rslt;
}

void CInspectionPattern::SetPatternModelSave(int nPtnIndex)
{
	PatternModelSave();
}

BOOL CInspectionPattern::PatternModelSave()
{
	BOOL rslt = TRUE;

	CString strFilePath;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	strFilePath.Format(_T("%s\\Recipe\\%s\\PatternModel_%d.mmf"), strTemp, m_strCurrentRecipe, m_nPatternModelIndex);

	if(m_milModelFile[0] != NULL)
		MmodSave(strFilePath, m_milModelFile[0], M_DEFAULT);

	return rslt;
}
/**
  @brief SetInitEachInspectionThread
  검사 Thread 초기화
*/
void CInspectionPattern::SetInitEachInspectionThread()
{
	m_bEachInspectionStart[0] = FALSE;
	m_bEachInspectionStart[1] = FALSE;
	m_bEachInspectionStart[2] = FALSE;

	m_bEachInspectionThreadEnd[0] = FALSE;
	m_bEachInspectionThreadEnd[1] = FALSE;
	m_bEachInspectionThreadEnd[2] = FALSE;

	m_bEachInspectionEnd[0] = FALSE;
	m_bEachInspectionEnd[1] = FALSE;
	m_bEachInspectionEnd[2] = FALSE;

	for (int i = 0; i < EACH_INSP_THREAD_CNT; i++)
	{
		for (int j = 0; j < m_EachResultData[i].stEachResultData.size(); j++)
			m_EachResultData[i].stEachResultData.pop_back();

		m_EachResultData[i].isAllResult = TRUE;
		m_EachResultData[i].isCountResult = TRUE;
		m_EachResultData[i].isOverlapResult = FALSE;
		if (m_EachResultData[i].stEachResultData.size() > 0)
		{
			m_EachResultData[i].stEachResultData.clear();
		}
		int nWidth = m_ImageSize.cx / EACH_INSP_THREAD_CNT;

		if (m_milInspImage[i] != NULL)
		{
			m_milInspImage[i]->Close();
			m_milInspImage[i] = NULL;
		}

		m_milInspImage[i] = CRepository::Instance()->m_MilLib.NewImage();
		m_milInspImage[i]->Alloc2D(CSize(nWidth + IMAGE_CUT_OFFSET, m_ImageSize.cy));
	}

	int nCPUIdx = 0;
	/*nCPUIdx = 1 << ((m_nPatternModelIndex * EACH_INSP_THREAD_CNT) + 1);
	SetThreadAffinityMask(m_pThreadEachInsp[0]->m_hThread, nCPUIdx);
	nCPUIdx = 1 << ((m_nPatternModelIndex * EACH_INSP_THREAD_CNT) + 2);
	SetThreadAffinityMask(m_pThreadEachInsp[1]->m_hThread, nCPUIdx);
	nCPUIdx = 1 << ((m_nPatternModelIndex * EACH_INSP_THREAD_CNT) + 3);
	SetThreadAffinityMask(m_pThreadEachInsp[2]->m_hThread, nCPUIdx);*/

	/*nCPUIdx = 1 << 1;
	SetThreadAffinityMask(m_pThreadEachInsp[0]->m_hThread, nCPUIdx);
	nCPUIdx = 1 << 2;
	SetThreadAffinityMask(m_pThreadEachInsp[1]->m_hThread, nCPUIdx);
	nCPUIdx = 1 << 3;
	SetThreadAffinityMask(m_pThreadEachInsp[2]->m_hThread, nCPUIdx);*/

	for (int i = 0; i < EACH_INSP_THREAD_CNT; i++)
	{
		switch (i)
		{
		case 0:
			m_pThreadEachInsp[0] = AfxBeginThread(EachInspThread1, this, THREAD_PRIORITY_NORMAL);
			break;
		case 1:
			m_pThreadEachInsp[1] = AfxBeginThread(EachInspThread2, this, THREAD_PRIORITY_NORMAL);
			break;
		case 2:
			m_pThreadEachInsp[2] = AfxBeginThread(EachInspThread3, this, THREAD_PRIORITY_NORMAL);
			break;
		}
		nCPUIdx = 1 << (i + 1);
		SetThreadAffinityMask(m_pThreadEachInsp[i]->m_hThread, nCPUIdx);

		if (m_pThreadEachInsp[i] != NULL)
		{
			m_pThreadEachInsp[i]->m_bAutoDelete = FALSE;
			m_pThreadEachInsp[i]->ResumeThread();
		}
	}
}
/**
  @brief StartInspectionThread
  검사 Thread 실행
*/
BOOL CInspectionPattern::StartInspectionThread(CMilBuffer* milImageBuff)
{
	BOOL rslt = FALSE;

	InitResultData();

	m_bEachInspectionStart[0] = FALSE;
	m_bEachInspectionStart[1] = FALSE;
	m_bEachInspectionStart[2] = FALSE;

	m_bEachInspectionThreadEnd[0] = FALSE;
	m_bEachInspectionThreadEnd[1] = FALSE;
	m_bEachInspectionThreadEnd[2] = FALSE;

	m_bEachInspectionEnd[0] = FALSE;
	m_bEachInspectionEnd[1] = FALSE;
	m_bEachInspectionEnd[2] = FALSE;

	if (!m_InspPara.bInspUse)
	{
		m_bEachInspectionEnd[0] = TRUE;
		m_bEachInspectionEnd[1] = TRUE;
		m_bEachInspectionEnd[2] = TRUE;
		return rslt;
	}

	int nWidth = m_ImageSize.cx / EACH_INSP_THREAD_CNT;
	CMilBuffer* milInspImage;
	milInspImage = CRepository::Instance()->m_MilLib.NewImage();
	milInspImage->Alloc2D(CSize(nWidth + IMAGE_CUT_OFFSET, m_ImageSize.cy));
	for (int i = 0; i < EACH_INSP_THREAD_CNT; i++)
	{
		for (int j = 0; j < m_EachResultData[i].stEachResultData.size(); j++)
			m_EachResultData[i].stEachResultData.pop_back();

		m_EachResultData[i].isAllResult = TRUE;
		m_EachResultData[i].isCountResult = TRUE;
		m_EachResultData[i].isOverlapResult = FALSE;
		if (m_EachResultData[i].stEachResultData.size() > 0)
		{
			m_EachResultData[i].stEachResultData.clear();
		}
	}
	for (int i = 0; i < EACH_INSP_THREAD_CNT; i++)
	{
		int nWidth = m_ImageSize.cx / EACH_INSP_THREAD_CNT;

		CRect rect;
		BYTE* pData = NULL;
		pData = DBG_NEW BYTE[(nWidth + IMAGE_CUT_OFFSET) * m_ImageSize.cy];

		switch (i)
		{
		case 0:
			rect.SetRect(0, 0, nWidth + IMAGE_CUT_OFFSET, m_ImageSize.cy);
			break;
		case 1:
			rect.SetRect((m_ImageSize.cx / 2) - ((nWidth + IMAGE_CUT_OFFSET) / 2), 0, (m_ImageSize.cx / 2) + ((nWidth + IMAGE_CUT_OFFSET) / 2) + 1, m_ImageSize.cy);
			break;
		case 2:
			rect.SetRect(m_ImageSize.cx - nWidth - IMAGE_CUT_OFFSET, 0, m_ImageSize.cx, m_ImageSize.cy);
			break;
		}

		milImageBuff->Get2D(pData, rect);

		milInspImage->Put(pData);

		//MimConvolve(milInspImage->GetIDBuffer(), m_milInspImage[i]->GetIDBuffer(), M_SHARPEN);
		m_milInspImage[i]->Copy(milInspImage);

		delete[] pData;
	}

	milInspImage->Close();
	milInspImage = NULL;
	
	m_bEachInspectionStart[0] = TRUE;
	m_bEachInspectionStart[1] = TRUE;
	m_bEachInspectionStart[2] = TRUE;

	return rslt;
}


UINT CInspectionPattern::EachInspThread1(LPVOID pParam)
{
	CInspectionPattern* pdlg = (CInspectionPattern*)pParam;

	pdlg->SetEachInspectionThread(0);

	return 0;
}

UINT CInspectionPattern::EachInspThread2(LPVOID pParam)
{
	CInspectionPattern* pdlg = (CInspectionPattern*)pParam;

	pdlg->SetEachInspectionThread(1);

	return 0;
}

UINT CInspectionPattern::EachInspThread3(LPVOID pParam)
{
	CInspectionPattern* pdlg = (CInspectionPattern*)pParam;

	pdlg->SetEachInspectionThread(2);

	return 0;
}
/**
  @brief WaitEachInspThreadEnd
  검사 Thread 종료 확인
*/
BOOL CInspectionPattern::WaitEachInspThreadEnd(int nIdx)
{
	BOOL rslt = TRUE;
	if (m_pThreadEachInsp[nIdx] != NULL)
	{
		DWORD dwRet = WaitForSingleObject(m_pThreadEachInsp[nIdx]->m_hThread, DEFAULT_TIMEOUT);

		if (dwRet == WAIT_FAILED)
		{
			rslt = FALSE;
		}
		else if (dwRet == WAIT_ABANDONED)
		{
			rslt = FALSE;
		}
		else if (dwRet == WAIT_TIMEOUT)
		{
			rslt = FALSE;
		}
	}

	return rslt;
}
/**
  @brief WaitEachInspThreadEnd
  검사 Thread 종료
*/
BOOL CInspectionPattern::ReleaseEachThread()
{
	BOOL rslt = TRUE;

	for (int i = 0; i < EACH_INSP_THREAD_CNT; i++)
	{
		if (WaitEachInspThreadEnd(i))
		{
			m_bEachInspecting[i] = FALSE;
			m_bEachInspectionEnd[i] = FALSE;

			if (m_pThreadEachInsp[i])
				delete m_pThreadEachInsp[i];

			m_pThreadEachInsp[i] = NULL;

			if (m_milInspImage[i] != NULL)
			{
				m_milInspImage[i]->Close();
				m_milInspImage[i] = NULL;
			}
		}
	}

	return TRUE;
}

void CInspectionPattern::SetEachInspectionThread(int nIdx)
{
	while (TRUE)
	{
		if (m_bEachInspectionStart[nIdx])
		{
			m_bEachInspectionStart[nIdx] = FALSE;
			m_bEachInspecting[nIdx] = TRUE;
			m_bEachInspectionEnd[nIdx] = FALSE;

			SetPatternSearchFunc(m_milInspImage[nIdx], nIdx);

			m_bEachInspecting[nIdx] = FALSE;
			m_bEachInspectionEnd[nIdx] = TRUE;
		}

		if (m_bEachInspectionThreadEnd[nIdx])
			break;

		Sleep(1);
	}
}
/**
  @brief GetAllInspectionEnd
  전체 검사 Thread 종료 확인
*/
BOOL CInspectionPattern::GetAllInspectionEnd()
{
	if (m_bEachInspectionEnd[0] && m_bEachInspectionEnd[1] && m_bEachInspectionEnd[2])
		return TRUE;

	return FALSE;
}
/**
  @brief PatternSearchFunc
  등록된 Pattern 으로 입력된 이미지에서 Search
*/
BOOL CInspectionPattern::PatternSearchFunc(CMilBuffer* milImageBuff, int nIdx)
{
	MappControl(M_ERROR, M_PRINT_DISABLE);
	long lStart = (long)GetTickCount64();
	CString strText;
	strText.Format(_T("%d_Pattern_%d_Each_SearchFunc Thread Start"), m_nPatternModelIndex + 1, nIdx + 1);
	g_MainClass->WriteLogMessage(strText);
	BOOL rslt = TRUE;
	MIL_ID milEachResult;
	MmodAllocResult(CMilSystem::Instance()->GetSystemID(), M_DEFAULT, &milEachResult);
	
	int nCutWidth = m_ImageSize.cx / EACH_INSP_THREAD_CNT;
	int nXOffset = 0;
	MIL_INT nPitch;
	MbufInquire(milImageBuff->GetIDBuffer(), M_PITCH, &nPitch);
	if (nIdx == 1)
		nXOffset = (m_ImageSize.cx / 2) - ((nCutWidth + IMAGE_CUT_OFFSET) / 2);
	else if (nIdx == 2)
		nXOffset = m_ImageSize.cx - nCutWidth - IMAGE_CUT_OFFSET;

	/* Set the search speed. */
	MmodControl(m_milModelFile[nIdx], M_ALL, M_NUMBER, M_ALL);
	MmodControl(m_milModelFile[nIdx], M_CONTEXT, M_NUMBER, M_ALL);
	MmodControl(m_milModelFile[nIdx], M_CONTEXT, M_SPEED, M_HIGH);

	MmodControl(m_milModelFile[nIdx], M_CONTEXT, M_SEARCH_SCALE_RANGE, M_ENABLE);
	//MmodControl(m_milModelFile[nIdx], M_CONTEXT, M_SEARCH_ANGLE_RANGE, M_ENABLE);
	//MmodControl(m_milModelFile[nIdx], M_CONTEXT, M_SEARCH_POSITION_RANGE, M_ENABLE);

	//MmodControl(m_milModelFile[nIdx], M_CONTEXT, M_DETAIL_LEVEL, M_MEDIUM);
	MmodControl(m_milModelFile[nIdx], M_CONTEXT, M_ACTIVE_EDGELS, m_InspPara.dActiveEdge);
	MmodControl(m_milModelFile[nIdx], M_CONTEXT, M_SMOOTHNESS, m_InspPara.dSmoothness);

	//MmodControl(m_milModelFile[nIdx], M_ALL, M_ANGLE, 0);
	//MmodControl(m_milModelFile[nIdx], M_ALL, M_ANGLE_DELTA_NEG, 180);
	//MmodControl(m_milModelFile[nIdx], M_ALL, M_ANGLE_DELTA_POS, 180);

	//MmodControl(m_milModelFile[nIdx], M_ALL, M_SCALE, 1.0);
	MmodControl(m_milModelFile[nIdx], M_ALL, M_SCALE_MIN_FACTOR, 0.8);
	MmodControl(m_milModelFile[nIdx], M_ALL, M_SCALE_MAX_FACTOR, 1.2);

	MmodControl(m_milModelFile[nIdx], M_ALL, M_POLARITY, M_SAME);

	MmodControl(m_milModelFile[nIdx], M_ALL, M_ACCEPTANCE, m_InspPara.dSearchScore);
	//MmodControl(m_milModelFile[nIdx], M_ALL, M_CERTAINTY, 90);

	MmodPreprocess(m_milModelFile[nIdx], M_DEFAULT);

	Delay(100);
	//MIL_INT nContext;
	//MmodInquire(m_milModelFile[nIdx], M_GENERAL, M_PREPROCESSED, &nContext);

	//MIL_ID milFilterImage;
	//MbufAlloc2d(CMilSystem::Instance()->GetSystemID(), nWidth + IMAGE_CUT_OFFSET, m_ImageSize.cy, 8 + M_UNSIGNED, M_IMAGE + M_PROC, &milFilterImage);
	//MimConvolve(milImageBuff->GetIDBuffer(), milFilterImage, M_SHARPEN2 + M_OVERSCAN_FAST);
	//strText.Format(_T("d:\\Test%dtest.bmp"), nIdx);
	//MbufExport(strText, M_BMP, milFilterImage);
	//MmodFind(m_milModelFile[nIdx], milFilterImage, *milResult);
	MmodFind(m_milModelFile[nIdx], milImageBuff->GetIDBuffer(), milEachResult);

	MIL_INT NumResults;
	MmodGetResult(milEachResult, M_DEFAULT, M_NUMBER + M_TYPE_MIL_INT, &NumResults);
 	if (NumResults >= 1)
	{
		int i = 0;
		MIL_INT* Model;
		Model = DBG_NEW MIL_INT[NumResults];
		MIL_DOUBLE* Score, * XPosition, * YPosition, * Angle, * Scale;
		Score = DBG_NEW MIL_DOUBLE[NumResults];
		XPosition = DBG_NEW MIL_DOUBLE[NumResults];
		YPosition = DBG_NEW MIL_DOUBLE[NumResults];
		Angle = DBG_NEW MIL_DOUBLE[NumResults];
		Scale = DBG_NEW MIL_DOUBLE[NumResults];

		MmodGetResult(milEachResult, M_DEFAULT, M_INDEX + M_TYPE_MIL_INT, Model);
		MmodGetResult(milEachResult, M_DEFAULT, M_POSITION_X, XPosition);
		MmodGetResult(milEachResult, M_DEFAULT, M_POSITION_Y, YPosition);
		MmodGetResult(milEachResult, M_DEFAULT, M_ANGLE, Angle);
		MmodGetResult(milEachResult, M_DEFAULT, M_SCALE, Scale);
		MmodGetResult(milEachResult, M_DEFAULT, M_SCORE, Score);

		int nWidth, nHeight;
		nWidth = m_InspPara.nPatternAddRect.Width() / 2;// / 2 / 2 / 2;
		nHeight =  m_InspPara.nPatternAddRect.Height() / 2;// / 2 / 2 / 2;

		for (i = 0; i < NumResults; i++)
		{
			CRect result_rect;
			result_rect.SetRect((int)XPosition[i] - nWidth, (int)YPosition[i] - nHeight, (int)XPosition[i] + nWidth, (int)YPosition[i] + nHeight);

			if (result_rect.left < 0)
				result_rect.left = 0;

			if (result_rect.top < 0)
				result_rect.top = 0;

			/*if (nIdx == 1)
			{
				if (result_rect.right > m_ImageSize.cx / 3 + IMAGE_CUT_OFFSET)
					result_rect.right = m_ImageSize.cx / 3 + IMAGE_CUT_OFFSET;
			}
			else if(nIdx == 0)
			{
				if (result_rect.right > m_ImageSize.cx / 3 + IMAGE_CUT_OFFSET)
					result_rect.right = m_ImageSize.cx / 3 + IMAGE_CUT_OFFSET;
			}
			else
			{
				if (result_rect.right > m_ImageSize.cx)
					result_rect.right = m_ImageSize.cx - 1;
			}*/
			if (result_rect.right > m_ImageSize.cx / 3 + IMAGE_CUT_OFFSET)
				result_rect.right = m_ImageSize.cx / 3 + IMAGE_CUT_OFFSET;

			if (result_rect.bottom > m_ImageSize.cy)
				result_rect.bottom = m_ImageSize.cy;

			//if (result_rect.right > m_ImageSize.cx / 3 + IMAGE_CUT_OFFSET)
			//	continue;

			BYTE* pData;
			pData = DBG_NEW BYTE[result_rect.Width() * result_rect.Height()];
			//if ((Angle[i] <= 15.) || (345. <= Angle[i]) ||
			//	(165. <= Angle[i] && Angle[i] <= 195.))
			//if ((Angle[i] <= 20.) || (160. <= Angle[i] && Angle[i] <= 200.) || (340. <= Angle[i]))
			{
				if (Score[i] >= m_InspPara.dSearchScore)
				{
					double dVal;

					milImageBuff->Get2D(pData, result_rect);
					MIL_INT milCnt = 0;
					MbufGetArc(milImageBuff->GetIDBuffer(), (MIL_INT)XPosition[i], (MIL_INT)YPosition[i], 2, 2, 0, 359, pData, &milCnt);
					long lSum = 0;

					for (int j = 0; j < milCnt; j++)
					{
						lSum += pData[j];
					}
					dVal = (double)(lSum / milCnt);
					/*long lSum = 0;
					int nCnt = 0;
					for (int y = (result_rect.Height() / 2) - 1; y < (result_rect.Height() / 2) + 1; y++)
					{
						for (int x = (result_rect.Width() / 2) - 1; x < (result_rect.Width() / 2) + 1; x++)
						{
							lSum += pData[(y * m_InspPara.nPatternAddRect.Width()) + x];
							nCnt++;
						}
					}
					dVal = lSum / (double)nCnt;*/

					if (dVal <= (double)m_InspPara.nSearchPatternThreshHold)
					{
						if (!OverlapResultPoint(XPosition[i] + nXOffset, YPosition[i], nIdx))
						{
							EACH_RESULT_DATA result_data;
							memset(&result_data, NULL, sizeof(EACH_RESULT_DATA));
							(dVal <= (double)m_InspPara.nSearchPatternOverlapThreshHold) ? result_data.isOverlap = TRUE : result_data.isOverlap = FALSE;
							result_data.dCenPosX = XPosition[i] + nXOffset;
							result_data.dCenPosY = YPosition[i];
							result_data.dScore = Score[i];
							result_data.dCenter5x5AvgVal = dVal;

							m_EachResultData[nIdx].stEachResultData.push_back(result_data);
						}
					}
				}
			}
			delete[] pData;
		}
		
		delete [] Model; delete [] Score; delete [] XPosition; delete [] YPosition; delete [] Angle; delete [] Scale;
	}
	else
	{
		rslt = FALSE;
	}
	MmodPreprocess(m_milModelFile[nIdx], M_RESET);

	long lEnd = (long)GetTickCount64();
	double dTackTime = (double)(lEnd - lStart) / 1000.;
	strText.Format(_T("%d_Pattern_%d_Each_SearchFunc Thread End(Tack Time : %0.3f Sec)"), m_nPatternModelIndex + 1, nIdx + 1, dTackTime);
	g_MainClass->WriteLogMessage(strText);

	//MbufFree(milFilterImage);

	MmodFree(milEachResult);

	strText.Empty();
	MappControl(M_ERROR, M_PRINT_ENABLE);
	return rslt;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CInspectionBlobs::CInspectionBlobs()
{
	memset(&m_stBlobsInspPara, NULL, sizeof(BLOB_INSPEC_PARA));
	m_stBlobsResult.clear();
	m_milBlobsInspImage = NULL;
	m_strBlobsCurrentRecipe.Empty();
	m_dwBlobsResultColor = BLOBS_RESULT_COLOR;

	m_bBlobsInspectionThreadEnd = FALSE;
	m_bBlobsInspectionStart = FALSE;
	m_bBlobsInspecting = FALSE;
	m_bBlobsInspectionEnd = FALSE;
	m_pThreadBlobsInsp = NULL;

	
	m_stBlobsInspPara.dBlobMaxLenght = 10.;
	m_stBlobsInspPara.dBlobMinLenght = 1.;
	m_stBlobsInspPara.dBlobMaxArea = m_stBlobsInspPara.dBlobMaxLenght * m_stBlobsInspPara.dBlobMaxLenght;
	m_stBlobsInspPara.dBlobMinArea = m_stBlobsInspPara.dBlobMinLenght * m_stBlobsInspPara.dBlobMinLenght;
	m_stBlobsInspPara.stSearchAreaPara.nAreaVal = 10000;
	m_stBlobsInspPara.stSearchAreaPara.nBlobOpenClose = 5;

	m_nInspIndex = 0;
}

CInspectionBlobs::~CInspectionBlobs()
{
	m_bBlobsInspectionStart = FALSE;
	m_bBlobsInspectionThreadEnd = TRUE;
	if (GetBlobsThreadEnd())
	{
		m_pThreadBlobsInsp = NULL;
		ReleaseBlobsThread();
	}

	if (m_milBlobsInspImage != NULL)
	{
		m_milBlobsInspImage->Close();
		m_milBlobsInspImage = NULL;
	}
}

UINT CInspectionBlobs::BlobsInspThread(LPVOID pParam)
{
	CInspectionBlobs* pdlg = (CInspectionBlobs*)pParam;

	pdlg->SetBlobsInspectionThread();

	return 0;
}

void CInspectionBlobs::SetBlobsInspectionThread()
{
	while (TRUE)
	{
		if (m_bBlobsInspectionStart)
		{
			m_bBlobsInspectionStart = FALSE;
			SetBlobsInspection();
		}


		if (m_bBlobsInspectionThreadEnd)
			break;

		Sleep(1);
	}
}

/**
  @brief SetStartBlobsThread
  결과 data 초기화 및 검사 이미지 복사 후 Thread 시작
*/
void CInspectionBlobs::SetStartBlobsThread(CMilBuffer* milImage)
{
	m_stBlobsResult.clear();
	//m_milBlobsInspImage->Copy(milImage);

	//MbufCopy(milImage->GetIDBuffer(), m_milBlobsInspImage->GetIDBuffer());

	tImageProperties img_pro;
	milImage->GetImageProperties(&img_pro);

	BYTE* pData;
	pData = DBG_NEW BYTE[img_pro.ImageSize.cx * img_pro.ImageSize.cy];

	milImage->Get2D(pData, CRect(0, 0, img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	m_milBlobsInspImage->Put2D(CSize(0, 0), CSize(img_pro.ImageSize.cx, img_pro.ImageSize.cy), pData);

	delete[] pData;

	m_bBlobsInspectionStart = TRUE;
	
}

/**
  @brief SetPreProcess
  검사 이미지 전처리 함수
  Illuminate -> Rank Filter -> Gain Offset -> Smooth -> Emphasize -> Scale -> Scale -> Illuminate
  위순서로 전처리 진행
*/
void CInspectionBlobs::SetPreProcess(CMilBuffer* srcImage, CMilBuffer* dstImage)
{
	tImageProperties img_pro;
	srcImage->GetImageProperties(&img_pro);
	long long* lHisto;
	long lMax = 0, lMax2 = 0;;
	lHisto = new long long[256];
	CMilBuffer* milProcessRslt1, * milProcessRslt2, *milFilterRslt1, * milFilterRslt2;;
	milProcessRslt1 = CRepository::Instance()->m_MilLib.NewImage();
	milProcessRslt1->Alloc2D(CSize(img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	milProcessRslt2 = CRepository::Instance()->m_MilLib.NewImage();
	milProcessRslt2->Alloc2D(CSize(img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	milFilterRslt1 = CRepository::Instance()->m_MilLib.NewImage();
	milFilterRslt1->Alloc2D(CSize(img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	milFilterRslt2 = CRepository::Instance()->m_MilLib.NewImage();
	milFilterRslt2->Alloc2D(CSize(img_pro.ImageSize.cx, img_pro.ImageSize.cy));


	BYTE* bySrc2;
	bySrc2 = new BYTE[img_pro.ImageSize.cx * img_pro.ImageSize.cy];
	srcImage->Get2D(bySrc2, CRect(0, 0, img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	memset(bySrc2, 255, sizeof(BYTE) * img_pro.ImageSize.cx);
	srcImage->Put2D(CSize(0, 0), img_pro.ImageSize, bySrc2);
	
#ifdef SAVE_INSP_IMAGE
	CString strText;
	strText.Format(_T("d:\\PreProcess0_%d.bmp"), m_nInspIndex);
	srcImage->Export(strText, M_BMP);
#endif

	IlluminateFunc(srcImage, milProcessRslt1, m_stBlobsInspPara.stPreProcess.nIlluminateWH, m_stBlobsInspPara.stPreProcess.dIlluminateFactor);
	

#ifdef SAVE_INSP_IMAGE
	strText.Format(_T("d:\\PreProcess1_%d.bmp"), m_nInspIndex);
	milProcessRslt1->Export(strText, M_BMP);
#endif

	MIL_INT milRank = m_stBlobsInspPara.stPreProcess.nRank;
	if (milRank > 0)
	{
		MimRank(milProcessRslt1->GetIDBuffer(), milFilterRslt1->GetIDBuffer(), /*M_5X5_RECT*/M_3X3_CROSS, milRank, M_GRAYSCALE);
		//milFilterRslt1->Copy(milFilterRslt2);
	}
	else
		milFilterRslt1->Copy(milProcessRslt1);

	milFilterRslt1->Get2D(bySrc2, CRect(0, 0, img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	BYTE* byRslt;
	byRslt = new BYTE[img_pro.ImageSize.cx * img_pro.ImageSize.cy];
	memset(byRslt, NULL, sizeof(BYTE) * img_pro.ImageSize.cx * img_pro.ImageSize.cy);
	int nAddOffset = 0;// nMin + ((nMax - nMin) / 2);
	MilHistogram(milFilterRslt1, 256, lHisto);

	int nMin = 256, nMax = 0;
	for (int i = 0; i < 256; i++)
	{
		if (lHisto[i] > 0)
		{
			nMin = i;
			break;
		}
	}
	nAddOffset = nMin + 30;
	if (nAddOffset > 255)
		nAddOffset = 255;

	nAddOffset = 0;

	GainOffset(bySrc2, byRslt, img_pro.ImageSize.cx, img_pro.ImageSize.cy, img_pro.ImageSize.cx, img_pro.ImageSize.cy, m_stBlobsInspPara.stPreProcess.fGain, m_stBlobsInspPara.stPreProcess.nOffset, nAddOffset);

	milFilterRslt1->Put(byRslt);

	MimConvolve(milFilterRslt1->GetIDBuffer(), milFilterRslt2->GetIDBuffer(), M_SMOOTH);
	//MimConvolve(milFilterRslt1->GetIDBuffer(), milFilterRslt2->GetIDBuffer(), M_SHARPEN);

	EmpahsizeFunc(milFilterRslt2, milFilterRslt1, 7, 1);
#ifdef SAVE_INSP_IMAGE
	strText.Format(_T("d:\\PreProcess2_%d.bmp"), m_nInspIndex);
	milFilterRslt1->Export(strText, M_BMP);
#endif
	
	BYTE* bySrc, * byDst;
	bySrc = new BYTE[img_pro.ImageSize.cx * img_pro.ImageSize.cy];
	byDst = new BYTE[img_pro.ImageSize.cx * img_pro.ImageSize.cy];
	milFilterRslt1->Get2D(bySrc, CRect(0, 0, img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	milFilterRslt2->Get2D(byDst, CRect(0, 0, img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	
	MilHistogram(milFilterRslt1, 256, lHisto);
	MaxVal(&lHisto[1], 240, &lMax2);
	MaxVal(&lHisto[1], lMax2 - 11, &lMax);

	ScaleImage(bySrc, byDst, img_pro.ImageSize, 0, lMax - 1, 8);
	
	milFilterRslt1->Put2D(CSize(0, 0), img_pro.ImageSize, byDst);
#ifdef SAVE_INSP_IMAGE
	strText.Format(_T("d:\\PreProcess3_%d.bmp"), m_nInspIndex);
	milFilterRslt1->Export(strText, M_BMP);
#endif

	milFilterRslt1->Get2D(bySrc, CRect(0, 0, img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	milFilterRslt2->Get2D(byDst, CRect(0, 0, img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	ScaleImage(bySrc, byDst, img_pro.ImageSize, 0, 200, 8);
	milFilterRslt2->Put2D(CSize(0, 0), img_pro.ImageSize, byDst);
#ifdef SAVE_INSP_IMAGE
	strText.Format(_T("d:\\PreProcess4_%d.bmp"), m_nInspIndex);
	milFilterRslt2->Export(strText, M_BMP);
#endif

	IlluminateFunc(milFilterRslt2, milFilterRslt1, m_stBlobsInspPara.stPreProcess.nIlluminateWH, m_stBlobsInspPara.stPreProcess.dIlluminateFactor);

	delete[] lHisto;
	delete[] bySrc;
	delete[] byDst;
	delete[] byRslt;
	delete[] bySrc2;

#ifdef SAVE_INSP_IMAGE
	strText.Format(_T("d:\\PreProcess5_%d.bmp"), m_nInspIndex);
	milFilterRslt1->Export(strText, M_BMP);
#endif

	
	dstImage->Copy(milFilterRslt1);

	milProcessRslt2->Close();
	milFilterRslt1->Close();
	milFilterRslt2->Close();
	milProcessRslt1->Close();
	milProcessRslt1 = NULL;
	milProcessRslt2 = NULL;
	milFilterRslt1 = NULL;
	milFilterRslt2 = NULL;
};

/**
  @brief SetBlobsInspection
  검사 Process 함수
  SetPreProcess -> MimBinarize -> AutoSearchArea -> MaskingImage -> BlobsSearch
*/
void CInspectionBlobs::SetBlobsInspection()
{
	long lStart = (long)GetTickCount64();
	CString strText;
	//strText.Format(_T("Blobs_SearchFunc Thread Start"));
	//g_MainClass->WriteLogMessage(strText);
	m_bBlobsInspecting = TRUE;
	m_bBlobsInspectionEnd = FALSE;

	CMilBuffer* milProcessRslt1, * milProcessRslt2, * milProcessRslt3,* milResult, * milSearchArea;
	milProcessRslt1 = CRepository::Instance()->m_MilLib.NewImage();
	milProcessRslt1->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
	milProcessRslt2 = CRepository::Instance()->m_MilLib.NewImage();
	milProcessRslt2->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
	milProcessRslt3 = CRepository::Instance()->m_MilLib.NewImage();
	milProcessRslt3->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
	milSearchArea = CRepository::Instance()->m_MilLib.NewImage();
	milSearchArea->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
	
	milResult = CRepository::Instance()->m_MilLib.NewImage();
	milResult->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
	int nMinIdx = 200, nMaxIdx = 0;
	MIL_INT64* milHistoVal;
	milHistoVal = new MIL_INT64[256];

	if (m_stBlobsInspPara.stPreProcess.bPreProcess)
	{
		//strText.Format(_T("Blobs_SearchFunc Pre-Process Start"));
		//g_MainClass->WriteLogMessage(strText);
		milProcessRslt1->Copy(m_milBlobsInspImage);
		for (int i = 0; i < PRE_PROCESS_CNT; i++)
		{
			SetPreProcess(milProcessRslt1, milProcessRslt2);
			milProcessRslt1->Copy(milProcessRslt2);
		}
		//strText.Format(_T("Blobs_SearchFunc Pre-Process End"));
		//g_MainClass->WriteLogMessage(strText);

		MilHistogram(milProcessRslt1, 256, milHistoVal);
		long lMax = 0;
		MaxVal(milHistoVal, 256, &lMax);
		nMinIdx = lMax;
		MIL_INT64* milHistoVal2;
		milHistoVal2 = new MIL_INT64[256 - nMinIdx];
		int j = 1;
		for (int i = 0; i < 256 - nMinIdx; i++)
		{
			milHistoVal2[i] = milHistoVal[j++];
		}

		MaxVal(milHistoVal2, 256 - nMinIdx, &lMax);

		nMaxIdx = 256 - nMinIdx + 128;

		delete[] milHistoVal2;
	}
	else
	{
		milProcessRslt1->Copy(m_milBlobsInspImage);
		nMinIdx = m_stBlobsInspPara.nBlobMinThreshHold;
		nMaxIdx = m_stBlobsInspPara.nBlobMaxThreshHold;
	}

	MimBinarize(milProcessRslt1->GetIDBuffer(), milProcessRslt2->GetIDBuffer(), M_IN_RANGE, 0, nMinIdx - 5/*nMinIdx + 10*//*m_stBlobsInspPara.nBlobMaxThreshHold*/);// nMaxIdx);// nMinIdx + m_stBlobsInspPara.nBlobNearbyVal);

	delete[]milHistoVal;

#ifdef SAVE_INSP_IMAGE
	strText.Format(_T("D:\\Binarize_%d.bmp"), m_nInspIndex);
	milProcessRslt2->Export(strText, M_BMP);
#else
	//milProcessRslt2->Export(_T("C:\\X_Ray_Inspection\\Binarize.bmp"), M_BMP);
#endif

	if (m_stBlobsInspPara.stSearchAreaPara.bAutoSearchArea)
	{
		//strText.Format(_T("Blobs_SearchFunc AutoSearchArea Start"));
		//g_MainClass->WriteLogMessage(strText);
		AutoSearchArea(m_milBlobsInspImage, milSearchArea);
		//strText.Format(_T("Blobs_SearchFunc AutoSearchArea End"));
		//g_MainClass->WriteLogMessage(strText);
#ifdef SAVE_INSP_IMAGE
		strText.Format(_T("D:\\AutoSearchArea_%d.bmp"), m_nInspIndex);
		milSearchArea->Export(strText, M_BMP);
#endif

		if (m_stBlobsInspPara.stMaskingPara.isMasking)
		{
			//strText.Format(_T("Blobs_SearchFunc Masking Image Process Start"));
			//g_MainClass->WriteLogMessage(strText);
			CMilBuffer * milMaskImage;
			milMaskImage = CRepository::Instance()->m_MilLib.NewImage();
			milMaskImage->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));

			CString strFilePath;
			wchar_t strTemp[MAX_CHAR_LENG];
			GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
			strFilePath.Format(_T("%s\\Recipe\\%s\\Blobs_Mask.bmp"), strTemp, m_strBlobsCurrentRecipe);
			milMaskImage->BufImport(strFilePath);
			MaskingImage(milMaskImage, milSearchArea);
#ifdef SAVE_INSP_IMAGE
			strText.Format(_T("D:\\MaskingAutoSearchArea_%d.bmp"), m_nInspIndex);
			milSearchArea->Export(strText, M_BMP);
#endif
			milMaskImage->Close();
			milMaskImage = NULL;
			strFilePath.Empty();
			//strText.Format(_T("Blobs_SearchFunc Masking Image Process End"));
			//g_MainClass->WriteLogMessage(strText);
		}
	}
	else
	{
		milSearchArea->Clear(M_COLOR_WHITE);
	}

	//strText.Format(_T("Blobs_SearchFunc BlobsSearch Start"));
	//g_MainClass->WriteLogMessage(strText);
	BlobsSearch(milProcessRslt2, /*milProcessRslt1*/m_milBlobsInspImage, milSearchArea, m_stBlobsInspPara.stSearchAreaPara.bAutoSearchArea);
	//strText.Format(_T("Blobs_SearchFunc BlobsSearch End"));
	//g_MainClass->WriteLogMessage(strText);

	milSearchArea->Close();
	milProcessRslt3->Close();
	milProcessRslt2->Close();
	milProcessRslt1->Close();
	milResult->Close();

	milSearchArea = NULL;
	milProcessRslt3 = NULL;
	milProcessRslt2 = NULL;
	milProcessRslt1 = NULL;
	milResult = NULL;

	m_bBlobsInspecting = FALSE;
	m_bBlobsInspectionEnd = TRUE;
	long lEnd = (long)GetTickCount64();
	double dTackTime = (double)(lEnd - lStart) / 1000.;
	//strText.Format(_T("Blobs_SearchFunc Thread End(Tack Time : %0.3f Sec)"), dTackTime);
	//g_MainClass->WriteLogMessage(strText);
	strText.Empty();
}

/**
  @brief BlobsSearch
  전처리 이미지, 원본 이미지, 검사영역 이미지, 검사영역 이미지 사용 변수를 활용하여 Blobs 검출
  검출된 Blobs의 최소길이 각도, 센터X, 센터Y, 최소 길이, 최대 길이 값을 결과 data 값에 저장
*/
void CInspectionBlobs::BlobsSearch(CMilBuffer* bin_Image, CMilBuffer* gray_Image, CMilBuffer* search_area_image, BOOL isSearch_Area)
{
	CMilBuffer* srcImage;
	srcImage = CRepository::Instance()->m_MilLib.NewImage();
	srcImage->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));

	if (isSearch_Area)
		MblobReconstruct(bin_Image->GetIDBuffer(), search_area_image->GetIDBuffer(), srcImage->GetIDBuffer(), M_RECONSTRUCT_FROM_SEED, M_DEFAULT);
	else
	{
		//srcImage->Copy(bin_Image);
		BYTE* bySrcData;
		bySrcData = new BYTE[m_BlobsImageSize.cx * m_BlobsImageSize.cy];
		bin_Image->Get2D(bySrcData, CRect(0, 0, m_BlobsImageSize.cx, m_BlobsImageSize.cy));
		srcImage->Put2D(CSize(0, 0), m_BlobsImageSize, bySrcData);
		delete[] bySrcData;
	}

	CMilBlob* milBlobs;
	milBlobs = CRepository::Instance()->m_MilLib.NewBlobTool(srcImage);
	milBlobs->m_ResultList.pCXs = NULL;
	milBlobs->m_ResultList.pCYs = NULL;
	milBlobs->m_ResultList.pBoxLeft = NULL;
	milBlobs->m_ResultList.pBoxBottom = NULL;
	milBlobs->m_ResultList.pBoxRight = NULL;
	milBlobs->m_ResultList.pBoxTop = NULL;
	milBlobs->m_ResultList.pArea = NULL;
	milBlobs->AllocFeatureList();
	milBlobs->AllocResult();
	
	milBlobs->SelectFeature(M_AREA);
	milBlobs->SelectFeature(M_MEAN_PIXEL);
	milBlobs->SelectFeature(M_MAX_PIXEL);
	milBlobs->SelectFeature(M_SUM_PIXEL);
	milBlobs->SelectFeature(M_FERET_MAX_DIAMETER);
	milBlobs->SelectFeature(M_FERET_MIN_DIAMETER);
	milBlobs->SelectFeature(M_FERET_MIN_ANGLE);

	milBlobs->SelectFeature(M_CENTER_OF_GRAVITY_X);
	milBlobs->SelectFeature(M_CENTER_OF_GRAVITY_Y);
	milBlobs->SelectFeature(M_BOX_X_MIN);
	milBlobs->SelectFeature(M_BOX_Y_MIN);
	milBlobs->SelectFeature(M_BOX_X_MAX);
	milBlobs->SelectFeature(M_BOX_Y_MAX);
	milBlobs->SelectFeature(M_FERET_X);
	milBlobs->SelectFeature(M_FERET_Y);
	milBlobs->SelectFeature(M_LENGTH);
	milBlobs->SelectFeature(M_RECTANGULARITY);

	MblobControl(milBlobs->m_milResult, M_NUMBER_OF_FERETS, 180);
	MblobControl(milBlobs->m_milResult, M_RESULT_OUTPUT_UNITS, M_PIXEL);

	MblobCalculate(milBlobs->m_pMilBuffer->GetIDBuffer(), gray_Image->GetIDBuffer(), milBlobs->m_featureList, milBlobs->m_milResult);

	double dMinArea, dMaxArea, dMinLenght, dMaxLenght;
	dMinArea = (m_stBlobsInspPara.dBlobMinLenght * m_stBlobsInspPara.dBlobMinLenght) / m_dPixelSizeX;
	dMaxArea = (m_stBlobsInspPara.dBlobMaxArea * m_stBlobsInspPara.dBlobMaxArea) / m_dPixelSizeX;
	dMinLenght = m_stBlobsInspPara.dBlobMinLenght / m_dPixelSizeX;
	dMaxLenght = m_stBlobsInspPara.dBlobMaxLenght / m_dPixelSizeX;

	milBlobs->Select(M_EXCLUDE, M_AREA, M_OUT_RANGE, dMinArea, dMaxArea);
	milBlobs->Select(M_EXCLUDE, M_FERET_MIN_DIAMETER, M_OUT_RANGE, 1, dMaxLenght);
	//milBlobs->Select(M_EXCLUDE, M_FERET_MAX_DIAMETER, M_OUT_RANGE, dMinLenght, dMaxLenght);

	milBlobs->GetResult();

	int nBlobsNum = milBlobs->GetNumber();

	if (nBlobsNum > 0)
	{
#ifdef SAVE_INSP_IMAGE
		CString strText;
		CMilBuffer* milBlobDraw;
		milBlobDraw = CRepository::Instance()->m_MilLib.NewImage();
		milBlobDraw->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
		milBlobDraw->EnableGraph();
		MblobDraw(milBlobDraw->GetIDGraph(), milBlobs->m_milResult, milBlobDraw->GetIDBuffer(), M_DRAW_BLOBS_CONTOUR + M_DRAW_BLOBS, M_INCLUDED_BLOBS, M_DEFAULT);
		strText.Format(_T("D:\\BlobsSearch_All_Blobs_%d.bmp"), m_nInspIndex);
		milBlobDraw->Export(strText, M_BMP);

		milBlobDraw->EnableGraph(FALSE);
		milBlobDraw->Close();

#endif
		double* dMeanPixel, * dMaxDiameter, *dMinDiameter, * dMinAngle, *dLength, *dSumPixel, *dArea, *dRect, *dMaxPixel;
		dMeanPixel = new double[nBlobsNum];
		dSumPixel = new double[nBlobsNum];
		dArea = new double[nBlobsNum];
		dMaxDiameter = new double[nBlobsNum];
		dMinDiameter = new double[nBlobsNum];
		dMinAngle = new double[nBlobsNum];
		dLength = new double[nBlobsNum];
		dRect = new double[nBlobsNum];
		dMaxPixel = new double[nBlobsNum];

		MblobGetResult(milBlobs->m_milResult, M_MEAN_PIXEL, dMeanPixel);
		MblobGetResult(milBlobs->m_milResult, M_SUM_PIXEL, dSumPixel);
		MblobGetResult(milBlobs->m_milResult, M_AREA, dArea);
		MblobGetResult(milBlobs->m_milResult, M_FERET_MAX_DIAMETER, dMaxDiameter);
		MblobGetResult(milBlobs->m_milResult, M_FERET_MIN_DIAMETER, dMinDiameter);
		MblobGetResult(milBlobs->m_milResult, M_FERET_MAX_ANGLE, dMinAngle);
		MblobGetResult(milBlobs->m_milResult, M_LENGTH, dLength);
		MblobGetResult(milBlobs->m_milResult, M_RECTANGULARITY, dRect);
		MblobGetResult(milBlobs->m_milResult, M_MAX_PIXEL, dMaxPixel);
		
		for (int i = 0; i < nBlobsNum; i++)
		{
			//20221206 ngh
			///////////////////////////////////////
			//검출된 블럽 센터 기준으로 이미지 사이즈 업하여 재검출
			////////////////////////////////////////
			CRect cClipRect;
			int nCenX, nCenY;
			nCenX = (int)(milBlobs->m_ResultList.pCXs[i]);
			nCenY = (int)(milBlobs->m_ResultList.pCYs[i]);
			cClipRect.SetRect((int)(milBlobs->m_ResultList.pBoxLeft[i] - (dMaxLenght / 2)),
				(int)(milBlobs->m_ResultList.pBoxTop[i] - (dMaxLenght / 2)),
				(int)(milBlobs->m_ResultList.pBoxRight[i] + (dMaxLenght / 2 + 2)),
				(int)(milBlobs->m_ResultList.pBoxBottom[i] + (dMaxLenght / 2 + 2)));

			if (cClipRect.Width() < 2 || cClipRect.Height() < 2)
				continue;

			if (cClipRect.Width() < 15 || cClipRect.Height() < 15)
			{
				cClipRect.SetRect((int)(milBlobs->m_ResultList.pBoxLeft[i] - (16 / 2)),
					(int)(milBlobs->m_ResultList.pBoxTop[i] - (16 / 2)),
					(int)(milBlobs->m_ResultList.pBoxRight[i] + (16 / 2 + 2)),
					(int)(milBlobs->m_ResultList.pBoxBottom[i] + (16 / 2 + 2)));
			}

			if (cClipRect.right >= m_BlobsImageSize.cx)
			{
				int nWidth_offset = cClipRect.right - m_BlobsImageSize.cx;
				cClipRect.right = m_BlobsImageSize.cx - 1;
				cClipRect.left = cClipRect.left - nWidth_offset;
			}

			if (cClipRect.bottom >= m_BlobsImageSize.cy)
			{
				int nHeight_offset = cClipRect.bottom - m_BlobsImageSize.cy;
				cClipRect.bottom = m_BlobsImageSize.cy - 1;
				cClipRect.top = cClipRect.top - nHeight_offset;
			}

			if (cClipRect.top < 0)
			{
				int nHeight_offset = cClipRect.top;
				cClipRect.top = 0;
				cClipRect.bottom = cClipRect.bottom - nHeight_offset;
			}

			if (cClipRect.left < 0)
			{
				int nWidth_offset = cClipRect.left;
				cClipRect.left = 0;
				cClipRect.right = cClipRect.right - nWidth_offset;
			}

			if (cClipRect.Width() < 2 || cClipRect.Height() < 2)
				continue;


			BYTE* byClipData;
			byClipData = new BYTE[cClipRect.Width() * cClipRect.Height()];

			CMilBuffer* milClip_image;// , * milClip_image2;
			milClip_image = CRepository::Instance()->m_MilLib.NewImage();
			milClip_image->Alloc2D(CSize(cClipRect.Width(), cClipRect.Height()));
			gray_Image->Get2D(byClipData, cClipRect);
			milClip_image->Put2D(CSize(0, 0), CSize(cClipRect.Width(), cClipRect.Height()), byClipData);
#ifdef SAVE_INSP_IMAGE
			strText.Format(_T("D:\\BlobsSearch_All_Blobs5_%d_%d.bmp"), m_nInspIndex, i);
			milClip_image->Export(strText, M_BMP);
#endif		

			long lSum[5] = { 0, }, lIdx = 0;
			
			for (int h = 0; h < cClipRect.Height(); h++)
			{
				for (int w = 0; w < cClipRect.Width(); w++)
				{
					if(h < 2 && w < 2)
					{
						lSum[0] += byClipData[(h * cClipRect.Width()) + w];
						lIdx++;
					}
				}
			}
			for (int h = 0; h < cClipRect.Height(); h++)
			{
				for (int w = 0; w < cClipRect.Width(); w++)
				{
					if (h < 2 && w > cClipRect.Width() - 3)
					{
						lSum[1] += byClipData[(h * cClipRect.Width()) + w];
						lIdx++;
					}
				}
			}
			for (int h = 0; h < cClipRect.Height(); h++)
			{
				for (int w = 0; w < cClipRect.Width(); w++)
				{
					if (h > cClipRect.Height() - 2 && w < 3)
					{
						lSum[2] += byClipData[(h * cClipRect.Width()) + w];
						lIdx++;
					}
				}
			}
			for (int h = 0; h < cClipRect.Height(); h++)
			{
				for (int w = 0; w < cClipRect.Width(); w++)
				{
					if (h > cClipRect.Height() - 2 && w > cClipRect.Width() - 2)
					{
						lSum[3] += byClipData[(h * cClipRect.Width()) + w];
						lIdx++;
					}
				}
			}

			double dAvgSum = (double)(lSum[0] + lSum[1] + lSum[2] + lSum[3]) / lIdx;

			int nSumMax = 0, nSumMin = 255;
			for (int z = 0; z < 4; z++)
			{
				nSumMax = max(lSum[z], nSumMax);
				nSumMin = min(lSum[z], nSumMin);
			}

			long lMaxDiff = (nSumMax / 4) - (nSumMin / 4);
			double dSumAvg = dAvgSum;
			lIdx = 0;
			if (abs((lSum[0] / 4) - dAvgSum) > lMaxDiff / 2)
			{
				for (int z = 0; z < 4; z++)
				{
					if (z != 0)
					{
						lSum[4] += lSum[z];
						lIdx++;
					}
				}
			}
			else if (abs((lSum[1] / 4) - dAvgSum) > lMaxDiff / 2)
			{
				for (int z = 0; z < 4; z++)
				{
					if (z != 1)
					{
						lSum[4] += lSum[z];
						lIdx++;
					}
				}
			}
			else if (abs((lSum[2] / 4) - dAvgSum) > lMaxDiff / 2)
			{
				for (int z = 0; z < 4; z++)
				{
					if (z != 2)
					{
						lSum[4] += lSum[z];
						lIdx++;
					}
				}
			}
			else if (abs((lSum[3] / 4) - dAvgSum) > lMaxDiff / 2)
			{
				for (int z = 0; z < 4; z++)
				{
					if (z != 3)
					{
						lSum[4] += lSum[z];
						lIdx++;
					}
				}
			}


			m_nClipBlobsThreshHold = lSum[4] / (lIdx * 4);

			delete[] byClipData;

			CMilBuffer* milClipSearch, * milClipSearch2, * milClipSearch3;
			milClipSearch = CRepository::Instance()->m_MilLib.NewImage();
			milClipSearch->Alloc2D(CSize(cClipRect.Width() * CLIP_INSP_RET, cClipRect.Height() * CLIP_INSP_RET));
			milClipSearch2 = CRepository::Instance()->m_MilLib.NewImage();
			milClipSearch2->Alloc2D(CSize(cClipRect.Width() * CLIP_INSP_RET, cClipRect.Height() * CLIP_INSP_RET));
			milClipSearch3 = CRepository::Instance()->m_MilLib.NewImage();
			milClipSearch3->Alloc2D(CSize(cClipRect.Width()* CLIP_INSP_RET, cClipRect.Height()* CLIP_INSP_RET));

			if (CLIP_INSP_RET > 1)
				MimResize(milClip_image->GetIDBuffer(), milClipSearch->GetIDBuffer(), CLIP_INSP_RET, CLIP_INSP_RET, /*M_BILINEAR*/M_BICUBIC);
			else
				milClipSearch->Copy(milClip_image);

			BYTE* pSrcData, * pDstData1;
			pSrcData = new BYTE[cClipRect.Width() * CLIP_INSP_RET * cClipRect.Height() * CLIP_INSP_RET];
			pDstData1 = new BYTE[cClipRect.Width() * CLIP_INSP_RET * cClipRect.Height() * CLIP_INSP_RET];

			milClipSearch->Get2D(pSrcData, CRect(0, 0, cClipRect.Width() * CLIP_INSP_RET, cClipRect.Height() * CLIP_INSP_RET));
			milClipSearch2->Get2D(pDstData1, CRect(0, 0, cClipRect.Width() * CLIP_INSP_RET, cClipRect.Height() * CLIP_INSP_RET));
			memset(pDstData1, NULL, sizeof(BYTE) * (cClipRect.Width() * CLIP_INSP_RET * cClipRect.Height() * CLIP_INSP_RET));
#ifdef SAVE_INSP_IMAGE
			strText.Format(_T("D:\\BlobsSearch_All_Blobs3_%d_%d.bmp"), m_nInspIndex, i);
			milClipSearch->Export(strText, M_BMP);

#endif		
			if(dMeanPixel[i] > 200)
				MBinarizeAdaptive(milClipSearch, milClipSearch2, 1.5, dMeanPixel[i]/2);// 50);
			else
				MBinarizeAdaptive(milClipSearch, milClipSearch2, 1, dMeanPixel[i] / 2);// 50);
			MimArith(milClipSearch2->GetIDBuffer(), M_NULL, milClipSearch3->GetIDBuffer(), M_NOT);
			//milClipSearch2->Copy(milClipSearch3);
			MbufCopy(milClipSearch3->GetIDBuffer(), milClipSearch2->GetIDBuffer());
#ifdef SAVE_INSP_IMAGE
			strText.Format(_T("D:\\BlobsSearch_All_Blobs4_%d_%d.bmp"), m_nInspIndex, i);
			milClipSearch2->Export(strText, M_BMP);
#endif		

			delete[] pSrcData;
			delete[] pDstData1;

			CMilBlob* milBlobs2;
			milBlobs2 = CRepository::Instance()->m_MilLib.NewBlobTool(milClipSearch2);
			milBlobs2->m_ResultList.pCXs = NULL;
			milBlobs2->m_ResultList.pCYs = NULL;
			milBlobs2->m_ResultList.pBoxLeft = NULL;
			milBlobs2->m_ResultList.pBoxBottom = NULL;
			milBlobs2->m_ResultList.pBoxRight = NULL;
			milBlobs2->m_ResultList.pBoxTop = NULL;
			milBlobs2->m_ResultList.pArea = NULL;
			milBlobs2->AllocFeatureList();
			milBlobs2->AllocResult();

			milBlobs2->SelectFeature(M_AREA);
			milBlobs2->SelectFeature(M_MEAN_PIXEL);
			milBlobs2->SelectFeature(M_MAX_PIXEL);
			milBlobs2->SelectFeature(M_MIN_PIXEL);
			milBlobs2->SelectFeature(M_SUM_PIXEL);
			milBlobs2->SelectFeature(M_FERET_MAX_DIAMETER);
			milBlobs2->SelectFeature(M_FERET_MIN_DIAMETER);
			milBlobs2->SelectFeature(M_FERET_MIN_ANGLE);

			milBlobs2->SelectFeature(M_CENTER_OF_GRAVITY_X);
			milBlobs2->SelectFeature(M_CENTER_OF_GRAVITY_Y);
			milBlobs2->SelectFeature(M_BOX_X_MIN);
			milBlobs2->SelectFeature(M_BOX_Y_MIN);
			milBlobs2->SelectFeature(M_BOX_X_MAX);
			milBlobs2->SelectFeature(M_BOX_Y_MAX);
			milBlobs2->SelectFeature(M_FERET_X);
			milBlobs2->SelectFeature(M_FERET_Y);
			milBlobs2->SelectFeature(M_LENGTH);
			milBlobs2->SelectFeature(M_RECTANGULARITY);
			milBlobs2->SelectFeature(M_CONVEX_HULL);

			MblobControl(milBlobs2->m_milResult, M_NUMBER_OF_FERETS, 180);
			MblobControl(milBlobs2->m_milResult, M_RESULT_OUTPUT_UNITS, M_PIXEL);

			MblobCalculate(milBlobs2->m_pMilBuffer->GetIDBuffer(), milClipSearch->GetIDBuffer(), milBlobs2->m_featureList, milBlobs2->m_milResult);

			//milBlobs2->Select(M_EXCLUDE, M_AREA, M_OUT_RANGE, dMinArea * CLIP_INSP_RET, dMaxArea * CLIP_INSP_RET);
			milBlobs2->Select(M_EXCLUDE, M_FERET_MIN_DIAMETER, M_OUT_RANGE, dMinLenght * CLIP_INSP_RET, dMaxLenght * CLIP_INSP_RET);
			milBlobs2->Select(M_EXCLUDE, M_FERET_MAX_DIAMETER, M_OUT_RANGE, dMinLenght * CLIP_INSP_RET, dMaxLenght * CLIP_INSP_RET);

			milBlobs2->GetResult();

			int nBlobsNum2 = milBlobs2->GetNumber();

			if (nBlobsNum2 > 0)
			{
#ifdef SAVE_INSP_IMAGE
				CMilBuffer* milBlobDraw2;
				milBlobDraw2 = CRepository::Instance()->m_MilLib.NewImage();
				milBlobDraw2->Alloc2D(CSize(cClipRect.Width() * CLIP_INSP_RET, cClipRect.Height() * CLIP_INSP_RET));
				MbufClear(milBlobDraw2->GetIDBuffer(), M_COLOR_BLACK);
				milBlobDraw2->EnableGraph();
				MblobDraw(milBlobDraw2->GetIDGraph(), milBlobs2->m_milResult, milBlobDraw2->GetIDBuffer(), /*M_DRAW_BLOBS_CONTOUR +*/ M_DRAW_BLOBS + M_DRAW_CONVEX_HULL, M_INCLUDED_BLOBS, M_DEFAULT);

				strText.Format(_T("D:\\BlobsSearch_All_Blobs2_%d_%d.bmp"), m_nInspIndex, i);
				milBlobDraw2->Export(strText, M_BMP);

				milBlobDraw2->EnableGraph(FALSE);

				milBlobDraw2->Close();
				milBlobDraw2 = NULL;
#endif
				double* dMinPixel2, *dMeanPixel2, * dMaxDiameter2, * dMinDiameter2, * dMinAngle2, * dLength2, * dSumPixel2, * dArea2, * dRect2, * dMaxPixel2;
				dMinPixel2 = new double[nBlobsNum2];
				dMeanPixel2 = new double[nBlobsNum2];
				dSumPixel2 = new double[nBlobsNum2];
				dArea2 = new double[nBlobsNum2];
				dMaxDiameter2 = new double[nBlobsNum2];
				dMinDiameter2 = new double[nBlobsNum2];
				dMinAngle2 = new double[nBlobsNum2];
				dLength2 = new double[nBlobsNum2];
				dRect2 = new double[nBlobsNum2];
				dMaxPixel2 = new double[nBlobsNum2];

				MblobGetResult(milBlobs2->m_milResult, M_MIN_PIXEL, dMinPixel2);
				MblobGetResult(milBlobs2->m_milResult, M_MEAN_PIXEL, dMeanPixel2);
				MblobGetResult(milBlobs2->m_milResult, M_MAX_PIXEL, dMaxPixel2);
				MblobGetResult(milBlobs2->m_milResult, M_SUM_PIXEL, dSumPixel2);
				MblobGetResult(milBlobs2->m_milResult, M_AREA, dArea2);
				MblobGetResult(milBlobs2->m_milResult, M_FERET_MAX_DIAMETER, dMaxDiameter2);
				MblobGetResult(milBlobs2->m_milResult, M_FERET_MIN_DIAMETER, dMinDiameter2);
				MblobGetResult(milBlobs2->m_milResult, M_FERET_MAX_ANGLE, dMinAngle2);
				MblobGetResult(milBlobs2->m_milResult, M_LENGTH, dLength2);
				MblobGetResult(milBlobs2->m_milResult, M_RECTANGULARITY, dRect2);

				BYTE* byData;
				byData = new BYTE[cClipRect.Width() * CLIP_INSP_RET * cClipRect.Height() * CLIP_INSP_RET];
				MIL_INT lPitch = cClipRect.Width() * CLIP_INSP_RET;

				milClipSearch->Get2D(byData, CRect(0, 0, cClipRect.Width() * CLIP_INSP_RET, cClipRect.Height() * CLIP_INSP_RET));
				int nMaxIndex = 0;
				if (nBlobsNum2 > 1)
				{
					double dMaxArea = 255;
					for (int j = 0; j < nBlobsNum2; j++)
					{
						dMaxArea = min(dMinPixel2[j], dMaxArea);
					}

					
					for (int j = 0; j < nBlobsNum2; j++)
					{
						if (dMaxArea == dMinPixel2[j])
						{
							nMaxIndex = j;
							break;
						}
					}

				}


				int j = nMaxIndex;
				int nIdx = 0;
				double dAvg[8] = { 0., };

				int dLeft = (int)milBlobs2->m_ResultList.pBoxLeft[j] - 1;
				int dTop = (int)milBlobs2->m_ResultList.pBoxTop[j] - 1;
				int dRight = (int)milBlobs2->m_ResultList.pBoxRight[j] + 1;
				int dBottom = (int)milBlobs2->m_ResultList.pBoxBottom[j] + 1;


				//if (dLeft == 0 || dTop == 0 || dRight == cClipRect.Width() * CLIP_INSP_RET - 1 || dBottom == cClipRect.Height() * CLIP_INSP_RET - 1)
				//	continue;


				double dCenX, dCenY;
				dCenX = cClipRect.Width() * CLIP_INSP_RET / 2;
				dCenY = cClipRect.Height() * CLIP_INSP_RET / 2;

				//if (dCenX - 4 > milBlobs2->m_ResultList.pCXs[j] || milBlobs2->m_ResultList.pCXs[j] > dCenX + 4 ||
					//dCenY - 4 > milBlobs2->m_ResultList.pCYs[j] || milBlobs2->m_ResultList.pCYs[j] > dCenY + 4)
					//continue;

				CRect cRect;
				cRect.SetRect(dLeft, dTop, dRight, dBottom);
				long lNearbySum[5] = { 0, };

				for (int h = 0; h < cClipRect.Height() * CLIP_INSP_RET; h++)
				{
					for (int w = 0; w < cClipRect.Width() * CLIP_INSP_RET; w++)
					{
						if (h < 2 && w < 2)
						{
							lNearbySum[0] += byData[(h * cClipRect.Width() * CLIP_INSP_RET) + w];
							nIdx++;
						}
					}
				}
				for (int h = 0; h < cClipRect.Height() * CLIP_INSP_RET; h++)
				{
					for (int w = 0; w < cClipRect.Width() * CLIP_INSP_RET; w++)
					{
						if (h < 2 && w > (cClipRect.Width() * CLIP_INSP_RET) - 3)
						{
							lNearbySum[1] += byData[(h * cClipRect.Width() * CLIP_INSP_RET) + w];
							nIdx++;
						}
					}
				}
				for (int h = 0; h < cClipRect.Height() * CLIP_INSP_RET; h++)
				{
					for (int w = 0; w < cClipRect.Width() * CLIP_INSP_RET; w++)
					{
						if (h > (cClipRect.Height() * CLIP_INSP_RET) - 3 && w < 2)
						{
							lNearbySum[2] += byData[(h * cClipRect.Width() * CLIP_INSP_RET) + w];
							nIdx++;
						}
					}
				}
				for (int h = 0; h < cClipRect.Height() * CLIP_INSP_RET; h++)
				{
					for (int w = 0; w < cClipRect.Width() * CLIP_INSP_RET; w++)
					{
						if (h > (cClipRect.Height() * CLIP_INSP_RET) - 3 && w > (cClipRect.Width() * CLIP_INSP_RET) - 3)
						{
							lNearbySum[3] += byData[(h * cClipRect.Width() * CLIP_INSP_RET) + w];
							nIdx++;
						}
					}
				}
				int nAvgRslt = 0;
				long lMinVal = 256;
				for (int z = 0; z < 4; z++)
				{
					if (abs((lNearbySum[z] / 4) - dMeanPixel2[j]) >= m_stBlobsInspPara.nBlobNearbyVal)
					{
						nAvgRslt++;
					}

					lMinVal = min((lNearbySum[z] / 4), lMinVal);
				}

				long long llSum = 0;
				nIdx = 0;
				for (int h = 0; h < cClipRect.Height() * CLIP_INSP_RET; h++)
				{
					for (int w = 0; w < cClipRect.Width() * CLIP_INSP_RET; w++)
					{
						llSum += byData[(h * cClipRect.Width() * CLIP_INSP_RET) + w];
						nIdx++;
					}
				}
				double dSumAvg = (double)(llSum / nIdx);

				BOOL bPixelVal = FALSE;

				//if (lMinVal < dMeanPixel2[j])
				//	bPixelVal = FALSE;
				double dMaxPixelVal = dMaxPixel[j];
				double dMaxPixelRet = 1;// dMaxPixel2[j] / dMeanPixel2[j];
				if (m_stBlobsInspPara.nBlobMinThreshHold <= dMeanPixel2[j] &&
					dMeanPixel2[j] <= (m_stBlobsInspPara.nBlobMaxThreshHold * dMaxPixelRet))//m_nClipBlobsThreshHold - m_stBlobsInspPara.nBlobNearbyVal)//m_stBlobsInspPara.nBlobMaxThreshHold)
					bPixelVal = TRUE;


				if (!bPixelVal && m_BlobsImageSize.cy - (m_BlobsImageSize.cy / 10) <= (milBlobs2->m_ResultList.pCYs[j] / CLIP_INSP_RET + cClipRect.top))
				{
					if (m_stBlobsInspPara.nBlobMinThreshHold - 5 <= dMeanPixel2[j] &&
						dMeanPixel2[j] <= ((m_stBlobsInspPara.nBlobMaxThreshHold - 5) * dMaxPixelRet))//m_nClipBlobsThreshHold - m_stBlobsInspPara.nBlobNearbyVal)//m_stBlobsInspPara.nBlobMaxThreshHold)
						bPixelVal = TRUE;
				}



				BOOL bRet = TRUE;

				//if (dMaxDiameter2[j] / dMinDiameter2[j] > 4)
					//bRet = FALSE;

				if (nAvgRslt > 2 && bPixelVal && bRet)// && (dRect2[j] > 0.50))
				{
					BLOB_RESULT_PARA result_data;
					result_data.dAngle = dMinAngle2[j];
					result_data.dCenX = milBlobs2->m_ResultList.pCXs[j] / CLIP_INSP_RET + cClipRect.left;
					result_data.dCenY = milBlobs2->m_ResultList.pCYs[j] / CLIP_INSP_RET + cClipRect.top;
					result_data.dShortSize = dMinDiameter2[j] / CLIP_INSP_RET;
					result_data.dLongSize = dMaxDiameter2[j] / CLIP_INSP_RET;
					result_data.dLength = dLength2[j] / CLIP_INSP_RET;

					if (10 <= result_data.dCenX && result_data.dCenX <= (m_BlobsImageSize.cx - 10) &&
						10 <= result_data.dCenY && result_data.dCenY <= (m_BlobsImageSize.cy - 10))
					{

						CRect result_rect, result_rect2;
						result_rect.SetRect((int)(result_data.dCenX - (result_data.dLongSize / 2.) - 5),
							(int)(result_data.dCenY - (result_data.dLongSize / 2.) - 5),
							(int)(result_data.dCenX + (result_data.dLongSize / 2.) + 5),
							(int)(result_data.dCenY + (result_data.dLongSize / 2.) + 5));

						BOOL bInter = FALSE;
						for (int k = 0; k < m_stBlobsResult.size(); k++)
						{
							result_rect2.SetRect((int)(m_stBlobsResult[k].dCenX - (m_stBlobsResult[k].dLongSize / 2.) - 5),
								(int)(m_stBlobsResult[k].dCenY - (m_stBlobsResult[k].dLongSize / 2.) - 5),
								(int)(m_stBlobsResult[k].dCenX + (m_stBlobsResult[k].dLongSize / 2.) + 5),
								(int)(m_stBlobsResult[k].dCenY + (m_stBlobsResult[k].dLongSize / 2.) + 5));

							CRect result_rect3;
							result_rect3.IntersectRect(result_rect, result_rect2);

							if (result_rect3.Width() > 0 && result_rect3.Height() > 0)
							{
								bInter = TRUE;

								if (result_data.dShortSize > m_stBlobsResult[k].dShortSize)
								{
									m_stBlobsResult[k] = result_data;
								}

							}
							else
							{

							}

						}

						if (!bInter)
							m_stBlobsResult.push_back(result_data);

					}
				}

				delete[] byData;
				delete[] dMinPixel2;
				delete[] dMaxPixel2;
				delete[] dMeanPixel2;
				delete[] dMaxDiameter2;
				delete[] dMinDiameter2;
				delete[] dMinAngle2;
				delete[] dLength2;
				delete[] dSumPixel2;
				delete[] dArea2;
				delete[] dRect2;
			}

			milBlobs2->Free();

			milClip_image->Close();
			milClipSearch->Close();
			milClipSearch2->Close();
			milClipSearch3->Close();

			milClip_image = NULL;
			milClipSearch = NULL;
			milClipSearch2 = NULL;
			milClipSearch3 = NULL;
			milBlobs2 = NULL;
			////////////////////////////////////////////
		}


#ifndef NGH_NEW_TEST
		milBlobDraw->Close();
#endif
		delete[] dMinAngle;
		delete[] dMaxDiameter;
		delete[] dMinDiameter;
		delete[] dMeanPixel;
		delete[] dLength;
		delete[] dSumPixel;
		delete[] dArea;
		delete[] dRect;
		delete[] dMaxPixel;
	}

	milBlobs->Free();
	srcImage->Close();

	milBlobs = NULL;
	srcImage = NULL;
}

/**
  @brief GetSearchAreaCenterXY
  검사영역의 센터x,y 값 추출
*/
void CInspectionBlobs::GetSearchAreaCenterXY(CMilBuffer* srcImage, double* dCenX, double* dCenY)
{
	CMilBlob* milBlobs;
	milBlobs = CRepository::Instance()->m_MilLib.NewBlobTool(srcImage);
	milBlobs->m_ResultList.pCXs = NULL;
	milBlobs->m_ResultList.pCYs = NULL;
	milBlobs->m_ResultList.pBoxLeft = NULL;
	milBlobs->m_ResultList.pBoxBottom = NULL;
	milBlobs->m_ResultList.pBoxRight = NULL;
	milBlobs->m_ResultList.pBoxTop = NULL;
	milBlobs->m_ResultList.pArea = NULL;
	milBlobs->AllocFeatureList();
	milBlobs->AllocResult();
	MblobControl(milBlobs->m_milResult, M_NUMBER_OF_FERETS, 50);
	milBlobs->SelectFeature(M_FERET_MAX_DIAMETER);
	milBlobs->SelectFeature(M_FERET_MIN_DIAMETER);
	milBlobs->SelectFeature(M_FERET_MIN_ANGLE);
	milBlobs->SelectFeature(M_AREA);
	milBlobs->SelectFeature(M_BOX_X_MIN);
	milBlobs->SelectFeature(M_BOX_Y_MIN);
	milBlobs->SelectFeature(M_BOX_X_MAX);
	milBlobs->SelectFeature(M_BOX_Y_MAX);
	milBlobs->SelectFeature(M_CENTER_OF_GRAVITY_X);
	milBlobs->SelectFeature(M_CENTER_OF_GRAVITY_Y);
	milBlobs->Caculate();

	int nBlobCnt = milBlobs->GetNumber();

	double dAreaAngle = 0.;
	double dAreaCenX = 0., dAreaCenY = 0.;
	if (nBlobCnt > 0)
	{
		double* dAngle, * dAreaCenX, * dAreaCenY, *dArea;
		dAngle = new double[nBlobCnt];
		dAreaCenX = new double[nBlobCnt];
		dAreaCenY = new double[nBlobCnt];
		dArea = new double[nBlobCnt];

		MblobGetResult(milBlobs->m_milResult, M_FERET_MIN_ANGLE, dAngle);
		MblobGetResult(milBlobs->m_milResult, M_CENTER_OF_GRAVITY_X, dAreaCenX);
		MblobGetResult(milBlobs->m_milResult, M_CENTER_OF_GRAVITY_Y, dAreaCenY);
		MblobGetResult(milBlobs->m_milResult, M_AREA, dArea);

		dAreaAngle = dAngle[0] + 90.;
		//if (dAreaAngle < 0.)
		//	dAreaAngle += 360.;

		double dMax = 0;
		for (int i = 0; i < nBlobCnt; i++)
		{
			dMax = max(dArea[i], dMax);
		}

		int nIdx = 0;
		for (int i = 0; i < nBlobCnt; i++)
		{
			if (dArea[i] == dMax)
			{
				nIdx = i;
				break;
			}
		}

		*dCenX = dAreaCenX[nIdx];
		*dCenY = dAreaCenY[nIdx];

		delete[] dAngle;
		delete[] dAreaCenX;
		delete[] dAreaCenY;
		delete[] dArea;
	};

	milBlobs->Free();
}

//dst = area image + src
//src = mask image
/**
  @brief MaskingImage
  Mask Image를 이용하여 Src Image에 검사영역을 추출
*/
void CInspectionBlobs::MaskingImage(CMilBuffer* src, CMilBuffer* dst)
{
	CMilBlob* milBlobs;
	milBlobs = CRepository::Instance()->m_MilLib.NewBlobTool(dst);
	milBlobs->m_ResultList.pCXs = NULL;
	milBlobs->m_ResultList.pCYs = NULL;
	milBlobs->m_ResultList.pBoxLeft = NULL;
	milBlobs->m_ResultList.pBoxBottom = NULL;
	milBlobs->m_ResultList.pBoxRight = NULL;
	milBlobs->m_ResultList.pBoxTop = NULL;
	milBlobs->m_ResultList.pArea = NULL;
	milBlobs->AllocFeatureList();
	milBlobs->AllocResult();
	MblobControl(milBlobs->m_milResult, M_NUMBER_OF_FERETS, 180);
	milBlobs->SelectFeature(M_FERET_MAX_DIAMETER);
	milBlobs->SelectFeature(M_FERET_MIN_DIAMETER);
	milBlobs->SelectFeature(M_FERET_MIN_ANGLE);
	milBlobs->SelectFeature(M_AREA);
	milBlobs->SelectFeature(M_BOX_X_MIN);
	milBlobs->SelectFeature(M_BOX_Y_MIN);
	milBlobs->SelectFeature(M_BOX_X_MAX);
	milBlobs->SelectFeature(M_BOX_Y_MAX);
	milBlobs->SelectFeature(M_CENTER_OF_GRAVITY_X);
	milBlobs->SelectFeature(M_CENTER_OF_GRAVITY_Y);
	milBlobs->Caculate();

	int nBlobCnt = milBlobs->GetNumber();

	double dAreaAngle = 0.;
	double dAreaCenX = 0., dAreaCenY = 0.;
	if (nBlobCnt > 0)
	{
		double* dAngle, * dCenX, * dCenY, *dArea;
		dAngle = new double[nBlobCnt];
		dCenX = new double[nBlobCnt];
		dCenY = new double[nBlobCnt];
		dArea = new double[nBlobCnt];


		MblobGetResult(milBlobs->m_milResult, M_FERET_MIN_ANGLE, dAngle);
		MblobGetResult(milBlobs->m_milResult, M_CENTER_OF_GRAVITY_X, dCenX);
		MblobGetResult(milBlobs->m_milResult, M_CENTER_OF_GRAVITY_Y, dCenY);
		MblobGetResult(milBlobs->m_milResult, M_AREA, dArea);

		dAreaAngle = dAngle[0];// +90.;

		double dMax = 0;
		for (int i = 0; i < nBlobCnt; i++)
		{
			dMax = max(dArea[i], dMax);
		}

		int nIdx = 0;
		for (int i = 0; i < nBlobCnt; i++)
		{
			if (dArea[i] == dMax)
			{
				nIdx = i;
				break;
			}
		}

		dAreaCenX = dCenX[0];
		dAreaCenY = dCenY[0];

		delete[] dArea;
		delete[] dAngle;
		delete[] dCenX;
		delete[] dCenY;
	};

	CMilBuffer* milProcessRslt1, * milProcessRslt2, * milProcessRslt3, * milProcessRslt4;
	milProcessRslt1 = CRepository::Instance()->m_MilLib.NewImage();
	milProcessRslt1->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
	milProcessRslt2 = CRepository::Instance()->m_MilLib.NewImage();
	milProcessRslt2->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
	milProcessRslt3 = CRepository::Instance()->m_MilLib.NewImage();
	milProcessRslt3->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
	milProcessRslt4 = CRepository::Instance()->m_MilLib.NewImage();
	milProcessRslt4->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
	//MimRotate(src->GetIDBuffer(), milProcessRslt1->GetIDBuffer(), dAreaAngle, m_stBlobsInspPara.stMaskingPara.dCenX, m_stBlobsInspPara.stMaskingPara.dCenY, M_DEFAULT, M_DEFAULT, M_DEFAULT);
#ifdef SAVE_INSP_IMAGE
	milProcessRslt1->Export(_T("d:\\MaskingImage1_Rotate.bmp"), M_BMP);
#endif

	double dMaskCenX = (dAreaCenX - m_stBlobsInspPara.stMaskingPara.dCenX);
	double dMaskCenY = (dAreaCenY - m_stBlobsInspPara.stMaskingPara.dCenY);
	MimTranslate(src->GetIDBuffer(), milProcessRslt2->GetIDBuffer(), dMaskCenX, dMaskCenY, M_DEFAULT);
#ifdef SAVE_INSP_IMAGE
	milProcessRslt2->Export(_T("d:\\MaskingImage2_Trans.bmp"), M_BMP);
#endif

	//MimArith(milProcessRslt2->GetIDBuffer(), M_NULL, milProcessRslt3->GetIDBuffer(), M_NOT);
	MimArith(milProcessRslt2->GetIDBuffer(), M_NULL, milProcessRslt3->GetIDBuffer(), M_NOT);
	MimArith(dst->GetIDBuffer(), milProcessRslt3->GetIDBuffer(), milProcessRslt4->GetIDBuffer(), M_AND);
	dst->Copy(milProcessRslt4);

#ifdef SAVE_INSP_IMAGE
	milProcessRslt4->Export(_T("d:\\MaskingImage3_Arith.bmp"), M_BMP);
#endif


	milBlobs->Free();
	milProcessRslt1->Close();
	milProcessRslt2->Close();
	milProcessRslt3->Close();
	milProcessRslt4->Close();
	milBlobs = NULL;
	milProcessRslt1 = NULL;
	milProcessRslt2 = NULL;
	milProcessRslt3 = NULL;
	milProcessRslt4 = NULL;
}

void CInspectionBlobs::GetBlobsResult(vector<BLOB_RESULT_PARA>* stBlobsResult)
{
	for (int i = 0; i < m_stBlobsResult.size(); i++)
	{
		stBlobsResult->push_back(m_stBlobsResult[i]);
	}
}

/**
  @brief MBinarizeAdaptive
  MimBinarizeAdaptive 함수를 사용 하기 위한 함수
*/
void CInspectionBlobs::MBinarizeAdaptive(CMilBuffer* src, CMilBuffer* dst, double nMinContrast, double nMinGlobal)
{
	MIL_ID MilContext;

	MimAlloc(src->m_milIDSystem, M_BINARIZE_ADAPTIVE_CONTEXT, M_DEFAULT, &MilContext);

	/* Set binarization controls. */
	MimControl(MilContext, M_THRESHOLD_MODE, M_NIBLACK);
	MimControl(MilContext, M_FOREGROUND_VALUE, M_FOREGROUND_BLACK);
	MimControl(MilContext, M_MINIMUM_CONTRAST, nMinContrast);
	MimControl(MilContext, M_GLOBAL_MIN,  nMinGlobal);
	//if (nMinGlobal * 2 < 200)
	int nMax = (int)(nMinGlobal * 2. + 10.);
	if (nMax > 255)
		nMax = 250;
	
	MimControl(MilContext, M_GLOBAL_MAX, nMax);// nMinGlobal * 2);
	//MimControl(MilContext, M_NIBLACK_BIAS, 0.3);
	//MimControl(MilContext, M_AVERAGE_MODE, M_GAUSSIAN);
	//MimControl(MilContext, M_GLOBAL_OFFSET, 2);

	/* Perform binarization. */
	MimBinarizeAdaptive(MilContext, src->GetIDBuffer(), M_NULL, M_NULL, dst->GetIDBuffer(), M_NULL, M_DEFAULT);

	MimFree(MilContext);
}

/**
  @brief EmpahsizeFunc
  Halcon의 Empahsize 함수와 같은 방식의 함수
*/
void CInspectionBlobs::EmpahsizeFunc(CMilBuffer* src, CMilBuffer* dst, int nMaskWidthHeight, double dFactor)
{
	if (nMaskWidthHeight <= 1 || dFactor <= 0.)
	{
		dst->Copy(src);
		return;
	}
	tImageProperties img_pro;
	src->GetImageProperties(&img_pro);

	CMilBuffer* milFilter1, * milFilter2;
	milFilter1 = CRepository::Instance()->m_MilLib.NewImage();
	milFilter1->Alloc2D(CSize(img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	milFilter2 = CRepository::Instance()->m_MilLib.NewImage();
	milFilter2->Alloc2D(CSize(img_pro.ImageSize.cx, img_pro.ImageSize.cy));


	milFilter1->Copy(src);
	/*for (int i = 0; i < nMaskWidthHeight; i++)
	{
		MimConvolve(milFilter1->GetIDBuffer(), milFilter2->GetIDBuffer(), M_SMOOTH);
		milFilter1->Copy(milFilter2);
	}*/

	BYTE* pSrcData, * pFilterData, * pDstData;
	pSrcData = new BYTE[img_pro.ImageSize.cx * img_pro.ImageSize.cy];
	pFilterData = new BYTE[img_pro.ImageSize.cx * img_pro.ImageSize.cy];
	pDstData = new BYTE[img_pro.ImageSize.cx * img_pro.ImageSize.cy];

	src->Get2D(pSrcData, CRect(0, 0, img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	milFilter1->Get2D(pFilterData, CRect(0, 0, img_pro.ImageSize.cx, img_pro.ImageSize.cy));

	::MeanImageFunc(pSrcData, pFilterData, img_pro.ImageSize.cx, img_pro.ImageSize.cy, nMaskWidthHeight);

	//dst->Put2D(CSize(0, 0), img_pro.ImageSize, pFilterData);
	//milFilter1->ExportBMP(_T("d:\\TestMeanImage.bmp"));

	::EmpahsizeFunc(pSrcData, pFilterData, pDstData, img_pro.ImageSize.cx, img_pro.ImageSize.cy, dFactor);

	

	if (img_pro.ImageSize.cy == m_BlobsImageSize.cx && img_pro.ImageSize.cx == m_BlobsImageSize.cx)
	{
		milFilter2->Put2D(CSize(0, 0), img_pro.ImageSize, pDstData);

		LONG lMax = 0;
		long long* lpVal;
		lpVal = new long long[256];

		MilHistogram(milFilter2, 256, lpVal);

		::MaxVal(lpVal, 256, &lMax);

	
		for (int h = 0; h < img_pro.ImageSize.cy; h++)
		{
			for (int w = 0; w < img_pro.ImageSize.cx; w++)
			{
				if ((h < 5 || h > img_pro.ImageSize.cy - 6))
				{
					pDstData[(h * img_pro.ImageSize.cx) + w] = (BYTE)lMax;
				}
			}
		}

		for (int h = 0; h < img_pro.ImageSize.cy; h++)
		{
			for (int w = 0; w < img_pro.ImageSize.cx; w++)
			{
				if ((w < 10 || w > img_pro.ImageSize.cx - 11))
				{
					pDstData[(h * img_pro.ImageSize.cx) + w] = (BYTE)lMax;
				}
			}
		}

		delete[] lpVal;
	}

	dst->Put2D(CSize(0, 0), img_pro.ImageSize, pDstData);

	delete[] pSrcData;
	delete[] pFilterData;
	delete[] pDstData;
	

	milFilter1->Close();
	milFilter2->Close();

	milFilter1 = NULL;
	milFilter2 = NULL;
}

/**
  @brief IlluminateFunc
  Halcon의 Illuminate 함수와 같은 방식의 함수
*/
void CInspectionBlobs::IlluminateFunc(CMilBuffer* src, CMilBuffer* dst, int nMaskWidthHeight, double dFactor)
{
	if (nMaskWidthHeight <= 1 || dFactor <= 0.)
	{
		dst->Copy(src);
		return;
	}

	tImageProperties img_pro;
	src->GetImageProperties(&img_pro);
	
	CMilBuffer* milFilter1, * milFilter2;
	milFilter1 = CRepository::Instance()->m_MilLib.NewImage();
	milFilter1->Alloc2D(CSize(img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	milFilter2 = CRepository::Instance()->m_MilLib.NewImage();
	milFilter2->Alloc2D(CSize(img_pro.ImageSize.cx, img_pro.ImageSize.cy));


	milFilter1->Copy(src);
	for (int i = 0; i < nMaskWidthHeight / 2; i++)
	{
		MimConvolve(milFilter1->GetIDBuffer(), milFilter2->GetIDBuffer(), M_SMOOTH);
		milFilter1->Copy(milFilter2);
	}


	BYTE* pSrcData, * pFilterData, * pDstData;
	pSrcData = new BYTE[img_pro.ImageSize.cx * img_pro.ImageSize.cy];
	pFilterData = new BYTE[img_pro.ImageSize.cx * img_pro.ImageSize.cy];
	pDstData = new BYTE[img_pro.ImageSize.cx * img_pro.ImageSize.cy];

	src->Get2D(pSrcData, CRect(0, 0, img_pro.ImageSize.cx, img_pro.ImageSize.cy));
	milFilter1->Get2D(pFilterData, CRect(0, 0, img_pro.ImageSize.cx, img_pro.ImageSize.cy));

	::IlluminateFunc(pSrcData, pFilterData, pDstData, img_pro.ImageSize.cx, img_pro.ImageSize.cy, dFactor);

	milFilter2->Put2D(CSize(0, 0), img_pro.ImageSize, pDstData);

	LONG lMax = 0;
	long long* lpVal;
	lpVal = new long long[256];

	MilHistogram(milFilter2, 256, lpVal);
	
	::MaxVal(lpVal, 256, &lMax);
	
	for (int h = 0; h < img_pro.ImageSize.cy; h++)
	{
		for (int w = 0; w < img_pro.ImageSize.cx; w++)
		{
			if ((h < 5 || h > img_pro.ImageSize.cy - 6))
			{
				pDstData[(h * img_pro.ImageSize.cx) + w] = (BYTE)lMax;
			}
		}
	}

	for (int h = 0; h < img_pro.ImageSize.cy; h++)
	{
		for (int w = 0; w < img_pro.ImageSize.cx; w++)
		{
			if ((w < 10 || w > img_pro.ImageSize.cx - 11))
			{
				pDstData[(h * img_pro.ImageSize.cx) + w] = (BYTE)lMax;
			}
		}
	}

	dst->Put2D(CSize(0, 0), img_pro.ImageSize, pDstData);

	delete[] pSrcData;
	delete[] pFilterData;
	delete[] pDstData;
	delete[] lpVal;

	milFilter1->Close();
	milFilter2->Close();

	milFilter1 = NULL;
	milFilter2 = NULL;
}

/**
  @brief GetBlobsThreadEnd
  검사 Thread 종료 확인 함수
*/
BOOL CInspectionBlobs::GetBlobsThreadEnd()
{
	BOOL rslt = TRUE;
	if (m_pThreadBlobsInsp != NULL)
	{
		DWORD dwRet = WaitForSingleObject(m_pThreadBlobsInsp->m_hThread, DEFAULT_TIMEOUT);

		if (dwRet == WAIT_FAILED)
		{
			rslt = FALSE;
		}
		else if (dwRet == WAIT_ABANDONED)
		{
			rslt = FALSE;
		}
		else if (dwRet == WAIT_TIMEOUT)
		{
			rslt = FALSE;
		}
	}

	return rslt;
}

/**
  @brief GetBlobsThreadEnd
  검사 결과 data Display, ㅣist 표시
*/
void CInspectionBlobs::SetBlobsResultDisplay(CMilDisplay* milDisplay, vector<BLOB_RESULT_PARA> stBlobsResult, CListCtrl* list_ctrl)
{
	if(list_ctrl != NULL)
		list_ctrl->DeleteAllItems();

	if (stBlobsResult.size() > 0)
	{
		m_stBlobsResult.clear();

		for (int i = 0; i < stBlobsResult.size(); i++)
			m_stBlobsResult.push_back(stBlobsResult[i]);

	}


	if (m_stBlobsResult.size() > 0)
	{
		milDisplay->SetColor(M_RGB888(255, 0, 0));
		for (int i = 0; i < m_stBlobsResult.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				//MgraArc(milDisplay->m_milIDGraph, milDisplay->GetIDOverlay(), m_stBlobsResult[i].dCenX, m_stBlobsResult[i].dCenY, m_stBlobsResult[i].dShortSize + j, m_stBlobsResult[i].dLongSize + j, m_stBlobsResult[i].dAngle, m_stBlobsResult[i].dAngle + 360);
				MgraArcAngle(milDisplay->m_milIDGraph, milDisplay->GetIDOverlay(), m_stBlobsResult[i].dCenX, m_stBlobsResult[i].dCenY, m_stBlobsResult[i].dLongSize + 3 + j, m_stBlobsResult[i].dShortSize + 3 + j, m_stBlobsResult[i].dAngle, m_stBlobsResult[i].dAngle + 360, m_stBlobsResult[i].dAngle, M_CONTOUR);
			}

			if (list_ctrl != NULL)
			{
				CString strText;
				strText.Format(_T("%02d"), i + 1);
				list_ctrl->InsertItem(i, strText);
				strText.Format(_T("%0.3f"), m_stBlobsResult[i].dShortSize * m_dPixelSizeX);
				list_ctrl->SetItemText(i, 1, strText);
				strText.Format(_T("%0.3f"), m_stBlobsResult[i].dLongSize * m_dPixelSizeX);
				list_ctrl->SetItemText(i, 2, strText);
				strText.Format(_T("%0.3f"), m_stBlobsResult[i].dLength * m_dPixelSizeX);
				list_ctrl->SetItemText(i, 3, strText);
				strText.Empty();
			}
			else
			{
#ifdef _DEBUG
				milDisplay->FontSize(M_FONT_DEFAULT_SMALL);
				CString strText;
				strText.Format(_T("S Size : %0.3f, L Size : %0.3f"), m_stBlobsResult[i].dShortSize * m_dPixelSizeX, m_stBlobsResult[i].dLongSize * m_dPixelSizeX);
				milDisplay->OutputText(m_stBlobsResult[i].dCenX - 10, m_stBlobsResult[i].dCenY - 30, strText.GetBuffer());
#endif
			}
		}
	}
}

BOOL CInspectionBlobs::ReleaseBlobsThread()
{
	m_bBlobsInspecting = FALSE;
	m_bBlobsInspectionEnd = FALSE;

	if (m_pThreadBlobsInsp)
		delete m_pThreadBlobsInsp;

	m_pThreadBlobsInsp = NULL;

	return TRUE;
}

void CInspectionBlobs::InitBlobsThread()
{
	if (GetBlobsThreadEnd())
	{
		ReleaseBlobsThread();
	}
	m_pThreadBlobsInsp = AfxBeginThread(BlobsInspThread, this, THREAD_PRIORITY_NORMAL);

	if (m_milBlobsInspImage != NULL)
	{
		m_milBlobsInspImage->Close();
		m_milBlobsInspImage = NULL;
	}

	m_milBlobsInspImage = CRepository::Instance()->m_MilLib.NewImage();
	m_milBlobsInspImage->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
}

/**
  @brief GainOffset
  Image Data에 Gain Offset 적용
*/
void CInspectionBlobs::GainOffset(BYTE* bySrc, BYTE* byDst, int nSrcPitch, int nSrcHeight, int nDstPitch, int nDstHeight, float fGain, int nOffset, int nAddOffsetVal)
{
	double dVal = 0.;

	for (int h = 2; h < nDstHeight; h++)
	{
		for (int w = 0; w < nDstPitch; w++)
		{
			if (fGain != 0.)
			{
				if(nAddOffsetVal == 0)
					dVal = ((bySrc[(h * nSrcPitch) + w] * fGain + nOffset) / 1);
				else
				{
					if (bySrc[(h * nSrcPitch) + w] > nAddOffsetVal)
						dVal = ((bySrc[(h * nSrcPitch) + w] * fGain + nOffset) / 1);
					else
						dVal = ((bySrc[(h * nSrcPitch) + w] * fGain - nOffset) / 1);
				}
				if (dVal > 255)
					byDst[(h * nDstPitch) + w] = 255;
				else if (dVal < 0)
					byDst[(h * nDstPitch) + w] = 0;
				else
					byDst[(h * nDstPitch) + w] = (BYTE)dVal;
			}
			else
			{
				byDst[(h * nDstPitch) + w] = bySrc[(h * nSrcPitch) + w];
			}
		}
	}

}

/**
  @brief AutoSearchArea
  
*/
void CInspectionBlobs::AutoSearchArea(CMilBuffer* srcImage, CMilBuffer* dstMaskImage, CMilDisplay *milDisplay)
{
	CMilBuffer* milFilter, * milProcessRslt1, *milProcessRslt2, * milProcessRslt3;
	milFilter = CRepository::Instance()->m_MilLib.NewImage();
	milFilter->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
	milProcessRslt1 = CRepository::Instance()->m_MilLib.NewImage();
	milProcessRslt1->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
	milProcessRslt2 = CRepository::Instance()->m_MilLib.NewImage();
	milProcessRslt2->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
	milProcessRslt3 = CRepository::Instance()->m_MilLib.NewImage();
	milProcessRslt3->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
	MIL_INT milRank = 1;
	MimRank(srcImage->GetIDBuffer(), milFilter->GetIDBuffer(), M_5X5_RECT, milRank, M_GRAYSCALE);

	ULONG nMaxIdx = 0;
	MIL_INT64* milHistoVal;
	milHistoVal = new MIL_INT64[256];

	MilHistogram(milFilter, 256, milHistoVal);
	int nThreshHold = 128;
	for (int i = 1; i < 256; i++)
	{
		if (milHistoVal[i] > m_stBlobsInspPara.stSearchAreaPara.nAreaVal)
		{
			nThreshHold = i;
			break;
		}
	}
		
	MimBinarize(milFilter->GetIDBuffer(), milProcessRslt1->GetIDBuffer(), M_GREATER_OR_EQUAL, m_stBlobsInspPara.stSearchAreaPara.nThreshHold, M_NULL);
#ifdef SAVE_INSP_IMAGE
	milProcessRslt1->Export(_T("d:\\AutoSearchArea1_Bin.bmp"), M_BMP);
#endif
	MimClose(milProcessRslt1->GetIDBuffer(), milProcessRslt2->GetIDBuffer(), m_stBlobsInspPara.stSearchAreaPara.nBlobOpenClose, M_BINARY);
	//MimOpen(milProcessRslt2->GetIDBuffer(), milProcessRslt1->GetIDBuffer(), m_stBlobsInspPara.stSearchAreaPara.nBlobOpenClose, M_BINARY);
#ifdef SAVE_INSP_IMAGE
	milProcessRslt2->Export(_T("d:\\AutoSearchArea2_Close.bmp"), M_BMP);
#endif
	MimArith(milProcessRslt2->GetIDBuffer(), M_NULL, milProcessRslt3->GetIDBuffer(), M_NOT);
#ifdef SAVE_INSP_IMAGE
	milProcessRslt3->Export(_T("d:\\AutoSearchArea3_Not.bmp"), M_BMP);
#endif
	CMilBlob* milBlobs;
	milBlobs = CRepository::Instance()->m_MilLib.NewBlobTool(milProcessRslt3);
	milBlobs->m_ResultList.pCXs = NULL;
	milBlobs->m_ResultList.pCYs = NULL;
	milBlobs->m_ResultList.pBoxLeft = NULL;
	milBlobs->m_ResultList.pBoxBottom = NULL;
	milBlobs->m_ResultList.pBoxRight = NULL;
	milBlobs->m_ResultList.pBoxTop = NULL;
	milBlobs->m_ResultList.pArea = NULL;
	milBlobs->AllocFeatureList();
	milBlobs->AllocResult();
	MblobControl(milBlobs->m_milResult, M_NUMBER_OF_FERETS, 180);
	milBlobs->SelectFeature(M_AREA);
	milBlobs->SelectFeature(M_FERET_MAX_DIAMETER);
	milBlobs->SelectFeature(M_FERET_MIN_DIAMETER);
	milBlobs->SelectFeature(M_LENGTH);
	milBlobs->SelectFeature(M_BOX_X_MIN);
	milBlobs->SelectFeature(M_BOX_Y_MIN);
	milBlobs->SelectFeature(M_BOX_X_MAX);
	milBlobs->SelectFeature(M_BOX_Y_MAX);
	milBlobs->SelectFeature(M_CENTER_OF_GRAVITY_X);
	milBlobs->SelectFeature(M_CENTER_OF_GRAVITY_Y);
	milBlobs->SelectFeature(M_CONVEX_PERIMETER);
	milBlobs->SelectFeature(M_CONVEX_HULL);

	milBlobs->Caculate();

	double dMinLenght, dMaxLenght;
	dMaxLenght = m_stBlobsInspPara.stSearchAreaPara.dLongLenght / m_dPixelSizeX;
	dMinLenght = m_stBlobsInspPara.stSearchAreaPara.dShortLenght / m_dPixelSizeX;
	
	//milBlobs->Select(M_INCLUDE, M_CONVEX_PERIMETER, M_ALL_BLOBS, M_NULL, M_NULL);
	milBlobs->Select(M_EXCLUDE, M_AREA, M_LESS_OR_EQUAL, m_stBlobsInspPara.stSearchAreaPara.nAreaVal, M_NULL);
	milBlobs->Select(M_EXCLUDE, M_FERET_MAX_DIAMETER, M_GREATER_OR_EQUAL, dMaxLenght, M_NULL);
	milBlobs->Select(M_EXCLUDE, M_FERET_MAX_DIAMETER, M_LESS_OR_EQUAL, dMinLenght, M_NULL);

	milBlobs->GetResult();
	int nBlobCnt = milBlobs->GetNumber();

	if (nBlobCnt > 0)
	{
		CMilBuffer* milBlobDraw;
		milBlobDraw = CRepository::Instance()->m_MilLib.NewImage();
		milBlobDraw->Alloc2D(CSize(m_BlobsImageSize.cx, m_BlobsImageSize.cy));
		milBlobDraw->EnableGraph();

		MblobDraw(milBlobDraw->GetIDGraph(), milBlobs->m_milResult, milBlobDraw->GetIDBuffer(), M_DRAW_BLOBS_CONTOUR + M_DRAW_CENTER_OF_GRAVITY + M_DRAW_CONVEX_HULL, M_INCLUDED_BLOBS, M_DEFAULT);

		if(dstMaskImage != NULL)
			dstMaskImage->Copy(milBlobDraw);

#ifdef SAVE_INSP_IMAGE
		milBlobDraw->Export(_T("d:\\AutoSearchArea3_Blob.bmp"), M_BMP);
#endif
		//milBlobDraw->Export(_T("d:\\TestBlobs.bmp"));

		if (milDisplay != NULL)
		{
			CMilBlob* milBlobs2;
			milBlobs2 = CRepository::Instance()->m_MilLib.NewBlobTool(milBlobDraw);
			milBlobs2->m_ResultList.pCXs = NULL;
			milBlobs2->m_ResultList.pCYs = NULL;
			milBlobs2->m_ResultList.pBoxLeft = NULL;
			milBlobs2->m_ResultList.pBoxBottom = NULL;
			milBlobs2->m_ResultList.pBoxRight = NULL;
			milBlobs2->m_ResultList.pBoxTop = NULL;
			milBlobs2->m_ResultList.pArea = NULL;
			milBlobs2->AllocFeatureList();
			milBlobs2->AllocResult();
			MblobControl(milBlobs2->m_milResult, M_NUMBER_OF_FERETS, 180);
			milBlobs2->SelectFeature(M_AREA);
			milBlobs2->SelectFeature(M_FERET_MAX_DIAMETER);
			milBlobs2->SelectFeature(M_FERET_MIN_DIAMETER);
			milBlobs2->SelectFeature(M_LENGTH);
			milBlobs2->SelectFeature(M_BOX_X_MIN);
			milBlobs2->SelectFeature(M_BOX_Y_MIN);
			milBlobs2->SelectFeature(M_BOX_X_MAX);
			milBlobs2->SelectFeature(M_BOX_Y_MAX);
			milBlobs2->SelectFeature(M_CENTER_OF_GRAVITY_X);
			milBlobs2->SelectFeature(M_CENTER_OF_GRAVITY_Y);
			milBlobs2->SelectFeature(M_CONVEX_PERIMETER);
			milBlobs2->SelectFeature(M_CONVEX_HULL);
			milBlobs2->Caculate();
			milBlobs2->GetResult();
			nBlobCnt = milBlobs2->GetNumber();

			if (nBlobCnt > 0)
			{
				milDisplay->SetColor(M_COLOR_BLUE);
				MblobDraw(milDisplay->GetIDGraph(), milBlobs2->m_milResult, milDisplay->GetIDOverlay(), M_DRAW_BLOBS_CONTOUR, M_INCLUDED_BLOBS, M_DEFAULT);
			}
			milBlobs2->Free();
			milBlobs2 = NULL;
		}

		milBlobDraw->Close();
		milBlobDraw = NULL;
	}
	else
	{
		if (dstMaskImage != NULL)
			dstMaskImage->Clear(M_COLOR_WHITE);
	}


	milBlobs->Free();
	delete[]milHistoVal;
	milFilter->Close();
	milProcessRslt1->Close();
	milProcessRslt2->Close();
	milProcessRslt3->Close();

	milBlobs = NULL;
	milFilter = NULL;
	milProcessRslt1 = NULL;
	milProcessRslt2 = NULL;
	milProcessRslt3 = NULL;
}

long CInspectionBlobs::GetThreshHoldArea(CMilBuffer* srcImage, CRect rect, int nThreshHold)
{
	long lArea = 0;
	


	return lArea;
}

void CInspectionBlobs::InitBlobsImage(long nWidth, long nHeight, BOOL isInit)
{
	m_BlobsImageSize.SetSize(nWidth, nHeight);

	InitBlobsThread();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInspectionClass::CInspectionClass()
{
	m_bMainLog = TRUE;
	m_bInspectionLog = TRUE;
	g_MainClass = this;
}

CInspectionClass::~CInspectionClass()
{


}

void CInspectionClass::WriteLogMessage(CString strLog)
{
	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);
	CString strText;

	strText.Format(_T("%02d:%02d:%02d:%03d - %s"), lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds, strLog);

	if (m_hWndMain != NULL)
	{
		if(m_bMainLog)
			::SendMessage(m_hWndMain, USE_MSG_LOG_TO_MAIN, (WPARAM)strText.GetBuffer(), strText.GetLength());

		if(m_bInspectionLog)
			::SendMessage(m_hWndMain, USE_MSG_LOG_TO_INSPECTION, (WPARAM)strText.GetBuffer(), strText.GetLength());
	}
	strText.Empty();
}
#endif
