// CDialgInterLockStatus.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialgInterLockStatus.h"
#include "InspectionDlg.h"

// CDialgInterLockStatus 대화 상자

IMPLEMENT_DYNAMIC(CDialgInterLockStatus, CDialogEx)

CDialgInterLockStatus::CDialgInterLockStatus(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INTER_LOCK_STATUS, pParent)
{

}

CDialgInterLockStatus::~CDialgInterLockStatus()
{
}

void CDialgInterLockStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	int i = 0;
	DDX_Control(pDX, IDC_BTNENHCTRL_INTER_LOCK_0, m_btnInterLockStatus[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_INTER_LOCK_1, m_btnInterLockStatus[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_INTER_LOCK_2, m_btnInterLockStatus[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_INTER_LOCK_3, m_btnInterLockStatus[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_INTER_LOCK_4, m_btnInterLockStatus[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_INTER_LOCK_5, m_btnInterLockStatus[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_INTER_LOCK_6, m_btnInterLockStatus[i++]);
	DDX_Control(pDX, IDC_BTNENHCTRL_INTER_LOCK_7, m_btnInterLockStatus[i++]);
}
;

BEGIN_MESSAGE_MAP(CDialgInterLockStatus, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDialgInterLockStatus, CDialogEx)
END_EVENTSINK_MAP()

// CDialgInterLockStatus 메시지 처리기


BOOL CDialgInterLockStatus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

#ifdef CE_CONFIRM
	m_btnInterLockStatus[7].ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTNENHCTRL_INTER_LOCK_LABEL_7)->ShowWindow(SW_HIDE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialgInterLockStatus::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		for (int i = 0; i < 8; i++)
			m_btnInterLockStatus[i].SetBackColor(RGB(0, 255, 0));

		SetTimer(0, 100, NULL);
	}
	else
	{
		KillTimer(0);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CDialgInterLockStatus::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialgInterLockStatus::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	BOOL bInterLockStatus = FALSE;
	switch ((int)nIDEvent)
	{
	case 0:
		if (this->IsWindowVisible())
		{
			for (int i = 0; i < 8; i++)
			{
#ifdef CE_CONFIRM
				if (i == 7)
					continue;
#endif
				bInterLockStatus = GetInterLockStatus(i);

				if (m_bInterLockStatus[i] != bInterLockStatus)
				{
					m_bInterLockStatus[i] = bInterLockStatus;
					if (!m_bInterLockStatus[i])
					{
						m_btnInterLockStatus[i].SetBackColor(RGB(255, 0, 0));
					}
					else
					{
						m_btnInterLockStatus[i].SetBackColor(RGB(0, 255, 0));
					}
				}
			}
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CDialgInterLockStatus::GetInterLockStatus(int nIndex)
{
	BOOL bRslt = FALSE;
	CInspectionDlg* pDlgMain = (CInspectionDlg*)AfxGetApp()->m_pMainWnd;

	switch (nIndex)
	{
	case 0:
		bRslt = pDlgMain->GetEMG();
		break;
	case 1:
		bRslt = pDlgMain->GetKeySw();

		break;
	case 2:
		bRslt = pDlgMain->GetDoor();

		break;
	case 3:
		bRslt = !pDlgMain->GetInverterAlarm();

		break;

	case 4:
		bRslt = !pDlgMain->GetXRayInterLock();

		break;
	case 5:
		bRslt = !pDlgMain->GetXRayLampALM();
		break;
	case 6:
		bRslt = pDlgMain->GetXRayWarmupAlaram();
		break;
	case 7:
		bRslt = pDlgMain->GetMC01OnAlarm();
		break;
	}


	return bRslt;
}