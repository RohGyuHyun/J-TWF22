// CDialogTeachinBlobs.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "InspectionDlg.h"
#include "afxdialogex.h"
#include "CDialogTeachingBlobs.h"
#include "CIOClass.h"
#include "GetNumDlg.h"
#include "Repository.h"
#include "CATDetector.h"
#include "CInverterClass.h"


// CDialogTeachinBlobs 대화 상자

IMPLEMENT_DYNAMIC(CDialogTeachingBlobs, CDialogEx)

CDialogTeachingBlobs::CDialogTeachingBlobs(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INSPECTION_TEACHING_BLOBS, pParent)
	, m_edit_nInspTypeIndex(1)
{
	m_bGrabStart = FALSE;
	m_MilDisplay = NULL;
	m_MilDispBuff = NULL;
	m_MilResultDispBuff = NULL;
	m_dlgMasking = NULL;
	m_InspectionClass = NULL;
	m_Mil16bitBuff = NULL;
}

CDialogTeachingBlobs::~CDialogTeachingBlobs()
{
	if (m_dlgMasking != NULL)
	{
		m_dlgMasking->DestroyWindow();
		delete m_dlgMasking;
		m_dlgMasking = NULL;
	}

	if (m_dlgXRayStatus != NULL)
	{
		m_dlgXRayStatus->DestroyWindow();
		delete m_dlgXRayStatus;
		m_dlgXRayStatus = NULL;
	}

	/*if (m_DlgRect != NULL)
	{
		m_DlgRect->SetActiveImageRect(TRUE);
		m_DlgRect->DeleteAll();
		m_DlgRect->DestroyWindow();
		delete m_DlgRect;
		m_DlgRect = NULL;
	}*/

	if (m_MilDisplay != NULL)
	{
		m_MilDisplay->Close();
		m_MilDisplay = NULL;
	}
	if (m_MilDispBuff != NULL)
	{
		m_MilDispBuff->Close();
		m_MilDispBuff = NULL;
	}
	if (m_MilResultDispBuff != NULL)
	{
		m_MilResultDispBuff->Close();
		m_MilResultDispBuff = NULL;
	}

	if (m_Mil16bitBuff != NULL)
	{
		m_Mil16bitBuff->Close();
		m_Mil16bitBuff = NULL;
	}

}

void CDialogTeachingBlobs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	int i = 0;
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_0, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_1, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_2, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_3, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_4, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_5, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_6, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_7, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_8, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_9, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_10, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_11, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_12, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_13, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_14, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_15, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_16, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_18, m_ctrTeachingPara[i++]);

	DDX_Control(pDX, IDC_SLIDER_MIN, m_slider_Min);
	DDX_Control(pDX, IDC_SLIDER_MAX, m_slider_Max);
	DDX_Text(pDX, IDC_EDIT_INSP_TYPE_INDEX, m_edit_nInspTypeIndex);
}


BEGIN_MESSAGE_MAP(CDialogTeachingBlobs, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_IMG_LOAD, &CDialogTeachingBlobs::OnBnClickedButtonImgLoad)
	ON_BN_CLICKED(IDC_BUTTON_IMG_SAVE, &CDialogTeachingBlobs::OnBnClickedButtonImgSave)
	ON_BN_CLICKED(IDC_BUTTON_IMG_GRAB, &CDialogTeachingBlobs::OnBnClickedButtonImgGrab)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY_ORIGINAL_IMAGE, &CDialogTeachingBlobs::OnBnClickedButtonDisplayOriginalImage)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDialogTeachingBlobs::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDialogTeachingBlobs::OnBnClickedButtonClose)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_BLOBS_SEARCH, &CDialogTeachingBlobs::OnBnClickedButtonBlobsSearch)
	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDC_BUTTON_BLOBS_PRE_PROCESS, &CDialogTeachingBlobs::OnBnClickedButtonBlobsPreProcess)
	ON_MESSAGE(USE_MSG_IMAGE_VIEW_TO_TEACHING_BLOBS_DIALOG, &CDialogTeachingBlobs::OnImageViewToTeachingMessage)
	ON_BN_CLICKED(IDC_BUTTON_SET_SCAN_SPEED, &CDialogTeachingBlobs::OnBnClickedButtonSetScanSpeed)
	ON_BN_CLICKED(IDC_BUTTON_BLOBS_MASKING, &CDialogTeachingBlobs::OnBnClickedButtonBlobsMasking)
	ON_BN_CLICKED(IDC_BUTTON_BLOBS_SEARCH_AREA_START, &CDialogTeachingBlobs::OnBnClickedButtonBlobsSearchAreaStart)
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_INSP_TYPE_DOWN, &CDialogTeachingBlobs::OnBnClickedButtonInspTypeDown)
	ON_BN_CLICKED(IDC_BUTTON_INSP_TYPE_UP, &CDialogTeachingBlobs::OnBnClickedButtonInspTypeUp)
END_MESSAGE_MAP()


// CDialogTeachinBlobs 메시지 처리기
BOOL CDialogTeachingBlobs::LoadTeachingPara()
{
	CString key, dat;
	CString file_path, pat_path, extention, master_path, cur_path;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	file_path.Format(_T("%s\\Recipe\\%s\\Teaching.ini"), strTemp, m_strCurrentRecipeName);

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(file_path);
	CString strNode, strKey, strLabel;

	m_vInspPara.clear();
	strNode.Format(_T("TEACHING_BLOBS_INSP_TYPE"));
	strKey.Format(_T("INSP_TYPE_CNT"));
	int nInspTypeCnt = cDataIf.GetInt(strNode, strKey, 1);

	for (int i = 0; i < nInspTypeCnt; i++)
	{
		strNode.Format(_T("TEACHING_BLOBS_PARAMETER_%d"), i);

		strKey.Format(_T("X_RAY_KV"));
		m_InspPara.dVoltage = cDataIf.GetDouble(strNode, strKey, 10.);
		strKey.Format(_T("X_RAY_MA"));
		m_InspPara.dCurrent = cDataIf.GetDouble(strNode, strKey, 10.);

		strKey.Format(_T("SEARCH_AREA_USE"));
		m_InspPara.stSearchAreaPara.bAutoSearchArea = cDataIf.GetInt(strNode, strKey, 0);
		strKey.Format(_T("SEARCH_AREA_THRESH_HOLD"));
		m_InspPara.stSearchAreaPara.nThreshHold = cDataIf.GetInt(strNode, strKey, 128);
		strKey.Format(_T("SEARCH_AREA_SHORT_LENGHT"));
		m_InspPara.stSearchAreaPara.dShortLenght = cDataIf.GetDouble(strNode, strKey, 1.);
		strKey.Format(_T("SEARCH_AREA_LONG_LENGHT"));
		m_InspPara.stSearchAreaPara.dLongLenght = cDataIf.GetDouble(strNode, strKey, 10.);
		strKey.Format(_T("SEARCH_AREA_AREA_VAL"));
		m_InspPara.stSearchAreaPara.nAreaVal = cDataIf.GetInt(strNode, strKey, 10000);
		strKey.Format(_T("SEARCH_AREA_BLOB_OPEN_CLOSE"));
		m_InspPara.stSearchAreaPara.nBlobOpenClose = cDataIf.GetInt(strNode, strKey, 5);

		strKey.Format(_T("PRE_PROCESS_USE"));
		m_InspPara.stPreProcess.bPreProcess = cDataIf.GetInt(strNode, strKey, 1);
		strKey.Format(_T("PRE_PROCESS_RANK"));
		m_InspPara.stPreProcess.nRank = cDataIf.GetInt(strNode, strKey, 1);
		strKey.Format(_T("PRE_PROCESS_GAIN"));
		m_InspPara.stPreProcess.fGain = (float)cDataIf.GetDouble(strNode, strKey, 2.);
		strKey.Format(_T("PRE_PROCESS_OFFSET"));
		m_InspPara.stPreProcess.nOffset = cDataIf.GetInt(strNode, strKey, 0);
		strKey.Format(_T("PRE_PROCESS_ILLUMINATE_WIDTH_HEIGHT"));
		m_InspPara.stPreProcess.nIlluminateWH = cDataIf.GetInt(strNode, strKey, 50);
		strKey.Format(_T("PRE_PROCESS_ILLUMINATE_FACTOR"));
		m_InspPara.stPreProcess.dIlluminateFactor = cDataIf.GetDouble(strNode, strKey, 0.5);

		strKey.Format(_T("BLOB_MIN_THRESH_HOLD"));
		m_InspPara.nBlobMinThreshHold = cDataIf.GetInt(strNode, strKey, 50);
		strKey.Format(_T("BLOB_MAX_THRESH_HOLD"));
		m_InspPara.nBlobMaxThreshHold = cDataIf.GetInt(strNode, strKey, 200);

		strKey.Format(_T("BLOB_MIN_AREA"));
		m_InspPara.dBlobMinArea = cDataIf.GetDouble(strNode, strKey, 1. * 1.);
		strKey.Format(_T("BLOB_MAX_AREA"));
		m_InspPara.dBlobMaxArea = cDataIf.GetDouble(strNode, strKey, 10. * 10.);
		strKey.Format(_T("BLOB_MIN_LENGHT"));
		m_InspPara.dBlobMinLenght = cDataIf.GetDouble(strNode, strKey, 1.);
		strKey.Format(_T("BLOB_MAX_LENGHT"));
		m_InspPara.dBlobMaxLenght = cDataIf.GetDouble(strNode, strKey, 10.);
		strKey.Format(_T("BLOB_NEARBY_VAL"));
		m_InspPara.nBlobNearbyVal = cDataIf.GetInt(strNode, strKey, 10);

		strKey.Format(_T("MASK_USE"));
		m_InspPara.stMaskingPara.isMasking = cDataIf.GetInt(strNode, strKey, 0);
		strKey.Format(_T("MASK_SEARCH_AREA_CENTER_X"));
		m_InspPara.stMaskingPara.dCenX = cDataIf.GetDouble(strNode, strKey, 0);
		strKey.Format(_T("MASK_SEARCH_AREA_CENTER_Y"));
		m_InspPara.stMaskingPara.dCenY = cDataIf.GetDouble(strNode, strKey, 0);

		strKey.Format(_T("SCALE_IMAGE_USE"));
		m_InspPara.isScale = cDataIf.GetInt(strNode, strKey, 1);
		strKey.Format(_T("SCALE_MIN_THRESH_HOLD"));
		m_InspPara.nScaleMin = cDataIf.GetInt(strNode, strKey, 0);
		strKey.Format(_T("SCALE_MAX_THRESH_HOLD"));
		m_InspPara.nScaleMax = cDataIf.GetInt(strNode, strKey, 255);

		m_vInspPara.push_back(m_InspPara);
	}

	return TRUE;
}

BOOL CDialogTeachingBlobs::SaveTeachingPara()
{
	CString key, dat;
	CString file_path, pat_path, extention, master_path, cur_path;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	file_path.Format(_T("%s\\Recipe\\%s\\Teaching.ini"), strTemp, m_strCurrentRecipeName);

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(file_path);
	CString strNode, strKey, strLabel;

	strNode.Format(_T("TEACHING_BLOBS_INSP_TYPE"));
	strKey.Format(_T("INSP_TYPE_CNT"));
	cDataIf.SetInt(strNode, strKey, (int)m_vInspPara.size());

	for (int i = 0; i < m_vInspPara.size(); i++)
	{
		m_InspPara = m_vInspPara[i];
		strNode.Format(_T("TEACHING_BLOBS_PARAMETER_%d"), i);

		strKey.Format(_T("X_RAY_KV"));
		cDataIf.SetDouble(strNode, strKey, m_InspPara.dVoltage);
		strKey.Format(_T("X_RAY_MA"));
		cDataIf.SetDouble(strNode, strKey, m_InspPara.dCurrent);

		strKey.Format(_T("SEARCH_AREA_USE"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.stSearchAreaPara.bAutoSearchArea);
		strKey.Format(_T("SEARCH_AREA_THRESH_HOLD"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.stSearchAreaPara.nThreshHold);
		strKey.Format(_T("SEARCH_AREA_SHORT_LENGHT"));
		cDataIf.SetDouble(strNode, strKey, m_InspPara.stSearchAreaPara.dShortLenght);
		strKey.Format(_T("SEARCH_AREA_LONG_LENGHT"));
		cDataIf.SetDouble(strNode, strKey, m_InspPara.stSearchAreaPara.dLongLenght);
		strKey.Format(_T("SEARCH_AREA_AREA_VAL"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.stSearchAreaPara.nAreaVal);
		strKey.Format(_T("SEARCH_AREA_BLOB_OPEN_CLOSE"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.stSearchAreaPara.nBlobOpenClose);

		strKey.Format(_T("PRE_PROCESS_USE"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.stPreProcess.bPreProcess);
		strKey.Format(_T("PRE_PROCESS_RANK"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.stPreProcess.nRank);
		strKey.Format(_T("PRE_PROCESS_GAIN"));
		cDataIf.SetDouble(strNode, strKey, m_InspPara.stPreProcess.fGain);
		strKey.Format(_T("PRE_PROCESS_OFFSET"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.stPreProcess.nOffset);
		strKey.Format(_T("PRE_PROCESS_ILLUMINATE_WIDTH_HEIGHT"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.stPreProcess.nIlluminateWH);
		strKey.Format(_T("PRE_PROCESS_ILLUMINATE_FACTOR"));
		cDataIf.SetDouble(strNode, strKey, m_InspPara.stPreProcess.dIlluminateFactor);


		strKey.Format(_T("BLOB_MIN_THRESH_HOLD"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.nBlobMinThreshHold);
		strKey.Format(_T("BLOB_MAX_THRESH_HOLD"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.nBlobMaxThreshHold);

		strKey.Format(_T("BLOB_MIN_AREA"));
		cDataIf.SetDouble(strNode, strKey, m_InspPara.dBlobMinArea);
		strKey.Format(_T("BLOB_MAX_AREA"));
		cDataIf.SetDouble(strNode, strKey, m_InspPara.dBlobMaxArea);
		strKey.Format(_T("BLOB_MIN_LENGHT"));
		cDataIf.SetDouble(strNode, strKey, m_InspPara.dBlobMinLenght);
		strKey.Format(_T("BLOB_MAX_LENGHT"));
		cDataIf.SetDouble(strNode, strKey, m_InspPara.dBlobMaxLenght);
		strKey.Format(_T("BLOB_NEARBY_VAL"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.nBlobNearbyVal);



		strKey.Format(_T("MASK_USE"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.stMaskingPara.isMasking);
		strKey.Format(_T("MASK_SEARCH_AREA_CENTER_X"));
		cDataIf.SetDouble(strNode, strKey, m_InspPara.stMaskingPara.dCenX);
		strKey.Format(_T("MASK_SEARCH_AREA_CENTER_Y"));
		cDataIf.SetDouble(strNode, strKey, m_InspPara.stMaskingPara.dCenY);



		strKey.Format(_T("SCALE_IMAGE_USE"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.isScale);
		strKey.Format(_T("SCALE_MIN_THRESH_HOLD"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.nScaleMin);
		strKey.Format(_T("SCALE_MAX_THRESH_HOLD"));
		cDataIf.SetInt(strNode, strKey, m_InspPara.nScaleMax);
	}

	return TRUE;
}


BOOL CDialogTeachingBlobs::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_dlgXRayStatus = new CDialogXRayStatus();
	m_dlgXRayStatus->Create(IDD_DIALOG_X_RAY_STATUS, this);
	/*m_DlgRect = new CDialogRect();
	m_DlgRect->Create(IDD_DIALOG_RECT, this);
	m_DlgRect->SetDlgIdx(0);*/
	LoadTeachingPara();

	m_dlgMasking = new CDialogBlobsMasking();
	m_dlgMasking->Create(IDD_DIALOG_BLOBS_MASK, this);

	m_slider_Min.SetRange(0, 65535);
	m_slider_Min.SetRangeMin(0);
	m_slider_Min.SetRangeMax(65535);
	m_slider_Min.SetPos(0);
	m_slider_Min.SetTicFreq(10);
	m_slider_Min.SetLineSize(1);

	m_slider_Max.SetRange(0, 65535);
	m_slider_Max.SetRangeMin(0);
	m_slider_Max.SetRangeMax(65535);
	m_slider_Max.SetPos(65535);
	m_slider_Max.SetTicFreq(10);
	m_slider_Max.SetLineSize(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDialogTeachingBlobs::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F5)
			OnBnClickedButtonImgLoad();
		else if (pMsg->wParam == VK_F6)
			OnBnClickedButtonImgSave();
		else if (pMsg->wParam == VK_F7)
			OnBnClickedButtonImgGrab();
		else if (pMsg->wParam == VK_F8)
			OnBnClickedButtonDisplayOriginalImage();
		else if (pMsg->wParam == VK_F9)
			OnBnClickedButtonSave();
		else if (pMsg->wParam == VK_F12)
			OnBnClickedButtonClose();
		else if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	CRect main_rect, temp_rect;
	CPoint point, pPoint, pPanPoint;
	point = pMsg->pt;
	pPanPoint = pMsg->pt;

	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetWindowRect(&temp_rect);
	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetClientRect(&main_rect);
	point.x -= temp_rect.left;
	point.y -= temp_rect.top;
	
	if (m_MilDisplay != NULL && this->IsWindowEnabled())//)
	{
		if (main_rect.left <= point.x && point.x <= main_rect.right &&
			main_rect.top <= point.y && point.y <= main_rect.bottom)
		{
			double dRet = m_MilDisplay->GetZoomFactor().dblCX;


			pPoint.x = (int)((double)point.x / dRet) + m_DispPanImg.left;// m_dRectRet[0]);
			pPoint.y = (int)((double)point.y / dRet) + m_DispPanImg.top;// m_dRectRet[0]);


			if (pPoint.x >= m_ImageSize.cx)
				pPoint.x = m_ImageSize.cx - 1;

			if (pPoint.y >= m_ImageSize.cy)
				pPoint.y = m_ImageSize.cy - 1;

#ifdef MIL_USE
			int nVal = 0;
			if(m_MilResultDispBuff == NULL)
				return CDialogEx::PreTranslateMessage(pMsg);

			m_MilResultDispBuff->GetPixelGray(pPoint.x, pPoint.y, &nVal);

			CString strText;
			CBtnenhctrl* pBtn;
			pBtn = (CBtnenhctrl*)(GetDlgItem(IDC_BTNENHCTRL_POINT_X_VAL));
			strText.Format(_T("%d"), pPoint.x);
			pBtn->SetWindowText(strText);
			pBtn = (CBtnenhctrl*)(GetDlgItem(IDC_BTNENHCTRL_POINT_Y_VAL));
			strText.Format(_T("%d"), pPoint.y);
			pBtn->SetWindowText(strText);
			pBtn = (CBtnenhctrl*)(GetDlgItem(IDC_BTNENHCTRL_POINT_GRAY_VAL));
			strText.Format(_T("%d"), nVal);
			pBtn->SetWindowText(strText);

			m_pImagePoint = pPoint;
#endif

			if (pMsg->message == WM_LBUTTONDOWN)
			{
				m_PanPoint = pPoint;
				m_bPanDisp = TRUE;
			}

			if (pMsg->message == WM_LBUTTONUP)
			{
				m_bPanDisp = FALSE;
			}

			if (pMsg->message == WM_MOUSEMOVE && m_bPanDisp)
			{
				double dPanX, dPanY;
				dPanX = (m_PanPoint.x - pPoint.x);// *dRet;
				dPanY = (m_PanPoint.y - pPoint.y);// *dRet;

				CRect tempRect;
				tempRect = m_DispPanImg;

				m_DispPanImg.left += (LONG)dPanX;
				m_DispPanImg.right += (LONG)dPanX;
				m_DispPanImg.top += (LONG)dPanY;
				m_DispPanImg.bottom += (LONG)dPanY;


				if (m_DispPanImg.left < 0)
				{
					m_DispPanImg.left = 0;
					m_DispPanImg.right = tempRect.Width() - 1;
				}

				if (m_DispPanImg.top < 0)
				{
					m_DispPanImg.top = 0;
					m_DispPanImg.bottom = tempRect.Height() - 1;
				}

				if (m_DispPanImg.right > m_ImageSize.cx)
				{
					m_DispPanImg.left = m_ImageSize.cx - tempRect.Width() - 1;
					m_DispPanImg.right = m_ImageSize.cx - 1;
				}

				if (m_DispPanImg.bottom > m_ImageSize.cy)
				{
					m_DispPanImg.top = m_ImageSize.cy - tempRect.Height() - 1;
					m_DispPanImg.bottom = m_ImageSize.cy - 1;
				}

				m_MilDisplay->SetPanFactor(m_DispPanImg.left, m_DispPanImg.top);
			}
		}
		else
		{
			m_bPanDisp = FALSE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogTeachingBlobs::OnBnClickedButtonImgLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE, _T("*.tiff"), _T("*.tiff"), OFN_HIDEREADONLY, _T("tiff(*.tiff)"));

	if (dlg.DoModal() == IDOK)
	{
		OnBnClickedButtonDisplayOriginalImage();
#ifdef MIL_USE
		//m_MilResultDispBuff->BufImport(dlg.GetPathName());
		//m_MilDispBuff->Copy(m_MilResultDispBuff);
		m_Mil16bitBuff->BufImport(dlg.GetPathName(), M_TIFF);


		CClientDC dc(this);
		CRect rect, rect1;
		GetDlgItem(IDC_STATIC_ORG_HISTO)->GetClientRect(&rect);
		GetDlgItem(IDC_STATIC_ORG_HISTO)->GetWindowRect(&rect1);
		
		MIL_INT64* milHistoVal;
		milHistoVal = new MIL_INT64[65536];
		MilHistogram(m_Mil16bitBuff, 65536, milHistoVal);
		unsigned int* pHisto;
		pHisto = new unsigned int[65536];
		for (int i = 0; i < 65536; i++)
		{
			pHisto[i] = (UINT)milHistoVal[i];
		}

		
		DrawGraph(&dc, rect, 756, 335, 65536, milHistoVal);

		BYTE* byData;
		byData = new BYTE[m_ImageSize.cx * m_ImageSize.cy];
		memset(byData, NULL, sizeof(BYTE) * (m_ImageSize.cx * m_ImageSize.cy));
		CRect img_rect;
		img_rect.left = 0;
		img_rect.top = 0;
		img_rect.right = m_ImageSize.cx;
		img_rect.bottom = m_ImageSize.cy;

		CMilBuffer* milBuffer;
		milBuffer = CRepository::Instance()->m_MilLib.NewImage();
		milBuffer->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));

		WORD* pData1, * pData2;
		pData1 = new WORD[m_ImageSize.cx * m_ImageSize.cy];
		pData2 = new WORD[m_ImageSize.cx * m_ImageSize.cy];
		m_Mil16bitBuff->Get2D(pData1, img_rect);

		//20221220 ngh
		/*int nTapCnt = 8;
		int nTapIndex = 1;
		int nTapWidth = m_ImageSize.cy / nTapCnt;
		for (int h = 0; h < m_ImageSize.cy * INSP_RET; h++)
		{
			for (int w = 0; w < m_ImageSize.cx * INSP_RET; w++)
			{
				if ((nTapWidth - 10) <= h)
				{
					if (((nTapWidth * nTapIndex) - 10) < h && h < ((nTapWidth * nTapIndex) + 10))
					{
						if (pData1[(h * m_ImageSize.cx) + w] < (65535 / 3))
						{
							WORD wOffsetData = pData1[(h * m_ImageSize.cx) + w] / 15;
							pData1[(h * m_ImageSize.cx) + w] -= wOffsetData;
						}
					}

					if (h > (nTapWidth * nTapIndex) + 10)
					{
						nTapIndex++;
						if (nTapIndex == 8)
							break;
					}
				}
			}
		}*/


		if (m_InspPara.isScale)
		{
			UpdateData(TRUE);
			int nMin, nMax;
			nMin = GetDlgItemInt(IDC_BTNENHCTRL_MIN_GRAY_HISTO);
			nMax = GetDlgItemInt(IDC_BTNENHCTRL_MAX_GRAY_HISTO);

			ScaleImage(pData1, pData2, m_ImageSize, nMin, nMax, 16);
		}
		else
			m_Mil16bitBuff->Get2D(pData2, img_rect);
		
		for (int y = 0; y < m_ImageSize.cy; y++)
		{
			for (int x = 0; x < m_ImageSize.cx; x++)
			{
				byData[(y * m_ImageSize.cx) + x] = pData2[(y * m_ImageSize.cx) + x] / 256;
			}
		}

		milBuffer->Put2D(CSize(0, 0), m_ImageSize, byData);

		delete[] pData1;
		delete[] pData2;


		m_MilResultDispBuff->Copy(milBuffer);

		milBuffer->Close();

		GetDlgItem(IDC_STATIC_ORG_HISTO)->GetClientRect(&rect);
		GetDlgItem(IDC_STATIC_ORG_HISTO)->GetWindowRect(&rect1);
		delete[] milHistoVal;
		milHistoVal = new MIL_INT64[256];

		MilHistogram(m_MilResultDispBuff, 256, milHistoVal);
		delete[] pHisto;
		pHisto = new unsigned int[256];
		
		for (int i = 0; i < 256; i++)
		{
			pHisto[i] = (UINT)milHistoVal[i];
		}

		DrawGraph(&dc, rect, 756, 442, 256, milHistoVal);

		delete[] byData;
		delete[] pHisto;
		delete[] milHistoVal;
#endif
	}
}


void CDialogTeachingBlobs::OnBnClickedButtonImgSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(FALSE, _T("*.bmp"), _T("*.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("bmp(*.bmp)"));

	if (dlg.DoModal() == IDOK)
	{
#ifdef MIL_USE
		m_MilDispBuff->Export(dlg.GetPathName());
#endif
	}
}


void CDialogTeachingBlobs::OnBnClickedButtonImgGrab()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_bGrabStart)
	{
		m_bGrabStart = TRUE;
#ifdef INVERTER_COM
		CInspectionDlg* pdlg = (CInspectionDlg*)AfxGetApp()->m_pMainWnd;
		SYSTEM_PARA sys;
		sys = pdlg->GetSystemPara();
		CInverterClass::Instance()->SetInverterMove(sys.stInverter.nDirection, sys.stInverter.dSpeed, TRUE);
#else
		CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_MOTOR_CCW, TRUE);
#endif
		//Sleep(1000);
		CAtDetector::Instance()->GrabStart();
	}
	else
	{
		m_bGrabStart = FALSE;
#ifdef INVERTER_COM
		CInverterClass::Instance()->SetInverterMove();
#else
		//inverter stop
		CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_MOTOR_CCW, FALSE);
#endif
	}
}


void CDialogTeachingBlobs::OnBnClickedButtonDisplayOriginalImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE* byData;
	byData = new BYTE[m_ImageSize.cx * m_ImageSize.cy];
	CRect img_rect;
	img_rect.left = 0;
	img_rect.top = 0;
	img_rect.right = m_ImageSize.cx;
	img_rect.bottom = m_ImageSize.cy;

	WORD* pData1;
	pData1 = new WORD[m_ImageSize.cx * m_ImageSize.cy];
	m_Mil16bitBuff->Get2D(pData1, img_rect);

	for (int y = 0; y < m_ImageSize.cy; y++)
	{
		for (int x = 0; x < m_ImageSize.cx; x++)
		{
			byData[(y * m_ImageSize.cx) + x] = pData1[(y * m_ImageSize.cx) + x] / 256;
		}
	}

	m_MilDispBuff->Put2D(CSize(0, 0), m_ImageSize, byData);

	delete[] pData1;
	delete[] byData;

	m_MilResultDispBuff->Copy(m_MilDispBuff);
	m_MilDisplay->ClearOverlay();
}


void CDialogTeachingBlobs::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdatePara(TRUE);
	SaveTeachingPara();
}


void CDialogTeachingBlobs::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->ShowWindow(SW_HIDE);
}

BEGIN_EVENTSINK_MAP(CDialogTeachingBlobs, CDialogEx)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_1, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_2, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_3, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_4, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_5, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_7, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_8, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowDouble, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_9, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_10, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_11, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_12, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_13, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_14, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_15, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_16, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_17, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowDouble, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_0, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterCheck, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_6, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterCheck, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_18, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterCheck, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_MIN_GRAY_HISTO, DISPID_CLICK, CDialogTeachingBlobs::ClickBtnenhctrlMinGrayHisto, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_MAX_GRAY_HISTO, DISPID_CLICK, CDialogTeachingBlobs::ClickBtnenhctrlMaxGrayHisto, VTS_NONE)
	ON_EVENT(CDialogTeachingBlobs, IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_19, DISPID_CLICK, CDialogTeachingBlobs::SelectParameterNumDlgShowInt, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogTeachingBlobs::SelectParameterNumDlgShowInt()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetFocus();
	CGetNumDlg dlg;
	CString strText;
	pBtnEnh->GetWindowTextW(strText);
	int nVal = _wtoi(strText);

	dlg.SetstrNum(nVal);

	if (dlg.DoModal() != IDOK) return;

	strText = dlg.GetstrNum();

	int nMin = 0;
	if (pBtnEnh->GetDlgCtrlID() == IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_9)
		nMin = -999;

	int nMax = 9999;
	if (pBtnEnh->GetDlgCtrlID() == IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_19)
	{
		nMin = 1;
		nMax = 5;
	}

	if (_wtoi(strText) < nMin)
		return;

	if (_wtoi(strText) > nMax)
		return;

	pBtnEnh->SetWindowTextW(strText);

	if (pBtnEnh->GetDlgCtrlID() == IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_19)
	{
		vector< BLOB_INSPEC_PARA> vInspPara;
		for (int i = 0; i < m_vInspPara.size(); i++)
		{
			vInspPara.push_back(m_vInspPara[i]);
		}

		int nTypeCnt = _wtoi(strText);

		if (nTypeCnt < m_vInspPara.size())
		{
			m_vInspPara.clear();
			for (int i = 0; i < nTypeCnt; i++)
				m_vInspPara.push_back(vInspPara[i]);
		}
		else if (nTypeCnt > m_vInspPara.size())
		{
			m_vInspPara.push_back(m_vInspPara[m_vInspPara.size() - 1]);
		}
	}

	UpdatePara(TRUE);
}

void CDialogTeachingBlobs::SelectParameterNumDlgShowDouble()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetFocus();
	CGetNumDlg dlg;
	CString strText;
	pBtnEnh->GetWindowTextW(strText);
	double nVal = _wtof(strText);

	dlg.SetstrNum(nVal);

	if (dlg.DoModal() != IDOK) return;

	strText = dlg.GetstrNum();
	if (_wtoi(strText) < 0)
		return;

	pBtnEnh->SetWindowTextW(strText);

	UpdatePara(TRUE);
}


void CDialogTeachingBlobs::SelectParameterCheck()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetFocus();
	int nUse = pBtnEnh->GetValue();

	if (nUse)
		pBtnEnh->SetWindowText(_T("사용"));
	else
		pBtnEnh->SetWindowText(_T("미사용"));
}


void CDialogTeachingBlobs::UpdatePara(BOOL isUpdate)
{
	CString strText;
	int i = 0;

	if (isUpdate)
	{
		m_InspPara.stSearchAreaPara.bAutoSearchArea = m_ctrTeachingPara[i++].GetValue();
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara.stSearchAreaPara.nThreshHold = _wtoi(strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara.stSearchAreaPara.dShortLenght = _wtof(strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara.stSearchAreaPara.dLongLenght = _wtof(strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara.stSearchAreaPara.nAreaVal = _wtoi(strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara.stSearchAreaPara.nBlobOpenClose = _wtoi(strText);

		m_InspPara.stPreProcess.bPreProcess = m_ctrTeachingPara[i++].GetValue();
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara.stPreProcess.nRank = _wtoi(strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara.stPreProcess.fGain = (float)_wtof(strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara.stPreProcess.nOffset = _wtoi(strText);

		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara.nBlobMinThreshHold = _wtoi(strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara.nBlobMaxThreshHold = _wtoi(strText);

		m_ctrTeachingPara[i++].GetWindowText(strText);
		//m_InspPara.dBlobMinArea = _wtof(strText);
		m_InspPara.stPreProcess.nIlluminateWH = _wtoi(strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		//m_InspPara.dBlobMaxArea = _wtof(strText);
		m_InspPara.stPreProcess.dIlluminateFactor = _wtof(strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara.dBlobMinLenght = _wtof(strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara.dBlobMaxLenght = _wtof(strText);

		m_InspPara.dBlobMinArea = m_InspPara.dBlobMinLenght * m_InspPara.dBlobMinLenght;
		m_InspPara.dBlobMaxArea = m_InspPara.dBlobMaxLenght * m_InspPara.dBlobMaxLenght;

		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara.nBlobNearbyVal = _wtoi(strText);

		double dKV, dMA;
		m_dlgXRayStatus->GetKVtoMA(&dKV, &dMA);
		m_InspPara.dVoltage = dKV;
		m_InspPara.dCurrent = dMA;

		m_InspPara.isScale = m_ctrTeachingPara[i++].GetValue();
		m_InspPara.nScaleMin = m_slider_Min.GetPos();
		m_InspPara.nScaleMax = m_slider_Max.GetPos();


		m_vInspPara[m_edit_nInspTypeIndex - 1] = m_InspPara;

		for (int i = 0; i < m_vInspPara.size(); i++)
		{
			m_vInspPara[i].dVoltage = m_InspPara.dVoltage;
			m_vInspPara[i].dCurrent= m_InspPara.dCurrent;
		}
	}
	else
	{
		m_InspPara = m_vInspPara[m_edit_nInspTypeIndex - 1];

		m_ctrTeachingPara[i++].SetValue(m_InspPara.stSearchAreaPara.bAutoSearchArea);
		strText.Format(_T("%d"), m_InspPara.stSearchAreaPara.nThreshHold);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		strText.Format(_T("%0.3f"), m_InspPara.stSearchAreaPara.dShortLenght);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		strText.Format(_T("%0.3f"), m_InspPara.stSearchAreaPara.dLongLenght);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		strText.Format(_T("%d"), m_InspPara.stSearchAreaPara.nAreaVal);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		strText.Format(_T("%d"), m_InspPara.stSearchAreaPara.nBlobOpenClose);
		m_ctrTeachingPara[i++].SetWindowText(strText);

		m_ctrTeachingPara[i++].SetValue(m_InspPara.stPreProcess.bPreProcess);
		strText.Format(_T("%d"), m_InspPara.stPreProcess.nRank);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		strText.Format(_T("%f"), m_InspPara.stPreProcess.fGain);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		strText.Format(_T("%d"), m_InspPara.stPreProcess.nOffset);
		m_ctrTeachingPara[i++].SetWindowText(strText);


		strText.Format(_T("%d"), m_InspPara.nBlobMinThreshHold);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		strText.Format(_T("%d"), m_InspPara.nBlobMaxThreshHold);
		m_ctrTeachingPara[i++].SetWindowText(strText);

		//strText.Format(_T("%0.3f"), m_InspPara.dBlobMinArea);
		strText.Format(_T("%d"), m_InspPara.stPreProcess.nIlluminateWH);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		//strText.Format(_T("%0.3f"), m_InspPara.dBlobMaxArea);
		strText.Format(_T("%0.3f"), m_InspPara.stPreProcess.dIlluminateFactor);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		strText.Format(_T("%0.3f"), m_InspPara.dBlobMinLenght);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		strText.Format(_T("%0.3f"), m_InspPara.dBlobMaxLenght);
		m_ctrTeachingPara[i++].SetWindowText(strText);

		m_InspPara.dBlobMinArea = m_InspPara.dBlobMinLenght * m_InspPara.dBlobMinLenght;
		m_InspPara.dBlobMaxArea = m_InspPara.dBlobMaxLenght * m_InspPara.dBlobMaxLenght;

		//strText.Format(_T("%0.3f"), m_InspPara.dBlobMinArea);
		//SetDlgItemText(IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_12, strText);
		//strText.Format(_T("%0.3f"), m_InspPara.dBlobMaxArea);
		//SetDlgItemText(IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_13, strText);

		strText.Format(_T("%d"), m_InspPara.nBlobNearbyVal);
		m_ctrTeachingPara[i++].SetWindowText(strText);

		m_dlgXRayStatus->SettingKVtoMA(m_InspPara.dVoltage, m_InspPara.dCurrent);

		m_ctrTeachingPara[i++].SetValue(m_InspPara.isScale);
		m_slider_Min.SetPos(m_InspPara.nScaleMin);
		m_slider_Max.SetPos(m_InspPara.nScaleMax);
	}
}

void CDialogTeachingBlobs::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		SetTimer(0, 100, NULL);
	}
	else
	{
		if (m_dlgMasking->IsWindowVisible())
			m_dlgMasking->ShowWindow(SW_HIDE);

		CAtDetector::Instance()->GrabStop();
		m_dlgXRayStatus->ShowWindow(bShow);
		//m_DlgRect->ShowWindow(SW_HIDE);
		if (m_MilDisplay != NULL)
		{
			m_MilDisplay->EnableGraph(FALSE);
			m_MilDisplay->EnableOverlay(FALSE);
			m_MilDisplay->Close();
			m_MilDisplay = NULL;
		}
		if (m_MilDispBuff != NULL)
		{
			m_MilDispBuff->Close();
			m_MilDispBuff = NULL;
		}
		if (m_MilResultDispBuff != NULL)
		{
			m_MilResultDispBuff->Close();
			m_MilResultDispBuff = NULL;
		}

		if (m_Mil16bitBuff != NULL)
		{
			m_Mil16bitBuff->Close();
			m_Mil16bitBuff = NULL;
		}

		if (m_InspectionClass != NULL)
		{
			delete m_InspectionClass;
			m_InspectionClass = NULL;
		}
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogTeachingBlobs::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch ((int)nIDEvent)
	{
	case 0:
		if (this->IsWindowVisible())
		{
			KillTimer(0);
			m_dlgXRayStatus->ShowWindow(SW_SHOW);
			//m_DlgRect->ShowWindow(SW_SHOW);
			m_edit_nInspTypeIndex = 1;
			UpdateData(FALSE);
			LoadTeachingPara();
			UpdatePara(FALSE);


			SetDlgItemInt(IDC_BTNENHCTRL_MIN_GRAY_HISTO, m_InspPara.nScaleMin);
			SetDlgItemInt(IDC_BTNENHCTRL_MAX_GRAY_HISTO, m_InspPara.nScaleMax);


			m_InspectionClass = new CInspectionClass();
			m_InspectionClass->InitBlobsImage(m_ImageSize.cx, m_ImageSize.cy);

			//m_DlgRect->SetDlgIdx(0);
			//m_DlgRect->SetMainMessage(this->m_hWnd, USE_MSG_IMAGE_VIEW_TO_TEACHING_BLOBS_DIALOG);
			CRect rect_temp;
			GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetWindowRect(&m_DispRect[0]);
			double dRet, dRet2;
			dRet = (double)m_DispRect[0].Width() / m_ImageSize.cx;
			dRet2 = (double)m_DispRect[0].Height() / m_ImageSize.cy;
			GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetClientRect(&rect_temp);
			if (dRet < dRet2)
			{
				m_DispRect[0].bottom = (long)(dRet * (double)m_ImageSize.cy + (double)m_DispRect[0].top);
				rect_temp.bottom = (long)(dRet * (double)m_ImageSize.cy + (double)rect_temp.top);
			}

			m_dRectRet[0] = m_ImageSize.cx / (double)m_DispRect[0].Width();
			m_dRectRet[1] = m_ImageSize.cy / (double)m_DispRect[0].Height();

			//m_DlgRect->SetZoom(dRet, m_ImageSize.cx, m_ImageSize.cy);
			//m_DlgRect->SetEnable(0, TRUE);

			//m_DlgRect->MoveWindow(m_DispRect[0]);
			//m_DlgRect->SetActiveImageRect(TRUE);
			//m_DlgRect->DeleteAll();
			//m_DlgRect->SetImageSize(m_ImageSize.cx, m_ImageSize.cy);
			//m_DlgRect->Invalidate();
			m_dlgXRayStatus->SettingKVtoMA(m_InspPara.dVoltage, m_InspPara.dCurrent);

			m_MilDisplay = CRepository::Instance()->m_MilLib.NewDisplay();
			m_MilDispBuff = CRepository::Instance()->m_MilLib.NewImage();
			m_MilResultDispBuff = CRepository::Instance()->m_MilLib.NewImage();

			m_Mil16bitBuff = CRepository::Instance()->m_MilLib.NewImage(16);

			m_MilDispBuff->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
			m_MilDisplay->SetZoomFactor(dRet, dRet);
			m_DispPanImg.SetRect(0, 0, m_ImageSize.cx - 1, m_ImageSize.cy - 1);


			m_Mil16bitBuff->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));

			m_MilResultDispBuff->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
			//m_MilResultDispBuff->SelectWindow(m_MilDisplay, m_DlgRect->m_hWnd);
			m_MilResultDispBuff->SelectWindow(m_MilDisplay, GetDlgItem(IDC_STATIC_TEACHING_DISP)->m_hWnd);

			m_MilDisplay->EnableOverlay();
			m_MilDisplay->EnableGraph();
			m_MilDisplay->SetColor(M_RGB888(0, 0, 255));

			CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_0);
			int nUse = pBtnEnh->GetValue();

			if (nUse)
				pBtnEnh->SetWindowText(_T("사용"));
			else
				pBtnEnh->SetWindowText(_T("미사용"));

			pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_6);
			nUse = pBtnEnh->GetValue();

			if (nUse)
				pBtnEnh->SetWindowText(_T("사용"));
			else
				pBtnEnh->SetWindowText(_T("미사용"));

			pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_18);
			nUse = pBtnEnh->GetValue();

			if (nUse)
				pBtnEnh->SetWindowText(_T("사용"));
			else
				pBtnEnh->SetWindowText(_T("미사용"));


			m_slider_Min.SetPos(m_InspPara.nScaleMin);
			m_slider_Max.SetPos(m_InspPara.nScaleMax);

			CInspectionDlg* pdlg = (CInspectionDlg*)AfxGetApp()->m_pMainWnd;
			SYSTEM_PARA sys;
			sys = pdlg->GetSystemPara();
			pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_17);
			CString strText;
			strText.Format(_T("%0.2f"), sys.stDetector.dScanSpeed);
			pBtnEnh->SetWindowText(strText);

			strText.Format(_T("%d"), m_vInspPara.size());
			GetDlgItem(IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_19)->SetWindowText(strText);
		}
		break;
	case 1:
		if (m_InspectionClass->GetBlobsInspectionEnd())
		{
			KillTimer(1);
			vector<BLOB_RESULT_PARA> stBlobsResult;
			m_MilDisplay->ClearOverlay();
			m_MilDisplay->EnableGraph(FALSE);
			m_MilDisplay->EnableOverlay(FALSE);
			m_MilDisplay->EnableGraph();
			m_MilDisplay->EnableOverlay();
			MaskingImageView();
			m_MilDisplay->FontSize(M_FONT_DEFAULT_LARGE);
			m_InspectionClass->SetBlobsResultDisplay(m_MilDisplay, stBlobsResult);
		}
		break;
	case 100:
		if (!m_dlgMasking->IsWindowVisible())
		{
			KillTimer(100);
			m_InspPara.stMaskingPara = m_dlgMasking->GetMaskingPara();

		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDialogTeachingBlobs::MaskingImageView()
{
	if (m_InspPara.stMaskingPara.isMasking)
	{
		CMilBuffer* milBuffer = CRepository::Instance()->m_MilLib.NewImage();
		milBuffer->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
		m_MilDisplay->ClearOverlay();
		m_InspectionClass->SetPixelSize(m_dPixelSizeX, m_dPixelSizeY);
		m_InspectionClass->SetInspBlobsPara(m_InspPara);
		m_InspectionClass->SetAutoSearchArea(m_MilResultDispBuff, milBuffer);
		double dCenX = 0., dCenY = 0.;
		m_InspectionClass->GetSearchAreaCenterXY(milBuffer, &dCenX, &dCenY);
		if (dCenX > 0. && dCenY > 0.)
		{
			m_MilDisplay->SetColor(M_COLOR_MAGENTA);

			MgraArcFill(m_MilDisplay->GetIDGraph(), m_MilDisplay->GetIDOverlay(), dCenX, dCenY, 3, 3, 0, 360);
		}

		CMilBuffer* milMask, * milMask2;;
		CMilBlob* milBlobs;
		milMask = CRepository::Instance()->m_MilLib.NewImage();
		milMask->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
		milMask2 = CRepository::Instance()->m_MilLib.NewImage();
		milMask2->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
		CString strFilePath;
		wchar_t strTemp[MAX_CHAR_LENG];
		GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
		strFilePath.Format(_T("%s\\Recipe\\%s\\Blobs_Mask.bmp"), strTemp, m_strCurrentRecipeName);
		milMask->BufImport(strFilePath);

		milBlobs = CRepository::Instance()->m_MilLib.NewBlobTool(milMask);
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

		milBlobs->GetResult();
		int nBlobCnt = milBlobs->GetNumber();

		double dMaskCenX = (dCenX - m_InspPara.stMaskingPara.dCenX);
		double dMaskCenY = (dCenY - m_InspPara.stMaskingPara.dCenY);
		milMask2->Clear(M_BLACK);
		MimTranslate(milMask->GetIDBuffer(), milMask2->GetIDBuffer(), dMaskCenX, dMaskCenY, M_DEFAULT);

		milBlobs->Free();
		milBlobs = NULL;

		milBlobs = CRepository::Instance()->m_MilLib.NewBlobTool(milMask2);
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

		milBlobs->GetResult();
		nBlobCnt = milBlobs->GetNumber();

		MblobDraw(m_MilDisplay->GetIDGraph(), milBlobs->m_milResult, m_MilDisplay->GetIDOverlay(), M_DRAW_HOLES_CONTOUR + M_DRAW_CENTER_OF_GRAVITY, M_INCLUDED_BLOBS, M_DEFAULT);

		milBlobs->Free();
		milBlobs = NULL;

		milBuffer->Close();
		milMask->Close();
		milMask2->Close();
		milBuffer = NULL;
		milMask = NULL;
		milMask2 = NULL;
	}
}

void CDialogTeachingBlobs::OnBnClickedButtonBlobsSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnBnClickedButtonDisplayOriginalImage();
	UpdatePara(TRUE);
	m_InspectionClass->SetPixelSize(m_dPixelSizeX, m_dPixelSizeY);
	m_InspectionClass->SetInspBlobsPara(m_InspPara);
	m_InspectionClass->SetRecipeName(m_strCurrentRecipeName);
	m_InspectionClass->SetInspIndex(m_edit_nInspTypeIndex - 1);
	m_InspectionClass->SetStartBlobsThread(m_MilResultDispBuff);
	SetTimer(1, 100, NULL);
}


LRESULT CDialogTeachingBlobs::OnImageViewToTeachingMessage(WPARAM para0, LPARAM para1)
{
	int nEvent = (int)para0;
	int nTrackerIdx = (int)para1;

	switch (nEvent)
	{
	case 0:
	{

	}
	break;
	case 1:
	{
		if (!this->IsWindowVisible())
			break;

#ifdef MIL_USE
		CMilBuffer* milTempBuff, * milTempBuff2;
		milTempBuff = CRepository::Instance()->m_MilLib.NewImage();
		milTempBuff->Alloc2D(CSize(m_ImageSize.cy, m_ImageSize.cx));
		milTempBuff2 = CRepository::Instance()->m_MilLib.NewImage(16);
		milTempBuff2->Alloc2D(CSize(m_ImageSize.cy, m_ImageSize.cx));
		long nPitch;
#ifdef HW_USE
		nPitch = m_ImageSize.cy;
#else
		MbufInquire(milTempBuff->GetIDBuffer(), M_PITCH, &nPitch);
#endif
		BYTE* pData;
		WORD* pData2, * pData4;
		pData2 = DBG_NEW WORD[nPitch * m_ImageSize.cx];
		pData4 = DBG_NEW WORD[nPitch * m_ImageSize.cx];
		memcpy(pData2, (void*)para1, sizeof(WORD) * nPitch * m_ImageSize.cx);
		pData = DBG_NEW BYTE[m_ImageSize.cy * m_ImageSize.cx];
		milTempBuff2->Put2D(CSize(0, 0), CSize(m_ImageSize.cy, m_ImageSize.cx), pData2);

		MimRotate(milTempBuff2->GetIDBuffer(), m_Mil16bitBuff->GetIDBuffer(), 90, m_ImageSize.cy / 2, m_ImageSize.cx / 2, m_ImageSize.cx / 2, m_ImageSize.cy / 2, M_DEFAULT + M_OVERSCAN_DISABLE);
		milTempBuff2->Close();

		if (m_InspPara.isScale)
		{
			ScaleImage((void*)pData2, (void*)pData4, m_ImageSize, m_InspPara.nScaleMin, m_InspPara.nScaleMax, 16);
		}
		else
			memcpy(pData4, pData2, sizeof(WORD) * (nPitch * m_ImageSize.cx));

		for (int y = 0; y < m_ImageSize.cx; y++)
		{
			for (int x = 0; x < m_ImageSize.cy; x++)
			{
				pData[(y * m_ImageSize.cy) + x] = pData4[(y * m_ImageSize.cy) + x] / 256;
			}
		}

		milTempBuff->Put(pData);

		MimRotate(milTempBuff->GetIDBuffer(), m_MilResultDispBuff->GetIDBuffer(), 90, m_ImageSize.cy / 2, m_ImageSize.cx / 2, m_ImageSize.cx / 2, m_ImageSize.cy / 2, M_DEFAULT + M_OVERSCAN_DISABLE);
		milTempBuff->Close();

		delete[] pData4;
		delete[] pData2;
		delete[] pData;
		m_MilDispBuff->Copy(m_MilResultDispBuff);

//		Sleep(100);

#endif
	}
	break;
	}

	return 0;
}

void CDialogTeachingBlobs::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);
	CRect rect;
	GetDlgItem(IDC_STATIC_X_RAY_STATUS)->GetWindowRect(&rect);
	m_dlgXRayStatus->MoveWindow(rect);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogTeachingBlobs::OnBnClickedButtonBlobsPreProcess()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnBnClickedButtonDisplayOriginalImage();
	UpdatePara(TRUE);
	m_InspectionClass->SetInspBlobsPara(m_InspPara);
	m_InspectionClass->SetInspIndex(m_edit_nInspTypeIndex - 1);
	for (int i = 0; i < PRE_PROCESS_CNT; i++)
	{
		m_InspectionClass->SetPreProcess(m_MilResultDispBuff, m_MilResultDispBuff);
	}
}


void CDialogTeachingBlobs::OnBnClickedButtonSetScanSpeed()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CInspectionDlg* pdlg = (CInspectionDlg*)AfxGetApp()->m_pMainWnd;
	SYSTEM_PARA sys;
	sys = pdlg->GetSystemPara();
	CString strText;
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_BLOBS_PARA_17)->GetWindowText(strText);
	double dScanSpeed = _wtof(strText);
	sys.stDetector.dScanSpeed = dScanSpeed;
	sys.stInverter.dSpeed = dScanSpeed;
	CAtDetector::Instance()->SetScanSpeed(dScanSpeed);
	pdlg->SetSystemPara(sys);
	pdlg->SetSystemParaSave();
}


void CDialogTeachingBlobs::OnBnClickedButtonBlobsMasking()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_dlgMasking == NULL)
		return;

	if (!m_dlgMasking->IsWindowVisible())
	{
		//AutoSearchArea
		CMilBuffer* milBuffer = CRepository::Instance()->m_MilLib.NewImage();
		milBuffer->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));

		double dCenX = 0., dCenY = 0.;
		m_InspectionClass->SetInspBlobsPara(m_InspPara);
		m_InspectionClass->SetAutoSearchArea(m_MilResultDispBuff, milBuffer);
		m_InspectionClass->GetSearchAreaCenterXY(milBuffer, &dCenX, &dCenY);
		m_InspPara.stMaskingPara.dCenX = dCenX;
		m_InspPara.stMaskingPara.dCenY = dCenY;
		m_dlgMasking->SetSearchAreaCenXY(dCenX, dCenY);
		m_dlgMasking->SetRecipeName(m_strCurrentRecipeName);
		m_dlgMasking->SetDispImage(m_MilResultDispBuff);
		m_dlgMasking->SetMaskingPara(m_InspPara.stMaskingPara);
		m_dlgMasking->ShowWindow(SW_SHOW);
		SetTimer(100, 100, NULL);
		milBuffer->Close();
	}
}


void CDialogTeachingBlobs::OnBnClickedButtonBlobsSearchAreaStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMilBuffer* milBuffer = CRepository::Instance()->m_MilLib.NewImage();
	milBuffer->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
	m_MilDisplay->ClearOverlay();
	m_InspectionClass->SetPixelSize(m_dPixelSizeX, m_dPixelSizeY);
	m_InspectionClass->SetInspBlobsPara(m_InspPara);
	m_InspectionClass->SetAutoSearchArea(m_MilResultDispBuff, milBuffer, m_MilDisplay);
	double dCenX = 0., dCenY = 0.;
	m_InspectionClass->GetSearchAreaCenterXY(milBuffer, &dCenX, &dCenY);
	if (dCenX > 0. && dCenY > 0.)
	{
		m_MilDisplay->SetColor(M_COLOR_MAGENTA);

		MgraArcFill(m_MilDisplay->GetIDGraph(), m_MilDisplay->GetIDOverlay(), dCenX, dCenY, 3, 3, 0, 360);
	}


	//milBuffer->BufImport(_T("d:\\PreProcess2_0.bmp"), M_BMP);

	//m_InspectionClass->MBinarizeAdaptive(milBuffer, m_MilResultDispBuff, 10, 161);

	for (int i = 0; i < m_vInspPara.size(); i++)
	{
		m_vInspPara[i].stMaskingPara.dCenX = dCenX;
		m_vInspPara[i].stMaskingPara.dCenY = dCenY;
	}

	milBuffer->Close();
}


void CDialogTeachingBlobs::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar)
	{
		int nMin, nMax;
		CString strText;
		GetDlgItemText(IDC_BTNENHCTRL_MIN_GRAY_HISTO, strText);
		nMin = _wtoi(strText);
		GetDlgItemText(IDC_BTNENHCTRL_MAX_GRAY_HISTO, strText);
		nMax = _wtoi(strText);
		if (pScrollBar == (CScrollBar*)&m_slider_Min)
		{
			nMin = m_slider_Min.GetPos();
			if (nMax <= nMin)
			{
				m_slider_Min.SetPos(nMax - 1);
				nMin = nMax - 1;
			}
		}
		else if (pScrollBar == (CScrollBar*)&m_slider_Max)
		{
			nMax = m_slider_Max.GetPos();
			if (nMax <= nMin)
			{
				m_slider_Max.SetPos(nMin + 1);
				nMax = nMin + 1;
			}
		}

		m_slider_Min.SetSelection(nMin, nMax);
		m_slider_Max.SetSelection(nMin, nMax);

		SetDlgItemInt(IDC_BTNENHCTRL_MIN_GRAY_HISTO, nMin);
		SetDlgItemInt(IDC_BTNENHCTRL_MAX_GRAY_HISTO, nMax);

		UpdateData(FALSE);

		for (int i = 0; i < m_vInspPara.size(); i++)
		{
			m_vInspPara[i].nScaleMin = nMin;
			m_vInspPara[i].nScaleMax = nMax;
		}

		CClientDC dc(this);
		CRect rect, rect1;
		MIL_INT64* milHistoVal;
		milHistoVal = new MIL_INT64[256];

		BYTE* byData;
		byData = new BYTE[m_ImageSize.cx * m_ImageSize.cy];
		CRect img_rect;
		img_rect.left = 0;
		img_rect.top = 0;
		img_rect.right = m_ImageSize.cx;
		img_rect.bottom = m_ImageSize.cy;

		CMilBuffer* milBuffer;
		milBuffer = CRepository::Instance()->m_MilLib.NewImage();
		milBuffer->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));

		WORD* pData1, * pData2;
		pData1 = new WORD[m_ImageSize.cx * m_ImageSize.cy];
		pData2 = new WORD[m_ImageSize.cx * m_ImageSize.cy];
		m_Mil16bitBuff->Get2D(pData1, img_rect);

		if (m_InspPara.isScale)
		{
			UpdateData(TRUE);
			int nMin, nMax;
			nMin = GetDlgItemInt(IDC_BTNENHCTRL_MIN_GRAY_HISTO);
			nMax = GetDlgItemInt(IDC_BTNENHCTRL_MAX_GRAY_HISTO);

			ScaleImage(pData1, pData2, m_ImageSize, nMin, nMax, 16);
		}
		else
			memcpy(pData2, pData1, sizeof(WORD) * (m_ImageSize.cx * m_ImageSize.cy));


		for (int y = 0; y < m_ImageSize.cy; y++)
		{
			for (int x = 0; x < m_ImageSize.cx; x++)
			{
				byData[(y * m_ImageSize.cx) + x] = pData2[(y * m_ImageSize.cx) + x] / 256;
			}
		}

		milBuffer->Put2D(CSize(0, 0), m_ImageSize, byData);

		delete[] pData1;
		delete[] pData2;


		m_MilResultDispBuff->Copy(milBuffer);

		milBuffer->Close();

		GetDlgItem(IDC_STATIC_ORG_HISTO)->GetClientRect(&rect);
		GetDlgItem(IDC_STATIC_ORG_HISTO)->GetWindowRect(&rect1);

		MilHistogram(m_MilResultDispBuff, 256, milHistoVal);
		unsigned int* pHisto;
		pHisto = new unsigned int[256];
		for (int i = 0; i < 256; i++)
		{
			pHisto[i] = (UINT)milHistoVal[i];
		}

		DrawGraph(&dc, rect, 756, 442, 256, milHistoVal);

		delete[] byData;
		delete[] pHisto;
		delete[] milHistoVal;

	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}




void CDialogTeachingBlobs::ClickBtnenhctrlMinGrayHisto()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetFocus();
	CGetNumDlg dlg;
	CString strText;
	pBtnEnh->GetWindowTextW(strText);
	int nVal = _wtoi(strText);

	dlg.SetstrNum(nVal);

	if (dlg.DoModal() != IDOK) return;

	strText = dlg.GetstrNum();

	int nMin, nMax;
	nMin = _wtoi(strText);
	nMax = GetDlgItemInt(IDC_BTNENHCTRL_MAX_GRAY_HISTO);
	if (nMin < 0)
		nMin = 0;

	if (nMin > nMax)
		nMin = nMax - 1;

	strText.Format(_T("%d"), nMin);
	m_slider_Min.SetPos(nMin);
	pBtnEnh->SetWindowTextW(strText);

	CClientDC dc(this);
	CRect rect, rect1;
	GetDlgItem(IDC_STATIC_ORG_HISTO)->GetClientRect(&rect);
	GetDlgItem(IDC_STATIC_ORG_HISTO)->GetWindowRect(&rect1);

	if (m_InspPara.isScale)
	{
		CClientDC dc(this);
		CRect rect, rect1;
		MIL_INT64* milHistoVal;
		milHistoVal = new MIL_INT64[256];

		BYTE* byData;
		byData = new BYTE[m_ImageSize.cx * m_ImageSize.cy];
		CRect img_rect;
		img_rect.left = 0;
		img_rect.top = 0;
		img_rect.right = m_ImageSize.cx;
		img_rect.bottom = m_ImageSize.cy;

		CMilBuffer* milBuffer;
		milBuffer = CRepository::Instance()->m_MilLib.NewImage();
		milBuffer->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));

		WORD* pData1, * pData2;
		pData1 = new WORD[m_ImageSize.cx * m_ImageSize.cy];
		pData2 = new WORD[m_ImageSize.cx * m_ImageSize.cy];
		m_Mil16bitBuff->Get2D(pData1, img_rect);

		if (m_InspPara.isScale)
		{
			UpdateData(TRUE);
			int nMin, nMax;
			nMin = GetDlgItemInt(IDC_BTNENHCTRL_MIN_GRAY_HISTO);
			nMax = GetDlgItemInt(IDC_BTNENHCTRL_MAX_GRAY_HISTO);

			ScaleImage(pData1, pData2, m_ImageSize, nMin, nMax, 16);
		}
		else
			memcpy(pData2, pData1, sizeof(WORD) * (m_ImageSize.cx * m_ImageSize.cy));


		for (int y = 0; y < m_ImageSize.cy; y++)
		{
			for (int x = 0; x < m_ImageSize.cx; x++)
			{
				byData[(y * m_ImageSize.cx) + x] = pData2[(y * m_ImageSize.cx) + x] / 256;
			}
		}

		milBuffer->Put2D(CSize(0, 0), m_ImageSize, byData);

		delete[] pData1;
		delete[] pData2;


		m_MilResultDispBuff->Copy(milBuffer);

		milBuffer->Close();

		GetDlgItem(IDC_STATIC_ORG_HISTO)->GetClientRect(&rect);
		GetDlgItem(IDC_STATIC_ORG_HISTO)->GetWindowRect(&rect1);

		MilHistogram(m_MilResultDispBuff, 256, milHistoVal);
		unsigned int* pHisto;
		pHisto = new unsigned int[256];
		for (int i = 0; i < 256; i++)
		{
			pHisto[i] = (UINT)milHistoVal[i];
		}

		DrawGraph(&dc, rect, 756, 442, 256, milHistoVal);

		delete[] byData;
		delete[] pHisto;
		delete[] milHistoVal;
	}
}


void CDialogTeachingBlobs::ClickBtnenhctrlMaxGrayHisto()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetFocus();
	CGetNumDlg dlg;
	CString strText;
	pBtnEnh->GetWindowTextW(strText);
	int nVal = _wtoi(strText);

	dlg.SetstrNum(nVal);

	if (dlg.DoModal() != IDOK) return;

	strText = dlg.GetstrNum();

	int nMin, nMax;
	nMin = GetDlgItemInt(IDC_BTNENHCTRL_MIN_GRAY_HISTO); 
	nMax = _wtoi(strText);
	if (nMax > 65535)
		nMax = 65535;

	if (nMin > nMax)
		nMax = nMin + 1;

	strText.Format(_T("%d"), nMax);
	m_slider_Max.SetPos(nMax);
	pBtnEnh->SetWindowTextW(strText);

	CClientDC dc(this);
	CRect rect, rect1;
	GetDlgItem(IDC_STATIC_ORG_HISTO)->GetClientRect(&rect);
	GetDlgItem(IDC_STATIC_ORG_HISTO)->GetWindowRect(&rect1);

	if (m_InspPara.isScale)
	{
		CClientDC dc(this);
		CRect rect, rect1;
		MIL_INT64* milHistoVal;
		milHistoVal = new MIL_INT64[256];

		BYTE* byData;
		byData = new BYTE[m_ImageSize.cx * m_ImageSize.cy];
		CRect img_rect;
		img_rect.left = 0;
		img_rect.top = 0;
		img_rect.right = m_ImageSize.cx;
		img_rect.bottom = m_ImageSize.cy;

		CMilBuffer* milBuffer;
		milBuffer = CRepository::Instance()->m_MilLib.NewImage();
		milBuffer->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));

		WORD* pData1, * pData2;
		pData1 = new WORD[m_ImageSize.cx * m_ImageSize.cy];
		pData2 = new WORD[m_ImageSize.cx * m_ImageSize.cy];
		m_Mil16bitBuff->Get2D(pData1, img_rect);

		if (m_InspPara.isScale)
		{
			UpdateData(TRUE);
			int nMin, nMax;
			nMin = GetDlgItemInt(IDC_BTNENHCTRL_MIN_GRAY_HISTO);
			nMax = GetDlgItemInt(IDC_BTNENHCTRL_MAX_GRAY_HISTO);

			ScaleImage(pData1, pData2, m_ImageSize, nMin, nMax, 16);
		}
		else
			memcpy(pData2, pData1, sizeof(WORD) * (m_ImageSize.cx * m_ImageSize.cy));


		for (int y = 0; y < m_ImageSize.cy; y++)
		{
			for (int x = 0; x < m_ImageSize.cx; x++)
			{
				byData[(y * m_ImageSize.cx) + x] = pData2[(y * m_ImageSize.cx) + x] / 256;
			}
		}

		milBuffer->Put2D(CSize(0, 0), m_ImageSize, byData);

		delete[] pData1;
		delete[] pData2;


		m_MilResultDispBuff->Copy(milBuffer);

		milBuffer->Close();

		GetDlgItem(IDC_STATIC_ORG_HISTO)->GetClientRect(&rect);
		GetDlgItem(IDC_STATIC_ORG_HISTO)->GetWindowRect(&rect1);

		MilHistogram(m_MilResultDispBuff, 256, milHistoVal);
		unsigned int* pHisto;
		pHisto = new unsigned int[256];
		for (int i = 0; i < 256; i++)
		{
			pHisto[i] = (UINT)milHistoVal[i];
		}

		DrawGraph(&dc, rect, 756, 442, 256, milHistoVal);

		delete[] byData;
		delete[] pHisto;
		delete[] milHistoVal;
	}
}


BOOL CDialogTeachingBlobs::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CdblPoint dbPoint;
	dbPoint = m_MilDisplay->GetZoomFactor();


	CRect rect_temp;
	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetWindowRect(&m_DispRect[0]);
	double dRet, dRet2;
	dRet = (double)m_DispRect[0].Width() / m_ImageSize.cx;
	dRet2 = (double)m_DispRect[0].Height() / m_ImageSize.cy;
	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetClientRect(&rect_temp);
	//m_DispPanImg.SetRect(0, 0, m_ImageSize.cx - 1, m_ImageSize.cy - 1);

	if (dRet < dRet2)
	{
		m_DispRect[0].bottom = (long)(dRet * (double)m_ImageSize.cy + (double)m_DispRect[0].top);
		rect_temp.bottom = (long)(dRet * (double)m_ImageSize.cy + (double)rect_temp.top);
	}

	CRect temp_rect, main_rect;
	CPoint point, pPoint;
	point = pt;
	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetWindowRect(&temp_rect);
	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetClientRect(&main_rect);
	point.x -= temp_rect.left;
	point.y -= temp_rect.top;
	/*if(m_DlgRect == NULL)
		return CDialogEx::PreTranslateMessage(pMsg);*/

	if (/*m_DlgRect->IsWindowVisible() && */(this->IsWindowEnabled()))//|| m_DlgRect->IsWindowEnabled()))
	{
		if (main_rect.left <= point.x && point.x <= main_rect.right &&
			main_rect.top <= point.y && point.y <= main_rect.bottom)
		{
			 m_MilDisplay->GetZoomFactor();
			pPoint.x = (int)((double)point.x / dbPoint.dblPosX) + m_DispPanImg.left;// m_dRectRet[0]);
			pPoint.y = (int)((double)point.y / dbPoint.dblPosY) + m_DispPanImg.top;// m_dRectRet[0]);


			if (pPoint.x >= m_ImageSize.cx)
				pPoint.x = m_ImageSize.cx - 1;

			if (pPoint.y >= m_ImageSize.cy)
				pPoint.y = m_ImageSize.cy - 1;


			m_pImagePoint = pPoint;

			if (zDelta > 0)
			{
				//zoom out
				dbPoint.Offset(-0.1, -0.1);
				if (dRet > dbPoint.dblPosX)
					dbPoint.dblPosX = dRet;

				m_MilDisplay->SetZoomFactor(dbPoint.dblPosX, dbPoint.dblPosX);

				if (dbPoint.dblPosX != dRet)
				{
					double dWidth = m_DispRect[0].Width() / dbPoint.dblPosX;
					double dHeight = m_DispRect[0].Height() / dbPoint.dblPosX;

					

					m_DispPanImg.left = m_pImagePoint.x /*m_DispPanImg.CenterPoint().x*/ - (LONG)(dWidth / 2);
					m_DispPanImg.right = m_pImagePoint.x/*m_DispPanImg.CenterPoint().x*/ + (LONG)(dWidth / 2);
					m_DispPanImg.top = m_pImagePoint.y/*m_DispPanImg.CenterPoint().y*/ - (LONG)(dHeight / 2);
					m_DispPanImg.bottom = m_pImagePoint.y/*m_DispPanImg.CenterPoint().y*/ + (LONG)(dHeight / 2);
					
					CRect tempRect;
					tempRect = m_DispPanImg;

					if (m_DispPanImg.left < 0)
					{
						m_DispPanImg.left = 0;
						m_DispPanImg.right = tempRect.Width() - 1;
					}

					if (m_DispPanImg.top < 0)
					{
						m_DispPanImg.top = 0;
						m_DispPanImg.bottom = tempRect.Height() - 1;
					}

					if (m_DispPanImg.right > m_ImageSize.cx)
					{
						m_DispPanImg.left = m_ImageSize.cx - tempRect.Width() - 1;
						m_DispPanImg.right = m_ImageSize.cx - 1;
					}

					if (m_DispPanImg.bottom > m_ImageSize.cy)
					{
						m_DispPanImg.top = m_ImageSize.cy - tempRect.Height() - 1;
						m_DispPanImg.bottom = m_ImageSize.cy - 1;
					}
				}
				else
				{
					m_DispPanImg.left = 0;
					m_DispPanImg.top = 0;
					m_DispPanImg.right = m_ImageSize.cx - 1;
					m_DispPanImg.bottom = m_ImageSize.cy - 1;
				}

				m_MilDisplay->SetPanFactor(m_DispPanImg.left, m_DispPanImg.top);
			}
			else
			{
				// zoom in
				dbPoint.Offset(0.1, 0.1);
				if (dRet > dbPoint.dblPosX)
					dbPoint.dblPosX = dRet;

				m_MilDisplay->SetZoomFactor(dbPoint.dblPosX, dbPoint.dblPosX);

				double dWidth = m_DispRect[0].Width() / dbPoint.dblPosX;
				double dHeight = m_DispRect[0].Height() / dbPoint.dblPosX;

				m_DispPanImg.left = m_pImagePoint.x /*m_DispPanImg.CenterPoint().x*/ - (LONG)(dWidth / 2);
				m_DispPanImg.right = m_pImagePoint.x/*m_DispPanImg.CenterPoint().x*/ + (LONG)(dWidth / 2);
				m_DispPanImg.top = m_pImagePoint.y/*m_DispPanImg.CenterPoint().y*/ - (LONG)(dHeight / 2);
				m_DispPanImg.bottom = m_pImagePoint.y/*m_DispPanImg.CenterPoint().y*/ + (LONG)(dHeight / 2);


				/*if (m_DispPanImg.left < 0)
				{
					int nVal = m_DispPanImg.Width();
					m_DispPanImg.left = 0;
					m_DispPanImg.right = m_DispPanImg.left + nVal;
				}

				if (m_DispPanImg.top < 0)
				{
					int nVal = m_DispPanImg.Height();
					m_DispPanImg.top = 0;
					m_DispPanImg.bottom = m_DispPanImg.top + nVal;
				}*/

				CRect tempRect;
				tempRect = m_DispPanImg;

				if (m_DispPanImg.left < 0)
				{
					m_DispPanImg.left = 0;
					m_DispPanImg.right = tempRect.Width() - 1;
				}

				if (m_DispPanImg.top < 0)
				{
					m_DispPanImg.top = 0;
					m_DispPanImg.bottom = tempRect.Height() - 1;
				}

				if (m_DispPanImg.right > m_ImageSize.cx)
				{
					m_DispPanImg.left = m_ImageSize.cx - tempRect.Width() - 1;
					m_DispPanImg.right = m_ImageSize.cx - 1;
				}

				if (m_DispPanImg.bottom > m_ImageSize.cy)
				{
					m_DispPanImg.top = m_ImageSize.cy - tempRect.Height() - 1;
					m_DispPanImg.bottom = m_ImageSize.cy - 1;
				}


				m_MilDisplay->SetPanFactor(m_DispPanImg.left, m_DispPanImg.top);

			}
		}
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CDialogTeachingBlobs::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//m_bPanDisp = TRUE;
	//m_PanPoint = point;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDialogTeachingBlobs::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//m_bPanDisp = FALSE;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDialogTeachingBlobs::OnBnClickedButtonInspTypeDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_edit_nInspTypeIndex--;
	if (1 > m_edit_nInspTypeIndex)
		m_edit_nInspTypeIndex = 1;

	UpdatePara(FALSE);
	UpdateData(FALSE);
}


void CDialogTeachingBlobs::OnBnClickedButtonInspTypeUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_edit_nInspTypeIndex++;
	if (m_vInspPara.size() < m_edit_nInspTypeIndex)
		m_edit_nInspTypeIndex = (int)m_vInspPara.size();

	UpdatePara(FALSE);
	UpdateData(FALSE);
}


