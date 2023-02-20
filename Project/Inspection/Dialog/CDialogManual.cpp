// CDialogManual.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogManual.h"
#include "ProfileDataIF.h"
#include "CIOClass.h"
#include "CDialogXRayWarmup.h"
#include "CATDetector.h"

// CDialogManual 대화 상자

IMPLEMENT_DYNAMIC(CDialogManual, CDialogEx)

CDialogManual::CDialogManual(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MANUAL, pParent)
{
	for (int i = 0; i < MAX_INPUT_NUM; i++)
	{
		m_stInput[i].nIONum = -1;
		memset(m_stInput[i].strIOLabel, NULL, sizeof(wchar_t) * MAX_CHAR_LENG);
		m_bInputState[i] = FALSE;
		m_bListInputState[i] = FALSE;
	}

	for (int i = 0; i < MAX_OUTPUT_NUM; i++)
	{
		m_stOutput[i].nIONum = -1;
		memset(m_stOutput[i].strIOLabel, NULL, sizeof(wchar_t) * MAX_CHAR_LENG);
		m_bOutputState[i] = FALSE;
		m_bListOutputState[i] = FALSE;
	}

	m_Img_list_slct = NULL;
}

CDialogManual::~CDialogManual()
{
	if (m_dlgXRayStatus != NULL)
	{
		m_dlgXRayStatus->DestroyWindow();
		delete m_dlgXRayStatus;
		m_dlgXRayStatus = NULL;
	}

	if (m_Img_list_slct != NULL)
		delete m_Img_list_slct;
}

void CDialogManual::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INPUT, m_listctrl_IO_Input);
	DDX_Control(pDX, IDC_LIST_OUTPUT, m_listctrl_IO_Output);
}


BEGIN_MESSAGE_MAP(CDialogManual, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDialogManual::OnBnClickedButtonClose)
	ON_WM_SHOWWINDOW()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_LIST_OUTPUT, &CDialogManual::OnNMClickListInput)
	ON_BN_CLICKED(IDC_BUTTON_X_RAY_WARMUP, &CDialogManual::OnBnClickedButtonXRayWarmup)
	ON_BN_CLICKED(IDC_BUTTON_DETECTOR_CAL, &CDialogManual::OnBnClickedButtonDetectorCal)
END_MESSAGE_MAP()


// CDialogManual 메시지 처리기


void CDialogManual::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->ShowWindow(SW_HIDE);
}


void CDialogManual::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	m_dlgXRayStatus->ShowWindow(bShow);
	if (bShow)
	{
		CRect rect;
		GetDlgItem(IDC_STATIC_X_RAY_STATUS)->GetWindowRect(&rect);

		m_dlgXRayStatus->MoveWindow(rect);
		//LoadIOLabelPara();
		//SetIOListLabel();

		for (int i = 0; i < MAX_INPUT_NUM; i++)
		{
			m_bListInputState[i] = FALSE;
		}

		for (int i = 0; i < MAX_OUTPUT_NUM; i++)
		{
			m_bListOutputState[i] = FALSE;
		}

		if (m_LoginType == LOGIN_ADMIN)
		{
			GetDlgItem(IDC_BUTTON_DETECTOR_CAL)->EnableWindow(TRUE);
		}
		else
			GetDlgItem(IDC_BUTTON_DETECTOR_CAL)->EnableWindow(FALSE);

		SetTimer(0, 100, NULL);
		Sleep(10);
		SetTimer(1, 100, NULL);
	}
	else
	{
		KillTimer(0);
		KillTimer(1);
		KillTimer(200);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CDialogManual::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_dlgXRayStatus = new CDialogXRayStatus();
	m_dlgXRayStatus->Create(IDD_DIALOG_X_RAY_STATUS, this);

	LoadIOLabelPara();
	SetIOListLabel();

	CIOClass::Instance()->SetIOPara(m_stInput, m_stOutput);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogManual::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);
	CRect rect;
	GetDlgItem(IDC_STATIC_X_RAY_STATUS)->GetWindowRect(&rect);
	m_dlgXRayStatus->MoveWindow(rect);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDialogManual::SetIOListLabel()
{
	LV_COLUMN lvColumn;
	//CFont font;
	
	CBitmap bitmap;
	//CString str;
	int i;

	//LoadIOLabelPara();

	m_listctrl_IO_Input.DeleteAllItems();
	m_listctrl_IO_Output.DeleteAllItems();

	if (m_Img_list_slct != NULL)
		delete m_Img_list_slct;

	m_Img_list_slct = new CImageList;
	m_Img_list_slct->Create(16, 16, ILC_COLOR4, 3, 3);

	BOOL bRslt = bitmap.LoadBitmap(IDB_BITMAP_IO_OFF);
	m_Img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	bRslt = bitmap.LoadBitmap(IDB_BITMAP_IO_ON);
	m_Img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	m_listctrl_IO_Input.SetImageList(m_Img_list_slct, LVSIL_SMALL);
	m_listctrl_IO_Output.SetImageList(m_Img_list_slct, LVSIL_SMALL);

	lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;
	lvColumn.pszText = _T("번호");
	lvColumn.iSubItem = 0;
	lvColumn.cx = 40;
	m_listctrl_IO_Input.InsertColumn(0, &lvColumn);
	m_listctrl_IO_Output.InsertColumn(0, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER | LVCFMT_FIXED_WIDTH;
	lvColumn.pszText = _T("입력 번호");
	lvColumn.iSubItem = 0;
	lvColumn.cx = 70;
	m_listctrl_IO_Input.InsertColumn(1, &lvColumn);
	lvColumn.pszText = _T("출력 번호");
	m_listctrl_IO_Output.InsertColumn(1, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT | LVCFMT_FIXED_WIDTH;
	lvColumn.pszText = _T("설명");
	lvColumn.iSubItem = 0;
	lvColumn.cx = 200;
	m_listctrl_IO_Input.InsertColumn(2, &lvColumn);
	m_listctrl_IO_Output.InsertColumn(2, &lvColumn);

	m_listctrl_IO_Input.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listctrl_IO_Output.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CString strText;
	for (i = 0; i < MAX_INPUT_NUM; i++)
	{
		strText.Format(_T("%02d"), i);
		m_listctrl_IO_Input.InsertItem(i, strText);
		if(m_stInput[i].nIONum != -1)
			strText.Format(_T("X%03d"), m_stInput[i].nIONum);
		else
			strText.Format(_T("사용 안함"));
		m_listctrl_IO_Input.SetItemText(i, 1, strText);
		m_listctrl_IO_Input.SetItemText(i, 2, m_stInput[i].strIOLabel);

		LV_ITEM lvitem;
		strText.Format(_T("%02d"), i);
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem = i;
		lvitem.iSubItem = 0;
		lvitem.iImage = 0;
		lvitem.pszText = (LPWSTR)(LPCWSTR)strText;
		m_listctrl_IO_Input.SetItem(&lvitem);
	}

	for (i = 0; i < MAX_OUTPUT_NUM; i++)
	{
		strText.Format(_T("%02d"), i);
		m_listctrl_IO_Output.InsertItem(i, strText);
		if (m_stOutput[i].nIONum != -1)
			strText.Format(_T("Y%03d"), m_stOutput[i].nIONum);
		else
			strText.Format(_T("사용 안함"));
		m_listctrl_IO_Output.SetItemText(i, 1, strText);
		m_listctrl_IO_Output.SetItemText(i, 2, m_stOutput[i].strIOLabel);

		LV_ITEM lvitem;
		strText.Format(_T("%02d"), i);
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem = i;
		lvitem.iSubItem = 0;
		lvitem.iImage = 0;
		lvitem.pszText = (LPWSTR)(LPCWSTR)strText;
		m_listctrl_IO_Output.SetItem(&lvitem);
	}

	//delete img_list_slct;
}

BOOL CDialogManual::LoadIOLabelPara()
{
	CString key, dat;
	CString file_path, pat_path, extention, master_path, cur_path;
	wchar_t strTemp[MAX_CHAR_LENG];
	GetCurrentDirectory(MAX_CHAR_LENG, strTemp);

	file_path.Format(_T("%s\\System\\%s"), strTemp, IO_STR_PARA_FILENAME);

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(file_path);
	CString strNode, strKey, strTempLabel;

	strNode.Format(_T("DIGITAL_INPUT_LABEL"));
	for (int i = 0; i < MAX_INPUT_NUM; i++)
	{
		strKey.Format(_T("%02d"), i);
		strTempLabel = cDataIf.GetString(strNode, strKey, _T("-1,Empty"));
		int nIndex = strTempLabel.Find(_T(","));
		CString strIndex, strLabel;
		strIndex.Format(_T("%s"), strTempLabel.Mid(0, nIndex));
		strLabel.Format(_T("%s"), strTempLabel.Mid(nIndex + 1, strTempLabel.GetLength()));
		if (_wtoi(strIndex) == -1)
			continue;

		m_stInput[i].nIONum = _wtoi(strIndex);
		memcpy(m_stInput[i].strIOLabel, strLabel.GetBuffer(), sizeof(wchar_t) * strLabel.GetLength() + 1);
	}

	strNode.Format(_T("DIGITAL_OUTPUT_LABEL"));
	for (int i = 0; i < MAX_INPUT_NUM; i++)
	{
		strKey.Format(_T("%02d"), i);
		strTempLabel = cDataIf.GetString(strNode, strKey, _T("-1,Empty"));
		int nIndex = strTempLabel.Find(_T(","));
		CString strIndex, strLabel;
		strIndex.Format(_T("%s"), strTempLabel.Mid(0, nIndex));
		strLabel.Format(_T("%s"), strTempLabel.Mid(nIndex + 1, strTempLabel.GetLength()));
		if (_wtoi(strIndex) == -1)
			continue;

		m_stOutput[i].nIONum = _wtoi(strIndex);
		memcpy(m_stOutput[i].strIOLabel, strLabel.GetBuffer(), sizeof(wchar_t) * strLabel.GetLength() + 1);
	}


	return TRUE;
}

BOOL CDialogManual::SaveIOLabelPara()
{


	return TRUE;
}

void CDialogManual::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	X_RAY_INFO info;
	switch ((int)nIDEvent)
	{
	case 0:
		if (this->IsWindowVisible())
		{
			//for (int i = 0; i < MAX_INPUT_NUM; i++)
			//{
			//	if(m_stInput[i].nIONum != -1)
			//		m_bInputState[i] = CIOClass::Instance()->GetInputBit(m_stInput[i].nIONum);
			//}
			UpdateInputDisplay();
		}
		break;
	case 1:
		if (this->IsWindowVisible())
		{
			//for (int i = 0; i < MAX_OUTPUT_NUM; i++)
			//{
			//	if (m_stOutput[i].nIONum != -1)
			//		m_bOutputState[i] = CIOClass::Instance()->GetOutputBit(m_stOutput[i].nIONum);
			//}
			UpdateOutputDisplay();
		}
		break;
	case 200:
		info = CXRayClass::Instance()->GetXrayInfo();
		double dKv, dMa, dSetKv, dSetMa;
		dKv = info.nNoScalingCurKV * info.dScalingCurKV;
		dMa = info.nNoScalingCurMA * info.dScalingCurMA;
		m_dlgXRayStatus->GetKVtoMA(&dSetKv, &dSetMa);
#ifdef HW_USE
		if (/*info.isOnOff &&*/
			((dSetKv - X_RAY_KV_OFFSET <= dKv) && (dKv <= dSetKv + X_RAY_KV_OFFSET)) &&
			((dSetMa - X_RAY_MA_OFFSET <= dMa) && (dMa <= dSetMa + X_RAY_MA_OFFSET)))
#endif
		{
			KillTimer(200);
			Sleep(1000);
			m_pCalThread = AfxBeginThread(ThreadCalStart, this, THREAD_PRIORITY_NORMAL);
			SetThreadAffinityMask(m_pCalThread, 1);
			if (m_pCalThread != NULL)
			{
				m_pCalThread->m_bAutoDelete = TRUE;
			}
		}

		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDialogManual::UpdateInputDisplay()
{
	int i;
	LV_ITEM lvitem;

	CString str;
	BOOL in;

	for (i = 0; i < MAX_INPUT_NUM; i++)
	{
		str.Format(_T("%02d"), i);

		//IO reading
		//in = CIOPort::Instance()->GetBit(i);
		if (m_stInput[i].nIONum != -1)
		{
			in = CIOClass::Instance()->GetInputBit(m_stInput[i].nIONum);
#ifdef CE_CONFIRM
			if (i == 0 || i == 1)
			{
				in = !in;
			}
#endif
		}
		else
			continue;

		if (in)
		{
			if ((m_bListInputState[i] != in))
			{
				m_listctrl_IO_Input.GetItem(&lvitem);
				lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
				lvitem.iItem = i;
				lvitem.iSubItem = 0;
				lvitem.iImage = 1;
				lvitem.pszText = (LPWSTR)(LPCWSTR)str;
				m_listctrl_IO_Input.SetItem(&lvitem);
				m_bListInputState[i] = in;
			}
			m_bInputState[i] = in;
		}
		else
		{
			if ((m_bListInputState[i] != in))
			{
				lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
				lvitem.iItem = i;
				lvitem.iSubItem = 0;
				lvitem.iImage = 0;
				lvitem.pszText = (LPWSTR)(LPCWSTR)str;
				m_listctrl_IO_Input.SetItem(&lvitem);
				m_bListInputState[i] = in;
			}
			m_bInputState[i] = in;
		}
	}
}

void CDialogManual::UpdateOutputDisplay()
{
	int i;
	LV_ITEM lvitem;

	CString str;
	BOOL in;

	for (i = 0; i < MAX_OUTPUT_NUM; i++)
	{
		str.Format(_T("%02d"), i);

		//IO reading
		//in = CIOPort::Instance()->GetBit(i);
		if (m_stOutput[i].nIONum != -1)
			in = CIOClass::Instance()->GetOutputBit(m_stOutput[i].nIONum);
		else
			continue;

		if (in)
		{
			if ((m_bListOutputState[i] != in))
			{
				lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
				lvitem.iItem = i;
				lvitem.iSubItem = 0;
				lvitem.iImage = 1;
				lvitem.pszText = (LPWSTR)(LPCWSTR)str;
				m_listctrl_IO_Output.SetItem(&lvitem);
				m_bListOutputState[i] = in;
			}
			m_bOutputState[i] = in;
		}
		else
		{
			if ((m_bListOutputState[i] != in))
			{
				lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
				lvitem.iItem = i;
				lvitem.iSubItem = 0;
				lvitem.iImage = 0;
				lvitem.pszText = (LPWSTR)(LPCWSTR)str;
				m_listctrl_IO_Output.SetItem(&lvitem);
				m_bListOutputState[i] = in;
			}
			m_bOutputState[i] = in;
		}
	}
}

void CDialogManual::OnNMClickListInput(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int idx = pNMListView->iItem;
	if(m_bOutputState[idx])
		CIOClass::Instance()->SetOutputBit(m_stOutput[idx].nIONum, FALSE);
	else
		CIOClass::Instance()->SetOutputBit(m_stOutput[idx].nIONum, TRUE);
	*pResult = 0;
}


void CDialogManual::OnBnClickedButtonXRayWarmup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, TRUE);
	Delay(1000);
	CDialogXRayWarmup dlg;
	dlg.SetXrayEndTime(m_cXrayEndTime);
	if (dlg.DoModal() == 1)
	{
		callEventfunc(0);
	}
}


BOOL CDialogManual::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F12)
			OnBnClickedButtonClose();
		else if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogManual::OnBnClickedButtonDetectorCal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CAtDetector::Instance()->SetCal();
	CString strText;
	double dKV, dMA;
	m_bCalEnd = FALSE;
	m_dlgXRayStatus->GetKVtoMA(&dKV, &dMA);
	if (CXRayClass::Instance()->SetXrayVolCur(dKV, dMA))
	{
		CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, TRUE);
		Delay(40);
		CXRayClass::Instance()->SetXrayOnOff(TRUE);

		SetTimer(200, 100, NULL);
	}
}

void CDialogManual::SetXRayValue(double dKv, double dMa)
{
	m_dlgXRayStatus->SettingKVtoMA(dKv, dMa);
}

UINT CDialogManual::ThreadCalStart(LPVOID pParam)
{
	CDialogManual* pdlg = (CDialogManual*)pParam;

	pdlg->CalStartThread();

	return 0;
}

void CDialogManual::CalStartThread()
{
	X_RAY_INFO info;
	info = CXRayClass::Instance()->GetXrayInfo();
	double dKv, dMa, dSetKv, dSetMa;
	dKv = info.nNoScalingCurKV * info.dScalingCurKV;
	dMa = info.nNoScalingCurMA * info.dScalingCurMA;
	m_dlgXRayStatus->GetKVtoMA(&dSetKv, &dSetMa);
#ifdef HW_USE
	if (/*info.isOnOff &&*/
		((dSetKv - X_RAY_KV_OFFSET <= dKv) && (dKv <= dSetKv + X_RAY_KV_OFFSET)) &&
		((dSetMa - X_RAY_MA_OFFSET <= dMa) && (dMa <= dSetMa + X_RAY_MA_OFFSET)))
#endif
	{
		if (CAtDetector::Instance()->SetCal(m_dScanSpeed, m_nLineToAvg))
			m_bCalEnd = TRUE;

	}

	CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);
}