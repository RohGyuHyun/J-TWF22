// CDialogXRayStatus.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogXRayStatus.h"
#include "CATDetector.h"
#include "CIOClass.h"
#include "GetNumDlg.h"

// CDialogXRayStatus 대화 상자

IMPLEMENT_DYNAMIC(CDialogXRayStatus, CDialogEx)

CDialogXRayStatus::CDialogXRayStatus(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_X_RAY_STATUS, pParent)
{
	//m_XRayClass = NULL;
	memset(&m_stXray_status, NULL, sizeof(X_RAY_STATUS));
	m_bNumPd = FALSE;
}

CDialogXRayStatus::~CDialogXRayStatus()
{
	
}

void CDialogXRayStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogXRayStatus, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_X_RAY_RESET, &CDialogXRayStatus::OnBnClickedButtonXRayReset)
	ON_BN_CLICKED(IDC_BUTTON_X_RAY_ON, &CDialogXRayStatus::OnBnClickedButtonXRayOn)
	ON_BN_CLICKED(IDC_BUTTON_X_RAY_OFF, &CDialogXRayStatus::OnBnClickedButtonXRayOff)
	ON_EN_SETFOCUS(IDC_EDIT_X_RAY_PARA_2, &CDialogXRayStatus::OnEnSetfocusEditXRayPara2)
	ON_EN_SETFOCUS(IDC_EDIT_X_RAY_PARA_3, &CDialogXRayStatus::OnEnSetfocusEditXRayPara3)
	ON_EN_KILLFOCUS(IDC_EDIT_X_RAY_PARA_2, &CDialogXRayStatus::OnEnKillfocusEditXRayPara2)
	ON_EN_KILLFOCUS(IDC_EDIT_X_RAY_PARA_3, &CDialogXRayStatus::OnEnKillfocusEditXRayPara3)
END_MESSAGE_MAP()


// CDialogXRayStatus 메시지 처리기


BOOL CDialogXRayStatus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//CAtDetector::Instance();
	CBtnenhctrl* pBtnEnh;
	pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_0);
	SetXrayStatus(pBtnEnh, FALSE);
	pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_1);
	SetXrayStatus(pBtnEnh, FALSE);
	pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_2);
	SetXrayStatus(pBtnEnh, FALSE);
	pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_3);
	SetXrayStatus(pBtnEnh, FALSE);
	pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_4);
	SetXrayStatus(pBtnEnh, FALSE);
	pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_5);
	SetXrayStatus(pBtnEnh, FALSE);
	pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_6);
	SetXrayStatus(pBtnEnh, FALSE);
	pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_7);
	SetXrayStatus(pBtnEnh, FALSE);

	SetTimer(0, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogXRayStatus::SetXrayStatus(CBtnenhctrl* pBtnEnh, BOOL isOnOff)
{
	if (isOnOff)
	{
		pBtnEnh->SetBackColor(RGB(255, 0, 0));
	}
	else
	{
		pBtnEnh->SetBackColor(RGB(0, 255, 0));
	}
}

void CDialogXRayStatus::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch ((int)nIDEvent)
	{
	case 0:
		if (this->IsWindowVisible())
		{
			X_RAY_INFO info;
			info = CXRayClass::Instance()->GetXrayInfo();

			CBtnenhctrl* pBtnEnh;
			pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_0);
			if (m_stXray_status.bArc != info.stXray_status.bArc)
				SetXrayStatus(pBtnEnh, info.stXray_status.bArc);
			pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_1);
			if (m_stXray_status.bOverTemperature != info.stXray_status.bOverTemperature)
				SetXrayStatus(pBtnEnh, info.stXray_status.bOverTemperature);
			pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_2);
			if (m_stXray_status.bOverVoltage != info.stXray_status.bOverVoltage)
				SetXrayStatus(pBtnEnh, info.stXray_status.bOverVoltage);
			pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_3);
			if (m_stXray_status.bUnderVoltage != info.stXray_status.bUnderVoltage)
				SetXrayStatus(pBtnEnh, info.stXray_status.bUnderVoltage);
			pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_4);
			if (m_stXray_status.bOverCurrent != info.stXray_status.bOverCurrent)
				SetXrayStatus(pBtnEnh, info.stXray_status.bOverCurrent);
			pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_5);
			if (m_stXray_status.bUnderCurrent != info.stXray_status.bUnderCurrent)
				SetXrayStatus(pBtnEnh, info.stXray_status.bUnderCurrent);
			pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_6);
			if (m_stXray_status.bWatchdogTimeOut != info.stXray_status.bWatchdogTimeOut)
				SetXrayStatus(pBtnEnh, info.stXray_status.bWatchdogTimeOut);
			pBtnEnh = (CBtnenhctrl*)GetDlgItem(IDC_BTNENHCTRL_STATUS_7);
			if (m_stXray_status.bOpenInterlock != info.stXray_status.bOpenInterlock)
				SetXrayStatus(pBtnEnh, info.stXray_status.bOpenInterlock);


			CString strText;
			double dKv, dMa, dTemp;
			dKv = info.nNoScalingCurKV * info.dScalingCurKV;
			dMa = info.nNoScalingCurMA * info.dScalingCurMA;
			dTemp = info.dTemp;
			strText.Format(_T("%0.3f"), dKv);
			SetDlgItemText(IDC_EDIT_X_RAY_PARA_0, strText);
			strText.Format(_T("%0.3f"), dMa);
			SetDlgItemText(IDC_EDIT_X_RAY_PARA_1, strText);
			strText.Format(_T("%0.3f 'C"), dTemp);
			SetDlgItemText(IDC_EDIT_X_RAY_PARA_4, strText);

			m_stXray_status = info.stXray_status;
		}
		break;
	case 999:
		X_RAY_INFO info;
		info = CXRayClass::Instance()->GetXrayInfo();
		double dKv, dMa;
		dKv = info.nNoScalingCurKV * info.dScalingCurKV;
		dMa = info.nNoScalingCurMA * info.dScalingCurMA;
#ifdef HW_USE
		if (info.isOnOff &&
			((0. - X_RAY_KV_OFFSET <= dKv) && (dKv <= 0. + X_RAY_KV_OFFSET)) &&
			((0. - X_RAY_MA_OFFSET <= dMa) && (dMa <= 0. + X_RAY_MA_OFFSET)))
#endif
		{
			KillTimer(999);
			CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDialogXRayStatus::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		X_RAY_INFO info;
		info = CXRayClass::Instance()->GetXrayInfo();

		//CString strText;
		//strText.Format(_T("0.000 kV"));
		//SetDlgItemText(IDC_EDIT_X_RAY_PARA_2, strText);
		//strText.Format(_T("0.000 mA"));
		//SetDlgItemText(IDC_EDIT_X_RAY_PARA_3, strText);
		//strText.Format(_T("0.000 'C"));// , "'", "C");
		//SetDlgItemText(IDC_EDIT_X_RAY_PARA_4, strText);

		SetTimer(0, 10, NULL);
	}
	else
	{
		KillTimer(999);
		KillTimer(0);
		CXRayClass::Instance()->SetXrayOnOff(FALSE);
		Delay(200);
		CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDialogXRayStatus::SettingKVtoMA(double dKV, double dMA)
{
	CXRayClass::Instance()->SetXrayVolCur(dKV, dMA);
	CString strText;
	strText.Format(_T("%0.3f"), dKV);
	SetDlgItemText(IDC_EDIT_X_RAY_PARA_2, strText);
	strText.Format(_T("%0.3f"), dMA);
	SetDlgItemText(IDC_EDIT_X_RAY_PARA_3, strText);
}

void CDialogXRayStatus::GetKVtoMA(double* dKV, double *dMA)
{
	CString strText;
	GetDlgItemText(IDC_EDIT_X_RAY_PARA_2, strText);
	*dKV = _wtof(strText);
	GetDlgItemText(IDC_EDIT_X_RAY_PARA_3, strText);
	*dMA = _wtof(strText);
}

void CDialogXRayStatus::OnBnClickedButtonXRayReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CXRayClass::Instance()->SetXrayRestFault();
}


void CDialogXRayStatus::OnBnClickedButtonXRayOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);
	
	CString strText;
	double dKV, dMA;
	GetDlgItemText(IDC_EDIT_X_RAY_PARA_2, strText);
	dKV = _wtof(strText);
	GetDlgItemText(IDC_EDIT_X_RAY_PARA_3, strText);
	dMA = _wtof(strText);
	if (CXRayClass::Instance()->SetXrayVolCur(dKV, dMA))
	{
		CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, TRUE);

		if (CXRayClass::Instance()->GetXrayWarmupCheck())
		{
			CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);
			AfxMessageBox(_T("X-Ray Warmup Start Please !!"));
		}
		else
		{
			CXRayClass::Instance()->SetXrayOnOff(TRUE);
			callEventfunc(1);
		}
	}
}


void CDialogXRayStatus::OnBnClickedButtonXRayOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CXRayClass::Instance()->SetXrayVolCur(0., 0.);
	CXRayClass::Instance()->SetXrayOnOff(FALSE);
	callEventfunc(0);
	SetTimer(999, 100, NULL);
}


BOOL CDialogXRayStatus::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	else if (pMsg->message == WM_LBUTTONDOWN)
	{
		/*pMsg->pt.x;
		pMsg->pt.y;
		CRect rect1, rect2;
		GetDlgItem(IDC_EDIT_X_RAY_PARA_2)->GetWindowRect(&rect1);
		GetDlgItem(IDC_EDIT_X_RAY_PARA_3)->GetWindowRect(&rect2);

		if (rect1.PtInRect(pMsg->pt))
		{
			CString strText;
			CGetNumDlg dlg;

			GetDlgItem(IDC_EDIT_X_RAY_PARA_2)->GetWindowText(strText);
			double nVal = _wtof(strText);

			dlg.SetstrNum(nVal);

			if (dlg.DoModal() != IDOK)
				return TRUE;

			strText = dlg.GetstrNum();
			if (_wtoi(strText) < 0)
				return TRUE;

			GetDlgItem(IDC_EDIT_X_RAY_PARA_2)->SetWindowTextW(strText);
		}
		else if(rect2.PtInRect(pMsg->pt))
		{
			CString strText;
			CGetNumDlg dlg;

			GetDlgItem(IDC_EDIT_X_RAY_PARA_3)->GetWindowText(strText);
			double nVal = _wtof(strText);

			dlg.SetstrNum(nVal);

			if (dlg.DoModal() != IDOK)
				return TRUE;

			strText = dlg.GetstrNum();
			if (_wtoi(strText) < 0)
				return TRUE;

			GetDlgItem(IDC_EDIT_X_RAY_PARA_3)->SetWindowTextW(strText);
		}*/
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogXRayStatus::OnEnSetfocusEditXRayPara2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_BUTTON_X_RAY_ON)->SetFocus();
	CString strText;
	CGetNumDlg dlg;
		
	GetDlgItem(IDC_EDIT_X_RAY_PARA_2)->GetWindowText(strText);
	double nVal = _wtof(strText);

	dlg.SetstrNum(nVal);

	if (dlg.DoModal() != IDOK)
		return;

	strText = dlg.GetstrNum();
	if (_wtoi(strText) < 0)
		return;

	GetDlgItem(IDC_EDIT_X_RAY_PARA_2)->SetWindowTextW(strText);
	
}


void CDialogXRayStatus::OnEnSetfocusEditXRayPara3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_BUTTON_X_RAY_ON)->SetFocus();
	CString strText;
	GetDlgItem(IDC_EDIT_X_RAY_PARA_3)->GetWindowText(strText);
	double nVal = _wtof(strText);
	char cText[128];
	sprintf_s(cText, strText.GetLength() + 1, "%S", strText.GetBuffer());
	CGetNumDlg dlg(20, cText, "Insert Number Only.", this);
	dlg.SetstrNum(nVal);

	if (dlg.DoModal() != IDOK) 
		return;

	strText = dlg.GetstrNum();
	if (_wtoi(strText) < 0)
		return;

	GetDlgItem(IDC_EDIT_X_RAY_PARA_3)->SetWindowTextW(strText);
	
}


void CDialogXRayStatus::OnEnKillfocusEditXRayPara2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDialogXRayStatus::OnEnKillfocusEditXRayPara3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
