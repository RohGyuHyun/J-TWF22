// ConvTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CGInspector.h"
#include "ConvTestDlg.h"
#include "afxdialogex.h"
#include "ConveyorCtrl.h"
#include "UniIOPort.h"

// CConvTestDlg dialog

IMPLEMENT_DYNAMIC(CConvTestDlg, CDialogEx)

CConvTestDlg::CConvTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONVTEST, pParent)
	, m_bAuto(FALSE)
	, m_Dir(0)
{

}

CConvTestDlg::~CConvTestDlg()
{
}

void CConvTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHK_INPUT, m_ctrlInput);
	DDX_Control(pDX, IDC_CHK_OUTPUT, m_ctrlOutput);
}


BEGIN_MESSAGE_MAP(CConvTestDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_FWD, &CConvTestDlg::OnBnClickedBtnFwd)
	ON_BN_CLICKED(IDC_BTN_BWD, &CConvTestDlg::OnBnClickedBtnBwd)
	ON_BN_CLICKED(IDC_BTN_AUTO, &CConvTestDlg::OnBnClickedBtnAuto)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CConvTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CConvTestDlg message handlers


void CConvTestDlg::OnBnClickedBtnFwd()
{
	// TODO: Add your control notification handler code here
	CConveyorCtrl::Instance()->ConvGo(CConveyorCtrl::CONV_DIR_FWD);
}


void CConvTestDlg::OnBnClickedBtnBwd()
{
	// TODO: Add your control notification handler code here
	CConveyorCtrl::Instance()->ConvGo(CConveyorCtrl::CONV_DIR_BWD);
}


void CConvTestDlg::OnBnClickedBtnAuto()
{
	// TODO: Add your control notification handler code here
	if (!m_bAuto)
	{
		m_bAuto = !m_bAuto;
		SetTimer(2, 50, 0);
		CConveyorCtrl::Instance()->ConvGoVMode(CConveyorCtrl::CONV_DIR_FWD);
		m_Dir = CConveyorCtrl::CONV_DIR_FWD;
	}
	else
	{
		m_bAuto = !m_bAuto;
		KillTimer(2);
		CConveyorCtrl::Instance()->ConvGoVMode(CConveyorCtrl::CONV_DIR_STOP);
	}
}


void CConvTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case 1:
		if (CIOPort::Instance()->GetBit(1))
			m_ctrlInput.SetCheck(TRUE);
		else
			m_ctrlInput.SetCheck(FALSE);
		if (CIOPort::Instance()->GetBit(5))
			m_ctrlOutput.SetCheck(TRUE);
		else
			m_ctrlOutput.SetCheck(FALSE);
		break;
	case 2:
		if (CIOPort::Instance()->GetBit(1))
		{
			if (m_Dir == CConveyorCtrl::CONV_DIR_BWD)
			{
				CConveyorCtrl::Instance()->ConvGoVMode(CConveyorCtrl::CONV_DIR_STOP);
				Sleep(50);
				CConveyorCtrl::Instance()->ConvGoVMode(CConveyorCtrl::CONV_DIR_FWD);
				m_Dir = CConveyorCtrl::CONV_DIR_FWD;
			}
		}

		if (CIOPort::Instance()->GetBit(5))
		{
			if (m_Dir == CConveyorCtrl::CONV_DIR_FWD)
			{
				CConveyorCtrl::Instance()->ConvGoVMode(CConveyorCtrl::CONV_DIR_STOP);
				Sleep(50);
				CConveyorCtrl::Instance()->ConvGoVMode(CConveyorCtrl::CONV_DIR_BWD);
				m_Dir = CConveyorCtrl::CONV_DIR_BWD;
			}
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CConvTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SetTimer(1, 100, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CConvTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
