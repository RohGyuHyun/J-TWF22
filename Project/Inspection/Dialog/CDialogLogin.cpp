// CDialogLogin.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogLogin.h"
#include "GetTextDlg.h"
#include "GetNumDlg.h"

// CDialogLogin 대화 상자

IMPLEMENT_DYNAMIC(CDialogLogin, CDialogEx)

CDialogLogin::CDialogLogin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
{

}

CDialogLogin::~CDialogLogin()
{
}

void CDialogLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogLogin, CDialogEx)
END_MESSAGE_MAP()


// CDialogLogin 메시지 처리기
BEGIN_EVENTSINK_MAP(CDialogLogin, CDialogEx)
	ON_EVENT(CDialogLogin, IDC_BTNENHCTRL_PW, DISPID_CLICK, CDialogLogin::ClickBtnenhctrlPw, VTS_NONE)
	ON_EVENT(CDialogLogin, IDC_BTNENHCTRL_USER_LOGIN, DISPID_CLICK, CDialogLogin::ClickBtnenhctrlUserLogin, VTS_NONE)
	ON_EVENT(CDialogLogin, IDC_BTNENHCTRL_ADMIN_LOGIN, DISPID_CLICK, CDialogLogin::ClickBtnenhctrlAdminLogin, VTS_NONE)
	ON_EVENT(CDialogLogin, IDC_BTNENHCTRL_EXIT, DISPID_CLICK, CDialogLogin::ClickBtnenhctrlExit, VTS_NONE)
	ON_EVENT(CDialogLogin, IDC_BTNENHCTRL_ADMIN_LOGIN2, DISPID_CLICK, CDialogLogin::ClickBtnenhctrlAdminLogin2, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogLogin::ClickBtnenhctrlPw()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnenhctrl* pBtnEnh = (CBtnenhctrl*)GetFocus();
	CGetTextDlg	dlg(30, _T(""), _T("Admin Logion PW Input"), this, TRUE);

	if (dlg.DoModal() == IDCANCEL) return;
	CString strText = dlg.GetStringValue();
	CString strPW;
	for (int i = 0; i < strText.GetLength(); i++)
		strPW.AppendFormat(_T("*"));

	pBtnEnh->SetWindowTextW(strPW);
	m_strPW.Format(_T("%s"), strText);
}


void CDialogLogin::ClickBtnenhctrlUserLogin()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CGetTextDlg	dlg(30, _T(""), _T("User Logion PW Input"), this, TRUE);

	if (dlg.DoModal() == IDCANCEL) return;
	CString strText = dlg.GetStringValue();

	if (strText.Compare(USER_PW_STRING))
	{
		AfxMessageBox(_T("Not PW"));
		return;
	}
	EndDialog(1);
}


void CDialogLogin::ClickBtnenhctrlAdminLogin()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CGetTextDlg	dlg(30, _T(""), _T("Manufacture Logion PW Input"), this, TRUE);

	if (dlg.DoModal() == IDCANCEL) return;
	CString strText = dlg.GetStringValue();

	if (strText.Compare(MANUFACTURE_PW_STRING))
	{
		AfxMessageBox(_T("Not PW"));
		return;
	}

	EndDialog(2);
}


void CDialogLogin::ClickBtnenhctrlAdminLogin2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CGetTextDlg	dlg(30, _T(""), _T("Admin Logion PW Input"), this, TRUE);

	if (dlg.DoModal() == IDCANCEL) return;
	CString strText = dlg.GetStringValue();

	if (strText.Compare(ADMIN_PW_STRING))
	{
		AfxMessageBox(_T("Not PW"));
		return;
	}

	EndDialog(3);
}



void CDialogLogin::ClickBtnenhctrlExit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	EndDialog(0);
}


BOOL CDialogLogin::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F5)
			ClickBtnenhctrlUserLogin();
		else if (pMsg->wParam == VK_F6)
			ClickBtnenhctrlAdminLogin();
		else if (pMsg->wParam == VK_F7)
			ClickBtnenhctrlAdminLogin2();
		else if (pMsg->wParam == VK_F12)
			ClickBtnenhctrlExit();
		else if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

