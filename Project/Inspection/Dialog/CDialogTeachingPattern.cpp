// CDialogTeachingPattern.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogTeachingPattern.h"
#include "Repository.h"
#include "CIOClass.h"
#include "CInverterClass.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// CDialogTeachingPattern 대화 상자

IMPLEMENT_DYNAMIC(CDialogTeachingPattern, CDialogEx)

CDialogTeachingPattern::CDialogTeachingPattern(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INSPECTION_TEACHING_PATTERN, pParent)
{
	m_bPatternDel = FALSE;
	m_dlgXRayStatus = NULL;
#ifdef MIL_USE
	m_MilDisplay = NULL;
	m_MilDispBuff = NULL;
	m_MilResultDispBuff = NULL;

	m_MilPatternDisplay = NULL;
	m_MilPatternDispBuff = NULL;
	m_MilPatternResultDispBuff = NULL;

	m_MilPatternMaskDisplay = NULL;
	m_MilPatternMaskDispBuff = NULL;

	m_GraphicList = NULL;
#endif
	m_bMouseMove = FALSE;
	m_bPatternAdd = FALSE;
	m_bRBMouseDown = FALSE;
	m_bLBMouseDown = FALSE;

	memset(&m_InspPara, NULL, sizeof(INSPEC_PARA));

	m_isDlgShow = FALSE;

	m_nPtnAddIndex = 0;
	m_bInitDisplay = FALSE;

#ifdef MIL_USE
	for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
		m_InspectionClass[i] = NULL;
#endif
}

CDialogTeachingPattern::~CDialogTeachingPattern()
{
	if (m_dlgXRayStatus != NULL)
	{
		m_dlgXRayStatus->DestroyWindow();
		delete m_dlgXRayStatus;
		m_dlgXRayStatus = NULL;
	}

#ifdef MIL_USE
	if (m_GraphicList != NULL)
	{
		MgraFree(m_GraphicList);
		m_GraphicList = NULL;
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

	if (m_MilDisplay != NULL)
	{
		m_MilDisplay->Close();
		m_MilDisplay = NULL;
	}

	if (m_MilPatternDispBuff != NULL)
	{
		m_MilPatternDispBuff->Close();
		m_MilPatternDispBuff = NULL;
	}

	if (m_MilPatternResultDispBuff != NULL)
	{
		m_MilPatternResultDispBuff->Close();
		m_MilPatternResultDispBuff = NULL;
	}

	if (m_MilPatternDisplay != NULL)
	{
		m_MilPatternDisplay->Close();
		m_MilPatternDisplay = NULL;
	}

	if (m_MilPatternMaskDispBuff != NULL)
	{
		m_MilPatternMaskDispBuff->Close();
		m_MilPatternMaskDispBuff = NULL;
	}

	if (m_MilPatternMaskDisplay != NULL)
	{
		m_MilPatternMaskDisplay->Close();
		m_MilPatternMaskDisplay = NULL;
	}

	for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
	{
		if (m_InspectionClass[i] != NULL)
		{
			m_InspectionClass[i]->InitImage(0, 0, FALSE);
			delete m_InspectionClass[i];
			m_InspectionClass[i] = NULL;
		}
	}
#endif
	if (m_DlgRect != NULL)
	{
		m_DlgRect->SetActiveImageRect(TRUE);
		m_DlgRect->DeleteAll();
		m_DlgRect->DestroyWindow();
		delete m_DlgRect;
		m_DlgRect = NULL;
	}
}

void CDialogTeachingPattern::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	int i = 0;
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_PARA_0, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_PARA_1, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_PARA_2, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_PARA_3, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_PARA_4, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_PARA_5, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_PARA_6, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_PARA_7, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_PARA_8, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_PARA_9, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_PARA_10, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_PARA_11, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_TEACHING_PARA_12, m_ctrTeachingPara[i++]);
	DDX_Control(pDX, IDC_SLIDER_PARA0, m_slider_Para0);
	DDX_Control(pDX, IDC_SLIDER_PARA1, m_slider_Para1);
}


BEGIN_MESSAGE_MAP(CDialogTeachingPattern, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_IMG_LOAD, &CDialogTeachingPattern::OnBnClickedButtonImgLoad)
	ON_BN_CLICKED(IDC_BUTTON_IMG_SAVE, &CDialogTeachingPattern::OnBnClickedButtonImgSave)
	ON_BN_CLICKED(IDC_BUTTON_IMG_GRAB, &CDialogTeachingPattern::OnBnClickedButtonImgGrab)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDialogTeachingPattern::OnBnClickedButtonClose)
	ON_WM_SHOWWINDOW()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SET_ROI, &CDialogTeachingPattern::OnBnClickedButtonSetRoi)
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_PATTERN_ADD, &CDialogTeachingPattern::OnBnClickedButtonPatternAdd)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_PATTERN_SEARCH, &CDialogTeachingPattern::OnBnClickedButtonPatternSearch)
	ON_BN_CLICKED(IDC_BUTTON_PATTERN_DEL, &CDialogTeachingPattern::OnBnClickedButtonSetRoi)
	ON_WM_RBUTTONDOWN()
	ON_WM_MOVE()
	ON_MESSAGE(USE_MSG_IMAGE_VIEW_TO_TEACHING_DIALOG, &CDialogTeachingPattern::OnImageViewToTeachingMessage)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDialogTeachingPattern::OnBnClickedButtonSave)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY_ORIGINAL_IMAGE, &CDialogTeachingPattern::OnBnClickedButtonDisplayOriginalImage)
END_MESSAGE_MAP()


// CDialogTeachingPattern 메시지 처리기


void CDialogTeachingPattern::OnBnClickedButtonImgLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE, _T("*.bmp"), _T("*.bmp"), OFN_HIDEREADONLY, _T("bmp(*.bmp)"));

	if (dlg.DoModal() == IDOK)
	{
#ifdef MIL_USE
		m_MilResultDispBuff->BufImport(dlg.GetPathName());
		m_MilDispBuff->Copy(m_MilResultDispBuff);
#endif
		//TestFunc();
	}
}

void CDialogTeachingPattern::PatternAddFunc()
{
#ifdef MIL_USE
	MgraClear(M_DEFAULT, m_GraphicList);
	/* Associate the graphic list to the display for annotations. */
	
	if (m_bPatternAdd)
	{
		if (m_GraphicList != NULL)
		{
			MgraFree(m_GraphicList);
			m_GraphicList = NULL;
		}
		MgraAllocList(CMilSystem::Instance()->GetSystemID(), M_DEFAULT, &m_GraphicList);
	}
	MdispControl(m_MilPatternDisplay->GetDisplayID(), M_ASSOCIATED_GRAPHIC_LIST_ID, m_GraphicList);
	
	m_InspectionClass[m_nPtnAddIndex]->SetPatternAddFunc(m_GraphicList, (MIL_ID)m_MilPatternDispBuff->GetIDBuffer(), (MIL_ID)m_MilPatternMaskDispBuff->GetIDBuffer(), 0, m_bPatternDel);
	m_InspectionClass[m_nPtnAddIndex]->SetPatternAddFunc(m_GraphicList, (MIL_ID)m_MilPatternDispBuff->GetIDBuffer(), (MIL_ID)m_MilPatternMaskDispBuff->GetIDBuffer(), 1, m_bPatternDel);
	m_InspectionClass[m_nPtnAddIndex]->SetPatternAddFunc(m_GraphicList, (MIL_ID)m_MilPatternDispBuff->GetIDBuffer(), (MIL_ID)m_MilPatternMaskDispBuff->GetIDBuffer(), 2, m_bPatternDel);

	m_bPatternAdd = TRUE;

	PatternAddImageCenAverageVal();
	SavePatternAddParaImage();
#endif
}

void CDialogTeachingPattern::SavePatternAddParaImage()
{
#ifdef MIL_USE
	CString strPath;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	strPath.Format(_T("%s\\Recipe\\%s\\Pattern_Add_Raw_Image.bmp"), strTemp, m_strCurrentRecipeName);
	m_MilResultDispBuff->Export(strPath, M_BMP);

	strPath.Format(_T("%s\\Recipe\\%s\\Pattern_Add_%d_Image.bmp"), strTemp, m_strCurrentRecipeName, m_nPtnAddIndex);
	m_MilPatternDispBuff->Export(strPath, M_BMP);

	strPath.Format(_T("%s\\Recipe\\%s\\Pattern_Add_Mask_%d_Image.bmp"), strTemp, m_strCurrentRecipeName, m_nPtnAddIndex);
	m_MilPatternMaskDispBuff->Export(strPath, M_BMP);
#endif
}

void CDialogTeachingPattern::LoadPatternAddParaImage()
{
#ifdef MIL_USE
	CString strPath;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	strPath.Format(_T("%s\\Recipe\\%s\\Pattern_Add_Raw_Image.bmp"), strTemp, m_strCurrentRecipeName);
	m_MilResultDispBuff->BufImport(strPath);
	m_MilDispBuff->Copy(m_MilResultDispBuff);

	strPath.Format(_T("%s\\Recipe\\%s\\Pattern_Add_%d_Image.bmp"), strTemp, m_strCurrentRecipeName, m_nPtnAddIndex);
	m_MilPatternResultDispBuff->BufImport(strPath);

	strPath.Format(_T("%s\\Recipe\\%s\\Pattern_Add_Mask_%d_Image.bmp"), strTemp, m_strCurrentRecipeName, m_nPtnAddIndex);
	m_MilPatternMaskDispBuff->BufImport(strPath);
#endif
}

BOOL CDialogTeachingPattern::PatternSearchFunc()
{
	////////////////////////////////////////////////////////////////////////////////////////
#ifdef MIL_USE
	m_InspectionClass[m_nPtnAddIndex]->StartInspectionThread(m_MilDispBuff);

	SetTimer(1, 100, NULL);
#endif
	return TRUE;
}

void CDialogTeachingPattern::OnBnClickedButtonImgSave()
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


void CDialogTeachingPattern::OnBnClickedButtonImgGrab()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
#ifdef INVERTER_COM
	int nDirection = 0, nSpeed = 0;
	CInverterClass::Instance()->SetInverterMove(nDirection, nSpeed, TRUE);
#else
	CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_MOTOR_CCW, TRUE);
#endif
	//SetTimer(998, 4000, NULL);
	CAtDetector::Instance()->GrabStart();
	//SetTimer(999, 9000, NULL);
}


void CDialogTeachingPattern::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->ShowWindow(SW_HIDE);
}


void CDialogTeachingPattern::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	
	if (bShow)
	{
		SetTimer(0, 100, NULL);
	}
	else
	{
		m_dlgXRayStatus->ShowWindow(bShow);
#ifdef MIL_USE
		if (m_GraphicList != NULL)
		{
			MgraFree(m_GraphicList);
			m_GraphicList = NULL;
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

		if (m_MilDisplay != NULL)
		{
			m_MilDisplay->EnableGraph(FALSE);
			m_MilDisplay->Close();
			m_MilDisplay = NULL;
		}		

		if (m_MilPatternDispBuff != NULL)
		{
			m_MilPatternDispBuff->Close();
			m_MilPatternDispBuff = NULL;
		}

		if (m_MilPatternResultDispBuff != NULL)
		{
			m_MilPatternResultDispBuff->Close();
			m_MilPatternResultDispBuff = NULL;
		}

		if (m_MilPatternDisplay != NULL)
		{
			m_MilPatternDisplay->EnableGraph(FALSE);
			

			m_MilPatternDisplay->Close();
			m_MilPatternDisplay = NULL;
		}

		

		if (m_MilPatternMaskDispBuff != NULL)
		{
			m_MilPatternMaskDispBuff->Close();
			m_MilPatternMaskDispBuff = NULL;
		}

		if (m_MilPatternMaskDisplay != NULL)
		{
			m_MilPatternMaskDisplay->Close();
			m_MilPatternMaskDisplay = NULL;
		}
		

		m_isDlgShow = FALSE;
		m_DlgRect->SetActiveTracker(0, FALSE);
#endif
		m_DlgRect->ShowWindow(SW_HIDE);
#ifdef MIL_USE
		for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
		{
			if (m_InspectionClass[i] != NULL)
			{
				m_InspectionClass[i]->InitImage(0, 0, FALSE);
				delete m_InspectionClass[i];
				m_InspectionClass[i] = NULL;
			}
		}
#endif
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CDialogTeachingPattern::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_dlgXRayStatus = new CDialogXRayStatus();
	m_dlgXRayStatus->Create(IDD_DIALOG_X_RAY_STATUS, this);
	m_DlgRect = new CDialogRect();
	m_DlgRect->Create(IDD_DIALOG_RECT, this);
	m_DlgRect->SetDlgIdx(0);

	SetDlgItemText(IDC_EDIT_TEACHING_PARA_0, _T("50"));
	SetDlgItemText(IDC_EDIT_TEACHING_PARA_1, _T("50"));
	SetDlgItemText(IDC_EDIT_TEACHING_PARA_2, _T("200"));
	SetDlgItemText(IDC_EDIT_TEACHING_PARA_3, _T("200"));

	SetDlgItemText(IDC_EDIT_TEACHING_PARA_4, _T("50.0"));

	SetDlgItemText(IDC_EDIT_TEACHING_PARA_8, _T("128"));
	SetDlgItemText(IDC_EDIT_TEACHING_PARA_9, _T("80"));
	SetDlgItemText(IDC_EDIT_TEACHING_PARA_10, _T("150"));
	SetDlgItemText(IDC_EDIT_TEACHING_PARA_11, _T("110"));

	GetDlgItem(IDC_STATIC_PATTERN_DISP)->GetWindowRect(&m_DefaultRect[0]);
	GetDlgItem(IDC_STATIC_PATTERN_MASK_DISP)->GetWindowRect(&m_DefaultRect[1]);

	m_slider_Para0.SetRange(0, 1000);
	m_slider_Para1.SetRange(0, 100);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogTeachingPattern::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);
	CRect rect;
	GetDlgItem(IDC_STATIC_X_RAY_STATUS)->GetWindowRect(&rect);
	m_dlgXRayStatus->MoveWindow(rect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogTeachingPattern::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch ((int)nIDEvent)
	{
	case 0:
		if (this->IsWindowVisible())
		{
			KillTimer(0);
			m_dlgXRayStatus->ShowWindow(SW_SHOW);
			LoadTeachingPara();
			UpdatePara(FALSE);
			MappControl(M_ERROR, M_PRINT_DISABLE);
			m_nPtnAddIndex = 0;
			SetDlgItemText(IDC_BTNENHCTRL_TEACHING_PTN_INDEX, _T("1"));

			m_DlgRect->SetDlgIdx(0);
			m_DlgRect->SetMainMessage(this->m_hWnd, USE_MSG_IMAGE_VIEW_TO_TEACHING_DIALOG);
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

			//GetDlgItem(IDC_STATIC_TEACHING_DISP)->MoveWindow(m_DispRect[0]);
			dRet = (double)m_DispRect[0].Width() / m_ImageSize.cx;
			dRet2 = (double)m_DispRect[0].Height() / m_ImageSize.cy;

			m_dRectRet[0] = m_ImageSize.cx / (double)m_DispRect[0].Width();
			m_dRectRet[1] = m_ImageSize.cy / (double)m_DispRect[0].Height();
#ifdef MIL_USE
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

			if(m_MilResultDispBuff != NULL)
			{
				m_MilResultDispBuff->Close();
				m_MilResultDispBuff = NULL;
			}

			m_MilDisplay = CRepository::Instance()->m_MilLib.NewDisplay();
			m_MilDispBuff = CRepository::Instance()->m_MilLib.NewImage();
			m_MilResultDispBuff = CRepository::Instance()->m_MilLib.NewImage();

			m_MilDispBuff->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
			m_MilDisplay->SetZoomFactor(dRet, dRet);
			
			m_MilResultDispBuff->Alloc2D(CSize(m_ImageSize.cx, m_ImageSize.cy));
			m_MilResultDispBuff->SelectWindow(m_MilDisplay, m_DlgRect->m_hWnd);

			m_MilDisplay->EnableOverlay();
			m_MilDisplay->EnableGraph();
			m_MilDisplay->SetColor(M_RGB888(0, 0, 255));
#endif

			m_DlgRect->SetZoom(dRet, m_ImageSize.cx, m_ImageSize.cy);
			m_DlgRect->SetEnable(0, TRUE);

			m_DlgRect->MoveWindow(m_DispRect[0]);
			m_DlgRect->SetActiveTracker(0, TRUE);
			m_DlgRect->ShowWindow(SW_SHOW);
			m_DlgRect->SetActiveImageRect(TRUE);
			m_DlgRect->DeleteAll();
			m_DlgRect->SetImageSize(m_ImageSize.cx, m_ImageSize.cy);
			char cTrackerCap[128];
			int nLen = 0;
			CString strText;
			strText.Format(_T("Pattern Add ROI"));
			sprintf_s(cTrackerCap, (size_t)(strText.GetLength() + 1), "%S", strText.GetBuffer());
			CRect rect;
			GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetClientRect(&rect);
			m_DlgRect->SetRect(rect_temp);
			m_DlgRect->AddTracker(0, m_InspPara[0].nPatternAddRect, RGB(0, 255, 255), cTrackerCap);
			//GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetWindowRect(&m_DispRect[0]);

			//m_DispRect[0].right = (long)(dRet * (double)m_ImageSize.cx + (double)m_DispRect[0].left);
			//m_DispRect[0].bottom = (long)(dRet * (double)m_ImageSize.cy + (double)m_DispRect[0].top);

			//m_DlgRect->MoveWindow(m_DispRect[0]);
			m_DlgRect->SetActiveTracker(0, FALSE);
			m_DlgRect->Invalidate();
#ifdef MIL_USE
			for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
			{
				m_InspectionClass[i] = new CInspectionClass();
				m_InspectionClass[i]->InitImage(m_ImageSize.cx, m_ImageSize.cy);
				m_InspectionClass[i]->SetInitEachInspectionThread();
			}

			GetDlgItem(IDC_STATIC_PATTERN_DISP)->MoveWindow(m_DefaultRect[0]);
			GetDlgItem(IDC_STATIC_PATTERN_MASK_DISP)->MoveWindow(m_DefaultRect[1]);


			//LoadPatternAddParaImage();
			CString strPath, strSearchPath;
			wchar_t strTemp[MAX_CHAR_LENG];
			GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
			strPath.Format(_T("%s\\Recipe\\%s\\Pattern_Add_Raw_Image.bmp"), strTemp, m_strCurrentRecipeName);
			strSearchPath.Format(_T("%s\\Recipe\\%s\\"), strTemp, m_strCurrentRecipeName);
			if (FileSearch(strSearchPath, _T("Pattern_Add_Raw_Image.bmp")))
			{
				m_MilResultDispBuff->BufImport(strPath);
				m_MilDispBuff->Copy(m_MilResultDispBuff);
				m_bInitDisplay = TRUE;
				m_bPatternDel = FALSE;
				m_bPatternAdd = TRUE;
				OnBnClickedButtonSetRoi();
				m_bPatternDel = FALSE;
				m_bPatternAdd = TRUE;
			}
#endif
			

			m_DlgRect->ShowWindow(SW_SHOW);
			m_isDlgShow = TRUE;
			m_dlgXRayStatus->SettingKVtoMA(m_InspPara->dVoltage, m_InspPara->dCurrent);
			MappControl(M_ERROR, M_PRINT_ENABLE);
		}
		break;
	case 1:
#ifdef MIL_USE
		if (m_InspectionClass[m_nPtnAddIndex]->GetAllInspectionEnd())
		{
			KillTimer(1);
			m_DlgRect->SetActiveImageRect(TRUE);

			m_DlgRect->DeleteAll();
			m_InspectionClass[m_nPtnAddIndex]->SetResultDisplay(TRUE);
			m_InspectionClass[m_nPtnAddIndex]->SetResultDisplay(m_MilDisplay, m_DlgRect);
			m_DlgRect->SetActiveImageRect(FALSE);

			m_DlgRect->Invalidate();

			//m_InspectionClass[m_nPtnAddIndex]->SetReleaseEachThread();
			InspButtonEnable(TRUE);
		}
#endif
		break;
	case 998:
		KillTimer(998);
		CAtDetector::Instance()->GrabStart();
		break;
	case 999://grab end conv stop
		KillTimer(999);
#ifdef INVERTER_COM
		CInverterClass::Instance()->SetInverterMove();
#else
		CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_MOTOR_CCW, FALSE);
#endif
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CDialogTeachingPattern::LoadTeachingPara()
{
	CString key, dat;
	CString file_path, pat_path, extention, master_path, cur_path;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	file_path.Format(_T("%s\\Recipe\\%s\\Teaching.ini"), strTemp, m_strCurrentRecipeName);

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(file_path);
	CString strNode, strKey, strLabel;

	strNode.Format(_T("TEACHING_PARAMETER"));
	strKey.Format(_T("X_RAY_KV"));
	m_InspPara->dVoltage = cDataIf.GetDouble(strNode, strKey, 10.);
	strKey.Format(_T("X_RAY_MA"));
	m_InspPara->dCurrent = cDataIf.GetDouble(strNode, strKey, 0.5);
	for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
	{
		strKey.Format(_T("PATTERN_ADD_ROI_%d_LEFT"), i);
		m_InspPara[i].nPatternAddRect.left = cDataIf.GetInt(strNode, strKey, 0);
		strKey.Format(_T("PATTERN_ADD_ROI_%d_TOP"), i);
		m_InspPara[i].nPatternAddRect.top = cDataIf.GetInt(strNode, strKey, 0);
		strKey.Format(_T("PATTERN_ADD_ROI_%d_RIGHT"), i);
		m_InspPara[i].nPatternAddRect.right = cDataIf.GetInt(strNode, strKey, 50);
		strKey.Format(_T("PATTERN_ADD_ROI_%d_BOTTOM"), i);
		m_InspPara[i].nPatternAddRect.bottom = cDataIf.GetInt(strNode, strKey, 50);

		strKey.Format(_T("PATTERN_ADD_%d_SMOOTHNESS"), i);
		m_InspPara[i].dSmoothness = cDataIf.GetDouble(strNode, strKey, 50);

		strKey.Format(_T("PATTERN_ADD_%d_ACTIVE_EDGE"), i);
		m_InspPara[i].dActiveEdge = cDataIf.GetDouble(strNode, strKey, 25);

		strKey.Format(_T("PATTERN_SEARCH_%d_PATTERN_CNT"), i);
		m_InspPara[i].nSearchPatternCnt = cDataIf.GetInt(strNode, strKey, 128);
		strKey.Format(_T("PATTERN_SEARCH_%d_PATTERN_SCORE"), i);
		m_InspPara[i].dSearchScore = cDataIf.GetDouble(strNode, strKey, 80.0);
		strKey.Format(_T("PATTERN_SEARCH_%d_PATTERN_THRESH_HOLD"), i);
		m_InspPara[i].nSearchPatternThreshHold = cDataIf.GetInt(strNode, strKey, 150);
		strKey.Format(_T("PATTERN_SEARCH_%d_PATTERN_OVERLAP_THRESH_HOLD"), i);
		m_InspPara[i].nSearchPatternOverlapThreshHold = cDataIf.GetInt(strNode, strKey, 110);

		strKey.Format(_T("PATTERN_SEARCH_%d_PATTERN_USE"), i);
		m_InspPara[i].bInspUse = cDataIf.GetInt(strNode, strKey, 1);
	}


	return TRUE;
}

BOOL CDialogTeachingPattern::SaveTeachingPara()
{
	CString key, dat;
	CString file_path, pat_path, extention, master_path, cur_path;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	file_path.Format(_T("%s\\Recipe\\%s\\Teaching.ini"), strTemp, m_strCurrentRecipeName);

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(file_path);
	CString strNode, strKey, strLabel;

	strNode.Format(_T("TEACHING_PARAMETER"));
	strKey.Format(_T("X_RAY_KV"));
	cDataIf.SetDouble(strNode, strKey, m_InspPara->dVoltage);
	strKey.Format(_T("X_RAY_MA"));
	cDataIf.SetDouble(strNode, strKey, m_InspPara->dCurrent);
	for (int i = 0; i < MAX_PATTERN_ADD_CNT; i++)
	{
		strKey.Format(_T("PATTERN_ADD_ROI_%d_LEFT"), i);
		cDataIf.SetInt(strNode, strKey, m_InspPara[i].nPatternAddRect.left);
		strKey.Format(_T("PATTERN_ADD_ROI_%d_TOP"), i);
		cDataIf.SetInt(strNode, strKey, m_InspPara[i].nPatternAddRect.top);
		strKey.Format(_T("PATTERN_ADD_ROI_%d_RIGHT"), i);
		cDataIf.SetInt(strNode, strKey, m_InspPara[i].nPatternAddRect.right);
		strKey.Format(_T("PATTERN_ADD_ROI_%d_BOTTOM"), i);
		cDataIf.SetInt(strNode, strKey, m_InspPara[i].nPatternAddRect.bottom);

		strKey.Format(_T("PATTERN_ADD_%d_SMOOTHNESS"), i);
		cDataIf.SetDouble(strNode, strKey, m_InspPara[i].dSmoothness);

		strKey.Format(_T("PATTERN_ADD_%d_ACTIVE_EDGE"), i);
		cDataIf.SetDouble(strNode, strKey, m_InspPara[i].dActiveEdge);

		strKey.Format(_T("PATTERN_SEARCH_%d_PATTERN_CNT"), i);
		cDataIf.SetInt(strNode, strKey, m_InspPara[i].nSearchPatternCnt);
		strKey.Format(_T("PATTERN_SEARCH_%d_PATTERN_SCORE"), i);
		cDataIf.SetDouble(strNode, strKey, m_InspPara[i].dSearchScore);
		strKey.Format(_T("PATTERN_SEARCH_%d_PATTERN_THRESH_HOLD"), i);
		cDataIf.SetInt(strNode, strKey, m_InspPara[i].nSearchPatternThreshHold);
		strKey.Format(_T("PATTERN_SEARCH_%d_PATTERN_OVERLAP_THRESH_HOLD"), i);
		cDataIf.SetInt(strNode, strKey, m_InspPara[i].nSearchPatternOverlapThreshHold);

		strKey.Format(_T("PATTERN_SEARCH_%d_PATTERN_USE"), i);
		cDataIf.SetInt(strNode, strKey, m_InspPara[i].bInspUse);
	}


	return TRUE;
}

void CDialogTeachingPattern::UpdatePara(BOOL isUpdate)
{
	CString strText;
	int i = 0, nPos = 0;
	if (isUpdate)
	{
		//GetDlgItemText(IDC_EDIT_TEACHING_PARA_0, strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara[m_nPtnAddIndex].nPatternAddRect.left = _wtoi(strText);
		//GetDlgItemText(IDC_EDIT_TEACHING_PARA_1, strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara[m_nPtnAddIndex].nPatternAddRect.top = _wtoi(strText);
		//GetDlgItemText(IDC_EDIT_TEACHING_PARA_2, strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara[m_nPtnAddIndex].nPatternAddRect.right = _wtoi(strText);
		//GetDlgItemText(IDC_EDIT_TEACHING_PARA_3, strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara[m_nPtnAddIndex].nPatternAddRect.bottom = _wtoi(strText);

		//GetDlgItemText(IDC_EDIT_TEACHING_PARA_4, strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara[m_nPtnAddIndex].dSmoothness = _wtoi(strText);

		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara[m_nPtnAddIndex].dActiveEdge = _wtoi(strText);

		i = 8;
		//GetDlgItemText(IDC_EDIT_TEACHING_PARA_8, strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara[m_nPtnAddIndex].nSearchPatternCnt = _wtoi(strText);
		//GetDlgItemText(IDC_EDIT_TEACHING_PARA_9, strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara[m_nPtnAddIndex].dSearchScore = _wtof(strText);
		//GetDlgItemText(IDC_EDIT_TEACHING_PARA_10, strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara[m_nPtnAddIndex].nSearchPatternThreshHold = _wtoi(strText);
		//GetDlgItemText(IDC_EDIT_TEACHING_PARA_11, strText);
		m_ctrTeachingPara[i++].GetWindowText(strText);
		m_InspPara[m_nPtnAddIndex].nSearchPatternOverlapThreshHold = _wtoi(strText);

		m_InspPara[m_nPtnAddIndex].bInspUse = m_ctrTeachingPara[i++].GetValue();

		double dKV, dMA;
		m_dlgXRayStatus->GetKVtoMA(&dKV, &dMA);
		m_InspPara->dVoltage = dKV;
		m_InspPara->dCurrent = dMA;
	}
	else
	{
		strText.Format(_T("%d"), m_InspPara[m_nPtnAddIndex].nPatternAddRect.left);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		//SetDlgItemText(IDC_EDIT_TEACHING_PARA_0, strText);
		strText.Format(_T("%d"), m_InspPara[m_nPtnAddIndex].nPatternAddRect.top);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		//SetDlgItemText(IDC_EDIT_TEACHING_PARA_1, strText);
		strText.Format(_T("%d"), m_InspPara[m_nPtnAddIndex].nPatternAddRect.right);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		//SetDlgItemText(IDC_EDIT_TEACHING_PARA_2, strText);
		strText.Format(_T("%d"), m_InspPara[m_nPtnAddIndex].nPatternAddRect.bottom);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		//SetDlgItemText(IDC_EDIT_TEACHING_PARA_3, strText);

		strText.Format(_T("%0.1f"), m_InspPara[m_nPtnAddIndex].dSmoothness);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		//SetDlgItemText(IDC_EDIT_TEACHING_PARA_4, strText);
		nPos = (int)(m_InspPara[m_nPtnAddIndex].dSmoothness * 10.);
		m_slider_Para0.SetPos(nPos);

		strText.Format(_T("%d"), (int)m_InspPara[m_nPtnAddIndex].dActiveEdge);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		//SetDlgItemText(IDC_EDIT_TEACHING_PARA_4, strText);
		nPos = (int)m_InspPara[m_nPtnAddIndex].dActiveEdge;
		m_slider_Para1.SetPos(nPos);

		i = 8;
		strText.Format(_T("%d"), m_InspPara[m_nPtnAddIndex].nSearchPatternCnt);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		//SetDlgItemText(IDC_EDIT_TEACHING_PARA_8, strText);
		strText.Format(_T("%0.1f"), m_InspPara[m_nPtnAddIndex].dSearchScore);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		//SetDlgItemText(IDC_EDIT_TEACHING_PARA_9, strText);
		strText.Format(_T("%d"), m_InspPara[m_nPtnAddIndex].nSearchPatternThreshHold);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		//SetDlgItemText(IDC_EDIT_TEACHING_PARA_10, strText);
		strText.Format(_T("%d"), m_InspPara[m_nPtnAddIndex].nSearchPatternOverlapThreshHold);
		m_ctrTeachingPara[i++].SetWindowText(strText);
		//SetDlgItemText(IDC_EDIT_TEACHING_PARA_11, strText);

		m_ctrTeachingPara[i++].SetValue(m_InspPara[m_nPtnAddIndex].bInspUse);

		m_dlgXRayStatus->SettingKVtoMA(m_InspPara->dVoltage, m_InspPara->dCurrent);
	}
}

void CDialogTeachingPattern::OnBnClickedButtonSetRoi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdatePara(TRUE);
#ifdef MIL_USE
	OnBnClickedButtonPatternDel();
	
	MgraAllocList(CMilSystem::Instance()->GetSystemID(), M_DEFAULT, &m_GraphicList);

	m_MilDisplay->SetColor(M_RGB888(0, 255, 255));

	m_MilPatternDisplay = CRepository::Instance()->m_MilLib.NewDisplay();
	m_MilPatternMaskDisplay = CRepository::Instance()->m_MilLib.NewDisplay();
	double dRetX, dRetY;
	CRect patternRect, temp_rect;;
	GetDlgItem(IDC_STATIC_PATTERN_DISP)->GetClientRect(&patternRect);
	GetDlgItem(IDC_STATIC_PATTERN_DISP)->GetWindowRect(&m_DispRect[1]);
	this->GetWindowRect(&temp_rect);
	dRetX = (double)m_DefaultRect[0].Width() / abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Width());
	dRetY = (double)m_DefaultRect[0].Height() / abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Height());

	if (m_InspPara[m_nPtnAddIndex].nPatternAddRect.Width() > m_InspPara[m_nPtnAddIndex].nPatternAddRect.Height())
		dRetY = dRetX;
	else
		dRetX = dRetY;
	
	m_MilPatternDisplay->SetZoomFactor(dRetX, dRetY);
	GetDlgItem(IDC_STATIC_PATTERN_DISP)->MoveWindow(m_DefaultRect[0].left - (temp_rect.left + 8), m_DefaultRect[0].top - (temp_rect.top + 31), (int)(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Width() * dRetX), (int)(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Height() * dRetY));

	m_MilPatternResultDispBuff = CRepository::Instance()->m_MilLib.NewImage();
	m_MilPatternResultDispBuff->Alloc2D(CSize(abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Width()), abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Height())));
	m_MilPatternResultDispBuff->SelectWindow(m_MilPatternDisplay, (GetDlgItem)(IDC_STATIC_PATTERN_DISP)->m_hWnd);

	m_MilPatternMaskDisplay->SetZoomFactor(dRetX, dRetY);
	GetDlgItem(IDC_STATIC_PATTERN_MASK_DISP)->GetWindowRect(&m_DispRect[0]);
	GetDlgItem(IDC_STATIC_PATTERN_MASK_DISP)->MoveWindow(m_DefaultRect[1].left - (temp_rect.left + 8), m_DefaultRect[1].top - (temp_rect.top + 31), (int)(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Width() * dRetX), (int)(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Height() * dRetY));

	m_MilPatternMaskDispBuff = CRepository::Instance()->m_MilLib.NewImage();
	m_MilPatternMaskDispBuff->Alloc2D(CSize(abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Width()), abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Height())));
	m_MilPatternMaskDispBuff->SelectWindow(m_MilPatternMaskDisplay, (GetDlgItem)(IDC_STATIC_PATTERN_MASK_DISP)->m_hWnd);
	
	if (m_bInitDisplay)
	{
		LoadPatternAddParaImage();
	}
	
	m_MilDisplay->ClearOverlay();
	//m_MilDisplay->Rect(m_InspPara.nPatternAddRect.left - 1, m_InspPara.nPatternAddRect.top - 1, m_InspPara.nPatternAddRect.right + 1, m_InspPara.nPatternAddRect.bottom + 1);
	//m_MilDisplay->Rect(m_InspPara.nPatternAddRect.left - 2, m_InspPara.nPatternAddRect.top - 2, m_InspPara.nPatternAddRect.right + 2, m_InspPara.nPatternAddRect.bottom + 2);
	m_MilDisplay->Rect(m_InspPara[m_nPtnAddIndex].nPatternAddRect.left, m_InspPara[m_nPtnAddIndex].nPatternAddRect.top, m_InspPara[m_nPtnAddIndex].nPatternAddRect.right, m_InspPara[m_nPtnAddIndex].nPatternAddRect.bottom);

	BYTE* pData;
	pData = DBG_NEW BYTE[abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Width()) * abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Height())];

	m_MilDispBuff->Get2D(pData, m_InspPara[m_nPtnAddIndex].nPatternAddRect);
	m_MilPatternResultDispBuff->Put2D(0, CSize(abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Width()), abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Height())), pData);

	if (!m_bInitDisplay)
	{
		memset(pData, 0, sizeof(BYTE) * abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Width()) * abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Height()));
		m_MilPatternMaskDispBuff->Put(pData);
	}
	
	delete [] pData;
	
	if (m_MilPatternDispBuff != NULL)
	{
		m_MilPatternDispBuff->Close();
		m_MilPatternDispBuff = NULL;
	}

	m_MilPatternDispBuff = CRepository::Instance()->m_MilLib.NewImage();
	m_MilPatternDispBuff->Alloc2D(CSize(abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Width()), abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Height())));

	m_MilPatternDispBuff->Copy(m_MilPatternResultDispBuff);
	
	PatternAddImageCenAverageVal();

	//if (m_bInitDisplay)
	{
		m_bPatternDel = FALSE;
	}

	if(m_InspPara[m_nPtnAddIndex].bInspUse)
		OnBnClickedButtonPatternAdd();
	m_bInitDisplay = FALSE;
	m_bPatternDel = TRUE;
#endif
}


void CDialogTeachingPattern::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bRBMouseDown = FALSE;
	CPoint pPoint;
	CRect main_rect;;
	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetClientRect(&main_rect);
	point.x -= 15;
	point.y -= 78;
	if (main_rect.left <= point.x && point.x <= main_rect.right &&
		main_rect.top <= point.y && point.y <= main_rect.bottom)
	{
		pPoint.x = (int)((double)point.x * (double)((double)m_ImageSize.cx / main_rect.Width()));
		pPoint.y = (int)((double)point.y * (double)((double)m_ImageSize.cy / main_rect.Height()));
		
		OnBnClickedButtonSetRoi();
	}

	CDialogEx::OnRButtonUp(nFlags, point);
}


void CDialogTeachingPattern::OnBnClickedButtonPatternAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdatePara(TRUE);
#ifdef MIL_USE
	m_InspectionClass[m_nPtnAddIndex]->SetInspPara(CMilSystem::Instance()->GetSystemID(), m_InspPara[m_nPtnAddIndex], m_strCurrentRecipeName, m_nPtnAddIndex);
	m_bPatternAdd = TRUE;
	PatternAddFunc();
#endif
}


void CDialogTeachingPattern::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bLBMouseDown = FALSE;
	if (m_bMouseMove)
	{
		m_bMouseMove = FALSE;
		OnBnClickedButtonPatternAdd();
		//PatternAddFunc();
		//m_MilPatternMaskDispBuff->Export(_T("d:\\TestMask.bmp"), M_BMP);
	}
	
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDialogTeachingPattern::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CPoint pPoint;
	CRect main_rect;;
	GetDlgItem(IDC_STATIC_PATTERN_MASK_DISP)->GetClientRect(&main_rect);

	point.x -= 598;
	point.y -= 371;

	if (main_rect.left <= point.x && point.x <= main_rect.right &&
		main_rect.top <= point.y && point.y <= main_rect.bottom)
	{
#ifdef MIL_USE
		m_MilPatternDisplay->SetColor(M_RGB888(0, 255, 0));
#endif
		if (!m_bPatternAdd)
		{
			AfxMessageBox(_T("Not Pattern Add"));
		}
		else
		{
			m_bLBMouseDown = TRUE;
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDialogTeachingPattern::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint pPoint;
	CRect main_rect;
#ifdef MIL_USE
	if (m_bLBMouseDown || m_bRBMouseDown)
	{	
		GetDlgItem(IDC_STATIC_PATTERN_MASK_DISP)->GetClientRect(&main_rect);

		point.x -= 598;
		point.y -= 371;

		if (main_rect.left <= point.x && point.x <= main_rect.right &&
			main_rect.top <= point.y && point.y <= main_rect.bottom)
		{
			m_bMouseMove = TRUE;
			pPoint.x = (int)((double)point.x * (double)((double)m_InspPara[m_nPtnAddIndex].nPatternAddRect.Width() / main_rect.Width()));// +m_InspPara.nPatternAddRect.left;
			pPoint.y = (int)((double)point.y * (double)((double)m_InspPara[m_nPtnAddIndex].nPatternAddRect.Height() / main_rect.Height()));// +m_InspPara.nPatternAddRect.top;

			BYTE* pData;

			tImageProperties pImageProperty;
			m_MilPatternMaskDispBuff->GetImageProperties(&pImageProperty);

			pData = DBG_NEW BYTE[pImageProperty.ImageSize.cx * pImageProperty.ImageSize.cy];

			CRect img_rect;
			img_rect.left = 0;
			img_rect.top = 0;
			img_rect.right = pImageProperty.ImageSize.cx;
			img_rect.bottom = pImageProperty.ImageSize.cy;
			m_MilPatternMaskDispBuff->Get2D(pData, img_rect);
			
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
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

					if (m_bLBMouseDown)
					{
						m_MilPatternDisplay->Dot((double)nX, (double)nY);
						pData[(nY * pImageProperty.ImageSize.cx) + nX] = 255;
					}


				}
			}

			m_MilPatternMaskDispBuff->Put2D(CSize(0, 0), pImageProperty.ImageSize, pData);


			delete [] pData;
			m_bPatternDel = FALSE;
		}
		else
		{
			m_bLBMouseDown = FALSE;
			m_bRBMouseDown = FALSE;
		}
	}
#endif
	CDialogEx::OnMouseMove(nFlags, point);
}

void CDialogTeachingPattern::InspButtonEnable(BOOL isEnable)
{
	GetDlgItem(IDC_BUTTON_IMG_LOAD)->EnableWindow(isEnable);
	GetDlgItem(IDC_BUTTON_IMG_SAVE)->EnableWindow(isEnable);
	GetDlgItem(IDC_BUTTON_IMG_GRAB)->EnableWindow(isEnable);
	GetDlgItem(IDC_BUTTON_DISPLAY_ORIGINAL_IMAGE)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_MINUS)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PLUS)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PARA_0)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PARA_1)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PARA_2)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PARA_3)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PARA_4)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PARA_5)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PARA_6)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PARA_7)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PARA_8)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PARA_9)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PARA_10)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PARA_11)->EnableWindow(isEnable);
	GetDlgItem(IDC_BTNENHCTRL_TEACHING_PARA_12)->EnableWindow(isEnable);
	GetDlgItem(IDC_BUTTON_SET_ROI)->EnableWindow(isEnable);
	GetDlgItem(IDC_BUTTON_PATTERN_ADD)->EnableWindow(isEnable);
	GetDlgItem(IDC_BUTTON_PATTERN_DEL)->EnableWindow(isEnable);
	GetDlgItem(IDC_BUTTON_PATTERN_SEARCH)->EnableWindow(isEnable);
}

void CDialogTeachingPattern::OnBnClickedButtonPatternSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdatePara(TRUE);
	InspButtonEnable(FALSE);
#ifdef MIL_USE
	m_InspectionClass[m_nPtnAddIndex]->SetInspPara(CMilSystem::Instance()->GetSystemID(), m_InspPara[m_nPtnAddIndex], m_strCurrentRecipeName, m_nPtnAddIndex);
	if (m_InspPara[m_nPtnAddIndex].bInspUse)
		PatternSearchFunc();
	else
		InspButtonEnable(TRUE);
#endif
}


void CDialogTeachingPattern::OnBnClickedButtonPatternDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bPatternDel = TRUE;
	m_bPatternAdd = FALSE;
#ifdef MIL_USE
	if (m_MilPatternDisplay != NULL)
	{
		m_MilPatternDisplay->Close();
		m_MilPatternDisplay = NULL;
	}
	if (m_MilPatternDispBuff != NULL)
	{
		m_MilPatternDispBuff->Close();
		m_MilPatternDispBuff = NULL;
	}
	if (m_MilPatternResultDispBuff != NULL)
	{
		m_MilPatternResultDispBuff->Close();
		m_MilPatternResultDispBuff = NULL;
	}

	if (m_MilPatternMaskDisplay != NULL)
	{
		m_MilPatternMaskDisplay->Close();
		m_MilPatternMaskDisplay = NULL;
	}
	if (m_MilPatternMaskDispBuff != NULL)
	{
		m_MilPatternMaskDispBuff->Close();
		m_MilPatternMaskDispBuff = NULL;
	}

	//MmodPreprocess(m_MilSearchContext, M_RESET);

	if (m_GraphicList != NULL)
	{
		MgraClear(M_DEFAULT, m_GraphicList);
		MgraFree(m_GraphicList);
		m_GraphicList = NULL;
	}
#endif
}


void CDialogTeachingPattern::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bRBMouseDown = TRUE;
	CDialogEx::OnRButtonDown(nFlags, point);
}


void CDialogTeachingPattern::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);
	if (m_isDlgShow)
	{
		GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetWindowRect(&m_DispRect[0]);
		m_DlgRect->MoveWindow(m_DispRect[0]);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

LRESULT CDialogTeachingPattern::OnImageViewToTeachingMessage(WPARAM para0, LPARAM para1)
{
	int nEvent = (int)para0;
	int nTrackerIdx = (int)para1;

	switch (nEvent)
	{
	case 0:
	{
		double left, top, right, bottom;
		m_DlgRect->GetTracker(nTrackerIdx, &left, &top, &right, &bottom);

		m_InspPara[m_nPtnAddIndex].nPatternAddRect.SetRect((int)left, (int)top, (int)right, (int)bottom);
		UpdatePara(FALSE);
		m_bInitDisplay = FALSE;
		m_bPatternAdd = TRUE;
		OnBnClickedButtonSetRoi();
		//OnBnClickedButtonPatternAdd();
	}
		break;
	case 1:
	{
#ifdef MIL_USE
		CMilBuffer* milTempBuff;
		milTempBuff = CRepository::Instance()->m_MilLib.NewImage();
 		milTempBuff->Alloc2D(CSize(m_ImageSize.cy, m_ImageSize.cx));

		long nPitch;
#ifdef HW_USE
		nPitch = m_ImageSize.cy;
#else
		MbufInquire(milTempBuff->GetIDBuffer(), M_PITCH, &nPitch);
#endif
		BYTE* pData;
		WORD* pData2;
		pData2 = DBG_NEW WORD[nPitch * m_ImageSize.cx];
		pData = DBG_NEW BYTE[m_ImageSize.cx * m_ImageSize.cy];
		memcpy(pData2, (void*)para1, sizeof(WORD) * nPitch * m_ImageSize.cx);

		for (int y = 0; y < m_ImageSize.cx; y++)
		{
			for (int x = 0; x < m_ImageSize.cy; x++)
			{
				if(CAtDetector::Instance()->GetImageType() == 0 || CAtDetector::Instance()->GetImageType() ==  2)
					pData[(y * m_ImageSize.cy) + x] = pData2[(y * nPitch) + x] / 256;
				else
					pData[(y * m_ImageSize.cy) + x] = (BYTE)pData2[(y * nPitch) + x];
			}
		}

		milTempBuff->Put(pData);
		MimRotate(milTempBuff->GetIDBuffer(), m_MilResultDispBuff->GetIDBuffer(), 90, m_ImageSize.cy / 2, m_ImageSize.cx / 2, m_ImageSize.cx / 2, m_ImageSize.cy / 2, M_DEFAULT + M_OVERSCAN_DISABLE);

		milTempBuff->Close();
		milTempBuff = NULL;
		delete [] pData2;
		delete[] pData;
		m_MilDispBuff->Copy(m_MilResultDispBuff);
		SetTimer(999, (int)(CAtDetector::Instance()->GetScanSpeed() / 5.) * 1000, NULL);
#endif
	}
		break;
	}

	return 0;
}

void CDialogTeachingPattern::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdatePara(TRUE);
	SaveTeachingPara();
#ifdef MIL_USE
	m_InspectionClass[m_nPtnAddIndex]->SetPatternModelSave(m_nPtnAddIndex - 1);
#endif
}

BEGIN_EVENTSINK_MAP(CDialogTeachingPattern, CDialogEx)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PARA_0, DISPID_CLICK, CDialogTeachingPattern::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PARA_1, DISPID_CLICK, CDialogTeachingPattern::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PARA_2, DISPID_CLICK, CDialogTeachingPattern::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PARA_3, DISPID_CLICK, CDialogTeachingPattern::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PARA_4, DISPID_CLICK, CDialogTeachingPattern::SelectParameterNumDlgShowDouble, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PARA_5, DISPID_CLICK, CDialogTeachingPattern::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PARA_6, DISPID_CLICK, CDialogTeachingPattern::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PARA_7, DISPID_CLICK, CDialogTeachingPattern::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PARA_8, DISPID_CLICK, CDialogTeachingPattern::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PARA_9, DISPID_CLICK, CDialogTeachingPattern::SelectParameterNumDlgShowDouble, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PARA_10, DISPID_CLICK, CDialogTeachingPattern::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PARA_11, DISPID_CLICK, CDialogTeachingPattern::SelectParameterNumDlgShowInt, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_MINUS, DISPID_CLICK, CDialogTeachingPattern::ClickBtnenhctrlTeachingMinus, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PLUS, DISPID_CLICK, CDialogTeachingPattern::ClickBtnenhctrlTeachingPlus, VTS_NONE)
	ON_EVENT(CDialogTeachingPattern, IDC_BTNENHCTRL_TEACHING_PARA_12, DISPID_CLICK, CDialogTeachingPattern::ClickBtnenhctrlTeachingPara12, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogTeachingPattern::SelectParameterNumDlgShowInt()
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
	if (_wtoi(strText) < 0)
		return;

	pBtnEnh->SetWindowTextW(strText);

	if (pBtnEnh->GetDlgCtrlID() == IDC_BTNENHCTRL_TEACHING_PARA_4 || pBtnEnh->GetDlgCtrlID() == IDC_BTNENHCTRL_TEACHING_PARA_5)
	{
		OnBnClickedButtonSetRoi();
		OnBnClickedButtonPatternAdd();
		m_slider_Para1.SetPos(_wtoi(strText));
	}
}

void CDialogTeachingPattern::SelectParameterNumDlgShowDouble()
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

	if (pBtnEnh->GetDlgCtrlID() == IDC_BTNENHCTRL_TEACHING_PARA_4 || pBtnEnh->GetDlgCtrlID() == IDC_BTNENHCTRL_TEACHING_PARA_5)
	{
		OnBnClickedButtonSetRoi();
		OnBnClickedButtonPatternAdd();
		int nPos = (int)(_wtof(strText) * 10.);
		m_slider_Para0.SetPos(nPos);
	}
}

void CDialogTeachingPattern::PatternAddImageCenAverageVal()
{
	CRect rect;
#ifdef MIL_USE
	CPoint cpCen;
	cpCen.SetPoint(abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Width()) / 2, abs(m_InspPara[m_nPtnAddIndex].nPatternAddRect.Height()) / 2);

	rect.SetRect(cpCen.x - 1, cpCen.y - 1, cpCen.x + 1, cpCen.y + 1);

	m_MilPatternDisplay->EnableGraph(FALSE);
	m_MilPatternDisplay->EnableOverlay(FALSE);

	m_MilPatternDisplay->EnableOverlay();
	m_MilPatternDisplay->EnableGraph();
	m_MilPatternDisplay->SetColor(M_RGB888(255, 0, 255));
	m_MilPatternDisplay->Rect(rect);
	//MgraRect(m_GraphicList, m_MilPatternDisplay->GetDisplayID(), rect.left, rect.top, rect.right, rect.bottom);
	BYTE* pData;
	pData = DBG_NEW BYTE[rect.Width() * rect.Height()];
	m_MilPatternResultDispBuff->Get2D(pData, rect);

	double dVal;
	long lSum = 0;
	int nCnt = 0;
	for (int y = 0; y < rect.Height(); y++)
	{
		for (int x = 0; x < rect.Width(); x++)
		{
			lSum += (long)pData[(y * rect.Width()) + x];
			nCnt++;
		}
	}
	dVal = lSum / (double)nCnt;
	CString strText;
	strText.Format(_T("%0.1f"), dVal);
	SetDlgItemText(IDC_BTNENHCTRL_TEACHING_CENTER_AVG_VAL, strText);;
	delete [] pData;
#endif
}

void CDialogTeachingPattern::ClickBtnenhctrlTeachingMinus()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_nPtnAddIndex--;
	if (m_nPtnAddIndex < 0)
		m_nPtnAddIndex = 0;

	CString strText;
	strText.Format(_T("%d"), m_nPtnAddIndex + 1);
	SetDlgItemText(IDC_BTNENHCTRL_TEACHING_PTN_INDEX, strText);
	UpdatePara(FALSE);

	m_DlgRect->SetActiveImageRect(TRUE);
	m_DlgRect->DeleteAll();
	m_DlgRect->SetImageSize(m_ImageSize.cx, m_ImageSize.cy);
	char cTrackerCap[128];
	int nLen = 0;
	strText.Format(_T("Pattern Add ROI"));
	sprintf_s(cTrackerCap, (size_t)(strText.GetLength() + 1), "%S", strText.GetBuffer());
	CRect rect;

	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetClientRect(&rect);
	double dRet = (double)rect.Width() / m_ImageSize.cx;
	rect.bottom = (long)(dRet * (double)m_ImageSize.cy + (double)rect.top);
	m_DlgRect->SetRect(rect);
	m_DlgRect->AddTracker(0, m_InspPara[m_nPtnAddIndex].nPatternAddRect, RGB(0, 255, 255), cTrackerCap);
	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetWindowRect(&m_DispRect[0]);
	//m_DlgRect->MoveWindow(m_DispRect[0]);
	m_DlgRect->SetActiveTracker(0, FALSE);
	m_DlgRect->Invalidate();

	m_bInitDisplay = TRUE;
	m_bPatternDel = FALSE;
	m_bPatternAdd = TRUE;
	OnBnClickedButtonSetRoi();
	m_bPatternDel = FALSE;
	m_bPatternAdd = FALSE;
}


void CDialogTeachingPattern::ClickBtnenhctrlTeachingPlus()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_nPtnAddIndex++;
	if (m_nPtnAddIndex > MAX_PATTERN_ADD_CNT - 1)
		m_nPtnAddIndex = MAX_PATTERN_ADD_CNT -1;

	CString strText;
	strText.Format(_T("%d"), m_nPtnAddIndex + 1);
	SetDlgItemText(IDC_BTNENHCTRL_TEACHING_PTN_INDEX, strText);
	UpdatePara(FALSE);

	m_DlgRect->SetActiveImageRect(TRUE);
	m_DlgRect->DeleteAll();
	m_DlgRect->SetImageSize(m_ImageSize.cx, m_ImageSize.cy);
	char cTrackerCap[128];
	int nLen = 0;
	strText.Format(_T("Pattern Add ROI"));
	sprintf_s(cTrackerCap, (size_t)(strText.GetLength() + 1), "%S", strText.GetBuffer());
	CRect rect;

	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetClientRect(&rect);
	double dRet = (double)rect.Width() / m_ImageSize.cx;
	rect.bottom = (long)(dRet * (double)m_ImageSize.cy + (double)rect.top);
	m_DlgRect->SetRect(rect);
	m_DlgRect->AddTracker(0, m_InspPara[m_nPtnAddIndex].nPatternAddRect, RGB(0, 255, 255), cTrackerCap);
	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetWindowRect(&m_DispRect[0]);
	//m_DlgRect->MoveWindow(m_DispRect[0]);
	m_DlgRect->SetActiveTracker(0, FALSE);
	m_DlgRect->Invalidate();

	m_bInitDisplay = TRUE;
	m_bPatternDel = FALSE;
	m_bPatternAdd = TRUE;
	OnBnClickedButtonSetRoi();
	m_bPatternDel = FALSE;
	m_bPatternAdd = FALSE;
}


BOOL CDialogTeachingPattern::PreTranslateMessage(MSG* pMsg)
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
	CPoint point, pPoint;
	point = pMsg->pt;

	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetWindowRect(&temp_rect);
	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetClientRect(&main_rect);
	point.x -= temp_rect.left;
	point.y -= temp_rect.top;
	if (m_DlgRect->IsWindowVisible())
	{
		if (main_rect.left <= point.x && point.x <= main_rect.right &&
			main_rect.top <= point.y && point.y <= main_rect.bottom)
		{
			pPoint.x = (int)((double)point.x * m_dRectRet[0]);// (double)((double)m_ImageSize.cx / main_rect.Width()));// +m_InspPara.nPatternAddRect.left;
			pPoint.y = (int)((double)point.y * m_dRectRet[0]);// (double)((double)m_ImageSize.cy / main_rect.Height()));// +m_InspPara.nPatternAddRect.top;


			if (pPoint.x >= m_ImageSize.cx)
				pPoint.x = m_ImageSize.cx - 1;

			if (pPoint.y >= m_ImageSize.cy)
				pPoint.y = m_ImageSize.cy - 1;

			if (m_pImagePoint.x == pPoint.x && m_pImagePoint.y == pPoint.y)
			{
				int k;
				k = 0;
			}
			else
			{
#ifdef MIL_USE
				int nVal = 0;
				m_MilDispBuff->GetPixelGray(pPoint.x, pPoint.y, &nVal);

				//m_DlgRect->SetPointGrayValueDraw(m_MilDisplay, pPoint, nVal);

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
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogTeachingPattern::ClickBtnenhctrlTeachingPara12()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetFocus();
	int nUse = pBtnEnh->GetValue();

	if(nUse)
	{
		m_bInitDisplay = TRUE;
		m_bPatternDel = FALSE;
		m_bPatternAdd = TRUE;
		OnBnClickedButtonSetRoi();
		m_bPatternDel = FALSE;
		m_bPatternAdd = TRUE;
		pBtnEnh->SetWindowText(_T("USE"));
	}
	else
	{
		m_bInitDisplay = FALSE;
		m_bPatternDel = FALSE;
		m_bPatternAdd = TRUE;
		OnBnClickedButtonSetRoi();
		m_bPatternDel = FALSE;
		m_bPatternAdd = TRUE;
		pBtnEnh->SetWindowText(_T("NOT"));
	}

}


void CDialogTeachingPattern::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int iPos = 0;
	double dPos = 0.;
	CString strText;
	if (IDC_SLIDER_PARA0 == pScrollBar->GetDlgCtrlID())
	{
		iPos = m_slider_Para0.GetPos();
		dPos = iPos / 10.;
		strText.Format(_T("%0.1f"), dPos);
		m_ctrTeachingPara[4].SetWindowText(strText);
	}
	else if (IDC_SLIDER_PARA1 == pScrollBar->GetDlgCtrlID())
	{
		iPos = m_slider_Para1.GetPos();
		strText.Format(_T("%d"), iPos);
		m_ctrTeachingPara[5].SetWindowText(strText);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDialogTeachingPattern::OnBnClickedButtonDisplayOriginalImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_DlgRect->SetActiveImageRect(TRUE);
	m_DlgRect->DeleteAll();
	m_DlgRect->SetImageSize(m_ImageSize.cx, m_ImageSize.cy);
	char cTrackerCap[128];
	int nLen = 0;
	CString strText;
	strText.Format(_T("Pattern Add ROI"));
	sprintf_s(cTrackerCap, (size_t)(strText.GetLength() + 1), "%S", strText.GetBuffer());
	CRect rect;
	GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetClientRect(&rect);
	m_DlgRect->AddTracker(0, m_InspPara[0].nPatternAddRect, RGB(0, 255, 255), cTrackerCap);
	//GetDlgItem(IDC_STATIC_TEACHING_DISP)->GetWindowRect(&m_DispRect[0]);

	//m_DispRect[0].right = (long)(dRet * (double)m_ImageSize.cx + (double)m_DispRect[0].left);
	//m_DispRect[0].bottom = (long)(dRet * (double)m_ImageSize.cy + (double)m_DispRect[0].top);

	//m_DlgRect->MoveWindow(m_DispRect[0]);
	m_DlgRect->SetActiveTracker(0, FALSE);
	m_DlgRect->Invalidate();
}
