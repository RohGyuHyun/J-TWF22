// AVMDScrollDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "AVMDScrollDlg.h"
#include "afxdialogex.h"
#include "Status.h"

// CAVMDScrollDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAVMDScrollDlg, CDialogEx)

CAVMDScrollDlg::CAVMDScrollDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SCROLL, pParent)
	, m_iScrValue(0)
	, m_iStandardValue(0)
	, m_strGraphType(_T(""))
	, m_nInputType(0)
{
	
}

CAVMDScrollDlg::CAVMDScrollDlg(CString strType, int nInputType /*=NULL*/)
	: m_iScrValue(0)
	, m_iStandardValue(0)
	, m_strGraphType(_T(""))
	, m_nInputType(0)
{
	m_strGraphType = strType;
	m_nInputType = nInputType;
}

CAVMDScrollDlg::~CAVMDScrollDlg()
{
}

void CAVMDScrollDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR, m_ctrlScrollValue);
	DDX_Scroll(pDX, IDC_SCROLLBAR, m_iScrValue);
}


BEGIN_MESSAGE_MAP(CAVMDScrollDlg, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAVMDScrollDlg �޽��� ó�����Դϴ�.

BOOL CAVMDScrollDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetSrollInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CAVMDScrollDlg::SetSrollInfo()
{
	m_scrInfo.cbSize = sizeof(m_scrInfo);
	m_scrInfo.fMask = SIF_ALL;

	if (CStatus::Instance()->m_strTimeDomainUnit == "g-Force")
	{
		m_scrInfo.nMin = (int)-SCROLL_RATIO;
		m_scrInfo.nMax = (int)SCROLL_RATIO;
		m_scrInfo.nPage = 1;
	}
	if (CStatus::Instance()->m_strTimeDomainUnit == "GAL")
	{
		m_scrInfo.nMin = (int)-SCROLL_RATIO;
		m_scrInfo.nMax = (int)SCROLL_RATIO;
		m_scrInfo.nPage = 1;
	}
	
	m_scrInfo.nTrackPos = m_iStandardValue;
	m_scrInfo.nPos = 0;
	m_ctrlScrollValue.SetScrollInfo(&m_scrInfo);

	if (m_nInputType == 0)
	{
		if (m_strGraphType == "RMS")
			CStatus::Instance()->m_iRMSStandardValue = m_iStandardValue;
		else if (m_strGraphType == 'X')
			CStatus::Instance()->m_iXStandardValue = m_iStandardValue;
		else if (m_strGraphType == 'Y')
			CStatus::Instance()->m_iYStandardValue = m_iStandardValue;
		else if (m_strGraphType == 'Z')
			CStatus::Instance()->m_iZStandardValue = m_iStandardValue;
	}
	if (m_nInputType == 1)
	{
		CStatus::Instance()->m_iCompareStandardValue = m_iStandardValue;
	}
	
}

void CAVMDScrollDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	int nSize = CStatus::Instance()->GetRepository()->GetDataCount(0);
	if (nSize == 0)
		return;

	if (pScrollBar)
	{
		if (pScrollBar == (CScrollBar*)&m_ctrlScrollValue)
		{
			SCROLLINFO scrInfo;
			if (pScrollBar->GetScrollInfo(&scrInfo))
			{
				switch (nSBCode)
				{
				case SB_LINEUP:
					scrInfo.nPos -= scrInfo.nPage;
					break;
				case SB_LINEDOWN:
					scrInfo.nPos += scrInfo.nPage;
					break;
				case SB_PAGEUP:
					scrInfo.nPos -= scrInfo.nPage;
					break;
				case SB_PAGEDOWN:
					scrInfo.nPos += scrInfo.nPage;
					break;
				case SB_THUMBPOSITION:
				case SB_THUMBTRACK:
					scrInfo.nPos = scrInfo.nTrackPos;
					break;
				}
				pScrollBar->SetScrollPos(scrInfo.nPos);
				m_iStandardValue = (-1) * scrInfo.nPos;
			
				if (m_iStandardValue > m_scrInfo.nMax)
					m_iStandardValue = m_scrInfo.nMax;
				if (m_iStandardValue < m_scrInfo.nMin)
					m_iStandardValue = m_scrInfo.nMin;
				
				if (m_nInputType == 0)
				{
					if (m_strGraphType == "RMS")
						CStatus::Instance()->m_iRMSStandardValue = m_iStandardValue;
					else if (m_strGraphType == 'X')
						CStatus::Instance()->m_iXStandardValue = m_iStandardValue;
					else if (m_strGraphType == 'Y')
						CStatus::Instance()->m_iYStandardValue = m_iStandardValue;
					else if (m_strGraphType == 'Z')
						CStatus::Instance()->m_iZStandardValue = m_iStandardValue;
				}
				if (m_nInputType == 1)
				{
					CStatus::Instance()->m_iCompareStandardValue = m_iStandardValue;
				}
			}
		}
	}
	CStatus::Instance()->UpdateScrn();
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CAVMDScrollDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CRect RectClient;
	GetClientRect(&RectClient);
	if (RectClient.IsRectNull())
		return;
	if (!m_ctrlScrollValue)
		return;

	m_ctrlScrollValue.MoveWindow(RectClient.left+5, RectClient.top, RectClient.Width() - 5, RectClient.Height()-10, 1);
	m_ctrlScrollValue.Invalidate();
}
