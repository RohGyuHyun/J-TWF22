// StarMessageWnd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "XNPI.h"
#include "MessageWnd.h"
#include "Status.h"


// CMessageWnd ��ȭ �����Դϴ�.

CMessageWnd * CMessageWnd::m_pInstance = NULL;

IMPLEMENT_DYNAMIC(CMessageWnd, CDialog)

CMessageWnd::CMessageWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageWnd::IDD, pParent)
	, m_StarMessageLabelText(_T(""))
	, m_iTextID(0)
{

}

CMessageWnd::~CMessageWnd()
{
}

void CMessageWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MESSAGETEXT, m_StarMessageLabel);
	DDX_Text(pDX, IDC_STATIC_MESSAGETEXT, m_StarMessageLabelText);
	DDX_Control(pDX, IDC_BUTTON_EXITSTAR, m_StarPrjExitButton);
}


BEGIN_MESSAGE_MAP(CMessageWnd, CDialog)
	ON_BN_CLICKED(IDOK, &CMessageWnd::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL, &CMessageWnd::OnBnClickedCancel)
	ON_BN_CLICKED(ID_OKMESSAGEBT, &CMessageWnd::OnBnClickedOkmessagebt)
	ON_BN_CLICKED(ID_CANCEL_MESSAGEBT, &CMessageWnd::OnBnClickedCancelMessagebt)
	ON_BN_CLICKED(IDC_BUTTON_EXITSTAR, &CMessageWnd::OnBnClickedButtonExitstar)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CMessageWnd �޽��� ó�����Դϴ�.

void CMessageWnd::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bSelectWnd = TRUE;
	OnOK();

}


void CMessageWnd::SetMessageText(CString str)
{
	m_StarMessageLabelText=str;
	UpdateData(false);

}

void CMessageWnd::UIIint()
{
	/// �ý�Ʈâ��UI �����κ�

	 m_StarMessageLabel.SetBkColor(RGB(0,0,0))
		 .SetTextColor(RGB(0,255,0))
		.FlashText(TRUE)
		.SetFontName(_T("Arial"))
		.SetFontSize(25);
	    
	 //////////////////////////////////////
	m_StarPrjExitButton.ShowWindow(false);
}
BOOL CMessageWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	UIIint();
	CStatus::Instance()->m_MsgWndOpen=TRUE;
	CRect RectStarMessage;
	GetClientRect(&RectStarMessage);
	MoveWindow(700,500,RectStarMessage.Width(),RectStarMessage.Height(),true);
	SetTimer(0,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CMessageWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int OldMessage=100;

	//int nEMGStage = -1;
	//if (CSCInspIO::Instance()->GetEMG(&nEMGStage))
	//{
	//	m_iTextID = 20;
	//}

	switch(m_iTextID)
	{
	case 20:
		//if(!(OldMessage==m_iTextID))
		//{
		//	CString strMsg;
		//	strMsg.Format(_T("EmerGency Stop  Please Check Stage No.%d"), nEMGStage);
		//	SetMessageText(strMsg);
		//}

		//OldMessage = 	m_iTextID;
		break;

	case 22:
		if(!(OldMessage==m_iTextID))
		{SetMessageText(_T("Please Error"));}

		OldMessage = 	m_iTextID;
		m_StarPrjExitButton.ShowWindow(true);

		break;
	case 99:
		OldMessage = 	m_iTextID;
		OnBnClickedOk();
		break;
	default:
		OldMessage = 	m_iTextID;
		if(!(OldMessage==m_iTextID))
			SetMessageText(_T("TEST TEST TEST TEST TEST "));
		break;

	}
	CDialog::OnTimer(nIDEvent);
}

void CMessageWnd::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CStatus::Instance()->m_MsgWndOpen=false;
    OnCancel();
}

void CMessageWnd::OnDestroy()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CStatus::Instance()->m_MsgWndOpen = FALSE;
}

void CMessageWnd::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bSelectWnd = TRUE;
	OnCancel();
}

void CMessageWnd::OnBnClickedOkmessagebt()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CMessageWnd::OnBnClickedCancelMessagebt()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bSelectWnd = TRUE;
	OnCancel();
}

void CMessageWnd::OnBnClickedButtonExitstar()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	CMainDlg::Instance()->StopEMS_Button_ThreadOn();
	OnClose();
//	pGetMainDlg->OnClose();
	
}


CMessageWnd * CMessageWnd::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CMessageWnd;
		m_pInstance->Create(IDD_MESSAGE_WND, NULL);
	}
	return m_pInstance;
}


void CMessageWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_bSelectWnd = FALSE;
}
