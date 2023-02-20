// CDialogTextBox.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogTextBox.h"


// CDialogTextBox 대화 상자

IMPLEMENT_DYNAMIC(CDialogTextBox, CDialogEx)

CDialogTextBox::CDialogTextBox(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MAIN_TEXT_BOX, pParent)
{
	m_bHide = FALSE;
}

CDialogTextBox::~CDialogTextBox()
{
}

void CDialogTextBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogTextBox, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogTextBox 메시지 처리기


BOOL CDialogTextBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDialogTextBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogTextBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		SetDlgItemText(IDC_BTNENHCTRL_TEXT_BOX, m_strTextBox);
	}
	else
	{
		KillTimer(0);
		this->ShowWindow(SW_HIDE);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogTextBox::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch ((int)nIDEvent)
	{
	case 0:
		if (m_bHide)
		{
			KillTimer(0);
			this->ShowWindow(SW_HIDE);
		}
		else
		{
			if (!this->IsWindowVisible())
			{
				this->ShowWindow(SW_SHOW);
			}
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDialogTextBox::SetTextBoxString(CString strText)
{
	m_strTextBox.Format(_T("%s"), strText);
	SetDlgItemText(IDC_BTNENHCTRL_TEXT_BOX, m_strTextBox);
}

void CDialogTextBox::SetTimerShowWindow(CString strText, int nMs)
{
	m_bHide = FALSE;
	SetTextBoxString(strText);
	if(nMs > 0)
		SetTimer(0, nMs, NULL);
}

void CDialogTextBox::SetTimerShowHide(BOOL isHide)
{
	m_bHide = isHide;
}