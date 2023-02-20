// StatusPanelDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "XNPI.h"
#include "StatusPanelDlg.h"
#include "afxdialogex.h"
#include "DetailInfoDlg.h"
#include "Status.h"


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
END_MESSAGE_MAP()


// CStatusPanelDlg �޽��� ó�����Դϴ�.


BOOL CStatusPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CRect DetailInfoRect;
	GetClientRect(&DetailInfoRect);
	m_pDetailInfoDlg = new CDetailInfoDlg(this);
	m_pDetailInfoDlg->Create(CDetailInfoDlg::IDD, this);
	m_pDetailInfoDlg->GetClientRect(&DetailInfoRect);
	m_pDetailInfoDlg->ShowWindow(SW_SHOW);
	SetTimer(TIMEREVENT_DISPMOUSEPOSINFO, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CStatusPanelDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nIDEvent)
	{
	case TIMEREVENT_DISPMOUSEPOSINFO:
		CStatus::Instance()->m_infoCamMousePos;
		SetDlgItemInt(IDC_DISPCAMNO, CStatus::Instance()->m_infoCamMousePos.nCamNo);
		SetDlgItemInt(IDC_DISPXPOS, (int)(CStatus::Instance()->m_infoCamMousePos.posImage.dblPosX));
		SetDlgItemInt(IDC_DISPYPOS, (int)(CStatus::Instance()->m_infoCamMousePos.posImage.dblPosY));
		SetDlgItemInt(IDC_DISPVALUE, CStatus::Instance()->m_infoCamMousePos.GrayValue);
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
