// StatusPanelDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "StatusPanelDlg.h"
#include "afxdialogex.h"
#include "DetailInfoDlg.h"
//#include "Status.h"


// CStatusPanelDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CStatusPanelDlg, CDialogEx)

CStatusPanelDlg::CStatusPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STSPANEL, pParent)
	, m_pDetailInfoDlg(NULL)
{

}

CStatusPanelDlg::~CStatusPanelDlg()
{
}

void CStatusPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStatusPanelDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CStatusPanelDlg �޽��� ó�����Դϴ�.


BOOL CStatusPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_pDetailInfoDlg = new CDetailInfoDlg(this);
	m_pDetailInfoDlg->Create(CDetailInfoDlg::IDD, this);
	m_pDetailInfoDlg->ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CStatusPanelDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nIDEvent)
	{
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CStatusPanelDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CStatusPanelDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	WndRePositioning();
}


void CStatusPanelDlg::WndRePositioning()
{
	if (!m_pDetailInfoDlg)return;
	CRect rectMain, DetailInfoRect;
	CWnd *pParent = GetParent();
	pParent->GetClientRect(rectMain);
	m_pDetailInfoDlg->GetClientRect(&DetailInfoRect);
	m_pDetailInfoDlg->MoveWindow(0, 0, rectMain.Width(), DetailInfoRect.Height());
}
