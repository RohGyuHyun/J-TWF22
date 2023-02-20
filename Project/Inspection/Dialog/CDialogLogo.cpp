// CDialogLogo.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogLogo.h"


// CDialogLogo 대화 상자

IMPLEMENT_DYNAMIC(CDialogLogo, CDialogEx)

CDialogLogo::CDialogLogo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FORMVIEW_LOGO, pParent)
{
	m_nTimer = 0;
}

CDialogLogo::~CDialogLogo()
{
}

void CDialogLogo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogLogo, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogLogo 메시지 처리기


BOOL CDialogLogo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDialogLogo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogLogo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch ((int)nIDEvent)
	{
	case 0:
		if (this->IsWindowVisible())
		{
			m_nTimer++;

			if (m_nTimer > 20)
			{
				KillTimer(0);
				EndDialog(0);
			}
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDialogLogo::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		SetTimer(0, 100, NULL);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
