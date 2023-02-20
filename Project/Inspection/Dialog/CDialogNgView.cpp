// CDialogNgView.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogNgView.h"
#include "Repository.h"

// CDialogNgView 대화 상자

IMPLEMENT_DYNAMIC(CDialogNgView, CDialogEx)

CDialogNgView::CDialogNgView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_NG_VIEW, pParent)
{
	m_milMainDisplay = NULL;
	m_milMainImage = NULL;
	m_milZoomDisplay = NULL;
	m_milZoomImage = NULL;
}

CDialogNgView::~CDialogNgView()
{
	m_ResultList.clear();

	if (m_milMainDisplay != NULL)
	{
		m_milMainDisplay->EnableOverlay(FALSE);
		m_milMainDisplay->EnableOverlay(FALSE);
		m_milMainDisplay->Close();
		m_milMainDisplay = NULL;
	}
	if (m_milMainImage != NULL)
	{
		m_milMainImage->Close();
		m_milMainImage = NULL;
	}
	if (m_milZoomDisplay != NULL)
	{
		m_milZoomDisplay->EnableOverlay(FALSE);
		m_milZoomDisplay->EnableOverlay(FALSE);
		m_milZoomDisplay->Close();
		m_milZoomDisplay = NULL;
	}
	if (m_milZoomImage != NULL)
	{
		m_milZoomImage->Close();
		m_milZoomImage = NULL;
	}
}

void CDialogNgView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NG, m_list_Ng);
	DDX_Control(pDX, IDC_LIST_RESULT, m_list_ResultList);
}


BEGIN_MESSAGE_MAP(CDialogNgView, CDialogEx)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_CLICK, IDC_LIST_NG, &CDialogNgView::OnNMClickListNg)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDialogNgView::OnBnClickedButtonClose)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RESULT, &CDialogNgView::OnNMClickListResult)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_RESULT, &CDialogNgView::OnLvnKeydownListResult)
END_MESSAGE_MAP()


// CDialogNgView 메시지 처리기


void CDialogNgView::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogNgView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CDialogNgView::SetInsertResultList(CString strResultName)
{
	m_ResultList.push_back(strResultName);
	m_list_Ng.InsertItem(0, m_ResultList[m_ResultList.size() - 1]);
}

void CDialogNgView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		m_milMainDisplay = CRepository::Instance()->m_MilLib.NewDisplay();
		m_milMainImage = CRepository::Instance()->m_MilLib.NewImage();
		m_milZoomDisplay = CRepository::Instance()->m_MilLib.NewDisplay();
		m_milZoomImage = CRepository::Instance()->m_MilLib.NewImage();

		m_milZoomImage->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
		m_milMainImage->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));

		CRect rect_temp;
		GetDlgItem(IDC_STATIC_ORG_IMAGE)->GetWindowRect(&rect_temp);
		double dRet, dRet2;
		dRet = (double)rect_temp.Width() / m_ImageSize.cx;
		dRet2 = (double)rect_temp.Height() / m_ImageSize.cy;
		if (dRet < dRet2)
		{
			rect_temp.bottom = (long)(dRet * (double)m_ImageSize.cy + (double)rect_temp.top);
		}

		m_milMainImage->SelectWindow(m_milMainDisplay, GetDlgItem(IDC_STATIC_ORG_IMAGE)->m_hWnd);
		m_milZoomImage->SelectWindow(m_milZoomDisplay, GetDlgItem(IDC_STATIC_NG_ZOOM_IMAGE)->m_hWnd);
		m_milMainDisplay->SetZoomFactor(dRet, dRet);
		m_milMainDisplay->EnableOverlay();
		m_milMainDisplay->EnableGraph();
		m_milMainDisplay->SetColor(M_RGB888(0, 0, 255));


		m_ResultList.clear();
		CString strFilePath;
		CTime m_Time = CTime::GetCurrentTime();;
		wchar_t strTemp[MAX_CHAR_LENG];
		GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
		strFilePath.Format(_T("%s\\ResultData\\%04d\\%02d\\%02d\\%s\\"), strTemp, m_Time.GetYear(), m_Time.GetMonth(), m_Time.GetDay(), m_strCurrentRecipe);
		GetFolderInFolderName(strFilePath, &m_ResultList);

		for (int i = 0; i < m_ResultList.size(); i++)
		{
			m_list_ResultList.InsertItem(0, m_ResultList[i]);
		}

		m_nSelectResultZoomDataIdx = -1;
		m_nSelectResultDataIdx = -1;
	}
	else
	{
		m_list_ResultList.DeleteAllItems();
		m_list_Ng.DeleteAllItems();
		m_ResultList.clear();

		if (m_milMainDisplay != NULL)
		{
			m_milMainDisplay->EnableOverlay(FALSE);
			m_milMainDisplay->EnableGraph(FALSE);
			m_milMainDisplay->Close();
			m_milMainDisplay = NULL;
		}
		if (m_milMainImage != NULL)
		{
			m_milMainImage->Close();
			m_milMainImage = NULL;
		}
		if (m_milZoomDisplay != NULL)
		{
			m_milZoomDisplay->EnableOverlay(FALSE);
			m_milZoomDisplay->EnableGraph(FALSE);
			m_milZoomDisplay->Close();
			m_milZoomDisplay = NULL;
		}
		if (m_milZoomImage != NULL)
		{
			m_milZoomImage->Close();
			m_milZoomImage = NULL;
		}
	}
}


BOOL CDialogNgView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString strCol;
	LV_COLUMN m_lvColumn;

	m_lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt = LVCFMT_LEFT | LVCFMT_FIXED_WIDTH;

	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			strCol.Format(_T("번호"));
			m_lvColumn.cx = 50;
			break;
		case 1:
			strCol.Format(_T("짧은 길이"));
			m_lvColumn.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;
			m_lvColumn.cx = 69;
			break;
		case 2:
			strCol.Format(_T("긴 길이"));
			m_lvColumn.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;
			m_lvColumn.cx = 69;
			break;
		case 3:
			strCol.Format(_T("전체 길이"));
			m_lvColumn.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;
			m_lvColumn.cx = 69;
			break;
		}
		m_lvColumn.pszText = strCol.GetBuffer();
		m_lvColumn.iSubItem = i;// +1;
		m_list_Ng.InsertColumn(i + 1, &m_lvColumn);
	}

	m_lvColumn.fmt = LVCFMT_LEFT | LVCFMT_FIXED_WIDTH;
	strCol.Format(_T("파일 이름"));
	m_lvColumn.cx = 50+69+69+69;
	m_lvColumn.pszText = strCol.GetBuffer();
	m_lvColumn.iSubItem = 1;
	m_list_ResultList.InsertColumn(1, &m_lvColumn);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDialogNgView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
		else if (pMsg->wParam == VK_F12)
			OnBnClickedButtonClose();
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogNgView::OnNMClickListNg(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int idx = pNMListView->iItem;
	if (idx < 0)
		return;

	if(idx != m_nSelectResultZoomDataIdx)
		ZoomResultDisplay(idx);	

	*pResult = 0;
}

void CDialogNgView::ReadNgData()
{

}

BOOL CDialogNgView::ReadData(CString strFilePath)
{
	if (m_nSelectResultDataIdx == -1)
		return FALSE;

	CString strText;
	CStdioFile pFile;
	BOOL bOpen = pFile.Open(strFilePath, CFile::modeRead);

	m_ResultData.clear();

	strText = m_list_ResultList.GetItemText(m_nSelectResultDataIdx, 0);
	m_strSelectResultFileNmae.Format(_T("%s"), strText.Mid(0, strText.Find('R', 0) - 1));
	m_bResult = FALSE;
	if (bOpen)
	{
		while (TRUE)
		{
			BOOL bRead = pFile.ReadString(strText);

			if (bRead)
			{
				if (strText.Find(_T("Index,")) < 0 && !strText.IsEmpty())
				{
					CString strData;
					BLOB_RESULT_PARA rslt_data;
					int i = 1;
					AfxExtractSubString(strData, strText, i++, ',');
					rslt_data.dCenX = _wtof(strData);
					AfxExtractSubString(strData, strText, i++, ',');
					rslt_data.dCenY = _wtof(strData);
					AfxExtractSubString(strData, strText, i++, ',');
					rslt_data.dAngle = _wtof(strData);
					AfxExtractSubString(strData, strText, i++, ',');
					rslt_data.dShortSize = _wtof(strData);
					AfxExtractSubString(strData, strText, i++, ',');
					rslt_data.dLongSize = _wtof(strData);
					AfxExtractSubString(strData, strText, i++, ',');
					rslt_data.dLength = _wtof(strData);
					m_ResultData.push_back(rslt_data);
				}
			}
			else
				break;
		}
	}

	pFile.Close();

	BOOL bRslt = m_list_Ng.DeleteAllItems();

	if (m_ResultData.size() > 0)
	{
		InitNgList();
		bRslt = FALSE;
		m_bResult = FALSE;
	}
	else
	{
		bRslt = TRUE;
		m_bResult = TRUE;
	}

	return bRslt;
}

void CDialogNgView::InitNgList()
{
	CString strText;
	int j = 1;	
	
	for (int i = 0; i < m_ResultData.size(); i++)
	{
		j = 1;
		strText.Format(_T("%d"), i + 1);
		m_list_Ng.InsertItem(i, strText);
		strText.Format(_T("%0.3f"), m_ResultData[i].dShortSize * m_dPixelSizeX);
		m_list_Ng.SetItemText(i, j++, strText);
		strText.Format(_T("%0.3f"), m_ResultData[i].dLongSize * m_dPixelSizeX);
		m_list_Ng.SetItemText(i, j++, strText);
		strText.Format(_T("%0.3f"), m_ResultData[i].dLength * m_dPixelSizeX);
		m_list_Ng.SetItemText(i, j++, strText);
	}
	//UpdateData(FALSE);
}

void CDialogNgView::ReadImage(CString strFilePath)
{
	CMilBuffer* milBuffer;
	milBuffer = CRepository::Instance()->m_MilLib.NewImage(16);
	milBuffer->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
	milBuffer->BufImport(strFilePath, M_TIFF);
	CRect img_rect;
	img_rect.left = 0;
	img_rect.top = 0;
	img_rect.right = m_ImageSize.cx;
	img_rect.bottom = m_ImageSize.cy;

	WORD* pData1;
	pData1 = new WORD[m_ImageSize.cx * m_ImageSize.cy];
	milBuffer->Get2D(pData1, img_rect);
	milBuffer->Close();
	BYTE* byData;
	byData = new BYTE[m_ImageSize.cx * m_ImageSize.cy];
	for (int y = 0; y < m_ImageSize.cy; y++)
	{
		for (int x = 0; x < m_ImageSize.cx; x++)
		{
			byData[(y * m_ImageSize.cx) + x] = pData1[(y * m_ImageSize.cx) + x] / 256;
		}
	}

	m_milMainImage->Put2D(CSize(0, 0), m_ImageSize, byData);

	delete[] byData;
	delete[] pData1;

	m_milMainDisplay->ClearOverlay();
	m_milZoomDisplay->ClearOverlay();
	if (m_bResult)
	{
		m_milZoomImage->Clear(M_COLOR_BLACK);
	}
	else
	{
		m_milZoomImage->Copy(m_milMainImage);
	}
}

void CDialogNgView::ResultDisplay()
{
	m_milMainDisplay->ClearOverlay();
	m_milMainDisplay->EnableGraph(FALSE);
	m_milMainDisplay->EnableOverlay(FALSE);
	m_milMainDisplay->EnableGraph();
	m_milMainDisplay->EnableOverlay();
	m_milMainDisplay->FontSize(2);

	if (m_ResultData.size() > 0)
	{
		m_milMainDisplay->SetColor(M_RGB888(255, 0, 0));
		for (int i = 0; i < m_ResultData.size(); i++)
		{
			for (int j = 0; j < 3; j++)
				MgraArcAngle(m_milMainDisplay->m_milIDGraph, m_milMainDisplay->GetIDOverlay(), m_ResultData[i].dCenX, m_ResultData[i].dCenY, m_ResultData[i].dLongSize + j, m_ResultData[i].dShortSize + j, m_ResultData[i].dAngle, m_ResultData[i].dAngle + 360, m_ResultData[i].dAngle, M_CONTOUR);
				//MgraArc(m_milMainDisplay->m_milIDGraph, m_milMainDisplay->GetIDOverlay(), m_ResultData[i].dCenX, m_ResultData[i].dCenY, m_ResultData[i].dLongSize + j, m_ResultData[i].dShortSize + j, m_ResultData[i].dAngle, m_ResultData[i].dAngle + 360);


			double dX, dY;
			dX = m_ResultData[i].dCenX - 30;// (m_ResultData[i].dLongSize);// / 2.);
			dY = m_ResultData[i].dCenY - 30;// (m_ResultData[i].dLongSize);// / 2.);
			char cText[50];
			CString strText;
			strText.Format(_T("%d"), i + 1);
			WideCharToMultiByteChar(strText, cText);
			m_milMainDisplay->OutputText(dX, dY, strText.GetBuffer());
		}

		ZoomResultDisplay(0);
	}	
}

void CDialogNgView::ZoomResultDisplay(int nIdx)
{
	m_nSelectResultZoomDataIdx = nIdx;
	double dRet = 0., dRet2;
	CRect rect;
	GetDlgItem(IDC_STATIC_NG_ZOOM_IMAGE)->GetWindowRect(&rect);
	dRet = (double)rect.Width() / (m_ResultData[nIdx].dLongSize + 20);
	dRet2 = (double)rect.Height() / (m_ResultData[nIdx].dLongSize + 20);
	if (dRet < dRet2)
	{
		rect.bottom = (long)(dRet * (double)(m_ResultData[nIdx].dLongSize + 50) + (double)rect.top);
	}

	m_milZoomImage->SelectWindow(m_milZoomDisplay, GetDlgItem(IDC_STATIC_NG_ZOOM_IMAGE)->m_hWnd);
	m_milZoomDisplay->SetZoomFactor(dRet, dRet);
	m_milZoomDisplay->ClearOverlay();
	m_milZoomDisplay->EnableGraph(FALSE);
	m_milZoomDisplay->EnableOverlay(FALSE);
	m_milZoomDisplay->EnableGraph();
	m_milZoomDisplay->EnableOverlay();

	//결과 data display
	CSize nZoomSize((int)(m_ResultData[nIdx].dLongSize + 20), (int)(m_ResultData[nIdx].dLongSize + 20));
	
	MdispPan(m_milZoomDisplay->GetDisplayID(), m_ResultData[nIdx].dCenX - (nZoomSize.cx / 2), m_ResultData[nIdx].dCenY - (nZoomSize.cy / 2));

	m_milZoomDisplay->SetColor(M_RGB888(255, 0, 0));
	for (int j = 0; j < 1; j++)
		MgraArcAngle(m_milZoomDisplay->m_milIDGraph, m_milZoomDisplay->GetIDOverlay(), m_ResultData[nIdx].dCenX, m_ResultData[nIdx].dCenY, m_ResultData[nIdx].dLongSize + j, m_ResultData[nIdx].dShortSize + j, m_ResultData[nIdx].dAngle, m_ResultData[nIdx].dAngle + 360, m_ResultData[nIdx].dAngle, M_CONTOUR);
		//MgraArc(m_milZoomDisplay->m_milIDGraph, m_milZoomDisplay->GetIDOverlay(), m_ResultData[nIdx].dCenX, m_ResultData[nIdx].dCenY, m_ResultData[nIdx].dLongSize + j, m_ResultData[nIdx].dShortSize + j, m_ResultData[nIdx].dAngle, m_ResultData[nIdx].dAngle + 360);

	//m_milZoomDisplay->FontSize(1);
	MgraFontScale(m_milZoomDisplay->m_milIDGraph, 0.5, 0.5);
	double dX, dY;
	dX = m_ResultData[nIdx].dCenX - 12;// (m_ResultData[i].dLongSize);// / 2.);
	dY = m_ResultData[nIdx].dCenY - 12;// (m_ResultData[i].dLongSize);// / 2.);
	char cText[50];
	CString strText;
	strText.Format(_T("%d"), nIdx + 1);
	WideCharToMultiByteChar(strText, cText);
	//m_milZoomDisplay->OutputText(dX, dY, strText.GetBuffer());
}

void CDialogNgView::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->ShowWindow(SW_HIDE);
}


void CDialogNgView::OnNMClickListResult(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int idx = pNMListView->iItem;

	if (m_nSelectResultDataIdx != idx)
	{
		m_nSelectResultDataIdx = idx;
		CString strText;
		strText = m_list_ResultList.GetItemText(idx, 0);
		CString strFilePath;
		CTime m_Time = CTime::GetCurrentTime();;
		wchar_t strTemp[MAX_CHAR_LENG];
		GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
		strFilePath.Format(_T("%s\\ResultData\\%04d\\%02d\\%02d\\%s\\%s"), strTemp, m_Time.GetYear(), m_Time.GetMonth(), m_Time.GetDay(), m_strCurrentRecipe, strText);

		if(ReadData(strFilePath))
			strFilePath.Format(_T("%s\\Orig\\%04d\\%02d\\%02d\\OK\\%s\\%s.tiff"), strTemp, m_Time.GetYear(), m_Time.GetMonth(), m_Time.GetDay(), m_strCurrentRecipe, m_strSelectResultFileNmae);
		else
			strFilePath.Format(_T("%s\\Orig\\%04d\\%02d\\%02d\\NG\\%s\\%s.tiff"), strTemp, m_Time.GetYear(), m_Time.GetMonth(), m_Time.GetDay(), m_strCurrentRecipe, m_strSelectResultFileNmae);
		ReadImage(strFilePath);
		ResultDisplay();
	}
	*pResult = 0;
}


void CDialogNgView::OnLvnKeydownListResult(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int idx = 0;
	
	if (pLVKeyDow->wVKey == 40)
		m_nSelectResultDataIdx++;
	else if (pLVKeyDow->wVKey == 38)
		m_nSelectResultDataIdx--;
	else
	{
		m_list_ResultList.SetSelectedColumn(m_nSelectResultDataIdx);
		return;
	}

	if (m_nSelectResultDataIdx < 0)
		m_nSelectResultDataIdx = 0;

	if (m_nSelectResultDataIdx > m_list_ResultList.GetItemCount())
		m_nSelectResultDataIdx = m_list_ResultList.GetItemCount() - 1;

	idx = m_nSelectResultDataIdx;
	if (idx < m_list_ResultList.GetItemCount())
	{
		CString strText;
		strText = m_list_ResultList.GetItemText(idx, 0);
		CString strFilePath;
		CTime m_Time = CTime::GetCurrentTime();;
		wchar_t strTemp[MAX_CHAR_LENG];
		GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
		strFilePath.Format(_T("%s\\ResultData\\%04d\\%02d\\%02d\\%s\\%s"), strTemp, m_Time.GetYear(), m_Time.GetMonth(), m_Time.GetDay(), m_strCurrentRecipe, strText);

		if (ReadData(strFilePath))
			strFilePath.Format(_T("%s\\Orig\\%04d\\%02d\\%02d\\OK\\%s\\%s.tiff"), strTemp, m_Time.GetYear(), m_Time.GetMonth(), m_Time.GetDay(), m_strCurrentRecipe, m_strSelectResultFileNmae);
		else
			strFilePath.Format(_T("%s\\Orig\\%04d\\%02d\\%02d\\NG\\%s\\%s.tiff"), strTemp, m_Time.GetYear(), m_Time.GetMonth(), m_Time.GetDay(), m_strCurrentRecipe, m_strSelectResultFileNmae);
		ReadImage(strFilePath);
		ResultDisplay();
	}
	else
		m_nSelectResultDataIdx--;

	*pResult = 0;
}
