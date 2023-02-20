// CDialogBlobsMasking.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogBlobsMasking.h"


// CDialogBlobsMasking 대화 상자

IMPLEMENT_DYNAMIC(CDialogBlobsMasking, CDialogEx)

CDialogBlobsMasking::CDialogBlobsMasking(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BLOBS_MASK, pParent)
	, m_chk_MaskPara0(FALSE)
	, m_chk_MaskPara1(FALSE)
{
	m_bLbtnMouseDown = FALSE;
	m_MilDisplay = NULL;
	m_MilDispBuff = NULL;
	m_MilResultDispBuff = NULL;
	m_MilMaskBuff = NULL;
	m_bMaskRemove = FALSE;
	m_nMaskSize = 20;
	m_InspectionClass = NULL;
}

CDialogBlobsMasking::~CDialogBlobsMasking()
{
	/*if (m_DlgRect != NULL)
	{
		m_DlgRect->DestroyWindow();
		delete m_DlgRect;
		m_DlgRect = NULL;
	}*/
}

void CDialogBlobsMasking::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_MASK_PARA_0, m_chk_MaskPara0);
	DDX_Check(pDX, IDC_CHECK_MASK_PARA_1, m_chk_MaskPara1);
	DDX_Control(pDX, IDC_COMBO_MASK_SIZE, m_combo_MaskSize);
}


BEGIN_MESSAGE_MAP(CDialogBlobsMasking, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MASKING_START, &CDialogBlobsMasking::OnBnClickedButtonMaskingStart)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDialogBlobsMasking::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDialogBlobsMasking::OnBnClickedButtonClose)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK_MASK_PARA_0, &CDialogBlobsMasking::OnBnClickedCheckMaskPara0)
	ON_BN_CLICKED(IDC_CHECK_MASK_PARA_1, &CDialogBlobsMasking::OnBnClickedCheckMaskPara1)
	ON_BN_CLICKED(IDC_BUTTON_MASKING_DELETE, &CDialogBlobsMasking::OnBnClickedButtonMaskingDelete)
	ON_CBN_EDITUPDATE(IDC_COMBO_MASK_SIZE, &CDialogBlobsMasking::OnCbnEditupdateComboMaskSize)
END_MESSAGE_MAP()


// CDialogBlobsMasking 메시지 처리기


void CDialogBlobsMasking::OnBnClickedButtonMaskingStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDialogBlobsMasking::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strFilePath;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	strFilePath.Format(_T("%s\\Recipe\\%s\\Blobs_Mask.bmp"), strTemp, m_strRecipeName);
	m_MilMaskBuff->ExportBMP(strFilePath);
	UpdatePara(TRUE);
}


void CDialogBlobsMasking::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->ShowWindow(SW_HIDE);
}


void CDialogBlobsMasking::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch ((int)nIDEvent)
	{
	case 0:
		if (this->IsWindowVisible())
		{
			KillTimer(0);

			m_InspectionClass = new CInspectionClass();
			m_InspectionClass->InitBlobsImage(m_ImageSize.cx, m_ImageSize.cy);

			CRect rect_temp;
			GetDlgItem(IDC_STATIC_MASKING_DISP)->GetWindowRect(&m_DispRect[0]);
			double dRet, dRet2;
			dRet = (double)m_DispRect[0].Width() / m_ImageSize.cx;
			dRet2 = (double)m_DispRect[0].Height() / m_ImageSize.cy;
			GetDlgItem(IDC_STATIC_MASKING_DISP)->GetClientRect(&rect_temp);
			if (dRet < dRet2)
			{
				m_DispRect[0].bottom = (long)(dRet * (double)m_ImageSize.cy + (double)m_DispRect[0].top);
				rect_temp.bottom = (long)(dRet * (double)m_ImageSize.cy + (double)rect_temp.top);
			}

			m_dRectRet = m_ImageSize.cx / (double)m_DispRect[0].Width();

			m_MilDisplay = CRepository::Instance()->m_MilLib.NewDisplay();
			if (m_MilDispBuff == NULL)
			{
				m_MilDispBuff = CRepository::Instance()->m_MilLib.NewImage();
				m_MilDispBuff->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
			}


			m_MilResultDispBuff = CRepository::Instance()->m_MilLib.NewImage();
			m_MilMaskBuff = CRepository::Instance()->m_MilLib.NewImage();

			
			m_MilDisplay->SetZoomFactor(dRet, dRet);

			m_MilResultDispBuff->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
			m_MilResultDispBuff->SelectWindow(m_MilDisplay, GetDlgItem(IDC_STATIC_MASKING_DISP)->m_hWnd);//m_DlgRect->m_hWnd);
			m_MilResultDispBuff->Copy(m_MilDispBuff);
			m_MilMaskBuff->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));

			m_MilDisplay->EnableOverlay();
			m_MilDisplay->EnableGraph();
			m_MilDisplay->SetColor(M_COLOR_GREEN);

			DrawMaskOverlay();

			CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MASK_BLOBS_PARA_0);
			CString strText;
			strText.Format(_T("%0.3f"), m_dSearchAreaCenX);
			pBtnEnh->SetWindowText(strText);

			pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_MASK_BLOBS_PARA_1);
			strText.Format(_T("%0.3f"), m_dSearchAreaCenY);
			pBtnEnh->SetWindowText(strText);

			m_chk_MaskPara0 = FALSE;
			m_bMaskRemove = FALSE;

			m_combo_MaskSize.SetCurSel(0);

			UpdatePara(FALSE);

		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDialogBlobsMasking::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		SetTimer(0, 100, NULL);
	}
	else
	{
		if (m_InspectionClass != NULL)
		{
			delete m_InspectionClass;
			m_InspectionClass = NULL;
		}

		if (m_MilDisplay != NULL)
		{
			m_MilDisplay->EnableGraph(FALSE);
			m_MilDisplay->EnableOverlay(FALSE);
			m_MilDisplay->Close();
			m_MilDisplay = NULL;
		}
		if (m_MilResultDispBuff != NULL)
		{
			m_MilResultDispBuff->Close();
			m_MilResultDispBuff = NULL;
		}
		if (m_MilDispBuff != NULL)
		{
			m_MilDispBuff->Close();
			m_MilDispBuff = NULL;
		}
		if (m_MilMaskBuff != NULL)
		{
			m_MilMaskBuff->Close();
			m_MilMaskBuff = NULL;
		}
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CDialogBlobsMasking::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//m_DlgRect = new CDialogRect();
	//m_DlgRect->Create(IDD_DIALOG_RECT, this);
	//m_DlgRect->SetDlgIdx(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogBlobsMasking::SetDispImage(CMilBuffer* src)
{
	if (src == NULL)
		return;

	tImageProperties pImageProperty;
	src->GetImageProperties(&pImageProperty);

	m_ImageSize = pImageProperty.ImageSize;
	if (m_MilDispBuff == NULL)
	{
		m_MilDispBuff = CRepository::Instance()->m_MilLib.NewImage();
		m_MilDispBuff->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
	}
	m_MilDispBuff->Copy(src);
}

BOOL CDialogBlobsMasking::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F9)
			OnBnClickedButtonSave();
		else if (pMsg->wParam == VK_F12)
			OnBnClickedButtonClose();
		else if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	CRect main_rect, temp_rect;
	CPoint point, pPoint;
	point = pMsg->pt;

	GetDlgItem(IDC_STATIC_MASKING_DISP)->GetWindowRect(&temp_rect);
	GetDlgItem(IDC_STATIC_MASKING_DISP)->GetClientRect(&main_rect);
	point.x -= temp_rect.left;
	point.y -= temp_rect.top;
	/*if(m_DlgRect == NULL)
		return CDialogEx::PreTranslateMessage(pMsg);*/

	if (/*m_DlgRect->IsWindowVisible() && */(this->IsWindowEnabled() ))//|| m_DlgRect->IsWindowEnabled()))
	{
		if (main_rect.left <= point.x && point.x <= main_rect.right &&
			main_rect.top <= point.y && point.y <= main_rect.bottom)
		{
			pPoint.x = (int)((double)point.x * m_dRectRet);
			pPoint.y = (int)((double)point.y * m_dRectRet);


			if (pPoint.x >= m_ImageSize.cx)
				pPoint.x = m_ImageSize.cx - 1;

			if (pPoint.y >= m_ImageSize.cy)
				pPoint.y = m_ImageSize.cy - 1;

			/*if (m_pImagePoint.x == pPoint.x && m_pImagePoint.y == pPoint.y)
			{

			}
			else*/
			{
#ifdef MIL_USE
				//int nVal = 0;
				//m_MilResultDispBuff->GetPixelGray(pPoint.x, pPoint.y, &nVal);

				CString strText;
				CBtnenhctrl* pBtn;
				pBtn = (CBtnenhctrl*)(GetDlgItem(IDC_BTNENHCTRL_MASK_POINT_X_VAL));
				strText.Format(_T("%d"), pPoint.x);
				pBtn->SetWindowText(strText);
				pBtn = (CBtnenhctrl*)(GetDlgItem(IDC_BTNENHCTRL_MASK_POINT_Y_VAL));
				strText.Format(_T("%d"), pPoint.y);
				pBtn->SetWindowText(strText);

				m_pImagePoint = pPoint;


				if (m_bLbtnMouseDown)
				{
					BYTE* pData;

					tImageProperties pImageProperty;
					m_MilMaskBuff->GetImageProperties(&pImageProperty);

					pData = DBG_NEW BYTE[pImageProperty.ImageSize.cx * pImageProperty.ImageSize.cy];

					CRect img_rect;
					img_rect.left = 0;
					img_rect.top = 0;
					img_rect.right = pImageProperty.ImageSize.cx;
					img_rect.bottom = pImageProperty.ImageSize.cy;
					m_MilMaskBuff->Get2D(pData, img_rect);

					int nMaskSize = m_nMaskSize / 3;

					for (int x = -nMaskSize ; x < (nMaskSize + 1); x++)
					{
						for (int y = -nMaskSize; y < (nMaskSize + 1); y++)
						{
							int nX, nY;
							nX = pPoint.x + x;
							nY = pPoint.y + y;

							if (nX < 0)
								nX = 0;

							if (nY < 0)
								nY = 0;

							if (nX > pImageProperty.ImageSize.cx)
								nX = pImageProperty.ImageSize.cx - 1;

							if (nY > pImageProperty.ImageSize.cy)
								nY = pImageProperty.ImageSize.cy - 1;

							if (m_bLbtnMouseDown)
							{
								
								if (!m_bMaskRemove)
								{
									pData[(nY * pImageProperty.ImageSize.cx) + nX] = 255;
								}
								else
								{
									pData[(nY * pImageProperty.ImageSize.cx) + nX] = 0;
								}
							}
						}
					}

					double dSx, dSy, dEx, dEy;
					dSx = pPoint.x - nMaskSize;
					if (dSx < 0)
						dSx = 0;

					dSy = pPoint.y - nMaskSize;
					if (dSy < 0)
						dSy = 0;

					dEx = pPoint.x + nMaskSize;
					if (dEx > m_ImageSize.cx)
						dEx = m_ImageSize.cx - 1;

					dEy = pPoint.y + nMaskSize;
					if (dEy > m_ImageSize.cx)
						dEy = m_ImageSize.cx - 1;

					if (!m_bMaskRemove)
					{
						m_MilDisplay->SetColor(M_COLOR_GREEN);
					}
					else
					{
						m_MilDisplay->SetColor(M_COLOR_RED);
					}

					MgraRectFill(m_MilDisplay->GetIDGraph(), m_MilDisplay->GetIDOverlay(), dSx, dSy, dEx, dEy);

					m_MilMaskBuff->Put2D(CSize(0, 0), pImageProperty.ImageSize, pData);

					delete[] pData;
				}

#endif
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogBlobsMasking::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bLbtnMouseDown = TRUE;
	switch (m_combo_MaskSize.GetCurSel())
	{
	case 0:
		m_nMaskSize = 3;
		break;
	case 1:
		m_nMaskSize = 5;
		break;
	case 2:
		m_nMaskSize = 10;
		break;
	case 3:
		m_nMaskSize = 20;
		break;
	case 4:
		m_nMaskSize = 30;
		break;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDialogBlobsMasking::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bLbtnMouseDown = FALSE;
	DrawMaskOverlay(FALSE);
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CDialogBlobsMasking::ClearMaskOverlay()
{
	m_MilDisplay->ClearOverlay();

	if (m_dSearchAreaCenX > 0. && m_dSearchAreaCenY > 0.)
	{
		m_MilDisplay->SetColor(M_COLOR_MAGENTA);

		MgraArcFill(m_MilDisplay->GetIDGraph(), m_MilDisplay->GetIDOverlay(), m_dSearchAreaCenX, m_dSearchAreaCenY, 3, 3, 0, 360);
	}
}

void CDialogBlobsMasking::DrawMaskOverlay(BOOL isLoadFile)
{
	if (m_ImageSize.cx == 0 || m_ImageSize.cy == 0)
		return;

	CMilBuffer* milBuffer;
	milBuffer = CRepository::Instance()->m_MilLib.NewImage();
	milBuffer->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
	if (isLoadFile)
	{
		CString strFilePath;
		wchar_t strTemp[MAX_CHAR_LENG];
		GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
		strFilePath.Format(_T("%s\\Recipe\\%s\\Blobs_Mask.bmp"), strTemp, m_strRecipeName);
		milBuffer->BufImport(strFilePath);
	}
	else
	{
		milBuffer->Copy(m_MilMaskBuff);
	}

	CMilBlob* milBlobs;
	milBlobs = CRepository::Instance()->m_MilLib.NewBlobTool(milBuffer);
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
	//milBlobs->SelectFeature(M_CONVEX_HULL);

	//milBlobs->Select(M_EXCLUDE, M_AREA, M_LESS_OR_EQUAL, 100, NULL);

	milBlobs->Caculate();

	int nBlobCnt = milBlobs->GetNumber();
	m_MilDisplay->ClearOverlay();
	double *dArea;
	dArea = new double[nBlobCnt];
	if (nBlobCnt > 0)
	{
		MblobGetResult(milBlobs->m_milResult, M_AREA, dArea);
		milBlobs->GetResult();
		m_MilDisplay->SetColor(M_COLOR_GREEN);
		MblobDraw(m_MilDisplay->GetIDGraph(), milBlobs->m_milResult, m_MilDisplay->GetIDOverlay(), M_DRAW_BLOBS, M_INCLUDED_BLOBS, M_DEFAULT);
	}

	//if (isLoadFile)
	//{
	//	double dMax = 0;
	//	for (int i = 0; i < nBlobCnt; i++)
	//	{
	//		dMax = max(dArea[i], dMax);
	//	}

	//	int nIdx = 0;
	//	for (int i = 0; i < nBlobCnt; i++)
	//	{
	//		if (dArea[i] == dMax)
	//		{
	//			nIdx = i;
	//			break;
	//		}
	//	}

	//	double dMaskCenX = (m_dSearchAreaCenX - milBlobs->m_ResultList.pCXs[nIdx]);
	//	double dMaskCenY = (m_dSearchAreaCenY - milBlobs->m_ResultList.pCYs[nIdx]);
	//	MimTranslate(m_MilDisplay->GetIDOverlay(), m_MilDisplay->GetIDOverlay(), dMaskCenX, dMaskCenY, M_DEFAULT);
	//}



	if (m_dSearchAreaCenX > 0. && m_dSearchAreaCenY > 0.)
	{
		m_MilDisplay->SetColor(M_COLOR_MAGENTA);
		
		MgraArcFill(m_MilDisplay->GetIDGraph(), m_MilDisplay->GetIDOverlay(), m_dSearchAreaCenX, m_dSearchAreaCenY, 3, 3, 0, 360);
	}

	
	delete[] dArea;

	m_MilMaskBuff->Copy(milBuffer);
	milBlobs->Free();
	milBuffer->Close();
}

void CDialogBlobsMasking::UpdatePara(BOOL isUpdate)
{
	if (isUpdate)
	{
		UpdateData(TRUE);
		m_stMaskingPara.isMasking = m_chk_MaskPara1;
	}
	else
	{

		m_chk_MaskPara1 = m_stMaskingPara.isMasking;
		UpdateData(FALSE);
	}
}

BOOL CDialogBlobsMasking::SaveMaskImage()
{
	BOOL rslt = TRUE;


	return rslt;
}

BOOL CDialogBlobsMasking::LoadMaskImage()
{
	BOOL rslt = TRUE;


	return rslt;
}

void CDialogBlobsMasking::OnBnClickedCheckMaskPara0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_chk_MaskPara0)
		m_chk_MaskPara0 = FALSE;
	else
		m_chk_MaskPara0 = TRUE;

	m_bMaskRemove = m_chk_MaskPara0;
}


void CDialogBlobsMasking::OnBnClickedCheckMaskPara1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_chk_MaskPara1)
		m_chk_MaskPara1 = FALSE;
	else
		m_chk_MaskPara1 = TRUE;

	m_bMaskingUse = m_chk_MaskPara1;
}


void CDialogBlobsMasking::OnBnClickedButtonMaskingDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE* pData;

	tImageProperties pImageProperty;
	m_MilMaskBuff->GetImageProperties(&pImageProperty);

	pData = DBG_NEW BYTE[pImageProperty.ImageSize.cx * pImageProperty.ImageSize.cy];

	CRect img_rect;
	img_rect.left = 0;
	img_rect.top = 0;
	img_rect.right = pImageProperty.ImageSize.cx;
	img_rect.bottom = pImageProperty.ImageSize.cy;
	m_MilMaskBuff->Get2D(pData, img_rect);
	memset(pData, NULL, sizeof(BYTE) * (pImageProperty.ImageSize.cx * pImageProperty.ImageSize.cy));
	m_MilMaskBuff->Put2D(CSize(0, 0), pImageProperty.ImageSize, pData);
	ClearMaskOverlay();
	delete[] pData;
}


void CDialogBlobsMasking::OnCbnEditupdateComboMaskSize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
